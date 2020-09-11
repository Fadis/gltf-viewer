#ifndef VW_FRAMEBUFFER_H
#define VW_FRAMEBUFFER_H
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
#include <vw/context.h>
#include <vw/render_pass.h>
#include <vw/image.h>
namespace vw {
  struct framebuffer_t {
    LIBSTAMP_SETTER( swapchain_image_view )
    LIBSTAMP_SETTER( depth_image )
    LIBSTAMP_SETTER( depth_image_view )
    LIBSTAMP_SETTER( framebuffer )
    vk::UniqueHandle< vk::ImageView, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > swapchain_image_view;
    image_t depth_image;
    vk::UniqueHandle< vk::ImageView, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > depth_image_view;
    vk::UniqueHandle< vk::Framebuffer, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > framebuffer;
  };
  struct framebuffer_fence_t {
    LIBSTAMP_SETTER( fence )
    LIBSTAMP_SETTER( image_acquired_semaphore )
    LIBSTAMP_SETTER( draw_complete_semaphore )
    LIBSTAMP_SETTER( image_ownership_semaphore )
    vk::UniqueHandle< vk::Fence, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > fence;
    vk::UniqueHandle< vk::Semaphore, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > image_acquired_semaphore;
    vk::UniqueHandle< vk::Semaphore, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > draw_complete_semaphore;
    vk::UniqueHandle< vk::Semaphore, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > image_ownership_semaphore;
  };
  std::vector< framebuffer_t > create_framebuffer(
    const context_t &context,
    const render_pass_t &render_pass
  );
  std::vector< framebuffer_fence_t > create_framebuffer_fences(
    const context_t &context,
    uint32_t image_count
  );
}
#endif

