//
// Created by 29697 on 24-11-25.
//

#include "AsioServicePool.h"
#include <iostream>
#include <memory>

AsioServicePool::AsioServicePool(std::size_t size): _ioServices(size), _works(size), _nextIOService(0) {
    for (size_t i = 0; i < size; i++) {
        _works[i] = std::make_unique<Work>(_ioServices[i]);
    }

    for (std::size_t i = 0; i < _ioServices.size(); i++) {
        _threads.emplace_back([this, i] {
            _ioServices[i].run();
        });
    }
}

AsioServicePool::~AsioServicePool() {
    std::cout << "AsioIOServicePool destruct" << std::endl;
}

boost::asio::io_context &AsioServicePool::GetIOService() {
    auto &service = _ioServices[_nextIOService++];
    if (_nextIOService == _ioServices.size()) {
        _nextIOService = 0;
    }
    return service;
}

void AsioServicePool::Stop() {
    for (auto &work: _works) {
        work->get_io_context().stop();
        work.reset();
    }
    for (auto &t: _threads) {
        t.join();
    }
}
