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

#include "interactive_prompt.hpp"

using namespace std;
using namespace larasynth;

namespace larasynth {

bool pick_yes_or_no( const string& question, char default_choice ) {
  // These are the choices that will be printed. Default will be uppercase.
  char yes = 'y';
  char no = 'n';

  if( default_choice == 'y' )
    yes = toupper( yes );
  else if( default_choice == 'n' )
    no = toupper( no );

  cout << question << " (" << yes << "/" << no << ") ";

  char choice = cin.get();

  if( choice == '\n' || choice == '\r' )
    choice = default_choice;

  return tolower( choice ) == 'y';
}

size_t pick_choice( const string& question,
                    const vector< ip_choice_t >& choices,
                    int default_choice ) {
  cout << question << endl;

  for( auto& choice : choices )
    cout << "  " << choice.choice_input << ": " << choice.description << endl;

  do {
    cout << "Enter your choice";
    if( default_choice >= 0 && default_choice < (int)choices.size() )
      cout << " [" << choices[default_choice].choice_input << "]";
    cout << ": ";

    string response;
    char next_char = cin.get();

    while( next_char != '\n' && next_char != '\r' ) {
      response += next_char;
      next_char = cin.get();
    }

    if( response == "" &&
        default_choice >= 0 && default_choice < (int)choices.size() )
      response = choices[default_choice].choice_input;

    for( size_t i = 0; i < choices.size(); ++i ) {
      if( response == choices[i].choice_input )
        return i;
    }

    cout << response << " is not a valid choice." << endl;
  } while( true );
}

size_t get_positive_integer( string& question ) {
  cout << question << endl;

  do {
    bool is_positive_integer = true;
    string response;

    cout << "Response: ";
    cin >> response;

    for( size_t i = 0; i < response.size(); ++i ) {
      if( !isdigit( response[i] ) ) {
        is_positive_integer = false;
        break;
      }
    }

    if( is_positive_integer ) {
      istringstream iss( response );
      size_t value;
      iss >> value;
      return value;
    }

    cout << response << " is not an integer" << endl;

  } while( true );
}

}
