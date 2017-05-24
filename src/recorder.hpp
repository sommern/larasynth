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
#include <fstream>
#include <csignal>
#include <chrono>
#include <thread>

#include "midi_config.hpp"
#include "config_directory.hpp"
#include "midi_client.hpp"
#include "write_training_example.hpp"
#include "run_modes.hpp"

namespace larasynth {

class Recorder {
public:
  Recorder( const std::string& filename,
            const std::vector<event_data_t>& ctrls,
            MidiClient* midi_client,
            volatile sig_atomic_t* shutdown_flag );

  void record();

private:
  std::string _filename;

  MidiClient* _midi_client;
  std::vector<event_data_t> _ctrls;

  volatile sig_atomic_t* _shutdown_flag;
};

}
