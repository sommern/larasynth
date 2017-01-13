#include <string>
#include <vector>
#include <utility>

#include "config_parser.hpp"
#include "config_parameter.hpp"
#include "config_parameters.hpp"
#include "gtest/gtest.h"

using namespace std;
using namespace larasynth;

TEST( ConfigParserTest, OneSection ) {
  string filename = "test_files/config_parser_test/one_section.conf";

  ConfigParser parser( filename );

  vector< pair<string,double> > expected_doubles;

  expected_doubles.emplace_back( "ed1", 0.4 );
  expected_doubles.emplace_back( "ed2", 4.0 );
  expected_doubles.emplace_back( "ed3", -0.1 );
  expected_doubles.emplace_back( "ed4", 101.42451 );

  vector< pair<string,int> > expected_ints;

  expected_ints.emplace_back( "ei1", 80 );
  expected_ints.emplace_back( "ei2", -100 );
  expected_ints.emplace_back( "ei3", 0 );
  expected_ints.emplace_back( "ei4", -1 );

  vector< pair<string,string> > expected_strings;

  expected_strings.emplace_back( "es1", " hi  " );
  expected_strings.emplace_back( "es2", "\"embedded\" quote" );

  ConfigParameters section = parser.get_section_params( "test" );

  for( auto& p : expected_doubles ) {
    string name = p.first;
    double value = p.second;

    double conf_value;
    
    ASSERT_TRUE( section.exists( name ) );
    section.set_var( name, conf_value );
    EXPECT_EQ( value, conf_value );
  }

  for( auto& p : expected_ints ) {
    string name = p.first;
    int value = p.second;

    int conf_value;

    ASSERT_TRUE( section.exists( name ) );
    section.set_var( name, conf_value );
    EXPECT_EQ( value, conf_value );
  }

  for( auto& p : expected_strings ) {
    string name = p.first;
    string value = p.second;

    string conf_value;

    ASSERT_TRUE( section.exists( name ) );
    section.set_var( name, conf_value );
    EXPECT_EQ( value, conf_value );
  }

  EXPECT_EQ( 0, section.get_unset_params().size() );
}

TEST( ConfigParserTest, TwoSections ) {
  string filename = "test_files/config_parser_test/two_sections.conf";

  ConfigParser parser( filename );

  ConfigParameters section_1 = parser.get_section_params( "section_one" );
  ConfigParameters section_2 = parser.get_section_params( "section_2" );

  size_t uint_conf_value;

  ASSERT_TRUE( section_1.exists( "GOSH_4_eva" ) );
  section_1.set_var( "GOSH_4_eva", uint_conf_value );
  EXPECT_EQ( 4, uint_conf_value );

  string string_conf_value;

  ASSERT_TRUE( section_2.exists( "hrm" ) );
  section_2.set_var( "hrm", string_conf_value );
  EXPECT_EQ( "yep", string_conf_value );

  EXPECT_EQ( 0, section_1.get_unset_params().size() );
  EXPECT_EQ( 0, section_2.get_unset_params().size() );  
}

TEST( ConfigParserTest, Lists ) {
  string filename = "test_files/config_parser_test/lists.conf";

  ConfigParser parser( filename );

  ConfigParameters section = parser.get_section_params( "test" );

  vector<double> expected_real_list = { -1.0, 0.5, -10.0, 100.2 };
  vector<double> conf_real_list;

  ASSERT_TRUE( section.exists( "real_list" ) );
  section.set_var( "real_list", conf_real_list );
  EXPECT_EQ( expected_real_list, conf_real_list );

  vector<int> expected_int_list = { 1, 3, 5, -2, -1 };
  vector<int> conf_int_list;

  ASSERT_TRUE( section.exists( "int_list" ) );
  section.set_var( "int_list", conf_int_list );
  EXPECT_EQ( expected_int_list, conf_int_list );

  vector<string> expected_string_list = { "gosh", "look", "here" };
  vector<string> conf_string_list;

  ASSERT_TRUE( section.exists( "string_list" ) );
  section.set_var( "string_list", conf_string_list );
  EXPECT_EQ( expected_string_list, conf_string_list );

  vector<size_t> expected_uint_list = { 1, 4, 2, 5, 3 };
  vector<size_t> conf_uint_list;

  ASSERT_TRUE( section.exists( "uint_list" ) );
  section.set_var( "uint_list", conf_uint_list );
  EXPECT_EQ( expected_uint_list, conf_uint_list );

  EXPECT_EQ( 0, section.get_unset_params().size() );
}

TEST( ConfigParserTest, Random ) {
  double real_normal_sum = 0.0;
  int int_normal_sum = 0;

  size_t count = 1000;

  for( size_t i = 0; i < count; ++i ) {
    string filename = "test_files/config_parser_test/random.conf";

    ConfigParser parser( filename );

    ConfigParameters section = parser.get_section_params( "test" );

    double rand_real;

    ASSERT_TRUE( section.exists( "rand_real" ) );
    section.set_var( "rand_real", rand_real );
    EXPECT_TRUE( rand_real >= -1.0 );
    EXPECT_TRUE( rand_real <= 1.0 );

    int rand_int;

    ASSERT_TRUE( section.exists( "rand_int" ) );
    section.set_var( "rand_int", rand_int );
    EXPECT_TRUE( rand_int >= -10 );
    EXPECT_TRUE( rand_int <= 10 );

    vector<double> real_list;

    ASSERT_TRUE( section.exists( "rand_real_list" ) );
    section.set_var( "rand_real_list", real_list );
    EXPECT_EQ( 0.1, real_list[0] );
    EXPECT_EQ( 2.5, real_list[2] );
    EXPECT_TRUE( real_list[1] >= -2.0 );
    EXPECT_TRUE( real_list[1] <= 2.0 );    

    real_normal_sum += real_list[3];

    vector<int> int_list;

    ASSERT_TRUE( section.exists( "rand_int_list" ) );
    section.set_var( "rand_int_list", int_list );
    EXPECT_EQ( 4, int_list[1] );
    EXPECT_TRUE( int_list[2] >= -100 );
    EXPECT_TRUE( int_list[2] <= 100 );    

    int_normal_sum += int_list[0];

    EXPECT_EQ( 0, section.get_unset_params().size() );
  }

  double real_mean = real_normal_sum / count;

  EXPECT_TRUE( real_mean > -0.07 );
  EXPECT_TRUE( real_mean < 0.07 );

  double int_mean = (double)int_normal_sum / count;

  EXPECT_TRUE( int_mean > 9.0 );
  EXPECT_TRUE( real_mean < 11.0 );
}

TEST( ConfigParserTest, UnsetParams ) {
  string filename = "test_files/config_parser_test/unset_params.conf";

  ConfigParser parser( filename );

  ConfigParameters section = parser.get_section_params( "test" );

  EXPECT_EQ( 3, section.get_unset_params().size() );

  int i;
  
  section.set_var( "i", i );

  EXPECT_EQ( 2, section.get_unset_params().size() );
}  

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
