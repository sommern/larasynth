#include <vector>

#include "tokens.hpp"
#include "gtest/gtest.h"

using namespace std;
using namespace larasynth;

TEST( TokensTest, EmptyTokens ) {
  Tokens tokens;

  EXPECT_EQ( 0, tokens.size() );
  EXPECT_EQ( 0, tokens.n_tokens_remaining() );
  EXPECT_FALSE( tokens.match( COMMA ) );
  EXPECT_FALSE( tokens.match( QUOTED_STRING, RESERVED_STRING ) );

  vector<token_t> types = { INTEGER, MINUS, DOT };
  EXPECT_FALSE( tokens.match( types ) );

  vector<string> values = { "5", "43", "gosh" };
  EXPECT_FALSE( tokens.match( values ) );
}

TEST( TokensTest, OneToken ) {
  Tokens tokens;

  tokens.add( INTEGER, "4", "example:1" );

  EXPECT_EQ( 1, tokens.size() );
  EXPECT_EQ( 1, tokens.n_tokens_remaining() );

  EXPECT_FALSE( tokens.match( COMMA ) );
  EXPECT_TRUE( tokens.match( INTEGER ) );
  EXPECT_TRUE( tokens.match( "4" ) );

  vector<token_t> types = { INTEGER };
  EXPECT_TRUE( tokens.match( types ) );

  vector<string> values = { "4" };
  EXPECT_TRUE( tokens.match( values ) );

  types = { INTEGER, COMMA };
  EXPECT_FALSE( tokens.match( types ) );

  values = { "4", "gosh" };
  EXPECT_FALSE( tokens.match( values ) );

  EXPECT_EQ( "4", tokens.eat() );

  EXPECT_EQ( 0, tokens.n_tokens_remaining() );
}

TEST( TokensTest, TwoTokens ) {
  Tokens tokens;

  tokens.add( RESERVED_STRING, "hi", "test:1" );
  tokens.add( END_OF_FILE, "", "test:2" );

  EXPECT_EQ( 2, tokens.size() );
  EXPECT_EQ( 2, tokens.n_tokens_remaining() );

  EXPECT_TRUE( tokens.match( RESERVED_STRING ) );
  EXPECT_TRUE( tokens.match( RESERVED_STRING, END_OF_FILE ) );

  EXPECT_FALSE( tokens.match( END_OF_FILE ) );
  EXPECT_FALSE( tokens.match( "gosh" ) );

  vector<token_t> types = { RESERVED_STRING, END_OF_FILE };
  EXPECT_TRUE( tokens.match( types ) );

  vector<string> values = { "hi", "" };
  EXPECT_TRUE( tokens.match( values ) );

  types = { END_OF_FILE, RESERVED_STRING };
  EXPECT_FALSE( tokens.match( types ) );

  values = { "", "hi" };
  EXPECT_FALSE( tokens.match( values ) );

  EXPECT_EQ( "hi", tokens.eat() );
  EXPECT_EQ( 1, tokens.n_tokens_remaining() );
  EXPECT_EQ( "", tokens.eat() );
  EXPECT_EQ( 0, tokens.n_tokens_remaining() );
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
