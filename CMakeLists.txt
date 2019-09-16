cmake_minimum_required(VERSION 3.14.5)

project(babel-client)
project(babel-server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(Qt5_DIR "/usr/local/Cellar/qt/5.13.1/lib/cmake/Qt5")
endif()

include_directories("${CMAKE_HOME_DIRECTORY}/.conan/data")
message("HOME DIRECTORY ${CMAKE_HOME_DIRECTORY}")

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

set(SERVER_PATH "${CMAKE_SOURCE_DIR}/Server")
set(CLIENT_PATH "${CMAKE_SOURCE_DIR}/Client")
set(COMMON_PATH "${CMAKE_SOURCE_DIR}/Common")

message("CLIENT PATH: ${CLIENT_PATH}")

set(BABEL_SERVER
    ${SERVER_PATH}/main.cpp
)

set(BABEL_CLIENT
    ${CLIENT_PATH}/main.cpp
    ${CLIENT_PATH}/AudioController.cpp
    ${CLIENT_PATH}/AudioController.hpp
    ${CLIENT_PATH}/SoundStream.cpp
    ${CLIENT_PATH}/SoundStream.hpp
	${CLIENT_PATH}/OggReader.cpp
	${CLIENT_PATH}/OggReader.hpp
	${CLIENT_PATH}/ISoundFormatReader.cpp
	${CLIENT_PATH}/ISoundFormatReader.hpp
)

set(BABEL_COMMON
    ""
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
find_package(boost)
find_package(sqlite3)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

target_link_libraries(babel-client portaudio::portaudio opus::opus boost::boost sqlite3::sqlite3 Qt5::Widgets)

target_link_libraries(babel-server boost::boost sqlite3::sqlite3)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	target_link_libraries(babel-client jack asound)
endif()
