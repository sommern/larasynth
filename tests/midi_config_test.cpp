#include "midi_config.hpp"
#include "config_parser.hpp"

#include "gtest/gtest.h"

using namespace std;
using namespace larasynth;

TEST( MidiConfigTest, AllValues ) {
  ConfigParser cp( "test_files/midi_config_test/all_values/larasynth.conf" );
  ConfigParameters params = cp.get_section_params( "midi" );
  MidiConfig config( params );

  EXPECT_EQ( "test_recording_source_port",
             config.get_recording_source_port() );
  EXPECT_EQ( "test_performing_source_port",
             config.get_performing_source_port() );
  EXPECT_EQ( "test_performing_destination_port",
             config.get_performing_destination_port() );

  vector<event_data_t> ctrls = config.get_ctrls();

  EXPECT_EQ( 3, ctrls.size() );

  EXPECT_EQ( 3, ctrls[0] );
  EXPECT_EQ( 5, ctrls[1] );
  EXPECT_EQ( 20, ctrls[2] );

  unordered_map<event_data_t,event_data_t> ctrl_defaults = config.get_ctrl_defaults();

  EXPECT_EQ( 3, ctrl_defaults.size() );

  EXPECT_EQ( 64, ctrl_defaults[3] );
  EXPECT_EQ( 127, ctrl_defaults[5] );
  EXPECT_EQ( 0, ctrl_defaults[20] );  
}

TEST( MidiConfigTest, OneController ) {
  ConfigParser cp( "test_files/midi_config_test/one_controller/larasynth.conf" );
  ConfigParameters params = cp.get_section_params( "midi" );
  MidiConfig config( params );

  EXPECT_EQ( "test_recording_source_port",
             config.get_recording_source_port() );
  EXPECT_EQ( "test_performing_source_port",
             config.get_performing_source_port() );
  EXPECT_EQ( "test_performing_destination_port",
             config.get_performing_destination_port() );

  vector<event_data_t> ctrls = config.get_ctrls();

  EXPECT_EQ( 1, ctrls.size() );
  EXPECT_EQ( 3, ctrls[0] );

  unordered_map<event_data_t,event_data_t> ctrl_defaults = config.get_ctrl_defaults();

  EXPECT_EQ( 1, ctrl_defaults.size() );
  EXPECT_EQ( 64, ctrl_defaults[3] );
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
