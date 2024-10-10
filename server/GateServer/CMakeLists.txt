cmake_minimum_required(VERSION 3.29)
project(GateServer)

set(CMAKE_CXX_STANDARD 20)

add_executable(GateServer Sources/main.cpp
        Sources/CServer.cpp
        Sources/Headers/CServer.h
        Sources/Headers/const.h
        Sources/HttpConnection.cpp
        Sources/Headers/HttpConnection.h
        Sources/Headers/Singleton.h
        Sources/LogicSystem.cpp
        Sources/Headers/LogicSystem.h)

target_include_directories(GateServer PRIVATE
        ${PROJECT_SOURCE_DIR}/Sources/Headers
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/boost
        ${PROJECT_SOURCE_DIR}/include/json
)

file(GLOB BOOST_LIBRARIES "${PROJECT_SOURCE_DIR}/lib/boostlib/*.lib")
file(GLOB JSONCPP_LIBRARIES "${PROJECT_SOURCE_DIR}/lib/jsonlib/libjsoncpp.dll.a")

target_link_libraries(GateServer PRIVATE
        ${JSONCPP_LIBRARIES}
)

add_custom_command(TARGET GateServer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/bin/libjsoncpp.dll"
        $<TARGET_FILE_DIR:GateServer>/libjsoncpp.dll
)

#set(BOOST_ROOT "D:/cpp_soft/boost_1_86_0") # 修改你本地的boost路径
#set(BOOST_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include") # 修改你本地的boost路径
#set(BOOST_LIBRARY_DIR "${BOOST_ROOT}/lib")
#file(GLOB BOOST_LIBRARIES "${BOOST_LIBRARY_DIR}/*.lib")
#set(JSONCPP_ROOT "D:/cpp_soft/jsoncpp") # 修改你本地的JSONCPP路径
#set(JSONCPP_INCLUDE_DIR "${JSONCPP_ROOT}/include") # 修改你本地的JSONCPP路径
#set(JSONCPP_LIBRARY_DIR "${JSONCPP_ROOT}/lib")
#set(JSONCPP_LIBRARIES "${JSONCPP_LIBRARY_DIR}/libjsoncpp.dll.a")
#
#find_package(Boost 1.86.0 REQUIRED)
#if (BOOST_FOUND)
#    message(STATUS "Boost include dir: ${BOOST_INCLUDE_DIR}")
#    message(STATUS "Boost lib dir:" ${BOOST_LIBRARY_DIR})
#    target_include_directories(GateServer PRIVATE ${BOOST_INCLUDE_DIR})
#    # target_link_libraries(GateServer PRIVATE ${BOOST_LIBRARIES})
#else ()
#    message(FATAL_ERROR "Boost not found")
#endif ()
#
#find_package(JSONCPP 1.9.6 REQUIRED)
#if (JSONCPP_FOUND)
#    message(STATUS "JsonCpp include dir: ${JSONCPP_INCLUDE_DIR}")
#    message(STATUS "JsonCpp library dir: ${JSONCPP_LIBRARY_DIR}")
#    target_include_directories(GateServer PRIVATE ${JSONCPP_INCLUDE_DIR})
#    target_link_libraries(GateServer PRIVATE ${JSONCPP_LIBRARIES})
#else ()
#    message(FATAL_ERROR "jsoncpp not found")
#endif ()

if (WIN32)
    message("This is Windows")
    target_link_libraries(GateServer PRIVATE ws2_32 mswsock)
endif ()

#include(FetchContent)
#
#FetchContent_Declare(
#        gRPC
#        GIT_REPOSITORY git@github.com:grpc/grpc.git
#        GIT_TAG master
#)
#
#set(FETCHCONTENT_QUIET OFF)
#FetchContent_MakeAvailable(gRPC)
#target_link_libraries(GateServer PRIVATE gRPC::grpc++)
