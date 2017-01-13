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
#include <map>

#include "midi_types.hpp"
#include "midi_state.hpp"

namespace larasynth {

class LstmResult {
public:
  explicit LstmResult( size_t epoch = 0 ) : _epoch( epoch ), _mse( 0.0 ) {}

  size_t get_epoch() const { return _epoch; }
  double get_mse() const { return _mse; }
  const std::vector<ctrl_values_t>& get_targets() const
  { return _targets; }
  const std::vector<ctrl_values_t>& get_outputs() const
  { return _outputs; }
  const std::vector< std::map<size_t, double> >& get_cell_states() const
  { return _cell_states; }

  void set_mse( const double mse ) { _mse = mse; }
  void add_target( const ctrl_values_t& target )
  { _targets.push_back( target ); }
  void add_output( const ctrl_values_t& output )
  { _outputs.push_back( output ); }
  void add_cell_states( const std::map<size_t, double>& cell_states )
  { _cell_states.push_back( cell_states ); }

private:
  size_t _epoch;
  double _mse;
  std::vector<ctrl_values_t> _targets;
  std::vector<ctrl_values_t> _outputs;
  std::vector< std::map<size_t, double> > _cell_states;
};

}
