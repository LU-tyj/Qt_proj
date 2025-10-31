cmake_minimum_required(VERSION 3.31)
project(GameHub)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/Users/touyijun/Qt/6.9.2/macos/lib")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(GameHub main.cpp
        mainwindow.cpp
        mainwindow.h)
target_link_libraries(GameHub
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

