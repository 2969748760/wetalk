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
#include <queue>
#include <condition_variable>
#include "Singleton.h"
#include <functional>
#include <map>
#include <unordered_map>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <atomic>
#include <hiredis/hiredis.h>
#include <cassert>

#define CODEPREFIX "code_"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

enum ErrorCodes {
    Success = 0,
    Error_Json = 1001, // json解析错误
    RPCFailed = 1002, // RPC请求错误
    VerifyExpired = 1003, // 验证码过期
    VerifyCodeErr = 1004, // 验证码错误
    UserExist = 1005, // 用户已存在
    PasswordErr = 1006, // 密码错误
    EmailNotMatch = 1007, // 邮箱不匹配
    PasswordUpdateFailed = 1008, // 密码更新失败
    PasswordInvalid = 1009, // 密码不合法
};
#endif //GATESERVER_CONST_H
