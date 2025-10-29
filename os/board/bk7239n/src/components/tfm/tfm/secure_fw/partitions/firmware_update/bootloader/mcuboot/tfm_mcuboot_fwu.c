/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa/crypto.h"
#include "tfm_sp_log.h"
#include "bootutil_priv.h"
#include "bootutil/bootutil.h"
#include "bootutil/image.h"
#include "flash_map_backend/flash_map_backend.h"
#include "sysflash/sysflash.h"
#include "tfm_bootloader_fwu_abstraction.h"
#include "tfm_fwu_req_mngr.h"
#include "tfm_boot_status.h"
#include "service_api.h"
#include "tfm_memory_utils.h"
#include "tfm_secure_api.h"
#include "Driver_Flash.h"
#include "flash_partition.h"
#include "flash_map/flash_map.h"

#define BOOT_XIP_FLAG_SIZE        (4096)

#if (MCUBOOT_IMAGE_NUMBER == 1)
#define MAX_IMAGE_INFO_LENGTH    (sizeof(struct image_version) + \
                                  SHARED_DATA_ENTRY_HEADER_SIZE)
#else
#define MAX_IMAGE_INFO_LENGTH    2 * (sizeof(struct image_version) + \
                                      SHARED_DATA_ENTRY_HEADER_SIZE)
#endif
#define TFM_MCUBOOT_FWU_INVALID_IMAGE_ID    0xFF

uint32_t tfm_hal_is_ignore_data_shared(void);

/*
 * \struct fwu_image_info_data
 *
 * \brief Contains the received boot status information from bootloader
 *
 * \details This is a redefinition of \ref tfm_boot_data to allocate the
 *          appropriate, service dependent size of \ref boot_data.
 */
typedef struct fwu_image_info_data_s {
    struct shared_data_tlv_header header;
    uint8_t data[MAX_IMAGE_INFO_LENGTH];
} fwu_image_info_data_t;

typedef struct tfm_fwu_mcuboot_ctx_s {
    /* The flash area corresponding to mcuboot_image_id. */
    const struct flash_area *fap;
    uint8_t mcuboot_image_id;

    /* The size of the downloaded data in the FWU process. */
    size_t loaded_size;
} tfm_fwu_mcuboot_ctx_t;

static tfm_fwu_mcuboot_ctx_t mcuboot_ctx[TFM_FWU_MAX_IMAGES];
static fwu_image_info_data_t boot_shared_data;

static inline bool is_64k_aligned(uint32_t addr)
{
	return ((addr & (KB(64) - 1)) == 0);
}

static inline bool is_32k_aligned(uint32_t addr)
{
	return ((addr & (KB(32) - 1)) == 0);
}

static int convert_id_from_bl_to_mcuboot(bl_image_id_t bl_image_id,
                                         uint8_t *mcuboot_image_id)
{
#if (MCUBOOT_IMAGE_NUMBER == 1)
#if TEMP_CODE
	if (bl_image_id == FWU_IMAGE_TYPE_FULL)
	{
		*mcuboot_image_id = FLASH_IMAGE_ALL_ID;
	}
	else
	{
		LOG_ERRFMT("TFM FWU: invalid image_type: %d", bl_image_id);
		return -1;
	}
#endif
	switch(bl_image_id)
	{
		case FWU_IMAGE_TYPE_FULL:
			*mcuboot_image_id = FLASH_IMAGE_ALL_ID;
			break;
		case FWU_IMAGE_TYPE_PRIMARY_MANIFEST:
			*mcuboot_image_id = FLASH_IMAGE_PRIMARY_MANIFEST_ID;
			break;
		case FWU_IMAGE_TYPE_SECONDARY_MANIFEST:
			*mcuboot_image_id = FLASH_IMAGE_SECONDARY_MANIFEST_ID;
			break;
		case FWU_IMAGE_TYPE_PRIMARY_BL2:
			*mcuboot_image_id = FLASH_IMAGE_PRIMARY_BL2_ID;
			break;
		case FWU_IMAGE_TYPE_SECONDARY_BL2:
			*mcuboot_image_id = FLASH_IMAGE_SECONDARY_BL2_ID;
			break;
		case FWU_IMAGE_TYPE_PARTITION_FLAG:
			*mcuboot_image_id = FLASH_IMAGE_PRIMARY_PARTITION_ID;
			break;
		case FWU_IMAGE_TYPE_PARTITION_B_FLAG:
			*mcuboot_image_id = FLASH_IMAGE_SECONDARY_PARTITON_ID;
			break;
		 default:
		LOG_ERRFMT("TFM FWU: invalid image_type: %d", bl_image_id);
		return -1;
	}
#else
    if (bl_image_id == FWU_IMAGE_TYPE_SECURE) {
        /* The image id in mcuboot. 0: the secure image. */
        *mcuboot_image_id = 0;
    } else if (bl_image_id == FWU_IMAGE_TYPE_NONSECURE) {
        /* The image id in mcuboot. 1: the non-secure image. */
        *mcuboot_image_id = 1;
    }  else {
        LOG_ERRFMT("TFM FWU: invalid image_type: %d", bl_image_id);
        return -1;
    }
#endif
    return 0;
}

#if (MCUBOOT_IMAGE_NUMBER > 1)
static int convert_id_from_mcuboot_to_bl(uint8_t mcuboot_image_id,
                                         bl_image_id_t *bl_image_id)
{
    uint8_t image_type;

    if (bl_image_id == NULL) {
        return -1;
    }

    if (mcuboot_image_id == 0) {
        /* The image id in mcuboot. 0: the secure image. */
        image_type = FWU_IMAGE_TYPE_SECURE;
    } else if (mcuboot_image_id == 1) {
        /* The image id in mcuboot. 1: the non-secure image. */
        image_type = FWU_IMAGE_TYPE_NONSECURE;
    }  else {
        LOG_ERRFMT("TFM FWU: invalid mcuboot image id\n\r: %d",
                mcuboot_image_id);
        return -1;
    }

    *bl_image_id = image_type;
    return 0;
}
#endif

/*
 * Get the flash area of the image mcuboot_image_id.
 */
static bool get_flash_image_index(uint8_t mcuboot_image_id, uint8_t *index)
{
    for (uint8_t i = 0; i < TFM_FWU_MAX_IMAGES; i++) {
        if (mcuboot_ctx[i].mcuboot_image_id == mcuboot_image_id) {
            *index = i;
            return true;
        }
    }
    return false;
}

static bool get_next_free_index(uint8_t *index)
{
    for (uint8_t i = 0; i < TFM_FWU_MAX_IMAGES; i++) {
        if (mcuboot_ctx[i].fap == NULL) {
            *index = i;
            return true;
        }
    }
    return false;
}

static int fwu_bootloader_get_shared_data(void)
{
    uint8_t temp_data[] = {0x16, 0x20, 0x10, 0x00, 0x00, 0x20, 0x80, 0x00, 0x00, 0x00,
                            0x01, 0x00, 0x00, 0x00, 0x00, 0x00};

    if(tfm_hal_is_ignore_data_shared()){
        memcpy(&boot_shared_data, temp_data, sizeof(temp_data));
        return PSA_SUCCESS;
    }

    return tfm_core_get_boot_data(TLV_MAJOR_FWU,
                                  (struct tfm_boot_data *)&boot_shared_data,
                                  sizeof(boot_shared_data));
}

#if 0 /* optimize code size for no used function*/
static psa_status_t get_running_image_version(uint8_t mcuboot_image_id,
                                              struct image_version *image_ver)
{
    struct shared_data_tlv_entry tlv_entry;
    uint8_t *tlv_end;
    uint8_t *tlv_curr;

    /* The bootloader writes the image version information into the memory which
     * is shared between MCUboot and TF-M. Read the shared memory.
     */
    if (boot_shared_data.header.tlv_magic != SHARED_DATA_TLV_INFO_MAGIC) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    tlv_end = (uint8_t *)&boot_shared_data +
                boot_shared_data.header.tlv_tot_len;
    tlv_curr = boot_shared_data.data;

    while (tlv_curr < tlv_end) {
        (void)memcpy(&tlv_entry, tlv_curr, SHARED_DATA_ENTRY_HEADER_SIZE);
        if ((GET_FWU_CLAIM(tlv_entry.tlv_type) == SW_VERSION) &&
            (GET_FWU_MODULE(tlv_entry.tlv_type) == mcuboot_image_id)) {
            if (tlv_entry.tlv_len != sizeof(struct image_version)) {
                return PSA_ERROR_GENERIC_ERROR;
            }
            memcpy(image_ver,
                tlv_curr + SHARED_DATA_ENTRY_HEADER_SIZE,
                tlv_entry.tlv_len);
            return PSA_SUCCESS;
        }
        tlv_curr += SHARED_DATA_ENTRY_HEADER_SIZE + tlv_entry.tlv_len;
    }

    return PSA_ERROR_GENERIC_ERROR;
}
#endif
psa_status_t fwu_bootloader_init(void)
{
#if defined(CONFIG_BINARY_SIGNING) && (CONFIG_BINARY_SIGNING == 1)
    if (fwu_bootloader_get_shared_data() != TFM_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }
#endif
    return PSA_SUCCESS;
}

#define REG_WRITE(_r, _v) ({\
		(*(volatile uint32_t *)(_r)) = (_v);\
	})

#define REG_SET(r, l, h, v) do { \
	uint32_t reg_v = *(volatile uint32_t*)(r);\
	reg_v &= ~((( 1 << ((h) + 1)) - 1) & (~( (1 << (l)) - 1)));\
	reg_v |= ((v) << (l));\
	*(volatile uint32_t*)(r) = reg_v;\
} while(0)
static void wdt_close()
{
    REG_WRITE(0x54000600, 0x5A0000);
    REG_WRITE(0x54000600, 0xA50000);
    REG_SET(0x54800000 + 4 * 2, 1, 1, 1);
    REG_WRITE(0x54800000 + 4 * 4, 0x5A0000);
    REG_WRITE(0x54800000 + 4 * 4, 0xA50000);
}


psa_status_t fwu_bootloader_staging_area_init(bl_image_id_t bootloader_image_id)
{
    uint8_t mcuboot_image_id = 0;
    uint8_t index;
    const struct flash_area *fap;
    uint32_t erase_size     = 0;
	psa_status_t     ret    = 0;

    if (convert_id_from_bl_to_mcuboot(bootloader_image_id,
                                      &mcuboot_image_id) != 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

	if(bootloader_image_id == FWU_IMAGE_TYPE_FULL)
	{
		uint32_t update_id = (flash_area_read_offset_enable() ^ 1);
		if(flash_area_open(update_id,&fap)!= 0)
		{
			LOG_ERRFMT("TFM FWU: opening flash failed.\r\n");
			return PSA_ERROR_GENERIC_ERROR;
		}
	}
	else
	{
	    if(flash_area_open(FLASH_AREA_IMAGE_SECONDARY(mcuboot_image_id),&fap) != 0)
	    {
	        LOG_ERRFMT("TFM FWU: opening flash failed.\r\n");
	        return PSA_ERROR_GENERIC_ERROR;
	    }
	}

    if (get_flash_image_index(mcuboot_image_id, &index) ||
        get_next_free_index(&index)) {
		#if DEBUG_ADDR_OFFSET
        printf("init index:%d\r\n",index);
		printf("init fa_off:0x%x\r\n",fap->fa_off);
		printf("init fa_id:0x%x\r\n",fap->fa_id);
		printf("init size:0x%x\r\n",fap->fa_size);
		#endif
        mcuboot_ctx[index].mcuboot_image_id = mcuboot_image_id;
        mcuboot_ctx[index].fap = fap;

        /* Reset the loaded_size. */
        mcuboot_ctx[index].loaded_size = 0;
    } else {
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

#if CONFIG_DIRECT_XIP

	if(bootloader_image_id == FWU_IMAGE_TYPE_FULL)
	{
		uint32_t update_id = (flash_area_read_offset_enable() ^ 1);
		flash_area_open(update_id,&fap);
		//erase_size = (fap->fa_size / 32 *34);
		erase_size =  partition_get_phy_size(update_id);
	}
	else
	{
		erase_size = (fap->fa_size);
	}
	#if DEBUG_ADDR_OFFSET
	printf("offset:[0x%x]erase_size:[0x%x]\r\n",fap->fa_off,erase_size);
	printf("fa_size:0x%x\r\n",fap->fa_size);
	#endif
	/*When app image is upgrading, clear the flag of copy done first, void the device suddenly power off*/
	/*clear the flag of copy done before erase area,because erasing area will consume about several seconds*/

    wdt_close();

	if(bootloader_image_id == FWU_IMAGE_TYPE_FULL)
	{
		ret = flash_area_erase_sector(boot_xip_status_addr_get(fap->fa_id),0,BOOT_XIP_FLAG_SIZE);
		//boot_write_xip_status(fap,XIP_COPY_DONE_TYPE,XIP_BACK);
		//flash_area_erase(fap, 0, erase_size-BOOT_XIP_FLAG_SIZE); //origin strategy
		if(ret != PSA_SUCCESS)
		{
			goto exit;
		}
	}
	else
	{
		ret = flash_area_erase(fap, 0, erase_size);
		if(ret != PSA_SUCCESS)
		{
			goto exit;
		}
	}

    flash_area_close(fap);
#else
    if (flash_area_erase(fap, 0, fap->fa_size) != 0) {
        return PSA_ERROR_GENERIC_ERROR;
    }
#endif
#if ORIGINAL_SOLUTION
#if CONFIG_DIRECT_XIP
    uint32_t update_id = (flash_area_read_offset_enable() ^ 1);
    flash_area_open(update_id,&fap);
    uint32_t erase_size = (fap->fa_size / 32 *34);
    wdt_close();
    flash_area_erase(fap, 0, erase_size);
    boot_write_xip_status(fap,XIP_MAGIC_TYPE,XIP_SET);
    flash_area_close(fap);
#else
    if (flash_area_erase(fap, 0, fap->fa_size) != 0) {
        LOG_ERRFMT("TFM FWU: erasing flash failed.\r\n");
        return PSA_ERROR_GENERIC_ERROR;
    }
#endif
#endif

    return PSA_SUCCESS;
exit:
	return PSA_ERROR_INSUFFICIENT_DATA;
}

psa_status_t fwu_bootloader_load_image(bl_image_id_t bootloader_image_id,
                                       size_t block_offset,
                                       const void *block,
                                       size_t block_size)
{
    uint8_t mcuboot_image_id = 0;
    uint8_t index;
    const struct flash_area *fap;

    if (block == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }
	//printf("bootloader_image_id:0x%x\r\n",bootloader_image_id);
    if (convert_id_from_bl_to_mcuboot(bootloader_image_id,
                                      &mcuboot_image_id) != 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }
	//printf("mcu_image_id:%d\r\n",mcuboot_image_id);
    /* The image should already be added into the mcuboot_ctx. */
    if (get_flash_image_index(mcuboot_image_id, &index)) {
		//printf("load index:%d\r\n",index);
        fap = mcuboot_ctx[index].fap;
		//printf("load fap_off:0x%x\r\n",fap->fa_off);
    } else {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (flash_area_update_dbus(fap, block_offset, block, block_size) != 0) {
        LOG_ERRFMT("TFM FWU: write flash failed.\r\n");
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* The overflow check has been done in flash_area_write. */
    mcuboot_ctx[index].loaded_size += block_size;
    return PSA_SUCCESS;
}

#if (MCUBOOT_IMAGE_NUMBER > 1)
/**
 * \brief Compare image version numbers not including the build number.
 *
 * \param[in] image_ver_1 The first image version to compare.
 *
 * \param[in] image_ver_2 The second image version to compare.
 *
 * \return true         image_ver_1 is greater or equal than image_ver_2.
 * \return false        image_ver_1 is less than image_ver_2.
 */
static bool is_version_greater_or_equal(const struct image_version *image_ver_1,
                                        const struct image_version *image_ver_2)
{
    if (image_ver_1->iv_major > image_ver_2->iv_major) {
        return true;
    }
    if (image_ver_1->iv_major < image_ver_2->iv_major) {
        return false;
    }
    /* The major version numbers are equal, continue comparison. */
    if (image_ver_1->iv_minor > image_ver_2->iv_minor) {
        return true;
    }
    if (image_ver_1->iv_minor < image_ver_2->iv_minor) {
        return false;
    }
    /* The minor version numbers are equal, continue comparison. */
    if (image_ver_1->iv_revision >= image_ver_2->iv_revision) {
        return true;
    }
    return false;
}
#endif

psa_status_t fwu_bootloader_install_image(bl_image_id_t bootloader_image_id,
                                          bl_image_id_t *dependency,
                                        psa_image_version_t *dependency_version)
{
    uint8_t mcuboot_image_id = 0;
    const struct flash_area *fap;
    uint8_t index;
    psa_status_t ret;
#if (MCUBOOT_IMAGE_NUMBER > 1)
    struct image_tlv_iter it;
    struct image_header hdr;
    int rc;
    uint32_t off;
    uint16_t len;
    struct image_dependency dep;
    struct image_version image_ver = { 0 };
    const struct flash_area *fap_secondary;
    struct image_header hdr_secondary;
    uint8_t boot_magic[BOOT_MAGIC_SZ];
    bool check_pass = false;
#endif

    if ((dependency == NULL || dependency_version == NULL)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (convert_id_from_bl_to_mcuboot(bootloader_image_id,
                                      &mcuboot_image_id) != 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* The image should already be added into the mcuboot_ctx. */
    if (get_flash_image_index(mcuboot_image_id, &index)) {
        fap = mcuboot_ctx[index].fap;
    } else {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

#if (MCUBOOT_IMAGE_NUMBER > 1)
    /* Read the image header. */
    if (flash_area_read(fap, 0, &hdr, sizeof(hdr)) != 0) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Return PSA_ERROR_GENERIC_ERROR if the image header is invalid. */
    if (hdr.ih_magic != IMAGE_MAGIC) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Initialize the iterator. */
    if (bootutil_tlv_iter_begin(&it, &hdr, fap, IMAGE_TLV_DEPENDENCY, true)) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Iterate and verify the image dependencies. */
    while (true) {
        rc = bootutil_tlv_iter_next(&it, &off, &len, NULL);
        if (rc < 0) {
            return PSA_ERROR_GENERIC_ERROR;
        } else if (rc > 0) {
            /* No more dependency found. */
            rc = 0;
            ret = PSA_SUCCESS_REBOOT;
            break;
        }
        check_pass = false;

        /* A dependency requirement is found. */
        if (flash_area_read(fap, off, &dep, len) != 0) {
            return PSA_ERROR_GENERIC_ERROR;
        }

        if (dep.image_id > MCUBOOT_IMAGE_NUMBER) {
            return PSA_ERROR_GENERIC_ERROR;
        }

        /* As this partition does not validate the image in the secondary slot,
         * so it has no information of which image will be chosen to run after
         * reboot. So if the dependency image in the primary slot or that in the
         * secondary slot can meet the dependency requirement, then the
         * dependency check pass.
         */
        /* Check the dependency image in the primary slot. */
        if(0 == tfm_hal_is_ignore_data_shared()){
            if (get_running_image_version(dep.image_id,
                                          &image_ver) != PSA_SUCCESS) {
                return PSA_ERROR_GENERIC_ERROR;
            }
            
            /* Check whether the version of the running image can meet the
             * dependency requirement.
             */
            if (is_version_greater_or_equal(&image_ver,
                                            &dep.image_min_version)) {
                check_pass = true;
            } else {
                /* The running image cannot meet the dependency requirement. Check
                 * the dependency image in the secondary slot.
                 */
                if ((flash_area_open(FLASH_AREA_IMAGE_SECONDARY(dep.image_id),
                                     &fap_secondary)) != 0) {
                    return PSA_ERROR_GENERIC_ERROR;
                }
                if (flash_area_read(fap_secondary,
                                    0,
                                    &hdr_secondary,
                                    sizeof(hdr_secondary)) != 0) {
                    flash_area_close(fap_secondary);
                    return PSA_ERROR_GENERIC_ERROR;
                }
            
                /* Check the version of the dependency image in the secondary slot
                 * only if the image header, as well as the boot magic, is good.
                 */
                if (hdr_secondary.ih_magic == IMAGE_MAGIC) {
                    /* Check the boot magic. */
                    if (flash_area_read(fap_secondary,
                                        fap_secondary->fa_size - BOOT_MAGIC_SZ,
                                        boot_magic,
                                        BOOT_MAGIC_SZ) != 0) {
                        flash_area_close(fap_secondary);
                        return PSA_ERROR_GENERIC_ERROR;
                    }
                    if ((memcmp(boot_magic, &boot_img_magic, BOOT_MAGIC_SZ) == 0) &&
                        (is_version_greater_or_equal(&hdr_secondary.ih_ver,
                                                     &dep.image_min_version))) {
                        /* The dependency image in the secondary slot meet the
                         * dependency requirement.
                         */
                        check_pass = true;
                    }
                }
                flash_area_close(fap_secondary);
            }
        }

        if (!check_pass) {
            if (convert_id_from_mcuboot_to_bl(dep.image_id,
                                              dependency) != 0) {
                return PSA_ERROR_GENERIC_ERROR;
            }

            /* Return the first dependency check failed image. */
            memcpy(dependency_version,
                   &dep.image_min_version,
                   sizeof(*dependency_version));
            ret = PSA_ERROR_DEPENDENCY_NEEDED;
            break;
        }
    }
#else
    /* No dependency check is needed in single image case. */
    ret = PSA_SUCCESS_REBOOT;
#endif

    /* Write the boot magic in the image trailer so that this image will be
     * taken as a candidate. Note that even if a dependency is required, the
     * boot magic should still be set. Therefore when circular dependency exists
     * the firmware update will not enter the loop of returning
     * PSA_ERROR_DEPENDENCY_NEEDED when installing. When all the dependencies
     * are installed, the user should call the psa_fwu_install API to install
     * this image again.
     */
    if (boot_set_pending_multi(mcuboot_image_id, false) != 0) {
        return PSA_ERROR_GENERIC_ERROR;
    } else {
        return ret;
    }
}

int fwu_security_write_xip_active(const struct flash_area *fap,uint8_t area_id)
{
	int ret = 0;
	uint32_t xip_image_ok_value  = 0;

	/*back up the data in flash before write value to flash*/

	xip_image_ok_value  = boot_read_xip_status(fap, XIP_IMAGE_OK_TYPE);
	if(xip_image_ok_value != XIP_ACTIVE)
	{
		if(xip_image_ok_value != XIP_UNACTIVE)
		{
			flash_area_erase_sector(boot_xip_status_addr_get(area_id),0,BOOT_XIP_FLAG_SIZE);
			ret = boot_write_xip_status(fap,XIP_MAGIC_TYPE,XIP_SET);
			if(ret != 0)
			{
				goto exit;
			}
			ret = boot_write_xip_status(fap,XIP_COPY_DONE_TYPE,XIP_SET);
			if(ret != 0)
			{
				goto exit;
			}
		}

		ret = boot_write_xip_status(fap,XIP_IMAGE_OK_TYPE,XIP_ACTIVE);
		if(ret != 0)
		{
			goto exit;
		}
	}

	return ret;
exit:
	return PSA_ERROR_GENERIC_ERROR;
}
psa_status_t fwu_bootloader_mark_image_accepted(
                                              bl_image_id_t bootloader_image_id)
{
    /* As RAM_LOAD and OVERWRITE_ONLY do not support image revert, the image
     * does not need to be confirmed explicitly in these two upgrade strategies.
     * Image revert is supported in SWAP upgrade strategy and DIRECT_XIP upgrade
     * strategy when MCUBOOT_DIRECT_XIP_REVERT is true. In these cases, the
     * image needs to be set as a permanent image explicitly. Then the accpeted
     * image can still be selected as the running image during next time reboot
     * up. Otherwise, the image will be reverted and the previous one will be
     * chosen as the running image.
     */
#if (defined(MCUBOOT_DIRECT_XIP) && defined(MCUBOOT_DIRECT_XIP_REVERT)) || \
    defined(MCUBOOT_SWAP_USING_SCRATCH) || defined(MCUBOOT_SWAP_USING_MOVE)
    uint8_t mcuboot_image_id = 0;

    if (convert_id_from_bl_to_mcuboot(bootloader_image_id,
                                    &mcuboot_image_id) != 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (boot_set_confirmed_multi(mcuboot_image_id) != 0) {
        return PSA_ERROR_GENERIC_ERROR;
    }
#else
    (void)bootloader_image_id;
#endif

#if (defined(CONFIG_DIRECT_XIP) && defined(CONFIG_XIP_NO_VERSION))
	int ret = 0;
    const struct flash_area *fap;

	/*Set excute area to XIP_ACTIVE*/
    uint32_t update_id = (flash_area_read_offset_enable());
    flash_area_open(update_id,&fap);
	ret = fwu_security_write_xip_active(fap,update_id);
	if(ret != 0)
	{
		goto exit;
	}
    flash_area_close(fap);

	/*Set non-excute area to XIP_BACK*/
    update_id ^= 1;
    flash_area_open(update_id,&fap);
    ret = boot_write_xip_status(fap,XIP_IMAGE_OK_TYPE,XIP_BACK);
	if(ret != 0)
	{
		goto exit;
	}
    flash_area_close(fap);

#endif
    return PSA_SUCCESS;
exit:
	return PSA_ERROR_GENERIC_ERROR;
}

psa_status_t fwu_bootloader_abort(bl_image_id_t bootloader_image_id)
{
    uint8_t mcuboot_image_id = 0;
    const struct flash_area *fap;
    uint8_t index;

    if (convert_id_from_bl_to_mcuboot(bootloader_image_id,
                                      &mcuboot_image_id) != 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    /* The image should already be added into the mcuboot_ctx. */
    if (get_flash_image_index(mcuboot_image_id, &index)) {
        fap = mcuboot_ctx[index].fap;
    } else {
        return PSA_ERROR_INVALID_ARGUMENT;
    }
    wdt_close();
    flash_area_erase(fap, 0, fap->fa_size);
    flash_area_close(fap);
    mcuboot_ctx[index].fap = NULL;
    mcuboot_ctx[index].loaded_size = 0;
    return PSA_SUCCESS;
}

#if 0//code size optimize
static psa_status_t util_img_hash(const struct flash_area *fap,
                                 size_t data_size,
                                 uint8_t *hash_result,
                                 size_t buf_size,
                                 size_t *hash_size)
{
    psa_hash_operation_t handle = psa_hash_operation_init();
    psa_status_t status;
    uint8_t tmpbuf[BOOT_TMPBUF_SZ];
    uint32_t tmp_buf_sz = BOOT_TMPBUF_SZ;
    uint32_t blk_sz;
    uint32_t off;

    /* Setup the hash object for the desired hash. */
    status = psa_hash_setup(&handle, PSA_ALG_SHA_256);
    if (status != PSA_SUCCESS) {
        return status;
    }

    for (off = 0; off < data_size; off += blk_sz) {
        blk_sz = data_size - off;
        if (blk_sz > tmp_buf_sz) {
            blk_sz = tmp_buf_sz;
        }

        if (flash_area_read_dbus(fap, off, tmpbuf, blk_sz)) {
            return PSA_ERROR_GENERIC_ERROR;
        }
        status = psa_hash_update(&handle, tmpbuf, blk_sz);
        if (status != PSA_SUCCESS) {
            return status;
        }
    }

    status = psa_hash_finish(&handle, hash_result, buf_size, hash_size);

    return status;
}

static psa_status_t get_secondary_image_info(uint8_t image_id,
                                             psa_image_info_t *info)
{
    const struct flash_area *fap = NULL;
    struct image_header hdr = {0};
    uint8_t hash[PSA_FWU_MAX_DIGEST_SIZE] = {0};
    size_t hash_size = 0;
    psa_status_t ret = PSA_SUCCESS;
    uint8_t index;
    size_t data_size;

    if (info == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if ((flash_area_open(FLASH_AREA_IMAGE_SECONDARY(image_id),
                            &fap)) != 0) {
        LOG_ERRFMT("TFM FWU: opening flash failed.\r\n");
        return PSA_ERROR_GENERIC_ERROR;
    }

    if (flash_area_read_dbus(fap, 0, &hdr, sizeof(hdr)) != 0) {
        flash_area_close(fap);
        LOG_ERRFMT("TFM FWU: reading flash failed.\r\n");
        return PSA_ERROR_GENERIC_ERROR;
    }


    /* Check if the image is in a FWU process. */
    if (get_flash_image_index(image_id, &index)) {

        /* Calculate hash on the downloaded data. */
        data_size = mcuboot_ctx[index].loaded_size;
    } else {
        /* Check if a valid image exists on the staging area.
         * If a valid image exists, the FWU partition has no information on the
         * image size downloaded as the image is copied from the running slot
         * during the reboot. So in this case when calculating the image hash,
         * the range starts from the image header till the end of the protected
         * TLV area.
         */
        if (hdr.ih_magic == IMAGE_MAGIC) {
            info->version.iv_major = hdr.ih_ver.iv_major;
            info->version.iv_minor = hdr.ih_ver.iv_minor;
            info->version.iv_revision = hdr.ih_ver.iv_revision;
            info->version.iv_build_num = hdr.ih_ver.iv_build_num;
            LOG_ERRFMT("version= %d., %d., %d.,+ %d\n\r",
                    info->version.iv_major,
                    info->version.iv_minor,
                    info->version.iv_revision,
                    info->version.iv_build_num);

            /* Calculate hash on from the image header to the protected TLV. */
            data_size = hdr.ih_hdr_size + hdr.ih_img_size +
                        hdr.ih_protect_tlv_size;
        } else {
            /* No image in the staging area. */
            return PSA_ERROR_DOES_NOT_EXIST;
        }
    }

    if (util_img_hash(fap, data_size, hash, (size_t)PSA_FWU_MAX_DIGEST_SIZE,
                      &hash_size) == PSA_SUCCESS) {
        tfm_memcpy(info->digest, hash, hash_size);
        ret = PSA_SUCCESS;
    } else {
        ret = PSA_ERROR_GENERIC_ERROR;
    }

    /* The actual image state will be filled in the tfm_fwu_req_mngr.c where
     * the image state is maintained.
     */
    info->state = PSA_IMAGE_UNDEFINED;
    flash_area_close(fap);
    return ret;
}

psa_status_t fwu_bootloader_get_image_info(bl_image_id_t bootloader_image_id,
                                  bool active_image,
                                  psa_image_info_t *info)
{
    struct image_version image_ver = { 0 };
    uint8_t mcuboot_image_id = 0;
#if !defined(MCUBOOT_DIRECT_XIP) && !defined(MCUBOOT_RAM_LOAD) && \
    !defined(MCUBOOT_OVERWRITE_ONLY)
    const struct flash_area *fap = NULL;
    uint8_t image_ok = BOOT_FLAG_UNSET;
#endif
    if (info == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    if (convert_id_from_bl_to_mcuboot(bootloader_image_id,
                                      &mcuboot_image_id) != 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    memset(info, 0, sizeof(psa_image_info_t));
    memset(info->digest, TFM_IMAGE_INFO_INVALID_DIGEST, sizeof(info->digest));

    if (active_image) {
    /* As DIRECT_XIP, RAM_LOAD and OVERWRITE_ONLY do not support image revert.
     * So the running image is in INSTALLED state in these three upgrade
     * strategies. In the SWAP case, the image_ok flag should be read to check
     * whether the running image has been confirmed as a pernament image.
     */
#if !defined(MCUBOOT_DIRECT_XIP) && !defined(MCUBOOT_RAM_LOAD) && \
    !defined(MCUBOOT_OVERWRITE_ONLY)
        if ((flash_area_open(FLASH_AREA_IMAGE_PRIMARY(mcuboot_image_id),
                            &fap)) != 0) {
            LOG_ERRFMT("TFM FWU: opening flash failed.\r\n");
            return PSA_ERROR_GENERIC_ERROR;
        }

        /* Get value of image-ok flag of the image to check whether application
         * itself is already confirmed.
         */
        if (boot_read_image_ok(fap, &image_ok) != 0) {
            return PSA_ERROR_GENERIC_ERROR;
        }
        if (image_ok == BOOT_FLAG_SET) {
            info->state = PSA_IMAGE_INSTALLED;
        } else {
            info->state = PSA_IMAGE_PENDING_INSTALL;
        }
#else
        info->state = PSA_IMAGE_INSTALLED;
#endif
        if (get_running_image_version(mcuboot_image_id,
                                      &image_ver) == PSA_SUCCESS) {
            info->version.iv_major = image_ver.iv_major;
            info->version.iv_minor = image_ver.iv_minor;
            info->version.iv_revision = image_ver.iv_revision;
            info->version.iv_build_num = image_ver.iv_build_num;

            /* The image in the primary slot is verified by the bootloader.
             * The image digest in the primary slot should not be exposed to
             * nonsecure.
             */
            return PSA_SUCCESS;
        } else {
            return PSA_ERROR_GENERIC_ERROR;
        }
    } else {
        return get_secondary_image_info(mcuboot_image_id, info);
    }
}
#endif
