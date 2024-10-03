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

set(BOOST_ROOT "D:/cpp_soft/boost_1_86_0") # 修改你本地的boost路径
set(BOOST_INCLUDE_DIR "${BOOST_ROOT}/include") # 修改你本地的boost路径
set(BOOST_LIBRARY_DIR "${BOOST_ROOT}/lib")
file(GLOB BOOST_LIBRARIES "${BOOST_LIBRARY_DIR}/*.lib")
set(JSONCPP_ROOT "D:/cpp_soft/jsoncpp") # 修改你本地的JSONCPP路径
set(JSONCPP_INCLUDE_DIR "${JSONCPP_ROOT}/include") # 修改你本地的JSONCPP路径
set(JSONCPP_LIBRARY_DIR "${JSONCPP_ROOT}/lib")
set(JSONCPP_LIBRARIES "${JSONCPP_LIBRARY_DIR}/libjsoncpp.dll.a")

#find_package(Boost 1.86.0 REQUIRED)
#if (BOOST_FOUND)
#    set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
#    message(BOOST_INCLUDE_DIR "${BOOST_INCLUDEDIR}")
#    message(BOOST_LIBRARY_DIR "${BOOST_LIBRARYDIR}")
#    target_include_directories(GateServer ${BOOST_INCLUDEDIR})
#    target_link_directories(GateServer ${BOOST_LIBRARYDIR})
#else()
#    message(FATAL_ERROR "Boost not found")
#endif (BOOST_FOUND)
#
#find_package(JSONCPP 1.9.6 REQUIRED)
#if(JSONCPP_FOUND)
#    set(JSONCPP_LIBRARY "${JSONCPP_ROOT}/lib/libjsoncpp.dll.a")
#    message("found JSONCPP")
#    target_include_directories(${JSONCPP_INCLUDEDIR})
#    target_link_libraries(${JSONCPP_LIBRARY})
#else (JSONCPP_FOUND)
#    message(FATAL_ERROR "jsoncpp not found")
#endif (JSONCPP_FOUND)

find_package(Boost 1.86.0 REQUIRED)
if(BOOST_FOUND)
    message(STATUS "Boost include dir: ${BOOST_INCLUDE_DIR}")
    message(STATUS "Boost lib dir:" ${BOOST_LIBRARY_DIR})
    target_include_directories(GateServer PRIVATE ${BOOST_INCLUDE_DIR})
    # target_link_libraries(GateServer PRIVATE ${BOOST_LIBRARIES})
else ()
    message(FATAL_ERROR "Boost not found")
endif ()

find_package(JSONCPP 1.9.6 REQUIRED)
if(JSONCPP_FOUND)
    message(STATUS "JsonCpp include dir: ${JSONCPP_INCLUDE_DIR}")
    message(STATUS "JsonCpp library dir: ${JSONCPP_LIBRARY_DIR}")
    target_include_directories(GateServer PRIVATE ${JSONCPP_INCLUDE_DIR})
    target_link_libraries(GateServer PRIVATE ${JSONCPP_LIBRARIES})
else()
    message(FATAL_ERROR "jsoncpp not found")
endif()

if(WIN32)
    message("This is Windows")
    target_link_libraries(GateServer PRIVATE ws2_32 mswsock)
endif ()

#get_cmake_property(_variableNames VARIABLES)
#foreach(_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

