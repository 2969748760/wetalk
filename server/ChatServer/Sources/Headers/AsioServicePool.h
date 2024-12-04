//
// Created by 29697 on 24-11-25.
//

#ifndef ASIOSERVICEPOOL_H
#define ASIOSERVICEPOOL_H

#include <vector>
#include <boost/asio.hpp>
#include "Singleton.h"


class AsioServicePool : public Singleton<AsioServicePool> {
    friend Singleton<AsioServicePool>;

public:
    using IOService = boost::asio::io_context;
    using Work = boost::asio::io_context::work;
    using WorkPtr = std::unique_ptr<Work>;
    ~AsioServicePool();
    AsioServicePool(const AsioServicePool &) = delete;
    AsioServicePool &operator=(const AsioServicePool &) = delete;
    boost::asio::io_context& GetIOService();
    void Stop();

private:
    explicit AsioServicePool(std::size_t size = std::thread::hardware_concurrency());
    std::vector<IOService> _ioServices;
    std::vector<WorkPtr> _works;
    std::vector<std::thread> _threads;
    std::size_t _nextIOService;
};


#endif //ASIOSERVICEPOOL_H
