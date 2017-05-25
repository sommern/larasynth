/*
Copyright 2017 Nathan Sommer

This file is part of Larasynth.

Larasynth is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Larasynth is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Larasynth.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <iostream>

#include "midi_types.hpp"
#include "event.hpp"
#include "midifile/MidiFile.h"

namespace larasynth {


/**
 * Thrown if any errors are encountered when reading a MIDI file.
 */
class MidiFileReaderException : public std::runtime_error {
public:
  MidiFileReaderException( const std::string& message )
    : std::runtime_error( "Error reading MIDI file:\n" +
                          message ) {};
};


/**
 * Provides functionality for reading events from a standard MIDI file and
 * writing the relevent events to a training example file.
 */
class MidiFileReader {
public:
  /**
   * Constructor
   *
   * @param midi_filename Name of the file to read from
   * @param ctrls Vector of controller numbers. Events which alter these
   *              controllers will be saved, all others will be ignored
   * @param output_filename Name of the file to write the events to
   */
  MidiFileReader( const std::string& midi_filename,
                  const std::vector<event_data_t>& ctrls );

  size_t get_track_count() const { return _track_count; }
  size_t get_note_on_count() const { return _note_on_count; }
  size_t get_ctrl_change_count() const { return _ctrl_change_count; }
  std::vector<Event> get_events() const { return _events; }

private:
  std::string _midi_filename;
  std::vector<event_data_t> _ctrls;

  MidiFile _midifile;

  size_t _track_count;
  size_t _note_on_count;
  size_t _ctrl_change_count;
  std::vector<Event> _events;
};

}
