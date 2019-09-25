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
#include <tinyara/config.h>
#include <stdint.h>
#include <araui/ui_asset.h>
#include "ui_asset_internal.h"
#include "utils/emoji.h"

#if defined(CONFIG_UI_USE_BUILTIN_EMOJI)
#include "utils/emoji_assets.h"
#endif

#define UTF8_EMOJI_START (0x1f600)
#define UTF8_EMOJI_END (0x1f644)
#define UTF8_MAX_EMOJI_COUNT (UTF8_EMOJI_END + UTF8_EMOJI_START + 1)

#if defined(CONFIG_UI_USE_BUILTIN_EMOJI)
static const uint8_t *g_emoji_bitmap[UTF8_MAX_EMOJI_COUNT] = {
	__emoji_u1F600, __emoji_u1F601, __emoji_u1F602, __emoji_u1F603, __emoji_u1F604,
	__emoji_u1F605, __emoji_u1F606, __emoji_u1F607, __emoji_u1F608, __emoji_u1F609,
	__emoji_u1F60A, __emoji_u1F60B, __emoji_u1F60C, __emoji_u1F60D, __emoji_u1F60E,
	__emoji_u1F60F, __emoji_u1F610, __emoji_u1F611, __emoji_u1F612, __emoji_u1F613,
	__emoji_u1F614, __emoji_u1F615, __emoji_u1F616, __emoji_u1F617, __emoji_u1F618,
	__emoji_u1F619, __emoji_u1F61A, __emoji_u1F61B, __emoji_u1F61C, __emoji_u1F61D,
	__emoji_u1F61E, __emoji_u1F61F, __emoji_u1F620, __emoji_u1F621, __emoji_u1F622,
	__emoji_u1F623, __emoji_u1F624, __emoji_u1F625, __emoji_u1F626, __emoji_u1F627,
	__emoji_u1F628, __emoji_u1F629, __emoji_u1F62A, __emoji_u1F62B, __emoji_u1F62C,
	__emoji_u1F62D, __emoji_u1F62E, __emoji_u1F62F, __emoji_u1F630, __emoji_u1F631,
	__emoji_u1F632, __emoji_u1F633, __emoji_u1F634, __emoji_u1F635, __emoji_u1F636,
	__emoji_u1F637, __emoji_u1F638, __emoji_u1F639, __emoji_u1F63A, __emoji_u1F63B,
	__emoji_u1F63C, __emoji_u1F63D, __emoji_u1F63E, __emoji_u1F63F, __emoji_u1F640,
	__emoji_u1F641, __emoji_u1F642, __emoji_u1F643, __emoji_u1F644
};
#else
static const uint8_t *g_emoji_bitmap[UTF8_MAX_EMOJI_COUNT] = { NULL, };
#endif

ui_bitmap_data_t *emoji_get_bitmap(uint32_t utf_code)
{
	if (is_emoji(utf_code)) {
		return (ui_bitmap_data_t *)(g_emoji_bitmap[utf_code - UTF8_EMOJI_START]);
	}

	return NULL;
}

bool is_emoji(uint32_t utf_code)
{
	return (utf_code >= UTF8_EMOJI_START && utf_code <= UTF8_EMOJI_END);
}
