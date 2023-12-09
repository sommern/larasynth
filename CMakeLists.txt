cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
project(larasynth
    LANGUAGES CXX # use C++
)

set(CMAKE_BUILD_TYPE "Release")

# remove the RTC1 flag if using MSVC
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    macro(RemoveDebugCXXFlag flag)
        string(REPLACE "${flag}" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    endmacro()
    macro(RemoveReleaseCXXFlag flag)
        string(REPLACE "${flag}" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    endmacro()

    RemoveDebugCXXFlag("/RTC1")
    RemoveReleaseCXXFlag("/RTC1")
endif()

# use these compiler options to clean up the output if not on Windows
if(NOT WIN32)
    add_compile_options(${TARGET_NAME} -fdiagnostics-color=auto -Wall -Wextra -Wpedantic -Wfatal-errors)
    add_compile_options(${TARGET_NAME} -Wcast-align -Wcast-qual -Wformat -Wpointer-arith -Wwrite-strings)
    add_compile_options(${TARGET_NAME} "$<$<CONFIG:Debug>:-g -O0 -fno-omit-frame-pointer -fno-inline>")
endif()

add_compile_options(${TARGET_NAME} "-O2") # optimization

add_subdirectory(src)

# make install
install(TARGETS lara laraLib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)

# find RtMidi
set(rtmidi_api "")
if(CMAKE_SYSTEM MATCHES "Linux") # Linux
    # check for jack
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(JACK REQUIRED jack)
    if(NOT JACK_FOUND)
        message(FATAL_ERROR "JACK support requires the jack library")
    endif()
    set(rtmidi_api, "${rtmidi_api} -D__UNIX_JACK__")
endif()
if(CMAKE_SYSTEM MATCHES "Darwin") # MacOS
    # check for coreMidi header
    check_include_file_cxx("CoreMIDI/CoreMIDI.h" have_coremidi REQUIRED)
    if(not have_coremidi)
        message(FATAL_ERROR "CoreMIDI header file not found")
    endif()
    set(rtmidi_api, "${rtmidi_api} -D__MACOSX_CORE__")
endif()

# configure pthreads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(lara PRIVATE Threads::Threads)