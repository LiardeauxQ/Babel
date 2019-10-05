cmake_minimum_required(VERSION 3.14.5)

set(CMAKE_CXX_STANDARD 17)

project(babel-client)
project(babel-server)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set compiler arguments
if (NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17 -Wno-unused")
endif ()

# Set Qt specific information
if (APPLE)
    set(Qt5_DIR "/usr/local/Cellar/qt/5.13.1/lib/cmake/Qt5")
elseif (WIN32)
	set(CMAKE_PREFIX_PATH "C:\\Qt\\5.13.1\\msvc2017_64\\" ${CMAKE_PREFIX_PATH})
	set(GUI_TYPE WIN32)
endif (APPLE)

# Include conan headers

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup()
else()
	message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

if (UNIX)
	include_directories("${CMAKE_HOME_DIRECTORY}/.conan/data")
endif ()

set(SERVER_PATH "${CMAKE_SOURCE_DIR}/Server")
set(CLIENT_PATH "${CMAKE_SOURCE_DIR}/Client")
set(COMMON_PATH "${CMAKE_SOURCE_DIR}/Common")

# Include headers
include_directories(${SERVER_PATH})
include_directories(${COMMON_PATH})
include_directories(${CLIENT_PATH})

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})

set(BABEL_SERVER
    ${SERVER_PATH}/main.cpp
    ${SERVER_PATH}/ServerApplication.hpp
    ${SERVER_PATH}/ServerApplication.cpp
    ${SERVER_PATH}/ServerConfig.cpp
    ${SERVER_PATH}/ServerConfig.hpp
    ${SERVER_PATH}/ServerError.hpp
    ${SERVER_PATH}/ServerError.cpp
    ${SERVER_PATH}/Session.hpp
    ${SERVER_PATH}/Session.cpp
    ${SERVER_PATH}/SharedData.hpp)

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
	${CLIENT_PATH}/opus/Encoder.cpp
	${CLIENT_PATH}/opus/Encoder.hpp
	${CLIENT_PATH}/opus/Decoder.cpp
	${CLIENT_PATH}/opus/Decoder.hpp
	${CLIENT_PATH}/opus/Opus.hpp
	${CLIENT_PATH}/ui/LoginWidget.cpp
	${CLIENT_PATH}/ui/LoginWidget.hpp
	${CLIENT_PATH}/ui/RegisterWidget.cpp
	${CLIENT_PATH}/ui/RegisterWidget.hpp
	${CLIENT_PATH}/ui/BabelMainWindow.cpp
	${CLIENT_PATH}/ui/BabelMainWindow.hpp
	${CLIENT_PATH}/ui/FriendListWidget.cpp
	${CLIENT_PATH}/ui/FriendListWidget.hpp
	${CLIENT_PATH}/ui/CallWidget.cpp
	${CLIENT_PATH}/ui/CallWidget.hpp
	${CLIENT_PATH}/ui/WidgetsHandler.cpp
	${CLIENT_PATH}/ui/WidgetsHandler.hpp
    ${CLIENT_PATH}/protocol/ServerRequest.cpp
    ${CLIENT_PATH}/protocol/ServerRequest.hpp
	${CLIENT_PATH}/AppManager.cpp
	${CLIENT_PATH}/AppManager.hpp
	${CLIENT_PATH}/protocol/UserSession.hpp
	${CLIENT_PATH}/Subject.cpp
	${CLIENT_PATH}/Subject.hpp
	${CLIENT_PATH}/Observer.hpp
	${CLIENT_PATH}/NotificationHandler.cpp
	${CLIENT_PATH}/NotificationHandler.hpp
	Client/Observer.cpp
	Client/protocol/ServerCommunication.cpp
	Client/protocol/ServerCommunication.hpp
	Client/protocol/ServerHandler.cpp
	Client/protocol/ServerHandler.hpp
	Client/protocol/UserSession.cpp Client/ui/AddFriendWidget.cpp Client/ui/AddFriendWidget.hpp Client/protocol/ServerResponse.cpp Client/protocol/ServerResponse.hpp)

set(BABEL_COMMON
    ${COMMON_PATH}/User.hpp
    ${COMMON_PATH}/Database.hpp
    ${COMMON_PATH}/Database.cpp
    ${COMMON_PATH}/DatabaseError.hpp
    ${COMMON_PATH}/DatabaseError.cpp
    ${COMMON_PATH}/Message.hpp
    ${COMMON_PATH}/protocol.h
	${COMMON_PATH}/Packet.hpp)

add_executable(babel-client
    ${GUI_TYPE}
    ${BABEL_CLIENT}
    ${BABEL_COMMON})

add_executable(babel-server
    ${BABEL_SERVER}
    ${BABEL_COMMON})

# Find conan package + Qt5
find_package(portaudio)
find_package(opus)
find_package(boost_asio)
find_package(boost_circular_buffer)
find_package(sqlite3)
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

# Client link
target_link_libraries(babel-client
	portaudio::portaudio
	opus::opus
	boost_circular_buffer::boost_circular_buffer
    boost_asio::boost_asio
	sqlite3::sqlite3
	Qt5::Widgets)

# Server link
target_link_libraries(babel-server
	sqlite3::sqlite3
	boost_asio::boost_asio)

# Copy dll to the binary folder
if(WIN32)
    if ($<CONFIG:Debug>)
        get_target_property(WIDGETDLL Qt5::Widgets IMPORTED_LOCATION_DEBUG)
    else ($<CONFIG:Debug>)
        get_target_property(WIDGETDLL Qt5::Widgets IMPORTED_LOCATION_RELEASE)
	endif ($<CONFIG:Debug>)

	add_custom_command (
    	TARGET babel-client POST_BUILD
    	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:babel-client>
    	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:babel-client>
    	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:babel-client>
	)
endif(WIN32)

# Link linux `asound` and `jack` library
if (UNIX AND NOT APPLE)
    target_link_libraries(babel-client
        jack
        asound)
endif ()
