This documentation is intended for any potential future developers of Larasynth.

TODO: maybe move the /ref dir and /gui dir and any automake stuff to a /futuredevs dir in /docs? If doing that then remove the CMakeLists.txts for those.

## Windows Port

When porting to Windows, some libraries were Unix specific, and therefore had to be switched over to the cross platform versions. As of right now, all of these have been ported over (to the best of my knowledge), but needing to use cross platform libraries (or having alternatives for non Unix systems) will be something that will be continuous throughout development from now on.

### A quick word on compilers

I originally chose [Mingw](https://code.visualstudio.com/docs/cpp/config-mingw) for compiling on Windows, but there was a bug associated with compiling using the `-O2` flag using that, so I pivoted to using MSVC. This is a bit less ideal since the process for building is a bit different as a result of it not being GCC (whereas Mingw is a GCC port for Windows).

## CMake

We switched over from Automake to CMake for the main reason of being able to run the Larasynth on Windows. There are a few outstanding issues/improvements to be made on this front.

### Outstanding Issues

These are the existing outstanding issues with CMake (and most of the ones with the project as a whole). This is not an exhaustive list, since there are probably some things that I've missed throughout this process. These are in no particular order, and I tried to be clear about the importance level of most of these issues.

#### Build Types 

For some reason, CMake refuses to change the build type on Windows. This presents a problem as the default build type is "Debug," which sets, among many other compiler flags we don't want, O0. I've brute forced also setting O2 which alleviates this problem, but there's a distinct possiblity that this might cause more bugs later down the line.

For reference, you can change the build type of a CMake run by adding the following flag when running it:

```nohighlight
-DCMAKE_BUILD_TYPE=Release
```

By "refuses to change the build type," I mean that when specifying the build type, CMake outputs the following message:

```nohighlight
CMake Warning:
  Manually-specified variables were not used by the project:

    CMAKE_BUILD_TYPE
```

It should, at the very least, be using the build type to determine what compiler flags are being used, so this shouldn't be the case.

#### gtest

The main thing not implemented in CMake right now is automated testing, which previously used gtest. I'd recommend just continuing to use gtest, and figuring out a way to get it working with CMake instead of switching to a new testing framework and having to rewrite all of the tests.

However, the current version of gtest in the [`lib`](lib/) directory utilizes some libraries that are Unix specific (ex - `unistd.h`), and would therefore have to either be ported over by modifying the gtest code to use the cross platform libraries, or you would have to start using a newer version of gtest that (hopefully) uses the cross platform libraries.

Note that there are CMakeLists.txt files in the `lib` and `tests` directories currently, but they don't have much of anything in them at the moment. They'll need `add_subdirectory()` in the CMakeLists.txt in the parent directory.

[This](https://cliutils.gitlab.io/modern-cmake/chapters/testing/googletest.html) looks like it would be a helpful resource for getting gtest working with CMake. 

#### Config.h

Automake originally produced a config.h file that included some miscellanious information, namely the presence of certain header files, and version information. I never got a chance to look further into this, so it's possible that this is entirely not needed for CMake, or at least for what we're doing here.

Here are some resources for this that I found:
- [Stackoverflow: generating a config.h in CMake like from automake](https://stackoverflow.com/questions/38419876/cmake-generate-config-h-like-from-autoconf)
- [Official CMake Documentation on config.h](https://cmake.org/cmake/help/v3.6/command/configure_file.html)

#### make install (WIP)

#### Linking JACK and CoreMidi (WIP)

I didn't get the chance to 100% test Larasynth with an actual synthesizer before the semester ended, so it's possible that I've not done what I need to do in terms of linking the necessary external libraries/frameworks (ie JACK and CoreMidi). This would likely be found to be a problem only when actually performing as JACK and CoreMidi would both only be used when RtMidi is being used, which would only be used when performing.

However, when I check for these I think the code should be correct.

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

## GUI (WIP)



### Resources

- [This was a helpful command for using QT with CMake](https://doc.qt.io/qt-6/qt-standard-project-setup.html)
