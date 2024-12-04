//
// Created by 29697 on 24-11-21.
//

#include "TCPManager.h"
#include <QAbstractSocket>
#include <iostream>
#include <QJsonDocument>

#include "UserMgr.h"

TcpManager::~TcpManager() {
}

TcpManager::TcpManager() : _host(""), _port(0), _b_recv_pending(false), _message_id(0), _message_len(0) {
    QObject::connect(&_socket, &QTcpSocket::connected, [&]() {
        std::cout << "connected to server" << std::endl;
        emit sig_con_success(true);
    });

    QObject::connect(&_socket, &QTcpSocket::readyRead, [&]() {
        _buffer.append(_socket.readAll());
        QDataStream stream(&_buffer, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_6_8);
        forever {
            // 先解析头部
            if (!_b_recv_pending) {
                if (_buffer.size() < static_cast<int>(sizeof(quint16) * 2)) {
                    return;
                }

                stream >> _message_id >> _message_len;

                _buffer = _buffer.mid(sizeof(quint16) * 2);

                qDebug() << "message id: " << _message_id << " message len: " << _message_len;
            }

            if (_buffer.size() < _message_len) {
                _b_recv_pending = true;
                return;
            }

            _b_recv_pending = false;

            QByteArray messageBody = _buffer.mid(0, _message_len);

            qDebug() << "message body: " << messageBody;

            _buffer = _buffer.mid(_message_len);
            handleMsg(ReqId(_message_id), _message_len, messageBody);
        }
    });

    QObject::connect(&_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
                     [&](QAbstractSocket::SocketError socketError) {
                         Q_UNUSED(socketError);
                         qDebug() << "socket error: " << _socket.errorString();
                     });

    QObject::connect(&_socket, &QTcpSocket::disconnected, [&]() {
        qDebug() << "disconnected from server";
    });

    QObject::connect(this, &TcpManager::sig_send_data, this, &TcpManager::slot_send_data);

    initHandlers();
}

void TcpManager::initHandlers() {
    _handlers.insert(ID_CHAT_LOGIN_RSP, [this](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        qDebug() << "handle id is " << id << " data is " << data;

        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        if (jsonDoc.isNull()) {
            qDebug() << "json doc is null";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if (!jsonObj.contains("error")) {
            int err = ErrorCodes::ERROR_JSON;
            qDebug() << "Login Failed, Err is Json Parse Err" << err;
            emit sig_login_failed(err);
            return;
        }

        int err = jsonObj["error"].toInt();
        if (err != ErrorCodes::SUCCESS) {
            qDebug() << "Login Failed, Err is " << err;
            emit sig_login_failed(err);
            return;
        }

        UserMgr::GetInstance()->SetUid(jsonObj["uid"].toInt());
        UserMgr::GetInstance()->SetName(jsonObj["name"].toString());
        UserMgr::GetInstance()->SetToken(jsonObj["token"].toString());
        emit sig_switch_chatdlg();
    });
}

void TcpManager::handleMsg(ReqId id, int len, QByteArray data) {
    auto find_iter = _handlers.find(id);
    if (find_iter == _handlers.end()) {
        qDebug() << "no handler for id: " << id;
        return;
    }

    find_iter.value()(id, len, data);
}

void TcpManager::slot_tcp_connect(ServerInfo si) {
    qDebug() << "receive tcp connect signal";
    qDebug() << "Connecting to server...";
    _host = si.Host;
    _port = static_cast<uint16_t>(si.Port.toInt());
    _socket.connectToHost(_host, _port);
}

void TcpManager::slot_send_data(ReqId reqId, QByteArray dataBytes) {
    uint16_t id = reqId;
    quint16 len = static_cast<quint16>(dataBytes.size());
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);
    out << id << len;
    block.append(dataBytes);
    _socket.write(block);
}
