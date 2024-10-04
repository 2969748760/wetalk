/**
  ******************************************************************************
  * @File           : LogicSystem.h
  * @Author         : 钟先生
  * @Brief          : XXX Function
  * @Date           : 2024/10/03
  ******************************************************************************
 */

#ifndef GATESERVER_LOGICSYSTEM_H
#define GATESERVER_LOGICSYSTEM_H

#include "const.h"
#include "Singleton.h"
#include <functional>
#include <map>


class HttpConnection;

typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandler;

class LogicSystem : public Singleton<LogicSystem> {
    friend class Singleton<LogicSystem>;

public:
    ~LogicSystem();

    bool HandleGet(std::string, std::shared_ptr<HttpConnection>);

    void ReGet(std::string, HttpHandler handler);

    void RePost(std::string, HttpHandler handler);

    bool HandlePost(std::string, std::shared_ptr<HttpConnection>);

private:
    LogicSystem();

    std::map<std::string, HttpHandler> _post_handlers;
    std::map<std::string, HttpHandler> _get_handlers;
};

#endif //GATESERVER_LOGICSYSTEM_H
