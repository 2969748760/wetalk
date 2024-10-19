cmake_minimum_required(VERSION 3.29)
project(GateServer)

set(CMAKE_CXX_STANDARD 20)

set(ENV{http_proxy} "http://127.0.0.1:7890")
set(ENV{https_proxy} "http://127.0.0.1:7890")

#file(GLOB BOOST_LIBRARIES "${PROJECT_SOURCE_DIR}/lib/boostlib/*.lib")
#file(GLOB JSONCPP_LIBRARIES "${PROJECT_SOURCE_DIR}/lib/jsoncpplib/libjsoncpp.dll.a")

add_executable(GateServer Sources/main.cpp
        Sources/CServer.cpp
        Sources/Headers/CServer.h
        Sources/Headers/const.h
        Sources/HttpConnection.cpp
        Sources/Headers/HttpConnection.h
        Sources/Headers/Singleton.h
        Sources/LogicSystem.cpp
        Sources/Headers/LogicSystem.h
)

#target_include_directories(GateServer PRIVATE
#        ${PROJECT_SOURCE_DIR}/Sources/Headers
#        ${PROJECT_SOURCE_DIR}/include
#        ${PROJECT_SOURCE_DIR}/include/boost
#        ${PROJECT_SOURCE_DIR}/include/jsoncpp
#)
#
#target_link_libraries(GateServer PRIVATE
#        ${JSONCPP_LIBRARIES}
#)
#
#add_custom_command(TARGET GateServer POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${PROJECT_SOURCE_DIR}/bin/libjsoncpp.dll"
#        $<TARGET_FILE_DIR:GateServer>/libjsoncpp.dll
#)

if (WIN32)
    message("This is Windows")
    target_link_libraries(GateServer PRIVATE ws2_32 mswsock)
endif ()

set(Boost_DIR "D:/.vcpkg-clion/vcpkg/installed/x64-windows/share/boost")
find_package(Boost CONFIG REQUIRED)
target_link_libraries(GateServer PRIVATE Boost::boost)

find_package(gRPC CONFIG REQUIRED)
target_link_libraries(GateServer PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts)

find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(GateServer PRIVATE JsonCpp::JsonCpp)