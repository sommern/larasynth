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

#include "lstm_validation_results.hpp"

using namespace larasynth;

LstmValidationResults::LstmValidationResults( size_t best_result_count )
  : _best_result_count( best_result_count ),
    _best_epoch( 0 )
{}

void LstmValidationResults::add_result( LstmResult result ) {
  _epochs.push_back( result.get_epoch() );
  _mses.push_back( result.get_mse() );

  if( _best_results.size() == _best_result_count
      && result.get_mse() >= _best_results.back().get_mse() )
    return;

  size_t i;
  for( i = 0; i < _best_results.size(); ++i ) {
    if( result.get_mse() < _best_results[i].get_mse() )
      break;
  }

  if( i == 0 )
    _best_epoch = result.get_epoch();

  _best_results.insert( _best_results.begin() + i, result );

  if( _best_results.size() > _best_result_count )
    _best_results.pop_back();
}  
       
