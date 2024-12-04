cmake_minimum_required(VERSION 3.30)
project(ChatServer)

set(CMAKE_CXX_STANDARD 20)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:utf-8>")

file(GLOB CPP_FILES "Sources/*.cpp")
file(GLOB H_FILES "Sources/Headers/*.h")
file(GLOB CC_FILES "Sources/*.cc")

add_executable(ChatServer ${CPP_FILES} ${H_FILES} ${CC_FILES})

target_include_directories(ChatServer PRIVATE
        ${PROJECT_SOURCE_DIR}/Sources/Headers
)

target_compile_definitions(ChatServer PRIVATE _WIN32_WINNT=0x0A00)

if (WIN32)
    message("This is Windows")
    target_link_libraries(ChatServer PRIVATE ws2_32 mswsock)
endif ()

find_package(Boost CONFIG REQUIRED COMPONENTS system filesystem)
target_link_libraries(ChatServer PRIVATE Boost::boost Boost::system Boost::filesystem)

find_package(gRPC CONFIG REQUIRED)
target_link_libraries(ChatServer PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts)

find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(ChatServer PRIVATE JsonCpp::JsonCpp)

find_package(redis++ CONFIG REQUIRED)
target_link_libraries(ChatServer PRIVATE redis++::redis++)

set(mysql-concpp_DIR "D:/cpp_soft/mysql-connector-c++-8.4.0-winx64-debug")
set(MYSQL_CONCPP_ROOT "D:/cpp_soft/mysql-connector-c++-8.4.0-winx64-debug")
set(MYSQL_CONCPP_LIBARIES
        "D:/cpp_soft/mysql-connector-c++-8.4.0-winx64-debug/lib64/vs14/mysqlcppconn.lib"
        "D:/cpp_soft/mysql-connector-c++-8.4.0-winx64-debug/lib64/vs14/mysqlcppconn8.lib"
)
find_library(MYSQLCPPCONN8_LIB mysqlcppconn8 PATHS "${mysql-concpp_DIR}/lib64/vs14")
find_package(mysql-concpp CONFIG REQUIRED debug)
target_include_directories(ChatServer PRIVATE "${mysql-concpp_DIR}/include")
target_link_libraries(ChatServer PRIVATE mysql::concpp)
target_link_libraries(ChatServer PRIVATE ${MYSQL_CONCPP_LIBARIES})

if (MSVC)
    message("This is MSVC")
    # 为MSVC设置调试和发布配置的运行时库
    foreach (build_type IN ITEMS RELEASE RELWITHDEBINFO MINSIZEREL DEBUG)
        string(TOUPPER ${build_type} upper_build_type)
        set(CMAKE_CXX_FLAGS_${upper_build_type} "${CMAKE_CXX_FLAGS_${upper_build_type}} /MDd")
    endforeach ()
endif ()
