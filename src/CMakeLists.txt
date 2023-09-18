cmake_minimum_required(VERSION 3.19)
project(larasynth)

target_include_directories(${PROJECT_SOURCE_DIR})
file(GLOB SOURCES CONFIGURE_DEPENDS *.cpp *.hpp *.h) # ask nathan about this
target_sources()