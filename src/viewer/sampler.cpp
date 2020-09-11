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
#include <vw/sampler.h>
#include <vw/exceptions.h>
#include <viewer/sampler.h>
namespace viewer {
  sampler_t create_sampler(
    const fx::gltf::Document &doc,
    int32_t index,
    const vw::context_t &context
  ) {
    if( index < 0 || doc.samplers.size() <= size_t( index ) ) throw vw::invalid_gltf( "参照されたsamplerが存在しない", __FILE__, __LINE__ );
    const auto &sampler = doc.samplers[ index ];
    sampler_t sampler_;
    sampler_.set_sampler(
      context.device->createSamplerUnique(
        vk::SamplerCreateInfo()
          .setMagFilter( vw::to_vulkan_mag_filter( sampler.magFilter ) )
          .setMinFilter( vw::to_vulkan_min_filter( sampler.minFilter ) )
          .setMipmapMode( vw::to_vulkan_mipmap_mode( sampler.minFilter ) )
          .setAddressModeU( vw::to_vulkan_address_mode( sampler.wrapS ) )
          .setAddressModeV( vw::to_vulkan_address_mode( sampler.wrapT ) )
          .setAddressModeW( vw::to_vulkan_address_mode( sampler.wrapT ) )
          .setAnisotropyEnable( false )
          .setCompareEnable( false )
          .setMipLodBias( 0.f )
          .setMinLod( 0.f )
          .setMaxLod( 32.f )
          .setBorderColor( vk::BorderColor::eFloatTransparentBlack )
          .setUnnormalizedCoordinates( false )
      )
    );
    return sampler_;
  }
  samplers_t create_sampler(
    const fx::gltf::Document &doc,
    const vw::context_t &context
  ) {
    samplers_t samplers;
    for( uint32_t i = 0u; i != doc.samplers.size(); ++i ) {
      samplers.push_back(
        create_sampler(
          doc,
          i,
          context
        )
      );
    }
    return samplers;
  }
  sampler_t create_default_sampler(
    const vw::context_t &context
  ) {
    sampler_t sampler_;
    sampler_.set_sampler(
      context.device->createSamplerUnique(
        vk::SamplerCreateInfo()
          .setMagFilter( vw::to_vulkan_mag_filter( fx::gltf::Sampler::MagFilter::Linear ) )
          .setMinFilter( vw::to_vulkan_min_filter( fx::gltf::Sampler::MinFilter::LinearMipMapLinear ) )
          .setMipmapMode( vw::to_vulkan_mipmap_mode( fx::gltf::Sampler::MinFilter::LinearMipMapLinear ) )
          .setAddressModeU( vw::to_vulkan_address_mode( fx::gltf::Sampler::WrappingMode::Repeat ) )
          .setAddressModeV( vw::to_vulkan_address_mode( fx::gltf::Sampler::WrappingMode::Repeat ) )
          .setAddressModeW( vw::to_vulkan_address_mode( fx::gltf::Sampler::WrappingMode::Repeat ) )
          .setAnisotropyEnable( false )
          .setCompareEnable( false )
          .setMipLodBias( 0.f )
          .setMinLod( 0.f )
          .setMaxLod( 32.f )
          .setBorderColor( vk::BorderColor::eFloatTransparentBlack )
          .setUnnormalizedCoordinates( false )
      )
    );
    return sampler_;
  }
}
