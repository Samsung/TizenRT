/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/**
 * @defgroup ASSET Asset
 * @ingroup ARAUI
 * @{
 */

#ifndef __UI_ASSET_H__
#define __UI_ASSET_H__

#include <stdint.h>
#include <sys/types.h>
#include <araui/ui_commons.h>

/**
 * @brief Handle type for every asset which used in the UI Framework.
 *
 * @see ui_image_asset_create_from_buffer()
 * @see ui_image_asset_destroy()
 * @see ui_font_asset_create()
 * @see ui_font_asset_destroy()
 */
typedef long ui_asset_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create Image Asset from the information at the Buffer.
 *
 * The image created using this function must be destroyed via the @ref ui_image_asset_destroy().
 *
 * @param[in] buf Pointer address of the buffer containing the image information
 * @return On success, the image asset handle is returned. On failure, UI_NULL is returned.
 *
 * @see ui_image_asset_destroy()
 * @see ui_color_depth_t
 */
ui_asset_t ui_image_asset_create_from_buffer(const uint8_t *buf);

/**
 * @brief Destroy the generated image asset and free the allocated memory.
 *
 * @param[in] image Handle of the image asset
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_image_asset_create_from_buffer()
 * @see ui_error_t
 */
ui_error_t ui_image_asset_destroy(ui_asset_t image);

/**
 * @brief Create Image Asset from the information at the File.
 *
 * The image created using this function must be destroyed via the @ref ui_image_asset_destroy().
 *
 * @param[in] filename Filepath and filename of the image file.
 * @return On success, the image asset handle is returned. On failure, UI_NULL is returned.
 *
 * @see ui_image_asset_destroy()
 */
ui_asset_t ui_image_asset_create_from_file(const char *filename);

/**
 * @brief Measures the width of image (in pixels).
 *
 * @param[in] font Handle of the image asset
 * @return On success, the width of image is returned. On failure, 0 is returned.
 */
size_t ui_image_asset_get_width(ui_asset_t image);

/**
 * @brief Measures the height of image (in pixels).
 *
 * @param[in] font Handle of the image asset
 * @return On success, the height of image is returned. On failure, 0 is returned.
 */
size_t ui_image_asset_get_height(ui_asset_t image);

/**
 * @brief Create Font Asset from the information at the File.
 *
 * The font created using this function must be destroyed via the @ref ui_font_asset_destroy().
 *
 * @param[in] filename Filepath and filename of the image file.
 * @return On success, the font asset handle is returned. On failure, UI_NULL is returned.
 *
 * @see ui_font_asset_destroy()
 */
ui_asset_t ui_font_asset_create_from_file(const char *filename);

/**
 * @brief Create Font Asset from the information at the buffer.
 *
 * The font created using this function must be destroyed via the @ref ui_font_asset_destroy().
 *
 * @param[in] buf Pointer address of the buffer containing the font information
 * @return On success, the font asset handle is returned. On failure, UI_NULL is returned.
 *
 * @see ui_font_asset_destroy()
 */
ui_asset_t ui_font_asset_create_from_buffer(const uint8_t *buf);

/**
 * @brief Destroy the generated font asset and free the allocated memory.
 *
 * @param[in] font Handle of the font asset
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_font_asset_create()
 * @see ui_error_t
 */
ui_error_t ui_font_asset_destroy(ui_asset_t font);

#ifdef __cplusplus
}
#endif

#endif

/** @} */ // end of ASSET group
