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

#include <cstddef>

namespace larasynth {
  static const size_t DEFAULT_EXAMPLE_REPETITIONS = 2;
  static const size_t DEFAULT_VALIDATION_EXAMPLE_REPETITIONS = 5;
  static const double DEFAULT_TEMPO_ADJUSTMENT_FACTOR = 0.0;
  static const double DEFAULT_TEMPO_JITTER_FACTOR = 0.0;
  static const double DEFAULT_MEAN_PADDING = 0.0;
  static const double DEFAULT_PADDING_STDDEV = 0.0;
  static const size_t DEFAULT_EPOCH_COUNT_BEFORE_VALIDATING = 50;
  static const size_t DEFAULT_ROUND_COUNT = 1;
  static const double DEFAULT_ZERO_NETWORK_BEFORE_EACH_EPOCH = 1.0;
  static const size_t DEFAULT_FEED_BACK_OUTPUT = true;
  static const double DEFAULT_BACKPROPAGATE_IF_CORRECT = 0.0;
  static const double DEFAULT_RESET_PROBABILITY = 1.0;
  static const double DEFAULT_ZERO_NETWORK_BEFORE_VALIDATION = 1.0;
  static const size_t DEFAULT_CONSECUTIVE_FAILURES_FOR_RESET = 1;
  static const double DEFAULT_SQUARED_ERROR_FAILURE_TOLERANCE = 0.0;
  static const double DEFAULT_ZERO_NETWORK_ON_RESET = 1.0;
  static const size_t DEFAULT_MAX_EPOCH_COUNT = 10000;
  static const double DEFAULT_MSE_THRESHOLD = 0.0;
  static const size_t DEFAULT_BEST_RESULT_COUNT = 5;
}
