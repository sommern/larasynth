/*
Copyright 2017 Nathan Sommer

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

#include "write_training_example.hpp"

using namespace std;
using namespace larasynth;


void larasynth::write_events( const vector<Event*>& events,
                              const string& filename ) {
  ofstream outfile( filename );

  for( Event* event : events ) {
    write_event( event, outfile );
  }
}


void larasynth::write_events( const vector<Event>& events,
                              const string& filename ) {
  ofstream outfile( filename );

  for( Event event : events ) {
    write_event( &event, outfile );
  }
}


void larasynth::write_event( const Event* event, ofstream& outfile ) {
  ostringstream line_oss;

  switch( event->type() ) {
  case CTRL_CHANGE:
    line_oss << "ctrl " << (unsigned int)event->controller() << " "
             << (unsigned int) event->value() << " " << event->time();
    break;
  case NOTE_ON:
    line_oss << "on " << (unsigned int)event->pitch() << " "
             << (unsigned int)event->velocity() << " "
             << event->time();
    break;
  case NOTE_OFF:
    line_oss << "off " << (unsigned int)event->pitch() << " "
             << event->time();
    break;
  default:
    break;
  }

  outfile << line_oss.str() << endl;;
}
