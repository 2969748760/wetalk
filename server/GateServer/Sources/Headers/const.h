/**
  ******************************************************************************
  * @File           : const.h
  * @Author         : 钟先生
  * @Brief          : XXX Function
  * @Date           : 2024/10/03
  ******************************************************************************
 */

#ifndef GATESERVER_CONST_H
#define GATESERVER_CONST_H

#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include "Singleton.h"
#include <functional>
#include <map>
#include <unordered_map>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

enum ErrorCodes {
    Success = 0,
    Error_Json = 1001,
    RPCFailed = 1002,
};
#endif //GATESERVER_CONST_H
