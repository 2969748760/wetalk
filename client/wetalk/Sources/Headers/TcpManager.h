//
// Created by 29697 on 24-11-21.
//

#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QTcpSocket>
#include "singleton.h"
#include "global.h"
#include <functional>
#include <QObject>


class TcpManager : public QObject, public Singleton<TcpManager>, public std::enable_shared_from_this<TcpManager> {
    Q_OBJECT

public:
    ~TcpManager();

private:
    friend class Singleton<TcpManager>;

    TcpManager();

    void initHandlers();

    void handleMsg(ReqId id, int len, QByteArray data);

    QTcpSocket _socket;
    QString _host;
    uint16_t _port;
    QByteArray _buffer;
    bool _b_recv_pending;
    quint16 _message_id;
    quint16 _message_len;
    QMap<ReqId, std::function<void(ReqId id, int len, QByteArray data)> > _handlers;

public slots:
    void slot_tcp_connect(ServerInfo);

    void slot_send_data(ReqId reqId, QByteArray dataBytes);

signals:
    void sig_con_success(bool bsuccess);
    void sig_send_data(ReqId id, QByteArray data);
    void sig_switch_chatdlg();
    void sig_login_failed(int);
};


#endif //TCPMANAGER_H
