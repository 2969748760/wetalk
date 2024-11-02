#include <iostream>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include "Headers/const.h"
#include "Headers/ConfigManager.h"
#include <hiredis/hiredis.h>
#include "Headers/RedisManager.h"
#include "Headers/MysqlManager.h"
#include "Headers/AsioIOServicePool.h"
#include <memory>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include "Headers/StatusServiceImpl.h"

void RunServer() {
    auto &cfg = ConfigManager::GetInstance();

    std::string server_address(cfg["StatusServer"]["Host"] + ":" + cfg["StatusServer"]["Port"]);
    StatusServiceImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    boost::asio::io_context io_context;
    boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);

    signals.async_wait([&server](const boost::system::error_code &error, int signal_number) {
        if (error) {
            return;
        }
        std::cout << "Shutting down server..." << std::endl;
        server->Shutdown();
    });

    std::thread([&io_context]() {
        io_context.run();
    }).detach();

    server->Wait();
    io_context.stop();
}


int main() {
    try {
        RunServer();
    } catch (std::exception const &e) {
        std::cerr << "Errors: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}
