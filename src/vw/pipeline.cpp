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
#include <vector>
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>
#include <vw/pipeline.h>
namespace vw {
  pipeline_t create_pipeline(
    const context_t &context,
    const render_pass_t &render_pass,
    uint32_t push_constant_size,
    const vk::ShaderModule &vs,
    const vk::ShaderModule &fs,
    const std::vector< vk::VertexInputBindingDescription > &vertex_input_binding,
    const std::vector< vk::VertexInputAttributeDescription > &vertex_input_attribute,
    bool cull,
    bool blend
  ) {
    pipeline_t pipeline;
    const std::vector< vk::PushConstantRange > push_constant_range{
      vk::PushConstantRange()
        .setStageFlags( vk::ShaderStageFlagBits::eVertex|vk::ShaderStageFlagBits::eFragment )
        .setOffset( 0 )
        .setSize( push_constant_size )
    };
    std::vector< vk::DescriptorSetLayout > raw_descriptor_set_layout;
    raw_descriptor_set_layout.reserve( context.descriptor_set_layout.size() );
    std::transform(
      context.descriptor_set_layout.begin(),
      context.descriptor_set_layout.end(),
      std::back_inserter( raw_descriptor_set_layout ),
      []( const auto &v ) { return *v; }
    );
    pipeline.set_pipeline_layout( context.device->createPipelineLayoutUnique(
      vk::PipelineLayoutCreateInfo()
        .setSetLayoutCount( context.descriptor_set_layout.size() )
        .setPSetLayouts( raw_descriptor_set_layout.data() )
        .setPushConstantRangeCount( push_constant_range.size() )
        .setPPushConstantRanges( push_constant_range.data() )
    ) );
    std::vector< vk::PipelineShaderStageCreateInfo > pipeline_shader_stages;
    pipeline_shader_stages.push_back(
      vk::PipelineShaderStageCreateInfo()
        .setStage( vk::ShaderStageFlagBits::eVertex )
        .setModule( vs )
        .setPName( "main" )
    );
    pipeline_shader_stages.push_back(
      vk::PipelineShaderStageCreateInfo()
        .setStage( vk::ShaderStageFlagBits::eFragment )
        .setModule( fs )
        .setPName( "main" )
    );
    const auto input_assembly_info = vk::PipelineInputAssemblyStateCreateInfo()
      .setTopology( vk::PrimitiveTopology::eTriangleList );
    const auto viewport_info = vk::PipelineViewportStateCreateInfo().setViewportCount( 1 ).setScissorCount( 1 );
    const auto rasterization_info = vk::PipelineRasterizationStateCreateInfo()
      .setDepthClampEnable( VK_FALSE )
      .setRasterizerDiscardEnable( VK_FALSE )
      .setPolygonMode( vk::PolygonMode::eFill )
      .setCullMode( cull ? vk::CullModeFlagBits::eBack : vk::CullModeFlagBits::eNone )
      .setFrontFace( vk::FrontFace::eCounterClockwise )
      .setDepthBiasEnable( VK_FALSE )
      .setLineWidth( 1.0f );
    const auto multisample_info = vk::PipelineMultisampleStateCreateInfo();
    const auto stencil_op = vk::StencilOpState()
      .setFailOp( vk::StencilOp::eKeep )
      .setPassOp( vk::StencilOp::eKeep )
      .setCompareOp( vk::CompareOp::eAlways );
    const auto depth_stencil_info = vk::PipelineDepthStencilStateCreateInfo()
      .setDepthTestEnable( VK_TRUE )
      .setDepthWriteEnable( VK_TRUE )
      .setDepthCompareOp( vk::CompareOp::eLessOrEqual )
      .setDepthBoundsTestEnable( VK_FALSE )
      .setStencilTestEnable( VK_FALSE )
      .setFront( stencil_op )
      .setBack( stencil_op );
    const std::array< vk::PipelineColorBlendAttachmentState, 1u > color_blend_attachments{
      vk::PipelineColorBlendAttachmentState()
        .setBlendEnable( blend )
        .setColorWriteMask(
          vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB |
          vk::ColorComponentFlagBits::eA
        )
    };
    const auto color_blend_info =
      vk::PipelineColorBlendStateCreateInfo()
        .setAttachmentCount( color_blend_attachments.size() )
        .setPAttachments( color_blend_attachments.data() );
    const std::array< vk::DynamicState, 2u > dynamic_states{
      vk::DynamicState::eViewport, vk::DynamicState::eScissor
    };
    const auto dynamic_state_info =
      vk::PipelineDynamicStateCreateInfo()
        .setDynamicStateCount( dynamic_states.size() )
       .setPDynamicStates( dynamic_states.data() );
    const vk::PipelineVertexInputStateCreateInfo vertex_input_state = vk::PipelineVertexInputStateCreateInfo()
      .setVertexAttributeDescriptionCount( vertex_input_attribute.size() )
      .setPVertexAttributeDescriptions( vertex_input_attribute.data() )
      .setVertexBindingDescriptionCount( vertex_input_binding.size() )
      .setPVertexBindingDescriptions( vertex_input_binding.data() );
    const std::vector< vk::GraphicsPipelineCreateInfo > pipeline_create_info{
      vk::GraphicsPipelineCreateInfo()
        .setStageCount( pipeline_shader_stages.size() )
        .setPStages( pipeline_shader_stages.data() )
        .setPVertexInputState( &vertex_input_state )
        .setPInputAssemblyState( &input_assembly_info )
        .setPViewportState( &viewport_info )
        .setPRasterizationState( &rasterization_info )
        .setPMultisampleState( &multisample_info )
        .setPDepthStencilState( &depth_stencil_info )
        .setPColorBlendState( &color_blend_info )
        .setPDynamicState( &dynamic_state_info )
        .setLayout( *pipeline.pipeline_layout )
        .setRenderPass( *render_pass.render_pass )
        .setSubpass( 0 )
    };
    auto raw_pipeline = context.device->createGraphicsPipelines(
      *context.pipeline_cache, pipeline_create_info
    );
    vk::ObjectDestroy< vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > deleter( *context.device, nullptr, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE () );
    pipeline.set_pipeline( vk::UniqueHandle< vk::Pipeline, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE >( raw_pipeline.value[ 0 ], deleter ) );
    return pipeline;
  }
}

