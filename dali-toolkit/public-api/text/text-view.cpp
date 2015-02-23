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
#include <dali-toolkit/public-api/text/text-view.h>

// EXTERNAL INCLUDES
#include <dali/public-api/math/vector2.h>

namespace Dali
{

namespace Toolkit
{

namespace Text
{

struct View::Impl
{
  VisualModelPtr mVisualModel;
};

View::View()
: mImpl( NULL )
{
  mImpl = new View::Impl();
}

View::~View()
{
  delete mImpl;
}

void View::SetVisualModel( VisualModelPtr visualModel )
{
  mImpl->mVisualModel = visualModel;
}

Length View::GetNumberOfGlyphs() const
{
  if( mImpl->mVisualModel )
  {
    return mImpl->mVisualModel->GetNumberOfGlyphs();
  }

  return 0;
}

void View::GetGlyphs( GlyphInfo* glyphs,
                      GlyphIndex glyphIndex,
                      Length numberOfGlyphs ) const
{
  if( mImpl->mVisualModel )
  {
    mImpl->mVisualModel->GetGlyphs( glyphs, glyphIndex, numberOfGlyphs );
  }
}

void View::GetGlyphPositions( Vector2* glyphPositions,
                              GlyphIndex glyphIndex,
                              Length numberOfGlyphs ) const
{
  if( mImpl->mVisualModel )
  {
    mImpl->mVisualModel->GetGlyphPositions( glyphPositions, glyphIndex, numberOfGlyphs );
  }
}

} // namespace Text

} // namespace Toolkit

} // namespace Dali
