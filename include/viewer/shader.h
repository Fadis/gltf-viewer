#ifndef VIEWER_SHADER_H
#define VIEWER_SHADER_H
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
#include <unordered_map>
#include <filesystem>
#include <optional>
#include <vulkan/vulkan.hpp>
namespace viewer {
  enum class shader_flag_t {
    base_color = ( 1 << 0 ),
    metallic_roughness = ( 1 << 1 ),
    normal = ( 1 << 2 ),
    occlusion = ( 1 << 3 ),
    emissive = ( 1 << 4 ),
    tangent = ( 1 << 5 ),
    skin = ( 1 << 6 ),
    vertex = ( 1 << 7 ),
    fragment = ( 1 << 8 ),
    special = ( 1 << 9 )
  };
  using shader_t = std::unordered_map< shader_flag_t, vk::UniqueHandle< vk::ShaderModule, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > >;
  std::optional< shader_flag_t > get_shader_flag( const std::filesystem::path &path );
}
#endif

