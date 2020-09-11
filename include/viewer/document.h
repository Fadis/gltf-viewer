#ifndef VIEWER_DOCUMENT_H
#define VIEWER_DOCUMENT_H
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
#include <vector>
#include <vulkan/vulkan.hpp>
#include <stamp/setter.h>
#include <vw/context.h>
#include <vw/render_pass.h>
#include <viewer/mesh.h>
#include <viewer/buffer.h>
#include <viewer/sampler.h>
#include <viewer/image.h>
#include <viewer/texture.h>
#include <viewer/node.h>
#include <viewer/shader.h>
namespace viewer {
  struct document_t {
    LIBSTAMP_SETTER( shader )
    LIBSTAMP_SETTER( mesh )
    LIBSTAMP_SETTER( buffer )
    LIBSTAMP_SETTER( sampler )
    LIBSTAMP_SETTER( default_sampler )
    LIBSTAMP_SETTER( image )
    LIBSTAMP_SETTER( texture )
    LIBSTAMP_SETTER( node )
    shader_t shader;
    meshes_t mesh;
    buffers_t buffer;
    samplers_t sampler;
    sampler_t default_sampler;
    images_t image;
    textures_t texture;
    node_t node;
  };
  document_t load_gltf(
    const vw::context_t &context,
    const vw::render_pass_t &render_pass,
    std::filesystem::path path,
    uint32_t swapchain_size,
    const std::filesystem::path &shader_dir,
    int shader_mask
  );
}
#endif

