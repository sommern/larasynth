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

#include "midi_file_reader.hpp"

using namespace std;
using namespace larasynth;


MidiFileReader::MidiFileReader( const string& midi_filename,
                                const vector<event_data_t>& ctrls )
  : _midi_filename( midi_filename )
  , _ctrls( ctrls )
  , _track_count( 0 )
  , _note_on_count( 0 )
  , _ctrl_change_count( 0 )
{
  if( !_midifile.read( _midi_filename ) )
    throw MidiFileReaderException("Error reading from " + _midi_filename);

  _midifile.doTimeAnalysis();

  _track_count = _midifile.getTrackCount();

  _midifile.joinTracks();

  MidiEventList& event_list = _midifile[0];

  Event event_to_write;

  for( size_t event_i = 0; (int)event_i < event_list.size(); ++event_i ) {
    MidiMessage& file_event = event_list[event_i];

    // convert time to microseconds
    size_t time = _midifile.getTimeInSeconds( 0, event_i ) * 1000000;

    if( file_event.isNoteOn() ) {
      event_to_write.set_note_on( 0, file_event.getKeyNumber(),
                                  file_event.getVelocity(), time );
      _events.push_back( event_to_write );
      ++_note_on_count;
    }
    else if( file_event.isNoteOff() ) {
      event_to_write.set_note_off( 0, file_event.getKeyNumber(),
                                   file_event.getVelocity(), time );
      _events.push_back( event_to_write );
    }
    else if( file_event.isController() ) {
      event_data_t ctrl_num = file_event.getControllerNumber();
      event_data_t ctrl_val = file_event.getControllerValue();

      // only write a controller event if the controller number is in ctrls
      if( find( ctrls.begin(), ctrls.end(), ctrl_num ) != ctrls.end() ) {
        event_to_write.set_ctrl( 0, ctrl_num, ctrl_val, time );
        _events.push_back( event_to_write );
        ++_ctrl_change_count;
      }
    }
  }
}
