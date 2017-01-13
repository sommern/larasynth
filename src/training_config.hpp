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

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdexcept>

#include "config_parameter.hpp"
#include "config_parameters.hpp"
#include "config_variable_to_set.hpp"
#include "training_defaults.hpp"
#include "debug.hpp"

namespace larasynth {

/**
 * Thrown if any errors are encountered when constructing the training
 * configuration from the configuration parameters.
 */
class TrainingConfigException : public std::runtime_error {
public:
  TrainingConfigException( const std::string& message )
    : std::runtime_error( "Error in configuration section [training]:\n" +
                          message ) {};
};

/**
 * Represents the configuration used for training. Needs a ConfigParameters
 * object which contains the parameters from a configuration file.
 *
 * Uses the parameters from the [training] section of a configuration file.
 *
 * See training_defaults.hpp for default parameter values.
 */
class TrainingConfig {
public:
  explicit TrainingConfig( ConfigParameters& params );

  void print_training_configuration();

  size_t get_example_repetitions() const
  { return _example_repetitions; }
  size_t get_validation_example_repetitions() const
  { return _validation_example_repetitions; }
  size_t get_epoch_count_before_validating() const
  { return _epoch_count_before_validating; }
  double get_tempo_adjustment_factor() const
  { return _tempo_adjustment_factor; }
  double get_tempo_jitter_factor() const
  { return _tempo_jitter_factor; }
  double get_mean_padding() const { return _mean_padding; }
  double get_padding_stddev() const { return _padding_stddev; }
  double get_zero_network_before_each_epoch() const
  { return _zero_network_before_each_epoch; }
  double get_zero_network_before_validation() const
  { return _zero_network_before_validation; }
  bool get_feed_back_output() { return _feed_back_output; }
  double get_backpropagate_if_correct() const
  { return _backpropagate_if_correct; }
  double get_reset_probability() const
  { return _reset_probability; }
  size_t get_consecutive_failures_for_reset() const
  { return _consecutive_failures_for_reset; }
  size_t get_squared_error_failure_tolerance() const
  { return _squared_error_failure_tolerance; }
  double get_zero_network_on_reset() const { return _zero_network_on_reset; }
  double get_mse_threshold() const { return _mse_threshold; }
  size_t get_max_epoch_count() const { return _max_epoch_count; }

private:
  // booleans
  size_t _feed_back_output;
  
  size_t _example_repetitions;
  size_t _validation_example_repetitions;

  double _tempo_adjustment_factor;
  double _tempo_jitter_factor;
  double _mean_padding;
  double _padding_stddev;

  // probabilities
  double _zero_network_before_each_epoch;
  double _zero_network_before_validation;
  double _backpropagate_if_correct;
  double _reset_probability;
  double _zero_network_on_reset;

  size_t _epoch_count_before_validating;
  size_t _round_count;
  size_t _consecutive_failures_for_reset;
  size_t _squared_error_failure_tolerance;

  size_t _max_epoch_count;
  double _mse_threshold;
};

}
