#ifndef __DALI_TOOLKIT_TEXT_CONTROLLER_H__
#define __DALI_TOOLKIT_TEXT_CONTROLLER_H__

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

// INTERNAL INCLUDES
#include <dali-toolkit/public-api/text/text-view.h>

// EXTERNAL INCLUDES
#include <dali/public-api/common/intrusive-ptr.h>
#include <dali/public-api/math/vector3.h>
#include <dali/public-api/math/vector2.h>
#include <dali/public-api/object/ref-object.h>
#include <string>

namespace Dali
{

namespace Toolkit
{

namespace Text
{

class Controller;
class LayoutEngine;

typedef IntrusivePtr<Controller> ControllerPtr;

/**
 * @brief A Text Controller is used by UI Controls which display text.
 *
 * It manipulates the Logical & Visual text models on behalf of the UI Controls.
 * It provides a view of the text that can be used by rendering back-ends.
 */
class Controller : public RefObject
{
private:

  /**
   * @brief Text related operations to be done in the relayout process.
   */
  enum OperationsMask
  {
    NO_OPERATION      = 0x0,
    CONVERT_TO_UTF32  = 0x1,
    GET_SCRIPTS       = 0x2,
    VALIDATE_FONTS    = 0x4,
    GET_LINE_BREAKS   = 0x8,
    GET_WORD_BREAKS   = 0x10,
    SHAPE_TEXT        = 0x20,
    GET_GLYPH_METRICS = 0x40,
    LAYOUT            = 0x80,
    REORDER           = 0x100,
    ALIGNEMENT        = 0x200,
    RENDER            = 0x400,
    ALL_OPERATIONS    = 0xFFF
  };

public:

  /**
   * @brief Create a new instance of a Controller.
   *
   * @return A pointer to a new Controller.
   */
  static ControllerPtr New();

  /**
   * @brief Replaces any text previously set.
   *
   * @note This will be converted into UTF-32 when stored in the text model.
   * @param[in] text A string of UTF-8 characters.
   */
  void SetText( const std::string& text );

  /**
   * @brief Triggers a relayout which updates View (if necessary).
   *
   * @note UI Controls are expected to minimize calls to this method e.g. call once after size negotiation.
   * @param[in] size A the size of a bounding box to layout text within.
   * @return True if the View was updated.
   */
  bool Relayout( const Vector2& size );

  /**
   *
   */
  bool DoRelayout( const Vector2& size, OperationsMask operations );

  /**
   * @copydoc Control::GetNaturalSize()
   */
  Vector3 GetNaturalSize();

  /**
   * @copydoc Control::GetHeightForWidth()
   */
  float GetHeightForWidth( float width );

  /**
   * @brief Return the layout engine.
   *
   * @return A reference to the layout engine.
   */
  LayoutEngine& GetLayoutEngine();

  /**
   * @brief Return a view of the text.
   *
   * @return A reference to the view.
   */
  View& GetView();

protected:

  /**
   * @brief A reference counted object may only be deleted by calling Unreference().
   */
  virtual ~Controller();

private:

  /**
   * @brief Private constructor.
   */
  Controller();

  /**
   * @brief Populates the visual model.
   */
  void UpdateVisualModel();

  // Undefined
  Controller( const Controller& handle );

  // Undefined
  Controller& operator=( const Controller& handle );

private:

  struct Impl;
  Impl* mImpl;

  Size mControlSize;
};

} // namespace Text

} // namespace Toolkit

} // namespace Dali

#endif // __DALI_TOOLKIT_TEXT_CONTROLLER_H__
