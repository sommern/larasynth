This documentation is intended for any potential future developers of Larasynth.

## Windows Port

When porting to Windows, some libraries were Unix specific, and therefore had to be switched over to the cross platform versions. As of right now, all of these have been ported over (to the best of my knowledge), but needing to use cross platform libraries (or having alternatives for non Unix systems) will be something that will be continuous throughout development from now on.

### A quick word on compilers

I originally chose [MinGW](https://code.visualstudio.com/docs/cpp/config-mingw) for compiling on Windows, but there was a bug associated with compiling using the `-O2` flag using that, so I pivoted to using MSVC. This is a bit less ideal since the process for building is a bit different as a result of it not being GCC (whereas MinGW is a GCC port for Windows).

## Automake

Larasynth previously used Automake as its build system, but we switched over to CMake. 

The old Automake files are all just hanging out in the project tree, mostly for (relatively easy) future reference. If you wanted to, you could probably try building using them, but I would recommend against it.

Additionally, when I started the process of switching from Automake to CMake, I ran some test builds and sent their relevant outputs to log files (mostly, see more in the config.h section), which I stored in the `futuredevs/ref` directory.

## CMake

We switched over from Automake to CMake for the main reason of being able to run the Larasynth on Windows. There are a few outstanding issues/improvements to be made on this front.

### Outstanding Issues

These are the existing outstanding issues with CMake (and most of the ones with the project as a whole). This is not an exhaustive list, since there are probably some things that I've missed throughout this process. These are in no particular order, and I tried to be clear about the importance level of most of these issues.

#### gtest

The main thing not implemented in CMake right now is automated testing, which previously used gtest. I'd recommend just continuing to use gtest, and figuring out a way to get it working with CMake instead of switching to a new testing framework and having to rewrite all of the tests.

However, the current version of gtest in the [`lib`](lib/) directory utilizes some libraries that are Unix specific (ex - `unistd.h`), and would therefore have to either be ported over by modifying the gtest code to use the cross platform libraries, or you would have to start using a newer version of gtest that (hopefully) uses the cross platform libraries.

Note that there are CMakeLists.txt files in the `lib` and `tests` directories currently, but they don't have much of anything in them at the moment. They'll need `add_subdirectory()` in the CMakeLists.txt in the parent directory.

[This](https://cliutils.gitlab.io/modern-cmake/chapters/testing/googletest.html) looks like it would be a helpful resource for getting gtest working with CMake. 

#### config.h

Automake originally produced a config.h file that included some miscellanious information about the build configuration, namely the presence of certain header files, and version information. I never got a chance to look further into this, so it's possible that this is entirely not needed for CMake, or at least for what we're doing here. There's an example of one of these that was produced from a test build I ran when we were still using Automake in the `futuredevs/ref` directory.

I've added a `lara.hpp` file as a sort of equivalent for an easy spot for changing the version string. The only difference between changing it in the .hpp file and in the build configuration file (ie in CMake) is that it's in a different spot (as far as I can tell).

Here are some resources for this that I found:
- [Stackoverflow: generating a config.h in CMake like from automake](https://stackoverflow.com/questions/38419876/cmake-generate-config-h-like-from-autoconf)
- [Official CMake Documentation on config.h](https://cmake.org/cmake/help/v3.6/command/configure_file.html)

#### Linking JACK and CoreMidi (WIP)

This is by far the biggest outstanding issue, if it actually is an issue, since it affects the functioning (or non functioning) of the program. I didn't get the chance to 100% test Larasynth with an actual synthesizer before the semester ended, so it's possible that I've not done what I need to do in terms of linking the necessary external libraries/frameworks (ie JACK and CoreMidi). This would likely be found as a problem only when actually performing since JACK and CoreMidi would both only be used when RtMidi is being used, which would only be used when performing.

In other words, you might have to separately link the libraries for JACK and CoreMidi to the lara executable (make sure to check your CMAKE_SYSTEM/platform first).

With that being said, at the very least the CMake scripting for checking for these should be correct.

### Resources

This is just a short list of resources I found helpful when switching to CMake.

- [Official CMake Docs](https://cmake.org/cmake/help/latest/)
- [Modern CMake](https://cliutils.gitlab.io/modern-cmake/)
- [CMake Wiki](https://gitlab.kitware.com/cmake/community/-/wikis/home)

These are some miscellanious resources (mostly Stackoverflow posts) I found when figuring out how to do specific things (if they haven't been mentioned already in this documentation).

- [Stackoverflow: Converting Autotools project to CMake project](https://stackoverflow.com/questions/7132862/how-do-i-convert-an-autotools-project-to-a-cmake-project)
- [Stackoverflow: Why we use add_compile_options()](https://stackoverflow.com/questions/60622643/why-my-cmake-does-not-add-cmake-cxx-flags-debug-to-cmake-cxx-flags)
- [Stackoverflow: Configuring Pthreads](https://stackoverflow.com/questions/1620918/cmake-and-libpthread)
- [Writing platform checks](https://gitlab.kitware.com/cmake/community/-/wikis/doc/tutorials/How-To-Write-Platform-Checks)
- [Stackoverflow: Unsetting a compiler option](https://stackoverflow.com/questions/32945670/set-compiler-option-to-default-in-cmake)
- [Stackoverflow: Checking C++ version](https://stackoverflow.com/questions/10984442/how-to-detect-c11-support-of-a-compiler-with-cmake)
- [Stackoverflow: Checking generator](https://stackoverflow.com/questions/26836361/check-if-generating-a-visual-studio-solution-or-makefile-from-cmake)
- [Stackoverflow: Why use find_package instead of find_library?](https://stackoverflow.com/questions/23832339/package-vs-library)
- [Install command](https://cliutils.gitlab.io/modern-cmake/chapters/install/installing.html)

## GUI

Making a GUI was the original (main) intention of this project, hence my fork name. This didn't pan out due to technical problems with QT (more specifically with QT and CMake). All of the files that I'm going to talk about that are related to this are in the `futuredevs/gui` directory. Some quick definitions: QT is the GUI library and its associated programs, QT Designer is a program that allows you to make GUI mockups, and QT Creator is a program that allows you to use a drag and drop interface to create a GUI using QT and integrate that with your codebase.

I made mockups of what a possible GUI for modifying the config files could look like using QT Designer, which produces .ui files. These are in the `/designer` subdirectory in .ui and .png formats. I originally tried importing these directly into QT Creator, which produced the incorrect C++ code, which is understandable. I also tried manually 1:1 recreating the GUI from the .ui files in QT Creator, which also didn't work because QT Creator was confused by my CMake configuration (I think). I tried messing with the project folder (ie which folder to use as the parent directory of the project), but no such luck. I think the problem with QT Creator is that it's intended to be your IDE for your project from the beginning, and that just wasn't feasible here.

I also had problems with getting CMake to even work with the test GUI file on its own (ie in the larasynth directory, but without QT Creator). It hypothetically should've built, but it couldn't find the necessary QT modules for it.

The `CMakeLists.txt` file that I was messing with is also in the directory, along with the test `gui.cpp` file.

### A note on using QT

While I had lots of technical problems with QT, that shouldn't discourage you from using it! I still highly recommend it for making a cross platform GUI, even if it didn't work out here. 

With that being said, I'm convinced that it's more trouble than it's worth to get an already existing (relatively large) codebase to work with QT Creator and an already existing set of CMakeLists.txt files. I think that the solution to this is to just manually write the code, which I really don't think is as bad as it sounds. You could also build a GUI in a standalone QT Creator project, copy the code from that into this, and connect the GUI elements manually, as opposed to using QT Creator for that part. This doesn't alleviate the CMake problem, but that problem was some silly syntax thing that I was doing, or a version conflict, or something silly like that.

### Resources
- [Download QT](https://wiki.qt.io/Building_Qt_5_from_Git#Getting_the_source_code)
    - Note that I'd recommend using your package manager for this and build from source as a last resort, as it gave me a lot of trouble.
- [QT Documentation on using it with CMake](https://doc.qt.io/qt-6/cmake-manual.html)
- [This was a helpful command for using QT with CMake](https://doc.qt.io/qt-6/qt-standard-project-setup.html)
