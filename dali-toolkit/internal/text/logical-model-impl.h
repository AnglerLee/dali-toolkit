#ifndef __DALI_TOOLKIT_TEXT_LOGICAL_MODEL_IMPL_H__
#define __DALI_TOOLKIT_TEXT_LOGICAL_MODEL_IMPL_H__

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
#include <dali/public-api/common/dali-vector.h>
#include <dali/public-api/common/intrusive-ptr.h>
#include <dali/public-api/object/ref-object.h>

// INTERNAL INCLUDES
#include <dali-toolkit/internal/text/bidirectional-line-info-run.h>
#include <dali-toolkit/internal/text/bidirectional-paragraph-info-run.h>
#include <dali-toolkit/internal/text/font-run.h>
#include <dali-toolkit/internal/text/script-run.h>

namespace Dali
{

namespace Toolkit
{

namespace Text
{

struct BidirectionalLineInfoRun;
struct BidirectionalParagraphInfoRun;
struct FontRun;
class LogicalModel;
typedef IntrusivePtr<LogicalModel> LogicalModelPtr;
struct ScriptRun;

/**
 * @brief A logical text model contains layout independent information.
 *
 * This includes:
 * - A series of UTF-32 characters in logical order
 */
class LogicalModel : public RefObject
{
public:

  /**
   * @brief Create a new instance of a LogicalModel.
   *
   * @return A pointer to a new LogicalModel.
   */
  static LogicalModelPtr New();

  // Text interface.

  /**
   * @brief Replaces any text previously set.
   *
   * @note If the number of characters is zero the text buffer is cleared.
   *
   * @param[in] text An array of UTF-32 characters.
   * @param[in] numberOfCharacters The length of the array.
   */
  void SetText( const Character* const text,
                Length numberOfCharacters );

  /**
   * @brief Retrieves the number of characters of the text.
   *
   * @return The number of characters.
   */
  Length GetNumberOfCharacters() const;

  /**
   * @brief Retrieves characters from the text in the given buffer.
   *
   * @pre The size of the @p text buffer needs to be big enough to copy the @p numberOfCharacters.
   * @param[out] text Pointer to a buffer where the text is copied.
   * @param[in] characterIndex The index to the first character to copy.
   * @param[in] numberOfCharacters The number of characters to be copied.
   */
  void GetText( Character* text,
                CharacterIndex characterIndex,
                Length numberOfCharacters ) const;

  /**
   * @brief Retrieves a character.
   *
   * @param[in] characterIndex Index to a character.
   *
   * @return A character.
   */
  Character GetCharacter( CharacterIndex characterIndex ) const;

  /**
   * @brief Replaces characters from the text.
   *
   * If the @p numberOfCharactersToRemove is zero, this operation is like an insert.
   * If the @p numberOfCharactersToInsert is zero, this operation is like a remove.
   *
   * @param[in] characterIndex Where to replace the text.
   * @param[in] numberOfCharactersToRemove The number of characters to be removed.
   * @param[in] text Pointer to a buffer with the text encoded in utf32.
   * @param[in] numberOfCharactersToInsert The number of characters in the buffer.
   */
  void ReplaceText( CharacterIndex characterIndex,
                    Length numberOfCharactersToRemove,
                    const Character* const text,
                    Length numberOfCharactersToInsert );

  // Language support interface.

  /**
   * @brief Sets the script runs.
   *
   * Replaces any scripts previously set.
   *
   * A run is a group of consecutive characters. A script run contains the script for a run.
   *
   * @note If the number of runs is zero the script buffer is cleared.
   *
   * @param[in] scripts Pointer to a buffer with all the script runs.
   * @param[in] numberOfRuns The number of script runs.
   */
  void SetScripts( const ScriptRun* const scripts,
                   Length numberOfRuns );

  /**
   * @brief Retrieves the number of script runs and the index to the first one for the given range of characters.
   *
   * A run is a group of consecutive characters. A script run contains the script for a run.
   *
   * @param[in] characterIndex Index to the first character.
   * @param[in] numberOfCharacters The number of characters.
   * @param[out] firstScriptRun Index to the script run containing the character index.
   * @param[out] numberOfScriptRuns The number of script runs.
   */
  void GetNumberOfScriptRuns( CharacterIndex characterIndex,
                              Length numberOfCharacters,
                              ScriptRunIndex& firstScriptRun,
                              Length& numberOfScriptRuns ) const;

  /**
   * @brief Retrieves the script runs for the given range of characters.
   *
   * The @p scriptRuns buffer needs to be big enough to copy the number of script runs.
   * Call GetNumberOfScriptRuns() to retrieve the number of script runs.
   *
   * @param[out] scriptRuns Pointer to a buffer where the script runs are copied.
   * @param[in] characterIndex Index to the first character.
   * @param[in] numberOfCharacters The number of characters.
   */
  void GetScriptRuns( ScriptRun* scriptRuns,
                      CharacterIndex characterIndex,
                      Length numberOfCharacters ) const;

  /**
   * @brief Retrieves the script for the given character index.
   *
   * @param[in] characterIndex Index to the character.
   *
   * @return The character's script.
   */
  Script GetScript( CharacterIndex characterIndex ) const;

  /**
   * @brief Replaces script runs for the given range of characters.
   *
   * If the @p numberOfCharactersToRemove is zero, this operation is like an insert.
   * If the @p numberOfCharactersToInsert is zero, this operation is like a remove.
   *
   * @param[in] characterIndex Index of the first character where to replace the scripts.
   * @param[in] numberOfCharactersToRemove The number of characters to be the script removed.
   * @param[in] scriptRuns Pointer to a buffer with the script runs.
   * @param[in] numberOfCharactersToInsert The number of characters to be the script inserted.
   */
  void ReplaceScripts( CharacterIndex characterIndex,
                       Length numberOfCharactersToRemove,
                       const ScriptRun* const scriptRuns,
                       Length numberOfCharactersToInsert );

  /**
   * @brief Sets the font runs.
   *
   * Replaces any fonts previously set.
   *
   * A run is a group of consecutive characters. A font run contains the font id for a run.
   *
   * @note If the number of runs is zero the font buffer is cleared.
   *
   * @param[in] fonts Pointer to a buffer with all the font runs.
   * @param[in] numberOfRuns The number of font runs.
   */
  void SetFonts( const FontRun* const fonts,
                 Length numberOfRuns );

  /**
   * @brief Retrieves the number of font runs and the index of the first one for the given range of characters.
   *
   * A run is a group of consecutive characters. A font run contains the font id for a run.
   *
   * @param[in] characterIndex Index to the first character.
   * @param[in] numberOfCharacters The number of characters.
   * @param[out] firstFontRun Index to the font run containing the character index.
   * @param[out] numberOfFontRuns The number of font runs.
   */
  void GetNumberOfFontRuns( CharacterIndex characterIndex,
                            Length numberOfCharacters,
                            FontRunIndex& firstFontRun,
                            Length& numberOfFontRuns ) const;

  /**
   * @brief Retrieves the font runs for the given range of characters.
   *
   * The @p fontRuns buffer needs to be big enough to copy the number of font runs.
   * Call GetNumberOfFontRuns() to retrieve the number of font runs.
   *
   * @param[out] fontRuns Pointer to a buffer where the font runs are copied.
   * @param[in] characterIndex Index to the first character.
   * @param[in] numberOfCharacters The number of characters.
   */
  void GetFontRuns( FontRun* fontRuns,
                    CharacterIndex characterIndex,
                    Length numberOfCharacters ) const;

  /**
   * @brief Retrieves the font id for the given character index.
   *
   * @param[in] characterIndex Index to the first character.
   *
   * @return The font id.
   */
  FontId GetFont( CharacterIndex characterIndex ) const;

  /**
   * @brief Replaces font runs for the given range of characters.
   *
   * If the @p numberOfCharactersToRemove is zero, this operation is like an insert.
   * If the @p numberOfCharactersToInsert is zero, this operation is like a remove.
   *
   * @param[in] characterIndex Index of the first character where to replace the fonts.
   * @param[in] numberOfCharactersToRemove The number of characters to be the font removed.
   * @param[in] fontRuns Pointer to a buffer with the font runs.
   * @param[in] numberOfCharactersToInsert The number of characters to be the font inserted.
   */
  void ReplaceFonts( CharacterIndex characterIndex,
                     Length numberOfCharactersToRemove,
                     const FontRun* const fontRuns,
                     Length numberOfCharactersToInsert );

  // Break info interface.

  /**
   * @brief Sets the line break info.
   *
   * See GetLineBreakInfo() to get how the line break info is encoded.
   *
   * Replaces any line break info previously set.
   *
   * @note If the @length is zero the break info buffer is cleared.
   *
   * @param[in] lineBreakInfo Pointer to a buffer with the line break info.
   * @param[in] length The size of the buffer.
   */
  void SetLineBreakInfo( const LineBreakInfo* const lineBreakInfo,
                         Length length );

  /**
   * @brief Retrieves the line break info in the given buffer.
   *
   * The size of the @p lineBreakInfo buffer needs to be big enough to copy the @p numberOfItems.
   *
   * Possible values for LineBreakInfo are:
   *
   *  - 0 is a LINE_MUST_BREAK.  Text must be broken into a new line.
   *  - 1 is a LINE_ALLOW_BREAK. Is possible to break the text into a new line.
   *  - 2 is a LINE_NO_BREAK.    Text can't be broken into a new line.
   *
     @verbatim
     i.e. Hello big\nworld produces:
          2222212220 22220
     @endverbatim
   *
   * @param[out] lineBreakInfo Pointer to a buffer where the line break info is copied.
   * @param[in] characterIndex Index to the first line break info item.
   * @param[in] numberOfItems The number of items to be copied.
   */
  void GetLineBreakInfo( LineBreakInfo* lineBreakInfo,
                         CharacterIndex characterIndex,
                         Length numberOfItems ) const;

  /**
   * @brief Retrieves the line break info for the given item index.
   *
   * @param[in] characterIndex Index to the line break info item.
   */
  LineBreakInfo GetLineBreakInfo( CharacterIndex characterIndex ) const;

  /**
   * @brief Replaces line break info.
   *
   * See GetLineBreakInfo() to get how the line break info is encoded.
   *
   * If the @p numberOfItemsToRemove is zero, this operation is like an insert.
   * If the @p numberOfItemsToInsert is zero, this operation is like a remove.
   *
   * @param[in] characterIndex Where to replace the line break info.
   * @param[in] numberOfItemsToRemove The number of items to be removed.
   * @param[in] lineBreakInfo Pointer to a buffer with the line break info.
   * @param[in] numberOfItemsToInsert The number of items in the buffer.
   */
  void ReplaceLineBreakInfo( CharacterIndex characterIndex,
                             Length numberOfItemsToRemove,
                             const LineBreakInfo* const lineBreakInfo,
                             Length numberOfItemsToInsert );

  /**
   * @brief Sets the word break info.
   *
   * See GetWordBreakInfo() to get how the word break info is encoded.
   *
   * Replaces any word break info previously set.
   *
   * @note If the @length is zero the break info buffer is cleared.
   *
   * @param[in] wordBreakInfo Pointer to a buffer with the word break info.
   * @param[in] length The size of the buffer.
   */
  void SetWordBreakInfo( const WordBreakInfo* const wordBreakInfo,
                         Length length );

  /**
   * @brief Retrieves the word break info in the given buffer.
   *
   * The size of the @p wordBreakInfo buffer needs to be big enough to copy the @p numberOfItems.
   *
   * The size of the buffer has to be big enough to store the whole word break info per character.
   * Call GetNumberOfCharacters() to get the number of characters.
   *
   * Possible values for WordBreakInfo are:
   *
   * - 0 is a WORD_BREAK.    Text can be broken into a new word.
   * - 1 is a WORD_NO_BREAK. Text can't be broken into a new word.
   *
     @verbatim
     i.e. Hello big\nworld produces:
          1111001100 11110
     @endverbatim
   *
   * @param[out] wordBreakInfo Pointer to a buffer where the word break info is copied.
   * @param[in] characterIndex Index to the first word break info item.
   * @param[in] numberOfItems The number of items to be copied.
   */
  void GetWordBreakInfo( WordBreakInfo* wordBreakInfo,
                         CharacterIndex characterIndex,
                         Length numberOfItems ) const;

  /**
   * @brief Retrieves the word break info for the given item index.
   *
   * @param[in] characterIndex Index to the word break info item.
   */
  WordBreakInfo GetWordBreakInfo( CharacterIndex characterIndex ) const;

  /**
   * @brief Replaces word break info.
   *
   * See GetWordBreakInfo() to get how the word break info is encoded.
   *
   * If the @p numberOfItemsToRemove is zero, this operation is like an insert.
   * If the @p numberOfItemsToInsert is zero, this operation is like a remove.
   *
   * @param[in] characterIndex Where to replace the word break info.
   * @param[in] numberOfItemsToRemove The number of items to be removed.
   * @param[in] wordBreakInfo Pointer to a buffer with the word break info.
   * @param[in] numberOfItemsToInsert The number of items in the buffer.
   */
  void ReplaceWordBreakInfo( CharacterIndex characterIndex,
                             Length numberOfItemsToRemove,
                             const WordBreakInfo* const wordBreakInfo,
                             Length numberOfItemsToInsert );

  // Bidirectional support interface.

  /**
   * @brief Sets the bidirectional info runs.
   *
   * Replaces any bidirectional info previously set.
   *
   * Each bidirectional info run stores bidirectional info for a whole 'paragraph' of text which contains right to left scripts.

   * In terms of the bidirectional algorithm, a 'paragraph' is understood as a run of characters between Paragraph Separators or appropriate Newline Functions.
   * A 'paragraph' may also be determined by higher-level protocols like a mark-up tag.
   *
   * @note If the number of runs is zero the bidirectional info buffer is cleared.
   *
   * @param[in] bidirectionalInfo Pointer to a buffer with all the bidirectional info runs.
   * @param[in] numberOfRuns The number of bidirectional info runs.
   */
  void SetBidirectionalInfo( const BidirectionalParagraphInfoRun* const bidirectionalInfo,
                             Length numberOfRuns );

  /**
   * @brief Retrieves the number of bidirectional info runs and the index to the first one for the given range of characters.
   *
   * It may be zero if there is no right to left scripts.
   *
   * @param[in] characterIndex Index to the first character.
   * @param[in] numberOfCharacters The number of characters.
   *
   * @return The number of bidirectional info runs.
   */
  void GetNumberOfBidirectionalInfoRuns( CharacterIndex characterIndex,
                                         Length numberOfCharacters,
                                         BidirectionalRunIndex& firstBidirectionalRun,
                                         Length& numberOfFontRuns ) const;

  /**
   * @brief Retrieves the bidirectional paragraph info runs for the given range of characters.
   *
   * The @p bidirectionalInfo buffer needs to be big enough to copy the number of bidirectional
   * paragraph info runs.
   * Call GetNumberOfBidirectionalInfoRuns() to retrieve the number of bidirectional runs.
   *
   * @param[out] bidirectionalInfo Pointer to a buffer where the bidirectional info runs are copied.
   * @param[in] characterIndex Index to the first character.
   * @param[in] numberOfCharacters The number of characters.
   */
  void GetBidirectionalInfo( BidirectionalParagraphInfoRun* bidirectionalInfo,
                             CharacterIndex characterIndex,
                             Length numberOfCharacters ) const;

  /**
   * @brief Replaces bidirectional info runs for the given range of characters.
   *
   * If the @p numberOfCharactersToRemove is zero, this operation is like an insert.
   * If the @p numberOfCharactersToInsert is zero, this operation is like a remove.
   *
   * @param[in] characterIndex Index of the first character where to replace the bidirectional info.
   * @param[in] numberOfCharactersToRemove The number of characters to be the bidirectional info removed.
   * @param[in] bidirectionalInfo Pointer to a buffer with the bidirectional info runs.
   * @param[in] numberOfCharactersToInsert The number of characters to be the bidirectional info inserted.
   */
  void ReplaceBidirectionalInfo( CharacterIndex characterIndex,
                                 Length numberOfCharactersToRemove,
                                 const BidirectionalParagraphInfoRun* const bidirectionalInfo,
                                 Length numberOfCharactersToInsert );

  /**
   * @brief Replaces the direction of the characters.
   *
   * @note If the number of characters is zero the directions buffer is cleared.
   *
   * @param[in] directions The directions of the characters.
   * @param[in] numberOfCharacters The number of characters.
   */
  void SetCharacterDirections( const CharacterDirection* const directions,
                               Length numberOfCharacters );

  /**
   * @brief Retrieves the direction of the characters.
   *
   * It sets @e true for right to left characters and @e false for left to right.
   * For neutral characters it check's the next and previous character's directions:
   * - If they are equals set that direction. If they are not, sets the paragraph's direction.
   * - If there is no next, sets the paragraph's direction.
   *
   * See SetBidirectionalInfo() to get an explanation of the 'paragraph' meaning in the bidirectional algorithm.
   *
   * @pre the @p directions vector should be initialized to @e false (left to right) as this method is not going
   *      to update it if there is no right to left characters.
   *
   * @param[out] directions Whether the characters are right to left or left to right.
   * @param[in] characterIndex Index to the first character.
   * @param[in] numberOfCharacters The number of characters.
   */
  void GetCharacterDirections( CharacterDirection* directions,
                               CharacterIndex characterIndex,
                               Length numberOfCharacters ) const;

  /**
   * @brief Retrieves the direction of a characters.
   *
   * See GetCharacterDirections().
   *
   * @param[in] characterIndex Index to a character.
   *
   * @return The character's direction.
   */
  CharacterDirection GetCharacterDirection( CharacterIndex characterIndex ) const;

  // Visual <--> Logical conversion tables.

  /**
   * @brief Sets the visual to logical and the logical to visual map tables.
   *
   * Replaces any map tables previously set.
   *
   * @note If the number of runs is zero the bidirectional info buffer is cleared.
   *
   * @param[in] bidirectionalInfo Pointer to a buffer with all the bidirectional info runs.
   * @param[in] numberOfRuns The number of bidirectional info runs.
   */
  void SetVisualToLogicalMap( const BidirectionalLineInfoRun* const bidirectionalInfo,
                              Length numberOfRuns );

  /**
   * @brief Replaces the visual to logical and logical to visual map tables for the given range of characters.
   *
   * If the @p numberOfCharactersToRemove is zero, this operation is like an insert.
   * If the @p numberOfCharactersToInsert is zero, this operation is like a remove.
   *
   * @param[in] characterIndex Index of the first character where to replace the map tables.
   * @param[in] numberOfCharactersToRemove The number of characters to be removed.
   * @param[in] bidirectionalInfo Pointer to a buffer with the bidirectional info runs.
   * @param[in] numberOfCharactersToInsert The number of characters to be inserted.
   */
  void ReplaceVisualToLogicalMap( CharacterIndex characterIndex,
                                  Length numberOfCharactersToRemove,
                                  const BidirectionalLineInfoRun* const bidirectionalInfo,
                                  Length numberOfCharactersToInsert );

  /**
   * @brief Retrieves the visual character index for the given logical character index.
   *
   * @param[in] logicalCharacterIndex The logical character index.
   *
   * @return The visual character index.
   */
  CharacterIndex GetVisualCharacterIndex( CharacterIndex logicalCharacterIndex ) const;

  /**
   * @brief Retrieves the logical character index for the given visual character index.
   *
   * @param[in] visualCharacterIndex The visual character index.
   *
   * @return The logical character index.
   */
  CharacterIndex GetLogicalCharacterIndex( CharacterIndex visualCharacterIndex ) const;

  /**
   * @brief Retrieves the whole or part of the logical to visual conversion map.
   *
   * The size of the buffer needs to be big enough to copy the @p numberOfCharacters.
   *
   * @param[out] logicalToVisualMap Pointer to a buffer where the conversion map is copied.
   * @param[in] characterIndex Index to the first character.
   * @param[in] numberOfCharacters The number of characters.
   */
  void GetLogicalToVisualMap( CharacterIndex* logicalToVisualMap,
                              CharacterIndex characterIndex,
                              Length numberOfCharacters ) const;

  /**
   * @brief Retrieves the whole or part of the visual to logical conversion map.
   *
   * The size of the buffer needs to be big enough to copy the @p numberOfCharacters.
   *
   * @param[out] visualToLogicalMap Pointer to a buffer where the conversion map is copied.
   * @param[in] characterIndex Index to the first character.
   * @param[in] numberOfCharacters The number of characters.
   */
  void GetVisualToLogicalMap( CharacterIndex* visualToLogicalMap,
                              CharacterIndex characterIndex,
                              Length numberOfCharacters ) const;

protected:

  /**
   * @brief A reference counted object may only be deleted by calling Unreference().
   */
  virtual ~LogicalModel();

private:

  /**
   * @brief Private constructor.
   */
  LogicalModel();

  // Undefined
  LogicalModel( const LogicalModel& handle );

  // Undefined
  LogicalModel& operator=( const LogicalModel& handle );

public:

  Vector<Character>                     mText;
  Vector<ScriptRun>                     mScriptRuns;
  Vector<FontRun>                       mFontRuns;
  Vector<LineBreakInfo>                 mLineBreakInfo;
  Vector<WordBreakInfo>                 mWordBreakInfo;
  Vector<BidirectionalParagraphInfoRun> mBidirectionalParagraphInfo;
  Vector<CharacterDirection>            mCharacterDirections;        ///< For each character, whether is right to left. ( @e flase is left to right, @e true right to left ).
  Vector<BidirectionalLineInfoRun>      mBidirectionalLineInfo;
  Vector<CharacterIndex>                mLogicalToVisualMap;         ///< Bidirectional logical to visual conversion table.
  Vector<CharacterIndex>                mVisualToLogicalMap;         ///< Bidirectional visual to logical conversion table.
};

} // namespace Text

} // namespace Toolkit

} // namespace Dali

#endif // __DALI_TOOLKIT_TEXT_LOGICAL_MODEL_IMPL_H__
