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
#include <vw/exceptions.h>
#include <viewer/texture.h>
namespace viewer {
  texture_t create_texture(
    const fx::gltf::Document &doc,
    int32_t index,
    const vw::context_t&,
    const images_t &images,
    const samplers_t &samplers,
    const sampler_t &default_sampler
  ) {
    if( index < 0 || doc.textures.size() <= size_t( index ) ) throw vw::invalid_gltf( "参照されたtextureが存在しない", __FILE__, __LINE__ );
    const auto &texture = doc.textures[ index ];
    const sampler_t &sampler =
      ( texture.sampler < 0 || samplers.size() <= size_t( texture.sampler ) ) ?
      default_sampler :
      samplers[ texture.sampler ];
    if( texture.source < 0 || images.size() <= size_t( texture.source ) ) throw vw::invalid_gltf( "参照されたimageが存在しない", __FILE__, __LINE__ );
    const auto &image = images[ texture.source ];
    texture_t texture_;
    texture_.set_unorm(
      vk::DescriptorImageInfo()
        .setImageLayout( vk::ImageLayout::eShaderReadOnlyOptimal )
        .setImageView( *image.unorm.image_view )
        .setSampler( *sampler.sampler )
    );
    texture_.set_srgb(
      vk::DescriptorImageInfo()
        .setImageLayout( vk::ImageLayout::eShaderReadOnlyOptimal )
        .setImageView( *image.srgb.image_view )
        .setSampler( *sampler.sampler )
    );
    return texture_;
  }
  textures_t create_texture(
    const fx::gltf::Document &doc,
    const vw::context_t &context,
    const images_t &images,
    const samplers_t &samplers,
    const sampler_t &default_sampler
  ) {
    textures_t textures;
    for( uint32_t i = 0u; i != doc.textures.size(); ++i ) {
      textures.push_back(
        create_texture(
          doc,
          i,
          context,
          images,
          samplers,
          default_sampler
        )
      );
    }
    return textures;
  }
}

