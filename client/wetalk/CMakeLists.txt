cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "D:/Qt/6.8.0/msvc2022_64")

add_compile_options("/Zc:__cplusplus")

project(wetalk)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}/Sources/Headers
)

file(GLOB CPP_FILES "Sources/*.cpp")
file(GLOB H_FILES "Sources/Headers/*.h")
file(GLOB UI_FILES "Sources/Forms/*.ui")

add_executable(wetalk ${H_FILES} ${CPP_FILES} ${UI_FILES} resources.qrc)

target_link_libraries(wetalk PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
get_filename_component(PWD ${CMAKE_SOURCE_DIR} ABSOLUTE)
get_filename_component(OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ABSOLUTE)
set(CONFIG_FILE ${PWD}/config.ini)

if (WIN32)
    add_custom_command(TARGET wetalk POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CONFIG_FILE}
            ${OUTPUT_DIR}/config.ini
    )

    add_custom_command(TARGET wetalk POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CONFIG_FILE}
            ${CMAKE_BINARY_DIR}/config.ini
    )
endif ()

# install(FILES ${CONFIG_FILE} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if(MSVC)
    message("This is MSVC")
    # 为MSVC设置调试和发布配置的运行时库
    foreach(build_type IN ITEMS RELEASE RELWITHDEBINFO MINSIZEREL DEBUG)
        string(TOUPPER ${build_type} upper_build_type)
        set(CMAKE_CXX_FLAGS_${upper_build_type} "${CMAKE_CXX_FLAGS_${upper_build_type}} /MDd")
    endforeach()
endif()

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
