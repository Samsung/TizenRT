/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <scsc/scsc_mx.h>
#include <sys/types.h>
#include <sys/statfs.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <tinyara/kmalloc.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/procfs.h>
#include <tinyara/fs/dirent.h>

#include "scsc_mx_impl.h"
#include "utils_misc.h"

/* Firmware directory definitions */

#define MX140_USE_OWN_LOAD_FILE 1

#define MX140_FW_BASE_DIR_ETC_WIFI      "/system/etc/wifi"

#ifdef MX140_USE_OWN_LOAD_FILE
#define MX140_FW_BASE_DIR               "/vendor/firmware/mx140/fw"
#else
#define MX140_FW_BASE_DIR               "mx140"
#endif

#define MX140_FW_CONF_SUBDIR            "conf"
#define MX140_FW_BIN                    "mx140.bin"
#define MX140_FW_PATH_MAX_LENGTH        (512)

#define MX140_FW_VARIANT_DEFAULT        "mx140"
#define MX140_FW_VARIANT_LEGACY_DEFAULT "full-service";

static char *firmware_variant = MX140_FW_VARIANT_LEGACY_DEFAULT;
//module_param(firmware_variant, charp, S_IRUGO | S_IWUSR);
//MODULE_PARM_DESC(firmware_variant, "mx140 firmware variant, default full-service");

static char *base_dir = MX140_FW_BASE_DIR;
//module_param(base_dir, charp, S_IRUGO | S_IWUSR);
//MODULE_PARM_DESC(base_dir, "Base directory for firmware and config");

//static bool enable_auto_sense;
//module_param(enable_auto_sense, bool, S_IRUGO | S_IWUSR);
//MODULE_PARM_DESC(enable_auto_sense, "Set to true to allow driver to switch to legacy f/w dir if new one is not populated");

static bool use_new_fw_structure = true;
//module_param(use_new_fw_structure, bool, S_IRUGO | S_IWUSR);
//MODULE_PARM_DESC(use_mcd_structure, "If enable_auto_sense is false and /etc/wifi is used, set this to true");

/* Reads a configuration file into memory (f/w profile specific) */
int mx140_file_request_conf(struct scsc_mx *mx, const struct firmware **conf, const char *config_rel_path)
{
#ifndef MX140_USE_OWN_LOAD_FILE
	struct device *dev;
#endif
	char config_path[MX140_FW_PATH_MAX_LENGTH];

	//dev = scsc_mx_get_device(mx);

	/* e.g. /vendor/firmware/mx140/fw/full-service/conf/wlan/wlan.hcf */

	snprintf(config_path, sizeof(config_path), "%s/%s/%s/%s", base_dir, firmware_variant, MX140_FW_CONF_SUBDIR, config_rel_path);

#ifdef MX140_USE_OWN_LOAD_FILE
	return mx140_request_file(mx, config_path, conf);
#else
	return request_firmware(conf, config_path, dev);
#endif
}

/* Read device configuration file into memory (whole device specific) */
int mx140_file_request_device_conf(struct scsc_mx *mx, const struct firmware **conf, const char *config_rel_path)
{
#ifndef MX140_USE_OWN_LOAD_FILE
	struct device *dev;
#endif
	char config_path[MX140_FW_PATH_MAX_LENGTH];

	//dev = scsc_mx_get_device(mx);

	/* e.g. /vendor/firmware/mx140/conf/wlan/mac.txt */

	snprintf(config_path, sizeof(config_path), "%s/%s/%s", base_dir, MX140_FW_CONF_SUBDIR, config_rel_path);

#ifdef MX140_USE_OWN_LOAD_FILE
	return mx140_request_file(mx, config_path, conf);
#else
	return request_firmware(conf, config_path, dev);
#endif
}

/* Release configuration file memory. */
void mx140_file_release_conf(struct scsc_mx *mx, const struct firmware *conf)
{
	(void)mx;

#ifdef MX140_USE_OWN_LOAD_FILE
	mx140_release_file(mx, conf);
#else
	if (conf) {
		release_firmware(conf);
	}
#endif
}

/* Download firmware binary into a buffer supplied by the caller */
int mx140_file_download_fw(struct scsc_mx *mx, void *dest, size_t dest_size, u32 *fw_image_size)
{
	const struct firmware *firm = NULL;
//  struct device         *dev;
	int r = 0;
	char img_path_name[MX140_FW_PATH_MAX_LENGTH];

#ifdef CONFIG_SCSC_T20_WLAN_DEBUG
	pr_info("firmware_variant=%s\n", firmware_variant);
#endif
	//dev = scsc_mx_get_device(mx);

	/* e.g. /vendor/firmware/mx140/fw/full-service/mx140.bin */

	if (use_new_fw_structure) {
		snprintf(img_path_name, sizeof(img_path_name), "%s/%s.bin", base_dir, firmware_variant);
	} else {
		snprintf(img_path_name, sizeof(img_path_name), "%s/%s/" MX140_FW_BIN, base_dir, firmware_variant);
	}

#ifdef MX140_USE_OWN_LOAD_FILE
	r = mx140_request_file(mx, img_path_name, &firm);
#else
	r = request_firmware(&firm, img_path_name, dev);
#endif
	if (r) {
		pr_err("Error Loading FW, error %d\n", r);
		return -EINVAL;
	}

	if (firm->size > dest_size) {
		pr_err("%s: firmware image too big for buffer (%zu > %u)", __func__, dest_size, *fw_image_size);
		r = -EINVAL;
	} else {
		memcpy(dest, firm->data, firm->size);
		*fw_image_size = firm->size;
	}
#ifdef MX140_USE_OWN_LOAD_FILE
	mx140_release_file(mx, firm);
#else
	release_firmware(firm);
#endif

	return r;
}

int mx140_request_file(struct scsc_mx *mx, char *path, const struct firmware **firmp)
{
	struct firmware *firm;

	/* Get firmware structure. */
	firm = kmm_zalloc(sizeof(*firm));
	if (!firm) {
		pr_err("%s: kzalloc(%zu) failed for %s\n", __func__, sizeof(*firmp), path);
		return -ENOMEM;
	}

	/* Pass to caller. Caller will free allocated memory through
	 * mx140_release_file().
	 */
	*firmp = firm;

	return 0;
}

int mx140_release_file(struct scsc_mx *mx, const struct firmware *firmp)
{
	if (!firmp || !firmp->data) {
		pr_err("%s: firmp=%p\n", __func__, firmp);
		return -EINVAL;
	}
	if (firmp->priv != NULL) {
		kmm_free(firmp->priv);
	}

	kmm_free((void *)firmp);
	return 0;
}

void mx140_basedir_file(struct scsc_mx *mx)
{
#ifdef MX140_USE_OWN_LOAD_FILE
	base_dir = MX140_FW_BASE_DIR;
	firmware_variant = MX140_FW_VARIANT_LEGACY_DEFAULT;
#endif
}
