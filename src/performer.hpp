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

#pragma once

#include <vector>
#include <deque>
#include <unordered_map>
#include <algorithm>
#include <csignal>
#include <unistd.h>

#include "midi_client.hpp"
#include "event_pool.hpp"
#include "midi_config.hpp"
#include "midi_state.hpp"
#include "midi_translator.hpp"
#include "representation_config.hpp"
#include "lstm_network.hpp"
#include "time_utilities.hpp"

namespace larasynth {

class Performer {
public:
  Performer( MidiClient* midi_client, LstmNetwork& network,
             MidiConfig& midi_config, RepresentationConfig& repr_config,
             MidiMinMax& min_max, volatile sig_atomic_t* shutdown_flag,
             bool verbose );

private:
  ctrl_values_t get_ctrl_values_from_network( MidiTranslator& translator );
  void set_ctrls( ctrl_values_t& old_vals, ctrl_values_t& new_vals );
  void play_notes( std::deque<Event*>& notes_to_play );
  void forward_events( std::deque<Event*>& events_to_forward );

  void return_events( std::deque<Event*>& events );

  MidiClient* _midi_client;
  LstmNetwork& _network;

  MidiState _midi_state;

  EventPool _event_pool;
  
  std::vector<event_data_t> _ctrls;

  std::vector<double> _net_input;

  volatile sig_atomic_t* _shutdown_flag;

  bool _verbose;
  std::unordered_map<Event*, Timer> _timers;
};

}
