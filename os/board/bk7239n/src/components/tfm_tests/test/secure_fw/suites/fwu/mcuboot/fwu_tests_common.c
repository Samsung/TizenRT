/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#if DOMAIN_NS == 1
#include <string.h>
#else
#include "tfm_memory_utils.h"
#endif
#include "fwu_tests_common.h"
#include "region_defs.h"

/* Size of image header (bytes). It takes two bytes in the image header. */
#define IMAGE_HEADER_SIZE        BL2_HEADER_SIZE
#define IMAGE_HEADER_SIZE_LOW    (uint8_t)(IMAGE_HEADER_SIZE & 0x000000FF)
#define IMAGE_HEADER_SIZE_HIGH  \
            (uint8_t)((IMAGE_HEADER_SIZE & 0x0000FF00) >> 8)

/* Size of the secure image itself. Does not include header. It takes four bytes
 * in the image header.
 */
#define SECURE_IMAGE_SIZE         IMAGE_S_CODE_SIZE
#define SECURE_IMAGE_SIZE_BYTE_0  \
            (uint8_t)(SECURE_IMAGE_SIZE & 0x000000FF)
#define SECURE_IMAGE_SIZE_BYTE_1  \
            (uint8_t)((SECURE_IMAGE_SIZE & 0x0000FF00) >> 8)
#define SECURE_IMAGE_SIZE_BYTE_2  \
            (uint8_t)((SECURE_IMAGE_SIZE & 0x00FF0000) >> 16)
#define SECURE_IMAGE_SIZE_BYTE_3  \
            (uint8_t)((SECURE_IMAGE_SIZE & 0xFF000000) >> 24)

/* The _off value of the protected TLV area in secure image. */
#define PROTECTED_TLV_OFF_SECURE (IMAGE_HEADER_SIZE + SECURE_IMAGE_SIZE)

/* Size of the nonsecure image itself. Does not include header. It takes four
 * bytes in the image header.
 */
#define NONSECURE_IMAGE_SIZE         IMAGE_NS_CODE_SIZE
#define NONSECURE_IMAGE_SIZE_BYTE_0  \
            (uint8_t)(NONSECURE_IMAGE_SIZE & 0x000000FF)
#define NONSECURE_IMAGE_SIZE_BYTE_1  \
            (uint8_t)((NONSECURE_IMAGE_SIZE & 0x0000FF00) >> 8)
#define NONSECURE_IMAGE_SIZE_BYTE_2  \
            (uint8_t)((NONSECURE_IMAGE_SIZE & 0x00FF0000) >> 16)
#define NONSECURE_IMAGE_SIZE_BYTE_3  \
            (uint8_t)((NONSECURE_IMAGE_SIZE & 0xFF000000) >> 24)

/* The _off value of the protected TLV area in nonsecure image. */
#define PROTECTED_TLV_OFF_NONSECURE (IMAGE_HEADER_SIZE + NONSECURE_IMAGE_SIZE)

#if (MCUBOOT_IMAGE_NUMBER > 1)
#define image_type_test FWU_IMAGE_TYPE_SECURE
#define image_type_dependency FWU_IMAGE_TYPE_NONSECURE
/* The off of the first byte of depdency version in the TLV area of secure
 * image.
 */
#define TLV_DEPENDENCY_VERSION_OFF_SECURE           115

/* The off of the first byte of depdency version in the TLV area of nonsecure
 * image.
 */
#define TLV_DEPENDENCY_VERSION_OFF_NONSECURE        116

/* The off of the image version in the image header. */
#define HEADER_IMAGE_VERSION_OFF                    20
#else
#define image_type_test FWU_IMAGE_TYPE_FULL
#endif

/* Version: Major */
#define   IMAGE_VERSION_FOR_TEST_MAJOR              0x09

/* Version: Minor */
#define   IMAGE_VERSION_FOR_TEST_MINOR              0x09

/* Version: Low byte o f revision */
#define   IMAGE_VERSION_FOR_TEST_REVISION_LOW       0x09

/* Version: High byte o f revision */
#define   IMAGE_VERSION_FOR_TEST_REVISION_HIGH      0x00

static psa_image_id_t test_image = \
                (psa_image_id_t)FWU_CALCULATE_IMAGE_ID(FWU_IMAGE_ID_SLOT_STAGE,
                                                       image_type_test,
                                                       0);
#if (MCUBOOT_IMAGE_NUMBER > 1)
static psa_image_id_t dependency_image = \
                (psa_image_id_t)FWU_CALCULATE_IMAGE_ID(FWU_IMAGE_ID_SLOT_STAGE,
                                                       image_type_dependency,
                                                       0);
#endif

/* The secure image header(from image header magic to image version). The image
 * version is 0.0.0 + 0.
 */
static const uint8_t header_test_image_version_zero[28] = \
                        { 0x3d, 0xb8, 0xf3, 0x96, /* IMAGE_MAGIC */ \
                          0x00, 0x00, 0x00, 0x00,  /* LoadAddr */  \
                          /* Image Header size */
                          IMAGE_HEADER_SIZE_LOW, IMAGE_HEADER_SIZE_HIGH, \
                          0x7B, 0x00, /* Protected TLV size */  \
                          /* Image size */
                          SECURE_IMAGE_SIZE_BYTE_0, SECURE_IMAGE_SIZE_BYTE_1, \
                          SECURE_IMAGE_SIZE_BYTE_2, SECURE_IMAGE_SIZE_BYTE_3, \
                          0x00, 0x00, 0x00, 0x00, /* Flags */  \
                          0x00, /* Version: Major */  \
                          0x00, /* Version: Minor */  \
                          0x00, 0x00, /* Version: Revision */  \
                          0x00, 0x00, 0x00, 0x00, /* Version: Build number */  \
                        };

/* The protected TLV data + part of the TLV data of the secure image.
 * The dependency nonsecure image and MIN_VER:0.0.0+0.
 */
static const uint8_t tlv_dep_version_zero[128] = \
                        { 0x08, 0x69, /* IMAGE_TLV_PROT_INFO_MAGIC */  \
                          0x7B, 0x00, /* The size of protected TLV area */
                          /* Secure count: */
                          0x50, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03, 0x01,  \
                          /* Boot record: */
                          0x60, 0x00, 0x5b, 0x00, 0xa5, 0x01, 0x63, 0x53,  \
                          0x50, 0x45, 0x04, 0x65, 0x31, 0x2e, 0x33, 0x2e,  \
                          0x30, 0x05, 0x58, 0x20, 0xfc, 0x57, 0x01, 0xdc,  \
                          0x61, 0x35, 0xe1, 0x32, 0x38, 0x47, 0xbd, 0xc4,  \
                          0x0f, 0x04, 0xd2, 0xe5, 0xbe, 0xe5, 0x83, 0x3b,  \
                          0x23, 0xc2, 0x9f, 0x93, 0x59, 0x3d, 0x00, 0x01,  \
                          0x8c, 0xfa, 0x99, 0x94, 0x06, 0x66, 0x53, 0x48,  \
                          0x41, 0x32, 0x35, 0x36, 0x02, 0x58, 0x20, 0x00,  \
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        \
                          /* DEPENDENCY: nonsecure, MIN_VER:0.0.0+0*/
                          0x40, 0x00, 0x0c, 0x00, 0x01, 0x00, 0x00, 0x00,  \
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                          /* custom_tlvs: None */
                          0x07, 0x69, /* IMAGE_TLV_INFO_MAGIC */  \
                          0x50, 0x01, 0x10 /* Part of the SHA256 */  \
                        };

#if (MCUBOOT_IMAGE_NUMBER > 1)
/* Compare image version numbers not including the build number.
 * Return 1 if image_ver_1 is greater than image_ver_2.
 * Return -1 if image_ver_1 is less than image_ver_2.
 * Return 0 if image_ver_1 equal to image_ver_2.
 */
static int version_compare(const psa_image_version_t *image_ver_1,
                           const psa_image_version_t *image_ver_2)
{
    if (image_ver_1->iv_major > image_ver_2->iv_major) {
        return 1;
    }
    if (image_ver_1->iv_major < image_ver_2->iv_major) {
        return -1;
    }
    /* The major version numbers are equal, continue comparison. */
    if (image_ver_1->iv_minor > image_ver_2->iv_minor) {
        return 1;
    }
    if (image_ver_1->iv_minor < image_ver_2->iv_minor) {
        return -1;
    }
    /* The minor version numbers are equal, continue comparison. */
    if (image_ver_1->iv_revision > image_ver_2->iv_revision) {
        return 1;
    }
    if (image_ver_1->iv_revision < image_ver_2->iv_revision) {
        return -1;
    }
    return 0;
}
#endif

void tfm_fwu_test_common_001(struct test_result_t *ret)
{
    psa_status_t status;
    psa_image_id_t dependency_uuid;
    psa_image_version_t dependency_version;
    psa_image_info_t info;

    /* The image header(from header magic to image version) is downloaded
     * into the target device by this psa_fwu_write operation.
     */
    status = psa_fwu_write(test_image,
                           0,
                           header_test_image_version_zero,
                           sizeof(header_test_image_version_zero));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("psa_fwu_write should not fail when writing the image header data.");
        return;
    }

    /* Query the staging image. */
    status = psa_fwu_query(test_image, &info);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Query should success");
        return;
    }

    /* Check the image state. */
    if (info.state != PSA_IMAGE_CANDIDATE) {
        TEST_FAIL("Image should be in CANDIDATE state after succesfull write");
        return;
    }

    /* Write the protected TLV + IMAGE_TLV_INFO_MAGIC + part of the SHA256.
     * The image dependency information is downloaded into the target device
     * by this psa_fwu_write operation.
     */
    status = psa_fwu_write(test_image,
                           (size_t)PROTECTED_TLV_OFF_SECURE,
                           tlv_dep_version_zero,
                           sizeof(tlv_dep_version_zero));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("psa_fwu_write should not fail when writing the protected TLV data.");
        return;
    }

    status = psa_fwu_install(test_image,
                             &dependency_uuid,
                             &dependency_version);

    /* Dependency check in multiple images:
     * dependency image ID: nonsecure image.
     * dependency image version: 0.0.0 + 0.
     * Currently running nonsecure image version: 0.0.0 + 0
     * So the dependency check should pass.
     * In the current implementation, image verification is deferred to
     * reboot, so PSA_SUCCESS_REBOOT is returned when success.
     */
    if (status != PSA_SUCCESS_REBOOT) {
        TEST_FAIL("Install should not fail when the image dependency check should pass.");
        return;
    }

    /* Query the staging image. */
    if (psa_fwu_query(test_image, &info) != PSA_SUCCESS) {
        TEST_FAIL("Query should success");
        return;
    }

    /* Check the image state. */
    if (info.state != PSA_IMAGE_REBOOT_NEEDED) {
        TEST_FAIL("PSA_SUCCESS_REBOOT is retuned in install and image should be in REBOOT_NEEDED state. ");
        return;
    }

    /* Abort the fwu process. */
    status = psa_fwu_abort(test_image);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Abort should not fail after install");
        return;
    }

    /* Query the staging image. */
    status = psa_fwu_query(test_image, &info);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Query should success");
        return;
    }

    /* Check the image state. */
    if (info.state != PSA_IMAGE_UNDEFINED) {
        TEST_FAIL("Image should be in UNDEFINED state after abort");
        return;
    }

    /* The image header(from header magic to image version) is downloaded
     * into the target device again by this psa_fwu_write operation.
     */
    status = psa_fwu_write(test_image,
                           0,
                           header_test_image_version_zero,
                           sizeof(header_test_image_version_zero));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Write should not fail with block_offset 0 after abort");
        return;
    }

    /* Query the staging image. */
    status = psa_fwu_query(test_image, &info);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Query should success");
        return;
    }

    /* Check the image state. */
    if (info.state != PSA_IMAGE_CANDIDATE) {
        TEST_FAIL("Image should be in CANDIDATE state after succesfull write");
        return;
    }

    /* Abort the fwu process. */
    status = psa_fwu_abort(test_image);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Abort should not fail after write");
        return;
    }

    ret->val = TEST_PASSED;
}

#if defined TFM_FWU_TEST_WRITE_WITH_NULL
/* The TF-M core will reboot the device if the parameter check fails
 * in partition request in libaray mode.
 */
void tfm_fwu_test_common_002(struct test_result_t *ret)
{
    psa_status_t status;

    /* Write the data block NULL. */
    status = psa_fwu_write(test_image,
                           0,
                           NULL,
                           sizeof(header_test_image_version_zero));
    if (status != PSA_ERROR_INVALID_ARGUMENT) {
        TEST_FAIL("Write should fail with data block NULL");
        return;
    }

    ret->val = TEST_PASSED;
}
#endif

void tfm_fwu_test_common_003(struct test_result_t *ret)
{
    psa_status_t status;

    /* Write the data block with block_offset + len overflow. */
    status = psa_fwu_write(test_image,
                           (size_t)0xFFFFFFFF,
                           header_test_image_version_zero,
                           0x10);
    if (status == PSA_SUCCESS) {
        TEST_FAIL("Write should fail with len + image_offset overflow");
        return;
    }

    ret->val = TEST_PASSED;
}

void tfm_fwu_test_common_004(struct test_result_t *ret)
{
    psa_status_t status;
    psa_image_version_t dependency_version;

    /* The image header(from header magic to image version) is downloaded
     * into the target device by this psa_fwu_write operation.
     */
    status = psa_fwu_write(test_image,
                           0,
                           header_test_image_version_zero,
                           sizeof(header_test_image_version_zero));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Write should not fail with block_offset 0");
        return;
    }

    /* Write the protected TLV + IMAGE_TLV_INFO_MAGIC + part of the SHA256.
     * The image dependency information is downloaded into the target device
     * by this psa_fwu_write operation.
     */
    status = psa_fwu_write(test_image,
                           (size_t)PROTECTED_TLV_OFF_SECURE,
                           tlv_dep_version_zero,
                           sizeof(tlv_dep_version_zero));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("psa_fwu_write should not fail when writing the protected TLV data.");
        return;
    }

    status = psa_fwu_install(test_image, NULL, &dependency_version);

    /* If PSA_ERROR_DEPENDENCY_NEEDED is returned, the dependency should be
     * filled in the dependency argument.PSA_ERROR_INVALID_ARGUMENT
     */
    if (status == PSA_ERROR_DEPENDENCY_NEEDED) {
        TEST_FAIL("Install should not return PSA_ERROR_DEPENDENCY_NEEDED with NULL dependency");
        return;
    }

    /* Abort the fwu process. */
    status = psa_fwu_abort(test_image);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Abort should not fail after install");
        return;
    }

    ret->val = TEST_PASSED;
}

void tfm_fwu_test_common_005(struct test_result_t *ret)
{
    psa_status_t status;
    psa_image_id_t dependency_uuid;

    /* The image header(from header magic to image version) is downloaded
     * into the target device by this psa_fwu_write operation.
     */
    status = psa_fwu_write(test_image,
                           0,
                           header_test_image_version_zero,
                           sizeof(header_test_image_version_zero));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Write should not fail with block_offset 0");
        return;
    }

    /* Write the protected TLV + IMAGE_TLV_INFO_MAGIC + part of the SHA256.
     * The image dependency information is downloaded into the target device
     * by this psa_fwu_write operation.
     */
    status = psa_fwu_write(test_image,
                           (size_t)PROTECTED_TLV_OFF_SECURE,
                           tlv_dep_version_zero,
                           sizeof(tlv_dep_version_zero));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("psa_fwu_write should not fail when writing the protected TLV data.");
        return;
    }

    status = psa_fwu_install(test_image, &dependency_uuid, NULL);

    /* If PSA_ERROR_DEPENDENCY_NEEDED is returned, the dependency should be
     * filled in the dependency argument.
     */
    if (status == PSA_ERROR_DEPENDENCY_NEEDED) {
        TEST_FAIL("Install should not return PSA_ERROR_DEPENDENCY_NEEDED with NULL dependency version");
        return;
    }

    /* Abort the fwu process. */
    status = psa_fwu_abort(test_image);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Abort should not fail after install");
        return;
    }

    ret->val = TEST_PASSED;
}

void tfm_fwu_test_common_006(struct test_result_t *ret)
{
    psa_status_t status;
    psa_image_id_t dependency_uuid;
    psa_image_version_t dependency_version;

    status = psa_fwu_install(test_image,
                             &dependency_uuid,
                             &dependency_version);

    /* Install will always fail if write is not called before. */
    if (status != PSA_ERROR_INVALID_ARGUMENT) {
        TEST_FAIL("Install should not fail after write");
        return;
    }

    ret->val = TEST_PASSED;
}

void tfm_fwu_test_common_007(struct test_result_t *ret)
{
    psa_status_t status;
    psa_image_id_t dependency_uuid;
    psa_image_version_t dependency_version;

    /* Write the data block with block_offset 0. */
    status = psa_fwu_write(test_image,
                           0,
                           header_test_image_version_zero,
                           sizeof(header_test_image_version_zero));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Write should not fail with block_offset 0");
        return;
    }

    /* Abort the fwu process. */
    status = psa_fwu_abort(test_image);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Abort should not fail after install");
        return;
    }

    status = psa_fwu_install(test_image, &dependency_uuid, &dependency_version);

    /* Install should fail as the fwu process has been aborted. */
    if (status != PSA_ERROR_INVALID_ARGUMENT) {
        TEST_FAIL("Install should fail after abort");
        return;
    }

    ret->val = TEST_PASSED;
}

void tfm_fwu_test_common_008(struct test_result_t *ret)
{
    psa_status_t status;

    status = psa_fwu_abort(test_image);

    /* Install will always fail if write is not called before. */
    if (status != PSA_ERROR_INVALID_ARGUMENT) {
        TEST_FAIL("Install should fail with no write before");
        return;
    }

    ret->val = TEST_PASSED;
}

void tfm_fwu_test_common_009(struct test_result_t *ret)
{
    psa_status_t status;
    psa_image_info_t info = { 0 };
    psa_image_id_t image_id = FWU_CALCULATE_IMAGE_ID(FWU_IMAGE_ID_SLOT_ACTIVE,
                                                     image_type_test,
                                                     0);

    /* Query the running image. */
    status = psa_fwu_query(image_id, &info);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Query should success");
        return;
    }

    if ((info.state != PSA_IMAGE_INSTALLED) &&
       (info.state != PSA_IMAGE_PENDING_INSTALL)) {
        TEST_FAIL("The active image should be in INSTALLED or PENDING_INSTALL state");
        return;
    }
    ret->val = TEST_PASSED;
}

#if defined TFM_FWU_TEST_QUERY_WITH_NULL
void tfm_fwu_test_common_010(struct test_result_t *ret)
{
    psa_status_t status;

    /* Query the running image. */
    status = psa_fwu_query(image_id, NULL);
    if (status == PSA_SUCCESS) {
        TEST_FAIL("Query should fail with NULL info");
        return;
    }

    ret->val = TEST_PASSED;
}
#endif

void tfm_fwu_test_common_011(struct test_result_t *ret)
{
    psa_status_t status;
    psa_image_info_t info = { 0 };

    psa_image_id_t image_id = FWU_CALCULATE_IMAGE_ID(3,
                                                     image_type_test,
                                                     0);

    /* Query the running image. */
    status = psa_fwu_query(image_id, &info);
    if (status != PSA_ERROR_INVALID_ARGUMENT) {
        TEST_FAIL("Query should fail with NULL info");
        return;
    }

    ret->val = TEST_PASSED;
}

void tfm_fwu_test_common_012(struct test_result_t *ret)
{
    psa_status_t status;
    psa_image_info_t info = { 0 };
    psa_image_id_t running_image = \
                (psa_image_id_t)FWU_CALCULATE_IMAGE_ID(FWU_IMAGE_ID_SLOT_ACTIVE,
                                                       image_type_test,
                                                       0);

    /* Accept the running image. */
    status = psa_fwu_accept(running_image);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Accept should not fail");
        return;
    }

    /* Query the running image. */
    status = psa_fwu_query(running_image, &info);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Query should success");
        return;
    }

    if (info.state != PSA_IMAGE_INSTALLED) {
        TEST_FAIL("The active image should be in INSTALLED state");
        return;
    }

    ret->val = TEST_PASSED;
}

#ifdef TFM_FWU_TEST_REQUEST_REBOOT
void tfm_fwu_test_common_013(struct test_result_t *ret)
{
    /* Request reboot. */
    psa_fwu_request_reboot();
    TEST_FAIL("Request reboot should not fail");

    ret->val = TEST_FAILED;
}
#endif

#if (MCUBOOT_IMAGE_NUMBER > 1)
void tfm_fwu_test_common_014(struct test_result_t *ret)
{
    psa_status_t status;
    psa_image_id_t dependency_uuid;
    psa_image_version_t dependency_version;
    psa_image_info_t info;
    const uint8_t header_test_image_version_not_zero[28] = \
                        { 0x3d, 0xb8, 0xf3, 0x96, /* IMAGE_MAGIC */ \
                          0x00, 0x00, 0x00, 0x00,  /* LoadAddr */  \
                          /* Image Header size */  \
                          IMAGE_HEADER_SIZE_LOW, IMAGE_HEADER_SIZE_HIGH,  \
                          0x7B, 0x00, /* Protected TLV size */  \
                          /* Image size */
                          SECURE_IMAGE_SIZE_BYTE_0,  \
                          SECURE_IMAGE_SIZE_BYTE_1,  \
                          SECURE_IMAGE_SIZE_BYTE_2,  \
                          SECURE_IMAGE_SIZE_BYTE_3,  \
                          0x00, 0x00, 0x00, 0x00, /* Flags */  \
                          IMAGE_VERSION_FOR_TEST_MAJOR, /* Version: Major */  \
                          IMAGE_VERSION_FOR_TEST_MINOR, /* Version: Minor */  \
                          /* Version: Low byte of revision */
                          IMAGE_VERSION_FOR_TEST_REVISION_LOW, \
                          /* Version: High byte of revision */
                          IMAGE_VERSION_FOR_TEST_REVISION_HIGH,  \
                          0x00, 0x00, 0x00, 0x00, /* Version: Build number */  \
                        };

    /* The protected TLV data + part of the TLV data of the secure image. */
    const uint8_t tlv_data_secure_image[128] = \
                        { 0x08, 0x69, /* IMAGE_TLV_PROT_INFO_MAGIC */  \
                          0x7B, 0x00, /* The size of protected TLV area */
                          /* Secure count: */
                          0x50, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03, 0x01,  \
                          /* Boot record: */
                          0x60, 0x00, 0x5b, 0x00, 0xa5, 0x01, 0x63, 0x53,  \
                          0x50, 0x45, 0x04, 0x65, 0x31, 0x2e, 0x33, 0x2e,  \
                          0x30, 0x05, 0x58, 0x20, 0xfc, 0x57, 0x01, 0xdc,  \
                          0x61, 0x35, 0xe1, 0x32, 0x38, 0x47, 0xbd, 0xc4,  \
                          0x0f, 0x04, 0xd2, 0xe5, 0xbe, 0xe5, 0x83, 0x3b,  \
                          0x23, 0xc2, 0x9f, 0x93, 0x59, 0x3d, 0x00, 0x01,  \
                          0x8c, 0xfa, 0x99, 0x94, 0x06, 0x66, 0x53, 0x48,  \
                          0x41, 0x32, 0x35, 0x36, 0x02, 0x58, 0x20, 0x00,  \
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                          /* DEPENDENCY: nonsecure, MIN_VER:0.0.0+0 */
                          0x40, 0x00, 0x0c, 0x00, 0x01, 0x00, 0x00, 0x00,  \
                          0x00, /* Version: Major */  \
                          0x00, /* Version: Minor */  \
                          /* Version: Low byte of revision */
                          0x00, \
                          /* Version: High byte of revision */
                          0x00,  \
                          0x00, 0x00, 0x00, 0x00, /* Version: Build number */  \
                          /* custom_tlvs: None */
                          0x07, 0x69, /* IMAGE_TLV_INFO_MAGIC */  \
                          0x50, 0x01, 0x10 /* Part of the SHA256 */  \
                        };

    /* The image header(from header magic to image version) is downloaded
     * into the target device by this psa_fwu_write operation.
     */
    status = psa_fwu_write(test_image,
                           0,
                           header_test_image_version_not_zero,
                           sizeof(header_test_image_version_not_zero));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("psa_fwu_write should not fail when writing the image header data.");
        return;
    }

    /* Query the staging image. */
    status = psa_fwu_query(test_image, &info);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Query should success");
        return;
    }

    /* Check the image state. */
    if (info.state != PSA_IMAGE_CANDIDATE) {
        TEST_FAIL("Image should be in CANDIDATE state after succesfull write");
        return;
    }

    /* Write the protected TLV + IMAGE_TLV_INFO_MAGIC + part of the SHA256.
     * The image dependency information is downloaded into the target device
     * by this psa_fwu_write operation.
     */
    /* The required min_version of dependency is 0.0.0. */
    status = psa_fwu_write(test_image,
                           (size_t)PROTECTED_TLV_OFF_SECURE,
                           tlv_data_secure_image,
                           sizeof(tlv_data_secure_image));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("psa_fwu_write should not fail when writing the protected TLV data.");
        return;
    }

    status = psa_fwu_install(test_image,
                             &dependency_uuid,
                             &dependency_version);

    /* Dependency check in multiple images:
     * dependency image ID: nonsecure image.
     * Currently running nonsecure image version should always equal or greater
     * than the required 0.0.0.
     * So the dependency check should pass.
     * PSA_SUCCESS_REBOOT should be returned.
     */
    if (status != PSA_SUCCESS_REBOOT) {
        TEST_FAIL("Install PSA_SUCCESS_REBOOT should be returned when the image dependency check pass.");
        return;
    }

    /* Test clean up: abort the fwu process of the test image. */
    status = psa_fwu_abort(test_image);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Abort should not fail after install");
        return;
    }
    ret->val = TEST_PASSED;
}

void tfm_fwu_test_common_015(struct test_result_t *ret)
{
    psa_status_t status;
    psa_image_id_t dependency_uuid;
    psa_image_version_t dependency_version;
    psa_image_info_t info;
    psa_image_id_t dependency_uuid_temp;
    psa_image_version_t dependency_version_temp;
    psa_image_id_t active_dependency_image;
    psa_image_version_t dep_version_in_tlv = {0, 0, 0, 0};
    const uint8_t header_test_image_version_not_zero[28] = \
                        { 0x3d, 0xb8, 0xf3, 0x96, /* IMAGE_MAGIC */ \
                          0x00, 0x00, 0x00, 0x00,  /* LoadAddr */  \
                          /* Image Header size */  \
                          IMAGE_HEADER_SIZE_LOW, IMAGE_HEADER_SIZE_HIGH,  \
                          0x7B, 0x00, /* Protected TLV size */  \
                          /* Image size */
                          SECURE_IMAGE_SIZE_BYTE_0,  \
                          SECURE_IMAGE_SIZE_BYTE_1,  \
                          SECURE_IMAGE_SIZE_BYTE_2,  \
                          SECURE_IMAGE_SIZE_BYTE_3,  \
                          0x00, 0x00, 0x00, 0x00, /* Flags */  \
                          IMAGE_VERSION_FOR_TEST_MAJOR, /* Version: Major */  \
                          IMAGE_VERSION_FOR_TEST_MINOR, /* Version: Minor */  \
                          /* Version: Low byte of revision */
                          IMAGE_VERSION_FOR_TEST_REVISION_LOW, \
                          /* Version: High byte of revision */
                          IMAGE_VERSION_FOR_TEST_REVISION_HIGH,  \
                          0x00, 0x00, 0x00, 0x00, /* Version: Build number */  \
                        };

    /* The nonsecure image header(from image header magic to image version). */
    uint8_t header_dep_image_version_not_zero[28] = \
                        { 0x3d, 0xb8, 0xf3, 0x96, /* IMAGE_MAGIC */ \
                          0x00, 0x00, 0x00, 0x00,  /* LoadAddr */  \
                          /* Image Header size */  \
                          IMAGE_HEADER_SIZE_LOW, IMAGE_HEADER_SIZE_HIGH,  \
                          0x7C, 0x00, /* Protected TLV size */  \
                          /* Image size */
                          NONSECURE_IMAGE_SIZE_BYTE_0,  \
                          NONSECURE_IMAGE_SIZE_BYTE_1,  \
                          NONSECURE_IMAGE_SIZE_BYTE_2,  \
                          NONSECURE_IMAGE_SIZE_BYTE_3,  \
                          0x00, 0x00, 0x00, 0x00, /* Flags */  \
                          0x00, /* Version: Major */  \
                          0x00, /* Version: Minor */  \
                          /* Version: Low byte of revision */
                          0x00, \
                          /* Version: High byte of revision */
                          0x00,  \
                          0x00, 0x00, 0x00, 0x00, /* Version: Build number */  \
                        };

    /* The protected TLV data + part of the TLV data of the secure image. */
    uint8_t tlv_data_nonsecure_image[128] = \
                        { 0x08, 0x69,  /* IMAGE_TLV_PROT_INFO_MAGIC */  \
                          0x7c, 0x00,  /* The size of protected TLV area */  \
                          /* Secure count: */
                          0x50, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                          /* Boot record: */
                          0x60, 0x00, 0x5c, 0x00, 0xa5, 0x01, 0x64, 0x4e,  \
                          0x53, 0x50, 0x45, 0x04, 0x65, 0x30, 0x2e, 0x30,  \
                          0x2e, 0x30, 0x05, 0x58, 0x20, 0xe1, 0x80, 0x15,  \
                          0x99, 0x3d, 0x6d, 0x27, 0x60, 0xb4, 0x99, 0x27,  \
                          0x4b, 0xae, 0xf2, 0x64, 0xb8, 0x3a, 0xf2, 0x29,  \
                          0xe9, 0xa7, 0x85, 0xf3, 0xd5, 0xbf, 0x00, 0xb9,  \
                          0xd3, 0x2c, 0x1f, 0x03, 0x96, 0x06, 0x66, 0x53,  \
                          0x48, 0x41, 0x32, 0x35, 0x36, 0x02, 0x58, 0x20,  \
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                          /* DEPENDENCY: secure, MIN_VER: */
                          0x40, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                          IMAGE_VERSION_FOR_TEST_MAJOR, /* Version: Major */  \
                          IMAGE_VERSION_FOR_TEST_MINOR, /* Version: Minor */  \
                          /* Version: Low byte of revision */
                          IMAGE_VERSION_FOR_TEST_REVISION_LOW, \
                          /* Version: High byte of revision */
                          IMAGE_VERSION_FOR_TEST_REVISION_HIGH,  \
                          0x00, 0x00, 0x00, 0x00, /* Version: Build number */  \
                          /* custom_tlvs: None */
                          0x07, 0x69, /* IMAGE_TLV_INFO_MAGIC */  \
                          0x50, 0x01  /* Part of the SHA256 */  \
                        };

    /* The image header(from header magic to image version) is downloaded
     * into the target device by this psa_fwu_write operation.
     */
    status = psa_fwu_write(test_image,
                           0,
                           header_test_image_version_not_zero,
                           sizeof(header_test_image_version_not_zero));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("psa_fwu_write should not fail when writing the image header data.");
        return;
    }

    /* Query the staging image. */
    status = psa_fwu_query(test_image, &info);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Query should success");
        return;
    }

    /* Check the image state. */
    if (info.state != PSA_IMAGE_CANDIDATE) {
        TEST_FAIL("Image should be in CANDIDATE state after succesfull write");
        return;
    }

    /* Write the protected TLV + IMAGE_TLV_INFO_MAGIC + part of the SHA256.
     * The image dependency information is downloaded into the target device
     * by this psa_fwu_write operation.
     */
    /* The protected TLV data + part of the TLV data of the secure image.
     */
    uint8_t tlv_data_secure_image[128] = \
                        { 0x08, 0x69, /* IMAGE_TLV_PROT_INFO_MAGIC */  \
                          0x7B, 0x00, /* The size of protected TLV area */
                          /* Secure count: */
                          0x50, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03, 0x01,  \
                          /* Boot record: */
                          0x60, 0x00, 0x5b, 0x00, 0xa5, 0x01, 0x63, 0x53,  \
                          0x50, 0x45, 0x04, 0x65, 0x31, 0x2e, 0x33, 0x2e,  \
                          0x30, 0x05, 0x58, 0x20, 0xfc, 0x57, 0x01, 0xdc,  \
                          0x61, 0x35, 0xe1, 0x32, 0x38, 0x47, 0xbd, 0xc4,  \
                          0x0f, 0x04, 0xd2, 0xe5, 0xbe, 0xe5, 0x83, 0x3b,  \
                          0x23, 0xc2, 0x9f, 0x93, 0x59, 0x3d, 0x00, 0x01,  \
                          0x8c, 0xfa, 0x99, 0x94, 0x06, 0x66, 0x53, 0x48,  \
                          0x41, 0x32, 0x35, 0x36, 0x02, 0x58, 0x20, 0x00,  \
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
                          /* DEPENDENCY: nonsecure, MIN_VER:0.0.0+0 */
                          0x40, 0x00, 0x0c, 0x00, 0x01, 0x00, 0x00, 0x00,  \
                          0x00, /* Version: Major */  \
                          0x00, /* Version: Minor */  \
                          /* Version: Low byte of revision */
                          0x00, \
                          /* Version: High byte of revision */
                          0x00,  \
                          0x00, 0x00, 0x00, 0x00, /* Version: Build number */  \
                          /* custom_tlvs: None */
                          0x07, 0x69, /* IMAGE_TLV_INFO_MAGIC */  \
                          0x50, 0x01, 0x10 /* Part of the SHA256 */  \
                        };

    /* Query the active dependency image. */
    active_dependency_image = FWU_CALCULATE_IMAGE_ID(
                                        FWU_IMAGE_ID_SLOT_ACTIVE,
                                        image_type_dependency,
                                        0);
    if (psa_fwu_query(active_dependency_image, &info) != PSA_SUCCESS) {
        TEST_FAIL("Query should success");
        return;
    }

    /* The required image version is greater than the running. */
    dep_version_in_tlv.iv_major = info.version.iv_major + 1;

    /* Update the version bytes of the TLV data. */
    memcpy(tlv_data_secure_image + TLV_DEPENDENCY_VERSION_OFF_SECURE,
           &dep_version_in_tlv,
           sizeof(dep_version_in_tlv));
    status = psa_fwu_write(test_image,
                           (size_t)PROTECTED_TLV_OFF_SECURE,
                           tlv_data_secure_image,
                           sizeof(tlv_data_secure_image));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("psa_fwu_write should not fail when writing the protected TLV data.");
        return;
    }
    status = psa_fwu_install(test_image,
                             &dependency_uuid,
                             &dependency_version);

    /* Dependency check in multiple images:
     * dependency image ID: nonsecure image.
     * Currently running nonsecure image version: info.version <
     * dep_version_in_tlv.
     * So the dependency check should fail.
     * PSA_ERROR_DEPENDENCY_NEEDED should be returned.
     */
    if (status != PSA_ERROR_DEPENDENCY_NEEDED) {
        TEST_FAIL("Install PSA_ERROR_DEPENDENCY_NEEDED should be returned when the image dependency check fails.");
        return;
    }

    /* Check the returned dependency information. */
    if ((version_compare(&dependency_version, &dep_version_in_tlv) != 0) ||
        (dependency_uuid != dependency_image)) {
        TEST_FAIL("psa_fwu_install returned the wrong dependency information.");
        return;
    }

    /* Query the staging image. */
    status = psa_fwu_query(test_image, &info);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Query should success");
        return;
    }

    /* Check the image state. */
    if (info.state != PSA_IMAGE_CANDIDATE) {
        TEST_FAIL("Image should be in CANDIDATE state after install returns PSA_ERROR_DEPENDENCY_NEEDED.");
        return;
    }

    /* ******************************************************************
     * Start the Firmware Update of the dependency image(nonsecure image)
     * ******************************************************************
     */
    /* Update the image version to meet the required min_version. */
    memcpy(header_dep_image_version_not_zero + HEADER_IMAGE_VERSION_OFF,
           &dep_version_in_tlv,
           sizeof(dep_version_in_tlv));

    /* The image header(from header magic to image version) is downloaded
     * into the target device by this psa_fwu_write operation.
     */
    status = psa_fwu_write(dependency_uuid,
                           0,
                           header_dep_image_version_not_zero,
                           sizeof(header_dep_image_version_not_zero));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("psa_fwu_write should not fail when writing the image header data.");
        return;
    }

    /* Install the test image again after writing the image header of the
     * dependency image. At this point, the dependency image has not been
     * installed. When checking the dependency, the dependency
     * image in the secondary slot cannot to used. So
     * PSA_ERROR_DEPENDENCY_NEEDED should still be returned by the install.
     */
    status = psa_fwu_install(test_image, &dependency_uuid, &dependency_version);
    if (status != PSA_ERROR_DEPENDENCY_NEEDED) {
        TEST_FAIL("Install should fail when the boot magic of the dependency image is not good.");
        return;
    }

    /* Check the returned dependency information. */
    if ((version_compare(&dependency_version, &dep_version_in_tlv) != 0) ||
        (dependency_uuid != dependency_image)) {
        TEST_FAIL("psa_fwu_install returned the wrong dependency information.");
        return;
    }

    /* Write the protected TLV + IMAGE_TLV_INFO_MAGIC + part of the SHA256.
     * The image dependency information is downloaded into the target device
     * by this psa_fwu_write operation.
     */
    /* The required image version(0,0,0) is less than or equal to the running.
     */
    status = psa_fwu_write(dependency_uuid,
                           (size_t)PROTECTED_TLV_OFF_NONSECURE,
                           tlv_data_nonsecure_image,
                           sizeof(tlv_data_nonsecure_image));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("psa_fwu_write should not fail when writing the protected TLV data.");
        return;
    }

    /* Install the dependency image(nonsecure image).
     * Dependency check:
     * dependency image ID: secure image which has been once installed.
     * dependency image version:IMAGE_VERSION_FOR_TEST_MAJOR,
     *                          IMAGE_VERSION_FOR_TEST_MINOR,
     *                          IMAGE_VERSION_FOR_TEST_REVISION_HIGH << 8 |
     *                          IMAGE_VERSION_FOR_TEST_REVISION_LOW,
     * which equals to the image version of the secure image in the secondary
     * slot. So no matter what's the version of the running secure image. The
     * dependency check should pass.
     * In the current implementation, image verification is deferred to
     * reboot, so PSA_SUCCESS_REBOOT is returned when success.
     */
    status = psa_fwu_install(dependency_uuid,
                             &dependency_uuid_temp,
                             &dependency_version_temp);
    if (status != PSA_SUCCESS_REBOOT) {
        TEST_FAIL("Install should fail after the dependency image successfully being installed.");
        return;
    }

    /* *********************************************************************
     * The dependency image(nonsecure image) has been successfully installed
     * *********************************************************************
     */
    /* Install the test image again.
     *
     * Dependency check:
     * dependency image ID: nonsecure image which has been successfully
     *                      installed.
     * dependency image version: running nonsecure image version.major + 1.
     *                           0.
     *                           0.
     * which equals to image version of the nonsecure image in the secondary
     * slot.
     * So the dependency check should pass.
     * In the current implementation, image verification is deferred to
     * reboot, so PSA_SUCCESS_REBOOT is returned when success.
     */
    status = psa_fwu_install(test_image,
                             &dependency_uuid,
                             &dependency_version);
    if (status != PSA_SUCCESS_REBOOT) {
        TEST_FAIL("Install should fail after the dependency image successfully being installed.");
        return;
    }

    /* Test clean up: abort the fwu process of the dependency image. */
    status = psa_fwu_abort(dependency_image);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Abort should not fail after install");
        return;
    }

    /* Test clean up: abort the fwu process of the test image. */
    status = psa_fwu_abort(test_image);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Abort should not fail after install");
        return;
    }
    ret->val = TEST_PASSED;
}
#endif

#ifdef TFM_PSA_API
void tfm_fwu_test_common_016(struct test_result_t *ret)
{
    psa_status_t status;

    /* `block size` boundary test. */
    status = psa_fwu_write(test_image,
                           0,
                           header_test_image_version_zero,
                           PSA_FWU_MAX_BLOCK_SIZE + 1);
    if (status != PSA_ERROR_INVALID_ARGUMENT) {
        TEST_FAIL("psa_fwu_write boundary test failed.");
        return;
    }
    /* `block size` boundary test. */
    status = psa_fwu_write(test_image,
                           0,
                           header_test_image_version_zero,
                           0);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("psa_fwu_write boundary test failed.");
        return;
    }
    ret->val = TEST_PASSED;
}
#endif
