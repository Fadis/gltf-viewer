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
#include <vw/list_device.h>
#include <vw/glfw.h>
#include <vw/config.h>
#include <vw/is_capable.h>
namespace vw {
  bool is_capable(
    const vk::Instance &instance,
    const vw::configs_t &configs,
    const std::vector< const char* > &dext,
    const std::vector< const char* > &dlayers
  ) {
    if( !configs.direct ) glfw::init();
    const auto devices = instance.enumeratePhysicalDevices();
    if( devices.size() <= configs.device_index ) {
      std::cout << "指定されたデバイスは存在しない" << std::endl;
      return false;
    }
    const auto &device = devices[ configs.device_index ];
    const auto avail_dext = device.enumerateDeviceExtensionProperties();
    for( const char *w: dext ) {
      if( std::find_if( avail_dext.begin(), avail_dext.end(), [&]( const auto &v ) { return !strcmp( v.extensionName,
w ); } ) == avail_dext.end() ) {
        std::cout << "指定されたデバイスは必要な拡張 " << w << " をサポートしていない" << std::endl;
        return false;
      }
    }
    const auto avail_dlayers = device.enumerateDeviceLayerProperties();
    for( const char *w: dlayers ) {
      if( std::find_if( avail_dlayers.begin(), avail_dlayers.end(), [&]( const auto &v ) { return !strcmp( v.layerName, w ); } ) == avail_dlayers.end() ) {
        std::cout << "指定されたデバイスは必要なレイヤー " << w << " をサポートしていない" << std::endl;
        return false;
      }
    }
    if( configs.direct ) {
      const auto displays = device.getDisplayPropertiesKHR();
      if( displays.size() <= configs.display_index ) {
        std::cout << "指定されたディスプレイは存在しない" << std::endl;
        return false;
      }
      const auto &display = displays[ configs.display_index ];
      auto modes = device.getDisplayModePropertiesKHR( display.display );
      uint32_t width = configs.width;
      uint32_t height = configs.height;
      if( width == 0 && height == 0 ) {
        width = display.physicalResolution.width;
        height = display.physicalResolution.height;
      }
      std::vector< vk::DisplayModePropertiesKHR > available_modes;
      std::copy_if( modes.begin(), modes.end(), std::back_inserter( available_modes ), [&]( const auto &m ) { return m.parameters.visibleRegion.width == width && m.parameters.visibleRegion.height == height; } );
      if( available_modes.empty() ) {
        std::cout << "ディスプレイは指定された解像度に対応していない" << std::endl;
        return false;
      }
    }
    return true;
  }
}

