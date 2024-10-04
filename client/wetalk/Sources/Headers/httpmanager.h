/**
  ******************************************************************************
  * @File           : HttpManager.h
  * @Author         : 钟先生
  * @Brief          : XXX Function
  * @Date           : 2024/09/27
  ******************************************************************************
 */

#ifndef WETALK_HTTPMANAGER_H
#define WETALK_HTTPMANAGER_H

#include "singleton.h"
#include <QString>
#include <QUrl>
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

// CRTP模式
class HttpManager : public QObject, public Singleton<HttpManager>, public std::enable_shared_from_this<HttpManager> {
Q_OBJECT

public:
    ~HttpManager();

    void PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod);

private:
    friend class Singleton<HttpManager>;

    QNetworkAccessManager _manager;

    HttpManager();

public slots:
    void slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);

signals:

    void sig_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);

    void sig_reg_mod_finish(ReqId id, QString res, ErrorCodes err);
};


#endif //WETALK_HTTPMANAGER_H
