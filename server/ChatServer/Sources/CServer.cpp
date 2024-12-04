//
// Created by 29697 on 24-11-25.
//

#include "CServer.h"
#include <iostream>

#include "AsioServicePool.h"
#include "UserMgr.h"

CServer::CServer(boost::asio::io_context &io_context, short port)
    : io_context(io_context), _port(port), _acceptor(
          io_context, tcp::endpoint(tcp::v4(), port)) {
    std::cout << "Server start success, listen on port: " << port << std::endl;
    StartAccept();
}

CServer::~CServer() {
    std::cout << "Server destruct listen on port : " << _port << std::endl;
}

void CServer::ClearSession(std::string uuid) {
    if (_sessions.find(uuid) != _sessions.end()) {
        UserMgr::GetInstance()->RmvUserSession(_sessions[uuid]->GetUserId());
    } else {
        std::lock_guard<std::mutex> lock(_mutex);
        _sessions.erase(uuid);
    }
}

void CServer::HandleAccept(std::shared_ptr<CSession> session, const boost::system::error_code &error) {
    if (!error) {
        session->Start();
        std::lock_guard<std::mutex> lock(_mutex);
        _sessions.insert(std::make_pair(session->GetSessionId(), session));
    } else {
        std::cout << "session accept failed, error is " << error.what() << std::endl;
    }
}

void CServer::StartAccept() {
    auto &io_context = AsioServicePool::GetInstance()->GetIOService();
    std::shared_ptr<CSession> new_session = std::make_shared<CSession>(io_context, this);
    _acceptor.async_accept(new_session->GetSocket(),
                           std::bind(&CServer::HandleAccept, this, new_session, std::placeholders::_1));
}
