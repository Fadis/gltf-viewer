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
#include <vulkan/vulkan.hpp>
#include <vw/config.h>
#include <vw/instance.h>
#include <vw/glfw.h>
namespace vw {
  vk::UniqueHandle< vk::Instance, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE >
  create_instance(
    const vw::configs_t &configs,
    const std::vector< const char* > &ext_,
    const std::vector< const char* > &layers_
  ) {
    auto ext = ext_;
    if( !configs.direct ) {
      glfw::init();
      uint32_t count = 0u;
      auto exts = glfwGetRequiredInstanceExtensions( &count );
      for( uint32_t i = 0u; i != count; ++i )
        ext.push_back( exts[ i ] );
    }
    else {
      ext.push_back( VK_KHR_SURFACE_EXTENSION_NAME );
      ext.push_back( VK_KHR_DISPLAY_EXTENSION_NAME );
      ext.push_back( VK_EXT_DISPLAY_SURFACE_COUNTER_EXTENSION_NAME );
      ext.push_back( VK_EXT_DIRECT_MODE_DISPLAY_EXTENSION_NAME );
    }
    auto layers = layers_;
    if( configs.validation ) layers.emplace_back( "VK_LAYER_KHRONOS_validation" );
    const auto app_info = vk::ApplicationInfo(
      configs.prog_name.c_str(), VK_MAKE_VERSION(1, 0, 0),
      "vw", VK_MAKE_VERSION(1, 0, 0),
      VK_API_VERSION_1_1
    );
    return vk::createInstanceUnique(
      vk::InstanceCreateInfo()
        .setPApplicationInfo( &app_info )
        .setEnabledExtensionCount( ext.size() )
        .setPpEnabledExtensionNames( ext.data() )
        .setEnabledLayerCount( layers.size() )
        .setPpEnabledLayerNames( layers.data() )
    );
  }
}

