/**
  ******************************************************************************
  * @File           : global.h
  * @Author         : 钟先生
  * @Brief          : XXX Function
  * @Date           : 2024/09/27
  ******************************************************************************
 */

#ifndef WETALK_GLOBAL_H
#define WETALK_GLOBAL_H

#include <QWidget>
#include <functional>
#include <QRegularExpression>
#include <memory>
#include <iostream>
#include <mutex>
#include <QByteArray>
#include <QNetworkReply>
#include "QStyle"
#include <QJsonObject>
#include <QDir>
#include <QSettings>

/*!
 * @brief repolish用来刷新qss
 */
extern std::function<void(QWidget *)> repolish;

enum ReqId {
    ID_GET_VERIFY_CODE = 1001, // 获取验证码
    ID_REG_USER = 1002, // 注册用户
};

enum Modules {
    REGISTERMOD = 0,
};

enum ErrorCodes {
    SUCCESS = 0,
    ERROR_JSON = 1, // json解析失败
    ERROR_NETWORK = 2, // 网络错误
};

extern QString gate_url_prefix;

#endif //WETALK_GLOBAL_H
