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
#include <vulkan/vulkan.hpp>
#include <fx/gltf.h>
#include <vw/exceptions.h>
namespace vw {
  uint32_t to_size( fx::gltf::Accessor::ComponentType type ) {
    if( type == fx::gltf::Accessor::ComponentType::None ) return 0u;
    if( type == fx::gltf::Accessor::ComponentType::Byte ) return 1u;
    if( type == fx::gltf::Accessor::ComponentType::UnsignedByte ) return 1u;
    if( type == fx::gltf::Accessor::ComponentType::Short ) return 2u;
    if( type == fx::gltf::Accessor::ComponentType::UnsignedShort ) return 2u;
    if( type == fx::gltf::Accessor::ComponentType::UnsignedInt ) return 4u;
    if( type == fx::gltf::Accessor::ComponentType::Float ) return 4u;
    return 0u;
  }
  uint32_t to_size( fx::gltf::Accessor::Type type ) {
    if( type == fx::gltf::Accessor::Type::None ) return 0u;
    if( type == fx::gltf::Accessor::Type::Scalar ) return 1u;
    if( type == fx::gltf::Accessor::Type::Vec2 ) return 2u;
    if( type == fx::gltf::Accessor::Type::Vec3 ) return 3u;
    if( type == fx::gltf::Accessor::Type::Vec4 ) return 4u;
    if( type == fx::gltf::Accessor::Type::Mat2 ) return 4u;
    if( type == fx::gltf::Accessor::Type::Mat3 ) return 9u;
    if( type == fx::gltf::Accessor::Type::Mat4 ) return 16u;
    return 0u;
  }
  uint32_t to_size(
    fx::gltf::Accessor::ComponentType componentType,
    fx::gltf::Accessor::Type type
  ) {
    return to_size( componentType ) * to_size( type );
  }
  vk::Format to_vulkan_format(
    fx::gltf::Accessor::ComponentType componentType,
    fx::gltf::Accessor::Type type,
    bool normalize
  ) {
    if( componentType == fx::gltf::Accessor::ComponentType::Byte ) {
      if( type == fx::gltf::Accessor::Type::Scalar ) {
        if( normalize ) return vk::Format::eR8Snorm;
        else return vk::Format::eR8Sscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec2 ) {
        if( normalize ) return vk::Format::eR8G8Snorm;
        else return vk::Format::eR8G8Sscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec3 ) {
        if( normalize ) return vk::Format::eR8G8B8Snorm;
        else return vk::Format::eR8G8B8Sscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec4 ) {
        if( normalize ) return vk::Format::eR8G8B8A8Snorm;
        else return vk::Format::eR8G8B8A8Sscaled;
      }
      else throw invalid_gltf( "使用できないアクセサの型", __FILE__, __LINE__ );
    }
    else if( componentType == fx::gltf::Accessor::ComponentType::UnsignedByte ) {
      if( type == fx::gltf::Accessor::Type::Scalar ) {
        if( normalize ) return vk::Format::eR8Unorm;
        else return vk::Format::eR8Uscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec2 ) {
        if( normalize ) return vk::Format::eR8G8Unorm;
        else return vk::Format::eR8G8Uscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec3 ) {
        if( normalize ) return vk::Format::eR8G8B8Unorm;
        else return vk::Format::eR8G8B8Uscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec4 ) {
        if( normalize ) return vk::Format::eR8G8B8A8Unorm;
        else return vk::Format::eR8G8B8A8Uscaled;
      }
      else throw invalid_gltf( "使用できないアクセサの型", __FILE__, __LINE__ );
    }
    else if( componentType == fx::gltf::Accessor::ComponentType::Short ) {
      if( type == fx::gltf::Accessor::Type::Scalar ) {
        if( normalize ) return vk::Format::eR16Snorm;
        else return vk::Format::eR16Sscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec2 ) {
        if( normalize ) return vk::Format::eR16G16Snorm;
        else return vk::Format::eR8G8Sscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec3 ) {
        if( normalize ) return vk::Format::eR16G16B16Snorm;
        else return vk::Format::eR16G16B16Sscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec4 ) {
        if( normalize ) return vk::Format::eR16G16B16A16Snorm;
        else return vk::Format::eR16G16B16A16Sscaled;
      }
      else throw invalid_gltf( "使用できないアクセサの型", __FILE__, __LINE__ );
    }
    else if( componentType == fx::gltf::Accessor::ComponentType::UnsignedShort ) {
      if( type == fx::gltf::Accessor::Type::Scalar ) {
        if( normalize ) return vk::Format::eR16Unorm;
        else return vk::Format::eR16Uscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec2 ) {
        if( normalize ) return vk::Format::eR16G16Unorm;
        else return vk::Format::eR16G16Uscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec3 ) {
        if( normalize ) return vk::Format::eR16G16B16Unorm;
        else return vk::Format::eR16G16B16Uscaled;
      }
      else if( type == fx::gltf::Accessor::Type::Vec4 ) {
        if( normalize ) return vk::Format::eR16G16B16A16Unorm;
        else return vk::Format::eR16G16B16A16Uscaled;
      }
      else throw invalid_gltf( "使用できないアクセサの型", __FILE__, __LINE__ );
    }
    else if( componentType == fx::gltf::Accessor::ComponentType::UnsignedInt ) {
      if( type == fx::gltf::Accessor::Type::Scalar ) {
        if( normalize ) throw invalid_gltf( "使用できないアクセサの型", __FILE__, __LINE__ );
        else return vk::Format::eR32Uint;
      }
      else throw invalid_gltf( "使用できないアクセサの型", __FILE__, __LINE__ );
    }
    else if( componentType == fx::gltf::Accessor::ComponentType::Float ) {
      if( type == fx::gltf::Accessor::Type::Scalar ) {
        return vk::Format::eR32Sfloat;
      }
      else if( type == fx::gltf::Accessor::Type::Vec2 ) {
        return vk::Format::eR32G32Sfloat;
      }
      else if( type == fx::gltf::Accessor::Type::Vec3 ) {
        return vk::Format::eR32G32B32Sfloat;
      }
      else if( type == fx::gltf::Accessor::Type::Vec4 ) {
        return vk::Format::eR32G32B32A32Sfloat;
      }
      else throw invalid_gltf( "使用できないアクセサの型", __FILE__, __LINE__ );
    }
    else throw invalid_gltf( "使用できないアクセサの型", __FILE__, __LINE__ );
  }
  vk::IndexType to_vulkan_index_type( fx::gltf::Accessor::ComponentType type ) {
    if( type == fx::gltf::Accessor::ComponentType::UnsignedShort )
      return vk::IndexType::eUint16;
    else if( type == fx::gltf::Accessor::ComponentType::UnsignedInt )
      return vk::IndexType::eUint32;
    else throw invalid_gltf( "使用できないインデックスの型", __FILE__, __LINE__ );
  }
}

