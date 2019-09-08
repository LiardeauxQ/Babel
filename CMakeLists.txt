cmake_minimum_required(VERSION 3.14.5)

project(babel)

add_definitions("-std=c++17")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(babel
    main.cpp
)

target_link_libraries(babel Qt5::Widgets)