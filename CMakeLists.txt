cmake_minimum_required(VERSION 3.14.5)
set(CMAKE_CXX_STANDARD 17)

project(babel-client)
project(babel-server)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories("${CMAKE_HOME_DIRECTORY}/.conan/data")
include_directories("./Common")
include_directories("./Client")
include_directories("./Server")

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

set(SERVER_PATH "${CMAKE_SOURCE_DIR}/Server")
set(CLIENT_PATH "${CMAKE_SOURCE_DIR}/Client")
set(COMMON_PATH "${CMAKE_SOURCE_DIR}/Common")

set(BABEL_SERVER
        ${SERVER_PATH}/main.cpp
        ${SERVER_PATH}/ServerApplication.hpp
        ${SERVER_PATH}/ServerApplication.cpp
        ${SERVER_PATH}/ServerError.hpp
        ${SERVER_PATH}/ServerError.cpp
        )

set(BABEL_CLIENT
        ${CLIENT_PATH}/main.cpp
        ${CLIENT_PATH}/AudioController.cpp
        ${CLIENT_PATH}/AudioController.hpp
        ${CLIENT_PATH}/AudioControllerError.hpp
        ${CLIENT_PATH}/AudioControllerError.cpp
        ${CLIENT_PATH}/SoundManager.hpp
        ${CLIENT_PATH}/SoundManager.cpp
        )

set(BABEL_COMMON
        ${COMMON_PATH}/User.hpp
        ${COMMON_PATH}/User.cpp
        ${COMMON_PATH}/Database.hpp
        ${COMMON_PATH}/Database.cpp
        )

add_executable(babel-client
        ${BABEL_CLIENT}
        ${BABEL_COMMON}
        )

add_executable(babel-server
        ${BABEL_SERVER}
        ${BABEL_COMMON}
        )

find_package(portaudio)
find_package(opus)
find_package(boost_asio)
find_package(boost_circular_buffer)
find_package(sqlite3)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

target_link_libraries(babel-client portaudio::portaudio opus::opus boost_circular_buffer::boost_circular_buffer sqlite3::sqlite3 Qt5::Widgets)

target_link_libraries(babel-server sqlite3::sqlite3 boost_asio::boost_asio)

if (UNIX)
    target_link_libraries(babel-client jack asound)
endif ()
