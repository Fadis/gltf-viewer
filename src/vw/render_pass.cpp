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
#include <iostream>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <vw/render_pass.h>
namespace vw {
  render_pass_t create_render_pass(
    const context_t &context
  ) {
    render_pass_t render_pass;
    const std::vector< vk::AttachmentDescription > attachments{
      vk::AttachmentDescription()
        .setFormat( context.surface_format.format )
        .setSamples( vk::SampleCountFlagBits::e1 )
        .setLoadOp( vk::AttachmentLoadOp::eClear )
        .setStoreOp( vk::AttachmentStoreOp::eStore )
        .setStencilLoadOp( vk::AttachmentLoadOp::eDontCare )
        .setStencilStoreOp( vk::AttachmentStoreOp::eDontCare )
        .setInitialLayout( vk::ImageLayout::eUndefined )
        .setFinalLayout( vk::ImageLayout::ePresentSrcKHR ),
      vk::AttachmentDescription()
        .setFormat( vk::Format::eD16Unorm )
        .setSamples( vk::SampleCountFlagBits::e1 )
        .setLoadOp( vk::AttachmentLoadOp::eClear )
        .setStoreOp( vk::AttachmentStoreOp::eDontCare )
        .setStencilLoadOp( vk::AttachmentLoadOp::eDontCare )
        .setStencilStoreOp( vk::AttachmentStoreOp::eDontCare )
        .setInitialLayout( vk::ImageLayout::eUndefined )
        .setFinalLayout( vk::ImageLayout::eDepthStencilAttachmentOptimal )
    };
    render_pass.set_attachments( attachments );
    const std::vector< vk::AttachmentReference > color_reference{
      vk::AttachmentReference().setAttachment( 0 ).setLayout( vk::ImageLayout::eColorAttachmentOptimal )
    };
    const auto depth_reference =
      vk::AttachmentReference().setAttachment( 1 ).setLayout( vk::ImageLayout::eDepthStencilAttachmentOptimal );
    const std::vector< vk::SubpassDescription > subpass{
      vk::SubpassDescription()
        .setPipelineBindPoint( vk::PipelineBindPoint::eGraphics )
        .setColorAttachmentCount( color_reference.size() )
        .setPColorAttachments( color_reference.data() )
        .setPDepthStencilAttachment( &depth_reference )
    };
    render_pass.set_render_pass( context.device->createRenderPassUnique(
      vk::RenderPassCreateInfo()
        .setAttachmentCount( attachments.size() )
        .setPAttachments( attachments.data() )
        .setSubpassCount( subpass.size() )
        .setPSubpasses( subpass.data() )
    ) );
    return render_pass;
  }
}

