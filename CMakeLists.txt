# https://stackoverflow.com/questions/7132862/how-do-i-convert-an-autotools-project-to-a-cmake-project

cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
project(larasynth
    LANGUAGES CXX # use C++
)

# TODO: https://stackoverflow.com/questions/38419876/cmake-generate-config-h-like-from-autoconf

# https://stackoverflow.com/questions/10984442/how-to-detect-c11-support-of-a-compiler-with-cmake
# might want the executable in src
# add_executable(lara lara.cpp)
# set_property(TARGET lara 
#     PROPERTY CXX_STANDARD 11 # require C++ 11
#     CXX_STANDARD_REQUIRED ON
# )

# https://stackoverflow.com/questions/60622643/why-my-cmake-does-not-add-cmake-cxx-flags-debug-to-cmake-cxx-flags
if(!Windows)
    add_compile_options(${TARGET_NAME} -fdiagnostics-color=auto -Wall -Wextra -Wpedantic -Wfatal-errors)
    add_compile_options(${TARGET_NAME} -Wcast-align -Wcast-qual -Wformat -Wpointer-arith -Wwrite-strings)
    add_compile_options(${TARGET_NAME} "$<$<CONFIG:Debug>:-g -O0 -fno-omit-frame-pointer -fno-inline>")
endif()

# might want to move these to the bottom 
# add_subdirectory(lib)
add_subdirectory(src)
# ignore tests for now
# add_subdirectory(tests)

# https://gitlab.kitware.com/cmake/community/-/wikis/doc/tutorials/How-To-Write-Platform-Checks
set(rtmidi_api, "")
if(Linux)
    # install jack
    find_library(jack jack_client_open REQUIRED)
    if(not jack)
        message(FATAL_ERROR "JACK support requires the jack library")
    endif()
    # install pthread
    # find_library(pthread pthread_create)
    # if(not jack)
    #     message(FATAL_ERROR "RtMidi requires the pthread library")
    # endif()
    set(rtmidi_api, "${rtmidi_api} -D__UNIX_JACK__")
endif()
if(Darwin)
    # check for coreMidi header
    check_include_file_cxx("CoreMIDI/CoreMIDI.h" have_coremidi REQUIRED)
    if(not have_coremidi)
        message(FATAL_ERROR "CoreMIDI header file not found")
    endif()
    set(rtmidi_api, "${rtmidi_api} -D__MACOSX_CORE__")
endif()

# https://stackoverflow.com/questions/1620918/cmake-and-libpthread
# configure pthreads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(lara PRIVATE Threads::Threads)

# TODO: gtest variables here

# message("Larasynth has been configured successfully!") # get this to print only when successful