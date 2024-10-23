//
// Created by 29697 on 24-10-23.
//

#ifndef ASIOIOSERVICEPOOL_H
#define ASIOIOSERVICEPOOL_H

#include <vector>
#include <boost/asio.hpp>
#include "Headers/Singleton.h"

class AsioIOServicePool : public Singleton<AsioIOServicePool> {
    friend Singleton<AsioIOServicePool>;

public:
    using IOService = boost::asio::io_context;
    using Work = boost::asio::io_context::work;
    using WorkPtr = std::unique_ptr<Work>;

    ~AsioIOServicePool();

    AsioIOServicePool(const AsioIOServicePool &) = delete;

    AsioIOServicePool &operator=(const AsioIOServicePool &) = delete;

    // 使用round-robin的方式获取io_service
    boost::asio::io_context &GetIOService();

    void Stop();

private:
    AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency());

    std::vector<IOService> _io_services;
    std::vector<WorkPtr> _works;
    std::vector<std::thread> _threads;
    std::size_t _next_io_service;
};


#endif //ASIOIOSERVICEPOOL_H
