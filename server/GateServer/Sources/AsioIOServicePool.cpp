//
// Created by 29697 on 24-10-23.
//

#include "Headers/AsioIOServicePool.h"
#include <iostream>

using namespace std;

AsioIOServicePool::AsioIOServicePool(std::size_t size) : _io_services(size), _works(size), _next_io_service(0) {
    for (std::size_t i = 0; i < size; i++) {
        _works[i] = std::unique_ptr<Work>(new Work(_io_services[i]));
    }
    // 启动线程
    for (std::size_t i = 0; i < _io_services.size(); i++) {
        _threads.emplace_back([this, i]() {
            _io_services[i].run();
        });
    }
}

AsioIOServicePool::~AsioIOServicePool() {
    Stop();
    std::cout<<"AsioIOServicePool destruct"<<std::endl;
}

boost::asio::io_context &AsioIOServicePool::GetIOService() {
    auto &io_service = _io_services[_next_io_service++];
    if (_next_io_service == _io_services.size()) {
        _next_io_service = 0;
    }
    return io_service;
}

void AsioIOServicePool::Stop() {
    for(auto& work:_works) {
        work->get_io_context().stop();
        work.reset();
    }
    for(auto& thread:_threads) {
        thread.join();
    }
}