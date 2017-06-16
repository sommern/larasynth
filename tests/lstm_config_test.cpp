#include "lstm_config.hpp"
#include "config_parser.hpp"
#include "littlelstm/lstm_layer_config.hpp"

#include "gtest/gtest.h"

using namespace std;
using namespace larasynth;

class LstmConfigTest : public ::testing::Test {
protected:
  string prefix = "test_files/lstm_config_test/";
};

TEST_F( LstmConfigTest, Defaults ) {
  ConfigParser cp( prefix + "defaults/larasynth.conf" );

  ConfigParameters params = cp.get_section_params( "lstm" );
  
  LstmConfig config( params );

  EXPECT_EQ( 0.05, config.get_learning_rate() );
  EXPECT_EQ( 0.8, config.get_momentum() );

  vector<size_t> block_counts = config.get_block_counts();

  ASSERT_EQ( 1, block_counts.size() );
  EXPECT_EQ( 17, block_counts[0] );
}

TEST_F( LstmConfigTest, Values ) {
  ConfigParser cp( prefix + "values/larasynth.conf" );

  ConfigParameters params = cp.get_section_params( "lstm" );
  
  LstmConfig config( params );

  EXPECT_EQ( 0.05, config.get_learning_rate() );
  EXPECT_EQ( 0.9, config.get_momentum() );

  vector<size_t> block_counts = config.get_block_counts();

  ASSERT_EQ( 2, block_counts.size() );
  EXPECT_EQ( 19, block_counts[0] );
  EXPECT_EQ( 29, block_counts[1] );
}

TEST_F( LstmConfigTest, UniformRandDouble ) {
  for( size_t i = 0; i < 100; ++i ) {
    ConfigParser cp( prefix + "uniform_rand_double/larasynth.conf" );

    ConfigParameters params = cp.get_section_params( "lstm" );
  
    LstmConfig config( params );

    EXPECT_LE( 0.5, config.get_momentum() );
    EXPECT_GE( 1.5, config.get_momentum() );
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
