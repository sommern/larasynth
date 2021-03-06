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

#include "rtmidi_client.hpp"

using namespace std;
using namespace larasynth;

RtMidiClient::RtMidiClient(  const string& client_name,
                             const string& their_output_port_name,
                             const string& their_input_port_name,
                             run_mode mode )
  : _midi_in( nullptr )
  , _midi_out( nullptr )
  ,_client_name( client_name )
  , _our_input_port_name( client_name + " input" )
  , _our_output_port_name( client_name + " output" )
  , _their_output_port_name( their_output_port_name )
  , _their_input_port_name( their_input_port_name )
  , _mode( mode )
{
  if( _mode == PERFORM )
    setup_midi_out();

  setup_midi_in();
}

void RtMidiClient::setup_midi_in() {
  _midi_in = new RtMidiIn( RtMidi::UNSPECIFIED, _client_name );

  size_t input_port_count = _midi_in->getPortCount();

  int input_port_i = -1;

  vector<ip_choice_t> port_choices;

  for( size_t i = 0; i < input_port_count; ++i ) {
    string port_name = _midi_in->getPortName( i );

    port_choices.emplace_back( to_string( i + 1 ), port_name );

    if( port_name == _their_output_port_name ) {
      input_port_i = i;
      break;
    }
  }

  if( input_port_i == -1 ) {
    if( _their_output_port_name != "" )
      cout << "Port " << _their_output_port_name << " does not exist." << endl;

    port_choices.emplace_back( "0",
                               "Open a port for other devices to connect to" );
    port_choices.emplace_back( "q", "quit" );

    input_port_i = pick_choice( "Select a MIDI port port to receive from:",
                                port_choices, port_choices.size() - 1 );

    cout << endl;

    if( input_port_i == (int)input_port_count + 1 )
      throw_exception( "No input port selected." );
  }  

  if( input_port_i < (int)input_port_count ) {
    _midi_in->openPort( input_port_i, _our_input_port_name );
    cout << "Receiving from port \"" << port_choices[input_port_i].description
         << "\"" << endl;
  }
  else {
    _midi_in->openVirtualPort( _our_input_port_name );
    cout << "Opened port \"" << _our_output_port_name
         << "\" for other devices to connect to" << endl;
  }

  cout << endl;

  _midi_in->setCallback( &input_callback, &_input_queue );
}

void RtMidiClient::setup_midi_out() {
  _midi_out = new RtMidiOut( RtMidi::UNSPECIFIED, _client_name );

  size_t output_port_count = _midi_out->getPortCount();

  int output_port_i = -1;

  vector<ip_choice_t> port_choices;

  for( size_t i = 0; i < output_port_count; ++i ) {
    string port_name = _midi_out->getPortName( i );

    port_choices.emplace_back( to_string( i + 1 ), port_name );

    if( port_name == _their_input_port_name ) {
      output_port_i = i;
      break;
    }
  }

  if( output_port_i == -1 ) {
    if( _their_input_port_name != "" )
      cout << "Port " << _their_input_port_name << " does not exist." << endl;

    port_choices.emplace_back( "0",
                               "Open a port for other devices to connect to" );
    port_choices.emplace_back( "q", "quit" );

    output_port_i = pick_choice( "Select a MIDI port to send to:",
                                 port_choices, port_choices.size() - 1 );

    cout << endl;

    if( output_port_i == (int)output_port_count + 1 )
      throw_exception( "No output port selected." );
  }  

  
  if( output_port_i < (int)output_port_count ) {
    _midi_out->openPort( output_port_i, _our_output_port_name );
    cout << "Sending to port " << port_choices[output_port_i].description
         << endl;
  }
  else {
    _midi_out->openVirtualPort( _our_output_port_name );
    cout << "Opened port \"" << _our_output_port_name
         << "\" for other devices to connect to" << endl;
  }

  cout << endl;
}

RtMidiClient::~RtMidiClient() {
  delete _midi_in;
  delete _midi_out;
}  

void RtMidiClient::throw_exception( string error_msg ) {
  if( _midi_in != nullptr )
    delete _midi_in;
  if( _midi_out != nullptr )
    delete _midi_out;

  throw MidiException( error_msg );
}

namespace larasynth {
  void input_callback( double delta_time, vector<unsigned char>* message,
                       void* in_q) {
    static_cast<EventQueue*>( in_q )->push( message );
  }
}
