/*
 * Copyright (c) 2024 Syntiant Corp.  All rights reserved.
 * Contact at http://www.syntiant.com
 *
 * This software is available to you under a choice of one of two licenses.
 * You may choose to be licensed under the terms of the GNU General Public
 * License (GPL) Version 2, available from the file LICENSE in the main
 * directory of this source tree, or the OpenIB.org BSD license below.  Any
 * code involving Linux software will require selection of the GNU General
 * Public License (GPL) Version 2.
 *
 * OPENIB.ORG BSD LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 	** SDK: v112.3.6-Samsung **
*/
/*
 * ILib-internal driver interface definitions
 */
#ifndef SYNTIANT_NDP_DRIVER_H
#define SYNTIANT_NDP_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <syntiant_ilib/syntiant_portability.h>
#include <syntiant_ilib/syntiant_ndp.h>

#ifndef EXCLUDE_SYNTIANT_CORE_1
#include <syntiant_ilib/syntiant_ndp10x_driver.h>
#endif

#ifndef EXCLUDE_SYNTIANT_CORE_2
#include <syntiant_ilib/syntiant_ndp120_driver.h>
#include <syntiant_ilib/syntiant_ndp115_driver.h>
#endif

#include <syntiant_packager/syntiant_package.h>

struct syntiant_ndp_driver_s {
    unsigned int major;
    unsigned int minor;
    unsigned int patch;
    unsigned int *device_types;
    unsigned int device_family;
    int (*init)(struct syntiant_ndp_device_s *ndp,
        enum syntiant_ndp_init_mode_e init_mode);
    int (*uninit)(struct syntiant_ndp_device_s *ndp,
        enum syntiant_ndp_init_mode_e init_mode);
    int (*op_size)(struct syntiant_ndp_device_s *ndp, int mcu,
                   unsigned int *size);
    int (*interrupts)(struct syntiant_ndp_device_s *ndp, int *on);
    int (*poll)(
        struct syntiant_ndp_device_s *ndp, uint32_t *notifications, int clear);
    int (*load)(struct syntiant_ndp_device_s *ndp, void *package, int len);
    int (*get_config)(struct syntiant_ndp_device_s *ndp,
        struct syntiant_ndp_config_s *config);
    int (*send_data)(struct syntiant_ndp_device_s *ndp, const uint8_t *data,
                     unsigned int len, int type, uint32_t offset);
    int (*extract_data)(struct syntiant_ndp_device_s *ndp,
                        int type, int from, uint8_t *data, unsigned int *len);
    int (*get_match_summary)(
        struct syntiant_ndp_device_s *ndp, uint32_t *summary);
    int (*get_match_binary)(
        struct syntiant_ndp_device_s *ndp, uint8_t *matches, unsigned int len);
    int (*get_match_strength)(struct syntiant_ndp_device_s *ndp,
        uint8_t *strengths, unsigned int len, int type);
    int (*read_block)(struct syntiant_ndp_device_s *ndp, int mcu,
                      uint32_t address, void *value, unsigned int count);
    int (*write_block)(struct syntiant_ndp_device_s *ndp, int mcu,
                       uint32_t address, const void *value, unsigned int count);
    void (*sleep) (uint32_t delay_us);
};

/**
 * @brief NDP interface library internal state object
 */
struct syntiant_ndp_device_s {
    struct syntiant_ndp_integration_interfaces_s iif;
    struct syntiant_ndp_driver_s *driver;
    unsigned int device_type;
    uint8_t init;
    syntiant_pkg_parser_state_t pstate;
    union {
#ifndef EXCLUDE_SYNTIANT_CORE_1
        struct syntiant_ndp10x_device_s ndp10x;
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
        struct syntiant_ndp120_device_s ndp120;
#endif
    } d;
};

#ifdef __cplusplus
}
#endif

#endif
