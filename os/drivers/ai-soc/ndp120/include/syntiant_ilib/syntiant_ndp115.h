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
#ifndef SYNTIANT_NDP115_H
#define SYNTIANT_NDP115_H

#ifdef __cplusplus
extern "C" {
#endif

#include <syntiant_ilib/syntiant_ndp120.h>

enum syntiant_ndp115_pinoutmux_aud1_config_e {
    SYNTIANT_NDP115_AUD1_PIN_SEL_AUD2,
    SYNTIANT_NDP115_AUD1_PIN_SEL_AUD1,
    SYNTIANT_NDP115_AUD1_PIN_SEL_I2C
};

enum syntiant_ndp115_pinoutmux_mspi_config_e {
    SYNTIANT_NDP115_MSPI_PIN_SEL_MSPI,
    SYNTIANT_NDP115_MSPI_PIN_SEL_I2C_JTAG
};

typedef struct syntiant_ndp115_pinmux_config_s {
    enum syntiant_ndp115_pinoutmux_aud1_config_e aud1_config;
    enum syntiant_ndp115_pinoutmux_mspi_config_e mspi_config;
} syntiant_ndp115_pinmux_config_t;

int syntiant_ndp115_config_pinmux(
    struct syntiant_ndp_device_s *ndp,
    const syntiant_ndp115_pinmux_config_t *config);

#ifdef __cplusplus
}
#endif

#endif
