cmake_minimum_required(VERSION 3.14.5)

project(babel-client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
message(${CMAKE_BINARY_DIR})

add_executable(babel-client
        main.cpp
        AudioController.cpp
        AudioController.hpp
)

find_package(portaudio)
find_package(opus)
find_package(boost)
find_package(sqlite3)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

target_link_libraries(babel-client portaudio::portaudio opus::opus boost::boost sqlite3::sqlite3)
target_link_libraries(babel-client Qt5::Widgets)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	target_link_libraries(babel-client jack asound)
endif()
