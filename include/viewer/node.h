#ifndef VIEWER_NODE_H
#define VIEWER_NODE_H
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
#include <fx/gltf.h>
#include <vulkan/vulkan.hpp>
#include <glm/mat4x4.hpp>
#include <stamp/setter.h>
#include <vw/context.h>
#include <viewer/buffer.h>
#include <viewer/mesh.h>
namespace viewer {
  struct node_t {
    node_t() : has_mesh( false ), mesh( 0 ) {}
    LIBSTAMP_SETTER( matrix )
    LIBSTAMP_SETTER( children )
    LIBSTAMP_SETTER( gltf )
    LIBSTAMP_SETTER( has_mesh )
    LIBSTAMP_SETTER( mesh )
    LIBSTAMP_SETTER( min )
    LIBSTAMP_SETTER( max )
    glm::mat4 matrix;
    std::vector< node_t > children;
    fx::gltf::Node gltf;
    bool has_mesh;
    int32_t mesh;
    glm::vec3 min;
    glm::vec3 max;
  };
  node_t create_node(
    const fx::gltf::Document &doc,
    int32_t index,
    const vw::context_t &context,
    const glm::mat4 &upper,
    const meshes_t &mesh
  );
  node_t create_node(
    const fx::gltf::Document &doc,
    const vw::context_t &context,
    const meshes_t &mesh
  );
  void draw_node(
    const vw::context_t &context,
    vk::CommandBuffer &commands,
    const node_t &node,
    const meshes_t &meshes,
    const buffers_t &buffers,
    uint32_t current_frame,
    const glm::mat4 &projection,
    const glm::mat4 &lookat,
    const glm::vec3 &camera_pos,
    const glm::vec3 &light_pos
  );
}
#endif

