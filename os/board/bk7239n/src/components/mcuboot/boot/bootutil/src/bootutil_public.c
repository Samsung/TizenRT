/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2017-2019 Linaro LTD
 * Copyright (c) 2016-2019 JUUL Labs
 * Copyright (c) 2019-2021 Arm Limited
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * Original license:
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/**
 * @file
 * @brief Public MCUBoot interface API implementation
 *
 * This file contains API implementation which can be combined with
 * the application in order to interact with the MCUBoot bootloader.
 * This file contains shared code-base betwen MCUBoot and the application
 * which controls DFU process.
 */

#include <string.h>
#include <inttypes.h>
#include <stddef.h>

#include "sysflash/sysflash.h"
#include "flash_map_backend/flash_map_backend.h"
#include "flash_partition.h"

#include "bootutil/image.h"
#include "bootutil/bootutil_public.h"
#include "bootutil/bootutil_log.h"
#ifdef MCUBOOT_ENC_IMAGES
#include "bootutil/enc_key_public.h"
#endif

#include "bootutil/boot_public_hooks.h"
#include "bootutil_priv.h"
#include "flash_driver.h"

#define BOOT_XIP_FLAG_OFFSET            (4096)
#define BOOT_XIP_MAGIC_OFFSET           (0)
#define BOOT_XIP_COPY_DONE_OFFSET       (32)
#define BOOT_XIP_IMAGE_OK_OFFSET        (64)

extern uint32_t flash_write_bytes_dbus(uint32_t address, const uint8_t *user_buf, uint32_t size);

#ifdef CONFIG_MCUBOOT
BOOT_LOG_MODULE_DECLARE(mcuboot);
#else
BOOT_LOG_MODULE_REGISTER(mcuboot_util);
#endif

#if BOOT_MAX_ALIGN == 8
const union boot_img_magic_t boot_img_magic = {
    .val = {
        0x77, 0xc2, 0x95, 0xf3,
        0x60, 0xd2, 0xef, 0x7f,
        0x35, 0x52, 0x50, 0x0f,
        0x2c, 0xb6, 0x79, 0x80
    }
};
#else
const union boot_img_magic_t boot_img_magic = {
    .align = BOOT_MAX_ALIGN,
    .magic = {
        0x2d, 0xe1,
        0x5d, 0x29, 0x41, 0x0b,
        0x8d, 0x77, 0x67, 0x9c,
        0x11, 0x0f, 0x1f, 0x8a
    }
};
#endif

struct boot_swap_table {
    uint8_t magic_primary_slot;
    uint8_t magic_secondary_slot;
    uint8_t image_ok_primary_slot;
    uint8_t image_ok_secondary_slot;
    uint8_t copy_done_primary_slot;

    uint8_t swap_type;
};

/**
 * This set of tables maps image trailer contents to swap operation type.
 * When searching for a match, these tables must be iterated sequentially.
 *
 * NOTE: the table order is very important. The settings in the secondary
 * slot always are priority to the primary slot and should be located
 * earlier in the table.
 *
 * The table lists only states where there is action needs to be taken by
 * the bootloader, as in starting/finishing a swap operation.
 */
static const struct boot_swap_table boot_swap_tables[] = {
    {
        .magic_primary_slot =       BOOT_MAGIC_ANY,
        .magic_secondary_slot =     BOOT_MAGIC_GOOD,
        .image_ok_primary_slot =    BOOT_FLAG_ANY,
        .image_ok_secondary_slot =  BOOT_FLAG_UNSET,
        .copy_done_primary_slot =   BOOT_FLAG_ANY,
        .swap_type =                BOOT_SWAP_TYPE_TEST,
    },
    {
        .magic_primary_slot =       BOOT_MAGIC_ANY,
        .magic_secondary_slot =     BOOT_MAGIC_GOOD,
        .image_ok_primary_slot =    BOOT_FLAG_ANY,
        .image_ok_secondary_slot =  BOOT_FLAG_SET,
        .copy_done_primary_slot =   BOOT_FLAG_ANY,
        .swap_type =                BOOT_SWAP_TYPE_PERM,
    },
    {
        .magic_primary_slot =       BOOT_MAGIC_GOOD,
        .magic_secondary_slot =     BOOT_MAGIC_UNSET,
        .image_ok_primary_slot =    BOOT_FLAG_UNSET,
        .image_ok_secondary_slot =  BOOT_FLAG_ANY,
        .copy_done_primary_slot =   BOOT_FLAG_SET,
        .swap_type =                BOOT_SWAP_TYPE_REVERT,
    },
};

#define BOOT_SWAP_TABLES_COUNT \
    (sizeof boot_swap_tables / sizeof boot_swap_tables[0])

static int
boot_magic_decode(const uint8_t *magic)
{
    if (memcmp(magic, BOOT_IMG_MAGIC, BOOT_MAGIC_SZ) == 0) {
        return BOOT_MAGIC_GOOD;
    }
    return BOOT_MAGIC_BAD;
}

static int
boot_flag_decode(uint8_t flag)
{
    if (flag != BOOT_FLAG_SET) {
        return BOOT_FLAG_BAD;
    }
    return BOOT_FLAG_SET;
}

static inline uint32_t
boot_magic_off(const struct flash_area *fap)
{
    return flash_area_get_size(fap) - BOOT_MAGIC_SZ;
}

static inline uint32_t
boot_image_ok_off(const struct flash_area *fap)
{
    return ALIGN_DOWN(boot_magic_off(fap) - BOOT_MAX_ALIGN, BOOT_MAX_ALIGN);
}

static inline uint32_t
boot_copy_done_off(const struct flash_area *fap)
{
    return boot_image_ok_off(fap) - BOOT_MAX_ALIGN;
}

static inline uint32_t
boot_swap_size_off(const struct flash_area *fap)
{
    return boot_swap_info_off(fap) - BOOT_MAX_ALIGN;
}

uint32_t
boot_swap_info_off(const struct flash_area *fap)
{
    return boot_copy_done_off(fap) - BOOT_MAX_ALIGN;
}

#if CONFIG_DIRECT_XIP
uint32_t boot_xip_status_addr_get(boot_area_e area_id)
{
	uint32_t phy_offset = 0xFFFFFFFF;
	if(area_id == BOOT_AREA_A)
	{
		phy_offset = (CONFIG_PRIMARY_ALL_PHY_PARTITION_OFFSET + CONFIG_PRIMARY_ALL_PHY_PARTITION_SIZE - BOOT_XIP_FLAG_OFFSET);
	}
	else if (area_id == BOOT_AREA_B)
	{
		phy_offset = (CONFIG_SECONDARY_ALL_PHY_PARTITION_OFFSET + CONFIG_SECONDARY_ALL_PHY_PARTITION_SIZE - BOOT_XIP_FLAG_OFFSET);
	}
	return phy_offset;
}
static  uint32_t
boot_xip_magic_off(const struct flash_area *fap)
{
    uint32_t phy_offset = CEIL_ALIGN_34(partition_get_phy_offset(fap->fa_id) + 
                                        partition_get_phy_size(fap->fa_id) - 4096);
    //uint32_t vir_offset = FLASH_PHY2VIRTUAL(phy_offset) -  FLASH_PHY2VIRTUAL(CEIL_ALIGN_34(partition_get_phy_offset(fap->fa_id)));
    return phy_offset;
}

uint32_t boot_xip_copy_done_off(const struct flash_area *fap)
{
    return boot_xip_magic_off(fap) + 32;
}

uint32_t boot_xip_image_ok_off(const struct flash_area *fap)
{
    return boot_xip_magic_off(fap) + 64;
}
#endif

/**
 * Determines if a status source table is satisfied by the specified magic
 * code.
 *
 * @param tbl_val               A magic field from a status source table.
 * @param val                   The magic value in a trailer, encoded as a
 *                                  BOOT_MAGIC_[...].
 *
 * @return                      1 if the two values are compatible;
 *                              0 otherwise.
 */
int
boot_magic_compatible_check(uint8_t tbl_val, uint8_t val)
{
    switch (tbl_val) {
    case BOOT_MAGIC_ANY:
        return 1;

    case BOOT_MAGIC_NOTGOOD:
        return val != BOOT_MAGIC_GOOD;

    default:
        return tbl_val == val;
    }
}

#ifdef MCUBOOT_ENC_IMAGES
static inline uint32_t
boot_enc_key_off(const struct flash_area *fap, uint8_t slot)
{
#if MCUBOOT_SWAP_SAVE_ENCTLV
    return boot_swap_size_off(fap) - ((slot + 1) * BOOT_ENC_TLV_ALIGN_SIZE);
#else
    return boot_swap_size_off(fap) - ((slot + 1) * BOOT_ENC_KEY_ALIGN_SIZE);
#endif
}
#endif

bool bootutil_buffer_is_erased(const struct flash_area *area,
                               const void *buffer, size_t len)
{
    size_t i;
    uint8_t *u8b;
    uint8_t erased_val;

    if (buffer == NULL || len == 0) {
        return false;
    }

    erased_val = flash_area_erased_val(area);
    for (i = 0, u8b = (uint8_t *)buffer; i < len; i++) {
        if (u8b[i] != erased_val) {
            return false;
        }
    }

    return true;
}

static int
boot_read_flag(const struct flash_area *fap, uint8_t *flag, uint32_t off)
{
    int rc;

    rc = flash_area_read(fap, off, flag, sizeof *flag);
    if (rc < 0) {
        return BOOT_EFLASH;
    }
    if (bootutil_buffer_is_erased(fap, flag, sizeof *flag)) {
        *flag = BOOT_FLAG_UNSET;
    } else {
        *flag = boot_flag_decode(*flag);
    }

    return 0;
}

static inline int
boot_read_copy_done(const struct flash_area *fap, uint8_t *copy_done)
{
    return boot_read_flag(fap, copy_done, boot_copy_done_off(fap));
}

#if CONFIG_DIRECT_XIP
uint32_t
boot_read_xip_status(const struct flash_area *fap, uint32_t type)
{
   	uint32_t off    = 0;
	uint32_t status = 0;

    switch (type)
    {
    case XIP_MAGIC_TYPE:
        off = boot_xip_magic_off(fap);
        break;
    case XIP_COPY_DONE_TYPE:
        off = boot_xip_copy_done_off(fap);
        break;
    case XIP_IMAGE_OK_TYPE:
        off = boot_xip_image_ok_off(fap);
        break;
    default:
        break;
    }

    bk_flash_read_bytes(off,(uint8_t *)&status, 4);

    return status;
}
int boot_check_xip_flag_address_valid(uint32_t addr, uint32_t type)
{
	uint32_t off           = 0;
	uint32_t update_id     = (flash_area_read_offset_enable());
	uint32_t phy_offset_1  = boot_xip_status_addr_get(update_id);
	uint32_t addr_offset_1 = CEIL_ALIGN_34(phy_offset_1);

	update_id = (flash_area_read_offset_enable() ^ 1);
	uint32_t phy_offset_2  = boot_xip_status_addr_get(update_id);
	uint32_t addr_offset_2 = CEIL_ALIGN_34(phy_offset_2);

	switch (type)
    {
    case XIP_MAGIC_TYPE:
		off = BOOT_XIP_MAGIC_OFFSET;
        break;
    case XIP_COPY_DONE_TYPE:
		off = BOOT_XIP_COPY_DONE_OFFSET;
        break;
    case XIP_IMAGE_OK_TYPE:
		off = BOOT_XIP_IMAGE_OK_OFFSET;
        break;
    default:
        return -1;
    }

	if(((addr_offset_1 + off) == addr)||((addr_offset_2 + off) == addr))
    {
		 return 0;
    }
	else
	{
		#if CONFIG_XIP_FLAG_DEBUG
		printf("xip flag err[addr:0x%x]\r\n",addr);
		printf("xip flag err[offset1:0x%x]\r\n",addr_offset_1);
		printf("xip flag err[offset2:0x%x]\r\n",addr_offset_2);
		printf("xip flag err[off:0x%x]\r\n",off);
		printf("xip flag err[type:0x%x]\r\n",type);
		#endif
        return -1;
	}
    return 0;
}
int boot_write_xip_status(const struct flash_area *fap, uint32_t type, uint32_t status)
{
    uint32_t off       = 0;
	uint32_t read_data = 0;
	int  ret           = 0;
    switch (type)
    {
    case XIP_MAGIC_TYPE:
        off = boot_xip_magic_off(fap);
		ret = boot_check_xip_flag_address_valid(off, XIP_MAGIC_TYPE);
        break;
    case XIP_COPY_DONE_TYPE:
        off = boot_xip_copy_done_off(fap);
		ret = boot_check_xip_flag_address_valid(off, XIP_COPY_DONE_TYPE);
        break;
    case XIP_IMAGE_OK_TYPE:
        off = boot_xip_image_ok_off(fap);
		ret = boot_check_xip_flag_address_valid(off, XIP_IMAGE_OK_TYPE);
        break;
    default:
        return -1;
    }

	if(ret != 0)
	{
		return -1;
	}

    ret = flash_write_bytes_dbus(off, (uint8_t *)&status, 4);
	if(ret != 0)
	{
		return -1;
	}

	bk_flash_read_bytes(off, (uint8_t *)&read_data, 4);
	if(memcmp(&status, &read_data,4) != 0)
	{
		return -1;
	}

	return 0;
}

void
boot_erase_xip_status_all(const struct flash_area *fap)
{
    uint32_t phy_off = boot_xip_magic_off(fap);
    phy_off &= ~(0x1000-1);
    flash_area_erase(fap,phy_off - fap->fa_off,4096);
}
#endif

int
boot_read_swap_state(const struct flash_area *fap,
                     struct boot_swap_state *state)
{
    uint8_t magic[BOOT_MAGIC_SZ];
    uint32_t off;
    uint8_t swap_info;
    int rc;

    off = boot_magic_off(fap);
    rc = flash_area_read(fap, off, magic, BOOT_MAGIC_SZ);
    if (rc < 0) {
        return BOOT_EFLASH;
    }
    if (bootutil_buffer_is_erased(fap, magic, BOOT_MAGIC_SZ)) {
        state->magic = BOOT_MAGIC_UNSET;
    } else {
        state->magic = boot_magic_decode(magic);
    }

    off = boot_swap_info_off(fap);
    rc = flash_area_read(fap, off, &swap_info, sizeof swap_info);
    if (rc < 0) {
        return BOOT_EFLASH;
    }

    /* Extract the swap type and image number */
    state->swap_type = BOOT_GET_SWAP_TYPE(swap_info);
    state->image_num = BOOT_GET_IMAGE_NUM(swap_info);

    if (bootutil_buffer_is_erased(fap, &swap_info, sizeof swap_info) ||
            state->swap_type > BOOT_SWAP_TYPE_REVERT) {
        state->swap_type = BOOT_SWAP_TYPE_NONE;
        state->image_num = 0;
    }

    rc = boot_read_copy_done(fap, &state->copy_done);
    if (rc) {
        return BOOT_EFLASH;
    }

    return boot_read_image_ok(fap, &state->image_ok);
}

int
boot_read_swap_state_by_id(int flash_area_id, struct boot_swap_state *state)
{
    const struct flash_area *fap;
    int rc;

    rc = flash_area_open(flash_area_id, &fap);
    if (rc != 0) {
        return BOOT_EFLASH;
    }

    rc = boot_read_swap_state(fap, state);
    flash_area_close(fap);
    return rc;
}

int
boot_write_magic(const struct flash_area *fap)
{
    uint32_t off;
    uint32_t pad_off;
    int rc;
    uint8_t magic[BOOT_MAGIC_ALIGN_SIZE];
    uint8_t erased_val;

    off = boot_magic_off(fap);

    /* image_trailer structure was modified with additional padding such that
     * the pad+magic ends up in a flash minimum write region. The address
     * returned by boot_magic_off() is the start of magic which is not the
     * start of the flash write boundary and thus writes to the magic will fail.
     * To account for this change, write to magic is first padded with 0xFF
     * before writing to the trailer.
     */
    pad_off = ALIGN_DOWN(off, BOOT_MAX_ALIGN);

    erased_val = flash_area_erased_val(fap);

    memset(&magic[0], erased_val, sizeof(magic));
    memcpy(&magic[BOOT_MAGIC_ALIGN_SIZE - BOOT_MAGIC_SZ], BOOT_IMG_MAGIC, BOOT_MAGIC_SZ);

    BOOT_LOG_DBG("writing magic; fa_id=%d off=0x%lx (0x%lx)",
                 flash_area_get_id(fap), (unsigned long)off,
                 (unsigned long)(flash_area_get_off(fap) + off));
    rc = flash_area_write(fap, pad_off, &magic[0], BOOT_MAGIC_ALIGN_SIZE);

    if (rc != 0) {
        return BOOT_EFLASH;
    }

    return 0;
}

/**
 * Write trailer data; status bytes, swap_size, etc
 *
 * @returns 0 on success, != 0 on error.
 */
int
boot_write_trailer(const struct flash_area *fap, uint32_t off,
        const uint8_t *inbuf, uint8_t inlen)
{
    uint8_t buf[BOOT_MAX_ALIGN];
    uint8_t erased_val;
    uint32_t align;
    int rc;

    align = flash_area_align(fap);
    align = ALIGN_UP(inlen, align);
    if (align > BOOT_MAX_ALIGN) {
        return -1;
    }
    erased_val = flash_area_erased_val(fap);

    memcpy(buf, inbuf, inlen);
    memset(&buf[inlen], erased_val, align - inlen);

    rc = flash_area_write(fap, off, buf, align);
    if (rc != 0) {
        return BOOT_EFLASH;
    }

    return 0;
}

int
boot_write_trailer_flag(const struct flash_area *fap, uint32_t off,
        uint8_t flag_val)
{
    const uint8_t buf[1] = { flag_val };
    return boot_write_trailer(fap, off, buf, 1);
}

int
boot_write_image_ok(const struct flash_area *fap)
{
    uint32_t off;

    off = boot_image_ok_off(fap);
    BOOT_LOG_DBG("writing image_ok; fa_id=%d off=0x%lx (0x%lx)",
                 flash_area_get_id(fap), (unsigned long)off,
                 (unsigned long)(flash_area_get_off(fap) + off));
    return boot_write_trailer_flag(fap, off, BOOT_FLAG_SET);
}

int
boot_read_image_ok(const struct flash_area *fap, uint8_t *image_ok)
{
    return boot_read_flag(fap, image_ok, boot_image_ok_off(fap));
}

/**
 * Writes the specified value to the `swap-type` field of an image trailer.
 * This value is persisted so that the boot loader knows what swap operation to
 * resume in case of an unexpected reset.
 */
int
boot_write_swap_info(const struct flash_area *fap, uint8_t swap_type,
                     uint8_t image_num)
{
    uint32_t off;
    uint8_t swap_info;

    BOOT_SET_SWAP_INFO(swap_info, image_num, swap_type);
    off = boot_swap_info_off(fap);
    BOOT_LOG_DBG("writing swap_info; fa_id=%d off=0x%lx (0x%lx), swap_type=0x%x"
                 " image_num=0x%x",
                 flash_area_get_id(fap), (unsigned long)off,
                 (unsigned long)(flash_area_get_off(fap) + off),
                 swap_type, image_num);
    return boot_write_trailer(fap, off, (const uint8_t *) &swap_info, 1);
}

int
boot_swap_type_multi(int image_index)
{
    const struct boot_swap_table *table;
    struct boot_swap_state primary_slot;
    struct boot_swap_state secondary_slot;
    int rc;
    size_t i;

    rc = BOOT_HOOK_CALL(boot_read_swap_state_primary_slot_hook,
                        BOOT_HOOK_REGULAR, image_index, &primary_slot);
    if (rc == BOOT_HOOK_REGULAR)
    {
        rc = boot_read_swap_state_by_id(FLASH_AREA_IMAGE_PRIMARY(image_index),
                                        &primary_slot);
    }
    if (rc) {
        return BOOT_SWAP_TYPE_PANIC;
    }

    rc = boot_read_swap_state_by_id(FLASH_AREA_IMAGE_SECONDARY(image_index),
                                    &secondary_slot);
    if (rc == BOOT_EFLASH) {
        BOOT_LOG_INF("Secondary image of image pair (%d.) "
                     "is unreachable. Treat it as empty", image_index);
        secondary_slot.magic = BOOT_MAGIC_UNSET;
        secondary_slot.swap_type = BOOT_SWAP_TYPE_NONE;
        secondary_slot.copy_done = BOOT_FLAG_UNSET;
        secondary_slot.image_ok = BOOT_FLAG_UNSET;
        secondary_slot.image_num = 0;
    } else if (rc) {
        return BOOT_SWAP_TYPE_PANIC;
    }

    for (i = 0; i < BOOT_SWAP_TABLES_COUNT; i++) {
        table = boot_swap_tables + i;

        if (boot_magic_compatible_check(table->magic_primary_slot,
                                        primary_slot.magic) &&
            boot_magic_compatible_check(table->magic_secondary_slot,
                                        secondary_slot.magic) &&
            (table->image_ok_primary_slot == BOOT_FLAG_ANY   ||
                table->image_ok_primary_slot == primary_slot.image_ok) &&
            (table->image_ok_secondary_slot == BOOT_FLAG_ANY ||
                table->image_ok_secondary_slot == secondary_slot.image_ok) &&
            (table->copy_done_primary_slot == BOOT_FLAG_ANY  ||
                table->copy_done_primary_slot == primary_slot.copy_done)) {
            BOOT_LOG_INF("Swap type: %s",
                         table->swap_type == BOOT_SWAP_TYPE_TEST   ? "test"   :
                         table->swap_type == BOOT_SWAP_TYPE_PERM   ? "perm"   :
                         table->swap_type == BOOT_SWAP_TYPE_REVERT ? "revert" :
                         "BUG; can't happen");
            if (table->swap_type != BOOT_SWAP_TYPE_TEST &&
                    table->swap_type != BOOT_SWAP_TYPE_PERM &&
                    table->swap_type != BOOT_SWAP_TYPE_REVERT) {
                return BOOT_SWAP_TYPE_PANIC;
            }
            return table->swap_type;
        }
    }

    BOOT_LOG_INF("Swap type: none");
    return BOOT_SWAP_TYPE_NONE;
}

/*
 * This function is not used by the bootloader itself, but its required API
 * by external tooling like mcumgr.
 */
int
boot_swap_type(void)
{
    return boot_swap_type_multi(0);
}

/**
 * Marks the image with the given index in the secondary slot as pending. On the
 * next reboot, the system will perform a one-time boot of the the secondary
 * slot image.
 *
 * @param image_index       Image pair index.
 *
 * @param permanent         Whether the image should be used permanently or
 *                          only tested once:
 *                               0=run image once, then confirm or revert.
 *                               1=run image forever.
 *
 * @return                  0 on success; nonzero on failure.
 */
int
boot_set_pending_multi(int image_index, int permanent)
{
    const struct flash_area *fap;
    struct boot_swap_state state_secondary_slot;
    uint8_t swap_type;
    int rc;

    rc = flash_area_open(FLASH_AREA_IMAGE_SECONDARY(image_index), &fap);
    if (rc != 0) {
        return BOOT_EFLASH;
    }

    rc = boot_read_swap_state(fap, &state_secondary_slot);
    if (rc != 0) {
        goto done;
    }

    switch (state_secondary_slot.magic) {
    case BOOT_MAGIC_GOOD:
        /* Swap already scheduled. */
        break;

    case BOOT_MAGIC_UNSET:
        rc = boot_write_magic(fap);

        if (rc == 0 && permanent) {
            rc = boot_write_image_ok(fap);
        }

        if (rc == 0) {
            if (permanent) {
                swap_type = BOOT_SWAP_TYPE_PERM;
            } else {
                swap_type = BOOT_SWAP_TYPE_TEST;
            }
            rc = boot_write_swap_info(fap, swap_type, 0);
        }

        break;

    case BOOT_MAGIC_BAD:
        /* The image slot is corrupt.  There is no way to recover, so erase the
         * slot to allow future upgrades.
         */
        flash_area_erase(fap, 0, flash_area_get_size(fap));
        rc = BOOT_EBADIMAGE;
        break;

    default:
        assert(0);
        rc = BOOT_EBADIMAGE;
    }

done:
    flash_area_close(fap);
    return rc;
}

/**
 * Marks the image with index 0 in the secondary slot as pending. On the next
 * reboot, the system will perform a one-time boot of the the secondary slot
 * image. Note that this API is kept for compatibility. The
 * boot_set_pending_multi() API is recommended.
 *
 * @param permanent         Whether the image should be used permanently or
 *                          only tested once:
 *                               0=run image once, then confirm or revert.
 *                               1=run image forever.
 *
 * @return                  0 on success; nonzero on failure.
 */
int
boot_set_pending(int permanent)
{
    return boot_set_pending_multi(0, permanent);
}

/**
 * Marks the image with the given index in the primary slot as confirmed.  The
 * system will continue booting into the image in the primary slot until told to
 * boot from a different slot.
 *
 * @param image_index       Image pair index.
 *
 * @return                  0 on success; nonzero on failure.
 */
int
boot_set_confirmed_multi(int image_index)
{
    const struct flash_area *fap = NULL;
    struct boot_swap_state state_primary_slot;
    int rc;

    rc = flash_area_open(FLASH_AREA_IMAGE_PRIMARY(image_index), &fap);
    if (rc != 0) {
        return BOOT_EFLASH;
    }

    rc = boot_read_swap_state(fap, &state_primary_slot);
    if (rc != 0) {
        goto done;
    }

    switch (state_primary_slot.magic) {
    case BOOT_MAGIC_GOOD:
        /* Confirm needed; proceed. */
        break;

    case BOOT_MAGIC_UNSET:
        /* Already confirmed. */
        goto done;

    case BOOT_MAGIC_BAD:
        /* Unexpected state. */
        rc = BOOT_EBADVECT;
        goto done;
    }

    /* Intentionally do not check copy_done flag
     * so can confirm a padded image which was programed using a programing
     * interface.
     */

    if (state_primary_slot.image_ok != BOOT_FLAG_UNSET) {
        /* Already confirmed. */
        goto done;
    }

    rc = boot_write_image_ok(fap);

done:
    flash_area_close(fap);
    return rc;
}

/**
 * Marks the image with index 0 in the primary slot as confirmed.  The system
 * will continue booting into the image in the primary slot until told to boot
 * from a different slot.  Note that this API is kept for compatibility. The
 * boot_set_confirmed_multi() API is recommended.
 *
 * @return                  0 on success; nonzero on failure.
 */
int
boot_set_confirmed(void)
{
    return boot_set_confirmed_multi(0);
}
