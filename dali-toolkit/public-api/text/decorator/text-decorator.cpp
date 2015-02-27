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
#include <dali-toolkit/public-api/text/decorator/text-decorator.h>

// EXTERNAL INCLUDES
#include <dali/public-api/common/constants.h>
#include <dali/public-api/images/image.h>
#include <dali/public-api/math/vector2.h>
#include <dali/public-api/math/vector4.h>

namespace Dali
{

namespace Toolkit
{

namespace Text
{

struct Decorator::Impl
{
  struct CursorImpl
  {
    CursorImpl()
    : x(0.0f),
      y(0.0f),
      height(0.0f),
      color(Dali::Color::WHITE)
    {
    }

    float x;
    float y;
    float height;

    Vector4 color;
  };

  Impl(Dali::Toolkit::Internal::Control& parent, Observer& observer)
  : mParent(parent),
    mObserver(observer),
    mActiveCursor(ACTIVE_CURSOR_NONE),
    mCursorBlinkInterval(0.5f),
    mCursorBlinkDuration(0.0f)
  {
  }

  void Relayout( const Vector2& size )
  {
    // TODO
  }

  Internal::Control& mParent;
  Observer& mObserver;

  unsigned int mActiveCursor;

  CursorImpl mCursor[CURSOR_COUNT];

  Image mCursorImage;
  Image mGrabHandleImage;

  float mCursorBlinkInterval;
  float mCursorBlinkDuration;
};

DecoratorPtr Decorator::New( Internal::Control& parent, Observer& observer )
{
  return DecoratorPtr( new Decorator(parent, observer) );
}

void Decorator::Relayout( const Vector2& size )
{
  mImpl->Relayout( size );
}

void Decorator::SetActiveCursor( ActiveCursor activeCursor )
{
  mImpl->mActiveCursor = activeCursor;
}

unsigned int Decorator::GetActiveCursor() const
{
  return mImpl->mActiveCursor;
}

void Decorator::SetPosition( Cursor cursor, float x, float y, float height )
{
  mImpl->mCursor[cursor].x = x;
  mImpl->mCursor[cursor].y = y;
  mImpl->mCursor[cursor].height = height;
}

void Decorator::GetPosition( Cursor cursor, float& x, float& y, float& height ) const
{
  x = mImpl->mCursor[cursor].x;
  y = mImpl->mCursor[cursor].y;
  height = mImpl->mCursor[cursor].height;
}

void Decorator::SetCursorImage( Dali::Image image )
{
  mImpl->mCursorImage = image;
}

Dali::Image Decorator::GetCursorImage() const
{
  return mImpl->mCursorImage;
}

void Decorator::SetColor( Cursor cursor, const Dali::Vector4& color )
{
  mImpl->mCursor[cursor].color = color;
}

const Dali::Vector4& Decorator::GetColor( Cursor cursor ) const
{
  return mImpl->mCursor[cursor].color;
}

void Decorator::StartCursorBlink()
{
  // TODO
}

void Decorator::StopCursorBlink()
{
  // TODO
}

void Decorator::SetCursorBlinkInterval( float seconds )
{
  mImpl->mCursorBlinkInterval = seconds;
}

float Decorator::GetCursorBlinkInterval() const
{
  return mImpl->mCursorBlinkInterval;
}

void Decorator::SetCursorBlinkDuration( float seconds )
{
  mImpl->mCursorBlinkDuration = seconds;
}

float Decorator::GetCursorBlinkDuration() const
{
  return mImpl->mCursorBlinkDuration;
}

void Decorator::SetGrabHandleImage( Dali::Image image )
{
  mImpl->mGrabHandleImage = image;
}

Dali::Image Decorator::GetGrabHandleImage() const
{
  return mImpl->mGrabHandleImage;
}

Decorator::~Decorator()
{
  delete mImpl;
}

Decorator::Decorator(Dali::Toolkit::Internal::Control& parent, Observer& observer)
: mImpl( NULL )
{
  mImpl = new Decorator::Impl(parent, observer);
}

} // namespace Text

} // namespace Toolkit

} // namespace Dali
