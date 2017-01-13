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
Unix style command line and with editing text files.

Larasynth is known to run on the following operating systems:

* Linux with [JACK](http://www.jackaudio.org/)
* OS X 10.9 (Mavericks) or later

You also need:

* A MIDI controller (such as a keyboard) that can communicate with your
computer.
* A synthesizer which can be controlled by MIDI. This can be a software
synthesizer or a hardware synthesizer that can hook up to your computer.
* A MIDI sequencer or a digital audio workstation (DAW) in which you can edit
MIDI tracks.

## Building Larasynth

Building Larasynth requires `gcc` (4.9 or later) and `make`.

To build and install Larasynth from the source code, clone this repository and
then run the following commands from within the `larasynth` directory:

```
./configure
make
sudo make install
```

## Using Larasynth

See the [user guide](https://larasynth.readthedocs.io/) for details on using
Larasynth.

## Feedback

Whether you love or hate Larasynth, I would love to hear about it! Reach me by
email: nsommer@wooster.edu

## Contributing

Report bugs and make feature requests through the GitHub issue tracker.

If you would like to contribute code to the project, create a pull request
through GitHub.
