#ifndef VW_IMAGE_H
#define VW_IMAGE_H
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
  struct image_t {
    image_t() : width( 0 ), height( 0 ) {}
    LIBSTAMP_SETTER( image )
    LIBSTAMP_SETTER( allocation )
    LIBSTAMP_SETTER( image_view )
    LIBSTAMP_SETTER( width )
    LIBSTAMP_SETTER( height )
    std::shared_ptr< vk::Image > image;
    std::shared_ptr< VmaAllocation > allocation;
    vk::UniqueHandle< vk::ImageView, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > image_view;
    unsigned int width;
    unsigned int height;
  };
  image_t get_image(
    const context_t &context,
    const vk::ImageCreateInfo &image_create_info,
    VmaMemoryUsage usage
  );
  image_t load_image(
    const context_t &context,
    const std::string &filename,
    vk::ImageUsageFlagBits usage,
    bool mipmap,
    bool srgb
  );
  void dump_image(
    const context_t &context,
    const image_t &image,
    const std::string &filename,
    unsigned int mipmap
  );
}
#endif

