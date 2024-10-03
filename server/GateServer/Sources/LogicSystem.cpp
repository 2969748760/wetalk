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

LogicSystem::LogicSystem() {
    ReGet("/get_test", [](std::shared_ptr<HttpConnection> connection) {
        beast::ostream(connection->_response.body()) << "receive get_test req";
    });
}

bool LogicSystem::HandleGet(std::string path, std::shared_ptr<HttpConnection> connection) {
    if(_get_handlers.find(path) == _get_handlers.end()) {
        return false;
    }
    _get_handlers[path](connection);
    return true;
}

LogicSystem::~LogicSystem() {

}
