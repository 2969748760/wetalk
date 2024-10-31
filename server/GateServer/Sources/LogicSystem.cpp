/**
  ******************************************************************************
  * @File           : LogicSystem.cpp
  * @Author         : 钟先生
  * @Brief          : XXX Function
  * @Date           : 2024/10/03
  ******************************************************************************
 */

#include "Headers/LogicSystem.h"
#include "Headers/HttpConnection.h"
#include "Headers/VerityGrpcClient.h"
#include "Headers/RedisManager.h"
#include "Headers/MysqlManager.h"

void LogicSystem::RegGet(std::string url, HttpHandler handler) {
    _get_handlers.insert(make_pair(url, handler));
}

void LogicSystem::RegPost(std::string url, HttpHandler handler) {
    _post_handlers.insert(make_pair(url, handler));
}

LogicSystem::LogicSystem() {
    RegGet("/get_test", [](std::shared_ptr<HttpConnection> connection) {
        beast::ostream(connection->_response.body()) << "receive get_test req" << std::endl;
        int i = 0;
        for (auto &elem: connection->_get_params) {
            i++;
            beast::ostream(connection->_response.body()) << "param" << i << " : " << elem.first << " = " << elem.second
                    << std::endl;
        }
    });

    RegPost("/get_verifycode", [](std::shared_ptr<HttpConnection> connection) {
        auto body_str = beast::buffers_to_string(connection->_request.body().data());
        std::cout << "receive body is " << body_str << std::endl;
        connection->_response.set(http::field::content_type, "text/json");
        Json::Value root;
        Json::Reader reader;
        Json::Value src_root;
        bool parse_success = reader.parse(body_str, src_root);
        if (!parse_success) {
            std::cout << "Faild to parse JSON data!" << std::endl;
            root["error"] = ErrorCodes::Error_Json;
            std::string jsonstr = root.toStyledString();
            beast::ostream(connection->_response.body()) << jsonstr;
            return true;
        }

        if (!src_root.isMember("email")) {
            std::cout << "Faild to parse JSON data!" << std::endl;
            root["error"] = ErrorCodes::Error_Json;
            std::string jsonstr = root.toStyledString();
            beast::ostream(connection->_response.body()) << jsonstr;
            return true;
        }

        auto email = src_root["email"].asString();
        GetVerifyRsp response = VerifyGrpcClient::GetInstance()->GetVerifyCode(email);
        std::cout << "email is " << email << std::endl;
        root["error"] = response.error();
        root["email"] = src_root["email"];
        std::string jsonstr = root.toStyledString();
        beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    });

    RegPost("/user_register", [](std::shared_ptr<HttpConnection> connection) {
        auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
        std::cout << "receive body is " << body_str << std::endl;
        connection->_response.set(http::field::content_type, "text/json");
        Json::Value root;
        Json::Reader reader;
        Json::Value src_root;
        bool parse_success = reader.parse(body_str, src_root);
        if (!parse_success) {
            std::cout << "Failed to parse JSON data!" << std::endl;
            root["error"] = ErrorCodes::Error_Json;
            std::string jsonstr = root.toStyledString();
            beast::ostream(connection->_response.body()) << jsonstr;
            return true;
        }

        auto email = src_root["email"].asString();
        auto name = src_root["user"].asString();
        auto passwd = src_root["passwd"].asString();
        auto confirm = src_root["confirm"].asString();

        if (passwd != confirm) {
            std::cout << "password not match" << std::endl;
            root["error"] = ErrorCodes::PasswordErr;
            std::string jsonstr = root.toStyledString();
            beast::ostream(connection->_response.body()) << jsonstr;
            return true;
        }

        //先查找redis中email对应的验证码是否合理
        std::string verify_code;
        bool b_get_verify = RedisManager::GetInstance()->Get(CODEPREFIX + src_root["email"].asString(), verify_code);
        if (!b_get_verify) {
            std::cout << " get verify code expired" << std::endl;
            root["error"] = ErrorCodes::VerifyExpired;
            std::string jsonstr = root.toStyledString();
            beast::ostream(connection->_response.body()) << jsonstr;
            return true;
        }

        if (verify_code != src_root["verify_code"].asString()) {
            std::cout << " verify code error" << std::endl;
            root["error"] = ErrorCodes::VerifyCodeErr;
            std::string jsonstr = root.toStyledString();
            beast::ostream(connection->_response.body()) << jsonstr;
            return true;
        }

        int uid = MysqlManager::GetInstance()->RegUser(name, email, passwd);
        if (uid == 0 || uid == -1) {
            std::cout << " user or email already exist" << std::endl;
            root["error"] = ErrorCodes::UserExist;
            std::string jsonstr = root.toStyledString();
            beast::ostream(connection->_response.body()) << jsonstr;
            return true;
        }

        //查找数据库判断用户是否存在
        root["error"] = 0;
        root["email"] = email;
        root["uid"] = uid;
        root["user"] = name;
        root["passwd"] = passwd;
        root["confirm"] = confirm;
        root["verify_code"] = verify_code;
        std::string jsonstr = root.toStyledString();
        beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    });

    RegPost("/reset_pwd", [](std::shared_ptr<HttpConnection> connection) {
        auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
        std::cout << "receive body is " << body_str << std::endl;
        connection->_response.set(http::field::content_type, "text/json");
        Json::Value root;
        Json::Reader reader;
        Json::Value src_root;
        bool parse_success = reader.parse(body_str, src_root);
        if (!parse_success) {
            std::cout << "Failed to parse JSON data!" << std::endl;
            root["error"] = ErrorCodes::Error_Json;
            std::string json_str = root.toStyledString();
            beast::ostream(connection->_response.body()) << json_str;
            return true;
        }

        auto email = src_root["email"].asString();
        auto user = src_root["user"].asString();
        auto passwd = src_root["passwd"].asString();

        std::string verify_code;
        bool b_get_verify = RedisManager::GetInstance()->Get(CODEPREFIX + src_root["email"].asString(), verify_code);
        if (!b_get_verify) {
            std::cout << "get verify code expired" << std::endl;
            root["error"] = ErrorCodes::VerifyExpired;
            std::string json_str = root.toStyledString();
            beast::ostream(connection->_response.body()) << json_str;
            return true;
        }

        if (verify_code != src_root["verifycode"].asString()) {
            std::cout << "verify code error" << std::endl;
            root["error"] = ErrorCodes::VerifyCodeErr;
            std::string json_str = root.toStyledString();
            beast::ostream(connection->_response.body()) << json_str;
            return true;
        }

        bool email_valid = MysqlManager::GetInstance()->CheckEmail(user, email);
        if (!email_valid) {
            std::cout << "user email not match" << std::endl;
            root["error"] = ErrorCodes::EmailNotMatch;
            std::string json_str = root.toStyledString();
            beast::ostream(connection->_response.body()) << json_str;
            return true;
        }

        bool b_up = MysqlManager::GetInstance()->UpdatePwd(user, passwd);
        if (!b_up) {
            std::cout << "update password failed" << std::endl;
            root["error"] = ErrorCodes::PasswordUpdateFailed;
            std::string json_str = root.toStyledString();
            beast::ostream(connection->_response.body()) << json_str;
            return true;
        }

        std::cout << "succeed to update password" << std::endl;
        root["error"] = 0;
        root["email"] = email;
        root["user"] = user;
        root["passwd"] = passwd;
        root["verifycode"] = src_root["verifycode"].asString();
        std::string json_str = root.toStyledString();
        beast::ostream(connection->_response.body()) << json_str;
        return true;
    });
}

bool LogicSystem::HandleGet(std::string path, std::shared_ptr<HttpConnection> connection) {
    if (_get_handlers.find(path) == _get_handlers.end()) {
        return false;
    }
    _get_handlers[path](connection);
    return true;
}

LogicSystem::~LogicSystem() {
}

bool LogicSystem::HandlePost(std::string path, std::shared_ptr<HttpConnection> connection) {
    if (_post_handlers.find(path) == _post_handlers.end()) {
        return false;
    }
    _post_handlers[path](connection);
    return true;
}
