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
#include <fstream>
#include <iterator>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <OpenImageIO/imageio.h>
#include <vw/image.h>
#include <vw/buffer.h>
#include <vw/command_buffer.h>
#include <vw/exceptions.h>
namespace vw {
  image_t get_image(
    const context_t &context,
    const vk::ImageCreateInfo &image_create_info,
    VmaMemoryUsage usage
  ) {
    image_t image;
    const std::shared_ptr< VmaAllocation > allocation( new VmaAllocation() );
    VmaAllocationCreateInfo image_alloc_info = {};
    VkImageCreateInfo raw_image_create_info = image_create_info;
    image_alloc_info.usage = usage;
    VkImage image_;
    const auto result = vmaCreateImage( *context.allocator, &raw_image_create_info, &image_alloc_info, &image_, allocation.get(), nullptr );
    if( result != VK_SUCCESS ) vk::throwResultException( vk::Result( result ), "イメージを作成できない" );
    image.set_allocation( allocation );
    image.emplace_image(
      new vk::Image( image_ ),
      [allocator=context.allocator,allocation]( vk::Image *p ) {
        if( p ) {
          vmaDestroyImage( *allocator, *p, *allocation );
          delete p;
        }
      }
    );
    image.set_width( image_create_info.extent.width );
    image.set_height( image_create_info.extent.height );
    return image;
  }
  uint32_t get_pot( uint32_t v ) {
    for( uint32_t i = 0u; i != 32u; ++i )
      if( ( 1u << i ) >= v ) return i;
    return 32u;
  }
  bool is_pot( uint32_t v ) {
    return 1u << get_pot( v ) == v;
  }
  void convert_image(
    const vk::CommandBuffer &commands,
    const image_t &image,
    uint32_t mip_base,
    uint32_t mip_count,
    vk::ImageLayout from,
    vk::ImageLayout to
  ) {
    commands.pipelineBarrier(
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eFragmentShader,
      vk::DependencyFlagBits( 0 ),
      {},
      {},
      {
        vk::ImageMemoryBarrier()
          .setOldLayout( from )
          .setNewLayout( to )
          .setSrcAccessMask( vk::AccessFlagBits::eTransferRead )
          .setDstAccessMask( vk::AccessFlagBits::eShaderRead )
          .setImage( *image.image )
          .setSubresourceRange(
            vk::ImageSubresourceRange()
              .setAspectMask( vk::ImageAspectFlagBits::eColor )
              .setBaseMipLevel( mip_base )
              .setLevelCount( mip_count )
              .setBaseArrayLayer( 0 )
              .setLayerCount( 1 )
          )
      }
    );
  }
  image_t load_image(
    const context_t &context,
    const std::string &filename,
    vk::ImageUsageFlagBits usage,
    bool mipmap,
    bool srgb
  ) {
    using namespace OIIO_NAMESPACE;
    std::shared_ptr< ImageInput > texture_file(
      ImageInput::open( filename ),
      []( auto p ) { if( p ) ImageInput::destroy( p ); }
    );
    if( !texture_file ) throw unable_to_load_texture();
    const ImageSpec &spec = texture_file->spec();
    uint32_t mipmap_count = 1u;
    if( mipmap && spec.width == spec.height && is_pot( spec.width ) ) {
      mipmap_count = get_pot( spec.width );
    }
    auto final_image = get_image(
      context,
      vk::ImageCreateInfo()
        .setImageType( vk::ImageType::e2D )
        .setFormat( srgb ? vk::Format::eR8G8B8A8Srgb : vk::Format::eR8G8B8A8Unorm )
        .setExtent( { (uint32_t)spec.width, (uint32_t)spec.height, 1 } )
        .setMipLevels( mipmap_count )
        .setArrayLayers( 1 )
        .setSamples( vk::SampleCountFlagBits::e1 )
        .setTiling( vk::ImageTiling::eOptimal )
        .setUsage( usage | vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled )
        .setSharingMode( vk::SharingMode::eExclusive )
        .setQueueFamilyIndexCount( 0 )
        .setPQueueFamilyIndices( nullptr )
        .setInitialLayout( vk::ImageLayout::eUndefined ),
        VMA_MEMORY_USAGE_GPU_ONLY
    );
    auto temporary = get_buffer(
      context,
      vk::BufferCreateInfo()
        .setSize( spec.width * spec.height * 4 )
        .setUsage( vk::BufferUsageFlagBits::eTransferSrc )
        .setSharingMode( vk::SharingMode::eExclusive )
        .setQueueFamilyIndexCount( 0 )
        .setPQueueFamilyIndices( nullptr ),
        VMA_MEMORY_USAGE_CPU_TO_GPU
    );
    {
      void* mapped_memory;
      const auto result = vmaMapMemory( *context.allocator, *temporary.allocation, &mapped_memory );
      if( result != VK_SUCCESS ) vk::throwResultException( vk::Result( result ), "イメージをマップできない" );
      std::shared_ptr< uint8_t > mapped(
        reinterpret_cast< uint8_t* >( mapped_memory ),
        [allocator=context.allocator,allocation=temporary.allocation]( uint8_t *p ) {
          if( p ) vmaUnmapMemory( *allocator, *allocation );
        }
      );
      texture_file->read_image( TypeDesc::UINT8, mapped.get() );
      if( spec.nchannels == 3 ) {
        for( size_t i = spec.width * spec.height - 1; i; --i ) {
          mapped.get()[ i * 4 ] = mapped.get()[ i * spec.nchannels ];
          mapped.get()[ i * 4 + 1 ] = mapped.get()[ i * spec.nchannels + 1 ];
          mapped.get()[ i * 4 + 2 ] = mapped.get()[ i * spec.nchannels + 2 ];
          mapped.get()[ i * 4 + 3 ] = 255u;
        }
      }
    }
    auto commands = get_command_buffer( context, true );
    commands->begin(
      vk::CommandBufferBeginInfo()
        .setFlags( vk::CommandBufferUsageFlagBits::eOneTimeSubmit )
    );
    convert_image( *commands, final_image, 0, mipmap_count, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal );
    commands->copyBufferToImage(
      *temporary.buffer,
      *final_image.image,
      vk::ImageLayout::eTransferDstOptimal,
      {
        vk::BufferImageCopy()
          .setBufferOffset( vk::DeviceSize( 0 ) )
          .setImageSubresource(
            vk::ImageSubresourceLayers()
              .setAspectMask( vk::ImageAspectFlagBits::eColor )
              .setMipLevel( 0 )
              .setLayerCount( 1 )
          )
          .setImageExtent(
            vk::Extent3D()
              .setWidth( spec.width )
              .setHeight( spec.height )
              .setDepth( spec.depth )
          )
      }
    );
    uint32_t mip_width = spec.width;
    uint32_t mip_height = spec.height;
    convert_image( *commands, final_image, 0, 1, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eTransferSrcOptimal );
    for( uint32_t i = 1u; i < mipmap_count; ++i ) {
      commands->blitImage(
        *final_image.image, vk::ImageLayout::eTransferSrcOptimal,
        *final_image.image, vk::ImageLayout::eTransferDstOptimal,
        {
          vk::ImageBlit()
            .setSrcSubresource(
              vk::ImageSubresourceLayers()
                .setAspectMask( vk::ImageAspectFlagBits::eColor )
                .setBaseArrayLayer( 0 )
                .setLayerCount( 1 )
                .setMipLevel( i - 1 )
            )
            .setSrcOffsets( {
              vk::Offset3D( 0, 0, 0 ),
              vk::Offset3D( mip_width, mip_height, 1 ),
            } )
            .setDstSubresource(
              vk::ImageSubresourceLayers()
                .setAspectMask( vk::ImageAspectFlagBits::eColor )
                .setBaseArrayLayer( 0 )
                .setLayerCount( 1 )
                .setMipLevel( i )
            )
            .setDstOffsets( {
              vk::Offset3D( 0, 0, 0 ),
              vk::Offset3D( mip_width / 2, mip_height / 2, 1 ),
            } )
        },
        vk::Filter::eLinear
      );
      convert_image( *commands, final_image, i, 1, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eTransferSrcOptimal );
      mip_width /= 2;
      mip_height /= 2;
    }
    convert_image( *commands, final_image, 0, mipmap_count, vk::ImageLayout::eTransferSrcOptimal, vk::ImageLayout::eShaderReadOnlyOptimal );
    commands->end();
    auto graphics_queue = context.device->getQueue( context.graphics_queue_index, 0 );
    graphics_queue.submit(
    vk::SubmitInfo()
      .setCommandBufferCount( 1 )
      .setPCommandBuffers( &*commands ),
      vk::Fence()
    );
    graphics_queue.waitIdle();
    final_image.set_image_view(
      context.device->createImageViewUnique(
        vk::ImageViewCreateInfo()
          .setImage( *final_image.image )
          .setViewType( vk::ImageViewType::e2D )
          .setFormat( srgb ? vk::Format::eR8G8B8A8Srgb : vk::Format::eR8G8B8A8Unorm )
          .setSubresourceRange(
            vk::ImageSubresourceRange()
              .setAspectMask( vk::ImageAspectFlagBits::eColor )
              .setBaseMipLevel( 0 )
              .setLevelCount( mipmap_count )
              .setBaseArrayLayer( 0 )
              .setLayerCount( 1 )
          )
      )
    );
    return final_image;
  }
  void dump_image(
    const context_t &context,
    const image_t &image,
    const std::string &filename,
    unsigned int mipmap
  ) {
    auto width = image.width >> mipmap;
    auto height = image.height >> mipmap;
    auto temporary = get_buffer(
      context,
      vk::BufferCreateInfo()
        .setSize( width * height * 4 )
        .setUsage( vk::BufferUsageFlagBits::eTransferDst )
        .setSharingMode( vk::SharingMode::eExclusive )
        .setQueueFamilyIndexCount( 0 )
        .setPQueueFamilyIndices( nullptr ),
        VMA_MEMORY_USAGE_GPU_TO_CPU
    );
    auto commands = get_command_buffer( context, true );
    commands->begin(
      vk::CommandBufferBeginInfo()
        .setFlags( vk::CommandBufferUsageFlagBits::eOneTimeSubmit )
    );
    convert_image( *commands, image, mipmap, 1, vk::ImageLayout::eShaderReadOnlyOptimal, vk::ImageLayout::eTransferSrcOptimal );
    commands->copyImageToBuffer(
      *image.image,
      vk::ImageLayout::eTransferSrcOptimal,
      *temporary.buffer,
      {
        vk::BufferImageCopy()
          .setBufferOffset( vk::DeviceSize( 0 ) )
          .setImageSubresource(
            vk::ImageSubresourceLayers()
              .setAspectMask( vk::ImageAspectFlagBits::eColor )
              .setMipLevel( mipmap )
              .setLayerCount( 1 )
          )
          .setImageExtent(
            vk::Extent3D()
              .setWidth( width )
              .setHeight( height )
              .setDepth( 1 )
          )
      }
    );
    convert_image( *commands, image, mipmap, 1, vk::ImageLayout::eTransferSrcOptimal, vk::ImageLayout::eShaderReadOnlyOptimal );
    commands->end();
    auto graphics_queue = context.device->getQueue( context.graphics_queue_index, 0 );
    graphics_queue.submit(
    vk::SubmitInfo()
      .setCommandBufferCount( 1 )
      .setPCommandBuffers( &*commands ),
      vk::Fence()
    );
    graphics_queue.waitIdle();
    using namespace OIIO_NAMESPACE;
    auto out = ImageOutput::create( filename );
    if( !out ) throw -1;
    ImageSpec spec( width, height, 4, TypeDesc::UINT8 );
    out->open( filename, spec );
    {
      void* mapped_memory;
      const auto result = vmaMapMemory( *context.allocator, *temporary.allocation, &mapped_memory );
      if( result != VK_SUCCESS ) vk::throwResultException( vk::Result( result ), "イメージをマップできない" );
      std::shared_ptr< uint8_t > mapped(
        reinterpret_cast< uint8_t* >( mapped_memory ),
        [allocator=context.allocator,allocation=temporary.allocation]( uint8_t *p ) {
          if( p ) vmaUnmapMemory( *allocator, *allocation );
        }
      );
      out->write_image( TypeDesc::UINT8, mapped.get() );
    }
    out->close();
  }
}
