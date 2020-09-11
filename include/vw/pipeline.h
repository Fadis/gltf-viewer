#ifndef VW_PIPELINE_H
#define VW_PIPELINE_H
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
#include <vw/config.h>
#include <vw/context.h>
#include <vw/render_pass.h>
namespace vw {
  struct pipeline_t {
    LIBSTAMP_SETTER( pipeline_layout )
    LIBSTAMP_SETTER( pipeline )
    vk::UniqueHandle< vk::PipelineLayout, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > pipeline_layout;
    vk::UniqueHandle< vk::Pipeline, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > pipeline;
  };
  pipeline_t create_pipeline(
    const context_t &context,
    const render_pass_t &render_pass,
    uint32_t push_constant_size,
    const vk::ShaderModule &vs,
    const vk::ShaderModule &fs,
    const std::vector< vk::VertexInputBindingDescription > &vertex_input_binding,
    const std::vector< vk::VertexInputAttributeDescription > &vertex_input_attribute,
    bool cull,
    bool blend
  );
}
#endif

