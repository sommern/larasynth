cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
project(larasynth
    LANGUAGES CXX # use C++
)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
# qt_standard_project_setup() # only used with Qt6

add_executable(gui gui.cpp)
set_property(TARGET gui 
    PROPERTY CXX_STANDARD 17 # require C++ 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(gui PRIVATE Qt5::Widgets)