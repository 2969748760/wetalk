cmake_minimum_required(VERSION 3.29)
project(GateServer)

set(CMAKE_CXX_STANDARD 20)
set(Boost_VERBOSE ON)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:utf-8>")

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
        Sources/AsioIOServicePool.cpp
        Sources/Headers/AsioIOServicePool.h
        Sources/RedisManager.cpp
        Sources/Headers/RedisManager.h
        Sources/MysqlDao.cpp
        Sources/Headers/MysqlDao.h
        Sources/MysqlManager.cpp
        Sources/Headers/MysqlManager.h
        Sources/StatusGrpcClient.cpp
        Sources/Headers/StatusGrpcClient.h
)

target_compile_definitions(GateServer PRIVATE _WIN32_WINNT=0x0A00)
#target_compile_options(GateServer PRIVATE "/MDd")

target_include_directories(GateServer PRIVATE
        ${PROJECT_SOURCE_DIR}/Sources/Headers
)

if (WIN32)
    message("This is Windows")
    target_link_libraries(GateServer PRIVATE ws2_32 mswsock)
endif ()

find_package(Boost CONFIG REQUIRED COMPONENTS system filesystem)
target_link_libraries(GateServer PRIVATE Boost::boost Boost::system Boost::filesystem)

find_package(gRPC CONFIG REQUIRED)
target_link_libraries(GateServer PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts)

find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(GateServer PRIVATE JsonCpp::JsonCpp)

find_package(redis++ CONFIG REQUIRED)
target_link_libraries(GateServer PRIVATE redis++::redis++)

set(mysql-concpp_DIR "D:/cpp_soft/mysql-connector-c++-8.4.0-winx64-debug")
set(MYSQL_CONCPP_ROOT "D:/cpp_soft/mysql-connector-c++-8.4.0-winx64-debug")
set(MYSQL_CONCPP_LIBARIES
        "D:/cpp_soft/mysql-connector-c++-8.4.0-winx64-debug/lib64/vs14/mysqlcppconn.lib"
        "D:/cpp_soft/mysql-connector-c++-8.4.0-winx64-debug/lib64/vs14/mysqlcppconn8.lib"
)
find_library(MYSQLCPPCONN8_LIB mysqlcppconn8 PATHS "${mysql-concpp_DIR}/lib64/vs14")
find_package(mysql-concpp CONFIG REQUIRED debug)
target_include_directories(GateServer PRIVATE "${mysql-concpp_DIR}/include")
target_link_libraries(GateServer PRIVATE mysql::concpp)
target_link_libraries(GateServer PRIVATE ${MYSQL_CONCPP_LIBARIES})

# 设置编译选项
if (MSVC)
    message("This is MSVC")
    # 为MSVC设置调试和发布配置的运行时库
    foreach (build_type IN ITEMS RELEASE RELWITHDEBINFO MINSIZEREL DEBUG)
        string(TOUPPER ${build_type} upper_build_type)
        set(CMAKE_CXX_FLAGS_${upper_build_type} "${CMAKE_CXX_FLAGS_${upper_build_type}} /MDd")
    endforeach ()
endif ()
