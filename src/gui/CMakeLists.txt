cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
project(larasynth
    LANGUAGES CXX # use C++
)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_executable(gui gui.cpp)
set_property(TARGET gui 
    PROPERTY CXX_STANDARD 11 # require C++ 11
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(gui PRIVATE Qt6::Widgets)