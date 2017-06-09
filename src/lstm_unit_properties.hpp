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
#include <iostream>

#include "lstm_gated_connection.hpp"
#include "lstm_activation_function.hpp"
#include "lstm_types.hpp"

namespace larasynth {

class LstmUnitProperties {
public:
  LstmUnitProperties( Id_t id, lstm_unit_t type );
  LstmUnitProperties( Id_t id, lstm_unit_t type,
                      lstm_act_func_t act_func_type, Id_t self_conn_gater,
                      std::vector<LstmGatedConn> gated_conns );

  void print();

  Id_t get_id() const { return _id; }
  lstm_unit_t get_type() const { return _type; }
  const std::vector<LstmGatedConn>& get_gated_conns() const
  { return _gated_conns; }
  lstm_act_func_t get_act_func_type() const { return _act_func_type; }
  act_func_ptr_t get_act_func() const { return _act_func; }
  act_func_ptr_t get_act_func_derivative() const
  { return _act_func_derivative; }
  bool get_self_conn() const { return _self_conn_gater != NO_UNIT; }
  Id_t get_self_conn_gater() const { return _self_conn_gater; }

  void add_gated_conn( Id_t in_id, Id_t out_id );
  void set_act_func( lstm_act_func_t act_func_type );
  void set_self_conn_gater( Id_t self_conn_gater )
  { _self_conn_gater = self_conn_gater; }

private:
  Id_t _id;
  lstm_unit_t _type;
  std::vector<LstmGatedConn> _gated_conns;
  lstm_act_func_t _act_func_type;
  act_func_ptr_t _act_func;
  act_func_ptr_t _act_func_derivative;
  Id_t _self_conn_gater;
};

}
