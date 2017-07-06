#include "midi_translator.hpp"
#include "representation_config.hpp"
#include "midi_config.hpp"
#include "config_parser.hpp"
#include "config_parameters.hpp"
#include "midi_min_max.hpp"
#include "event.hpp"
#include "training_sequence.hpp"

#include "gtest/gtest.h"

#include <vector>

using namespace std;
using namespace larasynth;

TEST( MidiTranslatorTest, BasicTest ) {
  MidiMinMax min_max;

  Event event;

  size_t output_size = 12;
  unordered_map<event_data_t,size_t> ctrl_output_counts( { { 1, 3 },
                                                           { 2, 4 },
                                                           { 3, 5 } } );
  unordered_map<event_data_t,event_data_t> ctrl_defaults( { { 1, 64 },
                                                            { 2, 127 },
                                                            { 3, 0 } } );
  vector<double> input( 1 + output_size, 0.0 );
  vector<double> target( output_size, 0.0 );

  vector<double> defaults_output = { 0.0, 1.0, 0.0,
                                     0.0, 0.0, 0.0, 1.0,
                                     1.0, 0.0, 0.0, 0.0, 0.0 };

  TrainingSequence seq;

  // set ctrl mins and maxs
  event.set_ctrl( 0, 1, 63, 0 );
  seq.add_event( event );
  event.set_ctrl( 0, 1, 65, 0 );
  seq.add_event( event );

  event.set_ctrl( 0, 2, 0, 0 );
  seq.add_event( event );
  event.set_ctrl( 0, 2, 127, 0 );
  seq.add_event( event );

  event.set_ctrl( 0, 3, 0, 0 );
  seq.add_event( event );
  event.set_ctrl( 0, 3, 50, 0 );
  seq.add_event( event );

  min_max.consider_sequence( seq );

  EXPECT_EQ( 63, min_max.get_ctrl_min( 1 ) );
  EXPECT_EQ( 65, min_max.get_ctrl_max( 1 ) );

  EXPECT_EQ( 0, min_max.get_ctrl_min( 2 ) );
  EXPECT_EQ( 127, min_max.get_ctrl_max( 2 ) );

  EXPECT_EQ( 0, min_max.get_ctrl_min( 3 ) );
  EXPECT_EQ( 50, min_max.get_ctrl_max( 3 ) );

  feature_config_t feature_config;
  feature_config[SOME_NOTE_ON] = true;

  MidiTranslator trans( ctrl_output_counts, feature_config, ctrl_defaults,
                        min_max, TRAIN );

  trans.fill_input( input, TARGET_SOURCE );
  trans.fill_target( target );

  EXPECT_EQ( vector<double>( { 0.0,
                               0.0, 1.0, 0.0,
                               0.0, 0.0, 0.0, 1.0,
                               1.0, 0.0, 0.0, 0.0, 0.0 } ), input );
  EXPECT_EQ( vector<double>( { 0.0, 1.0, 0.0,
                               0.0, 0.0, 0.0, 1.0,
                               1.0, 0.0, 0.0, 0.0, 0.0 } ), target );

  trans.update_ctrl_value( 1, 63 );
  trans.fill_input( input, TARGET_SOURCE );
  trans.fill_target( target );

  EXPECT_EQ( vector<double>( { 0.0,
                               0.0, 1.0, 0.0,
                               0.0, 0.0, 0.0, 1.0,
                               1.0, 0.0, 0.0, 0.0, 0.0 } ), input );
  EXPECT_EQ( vector<double>( { 1.0, 0.0, 0.0,
                               0.0, 0.0, 0.0, 1.0,
                               1.0, 0.0, 0.0, 0.0, 0.0 } ), target );

  trans.report_output( target );

  trans.update_ctrl_value( 1, 64 );
  trans.fill_input( input, TARGET_SOURCE );
  trans.fill_target( target );

  EXPECT_EQ( vector<double>( { 0.0,
                               1.0, 0.0, 0.0,
                               0.0, 0.0, 0.0, 1.0,
                               1.0, 0.0, 0.0, 0.0, 0.0} ), input );
  EXPECT_EQ( vector<double>( { 0.0, 1.0, 0.0,
                               0.0, 0.0, 0.0, 1.0,
                               1.0, 0.0, 0.0, 0.0, 0.0 } ), target );

  trans.report_output( target );

  trans.update_ctrl_value( 1, 65 );
  trans.fill_input( input, TARGET_SOURCE );
  trans.fill_target( target );

  EXPECT_EQ( vector<double>( { 0.0,
                               0.0, 1.0, 0.0,
                               0.0, 0.0, 0.0, 1.0,
                               1.0, 0.0, 0.0, 0.0, 0.0} ), input );
  EXPECT_EQ( vector<double>( { 0.0, 0.0, 1.0,
                               0.0, 0.0, 0.0, 1.0,
                               1.0, 0.0, 0.0, 0.0, 0.0 } ), target );

  trans.report_output( target );

  trans.update_ctrl_value( 2, 5 );
  trans.fill_input( input, TARGET_SOURCE );
  trans.fill_target( target );

  EXPECT_EQ( vector<double>( { 0.0,
                               0.0, 0.0, 1.0,
                               0.0, 0.0, 0.0, 1.0,
                               1.0, 0.0, 0.0, 0.0, 0.0 } ), input );
  EXPECT_EQ( vector<double>( { 0.0, 0.0, 1.0,
                               1.0, 0.0, 0.0, 0.0,
                               1.0, 0.0, 0.0, 0.0, 0.0 } ), target );

  trans.report_output( target );

  trans.update_ctrl_value( 2, 22 );
  trans.fill_input( input, TARGET_SOURCE );
  trans.fill_target( target );

  EXPECT_EQ( vector<double>( { 0.0,
                               0.0, 0.0, 1.0,
                               1.0, 0.0, 0.0, 0.0,
                               1.0, 0.0, 0.0, 0.0, 0.0} ), input );
  EXPECT_EQ( vector<double>( { 0.0, 0.0, 1.0,
                               0.0, 1.0, 0.0, 0.0,
                               1.0, 0.0, 0.0, 0.0, 0.0 } ), target );

  trans.report_output( target );

  trans.update_ctrl_value( 2, 70 );
  trans.fill_input( input, TARGET_SOURCE );
  trans.fill_target( target );

  EXPECT_EQ( vector<double>( { 0.0,
                               0.0, 0.0, 1.0,
                               0.0, 1.0, 0.0, 0.0,
                               1.0, 0.0, 0.0, 0.0, 0.0} ), input );
  EXPECT_EQ( vector<double>( { 0.0, 0.0, 1.0,
                               0.0, 0.0, 1.0, 0.0,
                               1.0, 0.0, 0.0, 0.0, 0.0 } ), target );

  trans.report_output( target );

  trans.update_ctrl_value( 2, 120 );
  trans.fill_input( input, TARGET_SOURCE );
  trans.fill_target( target );

  EXPECT_EQ( vector<double>( { 0.0,
                               0.0, 0.0, 1.0,
                               0.0, 0.0, 1.0, 0.0,
                               1.0, 0.0, 0.0, 0.0, 0.0} ), input );
  EXPECT_EQ( vector<double>( { 0.0, 0.0, 1.0,
                               0.0, 0.0, 0.0, 1.0,
                               1.0, 0.0, 0.0, 0.0, 0.0 } ), target );

  trans.report_output( target );

  trans.update_ctrl_value( 3, 2 );
  trans.fill_input( input, TARGET_SOURCE );
  trans.fill_target( target );

  EXPECT_EQ( vector<double>( { 0.0,
                               0.0, 0.0, 1.0,
                               0.0, 0.0, 0.0, 1.0,
                               1.0, 0.0, 0.0, 0.0, 0.0} ), input );
  EXPECT_EQ( vector<double>( { 0.0, 0.0, 1.0,
                               0.0, 0.0, 0.0, 1.0,
                               1.0, 0.0, 0.0, 0.0, 0.0 } ), target );

  trans.report_output( target );

  trans.update_ctrl_value( 3, 10 );
  trans.fill_input( input, TARGET_SOURCE );
  trans.fill_target( target );

  EXPECT_EQ( vector<double>( { 0.0,
                               0.0, 0.0, 1.0,
                               0.0, 0.0, 0.0, 1.0,
                               1.0, 0.0, 0.0, 0.0, 0.0} ), input );
  EXPECT_EQ( vector<double>( { 0.0, 0.0, 1.0,
                               0.0, 0.0, 0.0, 1.0,
                               0.0, 1.0, 0.0, 0.0, 0.0 } ), target );

  trans.report_output( target );

  trans.update_ctrl_value( 3, 33 );
  trans.fill_input( input, TARGET_SOURCE );
  trans.fill_target( target );

  EXPECT_EQ( vector<double>( { 0.0,
                               0.0, 0.0, 1.0,
                               0.0, 0.0, 0.0, 1.0,
                               0.0, 1.0, 0.0, 0.0, 0.0 } ), input );
  EXPECT_EQ( vector<double>( { 0.0, 0.0, 1.0,
                               0.0, 0.0, 0.0, 1.0,
                               0.0, 0.0, 0.0, 1.0, 0.0 } ), target );

  trans.report_output( target );

  trans.update_ctrl_value( 3, 45 );
  trans.fill_input( input, TARGET_SOURCE );
  trans.fill_target( target );

  EXPECT_EQ( vector<double>( { 0.0,
                               0.0, 0.0, 1.0,
                               0.0, 0.0, 0.0, 1.0,
                               0.0, 0.0, 0.0, 1.0, 0.0} ), input );
  EXPECT_EQ( vector<double>( { 0.0, 0.0, 1.0,
                               0.0, 0.0, 0.0, 1.0,
                               0.0, 0.0, 0.0, 0.0, 1.0 } ), target );

  event.set_note_on( 0, 61, 50, 0 );

  trans.report_output( target );

  trans.report_note_event( &event );
  trans.fill_input( input, TARGET_SOURCE );
  trans.fill_target( target );

  EXPECT_EQ( vector<double>( { 1.0,
                               0.0, 0.0, 1.0,
                               0.0, 0.0, 0.0, 1.0,
                               0.0, 0.0, 0.0, 0.0, 1.0} ), input);
  EXPECT_EQ( vector<double>( { 0.0, 0.0, 1.0,
                               0.0, 0.0, 0.0, 1.0,
                               0.0, 0.0, 0.0, 0.0, 1.0 } ), target );

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
