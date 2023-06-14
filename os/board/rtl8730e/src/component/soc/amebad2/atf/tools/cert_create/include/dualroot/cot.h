/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DUALROOT_COT_H
#define DUALROOT_COT_H

/* Certificates. */
enum {
	/* Certificates owned by the silicon provider. */
	TRUSTED_BOOT_FW_CERT,
	TRUSTED_KEY_CERT,
	SCP_FW_KEY_CERT,
	SCP_FW_CONTENT_CERT,
	SOC_FW_KEY_CERT,
	SOC_FW_CONTENT_CERT,
	TRUSTED_OS_FW_KEY_CERT,
	TRUSTED_OS_FW_CONTENT_CERT,
	FWU_CERT,

	/* Certificates owned by the platform owner. */
	NON_TRUSTED_FW_CONTENT_CERT,
};

/* Certificate extensions. */
enum {
	/* Extensions used in certificates owned by the silicon provider. */
	TRUSTED_FW_NVCOUNTER_EXT,
	TRUSTED_BOOT_FW_HASH_EXT,
	TRUSTED_BOOT_FW_CONFIG_HASH_EXT,
	HW_CONFIG_HASH_EXT,
	TRUSTED_WORLD_PK_EXT,
	SCP_FW_CONTENT_CERT_PK_EXT,
	SCP_FW_HASH_EXT,
	SOC_FW_CONTENT_CERT_PK_EXT,
	SOC_AP_FW_HASH_EXT,
	SOC_FW_CONFIG_HASH_EXT,
	TRUSTED_OS_FW_CONTENT_CERT_PK_EXT,
	TRUSTED_OS_FW_HASH_EXT,
	TRUSTED_OS_FW_EXTRA1_HASH_EXT,
	TRUSTED_OS_FW_EXTRA2_HASH_EXT,
	TRUSTED_OS_FW_CONFIG_HASH_EXT,
	SCP_FWU_CFG_HASH_EXT,
	AP_FWU_CFG_HASH_EXT,
	FWU_HASH_EXT,

	/* Extensions used in certificates owned by the platform owner. */
	PROT_PK_EXT,
	NON_TRUSTED_FW_NVCOUNTER_EXT,
	NON_TRUSTED_FW_CONTENT_CERT_PK_EXT,
	NON_TRUSTED_WORLD_BOOTLOADER_HASH_EXT,
	NON_TRUSTED_FW_CONFIG_HASH_EXT,
};

/* Keys. */
enum {
	/* Keys owned by the silicon provider. */
	ROT_KEY,
	TRUSTED_WORLD_KEY,
	SCP_FW_CONTENT_CERT_KEY,
	SOC_FW_CONTENT_CERT_KEY,
	TRUSTED_OS_FW_CONTENT_CERT_KEY,

	/* Keys owned by the platform owner. */
	PROT_KEY,
};

#endif /* DUALROOT_COT_H */
