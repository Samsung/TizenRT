/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "crypto.h"
#include "otp.h"
#include "boot_hal.h"
#include "uart_stdout.h"
#include "fih.h"
#include "util.h"
#include "log.h"
#include "image.h"
#include "region_defs.h"

extern uint32_t platform_code_is_bl1_2;

static fih_int image_hash_check(struct bl1_2_image_t *img)
{
    uint8_t computed_bl2_hash[BL2_HASH_SIZE];
    uint8_t stored_bl2_hash[BL2_HASH_SIZE];
    fih_int fih_rc = FIH_FAILURE;

    FIH_CALL(bl1_sha256_compute, fih_rc, (uint8_t *)&img->protected_values,
                                         sizeof(img->protected_values),
                                         computed_bl2_hash);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_RET(fih_rc);
    }

    FIH_CALL(bl1_otp_read_bl2_image_hash, fih_rc, stored_bl2_hash);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_RET(fih_rc);
    }

    FIH_CALL(bl_secure_memeql, fih_rc, computed_bl2_hash, stored_bl2_hash,
                                       BL2_HASH_SIZE);
    FIH_RET(fih_rc);
}

static fih_int is_image_security_counter_valid(struct bl1_2_image_t *img)
{
    uint32_t security_counter;
    fih_int fih_rc;

    FIH_CALL(bl1_otp_read_nv_counter, fih_rc, BL1_NV_COUNTER_ID_BL2_IMAGE,
                                          &security_counter);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_RET(FIH_FAILURE);
    }

    /* Encodes 0 to true and 1 to false, so the actual comparison is flipped */
    FIH_RET(
        fih_int_encode_zero_equality(security_counter
                                     > img->protected_values.security_counter));
}

static fih_int is_image_signature_valid(struct bl1_2_image_t *img)
{
    fih_int fih_rc = FIH_FAILURE;

#ifdef TFM_BL1_PQ_CRYPTO
    /* TODO */
#else
    FIH_CALL(image_hash_check, fih_rc, img);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_RET(FIH_FAILURE);
    }
#endif /* TFM_BL1_PQ_CRYPTO */

    FIH_RET(fih_rc);
}

fih_int validate_image_at_addr(struct bl1_2_image_t *image)
{
    fih_int fih_rc = FIH_FAILURE;

    FIH_CALL(is_image_signature_valid, fih_rc, image);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        BL1_LOG("[ERR] BL2 image signature failed to validate\r\n");
        FIH_RET(FIH_FAILURE);
    }
    FIH_CALL(is_image_security_counter_valid, fih_rc, image);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        BL1_LOG("[ERR] BL2 image security_counter failed to validate\r\n");
        FIH_RET(FIH_FAILURE);
    }

    /* TODO work out if the image actually boots before updating the counter */
    FIH_CALL(bl1_otp_write_nv_counter, fih_rc, BL1_NV_COUNTER_ID_BL2_IMAGE,
                                               image->protected_values.security_counter);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        BL1_LOG("[ERR] NV counter update failed\r\n");
        FIH_RET(FIH_FAILURE);
    }

    FIH_RET(FIH_SUCCESS);
}

fih_int copy_and_decrypt_image(uint32_t image_id)
{
    int rc;
#ifdef TFM_BL1_MEMORY_MAPPED_FLASH
    fih_int fih_rc;
#endif /* TFM_BL1_MEMORY_MAPPED_FLASH */
    struct bl1_2_image_t *image_to_decrypt;
    struct bl1_2_image_t *image_after_decrypt =
        (struct bl1_2_image_t *)BL2_IMAGE_START;

#ifdef TFM_BL1_MEMORY_MAPPED_FLASH
    /* If we have memory-mapped flash, we can do the decrypt directly from the
     * flash and output to the SRAM. This is significantly faster if the AES
     * invocation calls through to a crypto accelerator with a DMA, and slightly
     * faster otherwise.
     */
    image_to_decrypt = (struct bl1_2_image_t *)(FLASH_BASE_ADDRESS +
                       bl1_image_get_flash_offset(image_id));

    /* Copy everything that isn't encrypted, to prevent TOCTOU attacks and
     * simplify logic.
     */
    FIH_CALL(bl_secure_memcpy, fih_rc, image_after_decrypt,
                        image_to_decrypt,
                        sizeof(struct bl1_2_image_t) -
                        sizeof(image_after_decrypt->protected_values.encrypted_data));
#else
    /* If the flash isn't memory-mapped, defer to the flash driver to copy the
     * entire block in to SRAM. We'll then do the decrypt in-place.
     */
    bl1_image_copy_to_sram(image_id, (uint8_t *)BL2_IMAGE_START);
    image_to_decrypt = (struct bl1_2_image_t *)BL2_IMAGE_START;
#endif /* TFM_BL1_MEMORY_MAPPED_FLASH */

    rc = bl1_aes_256_ctr_decrypt(TFM_BL1_KEY_BL2_ENCRYPTION,
                        image_to_decrypt->header.ctr_iv,
                        (uint8_t *)&image_to_decrypt->protected_values.encrypted_data,
                        sizeof(image_after_decrypt->protected_values.encrypted_data),
                        (uint8_t *)&image_after_decrypt->protected_values.encrypted_data);
    if (rc) {
        FIH_RET(fih_int_encode_zero_equality(rc));
    }

    if (image_after_decrypt->protected_values.encrypted_data.decrypt_magic
            != BL1_2_IMAGE_DECRYPT_MAGIC_EXPECTED) {
        FIH_RET(FIH_FAILURE);
    }

    FIH_RET(FIH_SUCCESS);
}

static fih_int validate_image(uint32_t image_id)
{
    fih_int fih_rc = FIH_FAILURE;
    struct bl1_2_image_t *image;

    FIH_CALL(copy_and_decrypt_image, fih_rc, image_id);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        BL1_LOG("[ERR] BL2 image failed to decrypt\r\n");
        FIH_RET(FIH_FAILURE);
    }
    image = (struct bl1_2_image_t *)BL2_IMAGE_START;

    BL1_LOG("[INF] BL2 image decrypted successfully\r\n");

    FIH_CALL(validate_image_at_addr, fih_rc, image);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        BL1_LOG("[ERR] BL2 image failed to validate\r\n");
        FIH_RET(FIH_FAILURE);
    }

    BL1_LOG("[INF] BL2 image validated successfully\r\n");

    FIH_RET(FIH_SUCCESS);
}

int main(void)
{
    platform_code_is_bl1_2 = 1;
    fih_int fih_rc = FIH_FAILURE;

    fih_rc = fih_int_encode_zero_equality(boot_platform_init());
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_PANIC;
    }
    BL1_LOG("[INF] starting TF-M bl1_2\r\n");

    fih_rc = fih_int_encode_zero_equality(boot_platform_post_init());
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_PANIC;
    }

#ifdef TEST_BL1_2
    run_bl1_2_testsuite();
#endif /* TEST_BL1_2 */

    BL1_LOG("[INF] Attempting to boot image 0\r\n");
    FIH_CALL(validate_image, fih_rc, 0);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        BL1_LOG("[INF] Attempting to boot image 1\r\n");
        FIH_CALL(validate_image, fih_rc, 1);
        if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
            FIH_PANIC;
        }
    }

    BL1_LOG("[INF] Jumping to BL2\r\n");
    boot_platform_quit((struct boot_arm_vector_table *)BL2_CODE_START);

    FIH_PANIC;
}
