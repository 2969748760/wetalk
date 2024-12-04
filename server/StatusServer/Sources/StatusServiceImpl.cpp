//
// Created by 29697 on 24-11-2.
//

#include "StatusServiceImpl.h"
#include "ConfigManager.h"
#include "const.h"
#include "RedisManager.h"
#include <climits>
#include <boost/uuid.hpp>

std::string generate_unique_string() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();

    std::string unique_string = to_string(uuid);

    return unique_string;
}


StatusServiceImpl::StatusServiceImpl() {
    auto &cfg = ConfigManager::GetInstance();
    auto server_list = cfg["chatservers"]["Name"];

    std::vector<std::string> words;

    std::stringstream ss(server_list);
    std::string word;

    while (std::getline(ss, word, ',')) {
        words.push_back(word);
    }

    for (auto &wd: words) {
        if (cfg[wd]["Name"].empty()) {
            continue;
        }

        ChatServer server;
        server.host = cfg[wd]["Host"];
        server.port = cfg[wd]["Port"];
        server.name = cfg[wd]["Name"];
        _servers[server.name] = server;
    }
}

Status StatusServiceImpl::GetChatServer(ServerContext *context, const GetChatServerReq *request,
                                        GetChatServerRsp *response) {
    std::string prefix("wetalk status server has received: ");
    const auto &server = getChatServer();
    response->set_host(server.host);
    response->set_port(server.port);
    response->set_error(ErrorCodes::Success);
    response->set_token(generate_unique_string());
    insertToken(request->uid(), response->token());
    return Status::OK;
}

Status StatusServiceImpl::Login(ServerContext *context, const LoginReq *request, LoginRsp *response) {
    auto uid = request->uid();
    auto token = request->token();

    std::string uid_str = std::to_string(uid);
    std::string token_key = USERTOKENPREFIX + uid_str;
    std::string token_value = "";
    bool success = RedisManager::GetInstance()->Get(token_key, token_value);
    if (success) {
        response->set_error(ErrorCodes::UidInvalid);
        return Status::OK;
    }

    if (token_value != token) {
        response->set_error(ErrorCodes::TokenInvalid);
        return Status::OK;
    }
    response->set_error(ErrorCodes::Success);
    response->set_uid(uid);
    response->set_token(token);
    return Status::OK;
}

void StatusServiceImpl::insertToken(int uid, std::string token) {
    std::string uid_str = std::to_string(uid);
    std::string token_key = USERTOKENPREFIX + uid_str;
    RedisManager::GetInstance()->Set(token_key, token);
}

ChatServer StatusServiceImpl::getChatServer() {
    std::lock_guard<std::mutex> guard(_server_mutex);
    auto minServer = _servers.begin()->second;
    auto count_str = RedisManager::GetInstance()->HGet(LOGIN_COUNT, minServer.name);
    if (count_str.empty()) {
        //不存在则默认设置为最大
        minServer.con_count = INT_MAX;
    } else {
        minServer.con_count = std::stoi(count_str);
    }


    // 使用范围基于for循环
    for (auto &server: _servers) {
        if (server.second.name == minServer.name) {
            continue;
        }

        auto count_str = RedisManager::GetInstance()->HGet(LOGIN_COUNT, server.second.name);
        if (count_str.empty()) {
            server.second.con_count = INT_MAX;
        } else {
            server.second.con_count = std::stoi(count_str);
        }

        if (server.second.con_count < minServer.con_count) {
            minServer = server.second;
        }
    }

    return minServer;
}
