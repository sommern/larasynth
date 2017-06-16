#include "littlelstm/lstm_network.hpp"
#include "littlelstm/lstm_architecture.hpp"

#include "gtest/gtest.h"

using namespace std;
using namespace littlelstm;

/**
 * Create training or validation data for the one-two-three test. Returns a
 * vector of pairs where the first item in the pair is an integer, either 1, 2,
 * or 3, and the second item is a boolean value. The boolean value is true if
 * the integer value is a 3, and the 3 completed a sequence of 1, 2, 3. For
 * example:
 *
 * 3 false
 * 1 false
 * 2 false
 * 1 false
 * 2 false
 * 3 true   <- true becaus this completes a 1, 2, 3 sequence
 * 3 false
 * ...
 */
vector< pair<int, bool> > make_one_two_three_data( RandGen& rand ) {
  vector< pair<int, bool> > training_data;

  int sequence_progress = -1;

  for( size_t count = 0; count < 200; ++count ) {
    int next;

    // 2/3 chance of continuing the sequence
    if( rand.uniform_int( 0, 2 ) ) {
      next = sequence_progress + 1;
    }
    else {
      next = rand.uniform_int( 0, 2 );
    }
      
    bool target;

    if( next == sequence_progress + 1 )
      sequence_progress = next;
    else
      sequence_progress = -1;

    if( sequence_progress == 2 ) {
      target = true;
      sequence_progress = -1;
    }
    else {
      target = false;
    }

    training_data.emplace_back( next, target );
  }

  return training_data;
}

/**
 * Return the index of the largest value in v.
 */
size_t argmax( vector<double> v ) {
  double max = v[0];
  size_t max_i = 0;

  for( size_t i = 1; i < v.size(); ++i ) {
    if( v[i] > max ) {
      max = v[i];
      max_i = i;
    }
  }

  return max_i;
}

/**
 * Ensure we can create a network at all.
 */
TEST( LstmNetworkTest, CreateNetwork ) {
  LstmArchitecture arch( 3, 1, { 21 } );
  LstmNetwork network( arch );

  ASSERT_EQ( 3, network.get_input_size() );
  ASSERT_EQ( 1, network.get_output_size() );
}

/**
 * Ensure we can feed forward and get output.
 */
TEST( LstmNetworkTest, FeedForward ) {
  LstmArchitecture arch( 3, 1, { 21 } );
  LstmNetwork network( arch );

  vector<double> input = { 1, 0, 0 };
  vector<double> output;

  network.feed_forward( input );
  output = network.get_output();

  ASSERT_EQ( 1, output.size() );
}

/**
 * Ensure the network can learn to always output a vector where the first value
 * is alwasy larger than the second value.
 */
TEST( LstmNetworkTest, LearnAlwaysFirst ) {
  LstmArchitecture arch( 3, 2, { 3, 5 } );
  LstmNetwork network( arch );

  RandGen rand;

  vector<double> input( 3 );
  vector<double> output;

  vector<double> target = { 1.0, 0.0 };

  bool perfect = false;

  for( size_t epoch = 0; epoch < 10; ++epoch ) {
    network.zero_network();

    // train
    for( size_t i = 0; i < input.size(); i++ )
      input[i] = rand.uniform_real( 0.0, 1.0 );

    network.feed_forward( input );
    network.backpropagate( target, 0.05, 0.8 );
      
    network.zero_network();

    // validate
    size_t correct_count = 0;
    size_t max_i = 0;

    for( size_t count = 0; count < 10; ++count ) {
      for( size_t i = 0; i < input.size(); i++ )
        input[i] = rand.uniform_real( 0.0, 1.0 );

      network.feed_forward( input );
      output = network.get_output();

      max_i = argmax( output );

      if( max_i == 0 )
        correct_count++;
    }

    if( correct_count == 10 )
      perfect = true;
  }

  ASSERT_EQ( true, perfect );
}

/**
 * Ensure the network can learn to detect the following sequence within a
 * training sequence:
 *
 * {1, 0, 0}
 * {0, 1, 0}
 * {0, 0, 1}
 */
TEST( LstmNetworkTest, LearnOneTwoThree ) {
  LstmArchitecture arch( 3, 2, { 3 } );
  LstmNetwork network( arch );

  RandGen rand;

  vector<double> input( 3 );
  vector<double> output;

  vector<double> target( 2 );

  bool perfect = false;

  for( size_t epoch = 0; epoch < 1000 && !perfect; ++epoch ) {
    network.zero_network();
    vector< pair<int, bool> > training_data = make_one_two_three_data( rand );

    // train
    for( auto& kv : training_data ) {
      input = { 0, 0, 0 };
      input[kv.first] = 1.0;

      target[0] = 0.0;
      target[1] = 0.0;

      if( kv.second )
        target[1] = 1.0;
      else
        target[0] = 1.0;

      network.feed_forward( input );

      output = network.get_output();

      if( kv.second != ( output[0] < output[1] ) )
        network.backpropagate( target, 0.05, 0.8 );
    }
      
    network.zero_network();
    vector< pair<int, bool> > validation_data = make_one_two_three_data( rand );

    // validate
    size_t correct_count = 0;
    size_t max_i = 0;

    for( auto& kv : training_data ) {
      input = { 0, 0, 0 };
      input[kv.first] = 1.0;

      target[0] = 0.0;
      target[1] = 0.0;

      network.feed_forward( input );
      output = network.get_output();

      if( kv.second == ( output[0] < output[1] ) ) {
        correct_count++;
      }
      else {
        break;
      }
    }

    if( correct_count == training_data.size() ) {
      perfect = true;
    }
  }

  ASSERT_EQ( true, perfect );
}  

int main( int argc, char ** argv ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}
