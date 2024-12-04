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
    ID_RESET_PWD = 1003, // 重置密码
    ID_LOGIN_USER = 1004, // 登录
    ID_CHAT_LOGIN = 1005, // 聊天登录
    ID_CHAT_LOGIN_RSP = 1006, // 登录聊天服务器回包
};

enum Modules {
    REGISTERMOD = 0,
    RESETMOD = 1,
    LOGINMOD = 2,
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

struct ServerInfo {
    QString Host;
    QString Port;
    QString Token;
    int Uid;
};

enum ChatUIMode {
    SearchMode, // 搜索模式
    ChatMode, // 聊天模式
    ContactMode, // 联系模式
};

enum ListItemType {
    CHAT_USER_ITEM, //聊天用户
    CONTACT_USER_ITEM, //联系人用户
    SEARCH_USER_ITEM, //搜索到的用户
    ADD_USER_TIP_ITEM, //提示添加用户
    INVALID_ITEM, //不可点击条目
    GROUP_TIP_ITEM, //分组提示条目
};

#endif //WETALK_GLOBAL_H
