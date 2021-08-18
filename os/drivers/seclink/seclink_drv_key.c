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

#include <stdio.h>
#include <errno.h>
#include <tinyara/seclink.h>
#include <tinyara/seclink_drv.h>
#include "seclink_drv_req.h"
#include "seclink_drv_utils.h"

#ifdef SLDRV_TAG
#undef SLDRV_TAG
#endif
#define SLDRV_TAG "[SECLINK_DRV_KEY]"

#define NKEY_SLOT 64
/*
 * +-----------------+------------------+
 * |    ROM (0~31)   | RAM(32~63)       |
 * +-----------------+------------------+
 */
#define KEY_ROM_ENTRY 0
#define KEY_RAM_ENTRY 32
static uint8_t g_key_inuse[NKEY_SLOT] = {0,};
#define KEYSLOT_USE 0
#define KEYSLOT_NOTUSE 1
#define KEYSLOT_INVALID_RANGE -1
/*
 * Description: Check that num is in rom area
 */
static inline int _is_rom_area(uint8_t num)
{
	if (num < KEY_RAM_ENTRY) {
		return 1;
	}
	return 0;
}

/*
 * Description: Set the slot number 'num' key slot is in use.
 */
static inline int _set_key_slot(uint8_t num)
{
	if (num >= NKEY_SLOT) {
		return -1;
	}
	g_key_inuse[num] = KEYSLOT_USE;
	return 0;
}

/*
 * Description: Unset the key slot of the slot number 'num'
 */
static inline int _unset_key_slot(uint8_t num)
{
	if (num >= NKEY_SLOT) {
		return -1;
	}
	g_key_inuse[num] = KEYSLOT_NOTUSE;
	return 0;
}

/*
 * Description: Check if slot number 'num' is in use.
 */
static inline int _get_key_slot_state(uint8_t num)
{
	if (num >= NKEY_SLOT) {
		return KEYSLOT_INVALID_RANGE;
	}

	return g_key_inuse[num];
}

/*
 * Description: handle key which is in injected(rom) area
 */
static int hd_handle_rom_key(int cmd, struct seclink_req *req, struct seclink_key_info *info, struct sec_lowerhalf_s *se)
{
	int res = 0;
	if (cmd != SECLINKIOC_GETKEY) {
		req->res = HAL_INVALID_REQUEST;
		return 0;
	}
	SLDRV_CALL(res, req->res, get_key, (info->mode, info->key_idx, info->key));
	return res;
}

/*
 * Description: handle key which is in RAM area
 */
static int hd_handle_ram_key(int cmd, struct seclink_req *req, struct seclink_key_info *info, struct sec_lowerhalf_s *se)
{
	int res = 0;
	int key_state = _get_key_slot_state(info->key_idx);
	SLDRV_LOG("keymgr request cmd(%x) (%d) key(%d) isempty(%d)\n", cmd, info->mode, info->key_idx, key_state);
	if (key_state == KEYSLOT_INVALID_RANGE) {
		req->res = HAL_INVALID_SLOT_RANGE;
		return 0;
	}

	switch (cmd) {
	case SECLINKIOC_SETKEY:
		if (key_state == KEYSLOT_USE) {
			req->res = HAL_KEY_IN_USE;
		} else {
			if (_set_key_slot(info->key_idx) == -1) {
				req->res = HAL_INVALID_SLOT_RANGE;
			} else {
				SLDRV_CALL(res, req->res, set_key, (info->mode, info->key_idx, info->key, info->prikey));
				if (req->res != HAL_SUCCESS) {
					_unset_key_slot(info->key_idx);
				}
			}
		}
		break;
	case SECLINKIOC_GETKEY:
		if (key_state == KEYSLOT_USE) {
			SLDRV_CALL(res, req->res, get_key, (info->mode, info->key_idx, info->key));
		} else {
			req->res = HAL_EMPTY_SLOT;
		}
		break;
	case SECLINKIOC_REMOVEKEY:
		if (key_state == KEYSLOT_USE) {
			_unset_key_slot(info->key_idx);
			SLDRV_CALL(res, req->res, remove_key, (info->mode, info->key_idx));
		} else {
			req->res = HAL_EMPTY_SLOT;
		}
		break;
	case SECLINKIOC_GENERATEKEY:
		if (key_state == KEYSLOT_USE) {
			req->res = HAL_KEY_IN_USE;
		} else {
			SLDRV_CALL(res, req->res, generate_key, (info->mode, info->key_idx));
			if (req->res == HAL_SUCCESS) {
				_set_key_slot(info->key_idx);
			}
		}
		break;
	}
	return 0;
}

/*
 * Description: initialize key slot
 */
void hd_initialize_key_slot(void)
{
	for (int i = 0; i < NKEY_SLOT; i++) {
		g_key_inuse[i] = KEYSLOT_NOTUSE;
	}
}

/*
 * Description: handle key request from Application layer.
 */
int hd_handle_key_request(int cmd, unsigned long arg, void *lower)
{
	SLDRV_ENTER;
	int res = 0;

	struct seclink_req *req = (struct seclink_req *)arg;
	if (!req) {
		return -EINVAL;
	}

	struct seclink_key_info *info = (struct seclink_key_info *)req->req_type.key;
	if (!info) {
		return -EINVAL;
	}

	struct sec_lowerhalf_s *se = (struct sec_lowerhalf_s *)lower;
	if (!se || !(se->ops)) {
		return -EINVAL;
	}

	if (info->key_idx > NKEY_SLOT) {
		req->res = HAL_INVALID_SLOT_RANGE;
		return 0;
	}

	if (_is_rom_area(info->key_idx)) {
		res = hd_handle_rom_key(cmd, req, info, se);
	} else {
		res = hd_handle_ram_key(cmd, req, info, se);
	}

	return res;
}
