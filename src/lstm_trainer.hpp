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

#include <cmath>

#include "lstm_activation_function.hpp"
#include "lstm_network.hpp"
#include "training_config.hpp"
#include "lstm_types.hpp"
#include "lstm_result.hpp"
#include "training_event_stream.hpp"
#include "midi_types.hpp"
#include "midi_translator.hpp"

namespace larasynth {

class LstmTrainer {
public:
  LstmTrainer( LstmNetwork& untrained_network,
                TrainingEventStream& training_stream,
                TrainingConfig& training_config,
                LstmConfig& network_config,
                MidiTranslator& midi_translator,
                size_t update_period );

  size_t get_epoch() { return _epoch; }
  size_t get_best_streak() { return _max_streak; }
  bool should_validate();

  void run_training_epoch();
  LstmResult validate();

private:
  void advance_stream_until_update_time();
  void feed_forward_next( ctrl_values_t& target_ctrl_values,
                          ctrl_values_t& output_ctrl_values,
                          feedback_source source,
                          bool print = false);
  double calculate_error( const ctrl_values_t& target_ctrl_values,
                          const ctrl_values_t& output_ctrl_values );
  bool should_backpropogate( bool correct );
  bool should_reset( bool correct, size_t consecutive_failure_count );
  void prepare_network_and_training_data();
  void shuffle();
  bool result_is_quality( LstmResult& result );
  double square( const double& val ) { return val * val; }
  bool prob_bool( double probability );

  LstmNetwork& _network;
  TrainingEventStream& _training_stream;
  TrainingConfig& _training_config;
  LstmConfig& _network_config;
  MidiTranslator& _midi_translator;

  size_t _epoch = 0;

  size_t _max_streak = 0;
  bool _new_best_streak = false;

  size_t _update_period;
  
  size_t _current_time = 0;
  size_t _next_update_time = 0;

  RandGen _rand_gen;
};

}
