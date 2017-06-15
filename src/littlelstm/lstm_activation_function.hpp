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

#include "lstm_types.hpp"

namespace littlelstm {

inline Real_t identity( Real_t x ) {
  return x;
}
    
inline Real_t identity_derivative( Real_t x ) {
  return 1.0;
}

inline Real_t logistic( Real_t x )
{
  return ( 1.0 / ( 1.0 + exp( x * -1.0 ) ) );
}

inline Real_t logistic_derivative( Real_t x ) {
  return ( x * ( 1.0 - x ) );
}

inline Real_t logistic_centered( Real_t x )
{
  return ( logistic( x ) * 2.0 - 1.0 );
}

inline Real_t logistic_centered_derivative( Real_t x )
{
  return ( x * ( 1.0 - x ) * 2.0 );
}

// tanh already defined in <cmath>

inline Real_t tanh_derivative( Real_t x ) {
  return 1.0 - x * x;
}

}
