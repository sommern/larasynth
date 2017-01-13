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

#include "lstm_result.hpp"

namespace larasynth {

class LstmValidationResults {
public:
  LstmValidationResults( size_t best_result_count );

  size_t get_best_epoch() { return _best_epoch; }
  std::vector<size_t>& get_epochs() { return _epochs; }
  std::vector<double>& get_mses() { return _mses; }
  std::vector<LstmResult>& get_best_results() { return _best_results; }

  void add_result( LstmResult result );

private:
  size_t _best_result_count;

  size_t _best_epoch;
  std::vector<size_t> _epochs;
  std::vector<double> _mses;
  std::vector<LstmResult> _best_results;
};

}
