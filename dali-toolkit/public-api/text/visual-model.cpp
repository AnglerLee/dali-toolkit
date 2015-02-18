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
#include <dali-toolkit/public-api/text/visual-model.h>

// INTERNAL INCLUDES
#include <dali/public-api/common/dali-vector.h>
#include <dali/public-api/math/vector2.h>
#include <dali-toolkit/public-api/text/line-run.h>

// EXTERNAL INCLUDES
#include <memory.h>
#include <vector>

namespace Dali
{

namespace Toolkit
{

namespace Text
{

const GlyphInfo GLYPH_INFO; // VCC to be removed.

struct VisualModel::Impl
{
  Vector<GlyphInfo>      mGlyphs;
  Vector<CharacterIndex> mGlyphsToCharacters;
  Vector<Length>         mCharactersPerGlyph;
  std::vector<Vector2>   mGlyphPositions;
};

VisualModelPtr VisualModel::New()
{
  return VisualModelPtr( new VisualModel() );
}

void VisualModel::SetGlyphs( const GlyphInfo* glyphs,
                             const CharacterIndex* characterIndices,
                             const Length* charactersPerGlyph,
                             Length numberOfGlyphs )
{
  Vector<GlyphInfo>& modelGlyphs = mImpl->mGlyphs;
  modelGlyphs.Resize( numberOfGlyphs );
  memcpy( &modelGlyphs[0], glyphs, numberOfGlyphs*sizeof(GlyphInfo) );

  Vector<CharacterIndex>& glyphsToCharacters = mImpl->mGlyphsToCharacters;
  glyphsToCharacters.Resize( numberOfGlyphs );
  memcpy( &glyphsToCharacters[0], characterIndices, numberOfGlyphs*sizeof(CharacterIndex) );

  Vector<Length>& modelCharactersPerGlyph = mImpl->mCharactersPerGlyph;
  modelCharactersPerGlyph.Resize( numberOfGlyphs );
  memcpy( &modelCharactersPerGlyph[0], charactersPerGlyph, numberOfGlyphs*sizeof(Length) );
}

Length VisualModel::GetNumberOfGlyphs() const
{
  return mImpl->mGlyphs.Count();
}

void VisualModel::GetGlyphs( GlyphIndex glyphIndex,
                             GlyphInfo* glyphs,
                             Length numberOfGlyphs ) const
{
  Vector<GlyphInfo>& modelGlyphs = mImpl->mGlyphs;
  memcpy( glyphs, &modelGlyphs[glyphIndex], numberOfGlyphs*sizeof(GlyphInfo) );
}

const GlyphInfo& VisualModel::GetGlyphInfo( GlyphIndex glyphIndex ) const
{
  return GLYPH_INFO;
}

CharacterIndex VisualModel::GetCharacterIndex( GlyphIndex glyphIndex ) const
{
  return mImpl->mGlyphsToCharacters[glyphIndex];
}

Length VisualModel::GetCharactersPerGlyph( GlyphIndex glyphIndex ) const
{
  return mImpl->mCharactersPerGlyph[glyphIndex];
}

GlyphIndex VisualModel::GetGlyphIndex( CharacterIndex characterIndex ) const
{
  GlyphIndex index( 0 );

  for( unsigned int i=0; i<mImpl->mGlyphsToCharacters.Count(); ++i )
  {
    if( mImpl->mGlyphsToCharacters[i] == characterIndex )
    {
      index = i;
      break;
    }
  }

  return index;
}

void VisualModel::GetCharacterToGlyphMap( GlyphIndex* characterToGlyphMap,
                                          CharacterIndex characterIndex,
                                          Length numberOfCharacters ) const
{
}

void VisualModel::GetCharactersPerGlyphMap( Length* charactersPerGlyph,
                                            GlyphIndex glyphIndex,
                                            Length numberOfGlyphs ) const
{
}

void VisualModel::GetGlyphToCharacterMap( CharacterIndex* glyphToCharacter,
                                          GlyphIndex glyphIndex,
                                          Length numberOfGlyphs ) const
{
}

void VisualModel::SetGlyphPositions( const Vector2* glyphPositions,
                                     Length numberOfGlyphs )
{
  std::vector<Vector2>& modelPositions = mImpl->mGlyphPositions;
  modelPositions.resize( numberOfGlyphs );
  memcpy( &modelPositions[0], glyphPositions, numberOfGlyphs*sizeof(Vector2) );
}

void VisualModel::GetGlyphPositions( GlyphIndex glyphIndex,
                                     Vector2* glyphPositions,
                                     Length numberOfGlyphs ) const
{
  std::vector<Vector2>& modelPositions = mImpl->mGlyphPositions;
  memcpy( glyphPositions, &modelPositions[0], numberOfGlyphs*sizeof(Vector2) );
}

const Vector2& VisualModel::GetGlyphPosition( GlyphIndex glyphIndex ) const
{
  return Vector2::ZERO;
}

void VisualModel::SetLines( const LineRun* const lines,
                            Length numberOfLines )
{
}

Length VisualModel::GetNumberOfLines() const
{
  return 0u;
}

void VisualModel::GetLines( LineRun* lines,
                            LineIndex lineIndex,
                            Length numberOfLines ) const
{
}

Length VisualModel::GetNumberOfLines( GlyphIndex glyphIndex,
                                      Length numberOfGlyphs ) const
{
  return 0u;
}

void VisualModel::GetLinesOfGlyphRange( LineRun* lines,
                                        GlyphIndex glyphIndex,
                                        Length numberOfGlyphs ) const
{
}

void VisualModel::SetNaturalSize( const Vector2& size  )
{
}

const Vector2& VisualModel::GetNaturalSize() const
{
  return Vector2::ZERO;
}

void VisualModel::SetActualSize( const Vector2& size )
{
}

const Vector2& VisualModel::GetActualSize() const
{
  return Vector2::ZERO;
}

VisualModel::~VisualModel()
{
  delete mImpl;
}

VisualModel::VisualModel()
: mImpl( NULL )
{
  mImpl = new VisualModel::Impl();
}

} // namespace Text

} // namespace Toolkit

} // namespace Dali
