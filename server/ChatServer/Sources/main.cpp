#include <iostream>
#include "LogicSystem.h"
#include <csignal>
#include <thread>
#include <mutex>
#include "AsioServicePool.h"
#include "CServer.h"
#include "ConfigManager.h"

bool bstop = false;
std::condition_variable cond_quit;
std::mutex mutex_quit;

int main() {
    try {
        auto &cfg = ConfigManager::GetInstance();
        auto pool = AsioServicePool::GetInstance();
        boost::asio::io_context io_context;
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&io_context, pool](auto, auto) {
            io_context.stop();
            pool->Stop();
        });
        auto port_str = cfg["SelfServer"]["Port"];
        CServer s(io_context, std::atoi(port_str.c_str()));
        io_context.run();
    } catch (std::exception& e) {
        std::cerr<<"Errors: "<<e.what()<<std::endl;
    }
    return 0;
}
