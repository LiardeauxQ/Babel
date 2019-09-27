cmake_minimum_required(VERSION 3.14.5)

set(CMAKE_CXX_STANDARD 17)

project(babel-client)
project(babel-server)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (APPLE)
    set(Qt5_DIR "/usr/local/Cellar/qt/5.13.1/lib/cmake/Qt5")
endif ()

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup()
else()
	message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

include_directories("${CMAKE_HOME_DIRECTORY}/.conan/data")
include_directories("./Common")
include_directories("./Client")
include_directories("./Server")

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})

set(SERVER_PATH "${CMAKE_SOURCE_DIR}/Server")
set(CLIENT_PATH "${CMAKE_SOURCE_DIR}/Client")
set(COMMON_PATH "${CMAKE_SOURCE_DIR}/Common")

set(BABEL_SERVER
        ${SERVER_PATH}/main.cpp
        ${SERVER_PATH}/ServerApplication.hpp
        ${SERVER_PATH}/ServerApplication.cpp
        ${SERVER_PATH}/ServerConfig.cpp
        ${SERVER_PATH}/ServerConfig.hpp
        ${SERVER_PATH}/ServerError.hpp
        ${SERVER_PATH}/ServerError.cpp)

set(BABEL_CLIENT
	${CLIENT_PATH}/main.cpp
	${CLIENT_PATH}/AudioController.cpp
	${CLIENT_PATH}/AudioController.hpp
	${CLIENT_PATH}/AudioControllerError.hpp
	${CLIENT_PATH}/AudioControllerError.cpp
	${CLIENT_PATH}/SoundManager.hpp
	${CLIENT_PATH}/SoundManager.cpp
	${CLIENT_PATH}/Error.cpp
	${CLIENT_PATH}/Error.hpp
	${CLIENT_PATH}/Packet.hpp
	${CLIENT_PATH}/opus/Encoder.cpp
	${CLIENT_PATH}/opus/Encoder.hpp
	${CLIENT_PATH}/opus/Decoder.cpp
	${CLIENT_PATH}/opus/Decoder.hpp
	${CLIENT_PATH}/opus/Opus.hpp
	${CLIENT_PATH}/ui/LoginWidget.cpp
	${CLIENT_PATH}/ui/LoginWidget.hpp
	${CLIENT_PATH}/ui/RegisterWidget.cpp
	${CLIENT_PATH}/ui/RegisterWidget.hpp Client/ui/MainWindow.cpp Client/ui/MainWindow.hpp Client/ui/FriendListWidget.cpp Client/ui/FriendListWidget.hpp Client/ui/CallWidget.cpp Client/ui/CallWidget.hpp Client/ui/WidgetsHandler.cpp Client/ui/WidgetsHandler.hpp)

set(BABEL_COMMON
        ${COMMON_PATH}/User.hpp
        ${COMMON_PATH}/User.cpp
        ${COMMON_PATH}/Database.hpp
        ${COMMON_PATH}/Database.cpp
        ${COMMON_PATH}/DatabaseError.hpp
        ${COMMON_PATH}/DatabaseError.cpp)

add_executable(babel-client ${BABEL_CLIENT} ${BABEL_COMMON})

add_executable(babel-server ${BABEL_SERVER} ${BABEL_COMMON})

find_package(portaudio)
find_package(opus)
find_package(boost_asio)
find_package(boost_circular_buffer)
find_package(sqlite3)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

target_link_libraries(babel-client portaudio::portaudio opus::opus boost_circular_buffer::boost_circular_buffer sqlite3::sqlite3 Qt5::Widgets)

target_link_libraries(babel-server sqlite3::sqlite3 boost_asio::boost_asio)

if (UNIX AND NOT APPLE)
    target_link_libraries(babel-client jack asound)
endif ()
