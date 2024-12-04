//
// Created by 29697 on 24-11-25.
//

#ifndef CSERVER_H
#define CSERVER_H

#include <boost/asio.hpp>
#include "CSession.h"
#include <map>
#include <mutex>

using boost::asio::ip::tcp;

class CServer {
public:
    CServer(boost::asio::io_context &io_context, short port);

    ~CServer();

    void ClearSession(std::string);

private:
    void HandleAccept(std::shared_ptr<CSession> session, const boost::system::error_code &error);

    void StartAccept();

    boost::asio::io_context &io_context;
    short _port;
    tcp::acceptor _acceptor;
    std::map<std::string, std::shared_ptr<CSession> > _sessions;
    std::mutex _mutex;
};


#endif //CSERVER_H
