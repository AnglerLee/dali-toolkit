#ifndef __DALI_TOOLKIT_ATLAS_GLYPH_MANAGER_H__
#define __DALI_TOOLKIT_ATLAS_GLYPH_MANAGER_H__

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
#include <dali-toolkit/internal/atlas-manager/atlas-manager.h>
#include <dali-toolkit/internal/text/text-definitions.h>

namespace Dali
{

namespace Toolkit
{

namespace Internal DALI_INTERNAL
{
class AtlasGlyphManager;
}


class DALI_IMPORT_API AtlasGlyphManager : public BaseHandle
{
public:

  /**
   * @brief Create a AtlasGlyphManager handle.
   *
   * Calling member functions with an uninitialised handle is not allowed.
   */
  AtlasGlyphManager();

  /**
   * @brief Destructor
   *
   * This is non-virtual since derived Handle types must not contain data or virtual methods.
   */
  ~AtlasGlyphManager();

  /**
   * @brief Create or retrieve AtlasGlyphManager singleton.
   *
   * @return A handle to the AtlasGlyphManager control.
   */
  static AtlasGlyphManager Get();

  /**
   * @brief Ask Atlas Manager to add a glyph
   *
   * @param[in] glyph glyph to add to an atlas
   * @param[in] bitmap bitmap to use for glyph addition
   * @param[out] slot information returned by atlas manager for addition
   */
  void Add( const Text::GlyphInfo& glyph,
            const BufferImage& bitmap,
            AtlasManager::AtlasSlot& slot );

  /**
   * @brief Generate mesh data for an image contained in an atlas
   *
   * @param[in] imageId ID of image to generate geometry for
   * @param[in] position top left of image
   * @param[out] meshData generated MeshData
   */
  void GenerateMeshData( uint32_t imageId,
                         const Vector2& position,
                         MeshData& meshData );

  /**
   * @brief Stitch Two Meshes together
   *
   * @param[in] first first mesh
   * @param[in] second second mesh
   */
  void StitchMesh( MeshData& first,
                   const MeshData& second );

  /**
   * @brief Check to see if a glyph is being cached
   *
   * @param[in] fontId The font that this glyph comes from
   * @param[in] index The GlyphIndex of this glyph
   * @param[out] slot container holding information about the glyph( mImage = 0 indicates not being cached )
   */
  void Cached( Text::FontId fontId,
               Text::GlyphIndex index,
               AtlasManager::AtlasSlot& slot );

  /**
   * @brief Set the Atlas size and block size for subsequent atlas generation
   *
   * @param[in] size size of the atlas in pixels
   * @param[in] blockSize size of a block in this atlas in pixels
   */
  void SetAtlasSize( const Vector2& size,
                     const Vector2& blockSize );

  /**
   * @brief Unreference an image from the atlas and remove from cache if no longer needed
   *
   * @param[in] imageId ID of the image
   */
  void Remove( uint32_t imageId );

  /**
   * @brief Get the Pixel Format used by an atlas
   *
   * @param atlasId Id of atlas to check
   * @return The pixel format of the atlas
   */
  Pixel::Format GetPixelFormat( uint32_t atlasId );

private:

  explicit DALI_INTERNAL AtlasGlyphManager(Internal::AtlasGlyphManager *impl);

};

} // namespace Toolkit

} // namespace Dali

#endif // __DALI_TOOLKIT_ATLAS_GLYPH_MANAGER_H__
