/****************************************************************************
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

/// @file tc_internal.h

/// @brief Header file for Filesystem TestCase Example
#ifndef __EXAMPLES_TESTCASE_FILESYSTEM_TC_INTERNAL_H
#define __EXAMPLES_TESTCASE_FILESYSTEM_TC_INTERNAL_H

#if defined(CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS)
#define TMP_MOUNT_DEV_DIR CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME
#elif defined(CONFIG_ARTIK05X_AUTOMOUNT_USERFS)
#define TMP_MOUNT_DEV_DIR CONFIG_ARTIK05X_AUTOMOUNT_USERFS_DEVNAME
#elif defined(CONFIG_ESP32_AUTOMOUNT_USERFS_DEVNAME)
#define TMP_MOUNT_DEV_DIR CONFIG_ESP32_AUTOMOUNT_USERFS_DEVNAME
#elif defined(CONFIG_ARCH_BOARD_LM3S6965EK)
#define TMP_MOUNT_DEV_DIR "/dev/smart0p0"
#elif defined(CONFIG_IMXRT_AUTOMOUNT_USERFS)
#define TMP_MOUNT_DEV_DIR CONFIG_IMXRT_AUTOMOUNT_USERFS_DEVNAME
#elif defined(CONFIG_AMEBAD_AUTOMOUNT_USERFS_DEVNAME)
#define TMP_MOUNT_DEV_DIR CONFIG_AMEBAD_AUTOMOUNT_USERFS_DEVNAME
#else
#define TMP_MOUNT_DEV_DIR "/dev/smart1"
#endif

#define SMARTFS_DEV_PATH TMP_MOUNT_DEV_DIR

/**********************************************************
* TC Function Declarations
**********************************************************/
void tc_fs_procfs_main(void);
void tc_fs_smartfs_procfs_main(void);
void tc_fs_smartfs_mksmartfs_p(void);
void tc_fs_smartfs_mksmartfs_invalid_path_n(void);

void itc_fs_main(void);

#endif /* __EXAMPLES_TESTCASE_FILESYSTEM_TC_INTERNAL_H */
