//
// Created by 29697 on 24-11-2.
//

#ifndef STATUSSERVICEIMPL_H
#define STATUSSERVICEIMPL_H

#include <grpcpp/grpcpp.h>
#include "Headers/message.grpc.pb.h"
#include <mutex>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using message::GetChatServerReq;
using message::GetChatServerRsp;
using message::StatusService;
using message::LoginReq;
using message::LoginRsp;

class ChatServer {
public:
    std::string host;
    std::string port;
    std::string name;
    int con_count;

    ChatServer() : host(""), port(""), name(""), con_count(0) {
    };

    ChatServer(const ChatServer &chatServer) : host(chatServer.host), port(chatServer.port), name(chatServer.name),
                                               con_count(chatServer.con_count) {
    };

    ChatServer &operator=(const ChatServer &chatServer) {
        if (this == &chatServer) {
            return *this;
        }
        this->host = chatServer.host;
        this->port = chatServer.port;
        this->name = chatServer.name;
        this->con_count = chatServer.con_count;
        return *this;
    }
};

class StatusServiceImpl final : public StatusService::Service {
public:
    StatusServiceImpl();

    Status GetChatServer(ServerContext *context, const GetChatServerReq *request, GetChatServerRsp *response) override;

    Status Login(ServerContext *context, const LoginReq *request, LoginRsp *response) override;

private:
    void insertToken(int uid, std::string token);

    ChatServer getChatServer();

    std::unordered_map<std::string, ChatServer> _servers;
    std::mutex _server_mutex;
};


#endif //STATUSSERVICEIMPL_H
