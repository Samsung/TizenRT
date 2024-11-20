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

#include <syntiant_ilib/syntiant_portability.h>
#include <syntiant_ilib/syntiant_ndp_ilib_version.h>
#include <syntiant_ilib/syntiant_ndp115.h>
#include <syntiant_ilib/syntiant_ndp115_driver.h>
#include <syntiant_ilib/syntiant_ndp_error.h>

static unsigned int syntiant_ndp115_device_types[] = {
    0x48,                   /* NDP115A0 */
    0x4A, 0x4B,             /* NDP115A0 with secure boot */
    0x0  /* len of list */
};

/* register ndp120.chip_config.pinoutmux */
#define NDP120_CHIP_CONFIG_PINOUTMUX 0x400091c8U
#define NDP120_CHIP_CONFIG_PINOUTMUX_AUD1_PIN_SEL_SHIFT 0
#define NDP120_CHIP_CONFIG_PINOUTMUX_AUD1_PIN_SEL_MASK 0x00000003U
#define NDP120_CHIP_CONFIG_PINOUTMUX_AUD1_PIN_SEL(v) \
        ((v) << NDP120_CHIP_CONFIG_PINOUTMUX_AUD1_PIN_SEL_SHIFT)
#define NDP120_CHIP_CONFIG_PINOUTMUX_AUD1_PIN_SEL_MASK_INSERT(x, v) \
        (((x) & ~NDP120_CHIP_CONFIG_PINOUTMUX_AUD1_PIN_SEL_MASK) | \
        ((v) << NDP120_CHIP_CONFIG_PINOUTMUX_AUD1_PIN_SEL_SHIFT))
#define NDP120_CHIP_CONFIG_PINOUTMUX_AUD1_PIN_SEL_DEFAULT 0x00000000U
#define NDP120_CHIP_CONFIG_PINOUTMUX_AUD1_PIN_SEL_AUD2 0x0U
#define NDP120_CHIP_CONFIG_PINOUTMUX_AUD1_PIN_SEL_AUD1 0x1U
#define NDP120_CHIP_CONFIG_PINOUTMUX_AUD1_PIN_SEL_I2C 0x2U
#define NDP120_CHIP_CONFIG_PINOUTMUX_AUD1_PIN_SEL_MAX 0x2U

#define NDP120_CHIP_CONFIG_PINOUTMUX_MSPI_PIN_SEL_SHIFT 2
#define NDP120_CHIP_CONFIG_PINOUTMUX_MSPI_PIN_SEL_MASK 0x00000004U
#define NDP120_CHIP_CONFIG_PINOUTMUX_MSPI_PIN_SEL(v) \
        ((v) << NDP120_CHIP_CONFIG_PINOUTMUX_MSPI_PIN_SEL_SHIFT)
#define NDP120_CHIP_CONFIG_PINOUTMUX_MSPI_PIN_SEL_MASK_INSERT(x, v) \
        (((x) & ~NDP120_CHIP_CONFIG_PINOUTMUX_MSPI_PIN_SEL_MASK) | \
        ((v) << NDP120_CHIP_CONFIG_PINOUTMUX_MSPI_PIN_SEL_SHIFT))
#define NDP120_CHIP_CONFIG_PINOUTMUX_MSPI_PIN_SEL_DEFAULT 0x00000000U
#define NDP120_CHIP_CONFIG_PINOUTMUX_MSPI_PIN_SEL_MSPI 0x0U
#define NDP120_CHIP_CONFIG_PINOUTMUX_MSPI_PIN_SEL_I2C_JTAG 0x1U
#define NDP120_CHIP_CONFIG_PINOUTMUX_MSPI_PIN_SEL_MAX 0x1U

#define NDP120_CHIP_CONFIG_PINOUTMUX_DEFAULT 0x00000000U

/* helper functions */
int syntiant_ndp115_config_pinmux(
    struct syntiant_ndp_device_s *ndp,
    const syntiant_ndp115_pinmux_config_t *config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t data = 0;

    if (ndp->device_type > 0x4B ||
        ndp->device_type < 0x48) {
        /* not ndp115 device, so no action */
        goto error;
    }

    /* configure for aud1 */
    if (config->aud1_config > NDP120_CHIP_CONFIG_PINOUTMUX_AUD1_PIN_SEL_MAX ||
        config->mspi_config > NDP120_CHIP_CONFIG_PINOUTMUX_MSPI_PIN_SEL_MAX) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    data = (uint32_t) (NDP120_CHIP_CONFIG_PINOUTMUX_AUD1_PIN_SEL(config->aud1_config)
                   | NDP120_CHIP_CONFIG_PINOUTMUX_MSPI_PIN_SEL(config->mspi_config));

    s = ndp->driver->write_block(ndp, 1, NDP120_CHIP_CONFIG_PINOUTMUX, &data, sizeof(data));
    if (s) goto error;

error:
    return s;
}

/* ========================= */
/* EXPORTED DRIVER FUNCTIONS */
/* ========================= */

struct syntiant_ndp_driver_s syntiant_ndp115_driver = {
    SYNTIANT_NDP_ILIB_SYNTIANT_CORE_2_MAJOR_VERSION,    /* major version */
    SYNTIANT_NDP_ILIB_SYNTIANT_CORE_2_MINOR_VERSION,    /* minor version */
    SYNTIANT_NDP_ILIB_SYNTIANT_CORE_2_PATCH_VERSION,    /* patch number */
    syntiant_ndp115_device_types,
    SYNTIANT_NDP_FAMILY_CORE_2,
    syntiant_ndp120_init,
    syntiant_ndp120_uninit,
    syntiant_ndp120_op_size,
    syntiant_ndp120_interrupts,
    syntiant_ndp120_poll,
    syntiant_ndp120_load,
    syntiant_ndp120_get_config,
    syntiant_ndp120_send_data,
    syntiant_ndp120_extract_data,
    syntiant_ndp120_get_match_summary,
    syntiant_ndp120_get_match_binary,
    syntiant_ndp120_get_match_strength,
    syntiant_ndp120_read_block,
    syntiant_ndp120_write_block,
    NULL
};

