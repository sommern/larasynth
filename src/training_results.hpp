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
#include <map>
#include <chrono>
#include <thread>

#include "lstm_config.hpp"
#include "representation_config.hpp"
#include "training_config.hpp"
#include "littlelstm/lstm_network.hpp"
#include "midi_min_max.hpp"
#include "filesystem_operations.hpp"
#include "lstm_result.hpp"
#include "debug.hpp"
#include "littlelstm/lstm_types.hpp"
#include "littlelstm/lstm_architecture.hpp"
#include "littlelstm/lstm_unit_properties.hpp"
#include "json/json.hpp"
#include "littlelstm/json_importer.hpp"
#include "littlelstm/json_exporter.hpp"

namespace larasynth {

enum training_results_mode { READ_RESULTS, WRITE_RESULTS };

class TrainingResultsException : public std::runtime_error {
public:
  explicit TrainingResultsException( const std::string& message )
    : runtime_error( message ) {};
};

class TrainingResults {
public:
/**
 * Constructor.
 *
 * The directory name must be an absolute or relative path to an existing
 * driectory, and it must have a trailing slash.
 */
  explicit TrainingResults( const std::string& directory_name,
                            training_results_mode mode );

  std::string get_filename() { return _filename; }

  double get_mse() { return _json["mse"]; }
  littlelstm::WeightsMap_t get_weights();
  std::vector< std::pair<Id_t,Id_t> > get_connections();
  std::vector<littlelstm::LstmUnitProperties> get_units_properties();
  MidiMinMax get_min_max();
  littlelstm::LstmNetwork get_trained_network();
  RepresentationConfig get_repr_config();

  void add_result( const LstmResult& result );
  void add_weights( const littlelstm::WeightsMap_t& weights );
  void add_network( const littlelstm::LstmNetwork& net );
  void add_min_max( const MidiMinMax& min_max );
  void add_lstm_config( const LstmConfig& lstm_config );
  void add_repr_config( const RepresentationConfig& repr_config );
  void add_training_config( const TrainingConfig& training_config );

  void write();
  nlohmann::json get_json() { return _json; }

  void add_connections( const std::vector< std::pair<Id_t, Id_t> >&
                        connections );
private:
  void add_units_properties( const std::vector<littlelstm::LstmUnitProperties>&
                             units_properties );
  void add_gated_conns( const std::vector<littlelstm::LstmGatedConn>& conns );
  std::unordered_map<Id_t, std::vector<littlelstm::LstmGatedConn> >
  get_all_gated_conns();

  std::string _filename;

  training_results_mode _mode;

  littlelstm::JsonImporter _importer;
  littlelstm::JsonExporter _exporter;
  nlohmann::json _json;
};

}
