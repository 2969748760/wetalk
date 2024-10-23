//
// Created by 29697 on 24-10-19.
//

#ifndef VERITYGRPCCLIENT_H
#define VERITYGRPCCLIENT_H

#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "const.h"
#include "Singleton.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVerifyReq;
using message::GetVerifyRsp;
using message::VerifyService;

class RPCConnectPool {
public:
    RPCConnectPool(size_t pool_size, std::string host, std::string port)
        : pool_size_(pool_size), host_(host), port_(port), b_stop_(false) {
        {
            for (size_t i = 0; i < pool_size_; i++) {
                std::shared_ptr<Channel> channel = grpc::CreateChannel(host_ + ":" + port_,
                                                                       grpc::InsecureChannelCredentials());
                connections_.push(VerifyService::NewStub(channel));
            }
        }
    }

    ~RPCConnectPool() {
        std::lock_guard<std::mutex> lock(mutex_);
        Close();
        while (!connections_.empty()) {
            connections_.pop();
        }
    }

    void Close() {
        b_stop_ = true;
        cond_.notify_all();
    }

    std::unique_ptr<VerifyService::Stub> getConnection() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this]() {
            if (b_stop_) {
                return true;
            }
            return !connections_.empty();
        });
        if (b_stop_) {
            return nullptr;
        }
        auto context = std::move(connections_.front());
        connections_.pop();
        return context;
    }

    void returnConnection(std::unique_ptr<VerifyService::Stub> connection) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (b_stop_)return;
        connections_.push(std::move(connection));
        cond_.notify_one();
    }

private:
    std::atomic<bool> b_stop_;
    size_t pool_size_;
    std::string host_;
    std::string port_;
    std::queue<std::unique_ptr<VerifyService::Stub> > connections_;
    std::condition_variable cond_;
    std::mutex mutex_;
};

class VerifyGrpcClient : public Singleton<VerifyGrpcClient> {
    friend class Singleton<VerifyGrpcClient>;

public:
    GetVerifyRsp GetVerifyCode(std::string email) {
        ClientContext context;
        GetVerifyRsp reply;
        GetVerifyReq request;
        request.set_email(email);
        auto stub = pool_->getConnection();
        Status status = stub->GetVerifyCode(&context, request, &reply);

        if (status.ok()) {
            pool_->returnConnection(std::move(stub));
            return reply;
        } else {
            pool_->returnConnection(std::move(stub));
            reply.set_error(ErrorCodes::RPCFailed);
            return reply;
        }
    }

private:
    VerifyGrpcClient();

    std::unique_ptr<RPCConnectPool> pool_;
};


#endif //VERITYGRPCCLIENT_H
