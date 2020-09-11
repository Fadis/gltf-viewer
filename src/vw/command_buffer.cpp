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
#include <vw/command_buffer.h>
namespace vw {
  vk::UniqueHandle< vk::CommandBuffer, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE >
  get_command_buffer( const context_t &context, bool graphics ) {
    std::vector< vk::UniqueHandle< vk::CommandBuffer, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > > cbs;
    if( graphics )
      cbs =
        context.device->allocateCommandBuffersUnique(
          vk::CommandBufferAllocateInfo()
            .setCommandPool( *context.graphics_command_pool )
            .setLevel( vk::CommandBufferLevel::ePrimary )
            .setCommandBufferCount( 1 )
        );
    else
      cbs =
        context.device->allocateCommandBuffersUnique(
          vk::CommandBufferAllocateInfo()
            .setCommandPool( *context.present_command_pool )
            .setLevel( vk::CommandBufferLevel::ePrimary )
            .setCommandBufferCount( 1 )
        );
    return std::move( cbs.front() );
  }
  std::vector< vk::UniqueHandle< vk::CommandBuffer, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > >
  get_command_buffer( const context_t &context, bool graphics, uint32_t count ) {
    std::vector< vk::UniqueHandle< vk::CommandBuffer, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > > cbs;
    if( graphics )
      cbs =
        context.device->allocateCommandBuffersUnique(
          vk::CommandBufferAllocateInfo()
            .setCommandPool( *context.graphics_command_pool )
            .setLevel( vk::CommandBufferLevel::ePrimary )
            .setCommandBufferCount( count )
        );
    else
      cbs =
        context.device->allocateCommandBuffersUnique(
          vk::CommandBufferAllocateInfo()
            .setCommandPool( *context.present_command_pool )
            .setLevel( vk::CommandBufferLevel::ePrimary )
            .setCommandBufferCount( count )
        );
    return cbs;
  }
}

