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

void LogicSystem::ReGet(std::string url, HttpHandler handler) {
    _get_handlers.insert(make_pair(url, handler));
}

void LogicSystem::RePost(std::string url, HttpHandler handler) {
    _post_handlers.insert(make_pair(url, handler));
}

LogicSystem::LogicSystem() {
    ReGet("/get_test", [](std::shared_ptr<HttpConnection> connection) {
        beast::ostream(connection->_response.body()) << "receive get_test req" << std::endl;
        int i = 0;
        for (auto &elem: connection->_get_params) {
            i++;
            beast::ostream(connection->_response.body()) << "param" << i << " : " << elem.first << " = " << elem.second
                                                         << std::endl;
        }
    });

    RePost("/get_varifycode", [](std::shared_ptr<HttpConnection> connection) {
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
        std::cout << "email is " << email << std::endl;
        root["error"] = ErrorCodes::Success;
        root["email"] = src_root["email"];
        std::string jsonstr = root.toStyledString();
        beast::ostream(connection->_response.body()) << jsonstr;
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
