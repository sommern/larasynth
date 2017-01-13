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
#include <unistd.h>
#include <map>
#include <chrono>
#include <thread>

#include "json.hpp"
#include "lstm_config.hpp"
#include "lstm_network.hpp"
#include "midi_min_max.hpp"
#include "lstm_result.hpp"
#include "debug.hpp"
#include "lstm_types.hpp"
#include "lstm_architecture.hpp"
#include "lstm_unit_properties.hpp"

namespace larasynth {

class LstmResultJson {
public:
  explicit LstmResultJson( std::string directory_name );

  size_t get_max_run_number();
  size_t get_lowest_mse_run_number();
  void set_to_lowest_mse_run_number();
  size_t get_lowest_mse_epoch();
  WeightsMap_t get_weights( size_t epoch );
  WeightsMap_t get_lowest_mse_weights();
  std::vector<size_t> get_class_counts();
  std::vector< std::pair<Id_t,Id_t> > get_connections();
  std::vector<LstmUnitProperties> get_units_properties();
  MidiMinMax get_min_max();
  LstmNetwork get_trained_network();

  void write_epoch_stats( const std::vector<size_t>& epochs,
                          const std::vector<double>& mses );
  void write_best_results( const std::vector<LstmResult>& results );
  void write_epoch_results( const LstmResult& result );
  void write_weights( size_t epoch, const WeightsMap_t weights );
  void write_architecture( const LstmArchitecture& arch );
  void write_class_counts( const std::vector<std::size_t>& class_counts );

  void write_min_max( const MidiMinMax& min_max );
  void update_progress( size_t epoch, double mse );
  void write_config( LstmConfig& config );
  //void write_config_file( const string& config_file_text );
  void write_numeric_param( const std::string& name, double value );
  void write_numeric_param( const std::string& name, int value );
  void write_numeric_param( const std::string& name, size_t value );

  void get_numeric_param( const std::string& name, double& value );
  void get_numeric_param( const std::string& name, int& value );
  void get_numeric_param( const std::string& name, size_t& value );  
  void get_numeric_param( const std::string& name, bool& value );

  void insert_from_other_database( const std::string& filename );

  void set_run_number( size_t run_number ) { _run_number = run_number; }
  size_t get_run_number() { return _run_number; }

private:
  void insert_connections( const std::vector< std::pair< Id_t, Id_t > >& conns );
  void insert_units_properties( const std::vector<LstmUnitProperties>&
                                units_properties );
  void insert_gated_conns( const std::vector<LstmGatedConn>& conns );

  std::vector<LstmGatedConn> get_gated_conns( Id_t gater_id );
  LstmUnitProperties unit_properties_from_row( sqlite3_stmt* stmt );

  size_t _run_number;
};

}
