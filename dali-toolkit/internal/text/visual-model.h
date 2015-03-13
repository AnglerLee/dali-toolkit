#ifndef __DALI_TOOLKIT_TEXT_VISUAL_MODEL_H__
#define __DALI_TOOLKIT_TEXT_VISUAL_MODEL_H__

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

// EXTERNAL INCLUDES
#include <dali/public-api/common/intrusive-ptr.h>
#include <dali/public-api/object/ref-object.h>

// INTERNAL INCLUDES
#include <dali-toolkit/internal/text/text-definitions.h>

namespace Dali
{

struct Vector2;

namespace Toolkit
{

namespace Text
{

struct LineRun;
class VisualModel;
typedef IntrusivePtr<VisualModel> VisualModelPtr;

/**
 * @brief A visual text model contains layout specific information.
 *
 * This includes:
 * - A series of glyphs in visual order i.e. after the bidirectional reordering.
 * - The position of each glyph within a 2D bounding box.
 */
class VisualModel : public RefObject
{
public:

  /**
   * @brief Create a new instance of a VisualModel.
   *
   * @return A pointer to a new VisualModel.
   */
  static VisualModelPtr New();

  // Glyph interface.

  /**
   * @brief Replaces any glyphs previously set.
   *
   * @note If the number of glyphs is zero, all buffers are cleared.
   * @note If one pointer is NULL and the number of glyphs is not zero, the buffer is not touched.
   *
   * @param[in] glyphs An array of glyphs in the visual order.
   * @param[in] characterIndices An array containing the first character in the logical model that each glyph relates to.
   * @param[in] charactersPerGlyph An array containing the number of characters per glyph.
   * @param[in] numberOfGlyphs The number of glyphs.
   */
  void SetGlyphs( const GlyphInfo* glyphs,
                  const CharacterIndex* characterIndices,
                  const Length* charactersPerGlyph,
                  Length numberOfGlyphs );

  /**
   * Retrieves the number of glyphs.
   *
   * @return The number of glyphs.
   */
  Length GetNumberOfGlyphs() const;

  /**
   * @brief Retrieves glyphs in the given buffer.
   *
   * The size of the @p glyphs buffer needs to be big enough to copy the @p numberOfGlyphs.
   * @param[out] glyphs Pointer to a buffer where the glyphs are copied.
   * @param[in] glyphIndex Index to the first glyph.
   * @param[in] numberOfGlyphs Number of glyphs to be copied.
   */
  void GetGlyphs( GlyphInfo* glyphs,
                  GlyphIndex glyphIndex,
                  Length numberOfGlyphs ) const;

  /**
   * Retrieves a glyph.
   *
   * @param[in] glyphIndex Index to a glyph.
   *
   * @return A glyph.
   */
  const GlyphInfo& GetGlyphInfo( GlyphIndex glyphIndex ) const;

  /**
   * Replaces glyphs.
   *
   * If the @p numberOfGlyphsToRemove is zero, this operation is like an insert.
   * If the @p numberOfGlyphsToInsert is zero, this operation is like a remove.
   *
   * @param[in] glyphIndex Where to replace the glyphs.
   * @param[in] numberOfGlyphsToRemove The number of glyphs to be removed.
   * @param[in] glyphs Pointer to a buffer with the new glyphs.
   * @param[in] numberOfCharacters Pointer to a buffer with the number of characters per glyph.
   * @param[in] numberOfGlyphsToInsert The number of new glyphs in the buffer.
   */
  void ReplaceGlyphs( GlyphIndex glyphIndex,
                      Length numberOfGlyphsToRemove,
                      const GlyphInfo* const glyphs,
                      const Length* const numberOfCharacters,
                      Length numberOfGlyphsToInsert );

  // Character <--> Glyph conversion

  /**
   * @brief Retrieves the first character in the logical model which a glyph represents.
   *
   * @note After shaping several characters may be represented by the same glyph.
   * Alternatively several glyphs may be required to display a character.
   * @param[in] glyphIndex The glyph index.
   * @return The character index.
   */
  CharacterIndex GetCharacterIndex( GlyphIndex glyphIndex ) const;

  /**
   * @brief Query the number of characters the glyph represents.
   *
   * @param[in] glyphIndex The glyph index.
   * @return The number of characters represented by the glyph.
   */
  Length GetCharactersPerGlyph( GlyphIndex glyphIndex ) const;

  /**
   * Retrieves the first glyph in the visual model which represents a given character.
   *
   * @note After shaping several characters may be represented by the same glyph.
   * Alternatively several glyphs may be required to display a character.
   * @param[in] characterIndex The character index.
   * @return The glyph index.
   */
  GlyphIndex GetGlyphIndex( CharacterIndex characterIndex ) const;

  /**
   * Retrieves the whole or part of the character to glyph conversion map.
   *
   * The size of the buffer needs to be big enough to copy the @p numberOfCharacters.
   *
   * @param[out] characterToGlyphMap Pointer to a buffer where the conversion map is copied.
   * @param[in] characterIndex Index to the first character.
   * @param[in] numberOfCharacters The number of characters.
   */
  void GetCharacterToGlyphMap( GlyphIndex* characterToGlyphMap,
                               CharacterIndex characterIndex,
                               Length numberOfCharacters ) const;

  /**
   * Retrieves the whole or part of the glyph to character conversion map.
   *
   * The size of the buffer needs to be big enough to copy the @p numberOfGlyphs.
   *
   * @param[out] glyphToCharacter Pointer to a buffer where the conversion map is copied.
   * @param[in] glyphIndex Index to the first glyph.
   * @param[in] numberOfGlyphs The number of glyphs.
   */
  void GetGlyphToCharacterMap( CharacterIndex* glyphToCharacter,
                               GlyphIndex glyphIndex,
                               Length numberOfGlyphs ) const;

  /**
   * Retrieves for each glyph the number of characters the glyph represents.
   *
   * @param[out] charactersPerGlyph Pointer to a buffer where the number of characters for each glyph are copied.
   * @param[in] glyphIndex Index to the first glyph.
   * @param[in] numberOfGlyphs The number of glyphs.
   */
  void GetCharactersPerGlyphMap( Length* charactersPerGlyph,
                                 GlyphIndex glyphIndex,
                                 Length numberOfGlyphs ) const;

  /**
   * Retrieves for each character the number of glyphs the character is shaped.
   *
   * @param[out] glyphsPerCharacter Pointer to a buffer where the number of glyphs for each character are copied.
   * @param[in] characterIndex Index to the first character.
   * @param[in] numberOfCharacters The number of characters.
   */
  void GetGlyphsPerCharacterMap( Length* glyphsPerCharacter,
                                 CharacterIndex characterIndex,
                                 Length numberOfCharacters ) const;

  // Position interface

  /**
   * @brief Replaces any glyph positions previously set.
   *
   * @note If the number of glyphs is zero the position buffer is cleared.
   *
   * @param[in] glyphPositions An array of visual positions for each glyph.
   * @param[in] numberOfGlyphs The number of positions.
   */
  void SetGlyphPositions( const Vector2* glyphPositions,
                          Length numberOfGlyphs );

  /**
   * Retrieves the number of glyph positions set.
   *
   * @note This may be less than the number of glyphs in the model.
   * @return The number of glyphs.
   */
  Length GetNumberOfGlyphPositions() const;

  /**
   * @brief Retrieves the glyph positions.
   *
   * @pre The size of the @p positions buffer needs to be big enough to copy the @p numberOfGlyphs positions.
   * @param[out] glyphPositions Pointer to a buffer where the glyph positions are copied.
   * @param[in] glyphIndex Index to the first glyph position.
   * @param[in] numberOfGlyphs The number of positions to be copied.
   */
  void GetGlyphPositions( Vector2* glyphPositions,
                          GlyphIndex glyphIndex,
                          Length numberOfGlyphs ) const;

  /**
   * Retrieve the glyph's position of the given glyph.
   *
   * @param[in] glyphIndex Index to the glyph.
   *
   * @return The glyph's position.
   */
  const Vector2& GetGlyphPosition( GlyphIndex glyphIndex ) const;

  /**
   * Replaces glyph's positions.
   *
   * If the @p numberOfGlyphsToRemove is zero, this operation is like an insert.
   * If the @p numberOfGlyphsToInsert is zero, this operation is like a remove.
   *
   * @param[in] glyphIndex Where to replace the glyph's positions.
   * @param[in] numberOfGlyphsToRemove The number of glyph's positions to be removed.
   * @param[in] positions Pointer to a buffer with the new glyph's positions.
   * @param[in] numberOfGlyphsToInsert The number of new glyph's positions in the buffer.
   */
  void ReplaceGlyphPositions( GlyphIndex glyphIndex,
                              Length numberOfGlyphsToRemove,
                              const Vector2* const positions,
                              Length numberOfGlyphsToInsert );

  // Line interface.

  /**
   * Sets the lines.
   *
   * Replaces any lines previously set.
   *
   * Every line is an item run containing the index to the first glyph of the line and the number of glyphs.
   *
   * @note If the number of lines is zero or the pointer is NULL, the lines buffer is cleared.
   *
   * @param[in] lines Pointer to a buffer containing all the line runs.
   * @param[in] numberOfLines The number of lines in the buffer.
   */
  void SetLines( const LineRun* const lines,
                 Length numberOfLines );

  /**
   * Retrieves the number of lines of the whole text.
   *
   * @return The number of lines.
   */
  Length GetNumberOfLines() const;

  /**
   * Retrieves lines.
   *
   * The size of the @p lines buffer needs to be big enough to copy the @p numberOfLines.
   *
   * @param[out] lines Pointer to a buffer where the lines are copied.
   * @param[in] lineIndex Index to the first line.
   * @param[in] numberOfLines Number of lines to be copied.
   */
  void GetLines( LineRun* lines,
                 LineIndex lineIndex,
                 Length numberOfLines ) const;

  /**
   * Retrieves the number of lines where the given range of glyphs is laid out.
   *
   * @param[in] glyphIndex Index to the first glyph.
   * @param[in] numberOfGlyphs The number of glyph.
   *
   * @return The number of lines.
   */
  Length GetNumberOfLines( GlyphIndex glyphIndex,
                           Length numberOfGlyphs ) const;
  /**
   * Retrieves the lines where the given range of glyphs is laid out.
   *
   * The size of the @p lines buffer needs to be big enough to copy the @p numberOfLines.
   *
   * @param[out] lines Pointer to a buffer where the lines are copied.
   * @param[in] glyphIndex Index to the first glyphs of the range.
   * @param[in] numberOfGlyphs Number of glyphs in the range.
   */
  void GetLinesOfGlyphRange( LineRun* lines,
                             GlyphIndex glyphIndex,
                             Length numberOfGlyphs ) const;

  /**
   * Replaces lines for the given range of glyphs.
   *
   * If the @p numberOfGlyphsToRemove is zero, this operation is like an insert.
   * If the @p numberOfGlyphsToInsert is zero, this operation is like a remove.
   *
   * @param[in] glyphIndex Index of the first glyph where to replace the line info.
   * @param[in] numberOfGlyphsToRemove The number of glyphs to be the line info removed.
   * @param[in] lines Pointer to a buffer with the lines.
   * @param[in] numberOfGlyphsToInsert The number of characters to be the line info inserted.
   */
  void ReplaceLines( GlyphIndex glyphIndex,
                     Length numberOfGlyphsToRemove,
                     const LineRun* const lines,
                     Length numberOfGlyphsToInsert );

  // Size interface

  /**
   * Sets the natural size.
   *
   * @param[in] size The text's natural size.
   */
  void SetNaturalSize( const Vector2& size  );

  /**
   * Retrieves the natural size.
   *
   * @return The text's natural size.
   */
  const Vector2& GetNaturalSize() const;

  /**
   * Sets the text's actual size after it has been laid out.
   *
   * @param[in] size The text's size.
   */
  void SetActualSize( const Vector2& size );

  /**
   * Retrieves the text's actual size after it has been laid out.
   *
   * @return The text's size.
   */
  const Vector2& GetActualSize() const;

protected:

  /**
   * @brief A reference counted object may only be deleted by calling Unreference().
   */
  virtual ~VisualModel();

private:

  /**
   * @brief Private constructor.
   */
  VisualModel();

  // Undefined
  VisualModel( const VisualModel& handle );

  // Undefined
  VisualModel& operator=( const VisualModel& handle );

private:

  struct Impl;
  Impl* mImpl;
};
} // namespace Text

} // namespace Toolkit

} // namespace Dali

#endif // __DALI_TOOLKIT_TEXT_VISUAL_MODEL_H__
