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
#include <iterator>
#include <fstream>
#include <vw/buffer.h>
#include <vw/command_buffer.h>
namespace vw {
  buffer_t get_buffer(
    const context_t &context,
    const vk::BufferCreateInfo &buffer_create_info,
    VmaMemoryUsage usage
  ) {
    buffer_t buffer;
    const std::shared_ptr< VmaAllocation > allocation( new VmaAllocation() );
    VmaAllocationCreateInfo buffer_alloc_info = {};
    VkBufferCreateInfo raw_buffer_create_info = buffer_create_info;
    buffer_alloc_info.usage = usage;
    VkBuffer buffer_;
    const auto result = vmaCreateBuffer( *context.allocator, &raw_buffer_create_info, &buffer_alloc_info, &buffer_, allocation.get(), nullptr );
    if( result != VK_SUCCESS ) vk::throwResultException( vk::Result( result ), "バッファを作成できない" );
    buffer.set_allocation( allocation );
    buffer.emplace_buffer(
      new vk::Buffer( buffer_ ),
      [allocator=context.allocator,allocation]( vk::Buffer *p ) {
        if( p ) {
          vmaDestroyBuffer( *allocator, *p, *allocation );
          delete p;
        }
      }
    );
    return buffer;
  }
  buffer_t load_buffer(
    const context_t &context,
    const std::vector< uint8_t > &data,
    vk::BufferUsageFlags usage
  ) {
    auto final_buffer = get_buffer(
      context,
      vk::BufferCreateInfo()
        .setSize( data.size() )
        .setUsage( usage | vk::BufferUsageFlagBits::eTransferDst ),
      VMA_MEMORY_USAGE_GPU_ONLY
    );
    auto temporary = get_buffer(
      context,
      vk::BufferCreateInfo()
        .setSize( data.size() )
        .setUsage( vk::BufferUsageFlagBits::eTransferSrc ),
      VMA_MEMORY_USAGE_CPU_TO_GPU
    );
    {
      void* mapped_memory;
      const auto result = vmaMapMemory( *context.allocator, *temporary.allocation, &mapped_memory );
      if( result != VK_SUCCESS ) vk::throwResultException( vk::Result( result ), "バッファをマップできない" );
      std::shared_ptr< uint8_t > mapped(
        reinterpret_cast< uint8_t* >( mapped_memory ),
        [allocator=context.allocator,allocation=temporary.allocation]( uint8_t *p ) {
          if( p ) vmaUnmapMemory( *allocator, *allocation );
        }
      );
      std::copy( data.begin(), data.end(), mapped.get() );
    }
    auto commands = get_command_buffer( context, true );
    commands->begin(
      vk::CommandBufferBeginInfo()
        .setFlags( vk::CommandBufferUsageFlagBits::eOneTimeSubmit )
    );
    commands->copyBuffer(
      *temporary.buffer,
      *final_buffer.buffer,
      {
        vk::BufferCopy()
          .setSrcOffset( 0 )
          .setDstOffset( 0 )
          .setSize( data.size() )
      }
    );
    commands->end();
    auto graphics_queue = context.device->getQueue( context.graphics_queue_index, 0 );
    graphics_queue.submit(
    vk::SubmitInfo()
      .setCommandBufferCount( 1 )
      .setPCommandBuffers( &*commands ),
      vk::Fence()
    );
    graphics_queue.waitIdle();
    return final_buffer;
  }
  buffer_t load_buffer_from_file(
    const context_t &context,
    const std::string &filename,
    vk::BufferUsageFlags usage
  ) {
    std::ifstream file( filename, std::ios::in | std::ios::binary );
    std::vector< uint8_t > data(
      ( std::istreambuf_iterator< char >( file ) ),
      std::istreambuf_iterator< char >()
    );
    return load_buffer(
      context,
      data,
      usage
    );
  }
}
