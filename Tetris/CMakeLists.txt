cmake_minimum_required(VERSION 3.31)
project(Tetris)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/Users/touyijun/Qt/6.9.2/macos/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Tetris main.cpp
        Widget.cpp
        Widget.h)
target_link_libraries(Tetris
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

