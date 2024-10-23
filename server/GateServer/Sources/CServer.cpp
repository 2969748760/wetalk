/**
  ******************************************************************************
  * @File           : CServer.cpp
  * @Author         : 钟先生
  * @Brief          : XXX Function
  * @Date           : 2024/10/03
  ******************************************************************************
 */

#include "Headers/CServer.h"

#include <AsioIOServicePool.h>

#include "Headers/HttpConnection.h"
#include "Headers/AsioIOServicePool.h"

CServer::CServer(boost::asio::io_context &ioc, unsigned short &port) : _ioc(ioc),
                                                                       _acceptor(ioc, tcp::endpoint(tcp::v4(), port)) {
}

void CServer::Start() {
    auto self = shared_from_this();
    auto &io_context = AsioIOServicePool::GetInstance()->GetIOService();
    std::shared_ptr<HttpConnection> new_connection = std::make_shared<HttpConnection>(io_context);
    _acceptor.async_accept(new_connection->GetSocket(), [self, new_connection](beast::error_code ec) {
        try {
            if (ec) {
                self->Start();
                return;
            }
            // 创建新连接，并且创建HttpConnection类管理这个连接
            new_connection->Start();
            self->Start();
        } catch (std::exception &exp) {
            std::cout << "exception is " << exp.what() << std::endl;
            self->Start();
        }
    });
}
