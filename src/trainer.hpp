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

#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <string>

#include <thread>
#include <chrono>
#include <csignal>

#include "config_directory.hpp"
#include "littlelstm/lstm_network.hpp"
#include "littlelstm/lstm_architecture.hpp"
#include "training_sequence.hpp"
#include "training_event_stream.hpp"
#include "training_results.hpp"
#include "lstm_trainer.hpp"
#include "lstm_validation_results.hpp"
#include "midi_config.hpp"
#include "midi_translator.hpp"
#include "training_config.hpp"

namespace larasynth {

class TrainerException : public std::runtime_error {
public:
  TrainerException( const std::string& message )
    : runtime_error( message ) {};
};

class Trainer {
public:
  Trainer( const std::string& config_directory_path,
           volatile sig_atomic_t* shutdown_flag );

private:
  static bool _keep_running;

  volatile sig_atomic_t* _shutdown_flag;
};

}
