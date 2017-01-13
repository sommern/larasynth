#include "training_results.hpp"
#include "gtest/gtest.h"

using namespace std;
using namespace larasynth;

class TrainingResultsTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    results_file = "test_files/training_results_test/results.json";
    if( is_regular_file( results_file ) )
      remove( results_file.c_str() );
  }

  string results_file;
};

TEST_F( TrainingResultsTest, WeightsTest ) {
  TrainingResults results( results_file, WRITE_RESULTS );

  vector< pair<Id_t, Id_t> > connections = {
    { 3, 10 },
    { 4, 11 }
  };

  results.add_connections( connections );

  WeightsMap_t weights_map;

  weights_map[10][3] = 0.5;
  weights_map[11][4] = 1.0;

  EXPECT_NO_THROW( results.add_weights( weights_map ) );

  WeightsMap_t result_weights_map;

  EXPECT_NO_THROW( result_weights_map = results.get_weights() );

  EXPECT_EQ( 0.5, result_weights_map[10][3] );
  EXPECT_EQ( 1.0, result_weights_map[11][4] );

  EXPECT_NO_THROW( results.write() );

  TrainingResults results_reader( results_file, READ_RESULTS );

  EXPECT_EQ( result_weights_map, results_reader.get_weights() );
}

TEST_F( TrainingResultsTest, MinMaxTest ) {
  TrainingResults results( results_file, WRITE_RESULTS );

  MidiMinMax min_max;

  min_max.set_note_min( 10 );
  min_max.set_note_max( 100 );

  min_max.set_ctrl_min( 1, 30 );
  min_max.set_ctrl_max( 1, 50 );

  EXPECT_NO_THROW( results.add_min_max( min_max ) );

  EXPECT_NO_THROW( results.write() );

  EXPECT_NO_THROW( {
    MidiMinMax written_min_max = results.get_min_max();
    EXPECT_EQ( min_max, written_min_max );
  } );

  TrainingResults results_reader( results_file, READ_RESULTS );

  EXPECT_NO_THROW( {
    MidiMinMax read_min_max = results_reader.get_min_max();
    EXPECT_EQ( min_max, read_min_max );
  } );
}

TEST_F( TrainingResultsTest, StatesTest ) {
  TrainingResults results( results_file, WRITE_RESULTS );

  map<size_t, double> cell_states;
  ctrl_values_t target;
  ctrl_values_t output;

  LstmResult result( 10 );

  target[1] = 2;
  output[1] = 3;

  cell_states[5] = 0.5;
  cell_states[10] = 1.2;

  result.add_target( target );
  result.add_output( output );
  result.add_cell_states( cell_states );

  target[1] = 3;
  output[1] = 2;

  cell_states[5] = 0.0;
  cell_states[10] = -0.5;

  result.add_target( target );
  result.add_output( output );
  result.add_cell_states( cell_states );

  EXPECT_NO_THROW( results.add_result( result ) );

  EXPECT_NO_THROW( results.write() );

  EXPECT_NO_THROW( TrainingResults results_reader( results_file, READ_RESULTS ) );
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}
