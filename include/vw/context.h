#ifndef VW_CONTEXT_H
#define VW_CONTEXT_H
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
#include <memory>
#include <variant>
#include <vulkan/vulkan.hpp>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wreorder"
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#include <vk_mem_alloc.h>
#pragma GCC diagnostic pop
#include <vw/config.h>
#include <vw/glfw.h>
namespace vw {
  struct display_info_t {
    LIBSTAMP_SETTER( display )
    LIBSTAMP_SETTER( display_mode )
    vk::DisplayPropertiesKHR display;
    vk::DisplayModePropertiesKHR display_mode;
  };
  struct input_state_t {
    input_state_t() :
      quit( false ),
      w( false ),
      s( false ),
      a( false ),
      d( false ),
      e( false ),
      c( false ),
      up( false ),
      down( false ),
      left( false ),
      right( false ) {}
    bool quit;
    bool w;
    bool s;
    bool a;
    bool d;
    bool e;
    bool c;
    bool up;
    bool down;
    bool left;
    bool right;
  };
  struct window_info_t {
    LIBSTAMP_SETTER( window )
    std::shared_ptr< GLFWwindow > window;
  };
  struct context_t {
    context_t() : graphics_queue_index( 0 ), present_queue_index( 0 ), surface_format( vk::Format::eUndefined ), swapchain_image_count( 0 ), width( 0 ), height( 0 ), input_state( new input_state_t() ) {}
    LIBSTAMP_SETTER( physical_device )
    LIBSTAMP_SETTER( surface )
    LIBSTAMP_SETTER( window )
    LIBSTAMP_SETTER( graphics_queue_index )
    LIBSTAMP_SETTER( present_queue_index )
    LIBSTAMP_SETTER( device )
    LIBSTAMP_SETTER( graphics_command_pool )
    LIBSTAMP_SETTER( present_command_pool )
    LIBSTAMP_SETTER( surface_format )
    LIBSTAMP_SETTER( swapchain_extent )
    LIBSTAMP_SETTER( swapchain_image_count )
    LIBSTAMP_SETTER( swapchain )
    LIBSTAMP_SETTER( descriptor_pool )
    LIBSTAMP_SETTER( descriptor_set_layout )
    LIBSTAMP_SETTER( descriptor_set )
    LIBSTAMP_SETTER( allocator )
    LIBSTAMP_SETTER( pipeline_cache )
    LIBSTAMP_SETTER( width )
    LIBSTAMP_SETTER( height )
    LIBSTAMP_SETTER( input_state )
    vk::PhysicalDevice physical_device;
    vk::UniqueHandle<vk::SurfaceKHR, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > surface;
    std::variant< display_info_t, window_info_t > window;
    uint32_t graphics_queue_index; 
    uint32_t present_queue_index;
    vk::UniqueHandle<vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > device;
    vk::UniqueHandle<vk::CommandPool, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > graphics_command_pool;
    vk::UniqueHandle<vk::CommandPool, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > present_command_pool;
    vk::SurfaceFormatKHR surface_format;
    vk::Extent2D swapchain_extent;
    uint32_t swapchain_image_count;
    vk::UniqueHandle< vk::SwapchainKHR, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > swapchain;
    vk::UniqueHandle< vk::DescriptorPool, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > descriptor_pool;
    std::vector< vk::UniqueHandle< vk::DescriptorSetLayout, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > > descriptor_set_layout;
    std::vector< vk::UniqueHandle< vk::DescriptorSet, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > > descriptor_set;
    std::shared_ptr< VmaAllocator > allocator;
    vk::UniqueHandle< vk::PipelineCache, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > pipeline_cache;
    unsigned int width;
    unsigned int height;
    std::shared_ptr< input_state_t > input_state;
  };
  context_t create_context(
    const vk::Instance &instance,
    const vw::configs_t &configs,
    const std::vector< const char* > &dext,
    const std::vector< const char* > &dlayers,
    const std::vector< vk::DescriptorPoolSize > &descriptor_pool_size,
    const std::vector< vk::DescriptorSetLayoutBinding > &descriptor_set_layout_bindings
  );
}
#endif

