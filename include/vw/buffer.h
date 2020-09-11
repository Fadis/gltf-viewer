#ifndef VW_BUFFER_H
#define VW_BUFFER_H
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
#include <memory>
#include <string>
#include <vulkan/vulkan.hpp>
#include <vw/context.h>
namespace vw {
  struct buffer_t {
    LIBSTAMP_SETTER( buffer )
    LIBSTAMP_SETTER( allocation )
    std::shared_ptr< vk::Buffer > buffer;
    std::shared_ptr< VmaAllocation > allocation;
  };
  buffer_t get_buffer(
    const context_t &context,
    const vk::BufferCreateInfo &buffer_create_info,
    VmaMemoryUsage usage
  );
  buffer_t load_buffer(
    const context_t &context,
    const std::vector< uint8_t > &data,
    vk::BufferUsageFlags usage
  );
  buffer_t load_buffer_from_file(
    const context_t &context,
    const std::string &filename,
    vk::BufferUsageFlags usage
  );
}
#endif

