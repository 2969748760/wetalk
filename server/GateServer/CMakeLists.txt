cmake_minimum_required(VERSION 3.29)
project(GateServer)

set(CMAKE_CXX_STANDARD 20)
set(Boost_VERBOSE ON)

# 设置代理
#set(ENV{http_proxy} "http://127.0.0.1:7890")
#set(ENV{https_proxy} "http://127.0.0.1:7890")

add_executable(GateServer Sources/main.cpp
        Sources/CServer.cpp
        Sources/Headers/CServer.h
        Sources/Headers/const.h
        Sources/HttpConnection.cpp
        Sources/Headers/HttpConnection.h
        Sources/Headers/Singleton.h
        Sources/LogicSystem.cpp
        Sources/Headers/LogicSystem.h
        Sources/message.grpc.pb.cc
        Sources/Headers/message.grpc.pb.h
        Sources/message.pb.cc
        Sources/Headers/message.pb.h
        Sources/VerityGrpcClient.cpp
        Sources/Headers/VerityGrpcClient.h
        Sources/ConfigManager.cpp
        Sources/Headers/ConfigManager.h
)

target_include_directories(GateServer PRIVATE
        ${PROJECT_SOURCE_DIR}/Sources/Headers
)

if (WIN32)
    message("This is Windows")
    target_link_libraries(GateServer PRIVATE ws2_32 mswsock)
endif ()

#set(Boost_DIR "D:/.vcpkg-clion/vcpkg/installed/x64-windows/share/boost")
find_package(Boost CONFIG REQUIRED COMPONENTS system filesystem)
target_link_libraries(GateServer PRIVATE Boost::boost Boost::filesystem Boost::system)

find_package(gRPC CONFIG REQUIRED)
target_link_libraries(GateServer PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts)

find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(GateServer PRIVATE JsonCpp::JsonCpp)