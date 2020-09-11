/*
 * Copyright (C) 2020 Naomasa Matsubayashi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#include <iostream>
#include <boost/filesystem/path.hpp>
#include <boost/program_options.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/spirit/include/qi.hpp>
#include <cstdlib>
#include <unistd.h>
#include <vw/config.h>
namespace vw {
  configs_t parse_configs( int argc, const char *argv[] ) {
    namespace po = boost::program_options;
    po::options_description desc( "Options" );
    std::string window_size;
    std::string device;
    std::string input;
    std::string shader;
    bool purple = false;
    int shader_mask = 0;
    desc.add_options()
      ( "help,h", "show this message" )
      ( "list,l", "show all available devices" )
      ( "window,w", po::value< std::string >(&window_size)->default_value( "native" ), "window size" )
      ( "fullscreen,f", "fullscreen" )
      ( "direct,D", "direct" )
      ( "device,d", po::value< std::string >(&device)->default_value( "0.0" ), "use specific device" )
      ( "validation,v", "use VK_LAYER_LUNARG_standard_validation" )
      ( "purple,p", po::bool_switch( &purple ), "make background purple" )
      ( "shader,s", po::value< std::string >(&shader)->default_value( "../shaders/" ), "shader dir" )
      ( "shader_mask,m", po::value< int >(&shader_mask)->default_value( 0 ), "shader mask" )
      ( "input,i", po::value< std::string >(&input)->default_value( "hoge.gltf" ), "glTF file path" );
    po::variables_map vm;
    po::store( po::parse_command_line( argc, argv, desc ), vm );
    po::notify( vm );
    if( vm.count( "help" ) ) {
      std::cout << desc << std::endl;
      exit( 0 );
    }
    namespace qi = boost::spirit::qi;
    boost::fusion::vector< unsigned int, unsigned int > parsed_device;
    {
      auto iter = device.begin();
      const auto end = device.end();
      if( !qi::parse( iter, end, qi::uint_ >> '.' >> qi::uint_, parsed_device ) ) {
        std::cerr << "不正なデバイス: " << device << std::endl;
        exit( 1 );
      }
    }
    if( window_size != "native" ) {
      boost::fusion::vector< unsigned int, unsigned int > parsed_window_size;
      {
        auto iter = window_size.begin();
        const auto end = window_size.end();
        if( !qi::parse( iter, end, qi::uint_ >> 'x' >> qi::uint_, parsed_window_size ) ) {
          std::cerr << "不正なウィンドウサイズ: " << window_size << std::endl;
          exit( 1 );
        }
      }
      return configs_t()
        .set_prog_name( boost::filesystem::path( argv[ 0 ] ).filename().native() )
        .set_device_index( boost::fusion::at_c< 0 >( parsed_device ) )
        .set_display_index( boost::fusion::at_c< 1 >( parsed_device ) )
        .set_list( vm.count( "list" ) )
        .set_width( boost::fusion::at_c< 0 >( parsed_window_size ) )
        .set_height( boost::fusion::at_c< 1 >( parsed_window_size ) )
        .set_fullscreen( vm.count( "fullscreen" ) )
        .set_validation( vm.count( "validation" ) )
        .set_direct( vm.count( "direct" ) )
        .set_input( std::move( input ) )
        .set_purple( purple )
        .set_shader( std::move( shader ) )
        .set_shader_mask( shader_mask );
    }
    else {
      return configs_t()
        .set_prog_name( boost::filesystem::path( argv[ 0 ] ).filename().native() )
        .set_device_index( boost::fusion::at_c< 0 >( parsed_device ) )
        .set_display_index( boost::fusion::at_c< 1 >( parsed_device ) )
        .set_list( vm.count( "list" ) )
        .set_width( 0 )
        .set_height( 0 )
        .set_fullscreen( vm.count( "fullscreen" ) )
        .set_validation( vm.count( "validation" ) )
        .set_direct( vm.count( "direct" ) )
        .set_input( std::move( input ) )
        .set_purple( purple )
        .set_shader( std::move( shader ) )
        .set_shader_mask( shader_mask );
    }
  }
}
