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
namespace vw {
  vk::Filter to_vulkan_mag_filter( fx::gltf::Sampler::MagFilter v ) {
    if( v == fx::gltf::Sampler::MagFilter::None ) return vk::Filter::eNearest;
    else if( v == fx::gltf::Sampler::MagFilter::Nearest ) return vk::Filter::eNearest;
    else if( v == fx::gltf::Sampler::MagFilter::Linear ) return vk::Filter::eLinear;
    else throw invalid_gltf( "不正なmag_filter", __FILE__, __LINE__ );
  }
  vk::Filter to_vulkan_min_filter( fx::gltf::Sampler::MinFilter v ) {
    if( v == fx::gltf::Sampler::MinFilter::None ) return vk::Filter::eNearest;
    else if( v == fx::gltf::Sampler::MinFilter::Nearest ) return vk::Filter::eNearest;
    else if( v == fx::gltf::Sampler::MinFilter::Linear ) return vk::Filter::eLinear;
    else if( v == fx::gltf::Sampler::MinFilter::NearestMipMapNearest ) return vk::Filter::eNearest;
    else if( v == fx::gltf::Sampler::MinFilter::LinearMipMapNearest ) return vk::Filter::eLinear;
    else if( v == fx::gltf::Sampler::MinFilter::NearestMipMapLinear ) return vk::Filter::eNearest;
    else if( v == fx::gltf::Sampler::MinFilter::LinearMipMapLinear ) return vk::Filter::eLinear;
    else throw invalid_gltf( "不正なmin_filter", __FILE__, __LINE__ );
  }
  vk::SamplerMipmapMode to_vulkan_mipmap_mode( fx::gltf::Sampler::MinFilter v ) {
    if( v == fx::gltf::Sampler::MinFilter::None ) return vk::SamplerMipmapMode::eNearest;
    else if( v == fx::gltf::Sampler::MinFilter::Nearest ) return vk::SamplerMipmapMode::eNearest;
    else if( v == fx::gltf::Sampler::MinFilter::Linear ) return vk::SamplerMipmapMode::eNearest;
    else if( v == fx::gltf::Sampler::MinFilter::NearestMipMapNearest ) return vk::SamplerMipmapMode::eNearest;
    else if( v == fx::gltf::Sampler::MinFilter::LinearMipMapNearest ) return vk::SamplerMipmapMode::eNearest;
    else if( v == fx::gltf::Sampler::MinFilter::NearestMipMapLinear ) return vk::SamplerMipmapMode::eLinear;
    else if( v == fx::gltf::Sampler::MinFilter::LinearMipMapLinear ) return vk::SamplerMipmapMode::eLinear;
    else throw invalid_gltf( "不正なmin_filter", __FILE__, __LINE__ );
  }
  vk::SamplerAddressMode to_vulkan_address_mode( fx::gltf::Sampler::WrappingMode v ) {
    if( v == fx::gltf::Sampler::WrappingMode::ClampToEdge ) return vk::SamplerAddressMode::eClampToEdge;
    else if( v == fx::gltf::Sampler::WrappingMode::MirroredRepeat ) return vk::SamplerAddressMode::eMirroredRepeat;
    else if( v == fx::gltf::Sampler::WrappingMode::Repeat ) return vk::SamplerAddressMode::eRepeat;
    else throw invalid_gltf( "不正なaddress_mode", __FILE__, __LINE__ );
  }
}

