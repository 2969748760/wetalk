//
// Created by 29697 on 24-11-25.
//

#include "LogicSystem.h"

#include <json/reader.h>

#include "RedisMgr.h"
#include "StatusGrpcClient.h"
#include "UserMgr.h"

LogicSystem::~LogicSystem() {
    _b_stop = true;
    _consume.notify_one();
    _worker_thread.join();
}

LogicSystem::LogicSystem(): _b_stop(false) {
    RegisterCallBacks();
    _worker_thread = std::thread(&LogicSystem::DealMsg, this);
}

void LogicSystem::PostMsgToQue(std::shared_ptr<LogicNode> msg) {
    std::unique_lock<std::mutex> unique_lk(_mutex);
    _msg_que.push(msg);
    if (_msg_que.size() == 1) {
        unique_lk.unlock();
        _consume.notify_one();
    }
}

void LogicSystem::DealMsg() {
    for (;;) {
        std::unique_lock<std::mutex> unique_lk(_mutex);
        while (_msg_que.empty() && !_b_stop) {
            _consume.wait(unique_lk);
        }

        if (_b_stop) {
            while (!_msg_que.empty()) {
                auto msg_node = _msg_que.front();
                std::cout << "recv_msg id is " << msg_node->_recvnode->_msg_id << std::endl;
                auto call_back_iter = _fun_callbacks.find(msg_node->_recvnode->_msg_id);
                if (call_back_iter == _fun_callbacks.end()) {
                    std::cout << "msg_id is not register" << std::endl;
                    _msg_que.pop();
                    continue;
                }
                call_back_iter->second(msg_node->_session, msg_node->_recvnode->_msg_id,
                                       std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_cur_len));
                _msg_que.pop();
            }
            break;
        }

        auto msg_node = _msg_que.front();
        std::cout << "recv_msg id  is " << msg_node->_recvnode->_msg_id << std::endl;
        auto call_back_iter = _fun_callbacks.find(msg_node->_recvnode->_msg_id);
        if (call_back_iter == _fun_callbacks.end()) {
            _msg_que.pop();
            std::cout << "msg id [" << msg_node->_recvnode->_msg_id << "] handler not found" << std::endl;
            continue;
        }
        call_back_iter->second(msg_node->_session, msg_node->_recvnode->_msg_id,
                               std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_cur_len));
        _msg_que.pop();
    }
}

void LogicSystem::RegisterCallBacks() {
    _fun_callbacks[MSG_CHAT_LOGIN] = std::bind(&LogicSystem::LoginHandler, this, std::placeholders::_1,
                                               std::placeholders::_2, std::placeholders::_3);
}

void LogicSystem::LoginHandler(std::shared_ptr<CSession> session, const short &msg_id, const std::string &msg_data) {
    Json::Reader reader;
    Json::Value root;
    reader.parse(msg_data, root);
    auto uid = root["uid"].asInt();
    auto token = root["token"].asString();
    std::cout << "user login uid is  " << uid << " user token  is "
            << token << std::endl;
    auto rsp = StatusGrpcClient::GetInstance()->Login(uid, root["token"].asString());
    Json::Value rtvalue;
    Defer defer([this, &rtvalue, session]() {
        std::string return_str = rtvalue.toStyledString();
        session->Send(return_str, MSG_CHAT_LOGIN_RSP);
    });

    std::string uid_str = std::to_string(uid);
    std::string token_key = USERTOKENPREFIX + uid_str;
    std::string token_value = "";
    bool success = RedisMgr::GetInstance()->Get(token_key, token_value);
    if (!success) {
        rtvalue["error"] = ErrorCodes::UidInvalid;
        return;
    }

    if (token_value != token) {
        rtvalue["error"] = ErrorCodes::TokenInvalid;
        return;
    }

    rtvalue["error"] = ErrorCodes::Success;

    std::string base_key = USER_BASE_INFO + uid_str;
    auto user_info = std::make_shared<UserInfo>();
    bool b_base = GetBaseInfo(base_key, uid, user_info);
    if (!b_base) {
        rtvalue["error"] = ErrorCodes::UidInvalid;
        return;
    }
    rtvalue["uid"] = uid;
    rtvalue["pwd"] = user_info->pwd;
    rtvalue["name"] = user_info->name;
    rtvalue["email"] = user_info->email;
    rtvalue["nick"] = user_info->nick;
    rtvalue["desc"] = user_info->desc;
    rtvalue["sex"] = user_info->sex;
    rtvalue["icon"] = user_info->icon;

    std::vector<std::shared_ptr<ApplyInfo> > apply_list;
    auto b_apply = GetFriendApplyInfo(uid, apply_list);
    if (b_apply) {
        for (auto &apply: apply_list) {
            Json::Value obj;
            obj["name"] = apply->_name;
            obj["uid"] = apply->_uid;
            obj["icon"] = apply->_icon;
            obj["nick"] = apply->_nick;
            obj["sex"] = apply->_sex;
            obj["desc"] = apply->_desc;
            obj["status"] = apply->_status;
            rtvalue["apply_list"].append(obj);
        }
    }

    std::vector<std::shared_ptr<UserInfo> > friend_list;
    bool b_friend_list = GetFriendList(uid, friend_list);
    for (auto &friend_ele: friend_list) {
        Json::Value obj;
        obj["name"] = friend_ele->name;
        obj["uid"] = friend_ele->uid;
        obj["icon"] = friend_ele->icon;
        obj["nick"] = friend_ele->nick;
        obj["sex"] = friend_ele->sex;
        obj["desc"] = friend_ele->desc;
        obj["back"] = friend_ele->back;
        rtvalue["friend_list"].append(obj);
    }

    auto server_name = ConfigManager::GetInstance().GetValue("SelfServer", "Name");
    auto rd_res = RedisMgr::GetInstance()->HGet(LOGIN_COUNT, server_name);
    int count = 0;
    if (!rd_res.empty()) {
        count = std::stoi(rd_res);
    }

    count++;
    auto count_str = std::to_string(count);
    RedisMgr::GetInstance()->HSet(LOGIN_COUNT, server_name, count_str);
    session->SetUserId(uid);
    std::string ipkey = USERIPPREFIX + uid_str;
    RedisMgr::GetInstance()->Set(ipkey, server_name);
    UserMgr::GetInstance()->SetUserSession(uid, session);
    return;
}
