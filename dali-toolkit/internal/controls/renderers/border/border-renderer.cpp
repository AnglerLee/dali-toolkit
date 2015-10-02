/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// CLASS HEADER
#include "border-renderer.h"

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>

//INTERNAL INCLUDES
#include <dali-toolkit/internal/controls/renderers/renderer-factory-impl.h>
#include <dali-toolkit/internal/controls/renderers/renderer-factory-cache.h>
#include <dali-toolkit/internal/controls/renderers/control-renderer-data-impl.h>

namespace Dali
{

namespace Toolkit
{

namespace Internal
{

namespace
{
const char * const RENDERER_TYPE("renderer-type");
const char * const RENDERER_TYPE_VALUE("border-renderer");

const char * const COLOR_NAME("border-color");
const char * const COLOR_UNIFORM_NAME("uBorderColor");
const char * const SIZE_NAME("border-size");
const char * const SIZE_UNIFORM_NAME("uBorderSize");

const char * const POSITION_ATTRIBUTE_NAME("aPosition");
const char * const DRIFT_ATTRIBUTE_NAME("aDrift");
const char * const INDEX_NAME("indices");


const char* VERTEX_SHADER = DALI_COMPOSE_SHADER(
  attribute mediump vec2 aPosition;\n
  attribute mediump vec2 aDrift;\n
  uniform mediump mat4 uMvpMatrix;\n
  uniform mediump vec3 uSize;\n
  uniform mediump float uBorderSize;\n
  \n
  void main()\n
  {\n
    vec2 position = aPosition*uSize.xy + aDrift*uBorderSize;\n
    gl_Position = uMvpMatrix * vec4(position, 0.0, 1.0);\n
  }\n
);

const char* FRAGMENT_SHADER = DALI_COMPOSE_SHADER(
  uniform lowp vec4 uColor;\n
  uniform lowp vec4 uBorderColor;\n
  \n
  void main()\n
  {\n
    gl_FragColor = uBorderColor*uColor;\n
  }\n
);
}

BorderRenderer::BorderRenderer()
: ControlRenderer(),
  mBorderColor( Color::TRANSPARENT ),
  mBorderSize( 0.f ),
  mBorderColorIndex( Property::INVALID_INDEX ),
  mBorderSizeIndex( Property::INVALID_INDEX )
{
}

BorderRenderer::~BorderRenderer()
{
}

void BorderRenderer::DoInitialize( RendererFactoryCache& factoryCache, const Property::Map& propertyMap )
{
  Initialize( factoryCache );

  Property::Value* color = propertyMap.Find( COLOR_NAME );
  if( !( color && color->Get(mBorderColor) ) )
  {
    DALI_LOG_ERROR( "Fail to provide a border color to the BorderRenderer object" );
  }

  Property::Value* size = propertyMap.Find( SIZE_NAME );
  if( !( size && size->Get(mBorderSize) ) )
  {
    DALI_LOG_ERROR( "Fail to provide a border size to the BorderRenderer object" );
  }
}

void BorderRenderer::SetClipRect( const Rect<int>& clipRect )
{
  ControlRenderer::SetClipRect( clipRect );

  //ToDo: renderer responds to the clipRect change
}

void BorderRenderer::DoSetOnStage( Actor& actor )
{
  mBorderColorIndex = (mImpl->mRenderer).RegisterProperty( COLOR_UNIFORM_NAME, mBorderColor );
  if( mBorderColor.a < 1.f )
  {
    (mImpl->mRenderer).GetMaterial().SetBlendMode( BlendingMode::ON );
  }
  mBorderSizeIndex = (mImpl->mRenderer).RegisterProperty( SIZE_UNIFORM_NAME, mBorderSize );
}

void BorderRenderer::DoCreatePropertyMap( Property::Map& map ) const
{
  map.Clear();
  map.Insert( RENDERER_TYPE, RENDERER_TYPE_VALUE );
  map.Insert( COLOR_NAME, mBorderColor );
  map.Insert( SIZE_NAME, mBorderSize );
}

void BorderRenderer::Initialize( RendererFactoryCache& factoryCache)
{
  mImpl->mGeometry = factoryCache.GetGeometry( RendererFactoryCache::BORDER_GEOMETRY );
  if( !(mImpl->mGeometry) )
  {
    mImpl->mGeometry =  CreateBorderGeometry();
    factoryCache.SaveGeometry( RendererFactoryCache::QUAD_GEOMETRY, mImpl->mGeometry );
  }

  mImpl->mShader = factoryCache.GetShader( RendererFactoryCache::BORDER_SHADER );
  if( !(mImpl->mShader) )
  {
    mImpl->mShader = Shader::New( VERTEX_SHADER, FRAGMENT_SHADER );
    factoryCache.SaveShader( RendererFactoryCache::COLOR_SHADER, mImpl->mShader );
  }
}

void BorderRenderer::SetBorderColor(const Vector4& color)
{
  mBorderColor = color;

  if( mImpl->mIsOnStage )
  {
    (mImpl->mRenderer).SetProperty( mBorderColorIndex, color );
    if( color.a < 1.f &&  (mImpl->mRenderer).GetMaterial().GetBlendMode() != BlendingMode::ON)
    {
      (mImpl->mRenderer).GetMaterial().SetBlendMode( BlendingMode::ON );
    }
  }
}

void BorderRenderer::SetBorderSize( float size )
{
  mBorderSize = size;

  if( mImpl->mIsOnStage )
  {
    (mImpl->mRenderer).SetProperty( mBorderSizeIndex, size );
  }
}

/**
 * Vertices and triangles of the border geometry:
 *
 * vertex position = aPosition*uSize.xy + aDrift*uBorderSize;
 *
 * 0--1--2--3
 * | /| /| /|
 * |/ |/ |/ |
 * 4--5--6--7
 * | /|  | /|
 * |/ |  |/ |
 * 8--9--10-11
 * | /| /| /|
 * |/ |/ |/ |
 * 12-13-14-15
 */
Geometry BorderRenderer::CreateBorderGeometry()
{
  const float halfWidth = 0.5f;
  const float halfHeight = 0.5f;
  struct BorderVertex { Vector2 position; Vector2 drift;};
  BorderVertex borderVertexData[16] =
  {
      { Vector2(-halfWidth, -halfHeight), Vector2(0.f, 0.f) },
      { Vector2(-halfWidth, -halfHeight), Vector2(1.f, 0.f) },
      { Vector2(halfWidth, -halfHeight),  Vector2(-1.f, 0.f) },
      { Vector2(halfWidth, -halfHeight),  Vector2(0.f, 0.f) },

      { Vector2(-halfWidth, -halfHeight), Vector2(0.f, 1.f) },
      { Vector2(-halfWidth, -halfHeight), Vector2(1.f, 1.f) },
      { Vector2(halfWidth, -halfHeight),  Vector2(-1.f, 1.f) },
      { Vector2(halfWidth, -halfHeight),  Vector2(0.f, 1.f) },

      { Vector2(-halfWidth, halfHeight), Vector2(0.f, -1.f) },
      { Vector2(-halfWidth, halfHeight), Vector2(1.f, -1.f) },
      { Vector2(halfWidth, halfHeight),  Vector2(-1.f, -1.f) },
      { Vector2(halfWidth, halfHeight),  Vector2(0.f, -1.f) },

      { Vector2(-halfWidth, halfHeight), Vector2(0.f, 0.f) },
      { Vector2(-halfWidth, halfHeight), Vector2(1.f, 0.f) },
      { Vector2(halfWidth, halfHeight),  Vector2(-1.f, 0.f) },
      { Vector2(halfWidth, halfHeight),  Vector2(0.f, 0.f) },
  };

  Property::Map borderVertexFormat;
  borderVertexFormat[POSITION_ATTRIBUTE_NAME] = Property::VECTOR2;
  borderVertexFormat[DRIFT_ATTRIBUTE_NAME] = Property::VECTOR2;
  PropertyBuffer borderVertices = PropertyBuffer::New( borderVertexFormat, 16 );
  borderVertices.SetData(borderVertexData);

  // Create indices
  unsigned int indexData[48] = { 0, 4, 1, 1, 4, 5, 1, 5, 2, 2, 5, 6, 2, 6,3, 3, 6, 7,
                                 4, 8, 5, 5, 8, 9, 6, 10, 7, 7, 10, 11,
                                 8, 12, 9, 9, 12, 13, 9, 13, 10, 10, 13, 14, 10, 11, 14, 11, 14, 15};

  Property::Map indexFormat;
  indexFormat[INDEX_NAME] = Property::INTEGER;
  PropertyBuffer indices = PropertyBuffer::New( indexFormat, 48 );
  indices.SetData(indexData);

  // Create the geometry object
  Geometry geometry = Geometry::New();
  geometry.AddVertexBuffer( borderVertices );
  geometry.SetIndexBuffer( indices );

  return geometry;
}

} // namespace Internal

} // namespace Toolkit

} // namespace Dali
