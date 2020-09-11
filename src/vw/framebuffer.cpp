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
#include <vw/framebuffer.h>
namespace vw {
  std::vector< framebuffer_t > create_framebuffer(
    const context_t &context,
    const render_pass_t &render_pass
  ) {
    std::vector< framebuffer_t > framebuffers;
    for( auto &swapchain_image: context.device->getSwapchainImagesKHR( *context.swapchain ) ) {
      framebuffers.push_back( framebuffer_t() );
      framebuffers.back().set_swapchain_image_view(
        context.device->createImageViewUnique(
          vk::ImageViewCreateInfo()
            .setImage( swapchain_image )
            .setViewType( vk::ImageViewType::e2D )
            .setFormat( context.surface_format.format )
            .setSubresourceRange( vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 ) )
        )
      );
      framebuffers.back().set_depth_image( get_image(
        context,
        vk::ImageCreateInfo()
          .setImageType( vk::ImageType::e2D )
          .setFormat( vk::Format::eD16Unorm )
          .setExtent( { context.width, context.height, 1 } )
          .setMipLevels( 1 )
          .setArrayLayers( 1 )
          .setUsage( vk::ImageUsageFlagBits::eDepthStencilAttachment ),
        VMA_MEMORY_USAGE_GPU_ONLY
      ) );
      framebuffers.back().set_depth_image_view(
        context.device->createImageViewUnique(
         vk::ImageViewCreateInfo()
          .setImage( *framebuffers.back().depth_image.image )
          .setViewType( vk::ImageViewType::e2D )
          .setFormat( vk::Format::eD16Unorm )
          .setSubresourceRange( vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1 ) )
        )
      );
      const std::array< vk::ImageView, 2 > attachments{
        *framebuffers.back().swapchain_image_view,
        *framebuffers.back().depth_image_view
      };
      framebuffers.back().set_framebuffer(
        context.device->createFramebufferUnique(
          vk::FramebufferCreateInfo()
            .setRenderPass( *render_pass.render_pass )
            .setAttachmentCount( attachments.size() )
            .setPAttachments( attachments.data() )
            .setWidth( context.width )
            .setHeight( context.height )
            .setLayers( 1 )
        )
      );
    }
    return framebuffers;
  }
  std::vector< framebuffer_fence_t > create_framebuffer_fences(
    const context_t &context,
    uint32_t image_count
  ) {
    std::vector< framebuffer_fence_t > fence;
    for( uint32_t i = 0; i != image_count; ++i ) {
      fence.push_back( framebuffer_fence_t() );  
      fence.back().set_fence(
        context.device->createFenceUnique(
          vk::FenceCreateInfo()
            .setFlags( vk::FenceCreateFlagBits::eSignaled )
        )
      );
      fence.back().set_image_acquired_semaphore(
        context.device->createSemaphoreUnique( vk::SemaphoreCreateInfo() )
      );
      fence.back().set_draw_complete_semaphore(
        context.device->createSemaphoreUnique( vk::SemaphoreCreateInfo() )
      );
      fence.back().set_image_ownership_semaphore(
        context.device->createSemaphoreUnique( vk::SemaphoreCreateInfo() )
      );
    }
    return fence;
  }
}
