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

#include "lstm_types.hpp"

namespace littlelstm {

class LstmGatedConn {
public:
  LstmGatedConn() {}
  LstmGatedConn( Id_t gater, Id_t in, Id_t out )
    : gater_id( gater ), in_id( in ), out_id( out ) {}

  Id_t gater_id;
  Id_t in_id;
  Id_t out_id;
};

inline bool operator < ( const LstmGatedConn& A, const LstmGatedConn& B ) {
  if( A.gater_id < B.gater_id )
    return true;
  if( A.in_id < B.in_id )
    return true;
  if( A.out_id < B.out_id )
    return true;

  return false;
}

struct LstmGatedConnData {
  Real_t gater_activation;
  Real_t right_term_sum;
};

}
