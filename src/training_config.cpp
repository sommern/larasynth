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

#include "training_config.hpp"

using namespace std;
using namespace larasynth;

TrainingConfig::TrainingConfig( ConfigParameters& params ) {
  unordered_map<string,pair<size_t*,size_t> > optional_booleans = {
    { "feed_back_output", { &_feed_back_output, DEFAULT_FEED_BACK_OUTPUT } }
  };

  for( auto& kv : optional_booleans ) {
    try {
      params.set_var( kv.first, *kv.second.first );
    }
    catch( ConfigParameterException& e ) {
      throw TrainingConfigException( e.what() );
    }
    catch( UndefinedParameterException& e ) {
      *kv.second.first = kv.second.second;
    }
  }

  size_t size_t_max = numeric_limits<size_t>::max();

  vector<ConfigVariableToSet<size_t> > optional_size_ts;

  optional_size_ts.emplace_back( "example_repetitions", &_example_repetitions,
                                 DEFAULT_EXAMPLE_REPETITIONS, (size_t)1,
                                 size_t_max );
  optional_size_ts.emplace_back( "validation_example_repetitions",
                                 &_validation_example_repetitions,
                                 DEFAULT_VALIDATION_EXAMPLE_REPETITIONS,
                                 (size_t)1, size_t_max );
  optional_size_ts.emplace_back( "epoch_count_before_validating",
                                 &_epoch_count_before_validating,
                                 DEFAULT_EPOCH_COUNT_BEFORE_VALIDATING,
                                 (size_t)1, size_t_max );
  optional_size_ts.emplace_back( "round_count", &_round_count,
                                 DEFAULT_ROUND_COUNT, (size_t)1, size_t_max );
  optional_size_ts.emplace_back( "consecutive_failures_for_reset",
                                 &_consecutive_failures_for_reset,
                                 DEFAULT_CONSECUTIVE_FAILURES_FOR_RESET,
                                 (size_t)1, size_t_max );
  optional_size_ts.emplace_back( "squared_error_failure_tolerance",
                                 &_squared_error_failure_tolerance,
                                 DEFAULT_SQUARED_ERROR_FAILURE_TOLERANCE,
                                 (size_t)0, size_t_max );

  for( auto& var_to_set : optional_size_ts ) {
    try {
      params.set_var( var_to_set.name, *var_to_set.var_ptr, var_to_set.min,
                      var_to_set.max );
    }
    catch( ConfigParameterException& e ) {
      throw TrainingConfigException( e.what() );
    }
    catch( UndefinedParameterException& e ) {
      *var_to_set.var_ptr = var_to_set.default_value;
    }
  }

  try {
    params.set_var( "max_epoch_count", _max_epoch_count );
  }
  catch( ConfigParameterException& e ) {
    throw TrainingConfigException( e.what() );
  }
  catch( UndefinedParameterException& e ) {
    _max_epoch_count = DEFAULT_MAX_EPOCH_COUNT;
  }

  double double_max = numeric_limits<double>::max();

  vector<ConfigVariableToSet<double> > optional_doubles;

  optional_doubles.emplace_back( "tempo_adjustment_factor",
                                 &_tempo_adjustment_factor,
                                 DEFAULT_TEMPO_ADJUSTMENT_FACTOR,
                                 0.0, 1.0 );
  optional_doubles.emplace_back( "tempo_jitter_factor",&_tempo_jitter_factor,
                                 DEFAULT_TEMPO_JITTER_FACTOR, 0.0, 1.0 );
  optional_doubles.emplace_back( "mean_padding", &_mean_padding,
                                 DEFAULT_MEAN_PADDING, 0.0, double_max );
  optional_doubles.emplace_back( "padding_stddev", &_padding_stddev,
                                 DEFAULT_PADDING_STDDEV, 0.0, double_max );
  optional_doubles.emplace_back( "zero_network_before_each_epoch",
                                 &_zero_network_before_each_epoch,
                                 DEFAULT_ZERO_NETWORK_BEFORE_EACH_EPOCH,
                                 0.0, 1.0 );
  optional_doubles.emplace_back( "zero_network_before_validation",
                                 &_zero_network_before_validation,
                                 DEFAULT_ZERO_NETWORK_BEFORE_VALIDATION,
                                 0.0, 1.0 );
  optional_doubles.emplace_back( "backpropagate_if_correct",
                                 &_backpropagate_if_correct,
                                 DEFAULT_BACKPROPAGATE_IF_CORRECT, 0.0, 1.0 );
  optional_doubles.emplace_back( "reset_probability", &_reset_probability,
                                 DEFAULT_RESET_PROBABILITY, 0.0, 1.0 );
  optional_doubles.emplace_back( "zero_network_on_reset",
                                 &_zero_network_on_reset,
                                 DEFAULT_ZERO_NETWORK_ON_RESET, 0.0, 1.0 );
  optional_doubles.emplace_back( "mse_threshold", &_mse_threshold,
                                 DEFAULT_MSE_THRESHOLD, 0.0, double_max );

  for( auto& var_to_set : optional_doubles ) {
    try {
      params.set_var( var_to_set.name, *var_to_set.var_ptr, var_to_set.min,
                      var_to_set.max );
    }
    catch( ConfigParameterException& e ) {
      throw TrainingConfigException( e.what() );
    }
    catch( UndefinedParameterException& e ) {
      *var_to_set.var_ptr = var_to_set.default_value;
    }
  }

  // any unset parameters are invalid
  set<string> unset_params = params.get_unset_params();
  if( unset_params.size() != 0 ) {
    ostringstream error;
    error << "Invalid parameter(s): " << endl;
    for( auto& param : unset_params )
      error << "  " << param << endl;

    throw TrainingConfigException( error.str() );
  }
}

void TrainingConfig::print_training_configuration() {
  cout << "Max epochs:    " << _max_epoch_count << endl;
  cout << "MSE threshold: " << _mse_threshold << endl;
  cout << "Epochs before validating: " << _epoch_count_before_validating
       << endl;
  cout << "Backpropagate if correct: "
       << _backpropagate_if_correct << endl;
  cout << "Reset probability: "
       << _reset_probability << endl;
  cout << "Consecutive failures for reset: "
       << _consecutive_failures_for_reset << endl;
  cout << "Squared error failure tolerance: "
       << _squared_error_failure_tolerance << endl;
}
