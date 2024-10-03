#include <iostream>
#include "Headers/CServer.h"

int main() {
    try {
        unsigned short port = static_cast<unsigned short>(8080);
        net::io_context ioc{1};
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) {
            if(error) {
                std::cout << "signal error is " << error.message() << std::endl;
                return;
            }
            ioc.stop();
        });
        std::make_shared<CServer>(ioc, port)->Start();
        ioc.run();
    } catch (std::exception const &exception) {
        std::cerr << "exception is " << exception.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}
