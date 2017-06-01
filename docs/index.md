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

You will need GCC and make installed to build Larasynth.

Download the latest source from here:

[https://github.com/sommern/larasynth/releases](https://github.com/sommern/larasynth/releases)

Extract from the tarball, build, and install (replace X.Y.Z with the version
that you downloaded):

```nohighlight
$ tar zxf larasynth-X.Y.Z.tar.gz
$ cd larasynth-X.Y.Z
$ ./configure
$ make
$ sudo make install
```

## Using Larasynth

See the [tutorial](tutorial.md) and the [topics guide](topics.md) for details
on using Larasynth.
