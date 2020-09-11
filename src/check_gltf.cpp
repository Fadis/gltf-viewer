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
#include <string>
#include <fx/gltf.h>
#include <stamp/exception.h>
#include <boost/program_options.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
LIBSTAMP_EXCEPTION( runtime_error, invalid_gltf, "不正なGLTF" );
void indent( unsigned int space_count ) {
  for( unsigned int i = 0; i != space_count; ++i ) std::cout << " ";
}
template< typename T >
glm::tmat4x4< T > to_matrix(
  const std::array< T, 3 > &t,
  const std::array< T, 4 > &r,
  const std::array< T, 3 > &s
) {
  glm::tmat4x4< T > m( 1.0f );
  m = glm::scale( m, glm::tvec3< T >( s[ 0 ], s[ 1 ], s[ 2 ] ) );
  auto rot = glm::tquat< T >( r[ 0 ], r[ 1 ], r[ 2 ], r[ 3 ] );
  auto rotm = glm::mat4_cast( rot );
  m = rotm * m;
  m = glm::translate( m, glm::tvec3< T >( t[ 0 ], t[ 1 ], t[ 2 ] ) );
  return m;
}
template< typename T >
glm::tmat4x4< T > to_matrix(
  const std::array< T, 16 > &t
) {
  return glm::tmat4x4< T >(
    t[ 0 ], t[ 1 ], t[ 2 ], t[ 3 ],
    t[ 4 ], t[ 5 ], t[ 6 ], t[ 7 ],
    t[ 8 ], t[ 9 ], t[ 10 ], t[ 11 ],
    t[ 12 ], t[ 13 ], t[ 14 ], t[ 15 ]
  );
}
template< typename T >
void dump_matrix( const glm::tmat4x4< T > &v ) {
  bool initial = true;
  std::cout << "列メジャー( ";
  for( unsigned int i = 0; i != 4; ++i ) {
    if( initial ) initial = false;
    else std::cout << ", ";
    std::cout << "( " << v[ i ][ 0 ] << ", " << v[ i ][ 1 ] << ", " << v[ i ][ 2 ] << ", " << v[ i ][ 3 ] << " )";
  }
  std::cout << " )";
}
void dump_matrix( const std::array< float, 16 > &v ) {
  bool initial = true;
  std::cout << "列メジャー( ";
  for( unsigned int i = 0; i != 4; ++i ) {
    if( initial ) initial = false;
    else std::cout << ", ";
    std::cout << "( " << v[ 0 + 4 * i ] << ", " << v[ 1 + 4 * i ] << ", " << v[ 2 + 4 * i ] << ", " << v[ 3 + 4 * i ] << " )";
  }
  std::cout << " )";
}
template< size_t i >
void dump_vector( const std::array< float, i > &v ) {
  bool initial = true;
  std::cout << "( ";
  for( const auto &e :v ) {
    if( initial ) initial = false;
    else std::cout << ", ";
    std::cout << e;
  }
  std::cout << " )";
}
std::string to_string( fx::gltf::Accessor::ComponentType type ) {
  if( type == fx::gltf::Accessor::ComponentType::None ) return "不明";
  if( type == fx::gltf::Accessor::ComponentType::Byte ) return "int8_t";
  if( type == fx::gltf::Accessor::ComponentType::UnsignedByte ) return "uint8_t";
  if( type == fx::gltf::Accessor::ComponentType::Short ) return "int16_t";
  if( type == fx::gltf::Accessor::ComponentType::UnsignedShort ) return "uint16_t";
  if( type == fx::gltf::Accessor::ComponentType::UnsignedInt ) return "uint32_t";
  if( type == fx::gltf::Accessor::ComponentType::Float ) return "float";
  return "不明";
}
std::string to_string( fx::gltf::Accessor::Type type ) {
  if( type == fx::gltf::Accessor::Type::None ) return "x不明";
  if( type == fx::gltf::Accessor::Type::Scalar ) return "x1";
  if( type == fx::gltf::Accessor::Type::Vec2 ) return "x2";
  if( type == fx::gltf::Accessor::Type::Vec3 ) return "x3";
  if( type == fx::gltf::Accessor::Type::Vec4 ) return "x4";
  if( type == fx::gltf::Accessor::Type::Mat2 ) return "x2x2";
  if( type == fx::gltf::Accessor::Type::Mat3 ) return "x3x3";
  if( type == fx::gltf::Accessor::Type::Mat4 ) return "x4x4";
  return "不明";
}
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

template< typename T >
std::string to_string( const std::vector< T > &v, bool is_float ) {
  std::string serialized = "[";
  bool initial = true;
  for( auto &e: v ) {
    if( initial ) initial = false;
    else serialized += ',';
    if( is_float )
      serialized += std::to_string( e );
    else
      serialized += std::to_string( int( e ) );
  }
  serialized += ']';
  return serialized;
}
template< typename T, size_t i >
std::string to_string( const std::array< T, i > &v, bool is_float ) {
  std::string serialized = "[";
  bool initial = true;
  for( auto &e: v ) {
    if( initial ) initial = false;
    else serialized += ',';
    if( is_float )
      serialized += std::to_string( e );
    else
      serialized += std::to_string( int( e ) );
  }
  serialized += ']';
  return serialized;
}
void dump_accessor( unsigned int space_count, const fx::gltf::Document &doc, int32_t index, const std::string &name ) {
  if( index < 0 || doc.accessors.size() <= size_t( index ) ) throw invalid_gltf( "参照されたaccessorsが存在しない", __FILE__, __LINE__ );
  const auto &accessor = doc.accessors[ index ];
  if( accessor.bufferView < 0 || doc.bufferViews.size() <= size_t( accessor.bufferView ) ) throw invalid_gltf( "参照されたbufferViewが存在しない", __FILE__, __LINE__ );
  const auto &view = doc.bufferViews[ accessor.bufferView ];
  if( view.buffer < 0 || doc.buffers.size() <= size_t( view.buffer ) ) throw invalid_gltf( "参照されたbufferが存在しない", __FILE__, __LINE__ );
  const auto &buffer = doc.buffers[ view.buffer ];
  indent( space_count );
  std::cout << name << ": ";
  const uint32_t default_stride = to_size( accessor.componentType ) * to_size( accessor.type );
  uint32_t stride = view.byteStride ? view.byteStride : default_stride;
  const uint32_t max_count = ( view.byteLength - ( accessor.byteOffset ) ) / stride;
  if( accessor.count > max_count ) throw invalid_gltf( "指定された要素数に対してbufferViewが小さすぎる" );
  const uint32_t offset = accessor.byteOffset + view.byteOffset;
  std::cout << buffer.uri << "の" << offset << "バイト目から" << offset + accessor.count * stride << "バイト目まで(" << accessor.count << "要素)を" << stride << "バイト間隔で" << to_string( accessor.componentType ) << to_string( accessor.type ) << "として使用";
  if( !accessor.min.empty() || !accessor.max.empty() ) {
    const bool is_float = accessor.componentType == fx::gltf::Accessor::ComponentType::Float;
    std::cout << "(" << to_string( accessor.min, is_float ) << "から" << to_string( accessor.max, is_float ) << ")";
  }
  std::cout << std::endl;
}
std::string to_string( fx::gltf::Material::AlphaMode mode ) {
  if( mode == fx::gltf::Material::AlphaMode::Opaque ) return "不透明";
  if( mode == fx::gltf::Material::AlphaMode::Mask ) return "マスク";
  if( mode == fx::gltf::Material::AlphaMode::Blend ) return "ブレンド";
  return "不明";
}
std::string to_string( fx::gltf::Sampler::MagFilter mode ) {
  if( mode == fx::gltf::Sampler::MagFilter::None ) return "未定義";
  if( mode == fx::gltf::Sampler::MagFilter::Nearest ) return "最近接";
  if( mode == fx::gltf::Sampler::MagFilter::Linear ) return "線形";
  return "不明";
}
std::string to_string( fx::gltf::Sampler::MinFilter mode ) {
  if( mode == fx::gltf::Sampler::MinFilter::None ) return "未定義";
  if( mode == fx::gltf::Sampler::MinFilter::Nearest ) return "最近接";
  if( mode == fx::gltf::Sampler::MinFilter::Linear ) return "線形";
  if( mode == fx::gltf::Sampler::MinFilter::NearestMipMapNearest ) return "最近接(ミップマップ最近接)";
  if( mode == fx::gltf::Sampler::MinFilter::LinearMipMapNearest ) return "線形(ミップマップ最近接)";
  if( mode == fx::gltf::Sampler::MinFilter::NearestMipMapLinear ) return "最近接(ミップマップ線形)";
  if( mode == fx::gltf::Sampler::MinFilter::LinearMipMapLinear ) return "線形(ミップマップ線形)";
  return "不明";
}
std::string to_string( fx::gltf::Sampler::WrappingMode mode ) {
  if( mode == fx::gltf::Sampler::WrappingMode::ClampToEdge ) return "繰り返さない";
  if( mode == fx::gltf::Sampler::WrappingMode::MirroredRepeat ) return "反転繰り返し";
  if( mode == fx::gltf::Sampler::WrappingMode::Repeat ) return "繰り返し";
  return "不明";
}
void dump_texture( unsigned int space_count, const fx::gltf::Document &doc, int32_t index, const std::string &name ) {
  if( index < 0 || doc.textures.size() <= size_t( index ) ) throw invalid_gltf( "参照されたtextureが存在しない", __FILE__, __LINE__ );
  const auto &texture = doc.textures[ index ];
  if( index < 0 || doc.images.size() <= size_t( texture.source ) ) throw invalid_gltf( "参照されたimageが存在しない", __FILE__, __LINE__ );
  const auto &image = doc.images[ texture.source ];
  if( index < 0 || doc.samplers.size() <= size_t( texture.sampler ) ) throw invalid_gltf( "参照されたsamplerが存在しない", __FILE__, __LINE__ );
  const auto &sampler = doc.samplers[ texture.sampler ];
  std::string image_uri;
  if( !image.uri.empty() ) {
    image_uri = image.uri;
  }
  else if( image.bufferView != -1 ) {
    image_uri = "buffer:///";
    image_uri += std::to_string( image.bufferView );
  }
  std::string mime = "不明";
  if( !image.mimeType.empty() )
    mime = image.mimeType;
  indent( space_count );
  std::cout << name << ": " << image_uri << " (" << mime << ") 拡大: " << to_string( sampler.magFilter ) << " 縮小: " << to_string( sampler.minFilter ) << " S軸: " << to_string( sampler.wrapS ) << " T軸: " << to_string( sampler.wrapT ) << std::endl;
}
void dump_material( unsigned int space_count, const fx::gltf::Document &doc, int32_t index ) {
  if( index < 0 || doc.materials.size() <= size_t( index ) ) throw invalid_gltf( "参照されたmaterialsが存在しない", __FILE__, __LINE__ );
  const auto &material = doc.materials[ index ];
  indent( space_count );
  std::cout << "マテリアル" << std::endl;
  ++space_count;
  indent( space_count );
  std::cout << "透過: " << to_string( material.alphaMode );
  if( material.alphaMode == fx::gltf::Material::AlphaMode::Mask ) std::cout << " 閾値:" << material.alphaCutoff << std::endl;
  else std::cout << std::endl;
  indent( space_count );
  if( material.doubleSided ) std::cout << "両面" << std::endl;
  else std::cout << "片面" << std::endl;
  if( material.normalTexture.index != -1 ) {
    dump_texture( space_count, doc, material.normalTexture.index, "法線" );
  }
  if( material.occlusionTexture.index != -1 ) {
    dump_texture( space_count, doc, material.occlusionTexture.index, "遮蔽" );
  }
  if( material.emissiveTexture.index != -1 ) {
    dump_texture( space_count, doc, material.occlusionTexture.index, "発光" );
  }
  if( material.pbrMetallicRoughness.baseColorTexture.index != -1 ) {
    dump_texture( space_count, doc, material.pbrMetallicRoughness.baseColorTexture.index, "アルベド" );
  }
  else {
    indent( space_count );
    std::cout << "アルベド: " << to_string( material.pbrMetallicRoughness.baseColorFactor, true ) << std::endl;
  }
  if( material.pbrMetallicRoughness.metallicRoughnessTexture.index != -1 ) {
    dump_texture( space_count, doc, material.pbrMetallicRoughness.metallicRoughnessTexture.index, "メタリック・ラフネス" );
  }
  else {
    indent( space_count );
    std::cout << "メタリックネス: " << std::to_string( material.pbrMetallicRoughness.metallicFactor ) << std::endl;
    indent( space_count );
    std::cout << "ラフネス: " << std::to_string( material.pbrMetallicRoughness.roughnessFactor ) << std::endl;
  }

  --space_count;
}
void dump_primitive( unsigned int space_count, const fx::gltf::Document &doc, const fx::gltf::Primitive &primitive ) {
  indent( space_count );
  std::cout << "プリミティブ" << std::endl;
  if( primitive.indices != -1 ) {
    dump_accessor( space_count + 1, doc, primitive.indices, "インデックス" );
  }
  for( const auto &[target,index]: primitive.attributes ) {
    dump_accessor( space_count + 1, doc, index, target );
  }
  if( primitive.material != -1 ){
    ++space_count;
    dump_material( space_count, doc, primitive.material );
    --space_count;
  }
}
void dump_camera( unsigned int space_count, const fx::gltf::Document &doc, int32_t index ) {
  if( index < 0 || doc.cameras.size() <= size_t( index ) ) throw invalid_gltf( "参照されたcameraが存在しない", __FILE__, __LINE__ );
  const auto &camera = doc.cameras[ index ];
  indent( space_count );
  std::cout << "カメラ" << std::endl;
  {
    ++space_count;
    if( !camera.name.empty() ) {
      indent( space_count );
      std::cout << "名前: " << camera.name << std::endl;
    }
    if( camera.type == fx::gltf::Camera::Type::Orthographic ) {
      auto projection = glm::ortho( camera.orthographic.xmag, -camera.orthographic.xmag, -camera.orthographic.ymag, camera.orthographic.ymag, camera.orthographic.znear, camera.orthographic.zfar );
      indent( space_count );
      std::cout << "並行投影: ";
      dump_matrix( projection );
      std::cout << std::endl;
    }
    else if( camera.type == fx::gltf::Camera::Type::Perspective ) {
      auto projection = glm::perspective( camera.perspective.yfov, camera.perspective.aspectRatio, -camera.perspective.znear, camera.perspective.zfar );
      indent( space_count );
      std::cout << "透視投影: ";
      dump_matrix( projection );
      std::cout << std::endl;
    }
    else throw invalid_gltf( "未知のカメラの種類", __FILE__, __LINE__ );
    --space_count;
  }
}
void dump_mesh( unsigned int space_count, const fx::gltf::Document &doc, int32_t index ) {
  if( index < 0 || doc.meshes.size() <= size_t( index ) ) throw invalid_gltf( "参照されたmeshが存在しない", __FILE__, __LINE__ );
  const auto &mesh = doc.meshes[ index ];
  indent( space_count );
  std::cout << "メッシュ" << std::endl;
  {
    ++space_count;
    if( !mesh.name.empty() ) {
      indent( space_count );
      std::cout << "名前: " << mesh.name << std::endl;
    }
    for( const auto &p: mesh.primitives ) {
      dump_primitive( space_count + 1, doc, p );
    }
    --space_count;
  }
}
void dump_node_simple( unsigned int space_count, const fx::gltf::Document &doc, int32_t index ) {
  if( index < 0 || doc.nodes.size() <= size_t( index ) ) throw invalid_gltf( "参照されたnodesが存在しない", __FILE__, __LINE__ );
  const auto &node = doc.nodes[ index ];
  if( !node.name.empty() ) {
    indent( space_count );
    std::cout << node.name << std::endl;
  }
}
void dump_skin( unsigned int space_count, const fx::gltf::Document &doc, int32_t index ) {
  if( index < 0 || doc.skins.size() <= size_t( index ) ) throw invalid_gltf( "参照されたskinが存在しない", __FILE__, __LINE__ );
  const auto &skin = doc.skins[ index ];
  ++space_count;
  if( !skin.name.empty() ) {
    indent( space_count );
    std::cout << "名前: " << skin.name << std::endl;
  }
  if( skin.inverseBindMatrices != -1 ) {
    dump_accessor( space_count + 1, doc, skin.inverseBindMatrices, "inverseBindMatrix" );
  }
  if( !skin.joints.empty() ) {
    indent( space_count );
    std::cout << "ジョイント" << std::endl;
    for( const auto &i: skin.joints )
      dump_node_simple( space_count + 1, doc, i );
  }
  --space_count;
}
bool has_mesh( const fx::gltf::Document &doc, int32_t index ) {
  if( index < 0 || doc.nodes.size() <= size_t( index ) ) throw invalid_gltf( "参照されたnodesが存在しない", __FILE__, __LINE__ );
  const auto &node = doc.nodes[ index ];
  if( std::find_if( node.children.begin(), node.children.end(), [&]( const auto &v ) { return has_mesh( doc, v ); } ) != node.children.end() ) return true;
  else return node.mesh != -1;
}
void dump_node( unsigned int space_count, const fx::gltf::Document &doc, int32_t index ) {
  if( index < 0 || doc.nodes.size() <= size_t( index ) ) throw invalid_gltf( "参照されたnodesが存在しない", __FILE__, __LINE__ );
  const auto &node = doc.nodes[ index ];
  indent( space_count );
  std::cout << "ノード" << std::endl;
  {
    ++space_count;
    if( !node.name.empty() ) {
      indent( space_count );
      std::cout << "名前: " << node.name << std::endl;
    }
    if( node.matrix != fx::gltf::defaults::IdentityMatrix ) {
      indent( space_count );
      std::cout << "行列: ";
      dump_matrix( node.matrix );
      std::cout << std::endl;
    }
    else {
      indent( space_count );
      std::cout << "行列: ";
      dump_matrix( to_matrix( node.translation, node.rotation, node.scale ) );
      std::cout << std::endl;
      if( node.translation != fx::gltf::defaults::NullVec3 ) {
        indent( space_count );
        std::cout << "移動: ";
        dump_vector( node.translation );
        std::cout << std::endl;
      }
      if( node.rotation != fx::gltf::defaults::IdentityRotation ) {
        indent( space_count );
        std::cout << "回転: ";
        dump_vector( node.rotation );
        std::cout << std::endl;
      }
      if( node.scale != fx::gltf::defaults::IdentityVec3 ) {
        indent( space_count );
        std::cout << "スケール: ";
        dump_vector( node.scale );
        std::cout << std::endl;
      }
    }
    std::cout << std::endl;
    if( node.mesh != -1 ) {
      indent( space_count );
      std::cout << "メッシュ" << std::endl;
      dump_mesh( space_count + 1, doc, node.mesh );
    }
    if( node.skin != -1 ) {
      indent( space_count );
      std::cout << "スキン" << std::endl;
      dump_skin( space_count + 1, doc, node.skin );
    }
    if( node.camera != -1 ) {
      dump_camera( space_count + 1, doc, node.camera );
    }
    --space_count;
  }
  for( const auto &child: node.children )
    dump_node( space_count + 1, doc, child );
}
std::string to_string( const fx::gltf::Animation::Sampler::Type &type ) {
  if( type == fx::gltf::Animation::Sampler::Type::Linear ) return "線形補間";
  else if( type == fx::gltf::Animation::Sampler::Type::Step ) return "補間なし";
  else if( type == fx::gltf::Animation::Sampler::Type::CubicSpline ) return "スプライン補間";
  else return "不明";
}
void dump_animation( unsigned int space_count, const fx::gltf::Document &doc, const fx::gltf::Animation &animation ) {
  indent( space_count );
  std::cout << "アニメーション" << std::endl;
  {
    ++space_count;
    if( !animation.name.empty() ) {
      indent( space_count );
      std::cout << "名前: " << animation.name << std::endl;
    }
    for( const auto &channel: animation.channels ) {
      indent( space_count );
      std::cout << "チャネル" << std::endl;
      {
        if( channel.sampler < 0 || animation.samplers.size() <= size_t( channel.sampler ) ) throw invalid_gltf( "参照されたsamplerが存在しない", __FILE__, __LINE__ );
        const auto &sampler = animation.samplers[ channel.sampler ];
        ++space_count;
        indent( space_count );
        std::cout << "タイプ: " << to_string( sampler.interpolation ) << std::endl;
        dump_accessor( space_count, doc, sampler.input, "入力" );
        dump_accessor( space_count, doc, sampler.output, "出力" );
        indent( space_count );
        std::cout << "対象" << std::endl;
        dump_node_simple( space_count + 1, doc, channel.target.node );
        indent( space_count );
        std::cout << "パス: " << channel.target.path << std::endl;
        --space_count;
      }
    }
    --space_count;
  }
}
int main( int argc, const char *argv[] ) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  std::string filename;
  desc.add_options()
    ( "help,h", "show this message" )
    ( "input,i", po::value< std::string >(&filename), "window size" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) ) {
    std::cout << desc << std::endl;
    exit( 0 );
  }
  if( !vm.count( "input" ) ) {
    std::cerr << "入力ファイルが指定されていません " << std::endl;
    std::cerr << desc << std::endl;
    exit( 1 );
  }
  fx::gltf::Document doc = fx::gltf::LoadFromText( filename );
  unsigned int space_count = 0u;
  for( const auto &scene: doc.scenes ) {
    indent( space_count );
    std::cout << "シーン" << std::endl;
    for( const auto &node_index: scene.nodes ) {
      dump_node( space_count + 1, doc, node_index );
    }
  }
  for( const auto &animation: doc.animations ) {
    dump_animation( space_count + 1, doc, animation );
  }
}
