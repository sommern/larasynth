# Larasynth

Larasynth stands for the **L**earning **A**uto-**R**egistrant for **A**udio
**Synth**esizers. Larasynth can learn to turn your synthesizer's knobs for you
as you play.

Registrants are assistants to organists. For complicated organ pieces that
require the organ's stops to be adjusted regularly during performance, the
registrant adjusts the stops while the organist plays the keyboard and pedals.

Larasynth is like a registrant for musicians who play digital synthesizers
controlled by MIDI controllers. Larasynth can be trained to adjust your
synthesizer's continuous controller parameters as you play, allowing
for expressive control of the synthesizer that you cannot achieve alone.

## Requirements

Larasynth is a terminal application, so it requires basic experience with the
Unix style command line and editing text files.

Larasynth is known to run on the following operating systems:

* Linux with [JACK](http://www.jackaudio.org/)
* macOS 10.9 (Mavericks) or later

You also need:

* A MIDI controller (such as a keyboard) that can communicate with your
computer.
* A synthesizer which can be controlled by MIDI. This can be a software
synthesizer or a hardware synthesizer that can connect to your computer.
* A MIDI sequencer or a digital audio workstation (DAW) in which you can edit
MIDI tracks.

## Installation

Larasynth can be installed with Homebrew on macOS. Linux users will need to
build Larasynth from source.

### macOS

Using Homebrew is the easiest way to install Larasynth and be able to update it
in the future. If you prefer, you can also download pre-compiled macOS binaries
[here](https://github.com/sommern/larasynth/releases).

If you do not have Homebrew installed, install it now. It is a painless
installation: [https://brew.sh/](https://brew.sh/)

Now you can install Larasynth using `brew`:

```nohighlight
$ brew tap sommern/homebrew-repo
$ brew install larasynth
```

### From Source

You will need GCC, make, and [CMake](https://cmake.org/download/) installed to build Larasynth. This process is heavily modified on Windows, and is described in the below section titled "Windows."

Download the latest source from here:

[https://github.com/sommern/larasynth/releases](https://github.com/sommern/larasynth/releases)

Extract from the tarball, build, and install (replace X.Y.Z with the version
that you downloaded):

```nohighlight
$ tar zxf larasynth-X.Y.Z.tar.gz
$ cd larasynth-X.Y.Z
$ mkdir build
$ cd build
# cmake ..
$ make
```

#### Windows

It is recommended that you use MSVC as your compiler when on Windows. You do not need GCC or make to build Larasynth on Windows, but you still do need CMake.

##### Setting up MSVC

Download MSVC [here](https://visualstudio.microsoft.com/downloads/#remote-tools-for-visual-studio-2022). Make sure to download the *Build Tools for Visual Studio 2022*. Select *Desktop development with C++* when in the installer and then click *Install*. Restart your computer, and in the command line, type:

```nohighlight
$ msbuild --version
```

This is to ensure that you have installed MSVC correctly. If you don't get something similar to the below output, you've done something wrong and need to start over:

```nohighlight
$ msbuild --version
MSBuild version 17.8.3+195e7f5a3 for .NET Framework
17.8.3.51904
```

##### Installing 

When installing from source on Windows, you will need to add a few command line arguments when running CMake.

```nohighlight
$ cmake .. -G "Visual Studio 17 2022"
```

Instead of running `make`, you will need to run `msbuild` on the .sln file produced by cmake:

```nohighlight
$ msbuild .\larasynth.sln -maxcpucount:8
```

The resulting .exe file will be in `build/src/Debug`.

If you choose to use a different compiler than MSVC, you will need to [change the generator used by CMake](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html). 

## Using Larasynth

See the [tutorial](tutorial.md) and the [topics guide](topics.md) for details
on using Larasynth.

## Contributors

- Nathan Sommer
    - [Github](https://github.com/sommern)

- Aaron Ripley
    - [Github](https://github.com/ripleya1)
