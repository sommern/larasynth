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

#include "lstm_unit_properties.hpp"

using namespace std;
using namespace larasynth;

LstmUnitProperties::LstmUnitProperties( Id_t id, lstm_unit_t type,
                                          lstm_act_func_t act_func_type,
                                          Id_t self_conn_gater,
                                          vector<LstmGatedConn> gated_conns )
  : _id( id )
  , _type( type )
  , _act_func_type( act_func_type )
  , _self_conn_gater( self_conn_gater )
  , _gated_conns( gated_conns )
{
  set_act_func( act_func_type );
}


void LstmUnitProperties::add_gated_conn( Id_t in_id, Id_t out_id ) {
  _gated_conns.push_back( LstmGatedConn( _id, in_id, out_id ) );
}

void LstmUnitProperties::set_act_func( lstm_act_func_t act_func_type ) {
  _act_func_type = act_func_type;

  switch( act_func_type ) {
  case LOGISTIC:
    _act_func = logistic;
    _act_func_derivative = logistic_derivative;
    break;
  case LOGISTIC_CENTERED:
    _act_func = logistic_centered;
    _act_func_derivative = logistic_centered_derivative;
    break;
  default:
    break;
  }
}

void LstmUnitProperties::print() {
  cout << "Unit " << _id << ":" << endl;
  cout << " Type: " << unit_type_to_string( _type ) << endl;
  if( _gated_conns.size() != 0 ) {
    cout << " Gated conns:" << endl;
    for( Index_t i = 0; i < _gated_conns.size(); ++i )
      cout << "  " << _gated_conns[i].in_id << " -> "
           << _gated_conns[i].out_id << endl;
  }
  if( get_self_conn() )
    cout << " Self connection gated by " << _self_conn_gater << endl;

  cout << endl;
}
