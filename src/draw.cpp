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
#include <iostream>
#include <vector>
#include <filesystem>
#include <boost/scope_exit.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wreorder"
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#include <vk_mem_alloc.h>
#pragma GCC diagnostic pop
#include <fx/gltf.h>
#include <vw/config.h>
#include <vw/list_device.h>
#include <vw/is_capable.h>
#include <vw/instance.h>
#include <vw/context.h>
#include <vw/render_pass.h>
#include <vw/shader.h>
#include <vw/pipeline.h>
#include <vw/framebuffer.h>
#include <vw/image.h>
#include <vw/buffer.h>
#include <vw/wait_for_idle.h>
#include <vw/command_buffer.h>
#include <viewer/document.h>



int main( int argc, const char *argv[] ) {
  const auto config = vw::parse_configs( argc, argv );
  auto instance = vw::create_instance(
    config,
    {},
    {}
  );
  if( config.list ) {
    vw::list_devices( *instance, config );
    return 0;
  }
  if( !vw::is_capable(
    *instance,
    config,
    {
      VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
      VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME
    }, {}
  ) ) {
    std::cout << "指定された条件に合うデバイスは無かった" << std::endl;
    return 0;
  }
  {
    auto context = vw::create_context(
      *instance,
      config,
      {
        VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME
      },
      {},
      {
        vk::DescriptorPoolSize().setType( vk::DescriptorType::eUniformBuffer ).setDescriptorCount( 400 ),
        vk::DescriptorPoolSize().setType( vk::DescriptorType::eCombinedImageSampler ).setDescriptorCount( 400 )
      },
      {
        vk::DescriptorSetLayoutBinding()
          .setDescriptorType( vk::DescriptorType::eUniformBuffer )
          .setDescriptorCount( 1 )
          .setBinding( 0 )
          .setStageFlags( vk::ShaderStageFlagBits::eFragment )
          .setPImmutableSamplers( nullptr ),
        vk::DescriptorSetLayoutBinding() // base color
          .setDescriptorType( vk::DescriptorType::eCombinedImageSampler )
          .setDescriptorCount( 1 )
          .setBinding( 1 )
          .setStageFlags( vk::ShaderStageFlagBits::eFragment )
          .setPImmutableSamplers( nullptr ),
        vk::DescriptorSetLayoutBinding() // roughness metallness
          .setDescriptorType( vk::DescriptorType::eCombinedImageSampler )
          .setDescriptorCount( 1 )
          .setBinding( 2 )
          .setStageFlags( vk::ShaderStageFlagBits::eFragment )
          .setPImmutableSamplers( nullptr ),
        vk::DescriptorSetLayoutBinding() // normal
          .setDescriptorType( vk::DescriptorType::eCombinedImageSampler )
          .setDescriptorCount( 1 )
          .setBinding( 3 )
          .setStageFlags( vk::ShaderStageFlagBits::eFragment )
          .setPImmutableSamplers( nullptr ),
        vk::DescriptorSetLayoutBinding() // occlusion
          .setDescriptorType( vk::DescriptorType::eCombinedImageSampler )
          .setDescriptorCount( 1 )
          .setBinding( 4 )
          .setStageFlags( vk::ShaderStageFlagBits::eFragment )
          .setPImmutableSamplers( nullptr ),
        vk::DescriptorSetLayoutBinding() // emissive
          .setDescriptorType( vk::DescriptorType::eCombinedImageSampler )
          .setDescriptorCount( 1 )
          .setBinding( 5 )
          .setStageFlags( vk::ShaderStageFlagBits::eFragment )
          .setPImmutableSamplers( nullptr )
      }
    );
    auto render_pass = vw::create_render_pass( context );
    auto framebuffer = vw::create_framebuffer(
      context, render_pass
    );
    auto fence = vw::create_framebuffer_fences(
      context, framebuffer.size()
    );
    viewer::document_t document = viewer::load_gltf(
      context,
      render_pass,
      std::filesystem::path( config.input ),
      framebuffer.size(),
      config.shader,
      config.shader_mask
    );
    auto center = ( document.node.min + document.node.max ) / 2.f;
    auto scale = std::abs( glm::length( document.node.max - document.node.min ) );
    uint32_t current_frame = 0u;
    auto command_buffer = vw::get_command_buffer( context, true, framebuffer.size() );
    auto graphics_queue = context.device->getQueue( context.graphics_queue_index, 0 );
    auto present_queue = context.device->getQueue( context.present_queue_index, 0 );
    const std::array< vk::ClearValue, 2 > clear_values{
      vk::ClearColorValue( std::array< float, 4u >{ config.purple ? 1.0f : 0.0f, 0.0f, config.purple ? 1.0f : 0.0f, 1.0f } ),
      vk::ClearDepthStencilValue( 1.f, 0 )
    };
    auto const viewport =
      vk::Viewport()
        .setWidth( context.width )
        .setHeight( context.height )
        .setMinDepth( 0.0f )
        .setMaxDepth( 1.0f );
    vk::Rect2D const scissor( vk::Offset2D(0, 0), vk::Extent2D( context.width, context.height ) );
    std::cout << scale << std::endl;
    const glm::mat4 projection = glm::perspective( 30.f, (float(context.width)/float(context.height)), std::min(0.1f*scale,0.5f), 150.f*scale );
    auto camera_pos = center + glm::vec3{ 0.f, 0.f, 1.0f*scale };
    float camera_angle = 0;//M_PI;
    auto speed = 0.01f*scale;
    auto light_pos = glm::vec3{ 4.2f*scale, 5.f*scale, 3.f*scale };
    while( !context.input_state->quit ) {
      if( context.input_state->a ) camera_angle += 0.01 * M_PI/2;
      if( context.input_state->d ) camera_angle -= 0.01 * M_PI/2;
      glm::vec3 camera_direction( std::sin( camera_angle ), 0, -std::cos( camera_angle ) );
      if( context.input_state->w ) camera_pos += camera_direction * glm::vec3( speed );
      if( context.input_state->s ) camera_pos -= camera_direction * glm::vec3( speed );
      if( context.input_state->e ) camera_pos[ 1 ] += speed;
      if( context.input_state->c ) camera_pos[ 1 ] -= speed;
      if( context.input_state->up ) light_pos[ 2 ] += speed;
      if( context.input_state->down ) light_pos[ 2 ] -= speed;
      if( context.input_state->left ) light_pos[ 0 ] -= speed;
      if( context.input_state->right ) light_pos[ 0 ] += speed;
      glm::mat4 lookat = glm::lookAt(
        camera_pos,
        camera_pos + camera_direction,
        glm::vec3{ 0.f, camera_pos[ 1 ] + 100.f*scale, 0.f }
      );
      const auto begin_time = std::chrono::high_resolution_clock::now();
      auto &fe = fence[ current_frame ];
      context.device->waitForFences( 1, &*fe.fence, VK_TRUE, UINT64_MAX );
      context.device->resetFences( 1, &*fe.fence );
      auto &gcb = command_buffer[ current_frame ];
      gcb->reset( vk::CommandBufferResetFlags( 0 ) );
      auto image_index = context.device->acquireNextImageKHR( *context.swapchain, UINT64_MAX, *fe.image_acquired_semaphore, vk::Fence() );
      auto &fb = framebuffer[ image_index.value ];
      gcb->reset( vk::CommandBufferResetFlags( 0 ) );
      gcb->begin(
        vk::CommandBufferBeginInfo()
          .setFlags( vk::CommandBufferUsageFlagBits::eOneTimeSubmit )
      );
      auto const pass_info = vk::RenderPassBeginInfo()
        .setRenderPass( *render_pass.render_pass )
        .setFramebuffer( *fb.framebuffer )
        .setRenderArea( vk::Rect2D( vk::Offset2D(0, 0), vk::Extent2D((uint32_t)context.width, (uint32_t)context.height) ) )
        .setClearValueCount( clear_values.size() )
        .setPClearValues( clear_values.data() );
      gcb->beginRenderPass( &pass_info, vk::SubpassContents::eInline );
      gcb->setViewport( 0, 1, &viewport );
      gcb->setScissor( 0, 1, &scissor );
      viewer::draw_node(
        context,
        *gcb,
        document.node,
        document.mesh,
        document.buffer,
        current_frame,
        projection,
        lookat,
        camera_pos,
        light_pos
      );
      gcb->endRenderPass();
      gcb->end();
      vk::PipelineStageFlags pipe_stage_flags = vk::PipelineStageFlagBits::eColorAttachmentOutput;
      graphics_queue.submit(
      vk::SubmitInfo()
        .setPWaitDstStageMask( &pipe_stage_flags )
        .setCommandBufferCount( 1 )
        .setPCommandBuffers( &*gcb )
        .setWaitSemaphoreCount( 1 )
        .setPWaitSemaphores( &*fe.image_acquired_semaphore )
        .setSignalSemaphoreCount( 1 )
        .setPSignalSemaphores( &*fe.draw_complete_semaphore ),
        *fe.fence
      );
      auto const present_info = vk::PresentInfoKHR()
        .setWaitSemaphoreCount( 1 )
        .setPWaitSemaphores( &*fe.draw_complete_semaphore )
        .setSwapchainCount( 1 )
        .setPSwapchains( &*context.swapchain )
        .setPImageIndices( &image_index.value );
      present_queue.presentKHR( &present_info );
      glfwPollEvents();
      ++current_frame;
      current_frame %= framebuffer.size();
      vw::wait_for_sync( begin_time );
    }
    vw::wait_for_idle( context );
  }
}


