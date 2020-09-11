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
#include <vulkan/vulkan.hpp>
#include <fx/gltf.h>
#include <vw/buffer.h>
#include <viewer/buffer.h>
namespace viewer {
  buffer_t create_uniform_buffer(
    const vw::context_t &context,
    const std::vector< uint8_t > &data
  ) {
    return buffer_t()
      .set_buffer(
        vw::load_buffer( context, data, vk::BufferUsageFlagBits::eUniformBuffer )
      );
  }
  buffers_t create_buffer(
    const fx::gltf::Document &doc,
    const vw::context_t &context,
    const std::filesystem::path cd
  ) {
    buffers_t buffers;
    for( const auto &buffer: doc.buffers ) {
      auto buffer_path = std::filesystem::path( buffer.uri );
      if( buffer_path.is_relative() ) buffer_path = cd / buffer_path;
      buffers.push_back(
        buffer_t()
          .set_buffer(
            vw::load_buffer_from_file( context, buffer_path.string(), vk::BufferUsageFlagBits::eVertexBuffer|vk::BufferUsageFlagBits::eIndexBuffer )
          )
      );
    }
    return buffers;
  }
}

