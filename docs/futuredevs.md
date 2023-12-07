This documentation is intended for any potential future developers of Larasynth.

## Windows Port

When porting to Windows, some libraries were Unix specific, and therefore had to be switched over to the cross platform versions. As of right now, all of these have been ported over (to the best of my knowledge), but needing to use cross platform libraries (or having alternatives for non Unix systems) will be something that will be continuous throughout development from now on.

## CMake

We switched over from Automake to CMake for the main reason of being able to run the Larasynth on Windows. There are a few outstanding issues/improvements to be made on this front.

### Miscellanious issues



### gtest

The main thing not implemented in CMake right now is automated testing, which previously used gtest. I'd recommend just continuing to use gtest, and figuring out a way to get it working with CMake instead of switching to a new testing framework and having to rewrite all of the tests.

However, the current version of gtest in the [`lib`](lib/) directory utilizes some libraries that are Unix specific (ex - `unistd.h`), and would therefore have to either be ported over by modifying the gtest code to use the cross platform libraries, or start using a newer version of gtest that (hopefully) uses the cross platform libraries.

[This](https://cliutils.gitlab.io/modern-cmake/chapters/testing/googletest.html) looks like it would be a helpful resource for getting gtest working with CMake. 

### Resources

This is just a list of resources I found helpful when switching to CMake.

- [Modern CMake](https://cliutils.gitlab.io/modern-cmake/)

## GUI
https://doc.qt.io/qt-6/qt-standard-project-setup.html