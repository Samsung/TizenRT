// Copyright 2022-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <assert.h>
#include "bootutil/image.h"
#include "bootutil/bootutil.h"
#include "bootutil/fault_injection_hardening.h"
#include "flash_map_backend/flash_map_backend.h"
#include "components/log.h"
#include "bk_tfm_log.h"

#define TAG "hook"
#define BL2_HOOK_LOGD BK_LOGD
#define BL2_HOOK_LOGI BK_LOGD
#define BL2_HOOK_LOGW BK_LOGD
#define BL2_HOOK_LOGE BK_LOGD
#define BL2_HOOK_DEBUG 0

#define BL2_HOOK_IMG_READ_DEBUG_LEN 0x100

static void dump_image_header(struct image_header *hdr)
{
	BL2_HOOK_LOGD(TAG, "magic=%x\r\n", hdr->ih_magic);
	BL2_HOOK_LOGD(TAG, "load_addr=%x\r\n", hdr->ih_load_addr);
	BL2_HOOK_LOGD(TAG, "hdr_size=%x\r\n", hdr->ih_hdr_size);
	BL2_HOOK_LOGD(TAG, "protect_tlv_size=%x\r\n", hdr->ih_protect_tlv_size);
	BL2_HOOK_LOGD(TAG, "img_size=%x\r\n", hdr->ih_img_size);
	BL2_HOOK_LOGD(TAG, "flags=%x\r\n", hdr->ih_flags);
	BL2_HOOK_LOGD(TAG, "pad=%x\r\n", hdr->_pad1);
}

static inline bool boot_u32_safe_add(uint32_t *dest, uint32_t a, uint32_t b)
{
	if (a > UINT32_MAX - b) {
		return false;
	} else {
		*dest = a + b;
		return true;
	}
}

static bool
boot_is_header_valid(const struct image_header *hdr, const struct flash_area *fap)
{
    uint32_t size;

    if (hdr->ih_magic != IMAGE_MAGIC) {
        BL2_HOOK_LOGE(TAG, "invalid img magic: %x\r\n", hdr->ih_magic);
        return false;
    }   

    if (!boot_u32_safe_add(&size, hdr->ih_img_size, hdr->ih_hdr_size)) {
        BL2_HOOK_LOGE(TAG, "invalid size: size=%x, img_size=%d, hdr_size=%x\r\n", size, hdr->ih_img_size, hdr->ih_hdr_size);
        return false;
    }

    if (size >= flash_area_get_size(fap)) {
        BL2_HOOK_LOGE(TAG, "invalid size: size=%x, area_size=%x\r\n", size, fap->fa_size);
        return false;
    }   

    return true;
}

uint32_t boot_get_1st_instruction_physical_off(const struct flash_area *area)
{
        uint32_t code_partition_phy_off = area->fa_off + BL2_HEADER_SIZE;
        uint32_t first_instruction_virtual_off = FLASH_PHY2VIRTUAL_CODE_START(code_partition_phy_off);
        uint32_t first_instruction_physical_off = FLASH_VIRTUAL2PHY(first_instruction_virtual_off);

        BL2_HOOK_LOGD(TAG, "code_partition_off=%x, 1st_instruction_physical_off=%x\r\n",
                code_partition_phy_off, first_instruction_physical_off);
        return first_instruction_physical_off;
}

uint32_t boot_get_1st_instruction_virtual_off(const struct flash_area *area)
{
	uint32_t code_partition_phy_off = area->fa_off + BL2_HEADER_SIZE;
	uint32_t first_instruction_virtual_off = FLASH_PHY2VIRTUAL_CODE_START(code_partition_phy_off);

	BL2_HOOK_LOGD(TAG, "code_partition_off=%x, 1st_instruction_virtual_off=%x, header=%x\r\n",
		code_partition_phy_off, first_instruction_virtual_off, BL2_HEADER_SIZE);
	return first_instruction_virtual_off;
}

uint32_t boot_get_img_padding_len(const struct flash_area *area)
{
	uint32_t padding_len = boot_get_1st_instruction_physical_off(area) - area->fa_off - BL2_HEADER_SIZE;
	return padding_len;
}

uint32_t boot_get_off_with_padding(const struct flash_area *area, uint32_t off)
{
	if ((off < BL2_HEADER_SIZE) || (off >= (area->fa_size - BL2_TRAILER_SIZE))) {
		return off;
	}

	return (off + boot_get_img_padding_len(area));
}
 
uint32_t boot_tlv_off(const struct flash_area *fap, const struct image_header *hdr)
{
	return boot_get_1st_instruction_virtual_off(fap) + hdr->ih_img_size;
}  

int boot_read_tlv_data(const struct flash_area *fap, uint32_t virtual_off, uint8_t* data, int size)
{
        BL2_HOOK_LOGD(TAG, "LOAD fa_off=%x, start=%x, size=%x\r\n", fap->fa_off, virtual_off, size);
        memcpy(data, (void*)(SOC_FLASH_DATA_BASE + virtual_off), size);
	BK_TFM_DUMP_BUF("tlv", data, size);
	return 0;
}

/* @retval 0: header was read/populated
 *         FIH_FAILURE: image is invalid,
 *         BOOT_HOOK_REGULAR if hook not implemented for the image-slot,
 *         othervise an error-code value.
 */
int boot_read_image_header_hook(int img_index, int slot,
                                struct image_header *img_hed)
{
	const struct flash_area *fap;
	struct image_header hdr = {0};
	int area_id = 0;
	int rc;

        area_id = flash_area_id_from_multi_image_slot(img_index, slot);
	rc = flash_area_open(area_id, &fap);
	if (rc == 0) {
		rc = flash_area_read(fap, ADAPT_SIGN_IMAGE_HEAD_OFFSET, &hdr, sizeof(hdr));
	}

	if (rc != 0) {
		BL2_HOOK_LOGE(TAG, "failed to open flash\r\n");
	}

	if (boot_is_header_valid(&hdr, fap) == false) {
		BL2_HOOK_LOGE(TAG, "invalid image header\r\n");
        }

	BL2_HOOK_LOGD(TAG, "read image=%d, slot=%d hdr\r\n", img_index, slot);
	dump_image_header(&hdr);
	BL2_HOOK_LOGD(TAG, "tlv off=%x\r\n", boot_tlv_off(fap, &hdr));
	flash_area_close(fap);
	return BOOT_HOOK_REGULAR;
}

/* @retval FIH_SUCCESS: image is valid,
 *         FIH_FAILURE: image is invalid,
 *         fih encoded BOOT_HOOK_REGULAR if hook not implemented for
 *         the image-slot.
 */
fih_int boot_image_check_hook(int img_index, int slot)
{
	FIH_RET(fih_int_encode(BOOT_HOOK_REGULAR));
}

int boot_perform_update_hook(int img_index, struct image_header *img_head,
                             const struct flash_area *area)
{
	return BOOT_HOOK_REGULAR;
}

static void dump_swap_state(int image_index, struct boot_swap_state *state)

{
	BL2_HOOK_LOGD(TAG, "magic=%x\r\n", state->magic);
	BL2_HOOK_LOGD(TAG, "swap_type=%x\r\n", state->swap_type);
	BL2_HOOK_LOGD(TAG, "image_num=%x\r\n", state->image_num);
	BL2_HOOK_LOGD(TAG, "copy_done=%x\r\n", state->copy_done);
	BL2_HOOK_LOGD(TAG, "image_ok=%x\r\n", state->image_ok);
}
	
int boot_read_swap_state_primary_slot_hook(int image_index,
                                           struct boot_swap_state *state)
{
	dump_swap_state(image_index, state);
	return BOOT_HOOK_REGULAR;
}

int boot_copy_region_post_hook(int img_index, const struct flash_area *area,
                               size_t size)
{
	return 0;
}

int boot_serial_uploaded_hook(int img_index, const struct flash_area *area,
                               size_t size)
{
	return 0;
}

int boot_img_install_stat_hook(int image_index, int slot, int *img_install_stat)
{
	return BOOT_HOOK_REGULAR;
}

int flash_area_read_post_hook(const struct flash_area *area, uint32_t off, void *dst, uint32_t len)
{
#if BL2_HOOK_DEBUG
	if ((len > 0) && (len < BL2_HOOK_IMG_READ_DEBUG_LEN)) {
		uint8_t *buf = (uint8_t*)dst;

        	BK_LOG_RAW("area%d read: flash_off=%x off=%x len=%x\r\n", area->fa_id, area->fa_off, off, len);
		for (int i = 0; i < len; i++) {
			BK_LOG_RAW("%02x ", buf[i]);

			if (i && (i % 16 == 0)) {
				BK_LOG_RAW("\r\n");
			}
		}

        	BK_LOG_RAW("\r\n");
	}
#endif

	return 0;
}
