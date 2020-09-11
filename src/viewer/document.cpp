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
#include <vw/shader.h>
#include <vw/pipeline.h>
#include <vw/framebuffer.h>
#include <vw/image.h>
#include <vw/buffer.h>
#include <vw/wait_for_idle.h>
#include <viewer/document.h>
#include <viewer/mesh.h>
#include <viewer/shader.h>
namespace viewer {
  document_t load_gltf(
    const vw::context_t &context,
    const vw::render_pass_t &render_pass,
    std::filesystem::path path,
    uint32_t swapchain_size,
    const std::filesystem::path &shader_dir,
    int shader_mask
  ) {
    fx::gltf::Document doc = fx::gltf::LoadFromText( path.string() );
    document_t document;
    shader_t shader;
    for( auto &path: std::filesystem::directory_iterator( shader_dir ) ) {
      auto flag = get_shader_flag( path.path() );
      if( flag ) {
        shader.emplace(
          *flag,
          vw::get_shader( context, path.path().string() )
        );
      }
    }
    size_t pcsize = sizeof( push_constants_t );
    document.set_sampler( viewer::create_sampler(
      doc,
      context
    ) );
    document.set_default_sampler( viewer::create_default_sampler(
      context
    ) );
    document.set_image( viewer::create_image(
      doc,
      context,
      path.parent_path()
    ) );
    document.set_texture( viewer::create_texture(
      doc,
      context,
      document.image,
      document.sampler,
      document.default_sampler
    ) );
    document.set_mesh( viewer::create_mesh(
      doc,
      context,
      render_pass,
      pcsize,
      shader,
      document.texture,
      swapchain_size,
      shader_mask
    ) );
    document.set_buffer( viewer::create_buffer(
      doc,
      context,
      path.parent_path()
    ) );
    document.set_node( viewer::create_node(
      doc,
      context,
      document.mesh
    ) );
    return document;
  }
}

