#ifndef VIEWER_MESH_H
#define VIEWER_MESH_H
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
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <fx/gltf.h>
#include <stamp/setter.h>
#include <vw/pipeline.h>
#include <vw/context.h>
#include <vw/render_pass.h>
#include <vw/buffer.h>
#include <viewer/texture.h>
#include <viewer/shader.h>
#include <viewer/buffer.h>
namespace viewer {
  struct buffer_view_t {
    buffer_view_t() : index( 0 ), offset( 0 ) {}
    LIBSTAMP_SETTER( index )
    LIBSTAMP_SETTER( offset )
    uint32_t index;
    uint32_t offset;
  };
  struct descriptor_set_t {
    LIBSTAMP_SETTER( descriptor_set )
    std::vector< vk::UniqueHandle< vk::DescriptorSet, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > > descriptor_set;
  };
  struct primitive_t {
    primitive_t() : indexed( false ), count( 0 ) {}
    LIBSTAMP_SETTER( pipeline )
    LIBSTAMP_SETTER( vertex_buffer )
    LIBSTAMP_SETTER( indexed )
    LIBSTAMP_SETTER( index_buffer )
    LIBSTAMP_SETTER( descriptor_set )
    LIBSTAMP_SETTER( count )
    LIBSTAMP_SETTER( index_buffer_type )
    LIBSTAMP_SETTER( min )
    LIBSTAMP_SETTER( max )
    LIBSTAMP_SETTER( uniform_buffer )
    vw::pipeline_t pipeline;
    std::unordered_map< uint32_t, buffer_view_t > vertex_buffer;
    bool indexed;
    buffer_view_t index_buffer;
    std::vector< descriptor_set_t > descriptor_set;
    uint32_t count;
    vk::IndexType index_buffer_type;
    glm::vec3 min;
    glm::vec3 max;
    buffer_t uniform_buffer;
  };
  struct uniforms_t {
    LIBSTAMP_SETTER( base_color )
    LIBSTAMP_SETTER( roughness )
    LIBSTAMP_SETTER( metalness )
    LIBSTAMP_SETTER( emissive )
    LIBSTAMP_SETTER( normal_scale )
    LIBSTAMP_SETTER( occlusion_strength )
    glm::vec4 base_color;
    glm::vec4 emissive;
    float roughness;
    float metalness;
    float normal_scale;
    float occlusion_strength;
  };
  struct push_constants_t {
    LIBSTAMP_SETTER( world_matrix )
    LIBSTAMP_SETTER( projection_matrix )
    LIBSTAMP_SETTER( camera_matrix )
    LIBSTAMP_SETTER( eye_pos )
    LIBSTAMP_SETTER( light_pos )
    glm::mat4 world_matrix;
    glm::mat4 projection_matrix;
    glm::mat4 camera_matrix;
    glm::vec4 eye_pos;
    glm::vec4 light_pos;
  };
  struct mesh_t {
    LIBSTAMP_SETTER( primitive )
    LIBSTAMP_SETTER( min )
    LIBSTAMP_SETTER( max )
    std::vector< primitive_t > primitive;
    glm::vec3 min;
    glm::vec3 max;
  };
  using meshes_t = std::vector< mesh_t >;
  mesh_t create_mesh(
    const fx::gltf::Document &doc,
    int32_t index,
    const vw::context_t &context,
    const vw::render_pass_t &render_pass,
    uint32_t push_constant_size,
    const shader_t &shader,
    const textures_t &textures,
    uint32_t swapchain_size,
    int shader_mask
  );
  meshes_t create_mesh(
    const fx::gltf::Document &doc,
    const vw::context_t &context,
    const vw::render_pass_t &render_pass,
    uint32_t push_constant_size,
    const shader_t &shader,
    const textures_t &textures,
    uint32_t swapchain_size,
    int shader_mask
  );
}
#endif

