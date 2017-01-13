#include <string>

#include "representation_config.hpp"
#include "config_parser.hpp"

#include "gtest/gtest.h"

using namespace std;
using namespace larasynth;

TEST( RepresentationConfigTest, AllValues ) {
  string conf_filename =
    "test_files/representation_config_test/all_values/larasynth.conf";
  ConfigParser cp( conf_filename );

  EXPECT_TRUE( cp.section_exists( "representation" ) );

  ConfigParameters params = cp.get_section_params( "representation" );
  RepresentationConfig config( params );

  EXPECT_EQ( 40, config.get_update_rate() );

  unordered_map<event_data_t, size_t> ctrl_output_counts = config.get_ctrl_output_counts();

  EXPECT_EQ( 5, ctrl_output_counts[3] );
  EXPECT_EQ( 10, ctrl_output_counts[8] );

  EXPECT_TRUE( config.get_input_feature_config()[SOME_NOTE_ON] );
  EXPECT_TRUE( config.get_input_feature_config()[NOTE_STRUCK] );
  EXPECT_TRUE( config.get_input_feature_config()[NOTE_RELEASED] );
  EXPECT_TRUE( config.get_input_feature_config()[VELOCITY] );
  EXPECT_TRUE( config.get_input_feature_config()[INTERVAL] );
  cout << config.get_input_feature_config() << endl;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
