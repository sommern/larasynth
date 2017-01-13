#include "config_directory.hpp"
#include "gtest/gtest.h"

using namespace larasynth;

class ConfigDirectoryTest : public ::testing::Test {
protected:
  std::string prefix = "test_files/config_directory_test/";
  std::string nonexistant_dir_name = prefix + "asdfasdkjkjasdkjfas";
  std::string empty_dir_name = prefix + "empty_dir";
  std::string config_only_dir_name = prefix + "config_only";
  std::string json_files_dir_name = prefix + "json_files";
};

TEST_F( ConfigDirectoryTest, EmptyName ) {
  EXPECT_ANY_THROW( ConfigDirectory dir( "" ) );
}

TEST_F( ConfigDirectoryTest, NonexistantDir ) {
  ConfigDirectory dir( nonexistant_dir_name );
  EXPECT_ANY_THROW( dir.process_directory() );
}

TEST_F( ConfigDirectoryTest, EmptyDir ) {
  ConfigDirectory dir( empty_dir_name );
  EXPECT_ANY_THROW( dir.process_directory() );
}

TEST_F( ConfigDirectoryTest, ConfigOnly ) {
  ConfigDirectory dir( config_only_dir_name );
  dir.process_directory();

  EXPECT_EQ( config_only_dir_name + "/larasynth.conf",
             dir.get_config_file_path() );

  EXPECT_FALSE( dir.training_examples_exist() );
  EXPECT_FALSE( dir.training_results_exist() );
}

TEST_F( ConfigDirectoryTest, JsonFiles ) {
  ConfigDirectory dir( json_files_dir_name );
  dir.process_directory();

  EXPECT_EQ( json_files_dir_name + "/larasynth.conf",
             dir.get_config_file_path() );
  EXPECT_TRUE( dir.training_results_exist() );
  EXPECT_TRUE( dir.training_examples_exist() );

  std::vector<std::string> result_json_files = dir.get_training_results_filenames();

  ASSERT_EQ( 1, result_json_files.size() );
  EXPECT_EQ( json_files_dir_name +
             "/training_results/results-2015-06-01-12:00:15.json",
             result_json_files[0] );

  std::vector<std::string> example_files = dir.get_training_example_filenames();

  ASSERT_EQ( 2, example_files.size() );
  EXPECT_EQ( json_files_dir_name +
             "/training_examples/example-2015-08-04-14:31:30.seq",
             example_files[0] );
  EXPECT_EQ( json_files_dir_name +
             "/training_examples/example-2015-08-04-14:35:22.seq",
             example_files[1] );  
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
