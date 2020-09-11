#ifndef VW_CONFIG_H
#define VW_CONFIG_H
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
#include <string>
#include <stamp/setter.h>
namespace vw {
  struct configs_t {
    configs_t() : list( false ), device_index( 0 ), width( 0 ), height( 0 ), fullscreen( false ), validation( false ), direct( false ), purple( false ), shader_mask( 0 ) {}
    LIBSTAMP_SETTER( prog_name )
    LIBSTAMP_SETTER( list )
    LIBSTAMP_SETTER( device_index )
    LIBSTAMP_SETTER( display_index )
    LIBSTAMP_SETTER( width )
    LIBSTAMP_SETTER( height )
    LIBSTAMP_SETTER( fullscreen )
    LIBSTAMP_SETTER( validation )
    LIBSTAMP_SETTER( direct )
    LIBSTAMP_SETTER( input )
    LIBSTAMP_SETTER( purple )
    LIBSTAMP_SETTER( shader )
    LIBSTAMP_SETTER( shader_mask )
    std::string prog_name; 
    bool list;
    unsigned int device_index;
    unsigned int display_index;
    unsigned int width;
    unsigned int height;
    bool fullscreen;
    bool validation;
    bool direct;
    std::string input;
    bool purple;
    std::string shader;
    int shader_mask;
  };
  configs_t parse_configs( int argc, const char *argv[] );
}
#endif
