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

extern QString md5Encrypt(const QString &input);

extern std::function<QString(QString)> xorString;

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

enum TipError {
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERROR = 1,
    TIP_PWD_ERROR = 2,
    TIP_CONFIRM_ERROR = 3,
    TIP_PWD_CONFIRM = 4,
    TIP_VERIFY_ERROR = 5,
    TIP_USER_ERROR = 6,
};

enum ClickLabelState {
    NORMAL = 0,
    SELECTED = 1,
};

extern QString gate_url_prefix;

#endif //WETALK_GLOBAL_H
