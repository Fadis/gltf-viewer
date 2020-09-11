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
#include <string>
#include <iostream>
#include <iterator>
#include <iostream>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <vw/config.h>
#include <vw/context.h>
#include <vw/exceptions.h>
#include <vw/glfw.h>
namespace vw {
  void on_key_event( GLFWwindow *raw_window, int key, int, int action, int ) {
    auto input_state = reinterpret_cast< input_state_t* >( glfwGetWindowUserPointer( raw_window ) );
    if( input_state ) {
      if( key == GLFW_KEY_Q && action == GLFW_PRESS ) input_state->quit = true;
      else if( key == GLFW_KEY_W && action == GLFW_PRESS ) input_state->w = true;
      else if( key == GLFW_KEY_A && action == GLFW_PRESS ) input_state->a = true;
      else if( key == GLFW_KEY_S && action == GLFW_PRESS ) input_state->s = true;
      else if( key == GLFW_KEY_D && action == GLFW_PRESS ) input_state->d = true;
      else if( key == GLFW_KEY_E && action == GLFW_PRESS ) input_state->e = true;
      else if( key == GLFW_KEY_C && action == GLFW_PRESS ) input_state->c = true;
      else if( key == GLFW_KEY_W && action == GLFW_RELEASE ) input_state->w = false;
      else if( key == GLFW_KEY_A && action == GLFW_RELEASE ) input_state->a = false;
      else if( key == GLFW_KEY_S && action == GLFW_RELEASE ) input_state->s = false;
      else if( key == GLFW_KEY_D && action == GLFW_RELEASE ) input_state->d = false;
      else if( key == GLFW_KEY_E && action == GLFW_RELEASE ) input_state->e = false;
      else if( key == GLFW_KEY_C && action == GLFW_RELEASE ) input_state->c = false;
      else if( key == GLFW_KEY_UP && action == GLFW_PRESS ) input_state->up = true;
      else if( key == GLFW_KEY_DOWN && action == GLFW_PRESS ) input_state->down = true;
      else if( key == GLFW_KEY_LEFT && action == GLFW_PRESS ) input_state->left = true;
      else if( key == GLFW_KEY_RIGHT && action == GLFW_PRESS ) input_state->right = true;
      else if( key == GLFW_KEY_UP && action == GLFW_RELEASE ) input_state->up = false;
      else if( key == GLFW_KEY_DOWN && action == GLFW_RELEASE ) input_state->down = false;
      else if( key == GLFW_KEY_LEFT && action == GLFW_RELEASE ) input_state->left = false;
      else if( key == GLFW_KEY_RIGHT && action == GLFW_RELEASE ) input_state->right = false;
    }
  }

  context_t create_context(
    const vk::Instance &instance,
    const vw::configs_t &configs,
    const std::vector< const char* > &dext,
    const std::vector< const char* > &dlayers,
    const std::vector< vk::DescriptorPoolSize > &descriptor_pool_size,
    const std::vector< vk::DescriptorSetLayoutBinding > &descriptor_set_layout_bindings
  ) {
    context_t context;
    const auto devices = instance.enumeratePhysicalDevices();
    if( devices.size() <= configs.device_index )
      throw unable_to_create_surface{};
    context.set_physical_device( devices[ configs.device_index ] );
    const auto avail_dext = context.physical_device.enumerateDeviceExtensionProperties();
    for( const char *w: dext ) {
      if( std::find_if( avail_dext.begin(), avail_dext.end(), [&]( const auto &v ) { return !strcmp( v.extensionName, w ); } ) == avail_dext.end() )
        throw unable_to_create_surface{};
    }
    const auto avail_dlayers = context.physical_device.enumerateDeviceLayerProperties();
    for( const char *w: dlayers ) {
      if( std::find_if( avail_dlayers.begin(), avail_dlayers.end(), [&]( const auto &v ) { return !strcmp( v.layerName, w ); } ) == avail_dlayers.end() )
        throw unable_to_create_surface{};
    }
    vk::DisplayPropertiesKHR display;
    uint32_t width = configs.width;
    uint32_t height = configs.height;
    if( configs.direct ) {
      const auto displays = context.physical_device.getDisplayPropertiesKHR();
      if( displays.size() <= configs.display_index )
        throw unable_to_create_surface{};
      display = displays[ configs.display_index ];
      auto modes = context.physical_device.getDisplayModePropertiesKHR( display.display );
      if( width == 0 && height == 0 ) {
        width = display.physicalResolution.width;
        height = display.physicalResolution.height;
      }
      std::vector< vk::DisplayModePropertiesKHR > available_modes;
      std::copy_if( modes.begin(), modes.end(), std::back_inserter( available_modes ), [&]( const auto &m ) { return m.parameters.visibleRegion.width == width && m.parameters.visibleRegion.height == height; } );
      if( available_modes.empty() )
        throw unable_to_create_surface{};
      auto highest_rate = std::max_element( available_modes.begin(), available_modes.end(), []( const auto &l, const auto &r ) { return l.parameters.refreshRate < r.parameters.refreshRate; } );
      auto &mode = *highest_rate;
      auto planes = context.physical_device.getDisplayPlanePropertiesKHR();
      uint32_t plane_index = 0u;
      for( uint32_t pi = 0u; pi != planes.size(); ++pi ) {
        auto planes = context.physical_device.getDisplayPlaneSupportedDisplaysKHR( pi );
        if( std::find( planes.begin(), planes.end(), display.display ) != planes.end() )
          plane_index = pi;
      }
      context.set_surface( instance.createDisplayPlaneSurfaceKHRUnique(
        vk::DisplaySurfaceCreateInfoKHR()
          .setDisplayMode( mode.displayMode )
          .setPlaneIndex( plane_index )
          .setPlaneStackIndex( 0 )
          .setImageExtent( mode.parameters.visibleRegion )
      ) );
      const auto props = context.physical_device.getProperties();
      std::cout << "デバイス " << props.deviceName << " に接続されているディスプレイ " << display.displayName << " に " << mode.parameters.visibleRegion.width << "x" << mode.parameters.visibleRegion.height << "@" << double( mode.parameters.refreshRate )/1000.0 << "Hz のサーフェスが作成されました" << std::endl;
    }
    else {
      glfwSetErrorCallback( []( int, const char* description ) { std::cout << description << std::endl; } );
      glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
      if( configs.width == 0 && configs.height == 0 ) {
        width = 640;
        height = 480;
      }
      else {
        width = configs.width;
        height = configs.height;
      }
      auto raw_window = glfwCreateWindow( width, height, configs.prog_name.c_str(), configs.fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr );
      if( !raw_window ) {
        throw unable_to_create_surface{};
      }
      int width_;
      int height_;
      glfwGetWindowSize( raw_window, &width_, &height_ );
      width = width_;
      height = height_;
      glfwSetWindowUserPointer( raw_window, reinterpret_cast< void* >( context.input_state.get() ) );
      glfwSetKeyCallback( raw_window, on_key_event );
      context.set_window(
        window_info_t()
          .emplace_window( raw_window, glfw_window_deleter() )
      );
      VkSurfaceKHR raw_surface;
      VkResult err = glfwCreateWindowSurface( instance, raw_window, nullptr, &raw_surface );
      if( err ) {
        vk::throwResultException( vk::Result( err ), "サーフェスを作成できない" );
        throw unable_to_create_surface{};
      }
      vk::ObjectDestroy< vk::Instance, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > deleter( instance, nullptr, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE () );
      context.set_surface( vk::UniqueHandle< vk::SurfaceKHR, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE >( raw_surface, deleter ) );
    }
    const auto queue_props = context.physical_device.getQueueFamilyProperties();
    std::vector< VkBool32 > supported;
    supported.reserve( queue_props.size() );
    for( uint32_t i = 0; i < queue_props.size(); ++i )
      supported.emplace_back( context.physical_device.getSurfaceSupportKHR( i, *context.surface ) );
    context.set_graphics_queue_index( std::distance( queue_props.begin(), std::find_if( queue_props.begin(), queue_props.end(), []( const auto &v ) { return bool( v.queueFlags & vk::QueueFlagBits::eGraphics ); } ) ) );
    context.set_present_queue_index(
      ( context.graphics_queue_index != queue_props.size() && supported[ context.graphics_queue_index ] == VK_TRUE ) ?
      context.graphics_queue_index :
      std::distance( supported.begin(), std::find( supported.begin(), supported.end(), VK_TRUE ) )
    );
    if( context.graphics_queue_index == queue_props.size() || context.present_queue_index == queue_props.size() ) {
      std::cerr << "必要なキューが備わっていない " << std::endl;
      throw unable_to_create_surface{};
    }
    const float priority = 0.0f;
    std::vector< vk::DeviceQueueCreateInfo > queues{
      vk::DeviceQueueCreateInfo()
        .setQueueFamilyIndex( context.graphics_queue_index ).setQueueCount( 1 ).setPQueuePriorities( &priority )
    };
    if ( context.graphics_queue_index != context.present_queue_index ) {
      queues.emplace_back(
        vk::DeviceQueueCreateInfo()
          .setQueueFamilyIndex( context.present_queue_index ).setQueueCount( 1 ).setPQueuePriorities( &priority )
      );
    }
    const auto features = context.physical_device.getFeatures();
    context.set_device( context.physical_device.createDeviceUnique(
      vk::DeviceCreateInfo()
        .setQueueCreateInfoCount( queues.size() )
        .setPQueueCreateInfos( queues.data() )
        .setEnabledExtensionCount( dext.size() )
        .setPpEnabledExtensionNames( dext.data() )
        .setEnabledLayerCount( dlayers.size() )
        .setPpEnabledLayerNames( dlayers.data() )
        .setPEnabledFeatures( &features )
    ) );
    context.set_graphics_command_pool( context.device->createCommandPoolUnique(
      vk::CommandPoolCreateInfo()
        .setQueueFamilyIndex( context.graphics_queue_index )
        .setFlags( vk::CommandPoolCreateFlagBits::eResetCommandBuffer )
    ) );
    context.set_present_command_pool( context.device->createCommandPoolUnique(
      vk::CommandPoolCreateInfo()
        .setQueueFamilyIndex( context.present_queue_index )
        .setFlags( vk::CommandPoolCreateFlagBits::eResetCommandBuffer )
    ) );
    const auto formats = context.physical_device.getSurfaceFormatsKHR( *context.surface );
    if( formats.empty() ) {
      std::cerr << "利用可能なピクセルフォーマットが無い" << std::endl;
      throw unable_to_create_surface{};
    }
    const std::vector< vk::Format > supported_formats {
      vk::Format::eA2B10G10R10UnormPack32,
      vk::Format::eA2R10G10B10UnormPack32,
      vk::Format::eB8G8R8A8Unorm,
      vk::Format::eR8G8B8A8Unorm,
      vk::Format::eA8B8G8R8UnormPack32,
      vk::Format::eB5G5R5A1UnormPack16,
      vk::Format::eA1R5G5B5UnormPack16
    };
    bool surface_format_selected = false;
    for( const auto &s: supported_formats ) {
      auto selected_format = std::find_if( formats.begin(), formats.end(), [&]( const auto &v ) { return v.format == s && v.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear; } );
      if( selected_format != formats.end() ) {
        context.set_surface_format( *selected_format );
        surface_format_selected = true;
        break;
      }
    }
    if( !surface_format_selected ) {
      std::cerr << "互換性のあるピクセルフォーマットが無い" << std::endl;
      throw unable_to_create_surface{};
    }
    const auto surface_capabilities = context.physical_device.getSurfaceCapabilitiesKHR( *context.surface );
    const auto present_modes = context.physical_device.getSurfacePresentModesKHR( *context.surface );
    if ( surface_capabilities.currentExtent.width == static_cast< uint32_t >( -1 ) ) {
      context.swapchain_extent.width = width;
      context.swapchain_extent.height = height;
    } else {
      context.set_swapchain_extent( surface_capabilities.currentExtent );
      width = surface_capabilities.currentExtent.width;
      height = surface_capabilities.currentExtent.height;
    }
    context.swapchain_image_count = std::min( surface_capabilities.minImageCount + 1, surface_capabilities.maxImageCount );
    context.set_swapchain( context.device->createSwapchainKHRUnique(
      vk::SwapchainCreateInfoKHR()
        .setSurface( *context.surface )
        .setMinImageCount( context.swapchain_image_count )
        .setImageFormat( context.surface_format.format )
        .setImageColorSpace( context.surface_format.colorSpace )
        .setImageExtent( { context.swapchain_extent.width, context.swapchain_extent.height } )
        .setImageArrayLayers( 1 )
        .setImageUsage( vk::ImageUsageFlagBits::eColorAttachment )
        .setImageSharingMode( vk::SharingMode::eExclusive )
        .setQueueFamilyIndexCount( 1 )
        .setPQueueFamilyIndices( &context.present_queue_index )
        .setPreTransform(
          ( surface_capabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity ) ?
          vk::SurfaceTransformFlagBitsKHR::eIdentity :
          surface_capabilities.currentTransform
        )
        .setCompositeAlpha( vk::CompositeAlphaFlagBitsKHR::eOpaque )
        .setPresentMode( vk::PresentModeKHR::eFifo )
        .setClipped( true )
    ) );
    context.set_descriptor_pool( context.device->createDescriptorPoolUnique(
      vk::DescriptorPoolCreateInfo()
        .setPoolSizeCount( descriptor_pool_size.size() )
        .setPPoolSizes( descriptor_pool_size.data() )
        .setMaxSets( 1000 )
        .setFlags( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet )
    ) );
    for( size_t i = 0u; i != context.swapchain_image_count; ++i ) {
      context.descriptor_set_layout.emplace_back( context.device->createDescriptorSetLayoutUnique(
        vk::DescriptorSetLayoutCreateInfo()
          .setBindingCount( descriptor_set_layout_bindings.size() )
          .setPBindings( descriptor_set_layout_bindings.data() ),
        nullptr
      ) );
    }
    std::vector< vk::DescriptorSetLayout > raw_descriptor_set_layout;
    raw_descriptor_set_layout.reserve( context.descriptor_set_layout.size() );
    std::transform(
      context.descriptor_set_layout.begin(),
      context.descriptor_set_layout.end(),
      std::back_inserter( raw_descriptor_set_layout ),
      []( const auto &v ) { return *v; }
    );
    context.set_descriptor_set( context.device->allocateDescriptorSetsUnique(
      vk::DescriptorSetAllocateInfo()
        .setDescriptorPool( *context.descriptor_pool )
        .setDescriptorSetCount( raw_descriptor_set_layout.size() )
        .setPSetLayouts( raw_descriptor_set_layout.data() )
    ) );
    VmaAllocatorCreateInfo allocator_info = {};
    allocator_info.physicalDevice = context.physical_device;
    allocator_info.device = *context.device;
    VmaAllocator allocator;
    {
      const auto result = vmaCreateAllocator( &allocator_info, &allocator );
      if( result != VK_SUCCESS ) vk::throwResultException( vk::Result( result ), "アロケータを作成できない" );
    }
    context.emplace_allocator(
      new VmaAllocator( allocator ),
      []( VmaAllocator *p ) {
        if( p ) {
          vmaDestroyAllocator( *p );
          delete p;
        }
      }
    );
    context.set_pipeline_cache( context.device->createPipelineCacheUnique( vk::PipelineCacheCreateInfo() ) );
    context.set_width( width );
    context.set_height( height );
    return context;
  }
}
