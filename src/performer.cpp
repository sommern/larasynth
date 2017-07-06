/*
Copyright 2016 Nathan Sommer

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

#include "performer.hpp"

using namespace std;
using namespace larasynth;
using namespace littlelstm;

Performer::Performer( MidiClient* midi_client, LstmNetwork& network,
                      MidiConfig& midi_config,
                      RepresentationConfig& repr_config,
                      MidiMinMax& min_max,
                      volatile sig_atomic_t* shutdown_flag, bool verbose )
  : _midi_client( midi_client )
  , _network( network )
  , _ctrls( midi_config.get_ctrls() )
  , _shutdown_flag( shutdown_flag )
  , _verbose( verbose )
{
  MidiTranslator translator( repr_config.get_ctrl_output_counts(),
                             repr_config.get_input_feature_config(),
                             midi_config.get_ctrl_defaults(),
                             min_max, PERFORM );

  Event* event;

  ctrl_values_t current_ctrl_vals;  
  ctrl_values_t new_ctrl_vals;

  size_t period = MICROSECONDS_PER_SECOND / repr_config.get_update_rate();

  cout << "Performing with an update interval of " << period << " microseconds"
       << endl;

  size_t next_update_time = 0;

  deque<Event*> notes_to_play;
  deque<Event*> events_to_forward;

  _net_input = vector<double>( _network.get_input_size(), 0.0 );
  vector<Real_t> net_output( _network.get_output_size(), 0.0 );

  translator.fill_target( net_output );

  while( !*_shutdown_flag ) {
    while( _midi_client->has_input_event() ) {
      event = _midi_client->get_input_event();

      if( event->type() == NOTE_ON || event->type() == NOTE_OFF ) {
        if( _verbose ) {
          _timers[event] = Timer();
        }

        notes_to_play.push_back( event );
        translator.report_note_event( event );
        new_ctrl_vals = get_ctrl_values_from_network( translator );
      }
      // an event that is not a note event, and not one of the controllers
      // larasynth is controlling, should be forwarded on
      else if( !( event->type() == CTRL_CHANGE &&
                  find( _ctrls.begin(), _ctrls.end(), event->controller() )
                  != _ctrls.end() ) ) {
        events_to_forward.push_back( event );
      }
      else {
        _midi_client->return_input_event( event );
      }
    }

    if( !notes_to_play.empty() ) {
      next_update_time = current_microseconds() + period;

      set_ctrls( current_ctrl_vals, new_ctrl_vals );
      play_notes( notes_to_play );
    }

    if( !events_to_forward.empty() ) {
      forward_events( events_to_forward );
    }

    if( current_microseconds() < next_update_time ) {
      usleep(1);
    }
    else {
      next_update_time = current_microseconds() + period;
      new_ctrl_vals = get_ctrl_values_from_network( translator );
      set_ctrls( current_ctrl_vals, new_ctrl_vals );
    }
  }

  cout << endl << "Shutting down." << endl;
}

ctrl_values_t
Performer::get_ctrl_values_from_network( MidiTranslator& translator ) {
  translator.fill_input( _net_input, OUTPUT_SOURCE );

  _network.feed_forward( _net_input );

  vector<double> net_output = _network.get_output();

  translator.report_output( net_output );

  return translator.get_output_ctrl_values();
}


void Performer::return_events( deque<Event*>& events ) {
  while( !events.empty() ) {
    _event_pool.return_event( events.front() );
    events.pop_front();
  }
}

void Performer::play_notes( deque<Event*>& notes_to_play ) {
  while( !notes_to_play.empty() ) {
    Event* note = notes_to_play.front();
    _midi_client->send_event( note );

    if( _verbose ) {
      double latency = _timers[note].get_elapsed_milliseconds();
      _timers.erase( note );

      cout << "note " << (unsigned int)note->pitch();

      if( note->type() == NOTE_ON && note->velocity() != 0 )
        cout << " on" << endl;
      else
        cout << " off" << endl;

      cout << "Latency: " << latency << " ms" << endl;
    }

    _midi_client->return_input_event( note );
    notes_to_play.pop_front();
  }
}

void Performer::forward_events( deque<Event*>& events_to_forward ) {
  while( !events_to_forward.empty() ) {
    Event* event = events_to_forward.front();
    _midi_client->send_event( event );
    _midi_client->return_input_event( event );
    events_to_forward.pop_front();
  }
}

void
Performer::set_ctrls( unordered_map<event_data_t,event_data_t>& old_vals,
                      unordered_map<event_data_t,event_data_t>& new_vals ) {
  Event ctrl_event;

  for( auto ctrl : _ctrls ) {
    if( new_vals[ctrl] != old_vals[ctrl] ) {
      old_vals[ctrl] = new_vals[ctrl];
      ctrl_event.set_ctrl( 0, ctrl, new_vals[ctrl], 0 );
      _midi_client->send_event( &ctrl_event );
      if( _verbose ) {
        cout << "set controller " << (unsigned int)ctrl << " to "
             << (unsigned int)new_vals[ctrl] << endl;
      }
    }
  }
}
