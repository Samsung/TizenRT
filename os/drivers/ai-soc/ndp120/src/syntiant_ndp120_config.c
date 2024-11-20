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
#include <syntiant_ilib/ndp120_regs.h>
#include <syntiant_ilib/ndp120_spi_regs.h>
#include <syntiant_ilib/syntiant_ndp.h>
#include <syntiant_ilib/syntiant_ndp120.h>
#include <syntiant_ilib/syntiant_ndp_error.h>
#include <syntiant_ilib/syntiant_ndp_driver.h>

#include <syntiant_ilib/syntiant_ndp120_driver.h>
#include <syntiant_ilib/syntiant_ndp120_mailbox.h>
#include <syntiant_ilib/syntiant_ndp120_ph.h>
#include <syntiant_packager/syntiant_package_consts.h>
#include <syntiant-dsp-firmware/ndp120_dsp_fw_state.h>

#include <syntiant-firmware/ndp120_firmware.h>
#include <syntiant-firmware/ndp120_result.h>
#include <syntiant-firmware/ndp120_orchestrator.h>
#include <syntiant-dsp-firmware/ndp120_dsp_sample.h>
#include <syntiant-firmware/ndp120_ph.h>

#include "syntiant_ndp120_config.h"

#define FLL_LOCK_WAIT_IN_MSEC   500
#define PLL_LOCK_WAIT_IN_MSEC   500

#define member_size(type, member) sizeof(((type *)0)->member)

enum syntiant_ndp120_internal_constants_e {
    /* TODO merge these elsewhere */
    SYNTIANT_NDP120_TARGET_CLOCK_RATE = 16000000,
    SYNTIANT_NDP120_TARGET_OVERDRIVE_CLOCK_RATE = 24000000,
    SYNTIANT_NDP120_MAX_MCU_RATE = 19200000,
    SYNTIANT_NDP120_CLKCTL0_MCUCLKDIV_NO_DIV = 1,
    SYNTIANT_NDP120_CLKCTL0_MCUCLKDIV_DIV2 = 2
};

typedef enum  {
    NDP120_WFI_ENABLE = 0U,
    NDP120_WFI_DISABLE = 1U
} syntiant_ndp120_wfi_control_t;

/* mode_0p9v_10p752MHz_32p768kHz */
static ndp120_pll_preset_value_t mode_0p9v_10p752MHz_32p768kHz[] = {
    {0x0,0x1f8},
    {0x1,0x1},
    {0x2,0x148},
    {0x3,0x1},
    {0x4,0xd94},
    {0x5,0x14bc},
    {0x7,0xc94},
    {0x8,0x1285},
    {0xa,0x1fe},
    {0xb,0x9d8},
    {0xc,0x445},
    {0x17,0x0},
    {0x18,0x0},
    {0x19,0x0},
    {0x1a,0x0},
    {0x20,0x6},
    {0x23,0x0},
    {0x24,0x0},
    {0x27,0x14},
    {0x38,0x0},
    {0x0, 0x0}
};

/* mode_0p9v_15p360MHz_32p768kHz */
static ndp120_pll_preset_value_t mode_0p9v_15p360MHz_32p768kHz[] = {
    {0x0,0x1f8},
    {0x1,0x1},
    {0x2,0x1d4},
    {0x3,0x6},
    {0x4,0xd94},
    {0x5,0x14bc},
    {0x7,0xc94},
    {0x8,0x1285},
    {0xa,0x1fe},
    {0xb,0xe10},
    {0xc,0x445},
    {0x17,0x0},
    {0x18,0x0},
    {0x19,0x0},
    {0x1a,0x0},
    {0x20,0x6},
    {0x23,0x0},
    {0x24,0x0},
    {0x27,0x14},
    {0x38,0x0},
    {0x0, 0x0}
};

/* mode_0p9v_21p504MHz_32p768kHz */
static ndp120_pll_preset_value_t mode_0p9v_21p504MHz_32p768kHz[] = {
    {0x0,0x1f8},
    {0x1,0x1},
    {0x2,0x290},
    {0x3,0x2},
    {0x4,0xd94},
    {0x5,0x14bc},
    {0x7,0xc94},
    {0x8,0x1285},
    {0xa,0x1fe},
    {0xb,0x13b0},
    {0xc,0x445},
    {0x17,0x0},
    {0x18,0x0},
    {0x19,0x0},
    {0x1a,0x0},
    {0x20,0x6},
    {0x23,0x0},
    {0x24,0x0},
    {0x27,0x14},
    {0x38,0x0},
    {0x0, 0x0}
};

/* mode_0p9v_15p360MHz_4p096MHz */
static ndp120_pll_preset_value_t mode_0p9v_15p360MHz_4p096MHz[] = {
    {0x0,0x1f8},
    {0x1,0xf},
    {0x2,0x38},
    {0x3,0x2},
    {0x4,0x9cc},
    {0x5,0xf80},
    {0x7,0x9fa},
    {0x8,0xfaf},
    {0xa,0x1fe},
    {0xb,0x1b0},
    {0xc,0x32},
    {0x17,0x0},
    {0x18,0x0},
    {0x19,0x0},
    {0x1a,0x0},
    {0x20,0x6},
    {0x23,0x0},
    {0x24,0x0},
    {0x27,0x14},
    {0x38,0x0},
    {0x0, 0x0}
};

/* mode_0p9v_21p504MHz_4p096MHz */
static ndp120_pll_preset_value_t mode_0p9v_21p504MHz_4p096MHz[] = {
    {0x0,0x1f8},
    {0x1,0xf},
    {0x2,0x4e},
    {0x3,0x6},
    {0x4,0x992},
    {0x5,0x10b7},
    {0x7,0x9b3},
    {0x8,0x10fa},
    {0xa,0x1fe},
    {0xb,0x25c},
    {0xc,0x46},
    {0x17,0x0},
    {0x18,0x0},
    {0x19,0x0},
    {0x1a,0x0},
    {0x20,0x6},
    {0x23,0x0},
    {0x24,0x0},
    {0x27,0x14},
    {0x38,0x0},
    {0x0, 0x0}
};

/* mode_1p0v_49p152MHz_32p768kHz */
static ndp120_pll_preset_value_t mode_1p0v_49p152MHz_32p768kHz[] = {
    {0x0,0x1f8},
    {0x1,0x1},
    {0x2,0x5dc},
    {0x3,0x0},
    {0x4,0xfc8},
    {0x5,0x1580},
    {0x7,0xec8},
    {0x8,0x14b5},
    {0xa,0x1fe},
    {0xb,0x2d00},
    {0xc,0xc83},
    {0x17,0x0},
    {0x18,0x4},
    {0x19,0x0},
    {0x1a,0x0},
    {0x20,0x1},
    {0x23,0x0},
    {0x24,0x0},
    {0x27,0x14},
    {0x38,0x0},
    {0x0, 0x0}
};

/* mode_1p0v_55p296MHz_32p768kHz */
static ndp120_pll_preset_value_t mode_1p0v_55p296MHz_32p768kHz[] = {
    {0x0,0x1f8},
    {0x1,0x1},
    {0x2,0x697},
    {0x3,0x4},
    {0x4,0xfc8},
    {0x5,0x1580},
    {0x7,0xec8},
    {0x8,0x14b5},
    {0xa,0x1fe},
    {0xb,0x32a0},
    {0xc,0xc83},
    {0x17,0x0},
    {0x18,0x4},
    {0x19,0x0},
    {0x1a,0x0},
    {0x20,0x1},
    {0x23,0x0},
    {0x24,0x0},
    {0x27,0x14},
    {0x38,0x0},
    {0x0, 0x0}
};

/* mode_1p0v_49p152MHz_4p096MHz */
static ndp120_pll_preset_value_t mode_1p0v_49p152MHz_4p096MHz[] = {
    {0x0,0x1f8},
    {0x1,0x8},
    {0x2,0x60},
    {0x3,0x0},
    {0x4,0xbc7},
    {0x5,0x12fa},
    {0x7,0xbf4},
    {0x8,0x11aa},
    {0xa,0x1fe},
    {0xb,0x2e1},
    {0xc,0xcd},
    {0x17,0x0},
    {0x18,0x4},
    {0x19,0x0},
    {0x1a,0x0},
    {0x20,0x1},
    {0x23,0x0},
    {0x24,0x0},
    {0x27,0x14},
    {0x38,0x0},
    {0x0, 0x0}
};

/* mode_1p1v_55p296MHz_32p768kHz */
static ndp120_pll_preset_value_t mode_1p1v_55p296MHz_32p768kHz[] = {
    {0x0,0x1f8},
    {0x1,0x1},
    {0x2,0x697},
    {0x3,0x4},
    {0x4,0xf88},
    {0x5,0x16ac},
    {0x7,0xe88},
    {0x8,0x15f4},
    {0xa,0x1fe},
    {0xb,0x32a0},
    {0xc,0xfff},
    {0x17,0x0},
    {0x18,0x4},
    {0x19,0x0},
    {0x1a,0x0},
    {0x20,0x1},
    {0x23,0x0},
    {0x24,0x0},
    {0x27,0x14},
    {0x38,0x0},
    {0x0, 0x0}
};

/* mode_1p1v_76p800MHz_32p768kHz */
static ndp120_pll_preset_value_t mode_1p1v_76p800MHz_32p768kHz[] = {
    {0x0,0x1f8},
    {0x1,0x1},
    {0x2,0x927},
    {0x3,0x6},
    {0x4,0xf88},
    {0x5,0x16ac},
    {0x7,0xe88},
    {0x8,0x15f4},
    {0xa,0x1fe},
    {0xb,0x4650},
    {0xc,0xfff},
    {0x17,0x0},
    {0x18,0x4},
    {0x19,0x0},
    {0x1a,0x0},
    {0x20,0x1},
    {0x23,0x0},
    {0x24,0x0},
    {0x27,0x14},
    {0x38,0x0},
    {0x0, 0x0}
};

/* mode_1p1v_98p304MHz_32p768kHz */
static ndp120_pll_preset_value_t mode_1p1v_98p304MHz_32p768kHz[] = {
    {0x0,0x1f8},
    {0x1,0x1},
    {0x2,0xbb8},
    {0x3,0x0},
    {0x4,0xf88},
    {0x5,0x16ac},
    {0x7,0xe88},
    {0x8,0x15f4},
    {0xa,0x1fe},
    {0xb,0x5a00},
    {0xc,0xfff},
    {0x17,0x0},
    {0x18,0x4},
    {0x19,0x0},
    {0x1a,0x0},
    {0x20,0x1},
    {0x23,0x0},
    {0x24,0x0},
    {0x27,0x14},
    {0x38,0x0},
    {0x0, 0x0}
};

/* mode_1p1v_98p304MHz_0p512MHz */
static ndp120_pll_preset_value_t mode_1p1v_98p304MHz_0p512MHz[] = {
    {0x0,0x1f8},
    {0x1,0x1},
    {0x2,0xc0},
    {0x3,0x0},
    {0x4,0xb86},
    {0x5,0x12a8},
    {0x7,0xba4},
    {0x8,0x12e5},
    {0xa,0x1fe},
    {0xb,0x5c2},
    {0xc,0x131},
    {0x17,0x0},
    {0x18,0x4},
    {0x19,0x0},
    {0x1a,0x0},
    {0x20,0x1},
    {0x23,0x0},
    {0x24,0x0},
    {0x27,0x14},
    {0x38,0x0},
    {0x0, 0x0}
};


/* mode_1p1v_76p800MHz_4p096MHz */
static ndp120_pll_preset_value_t mode_1p1v_76p800MHz_4p096MHz[] = {
    {0x0,0x1f8},
    {0x1,0x4},
    {0x2,0x4b},
    {0x3,0x0},
    {0x4,0xa86},
    {0x5,0x11a8},
    {0x7,0xaa4},
    {0x8,0x11e5},
    {0xa,0x1fe},
    {0xb,0x240},
    {0xc,0x98},
    {0x17,0x0},
    {0x18,0x4},
    {0x19,0x0},
    {0x1a,0x0},
    {0x20,0x1},
    {0x23,0x0},
    {0x24,0x0},
    {0x27,0x14},
    {0x38,0x0},
    {0x0, 0x0}
};

/* mode_1p1v_98p304MHz_4p096MHz */
static ndp120_pll_preset_value_t mode_1p1v_98p304MHz_4p096MHz[] = {
    {0x0,0x1f8},
    {0x1,0x4},
    {0x2,0x60},
    {0x3,0x0},
    {0x4,0xa86},
    {0x5,0x11a8},
    {0x7,0xaa4},
    {0x8,0x11e5},
    {0xa,0x1fe},
    {0xb,0x2e1},
    {0xc,0x98},
    {0x17,0x0},
    {0x18,0x4},
    {0x19,0x0},
    {0x1a,0x0},
    {0x20,0x1},
    {0x23,0x0},
    {0x24,0x0},
    {0x27,0x14},
    {0x38,0x0},
    {0x0, 0x0}
};

/* Define the table of PLL settings */
ndp120_pll_preset_t ndp120_pll_presets[] = {
    {"mode_0p9v_10p752MHz_32p768kHz", PLL_PRESET_OP_VOLTAGE_0p9, 32768, 10752000, mode_0p9v_10p752MHz_32p768kHz},
    {"mode_0p9v_15p360MHz_32p768kHz", PLL_PRESET_OP_VOLTAGE_0p9, 32768, 15360000, mode_0p9v_15p360MHz_32p768kHz},
    {"mode_0p9v_21p504MHz_32p768kHz", PLL_PRESET_OP_VOLTAGE_0p9, 32768, 21504000, mode_0p9v_21p504MHz_32p768kHz},
    {"mode_0p9v_15p360MHz_4p096MHz", PLL_PRESET_OP_VOLTAGE_0p9, 4096000, 15360000, mode_0p9v_15p360MHz_4p096MHz},
    {"mode_0p9v_21p504MHz_4p096MHz", PLL_PRESET_OP_VOLTAGE_0p9, 4096000, 21504000, mode_0p9v_21p504MHz_4p096MHz},
    {"mode_1p0v_49p152MHz_32p768kHz", PLL_PRESET_OP_VOLTAGE_1p0, 32768, 49152000, mode_1p0v_49p152MHz_32p768kHz},
    {"mode_1p0v_55p296MHz_32p768kHz", PLL_PRESET_OP_VOLTAGE_1p0, 32768, 55296000, mode_1p0v_55p296MHz_32p768kHz},
    {"mode_1p0v_49p152MHz_4p096MHz", PLL_PRESET_OP_VOLTAGE_1p0, 4096000, 49152000, mode_1p0v_49p152MHz_4p096MHz},
    {"mode_1p1v_55p296MHz_32p768kHz", PLL_PRESET_OP_VOLTAGE_1p1, 32768, 55296000, mode_1p1v_55p296MHz_32p768kHz},
    {"mode_1p1v_76p800MHz_32p768kHz", PLL_PRESET_OP_VOLTAGE_1p1, 32768, 76800000, mode_1p1v_76p800MHz_32p768kHz},
    {"mode_1p1v_98p304MHz_32p768kHz", PLL_PRESET_OP_VOLTAGE_1p1, 32768, 98304000, mode_1p1v_98p304MHz_32p768kHz},
    {"mode_1p1v_98p304MHz_0p512MHz", PLL_PRESET_OP_VOLTAGE_1p1, 512000, 98304000, mode_1p1v_98p304MHz_0p512MHz},
    {"mode_1p1v_76p800MHz_4p096MHz", PLL_PRESET_OP_VOLTAGE_1p1, 4096000, 76800000, mode_1p1v_76p800MHz_4p096MHz},
    {"mode_1p1v_98p304MHz_4p096MHz", PLL_PRESET_OP_VOLTAGE_1p1, 4096000, 98304000, mode_1p1v_98p304MHz_4p096MHz},
    {  NULL, 0, 0, 0, NULL}
};

ndp120_pll_preset_t ndp200_pll_presets[] = {
    {"mode_0p9v_21p504MHz_32p768kHz", PLL_PRESET_OP_VOLTAGE_0p9, 32768, 21504000, mode_0p9v_21p504MHz_32p768kHz},
    {"mode_1p0v_49p152MHz_32p768kHz", PLL_PRESET_OP_VOLTAGE_1p0, 32768, 49152000, mode_1p0v_49p152MHz_32p768kHz},
    {"mode_1p0v_55p296MHz_32p768kHz", PLL_PRESET_OP_VOLTAGE_1p0, 32768, 55296000, mode_1p0v_55p296MHz_32p768kHz},
    {"mode_1p1v_55p296MHz_32p768kHz", PLL_PRESET_OP_VOLTAGE_1p1, 32768, 55296000, mode_1p1v_55p296MHz_32p768kHz},
    {"mode_1p1v_76p800MHz_32p768kHz", PLL_PRESET_OP_VOLTAGE_1p1, 32768, 76800000, mode_1p1v_76p800MHz_32p768kHz},
    {"mode_1p1v_98p304MHz_32p768kHz", PLL_PRESET_OP_VOLTAGE_1p1, 32768, 98304000, mode_1p1v_98p304MHz_32p768kHz},
    {  NULL, 0, 0, 0, NULL}
};

/* Define the table of FLL settings */
ndp120_fll_preset_t ndp120_fll_presets[] = {
    {"mode_fll_0p9v_15p360MHz_32p768kHz", PLL_PRESET_OP_VOLTAGE_0p9, 32768, 15360000, 768000},
    {"mode_fll_0p9v_16p896MHz_32p768kHz", PLL_PRESET_OP_VOLTAGE_0p9, 32768, 16896000, 768000},
    { NULL, 0, 0, 0, 0}
};


/* exclude the null terminator element */
const unsigned ndp120_pll_presets_elements = ARRAY_LEN(ndp120_pll_presets) - 1;
const unsigned ndp120_fll_presets_elements = ARRAY_LEN(ndp120_fll_presets) - 1;

/* Define the table of FLL settings */
ndp120_fll_preset_t ndp200_fll_presets[] = {
    {"mode_fll_0p9v_24p000MHz_32p768kHz", PLL_PRESET_OP_VOLTAGE_0p9, 32768, 24000000, 768000},
    { NULL, 0, 0, 0, 0}
};

const unsigned ndp200_pll_presets_elements = ARRAY_LEN(ndp200_pll_presets) - 1;
const unsigned ndp200_fll_presets_elements = ARRAY_LEN(ndp200_fll_presets) - 1;

static enum ndp_device_type_e sc2_get_device_type(
    struct syntiant_ndp_device_s *ndp)
{
    if (0x30 <= ndp->device_type && ndp->device_type <= 0x3F) {
        return SYNTIANT_NDP120B0;
    } else if (0x40 <= ndp->device_type && ndp->device_type <= 0x43) {
        return SYNTIANT_NDP200A0;
    } else if (0x48 <= ndp->device_type && ndp->device_type <= 0x4B) {
        return SYNTIANT_NDP115A0;
    } else {
        return SYNTIANT_UNKNOWN_DEV;
    }
}

uint32_t ndp_get_scratch_address(struct syntiant_ndp_device_s *ndp)
{
    uint32_t addr = 0;
    switch (sc2_get_device_type(ndp)) {
    case SYNTIANT_NDP120B0:
    case SYNTIANT_NDP200A0:
        addr = NDP120_ILIB_SCRATCH_ORIGIN;
        break;
    case SYNTIANT_NDP115A0:
        addr = NDP115_ILIB_SCRATCH_ORIGIN;
        break;
    default:
        break;
    }
    return addr;
}

static int
syntiant_ndp120_config_clk_xtal_no_sync(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_clk_xtal_t *config)
{
    uint32_t data, data_old;
    int s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_ndp120_config_clk_xtal_t config_out = { 0 };

    if (!config->set && !config->get) goto error;

    s = ndp_mcu_read(NDP120_CHIP_CONFIG_CLKCTL2, &data);
    if (s) goto error;

    data_old = data;
    config_out.out = NDP120_CHIP_CONFIG_CLKCTL2_XTAL_OUT_EXTRACT(data);
    config_out.osc = NDP120_CHIP_CONFIG_CLKCTL2_XTAL_OSC_EXTRACT(data);

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_CLK_XTAL_OUT) {
        data = NDP120_CHIP_CONFIG_CLKCTL2_XTAL_OUT_MASK_INSERT(data, config->out);
    }
    if (config->set & SYNTIANT_NDP120_CONFIG_SET_CLK_XTAL_OSC) {
        data = NDP120_CHIP_CONFIG_CLKCTL2_XTAL_OSC_MASK_INSERT(data, config->osc);
    }
    if (data != data_old) {
        s = ndp_mcu_write(NDP120_CHIP_CONFIG_CLKCTL2, data);
        if (s) goto error;
    }
    *config = config_out;

error:
    return s;
}

int syntiant_ndp120_config_clk_xtal(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_clk_xtal_t *config)
{
    int s0, s = SYNTIANT_NDP_ERROR_NONE;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_ndp120_config_clk_xtal_no_sync(ndp, config);
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}


static int
update_pll_registers(struct syntiant_ndp_device_s *ndp)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t data;

    /* read/modify/write just in case other fields are added to register */
    s = ndp_mcu_read(NDP120_PLL_CONFIG_PLLCTL24, &data);
    if (s) goto error;

    data = NDP120_PLL_CONFIG_PLLCTL24_REG_UPDATE_MASK_INSERT(data, 1);
    s = ndp_mcu_write(NDP120_PLL_CONFIG_PLLCTL24, data);
    if (s) goto error;

    data = NDP120_PLL_CONFIG_PLLCTL24_REG_UPDATE_MASK_INSERT(data, 0);
    s = ndp_mcu_write(NDP120_PLL_CONFIG_PLLCTL24, data);
    if (s) goto error;

error:
    return s;
}

static int toggle_all_pdmctl_update(struct syntiant_ndp_device_s *ndp) {
    unsigned int i;
    int s;
    uint32_t pdmctl_data, adx;


    for(i=0; i < NDP120_DSP_CONFIG_PDMCTL_COUNT; ++i) {
        adx = NDP120_DSP_CONFIG_PDMCTL(i);
        s = ndp_mcu_read(adx, &pdmctl_data);
        if(s) goto error;

        pdmctl_data = NDP120_DSP_CONFIG_PDMCTL_UPDATE_MASK_INSERT(pdmctl_data, 0);
        s = ndp_mcu_write(adx, pdmctl_data);
        if (s) goto error;

        /* toggle=1 */
        pdmctl_data = NDP120_DSP_CONFIG_PDMCTL_UPDATE_MASK_INSERT(pdmctl_data, 1);
        s = ndp_mcu_write(adx, pdmctl_data);
        if (s) goto error;

        /* toggle=0 */
        pdmctl_data = NDP120_DSP_CONFIG_PDMCTL_UPDATE_MASK_INSERT(pdmctl_data, 0);
        s = ndp_mcu_write(adx, pdmctl_data);
        if (s) goto error;
    }

error:
    return s;
}

static int
config_calc_mult_mult(unsigned int* core_clock_rate,
            unsigned int input_clock_rate, unsigned int* clock_mult,
            unsigned int* clock_div)
{
    int s = SYNTIANT_NDP_ERROR_NONE;

    /*
     * compute clock multiplier with 3 frac bits, but note
     * the FLL target is actually core_clock_rate / 2
     * numerator factor is / 2 for core_clock_rate / 2,
     * and * 2 for 1 fractional bit with which to round up
     */
    *clock_mult = (uint32_t) (*core_clock_rate << (3 - 1 + 1)) / input_clock_rate;

    /* round up */
    *clock_mult = (*clock_mult / 2) + (*clock_mult & 0x1);

    /*
     * compute reciprocal of clock multiplier with (all) 14 frac bits
     * for tracking.
     * A simple multiplication by 2^14 would overflow the clock rate
     * in the worst case (4MHz), so we multiply by the maximum
     * 2^10 and divide the core clock by 2^4(+1-1) to ensure no overflow
     * again note the FLL target is actually core_clock_rate / 2
     * denominator factor is / 2 for core_clock_rate / 2,
     * and * 2 for 1 fractional bit with which to round up
     */
    *clock_div = (uint32_t) (input_clock_rate << 10)
                 / (*core_clock_rate >> (4 + 1 + 1));

    /* round up */
    *clock_div = (*clock_div / 2) + (*clock_div & 0x1);

    /*
     * Update core clock with real divisor
     * * 2 for core_clock_rate / 2,
     * * 2  for 1 fractional bit with which to round up
     */
    *core_clock_rate = (input_clock_rate * (*clock_mult)) >> (3 - 1 - 1);
    *core_clock_rate = (*core_clock_rate / 2) + (*core_clock_rate & 0x1);

    return s;
}

static int syntiant_ndp120_set_ema_setting_based_on_volt(
            struct syntiant_ndp_device_s *ndp, int operating_voltage)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t data, ema;

    if (operating_voltage == PLL_PRESET_OP_VOLTAGE_0p9) {
        ema = SYNTIANT_NDP120_EMA_7;
    } else if (operating_voltage == PLL_PRESET_OP_VOLTAGE_1p0 ||
        operating_voltage == PLL_PRESET_OP_VOLTAGE_1p1) {
        ema = SYNTIANT_NDP120_EMA_4;
    } else {
        s = SYNTIANT_NDP_ERROR_CONFIG;
        goto error;
    }

    /* chip_config.mem.margin_read = ema; */
    s = ndp_mcu_read(NDP120_CHIP_CONFIG_MEM, &data);
    if (s) goto error;
    data = NDP120_CHIP_CONFIG_MEM_MARGIN_READ_MASK_INSERT(data, ema);
    s = ndp_mcu_write(NDP120_CHIP_CONFIG_MEM, data);
    if (s) goto error;

    /* chip_config.dsp_mem_assist.margin_read = ema; */
    s = ndp_mcu_read(NDP120_CHIP_CONFIG_DSP_MEM_ASSIST, &data);
    if (s) goto error;
    data = NDP120_CHIP_CONFIG_DSP_MEM_ASSIST_MARGIN_READ_MASK_INSERT(data, ema);
    s = ndp_mcu_write(NDP120_CHIP_CONFIG_DSP_MEM_ASSIST, data);
    if (s) goto error;

    /* dnn_config.dnnmemcfg.ema = ema; */
    s = ndp_mcu_read(NDP120_DNN_CONFIG_DNNMEMCFG, &data);
    if (s) goto error;
    data = NDP120_DNN_CONFIG_DNNMEMCFG_EMA_MASK_INSERT(data, ema);
    s = ndp_mcu_write(NDP120_DNN_CONFIG_DNNMEMCFG, data);
    if (s) goto error;

error:
    return s;
}

static int
syntiant_ndp120_config_clk_src_no_sync(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_clk_src_t *config)
{
    uint32_t data, data_old;
    uint8_t ctl, ctl_old;
    syntiant_ndp120_config_clk_src_t config_out = { 0 };
    int s = SYNTIANT_NDP_ERROR_NONE;
    const char *chip_name;
    enum pbi_version_chip_type_t_ chip_type = 0;
    chip_name = syntiant_ndp120_get_config_devtype(ndp->device_type,
        &chip_type);
    if (!strcmp(chip_name,"unknown")) {
        s = SYNTIANT_NDP_ERROR_FAIL;
        goto error;
    }
    if (!config->set && !config->get) goto error;

    s = ndp_mcu_read(NDP120_CHIP_CONFIG_CLKCTL1, &data);
    if (s) goto error;

    data_old = data;
    config_out.refsel = NDP120_CHIP_CONFIG_CLKCTL1_REFSEL_EXTRACT(data);
    config_out.clksel = NDP120_CHIP_CONFIG_CLKCTL1_CLKSEL_EXTRACT(data);
    config_out.extsel = NDP120_CHIP_CONFIG_CLKCTL1_EXTSEL_EXTRACT(data);

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_REFSEL) {
        /* config->refsel ? xtalin : clkin */
        data = NDP120_CHIP_CONFIG_CLKCTL1_REFSEL_MASK_INSERT(
            data, config->refsel);
        /* make sure ie and smt is 1 regardless of refsel value */
        data = NDP120_CHIP_CONFIG_CLKCTL1_CLK_IE_MASK_INSERT(
            data, 1);
        data = NDP120_CHIP_CONFIG_CLKCTL1_CLK_SMT_MASK_INSERT(
            data, 1);
    }
    if (config->set & SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_CLKSEL) {
        data = NDP120_CHIP_CONFIG_CLKCTL1_CLKSEL_MASK_INSERT(
            data, config->clksel);
        DEBUG_PRINTF("config_clk_src: setting clksel=%s\n",
            config->clksel ? "PLL" : "FLL");
    }
    if (config->set & SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_EXTSEL) {
        data = NDP120_CHIP_CONFIG_CLKCTL1_EXTSEL_MASK_INSERT(
            data, config->extsel);
        DEBUG_PRINTF("config_clk_src: setting extsel=%s\n",
            config->extsel ? "EXT" : "INT");
    }
    if (data != data_old) {
        s = ndp_mcu_write(NDP120_CHIP_CONFIG_CLKCTL1, data);
        if (s) goto error;
    }

    s = ndp_mcu_read(NDP120_CHIP_CONFIG_CLKCTL0, &data);
    if (s) goto error;

    data_old = data;
    config_out.force_dnn_clk
        = NDP120_CHIP_CONFIG_CLKCTL0_FORCE_DNN_CLK_EXTRACT(data);
    config_out.force_aesotp_clk
        = NDP120_CHIP_CONFIG_CLKCTL0_FORCE_AESOTP_CLK_EXTRACT(data);

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_FORCE_DNN_CLK) {
        data = NDP120_CHIP_CONFIG_CLKCTL0_FORCE_DNN_CLK_MASK_INSERT(
            data, config->force_dnn_clk);
    }
    if (config->set & SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_FORCE_AESOTP_CLK) {
        data = NDP120_CHIP_CONFIG_CLKCTL0_FORCE_AESOTP_CLK_MASK_INSERT(
            data, config->force_aesotp_clk);
    }
    if (data != data_old) {
        s = ndp_mcu_write(NDP120_CHIP_CONFIG_CLKCTL0, data);
        if (s) goto error;
    }

    s = ndp_spi_read(NDP120_SPI_CTL, &ctl);
    if (s) goto error;

    ctl_old = ctl;

    config_out.force_ext = NDP120_SPI_CTL_EXTSEL_EXTRACT(ctl);
    config_out.force_int = NDP120_SPI_CTL_INTSEL_EXTRACT(ctl);

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_FORCE_EXT) {
        ctl = (uint8_t) NDP120_SPI_CTL_EXTSEL_MASK_INSERT(
            ctl, config->force_ext);
    }
    if (config->set & SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_FORCE_INT) {
        if (config->force_int) {
            /* have to clear extsel for intsel to take effect */
            ctl = (uint8_t) NDP120_SPI_CTL_EXTSEL_MASK_INSERT(ctl, 0);
        }
        ctl = (uint8_t) NDP120_SPI_CTL_INTSEL_MASK_INSERT(
            ctl, config->force_int);
    }
    if (ctl != ctl_old) {
        s = ndp_spi_write(NDP120_SPI_CTL, ctl);
        if (s) goto error;
    }
    *config = config_out;

    /* don't check error -- DSP may not be loaded */
    (void)syntiant_ndp120_update_clock_rate_in_dsp(ndp);

error:
    return s;
}

static int
get_put_int(struct syntiant_ndp_device_s *ndp, uint32_t adx, uint32_t mask, uint32_t *value)
{
    int s;
    uint32_t valid;

    if (value == NULL) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    s = syntiant_ndp120_scratch_get_valid(ndp, &valid);
    if (s && s != SYNTIANT_NDP_ERROR_CRC) goto error;
    s = 0;

    if (*value == 0xFFFFFFFF) {
        /* invalidate */
        s = syntiant_ndp120_scratch_set_valid(ndp, valid & (uint32_t)(~mask));
        if (s) goto error;
        *value = 0;
    } else if (*value) {
        /* put */
        s = ndp_mcu_write(adx, *value);
        if (s) goto error;
        valid |= mask;
        s = syntiant_ndp120_scratch_set_valid(ndp, valid);
        if (s) goto error;
    } else if (valid & mask) {
        /* get */
        s = ndp_mcu_read(adx, value);
        if (s) goto error;
    } else {
        /* get, but not valid */
        *value = 0;
    }

error:
    return s;
}

static int syntiant_ndp120_get_put_pll_clk_freq_no_sync(
    struct syntiant_ndp_device_s *ndp, uint32_t *freq)
{
    return get_put_int(ndp, SCRATCH_PLL_CLK_FREQ_ADX,
            SYNTIANT_CONFIG_PLL_CLK_FREQ_VALID, freq);
}

static int syntiant_ndp120_get_put_fll_clk_freq_no_sync(
    struct syntiant_ndp_device_s *ndp, uint32_t *freq)
{
    return get_put_int(ndp, SCRATCH_FLL_CLK_FREQ_ADX,
            SYNTIANT_CONFIG_FLL_CLK_FREQ_VALID, freq);
}

static int syntiant_ndp120_get_put_ext_clk_freq_no_sync(
    struct syntiant_ndp_device_s *ndp, uint32_t *freq)
{
    return get_put_int(ndp, SCRATCH_EXT_CLK_FREQ_ADX,
            SYNTIANT_CONFIG_EXT_CLK_FREQ_VALID, freq);
}

static int inspect_frequency_voltage_combination(
        int operating_voltage, uint32_t freq)
{
    int s = SYNTIANT_NDP_ERROR_CONFIG;

    switch (operating_voltage) {
        case PLL_PRESET_OP_VOLTAGE_0p9:
            if (freq == SYNTINAT_NDP120_LOWPOWER_MODE_10p752MHz ||
                freq == SYNTINAT_NDP120_LOWPOWER_MODE_15p360MHz ||
                freq == SYNTIANT_NDP120_LOWPOWER_MODE_16p896MHz ||
                freq == SYNTINAT_NDP120_LOWPOWER_MODE_21p504MHz ||
                freq == SYNTINAT_NDP120_LOWPOWER_MODE_24p000MHz ||
                freq == SYNTINAT_NDP120_LOWPOWER_MODE_30p720MHz) {
                s = SYNTIANT_NDP_ERROR_NONE;
            }
            break;

        case PLL_PRESET_OP_VOLTAGE_1p0:
            if (freq == SYNTINAT_NDP120_NORMALPOWER_MODE_49p152MHz ||
                freq == SYNTINAT_NDP120_NORMALPOWER_MODE_55p286MHz ||
                freq == SYNTINAT_NDP120_NORMALPOWER_MODE_61p440MHz) {
                s = SYNTIANT_NDP_ERROR_NONE;
            }
            break;

        case PLL_PRESET_OP_VOLTAGE_1p1:
            if (freq == SYNTINAT_NDP120_NORMALPOWER_MODE_55p286MHz||
                freq == SYNTINAT_NDP120_HIGHPOWER_MODE_76p800MHz ||
                freq == SYNTINAT_NDP120_HIGHPOWER_MODE_98p304MHz) {
                s = SYNTIANT_NDP_ERROR_NONE;
            }
            break;

        default:
            s = SYNTIANT_NDP_ERROR_CONFIG;
            break;
    }
    return s;
}

int
syntiant_ndp120_config_clk_src(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_clk_src_t *config)
{
    int s0, s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t input_freq = 0;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    if (config->extsel) {
        /* for external clk source freq should be freq <= 21.504 MHz, 0.9V*/
        s = syntiant_ndp120_get_put_ext_clk_freq_no_sync(ndp, &input_freq);
        if (s) goto error;
        s = inspect_frequency_voltage_combination(PLL_PRESET_OP_VOLTAGE_0p9,
            input_freq);
        if (s) {
            DEBUG_PRINTF("Invalid clk freq and voltage combination\n");
            goto error;
        }
        s = syntiant_ndp120_set_ema_setting_based_on_volt(ndp,
            PLL_PRESET_OP_VOLTAGE_0p9);
        if (s) goto error;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_REFSEL) {
        syntiant_ndp120_config_clk_xtal_t config_clk_xtal = {
            0, SYNTIANT_NDP120_CONFIG_SET_CLK_XTAL_OUT | SYNTIANT_NDP120_CONFIG_SET_CLK_XTAL_OSC, 0, 0
        };
        if (config->refsel) {
            config_clk_xtal.osc = config_clk_xtal.out = 1;
        }
        s = syntiant_ndp120_config_clk_xtal_no_sync(ndp, &config_clk_xtal);
        if (s) goto error;
        /* delay 100 ms */
    }

    s = syntiant_ndp120_config_clk_src_no_sync(ndp, config);
    if(s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;

    return s;
}

static int
wait_for_pll_lock(struct syntiant_ndp_device_s *ndp, long int timeout_ms)
{
    int s, locked = 0;
    uint32_t data;
    syntiant_ms_time timeout;
    syntiant_get_ms_time(&timeout);

    DEBUG_PRINTF("->> wait_for_pll_lock:\n");
    do {
        /* write to data_strobe */
        s = ndp_mcu_read(NDP120_PLL_CONFIG_PLLCTL15, &data);
        if (s) {
            DEBUG_PRINTF("wait_for_pll_lock: read PLLCTL15 failed s=%d\n", s);
            goto error;
        }
        data = NDP120_PLL_CONFIG_PLLCTL15_SAMPLE_STROBE_MASK_INSERT(data, 1);
        s = ndp_mcu_write(NDP120_PLL_CONFIG_PLLCTL15, data);
        if (s) {
            DEBUG_PRINTF("wait_for_pll_lock: write PLLCTL15 failed s=%d\n", s);
            goto error;
        }
        s = ndp_mcu_read(NDP120_PLL_CONFIG_PLLSTS7, &data);
        if (s) {
            DEBUG_PRINTF("wait_for_pll_lock: read PLLSTS7 failed s=%d\n", s);
            goto error;
        }
        if (NDP120_PLL_CONFIG_PLLSTS7_LOCK_DETECT_EXTRACT(data)) {
            DEBUG_PRINTF("wait_for_pll_lock: PLL locked! (PLLSTS7=%#x)\n", data);
            locked = 1;
            goto error;
        }
    } while (syntiant_get_ms_elapsed(&timeout) < (unsigned long)timeout_ms);
    DEBUG_PRINTF("wait_for_pll_lock: timeout (PLLSTS7=%#x)\n", data);
error:
    return locked;
}

static int
wait_for_fll_lock(struct syntiant_ndp_device_s *ndp, long int timeout_ms)
{
    int s, locked = 0;
    uint32_t data;
    syntiant_ms_time timeout;
    syntiant_get_ms_time(&timeout);

    DEBUG_PRINTF("->> wait_for_fll_lock:\n");
    do {
        s = ndp_mcu_read(NDP120_CHIP_CONFIG_FLLSTS0, &data);
        if (s) {
            DEBUG_PRINTF("wait_for_fll_lock: read FLLSTS0 failed s=%d\n", s);
            goto error;
        }
        if (NDP120_CHIP_CONFIG_FLLSTS0_LOCKED_EXTRACT(data)) {
            DEBUG_PRINTF("wait_for_fll_lock: FLL locked! (FLLSTS0=%#x)\n", data);
            locked = 1;
            goto error;
        }
    } while (syntiant_get_ms_elapsed(&timeout) < (unsigned long)timeout_ms);
    DEBUG_PRINTF("wait_for_fll_lock: timeout (FLLSTS0=%#x)\n", data);
error:
    return locked;
}

/* use PLL or else FLL */
static void select_fll_pll(struct syntiant_ndp_device_s *ndp, int pll) {
    syntiant_ndp120_config_clk_src_t src_config = { 0 };
    DEBUG_PRINTF("~~> select_fll_pll: switching clock (pll=%d)\n", pll);
    src_config.set |= SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_CLKSEL;
    src_config.set |= SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_EXTSEL;
    src_config.clksel = !!pll;
    src_config.extsel = 0;
    syntiant_ndp120_config_clk_src_no_sync(ndp, &src_config);
}

int syntiant_ndp120_get_put_pll_clk_freq(struct syntiant_ndp_device_s *ndp,
        uint32_t *freq)
{
    int s;
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;
    s = get_put_int(ndp, SCRATCH_PLL_CLK_FREQ_ADX,
            SYNTIANT_CONFIG_PLL_CLK_FREQ_VALID, freq);
    if (s) goto error;
error:
    s = (ndp->iif.unsync)(ndp->iif.d);
    return s;
}

int syntiant_ndp120_get_put_ext_clk_freq(struct syntiant_ndp_device_s *ndp,
        uint32_t *freq) {
    int s;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;
    s = syntiant_ndp120_get_put_ext_clk_freq_no_sync(ndp, freq);
    if (s) goto error;
error:
    s = (ndp->iif.unsync)(ndp->iif.d);
    return s;
}

int
syntiant_ndp120_config_clk_fll(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_clk_fll_t *config)
{
    ndp120_fll_preset_t *preset;
    uint8_t ctl;
    unsigned int input_clock_rate = 0;
    unsigned int core_clock_rate = 0;
    unsigned int clock_mult, clock_div, module_clock_div;
    uint32_t clkctl0, fllctl0, fllctl1, fllctl2;

    /* this value index different for each config */
    uint32_t data;
    uint32_t valid;
    uint32_t adx;
    syntiant_ndp120_config_clk_fll_t config_out = { 0 };
    static uint32_t prev_config_preset = -1U;
    int s0, s = SYNTIANT_NDP_ERROR_NONE;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    if (!config->set && !config->get) goto error;

    s = syntiant_ndp120_scratch_get_valid(ndp, &valid);
    if (s && s != SYNTIANT_NDP_ERROR_CRC) goto error;
    s = 0;

    if (config->get) {
        if (valid & SYNTIANT_CONFIG_FLL_PRESET_VALID) {
            adx = SCRATCH_FLL_PRESET;
            s = ndp_mcu_read(adx, &config_out.preset);
            if (s) goto error;
        } else {
            config_out.preset = prev_config_preset;
        }
        s = ndp_mcu_read(NDP120_CHIP_CONFIG_FLLSTS0, &data);
        if (s) goto error;
        config_out.locked = NDP120_CHIP_CONFIG_FLLSTS0_LOCKED_EXTRACT(data);
        DEBUG_PRINTF("FLL: config_out.locked=%d\n", config_out.locked);
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_CLK_FLL_PRESET) {
        if (sc2_get_device_type(ndp) == SYNTIANT_NDP200A0) {
            if (config->preset >= (ARRAY_LEN(ndp200_fll_presets) - 1)) {
                s = SYNTIANT_NDP_ERROR_ARG;
                goto error;
            }
            preset = &ndp200_fll_presets[config->preset];
        } else {
            if (config->preset >= (ARRAY_LEN(ndp120_fll_presets) - 1)) {
                s = SYNTIANT_NDP_ERROR_ARG;
                goto error;
            }
            preset = &ndp120_fll_presets[config->preset];
        }

        DEBUG_PRINTF("syntiant_ndp120_config_clk_fll: "
            "preset=%d input_freq=%d output_freq=%d pdm_freq=%d\n",
            config->preset, preset->input_freq, preset->output_freq,
            preset->pdm_freq);

        s = inspect_frequency_voltage_combination(preset->operating_voltage,
            preset->output_freq);
        if (s) {
            DEBUG_PRINTF("Invalid clk freq and voltage combination\n");
            goto error;
        }

        s = ndp_spi_read(NDP120_SPI_CTL, &ctl);
        if (s) goto error;

        /* retrieve input clock */
        s = syntiant_ndp120_get_put_ext_clk_freq_no_sync(
            ndp, (uint32_t *)&input_clock_rate);
        if (s) goto error;
        DEBUG_PRINTF("syntiant_ndp120_config_clk_fll: input_clock_rate=%d\n",
            input_clock_rate);

        /* for ndp200 there is no external clock */
        if (sc2_get_device_type(ndp) == SYNTIANT_NDP200A0) {
            input_clock_rate = preset->input_freq;
        }

        /* Determine internal clock rate */
        /* Ensure what FLL plans to use as input freq is the same
           as previously set */
        if ((preset->input_freq != input_clock_rate) || !input_clock_rate) {
            s = SYNTIANT_NDP_ERROR_CONFIG;
            goto error;
        }

        /* -------------------------------------------------------
         * Configures the internal FLL
         * ------------------------------------------------------- */

        /* find out if external clock is already on and turn it off */
        if (NDP120_SPI_CTL_EXTSEL_EXTRACT(ctl)) {
            ctl = (uint8_t) NDP120_SPI_CTL_EXTSEL_MASK_INSERT(ctl, 0);
            s = ndp_spi_write(NDP120_SPI_CTL, ctl);
            if (s) goto error;
        }

        /* select FLL */
        select_fll_pll(ndp, 0);

        /* turn on FLL ref clock clockgate in fllctl0 */
        fllctl0 = NDP120_CHIP_CONFIG_FLLCTL0_DEFAULT;
        fllctl0 = NDP120_CHIP_CONFIG_FLLCTL0_CLK_ENABLE_INSERT(fllctl0, 1);
        s = ndp_mcu_write(NDP120_CHIP_CONFIG_FLLCTL0, fllctl0);
        if (s) goto error;

        /* calculate targeted core clock rate */
        core_clock_rate = preset->output_freq;
        config_calc_mult_mult(&core_clock_rate, input_clock_rate,
            &clock_mult, &clock_div);
        DEBUG_PRINTF("config_calc_mult_mult: core_clock_rate=%d, "
            "input_clock_rate=%d, clock_mult=%#x, clock_div=%#x\n",
            core_clock_rate, input_clock_rate, clock_mult, clock_div);

        /* Forward write (no read/modify/write til clock rate is high */
        fllctl1 = NDP120_CHIP_CONFIG_FLLCTL1_DEFAULT;
        fllctl1 = NDP120_CHIP_CONFIG_FLLCTL1_FREQMULT_MASK_INSERT(
            fllctl1, clock_mult);
        fllctl1 = NDP120_CHIP_CONFIG_FLLCTL1_FREQDIV_MASK_INSERT(
            fllctl1, clock_div);
        s = ndp_mcu_write(NDP120_CHIP_CONFIG_FLLCTL1, fllctl1);
        if (s) goto error;

        /* fix for #108 [HLS - ClockMult] FLL loop gain bug */
        fllctl2 = NDP120_CHIP_CONFIG_FLLCTL2_DEFAULT;
        fllctl2 = NDP120_CHIP_CONFIG_FLLCTL2_LOOPGAIN1_MASK_INSERT(
            fllctl2, 0x7);
        s = ndp_mcu_write(NDP120_CHIP_CONFIG_FLLCTL2, fllctl2);
        if (s) goto error;

        /* Turn on enable, enable tracking, and clockgate enable */
        fllctl0 = NDP120_CHIP_CONFIG_FLLCTL0_DEFAULT;
        fllctl0 = NDP120_CHIP_CONFIG_FLLCTL0_ENABLETRACKING_MASK_INSERT(fllctl0, 1);
        fllctl0 = NDP120_CHIP_CONFIG_FLLCTL0_ENABLE_MASK_INSERT(fllctl0, 1);
        fllctl0 = NDP120_CHIP_CONFIG_FLLCTL0_CLK_ENABLE_INSERT(fllctl0, 1);
        s = ndp_mcu_write(NDP120_CHIP_CONFIG_FLLCTL0, fllctl0);
        if (s) goto error;

        /* wait for FLL lock */
        if (!wait_for_fll_lock(ndp, FLL_LOCK_WAIT_IN_MSEC)) {
            s = SYNTIANT_NDP_ERROR_FAIL;
            DEBUG_PRINTF("FLL unlocked\n");
            goto error;
        }
        DEBUG_PRINTF("FLL locked\n");

        /* -------------------------------------------------------
         *    This sections updates internal clock dividers
         * ------------------------------------------------------- */
        module_clock_div = SYNTIANT_NDP120_CLKCTL0_MCUCLKDIV_NO_DIV;
        /* For NDP200 don't divide the clk */
        if (core_clock_rate > SYNTIANT_NDP120_MAX_MCU_RATE &&
                sc2_get_device_type(ndp) != SYNTIANT_NDP200A0) {
            module_clock_div = SYNTIANT_NDP120_CLKCTL0_MCUCLKDIV_DIV2;
        }

        clkctl0 = NDP120_CHIP_CONFIG_CLKCTL0_MCUCLKDIV_MASK_INSERT(
            NDP120_CHIP_CONFIG_CLKCTL0_DEFAULT, module_clock_div);
        s = ndp_mcu_write(NDP120_CHIP_CONFIG_CLKCTL0, clkctl0);
        if (s) goto error;

        adx = SCRATCH_FLL_PRESET;
        s = ndp_mcu_write(adx, config->preset);
        if (s) goto error;
        valid |= SYNTIANT_CONFIG_FLL_PRESET_VALID;
        s = syntiant_ndp120_scratch_set_valid(ndp, valid);
        if (s) goto error;
        prev_config_preset = config->preset;

        /* save core clock rate in scratch area */
        data = core_clock_rate;
        s = syntiant_ndp120_get_put_fll_clk_freq_no_sync(ndp, &data);
        if (s) goto error;

        s = syntiant_ndp120_set_ema_setting_based_on_volt(ndp,
            preset->operating_voltage);
        if (s) goto error;
    }

    *config = config_out;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;

    return s;
}

int
syntiant_ndp120_config_clk_pll(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_clk_pll_t *config)
{
    ndp120_pll_preset_t *preset;
    ndp120_pll_preset_value_t *preset_value;

    /* this value index different for each config */
    uint32_t data;
    uint32_t valid;
    uint32_t adx;
    syntiant_ndp120_config_clk_pll_t config_out = { 0 };
    static uint32_t prev_config_preset = -1U;
    int s0, s = SYNTIANT_NDP_ERROR_NONE;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    if (!config->set && !config->get) goto error;

    if (config->set && config->get) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    s = syntiant_ndp120_scratch_get_valid(ndp, &valid);
    if (s && s != SYNTIANT_NDP_ERROR_CRC) goto error;
    s = 0;

    if (config->get) {
        if (valid & SYNTIANT_CONFIG_PLL_PRESET_VALID) {
            adx = SCRATCH_PLL_PRESET;
            s = ndp_mcu_read(adx, &config_out.preset);
            if (s) goto error;
        } else {
            config_out.preset = prev_config_preset;
        }
        /* write to data_strobe */
        s = ndp_mcu_read(NDP120_PLL_CONFIG_PLLCTL15, &data);
        if (s) goto error;
        data = NDP120_PLL_CONFIG_PLLCTL15_SAMPLE_STROBE_MASK_INSERT(data, 1);
        s = ndp_mcu_write(NDP120_PLL_CONFIG_PLLCTL15, data);
        if (s) goto error;
        s = ndp_mcu_read(NDP120_PLL_CONFIG_PLLSTS7, &data);
        if (s) goto error;
        config_out.locked = NDP120_PLL_CONFIG_PLLSTS7_LOCK_DETECT_EXTRACT(data);
        DEBUG_PRINTF("PLL config_out.locked=%d\n", config_out.locked);
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_CLK_PLL_PRESET) {
        if (config->preset >= (ARRAY_LEN(ndp120_pll_presets) - 1)) {
            s = SYNTIANT_NDP_ERROR_ARG;
            goto error;
        }
        preset = &ndp120_pll_presets[config->preset];
        preset_value = preset->values;

        s = inspect_frequency_voltage_combination(preset->operating_voltage,
            preset->output_freq);
        if (s) {
            DEBUG_PRINTF("Invalid clk freq and voltage combination\n");
            goto error;
        }

        /* select FLL */
        select_fll_pll(ndp, 0);

        /* disable PLL */
        s = ndp_mcu_read(NDP120_PLL_CONFIG_PLLCTL0, &data);
        if (s) goto error;
        data = NDP120_PLL_CONFIG_PLLCTL0_PLL_ENABLE_MASK_INSERT(data, 0);
        s = ndp_mcu_write(NDP120_PLL_CONFIG_PLLCTL0, data);
        if (s) goto error;

        /* tell the pll to update the regs */
        s = update_pll_registers(ndp);
        if (s) goto error;

        /* updates for other regs*/
        /* Offsets are byte offsets, must multiply by 4 for proper reg */
        while (preset_value->offset != 0 || preset_value->value != 0) {
            adx = NDP120_PLL_CONFIG + (uint32_t) (preset_value->offset << 2);
            s = ndp_mcu_write(adx, preset_value->value);
            if (s) goto error;
            ++preset_value;
        }
        adx = SCRATCH_PLL_PRESET;
        s = ndp_mcu_write(adx, config->preset);
        if (s) goto error;
        valid |= SYNTIANT_CONFIG_PLL_PRESET_VALID;
        s = syntiant_ndp120_scratch_set_valid(ndp, valid);
        if (s) goto error;
        prev_config_preset = config->preset;

        data = preset->output_freq;
        s = syntiant_ndp120_get_put_pll_clk_freq_no_sync(ndp, &data);
        if (s) goto error;

        /* tell the pll to update the regs */
        s = update_pll_registers(ndp);
        if (s) goto error;

        /* toggle the pll_config.pllctl0.dco_norm_enable 1 --> 0 --> 1 */
        s = ndp_mcu_read(NDP120_PLL_CONFIG_PLLCTL0, &data);
        if (s) goto error;
        data = NDP120_PLL_CONFIG_PLLCTL0_DCO_NORM_ENABLE_MASK_INSERT(data, 0);
        s = ndp_mcu_write(NDP120_PLL_CONFIG_PLLCTL0, data);
        if (s) goto error;

        /* tell the pll to update the regs */
        s = update_pll_registers(ndp);
        if (s) goto error;

        s = ndp_mcu_read(NDP120_PLL_CONFIG_PLLCTL0, &data);
        if (s) goto error;

        data = NDP120_PLL_CONFIG_PLLCTL0_DCO_NORM_ENABLE_MASK_INSERT(data, 1);
        s = ndp_mcu_write(NDP120_PLL_CONFIG_PLLCTL0, data);
        if (s) goto error;

        /* tell the pll to update the regs */
        s = update_pll_registers(ndp);
        if (s) goto error;

        /* enable PLL */
        s = ndp_mcu_read(NDP120_PLL_CONFIG_PLLCTL0, &data);
        if (s) goto error;
        data = NDP120_PLL_CONFIG_PLLCTL0_PLL_ENABLE_MASK_INSERT(data, 1);
        s = ndp_mcu_write(NDP120_PLL_CONFIG_PLLCTL0, data);
        if (s) goto error;

        /* tell the pll to update the regs */
        s = update_pll_registers(ndp);
        if (s) goto error;

        if (!wait_for_pll_lock(ndp, PLL_LOCK_WAIT_IN_MSEC)) {
            DEBUG_PRINTF("PLL unlocked...\n");
        } else {
            DEBUG_PRINTF("PLL locked\n");
        }
        if (!config->no_switch) {
            select_fll_pll(ndp, 1);
            /* Since PLL doesn't lock always at the first instance, setting
             * a wait loop to give it sufficient time for locking
             */
            if (!wait_for_pll_lock(ndp, PLL_LOCK_WAIT_IN_MSEC)) {
                s = SYNTIANT_NDP_ERROR_FAIL;
                DEBUG_PRINTF("PLL still unlocked\n");
                goto error;
            }
            DEBUG_PRINTF("PLL locked 2\n");
        }
        /* if crystal is used, after PLL locks, set ie and smt to 0 */
        s = ndp_mcu_read(NDP120_CHIP_CONFIG_CLKCTL1, &data);
        if (s) goto error;

        if (NDP120_CHIP_CONFIG_CLKCTL1_REFSEL_EXTRACT(data)) {
            data = NDP120_CHIP_CONFIG_CLKCTL1_CLK_IE_MASK_INSERT(data, 0);
            data = NDP120_CHIP_CONFIG_CLKCTL1_CLK_SMT_MASK_INSERT(data, 0);
            s = ndp_mcu_write(NDP120_CHIP_CONFIG_CLKCTL1, data);
            if (s) goto error;
        }


        /* disable the oscillator */
        s = ndp_mcu_read(NDP120_CHIP_CONFIG_FLLCTL0, &data);
        if (s) goto error;
        data = NDP120_CHIP_CONFIG_FLLCTL0_ENABLE_OSC_MASK_INSERT(data, 0);
        s = ndp_mcu_write(NDP120_CHIP_CONFIG_FLLCTL0, data);
        if (s) goto error;
        s = syntiant_ndp120_set_ema_setting_based_on_volt(ndp,
            preset->operating_voltage);
        if (s) goto error;
    }

    *config = config_out;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;

    return s;
}

static int syntiant_ndp120_dev_wfi_control(struct syntiant_ndp_device_s *ndp,
        syntiant_ndp120_wfi_control_t wfi_control)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t adx, payload_in_MSB = 0;
    adx = NDP120_DSP_MB_H2D_ADDR;
    payload_in_MSB = NDP120_H2D_MB_SET_PAYLOAD(payload_in_MSB, (uint32_t) wfi_control);
    s = syntiant_ndp120_write_block(ndp, 1, adx, &payload_in_MSB,
                                    sizeof(uint32_t));
    if (s) goto error;
    s = syntiant_ndp120_do_mailbox_req_no_sync(
        ndp, NDP120_DSP_MB_H2D_EXT_WFI_CONTROL, NULL);
    if (s) goto error;
    /* send H2M MB */
    s = syntiant_ndp120_do_mailbox_req_no_sync(
        ndp, wfi_control ?
            NDP_MBIN_REQUEST_WFI_CONTROL_ON : NDP_MBIN_REQUEST_WFI_CONTROL_OFF,
            NULL);
    if (s) goto error;
error:
    return s;
}

int
syntiant_ndp120_config_clk_div(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_clk_div_t *config)
{
    uint32_t data, main_clk = ndp->d.ndp120.core_clock_freq;
    uint32_t adx, payload_in_MSB = 0;
    int s0, s = SYNTIANT_NDP_ERROR_NONE;
    syntiant_ndp120_config_clk_div_t config_out = {0};

    DEBUG_PRINTF("~~> config_clk_div: mcuclkdiv=%u\n", config->mcuclkdiv);
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    if (!config->set && !config->get) goto error;

    /* mcu clkdiv */
    s = ndp_mcu_read(NDP120_CHIP_CONFIG_CLKCTL0, &data);
    if (s) goto error;
    config_out.mcuclkdiv = NDP120_CHIP_CONFIG_CLKCTL0_MCUCLKDIV_EXTRACT(data);
    if (!main_clk) {
        s = syntiant_ndp120_get_main_clk_freq_no_sync(ndp, &main_clk);
        if (s) {
            goto error;
        }
    }

    if (syntiant_ndp120_get_dsp_fw_pointer(ndp)) {
        /* send MB to disable WFI */
        s = syntiant_ndp120_dev_wfi_control(ndp, NDP120_WFI_DISABLE);
        if (s) goto error;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_CLK_DIV_MCUCLKDIV) {
        /* set the mcuclkdiv value so that core clk is > 1MHz */
        if (main_clk / config->mcuclkdiv < SYNTIANT_NDP120_DEFAULT_SPI_SPEED) {
            config->mcuclkdiv
                = main_clk / SYNTIANT_NDP120_DEFAULT_SPI_SPEED - 1;
        }
        config_out.mcuclkdiv = config->mcuclkdiv;

        adx = NDP120_DSP_MB_H2D_ADDR;
        payload_in_MSB = NDP120_H2D_MB_SET_PAYLOAD(payload_in_MSB,
                                                   config->mcuclkdiv);
        s = syntiant_ndp120_write_block(ndp, 1, adx, &payload_in_MSB,
                                        sizeof(uint32_t));
        if (s) goto error;
        s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
            NDP120_DSP_MB_H2D_EXT_MCU_CLK_DIV, NULL);
        if (s) goto error;
    }

    if (syntiant_ndp120_get_dsp_fw_pointer(ndp)) {
        /* send MB to reenable WFI */
        s = syntiant_ndp120_dev_wfi_control(ndp, NDP120_WFI_ENABLE);
        if (s) goto error;
    }

    *config = config_out;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;

    if (s) {
        DEBUG_PRINTF("->> config_clk_div: failed s=%d\n", s);
    }
    return s;
}

static int determine_main_clk_src(struct syntiant_ndp_device_s *ndp, uint32_t *src)
{
    int s;

    /* raw reg values */
    uint8_t spi_ctl;
    uint32_t mcu_chip_config_clkctl1;

    /* extracted fields */
    uint32_t spi_ctl_extsel;
    uint32_t spi_ctl_intsel;

    uint32_t mcu_chip_config_clksel;
    uint32_t mcu_chip_config_refsel;
    uint32_t mcu_chip_config_extsel;

    /* mux logic */
    uint32_t extsel;
    uint32_t clksel;
    uint32_t refsel;

    *src = 0;

    s = ndp_spi_read(NDP120_SPI_CTL, &spi_ctl);
    if (s) goto error;

    s = ndp_mcu_read(NDP120_CHIP_CONFIG_CLKCTL1, &mcu_chip_config_clkctl1);
    if (s) goto error;

    spi_ctl_extsel = NDP120_SPI_CTL_EXTSEL_EXTRACT(spi_ctl);
    spi_ctl_intsel = NDP120_SPI_CTL_INTSEL_EXTRACT(spi_ctl);

    mcu_chip_config_clksel = NDP120_CHIP_CONFIG_CLKCTL1_CLKSEL_EXTRACT(mcu_chip_config_clkctl1);
    mcu_chip_config_refsel = NDP120_CHIP_CONFIG_CLKCTL1_REFSEL_EXTRACT(mcu_chip_config_clkctl1);
    mcu_chip_config_extsel = NDP120_CHIP_CONFIG_CLKCTL1_EXTSEL_EXTRACT(mcu_chip_config_clkctl1);

    extsel = spi_ctl_extsel || ((!spi_ctl_intsel) && mcu_chip_config_extsel);
    clksel = (!spi_ctl_intsel) && mcu_chip_config_clksel;
    refsel = (!spi_ctl_extsel) && mcu_chip_config_refsel;

    /* FIXME we should deal with refsel */
    /* also, we need to store the xtal freq somewhere */

    (void)refsel; /* not using this right now */

    if (extsel) {
        *src = SYNTIANT_NDP120_MAIN_CLK_SRC_EXT;
    } else if (clksel) {
        *src = SYNTIANT_NDP120_MAIN_CLK_SRC_PLL;
    } else {
        *src = SYNTIANT_NDP120_MAIN_CLK_SRC_FLL;
    }

error:
    return s;
}

int
syntiant_ndp120_get_main_clk_freq_no_sync(
    struct syntiant_ndp_device_s *ndp, uint32_t *freq)
{
    int s;
    uint32_t src;

    DEBUG_PRINTF("~~> syntiant_ndp120_get_main_clk_freq_no_sync...\n");
    /* clear before "get" operation */
    *freq = 0;

    s = determine_main_clk_src(ndp, &src);
    if (s) goto error;

    switch (src) {
    case SYNTIANT_NDP120_MAIN_CLK_SRC_EXT:
        s = syntiant_ndp120_get_put_ext_clk_freq_no_sync(ndp, freq);
        break;
    case SYNTIANT_NDP120_MAIN_CLK_SRC_PLL:
        s = syntiant_ndp120_get_put_pll_clk_freq_no_sync(ndp, freq);
        break;
    case SYNTIANT_NDP120_MAIN_CLK_SRC_FLL:
        s = syntiant_ndp120_get_put_fll_clk_freq_no_sync(ndp, freq);
        break;
    default:
        s = SYNTIANT_NDP_ERROR_CONFIG;
        break;
    }
    if (!*freq) {
        *freq = ndp->d.ndp120.core_clock_freq;
        DEBUG_PRINTF("~~> get_main_clk_freq: using saved core_clock_freq %d\n",
            *freq);
    }
error:
    return s;
}

int
syntiant_ndp120_get_main_clk_freq(
    struct syntiant_ndp_device_s *ndp, uint32_t *freq)
{
    int s;
    int s0;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_ndp120_get_main_clk_freq_no_sync(ndp, freq);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;

    return s;
}

int
syntiant_ndp120_config_get_sample_ready(struct syntiant_ndp_device_s *ndp,
        uint32_t *enable)
{
    int s0;
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t fw_st_ptr;
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    fw_st_ptr = syntiant_ndp120_get_dsp_fw_pointer(ndp);
    if (!fw_st_ptr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    fw_st_ptr += (uint32_t) offsetof(ndp120_dsp_fw_base_t,
            config.notify_on_sample_ready);
    s = ndp_mcu_read(fw_st_ptr, enable);
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int
syntiant_ndp120_config_notify_on_sample_ready_no_sync(
    struct syntiant_ndp_device_s *ndp, uint32_t enable)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t fw_st_ptr;

    fw_st_ptr = syntiant_ndp120_get_dsp_fw_pointer(ndp);
    if (!fw_st_ptr) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    fw_st_ptr += (uint32_t) offsetof(ndp120_dsp_fw_base_t, config.notify_on_sample_ready);

    s = ndp_mcu_write(fw_st_ptr, enable);
    if (s) goto error;

error:
    return s;
}

int
syntiant_ndp120_config_notify_on_sample_ready(struct syntiant_ndp_device_s *ndp, uint32_t enable)
{
    int s0;
    int s = SYNTIANT_NDP_ERROR_NONE;
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    /* Before turning on sample ready interrupt, we need to retrieve DSP's
    PCM ring buffer pointers and save them to ilib */
    if (enable) {
        s = syntiant_ndp120_init_ring_buffer_pointers_no_sync(ndp, 0);
        if (s) goto error;
    }

    s = syntiant_ndp120_config_notify_on_sample_ready_no_sync(ndp, enable);
    if (s) {
        goto error;
    }

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_config_mic_no_sync(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_mic_t *config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    ndp120_dsp_fw_mic_settings_t mic_settings = { 0 };
    uint32_t addr = ndp->d.ndp120.dsp_fw_state_addr +
        (uint32_t)offsetof(ndp120_dsp_fw_state_t, mic_settings);

    DEBUG_PRINTF(
        "MIC: get=%#x set=%#x mic=%u sensitivity=%d delay=%u\n",
        config->get, config->set, config->mic, config->sensitivity, config->delay);

    s = ndp_mcu_read_block(addr, &mic_settings, sizeof(mic_settings));
    if (s) {
        DEBUG_PRINTF("config_mic read block failed s=%d\n", s);
        goto error;
    }

    if (config->set) {
        mic_settings.sensitivity[config->mic] = config->sensitivity;
        mic_settings.delay[config->mic] = config->delay;
        s = ndp_mcu_write_block(addr, &mic_settings, sizeof(mic_settings));
        if (s) {
            DEBUG_PRINTF("config_mic write block failed s=%d\n", s);
            goto error;
        }
        s = ndp_mcu_read_block(addr, &mic_settings, sizeof(mic_settings));
        if (s) {
            DEBUG_PRINTF("config_mic read block failed s=%d\n", s);
            goto error;
        }
    }

    if (config->get) {
        config->sensitivity = mic_settings.sensitivity[config->mic];
        config->delay = mic_settings.delay[config->mic];
    }

error:
    return s;
}

int syntiant_ndp120_config_mic(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_mic_t *config)
{
    int s0, s = SYNTIANT_NDP_ERROR_NONE;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_ndp120_config_mic_no_sync(ndp, config);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_config_decimation_no_sync(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_decimation_t *config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t data, old_data;


    if (!config->set && !config->get) {
        DEBUG_PRINTF("not get or set, returning\n");
        goto error;
    }

    DEBUG_PRINTF(
        "DECIMATION: get=%#x set=%#x mic=%u inshift=%u outshift=%u\n",
        config->get,  config->set, config->mic, config->inshift, config->outshift);

    s = ndp_mcu_read(NDP120_DSP_CONFIG_PDMCFG_B(config->mic), &data);
    if (s) goto error;
    old_data = data;

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_DECIMATION_INSHIFT) {
        data = NDP120_DSP_CONFIG_PDMCFG_B_INSHIFT_MASK_INSERT(data, config->inshift);
    } else {
        config->inshift = NDP120_DSP_CONFIG_PDMCFG_B_INSHIFT_EXTRACT(data);
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_DECIMATION_OUTSHIFT) {
        data = NDP120_DSP_CONFIG_PDMCFG_B_OUTSHIFT_MASK_INSERT(data, config->outshift);
    } else {
        config->outshift = NDP120_DSP_CONFIG_PDMCFG_B_OUTSHIFT_EXTRACT(data);
    }

    if (old_data != data) {
        s = ndp_mcu_write(NDP120_DSP_CONFIG_PDMCFG_B(config->mic), data);
        if (s) goto error;
        s = toggle_all_pdmctl_update(ndp);
        if (s) goto error;
    }

error:
    return s;
}

int syntiant_ndp120_config_decimation(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_decimation_t *config)
{
    int s0, s = SYNTIANT_NDP_ERROR_NONE;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_ndp120_config_decimation_no_sync(ndp, config);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_config_gain_no_sync(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_gain_t *config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    int updated = 0;
    uint32_t data, old_data;

    if (!config->set && !config->get) {
        DEBUG_PRINTF("not get or set, returning\n");
        goto error;
    }

    DEBUG_PRINTF(
        "GAIN: get=%#x set=%#x mic=%u decrmovalmode=%u agcshiftdir=%u "
        "agcshiftcnt=%u agcfingrainmul=%#x zcgainchange=%u\n",
        config->get, config->set, config->mic, config->dcremovalmode,
        config->agcshiftdir, config->agcshiftcnt,
        config->agcfinegrainmul, config->zcgainchange);

    /* pdmcfg_b */
    s = ndp_mcu_read(NDP120_DSP_CONFIG_PDMCFG_B(config->mic), &data);
    if (s) goto error;
    old_data = data;

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_GAIN_DCREMOVALMODE) {
        data = NDP120_DSP_CONFIG_PDMCFG_B_DCREMOVALMODE_MASK_INSERT(data,
            config->dcremovalmode);
    } else {
        config->dcremovalmode
            = NDP120_DSP_CONFIG_PDMCFG_B_DCREMOVALMODE_EXTRACT(data);
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_GAIN_DCSTATICREMVAL) {
        data = NDP120_DSP_CONFIG_PDMCFG_B_DCSTATICREMVAL_MASK_INSERT(data,
            config->dcstaticremval);
    } else {
        config->dcstaticremval
            = NDP120_DSP_CONFIG_PDMCFG_B_DCSTATICREMVAL_EXTRACT(data);
    }

    if (old_data != data) {
        s = ndp_mcu_write(NDP120_DSP_CONFIG_PDMCFG_B(config->mic), data);
        if (s) goto error;
        updated = 1;
    }

    /* misccfg */
    s = ndp_mcu_read(NDP120_DSP_CONFIG_MISCCFG(config->mic), &data);
    if (s) goto error;
    old_data = data;

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCSHIFTDIR) {
        data = NDP120_DSP_CONFIG_MISCCFG_AGCSHIFTDIR_MASK_INSERT(data,
            config->agcshiftdir);
    } else {
        config->agcshiftdir
            = NDP120_DSP_CONFIG_MISCCFG_AGCSHIFTDIR_EXTRACT(data);
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCSHIFTCNT) {
        data = NDP120_DSP_CONFIG_MISCCFG_AGCSHIFTCNT_MASK_INSERT(data,
            config->agcshiftcnt);
    } else {
        config->agcshiftcnt
            = NDP120_DSP_CONFIG_MISCCFG_AGCSHIFTCNT_EXTRACT(data);
    }

    if(config->set & SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCFINEGRAINMUL) {
        data = NDP120_DSP_CONFIG_MISCCFG_AGCFINEGRAINMUL_MASK_INSERT(data,
            config->agcfinegrainmul);
    } else {
        config->agcfinegrainmul
            = NDP120_DSP_CONFIG_MISCCFG_AGCFINEGRAINMUL_EXTRACT(data);
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_GAIN_ZCGAINCHANGE) {
        data = NDP120_DSP_CONFIG_MISCCFG_ZCGAINCHANGEENABLE_MASK_INSERT(data,
                    config->zcgainchange);
    } else {
        config->zcgainchange
            = NDP120_DSP_CONFIG_MISCCFG_ZCGAINCHANGEENABLE_EXTRACT(data);
    }

    if (old_data != data) {
        s = ndp_mcu_write(NDP120_DSP_CONFIG_MISCCFG(config->mic), data);
        if (s) goto error;
        updated = 1;
    }

    if (updated) {
        s = toggle_all_pdmctl_update(ndp);
        if (s) goto error;
    }

error:
    return s;
}

int syntiant_ndp120_config_gain(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_gain_t *config)
{
    int s0, s = SYNTIANT_NDP_ERROR_NONE;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_ndp120_config_gain_no_sync(ndp, config);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_config_farrow_no_sync(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_farrow_t *config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t data, old_data;

    if (!config->set && !config->get) {
        DEBUG_PRINTF("not get or set, returning\n");
        goto error;
    }

    DEBUG_PRINTF(
        "FARROW: get=%#x set=%#x interface=%u bypass=%u phase_step=%#x\n",
        config->get,  config->set, config->interface, config->bypass,
        config->phase_step);

    /* pdmcfg_b */
    s = ndp_mcu_read(NDP120_DSP_CONFIG_FARROWCFG(config->interface), &data);
    if (s) goto error;
    old_data = data;

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_FARROW_BYPASS) {
        data = NDP120_DSP_CONFIG_FARROWCFG_BYPASS_FARROW_MASK_INSERT(data, config->bypass);
    } else {
        config->bypass = NDP120_DSP_CONFIG_FARROWCFG_BYPASS_FARROW_EXTRACT(data);
    }

    if(config->set & SYNTIANT_NDP120_CONFIG_SET_FARROW_PHASE_STEP) {
        data = NDP120_DSP_CONFIG_FARROWCFG_FRPHASESTEP_MASK_INSERT(data, config->phase_step);
    } else {
        config->phase_step = NDP120_DSP_CONFIG_FARROWCFG_FRPHASESTEP_EXTRACT(data);
    }

    if (old_data != data) {
        s = ndp_mcu_write(NDP120_DSP_CONFIG_FARROWCFG(config->interface), data);
        if (s) goto error;
        s = toggle_all_pdmctl_update(ndp);
        if (s) goto error;
    }

error:
    return s;
}

int syntiant_ndp120_config_farrow(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_farrow_t *config)
{
    int s0, s = SYNTIANT_NDP_ERROR_NONE;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_ndp120_config_farrow_no_sync(ndp, config);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

#define BWCOEF_GET_SET(coef_u, coef_l) do { \
    if (config->set & SYNTIANT_NDP120_CONFIG_SET_BWCOEF_##coef_u) { \
        value = config->coef_l; \
        if (value == 0) { \
            value = NDP120_DSP_CONFIG_BWCOEF_##coef_u##_CFG_DEFAULT; \
        } \
        data = NDP120_DSP_CONFIG_BWCOEF_##coef_u##_CFG_BWCOEF_##coef_u##_INSERT(0, value); \
        s = ndp_mcu_write(NDP120_DSP_CONFIG_BWCOEF_##coef_u##_CFG(config->mic), data); \
        if (s) goto error; \
        updated = 1; \
    } else { \
        s = ndp_mcu_read(NDP120_DSP_CONFIG_BWCOEF_##coef_u##_CFG(config->mic), &data); \
        if (s) goto error; \
        config->coef_l = NDP120_DSP_CONFIG_BWCOEF_##coef_u##_CFG_BWCOEF_##coef_u##_EXTRACT(data); \
    } \
} while (0)

int syntiant_ndp120_config_bwcoef_no_sync(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_bwcoef_t *config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t data, value;
    int updated = 0;

    if (!config->set && !config->get) {
        goto error;
    }

    if (config->set && config->get) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    DEBUG_PRINTF(
        "BWCOEFF: get=%#x set=%#x mic=%u a1=%u a2=%u b0=%u b1=%u b2=%u\n",
        config->get, config->set, config->mic,
        config->a1, config->a2, config->b0, config->b1, config->b2);

    BWCOEF_GET_SET(A1, a1);
    BWCOEF_GET_SET(A2, a2);
    BWCOEF_GET_SET(B0, b0);
    BWCOEF_GET_SET(B1, b1);
    BWCOEF_GET_SET(B2, b2);

    if (updated) {
        s = toggle_all_pdmctl_update(ndp);
        if (s) goto error;
    }

error:
    return s;
}

int syntiant_ndp120_config_bwcoef(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_bwcoef_t *config)
{
    int s0, s = SYNTIANT_NDP_ERROR_NONE;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_ndp120_config_bwcoef_no_sync(ndp, config);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

static int
syntiant_ndp120_config_i2s_disable(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_i2s_t *config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t i2sctl, audctrl2;

    s = ndp_mcu_read(NDP120_DSP_CONFIG_I2SCTL(config->interface), &i2sctl);
    if (s) goto out;

    /* disable i2sctl */
    i2sctl = NDP120_DSP_CONFIG_I2SCTL_ENABLE_MASK_INSERT(i2sctl, 0);
    s = ndp_mcu_write(NDP120_DSP_CONFIG_I2SCTL(config->interface), i2sctl);
    if (s) goto out;

    /* disable aud2ctl */
    s = ndp_mcu_read(NDP120_CHIP_CONFIG_AUDCTRL2, &audctrl2);
    if (s) goto out;
    audctrl2 = NDP120_CHIP_CONFIG_AUDCTRL2_OE_MASK_INSERT(audctrl2,
            NDP120_CHIP_CONFIG_AUDCTRL2_OE_DISABLE);
    audctrl2 =  NDP120_CHIP_CONFIG_AUDCTRL2_IE_MASK_INSERT(audctrl2,
            NDP120_CHIP_CONFIG_AUDCTRL2_IE_DISABLE);
    /* reset aud2 out block */
    audctrl2 = NDP120_CHIP_CONFIG_AUDCTRL2_RSTB_MASK_INSERT(audctrl2, 0);
    s = ndp_mcu_write(NDP120_CHIP_CONFIG_AUDCTRL2, audctrl2);
    if (s) goto out;

out:
    return s;
}

int
syntiant_ndp120_config_i2s_no_sync(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_i2s_t *config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    int i;
    uint32_t temp;
    uint32_t data, data_old;
    uint32_t dsp_fw_state_ptr;
    uint32_t tdmchnctl;

    uint32_t frphasestep = 0;
    ndp120_dsp_fw_aud2_config_t aud2_config, aud2_config_old;

    DEBUG_PRINTF("->> syntiant_ndp120_config_i2s_no_sync...\n");
    if (config->interface > 2) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto done;
    }
    if (!config->set && !config->get) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto done;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_I2S_DISABLE) {
        return syntiant_ndp120_config_i2s_disable(ndp, config);
    }

    s = ndp_mcu_read(NDP120_DSP_CONFIG_I2SCTL(config->interface), &data);
    if (s) goto done;

    data_old = data;

    if (config-> set) {
        /* enable = 0 */
        data = NDP120_DSP_CONFIG_I2SCTL_ENABLE_MASK_INSERT(data, 0);
        s = ndp_mcu_write(NDP120_DSP_CONFIG_I2SCTL(config->interface), data);
        if (s) goto done;

        /* init = 1 */
        /* init = 0 */
        for (i = 1; i >= 0; --i) {
            data = NDP120_DSP_CONFIG_I2SCTL_INIT_MASK_INSERT(data, (uint32_t)i);
            s = ndp_mcu_write(NDP120_DSP_CONFIG_I2SCTL(config->interface), data);
            if (s) goto done;
        }
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_I2S_MODE) {
        switch (config->mode) {
        case SYNTIANT_NDP120_CONFIG_VALUE_I2S_MODE_STANDARD:
        /* fall-through */
        case SYNTIANT_NDP120_CONFIG_VALUE_I2S_MODE_TDM:
            data = NDP120_DSP_CONFIG_I2SCTL_MODE_MASK_INSERT(data, config->mode);
            break;
        default:
            s = SYNTIANT_NDP_ERROR_ARG;
            goto done;
        }
    }

    if (config->set) {
        if (config->freq < 16000 || config->freq > 48000) {
            s = SYNTIANT_NDP_ERROR_ARG;
            goto done;
        }
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_I2S_FRAMESIZE) {
        if (config->framesize > 0x20) {
            s = SYNTIANT_NDP_ERROR_ARG;;
            goto done;
        }
        data = NDP120_DSP_CONFIG_I2SCTL_FRAMESIZE_MASK_INSERT(data, config->framesize);
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_I2S_SAMPLESIZE) {
        if (config->samplesize > 0x20) {
            s = SYNTIANT_NDP_ERROR_ARG;
            goto done;
        }

        data = NDP120_DSP_CONFIG_I2SCTL_SAMPLESIZE_MASK_INSERT(data, config->samplesize);
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_I2S_MSB_INDEX) {
        if (config->msb_index > 0x1e) {
            s = SYNTIANT_NDP_ERROR_ARG;
            goto done;
        }
        data = NDP120_DSP_CONFIG_I2SCTL_MSBINDEX_MASK_INSERT(data, config->msb_index);
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_I2S_PACKED) {
        data = NDP120_DSP_CONFIG_I2SCTL_PACKED_MASK_INSERT(data, (uint32_t)!!config->packed);
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_I2S_EDGE) {
        data = NDP120_DSP_CONFIG_I2SCTL_NEGEDGEENABLE_MASK_INSERT(data, (uint32_t)!!config->edge);
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_I2S_DELAYED_FLOP_SENSITIVITY) {
        data = NDP120_DSP_CONFIG_I2SCTL_DELAYEDFLOPSENSITIVITY_MASK_INSERT(data, (uint32_t)!!config->delayed_flop_sensitivity);
    }

    if (config->interface == NDP120_DSP_AUDIO_CHAN_AUD2) {
        uint32_t audctrl2, audctrl2_old;
        uint32_t main_clk = 0;
        uint32_t aud2clkoutdiv, aud2clkdiv, framesize, packed;

        framesize = NDP120_DSP_CONFIG_I2SCTL_FRAMESIZE_EXTRACT(data);
        packed = NDP120_DSP_CONFIG_I2SCTL_PACKED_EXTRACT(data);

        /* is this only for aud2? ¯\_(ツ)_/¯ */
        if (config->set & SYNTIANT_NDP120_CONFIG_SET_I2S_AUDIO_OUT_FS_EXT_ENABLE) {
            data = NDP120_DSP_CONFIG_I2SCTL_AUDIO_OUT_FS_EXT_ENABLE_MASK_INSERT(data,
                    (uint32_t) !!config->audio_out_fs_ext_enable);
        }

        s = syntiant_ndp120_get_main_clk_freq_no_sync(ndp, &main_clk);
        if (s) goto done;

        DEBUG_PRINTF("~~> using main clock %u\n", main_clk);

        /* audctrl2 */
        s = ndp_mcu_read(NDP120_CHIP_CONFIG_AUDCTRL2, &audctrl2);
        if (s) goto done;
        audctrl2_old = audctrl2;

        if (config->set & SYNTIANT_NDP120_CONFIG_SET_I2S_AUD2CLKOUTNEEDED) {
            audctrl2 = NDP120_CHIP_CONFIG_AUDCTRL2_AUD2CLKOUTNEEDED_MASK_INSERT(audctrl2, (uint32_t) !!config->aud2clkoutneeded);
        }

        temp = (4 * config->freq * framesize * (packed ? 2 : 1));
        if (temp != 0 && config->set) {
            aud2clkoutdiv = main_clk / temp;
            aud2clkdiv = aud2clkoutdiv / 2;

            audctrl2 = NDP120_CHIP_CONFIG_AUDCTRL2_AUD2CLKOUTDIV_MASK_INSERT(audctrl2, aud2clkoutdiv);
            audctrl2 = NDP120_CHIP_CONFIG_AUDCTRL2_AUD2CLKDIV_MASK_INSERT(audctrl2, aud2clkdiv);
        }

        if (config->set & SYNTIANT_NDP120_CONFIG_SET_I2S_AUD2_OUT_MODE) {
            switch (config->aud2_out_mode) {
                case SYNTIANT_NDP120_CONFIG_VALUE_I2S_AUD2_OUT_MODE_MASTER:
                    audctrl2 = NDP120_CHIP_CONFIG_AUDCTRL2_OE_MASK_INSERT(audctrl2, NDP120_CHIP_CONFIG_AUDCTRL2_OE_I2S_MASTER);
                    audctrl2 = NDP120_CHIP_CONFIG_AUDCTRL2_IE_MASK_INSERT(audctrl2, NDP120_CHIP_CONFIG_AUDCTRL2_IE_I2S_MASTER);
                    break;
                case SYNTIANT_NDP120_CONFIG_VALUE_I2S_AUD2_OUT_MODE_SLAVE:
                    audctrl2 = NDP120_CHIP_CONFIG_AUDCTRL2_OE_MASK_INSERT(audctrl2, NDP120_CHIP_CONFIG_AUDCTRL2_OE_I2S_SLAVE);
                    audctrl2 = NDP120_CHIP_CONFIG_AUDCTRL2_IE_MASK_INSERT(audctrl2, NDP120_CHIP_CONFIG_AUDCTRL2_IE_I2S_SLAVE);
                    break;
            }
        }

        audctrl2 = NDP120_CHIP_CONFIG_AUDCTRL2_RSTB_MASK_INSERT(audctrl2, 1);

        if (config->set && audctrl2 != audctrl2_old) {
            s = ndp_mcu_write(NDP120_CHIP_CONFIG_AUDCTRL2, audctrl2);
            if (s) goto done;
        }
        dsp_fw_state_ptr = syntiant_ndp120_get_dsp_fw_pointer(ndp);
        if (!dsp_fw_state_ptr) {
            s = SYNTIANT_NDP_ERROR_UNINIT;
            goto done;
        }

        dsp_fw_state_ptr += (uint32_t)offsetof(ndp120_dsp_fw_state_t, aud2_config);
        s = ndp_mcu_read_block(dsp_fw_state_ptr, &aud2_config, sizeof(aud2_config));
        if (s) goto done;

        memcpy(&aud2_config_old, &aud2_config, sizeof(aud2_config));

        if (config->set & SYNTIANT_NDP120_CONFIG_SET_I2S_AUD2_SRC_TYPE) {
            aud2_config.src_type = (uint8_t)config->aud2_src_type;
        }

        if (config->set & SYNTIANT_NDP120_CONFIG_SET_I2S_AUD2_SRC_PARAM) {
            aud2_config.src_param = (uint8_t)config->aud2_src_param;
        }

        if (memcmp(&aud2_config, &aud2_config_old, sizeof(aud2_config))) {
            s = ndp_mcu_write_block(dsp_fw_state_ptr, &aud2_config, sizeof(aud2_config));
            if (s) goto done;
        }

        if (config->get) {
            config->aud2clkoutneeded = NDP120_CHIP_CONFIG_AUDCTRL2_AUD2CLKOUTNEEDED_EXTRACT(audctrl2_old);
            config->aud2_out_mode = NDP120_CHIP_CONFIG_AUDCTRL2_OE_EXTRACT(audctrl2_old) == NDP120_CHIP_CONFIG_AUDCTRL2_OE_I2S_MASTER ?
                SYNTIANT_NDP120_CONFIG_VALUE_I2S_AUD2_OUT_MODE_MASTER :
                SYNTIANT_NDP120_CONFIG_VALUE_I2S_AUD2_OUT_MODE_SLAVE;

            config->aud2_src_type = aud2_config_old.src_type;
            config->aud2_src_param = aud2_config_old.src_param;
            config->packed = NDP120_DSP_CONFIG_I2SCTL_PACKED_EXTRACT(data_old);

            aud2clkoutdiv = NDP120_CHIP_CONFIG_AUDCTRL2_AUD2CLKOUTDIV_EXTRACT(audctrl2_old);
            config->freq = main_clk  / aud2clkoutdiv / 4  / 16 / (config->packed ? 2 : 1);
        }
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_I2S_DUAL_CHANNEL_TDM) {
        /* Configuring AUD2 as TDM4 channel output interface */
        if (config->interface != NDP120_DSP_AUDIO_CHAN_AUD2) {
            s = SYNTIANT_NDP_ERROR_ARG;
            goto done;
        }

        data = NDP120_DSP_CONFIG_I2SCTL_DUALCHANTDM_MASK_INSERT(data,
            (uint32_t)!!config->dual_channel_tdm);
        /* set TDMCHNCTL[3] ; Assumption: 4 ch input from aud0 and aud1
         * Only TDM4 has been supported since NDP120 can take upto 4 ch input.
         */
        s = ndp_mcu_read(NDP120_DSP_CONFIG_TDMCHNCTL(
              NDP120_DSP_CONFIG_TDMCHNCTL_COUNT - 1), &tdmchnctl);
        if (s) goto done;
        tdmchnctl = NDP120_DSP_CONFIG_TDMCHNCTL_TDMCHNLLEFTSEL_MASK_INSERT(
            tdmchnctl, 2);
        tdmchnctl = NDP120_DSP_CONFIG_TDMCHNCTL_TDMCHNLRIGHTSEL_MASK_INSERT(
            tdmchnctl, 3);
        s = ndp_mcu_write(NDP120_DSP_CONFIG_TDMCHNCTL(
              NDP120_DSP_CONFIG_TDMCHNCTL_COUNT - 1), tdmchnctl);
        if (s) goto done;
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_I2S_LEFTCHENABLE) {
        data = NDP120_DSP_CONFIG_I2SCTL_LEFTCHENABLE_MASK_INSERT(data,
            (uint32_t)!!config->leftchenable);
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_I2S_RIGHTCHENABLE) {
        data = NDP120_DSP_CONFIG_I2SCTL_RIGHTCHENABLE_MASK_INSERT(data,
            (uint32_t)!!config->rightchenable);
    }

    if (config->get) {
        uint32_t farrowcfg;
        config->mode = NDP120_DSP_CONFIG_I2SCTL_MODE_EXTRACT(data_old);
        config->framesize = NDP120_DSP_CONFIG_I2SCTL_FRAMESIZE_EXTRACT(data_old);
        config->samplesize = NDP120_DSP_CONFIG_I2SCTL_SAMPLESIZE_EXTRACT(data_old);
        config->msb_index = NDP120_DSP_CONFIG_I2SCTL_MSBINDEX_EXTRACT(data_old);
        config->packed = NDP120_DSP_CONFIG_I2SCTL_PACKED_EXTRACT(data_old);
        config->edge = NDP120_DSP_CONFIG_I2SCTL_NEGEDGEENABLE_EXTRACT(data_old);
        config->delayed_flop_sensitivity = NDP120_DSP_CONFIG_I2SCTL_DELAYEDFLOPSENSITIVITY_EXTRACT(data_old);
        config->audio_out_fs_ext_enable = NDP120_DSP_CONFIG_I2SCTL_AUDIO_OUT_FS_EXT_ENABLE_EXTRACT(data_old);
        config->dual_channel_tdm = NDP120_DSP_CONFIG_I2SCTL_DUALCHANTDM_EXTRACT(data_old);
        config->leftchenable = NDP120_DSP_CONFIG_I2SCTL_LEFTCHENABLE_EXTRACT(data_old);
        config->rightchenable = NDP120_DSP_CONFIG_I2SCTL_RIGHTCHENABLE_EXTRACT(data_old);
        s = ndp_mcu_read(NDP120_DSP_CONFIG_FARROWCFG(config->interface), &farrowcfg);
        /* fallthru (farrowcfg is not used) */
    }

    if (config->set) {
        /* write initial i2sctl */
        if (data != data_old) {
            s = ndp_mcu_write(NDP120_DSP_CONFIG_I2SCTL(config->interface), data);
            if (s) goto done;
        }

        if (config->interface < 2) {
            /* update audctrl */
            s = ndp_mcu_read(NDP120_CHIP_CONFIG_AUDCTRL(config->interface), &data);
            if (s) goto done;
            data = NDP120_CHIP_CONFIG_AUDCTRL_MODE_MASK_INSERT(data, NDP120_CHIP_CONFIG_AUDCTRL_MODE_I2S_SLAVE);
            data = NDP120_CHIP_CONFIG_AUDCTRL_IE_MASK_INSERT(data, 0x7);
            data = NDP120_CHIP_CONFIG_AUDCTRL_PCLK0_EN_MASK_INSERT(data, 0x1);
            data = NDP120_CHIP_CONFIG_AUDCTRL_PCLK1_EN_MASK_INSERT(data, 0x1);
            s = ndp_mcu_write(NDP120_CHIP_CONFIG_AUDCTRL(config->interface), data);
            if (s) goto done;

            /* pdmctl.rstb --> 1 */
            data = 0;
            data = NDP120_DSP_CONFIG_PDMCTL_RSTB_INSERT(data, 1);
            s = ndp_mcu_write(NDP120_DSP_CONFIG_PDMCTL(config->interface), data);
            if (s) goto done;

            /* enable pdmfiltstage2bypass */
            s = ndp_mcu_read(NDP120_DSP_CONFIG_PDMCFG_A(config->interface), &data);
            if (s) goto done;

            if (config->freq == 48000) {
                frphasestep = 0x6000000;
            } else if (config->freq == 32000) {
                frphasestep = 0x4000000;
            } else { /* default is 16 Khz */
                frphasestep = 0;
            }

            /* disable pdmfiltstage2bypass if using default sampling rate
               of 16000 */
            data = NDP120_DSP_CONFIG_PDMCFG_A_PDMFILTSTAGE2BYPASS_MASK_INSERT(
                   data, (uint32_t)!frphasestep);
            s = ndp_mcu_write(NDP120_DSP_CONFIG_PDMCFG_A(config->interface),
                              data);
            if (s) goto done;

            s = ndp_mcu_read(NDP120_DSP_CONFIG_FARROWCFG(config->interface), &data);
            if (s) goto done;

            data = NDP120_DSP_CONFIG_FARROWCFG_FRPHASESTEP_MASK_INSERT(data, frphasestep);
            s = ndp_mcu_write(NDP120_DSP_CONFIG_FARROWCFG(config->interface), data);
            if (s) goto done;
        }

        /* agc, pdmcfg_b: */
        for (i = 0; i <= 1; ++i) {
            s = ndp_mcu_read(NDP120_DSP_CONFIG_MISCCFG((uint32_t) (config->interface << 1) + (uint32_t)i), &data);
            if (s) goto done;
            data = NDP120_DSP_CONFIG_MISCCFG_AGCSHIFTDIR_MASK_INSERT(data, 1);
            data = NDP120_DSP_CONFIG_MISCCFG_AGCSHIFTCNT_MASK_INSERT(data, 3);

            data = NDP120_DSP_CONFIG_MISCCFG_AGCFINEGRAINMUL_MASK_INSERT(data, (uint32_t) 1<<13);
            s = ndp_mcu_write(NDP120_DSP_CONFIG_MISCCFG((uint32_t) (config->interface << 1) + (uint32_t)i), data);
            if (s) goto done;

            if (config->interface < 2) {
                /* disable dc removal mode */
                s = ndp_mcu_read(NDP120_DSP_CONFIG_PDMCFG_B((uint32_t) (config->interface << 1) + (uint32_t)i), &data);
                if (s) goto done;
                data = NDP120_DSP_CONFIG_PDMCFG_B_DCREMOVALMODE_MASK_INSERT(data, NDP120_DSP_CONFIG_PDMCFG_B_DCREMOVALMODE_OFF);
                s = ndp_mcu_write(NDP120_DSP_CONFIG_PDMCFG_B((uint32_t) (config->interface << 1) + (uint32_t)i), data);
                if (s) goto done;
            }
        }

        /* i2sctl.enable = 1 */
        s = ndp_mcu_read(NDP120_DSP_CONFIG_I2SCTL(config->interface), &data);
        if (s) goto done;
        data = NDP120_DSP_CONFIG_I2SCTL_ENABLE_INSERT(data, 1);
        s = ndp_mcu_write(NDP120_DSP_CONFIG_I2SCTL(config->interface), data);
        if (s) goto done;

        s = toggle_all_pdmctl_update(ndp);
        if (s) goto done;
    }

done:
    return s;
}

int syntiant_ndp120_config_i2s(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_i2s_t *config)
{
    int s0, s = SYNTIANT_NDP_ERROR_NONE;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_ndp120_config_i2s_no_sync(ndp, config);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_get_pdm_enable_status(struct syntiant_ndp_device_s *ndp,
                                          uint32_t *pdm_enable)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    unsigned int i;
    uint32_t audctrl, pdmctl;
    uint8_t pclk0_en, pclk1_en, enable;

    *pdm_enable = 0;

    for (i = 0; i < 2; ++i) {
        s = ndp_mcu_read(NDP120_DSP_CONFIG_PDMCTL(i), &pdmctl);
        if (s) goto error;

        s = ndp_mcu_read(NDP120_CHIP_CONFIG_AUDCTRL(i), &audctrl);
        if (s) goto error;

        pclk0_en = NDP120_CHIP_CONFIG_AUDCTRL_PCLK0_EN_EXTRACT(audctrl);
        pclk1_en = NDP120_CHIP_CONFIG_AUDCTRL_PCLK1_EN_EXTRACT(audctrl);
        enable = NDP120_DSP_CONFIG_PDMCTL_ENABLE_EXTRACT(pdmctl);

        if (enable && (pclk0_en || pclk1_en)) {
            *pdm_enable = 1;
            goto error;
        }
    }

error:
    return s;
}

int
syntiant_ndp120_config_pdm_no_sync(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_pdm_t *config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    int need_to_reconfigure;
    uint32_t data;
    uint32_t adx;
    uint32_t valid;
    uint32_t decimation;
    uint32_t main_clk = 0;
    uint32_t main_clk_at_last_config = 0;

    if (!config->set && !config->get) {
        DEBUG_PRINTF("not get or set, returning\n");
        goto error;
    }

    DEBUG_PRINTF(
        "PDM: get=%#x set=%#x interface=%u sample_rate=%u pdm_rate=%u clk_mode=%u mode=%u clk=%u\n",
        config->get,  config->set, config->interface, config->sample_rate,
        config->pdm_rate, config->clk_mode, config->mode, config->clk);

    s = syntiant_ndp120_scratch_get_valid(ndp, &valid);
    if (s && s != SYNTIANT_NDP_ERROR_CRC) goto error;
    s = 0;

    /* GET/SET CACHED PARAMS:  */
    /* sample_rate             */
    /* rate                    */
    /* clk_mode                */
    /* mode                    */

    /* main clock */
    s = syntiant_ndp120_get_main_clk_freq_no_sync(ndp, &main_clk);
    if (s) goto error;
    DEBUG_PRINTF("using main clock of %d\n", main_clk);

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_PDM_PDM_RATE) {
        if (main_clk % config->pdm_rate != 0) {
            DEBUG_PRINTF(
                "Warning: main clk is not exactly divisble by pdm rate \n");
        }
    }

    /* main clock from last time we ran this config;
       if it changes, we need to re-do the config */

    /* this param not affected by interface number */
    if (valid & SYNTIANT_CONFIG_PDM_MAIN_CLK_AT_LAST_CONFIG) {
        s = ndp_mcu_read(SCRATCH_PDM_MAIN_CLK_AT_LAST_CONFIG_ADX, &main_clk_at_last_config);
        if (s) goto error;
    }

    /* for the "valid" word in the scratch area, we use two bits per PDM param;
       there are two PDM audio channels, so shift left using config->interface */

    /* sample rate */
    adx = SCRATCH_PDM_SAMPLE_RATE_ADX(config->interface);
    if (config->set & SYNTIANT_NDP120_CONFIG_SET_PDM_SAMPLE_RATE) {
        s = ndp_mcu_write(adx, config->sample_rate);
        if (s) goto error;
        valid |= (uint32_t)(SYNTIANT_CONFIG_PDM_SAMPLE_RATE_VALID << config->interface);
    } else if (valid & (uint32_t)(SYNTIANT_CONFIG_PDM_SAMPLE_RATE_VALID << config->interface)) {
        s = ndp_mcu_read(adx, &config->sample_rate);
        if (s) goto error;
        config->set |= SYNTIANT_NDP120_CONFIG_SET_PDM_SAMPLE_RATE;
    } else {
        s = SYNTIANT_NDP_ERROR_CONFIG;
        goto error;
    }

    /* pdm rate */
    adx = SCRATCH_PDM_RATE_ADX(config->interface);
    if (config->set & SYNTIANT_NDP120_CONFIG_SET_PDM_PDM_RATE) {
        s = ndp_mcu_write(adx, config->pdm_rate);
        if (s) goto error;
        valid |= (uint32_t)(SYNTIANT_CONFIG_PDM_RATE_VALID << config->interface);
    } else if (valid & (uint32_t)(SYNTIANT_CONFIG_PDM_RATE_VALID << config->interface)) {
        s = ndp_mcu_read(adx, &config->pdm_rate);
        if (s) goto error;
        config->set |= SYNTIANT_NDP120_CONFIG_SET_PDM_PDM_RATE;
    } else {
        s = SYNTIANT_NDP_ERROR_CONFIG;
        goto error;
    }

    /* pdm clk_mode */
    adx = SCRATCH_PDM_CLK_MODE_ADX(config->interface);
    if (config->set & SYNTIANT_NDP120_CONFIG_SET_PDM_CLK_MODE) {
        s = ndp_mcu_write(adx, config->clk_mode);
        if (s) goto error;
        valid |= (uint32_t)(SYNTIANT_CONFIG_PDM_CLK_MODE_VALID << config->interface);
    } else if (valid & (uint32_t)(SYNTIANT_CONFIG_PDM_CLK_MODE_VALID << config->interface)) {
        s = ndp_mcu_read(adx, &config->clk_mode);
        if (s) goto error;
        config->set |= SYNTIANT_NDP120_CONFIG_SET_PDM_CLK_MODE;
    } else {
        s = SYNTIANT_NDP_ERROR_CONFIG;
        goto error;
    }

    /* pdm mode */
    adx = SCRATCH_PDM_MODE_ADX(config->interface);
    if (config->set & SYNTIANT_NDP120_CONFIG_SET_PDM_MODE) {
        s = ndp_mcu_write(adx, config->mode);
        if (s) goto error;
        valid |= (uint32_t)(SYNTIANT_CONFIG_PDM_MODE_VALID << config->interface);
    } else if (valid & (uint32_t)(SYNTIANT_CONFIG_PDM_MODE_VALID << config->interface)) {
        s = ndp_mcu_read(adx, &config->mode);
        if (s) goto error;
        config->set |= SYNTIANT_NDP120_CONFIG_SET_PDM_MODE;
    } else {
        s = SYNTIANT_NDP_ERROR_CONFIG;
        goto error;
    }

    s = syntiant_ndp120_scratch_set_valid(ndp, valid);
    if (s) goto error;

    need_to_reconfigure = (main_clk != main_clk_at_last_config) || config->set;
    if (!need_to_reconfigure) {
        /* skip configuring */
        goto error;
    }

    /*                         */
    /* ACTUAL CONFIGURATION    */
    /*                         */

    decimation = config->pdm_rate / config->sample_rate;
    /* Make decimation a multiple of 4 */
    decimation = (decimation + 3U) & ~3U;
    /* frphasestep = config->pdm_rate / (decimation * config->sample_rate);
       frphasestep always calculates to 1 */
    DEBUG_PRINTF(
        "PDM: pdm_rate=%u sample_rate=%u decimation=%u frphasestep=%u\n",
        config->pdm_rate, config->sample_rate, decimation,
        config->pdm_rate / (decimation * config->sample_rate));

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_PDM_CLK) {

        /* clear enable */
        s = ndp_mcu_read(NDP120_DSP_CONFIG_PDMCTL(config->interface), &data);
        if (s) goto error;
        data = NDP120_DSP_CONFIG_PDMCTL_ENABLE_MASK_INSERT(data, 0);
        data = NDP120_DSP_CONFIG_PDMCTL_RSTB_INSERT(data, 1);
        s = ndp_mcu_write(NDP120_DSP_CONFIG_PDMCTL(config->interface), data);
        if (s) goto error;

        /* set decimation */
        s = ndp_mcu_read(NDP120_DSP_CONFIG_PDMCFG_A(config->interface), &data);
        if (s) goto error;
        data = NDP120_DSP_CONFIG_PDMCFG_A_DECIMATION_MASK_INSERT(data, decimation);
        s = ndp_mcu_write(NDP120_DSP_CONFIG_PDMCFG_A(config->interface), data);
        if (s) goto error;

        /* disable I2S */
        s = ndp_mcu_read(NDP120_DSP_CONFIG_I2SCTL(config->interface), &data);
        if (s) goto error;
        data = NDP120_DSP_CONFIG_I2SCTL_ENABLE_MASK_INSERT(data, 0);
        s = ndp_mcu_write(NDP120_DSP_CONFIG_I2SCTL(config->interface), data);
        if (s) goto error;

        if (config->clk == SYNTIANT_NDP120_CONFIG_VALUE_PDM_CLK_OFF) {
            s = ndp_mcu_read(NDP120_DSP_CONFIG_PDMCTL(config->interface), &data);
            if (s) goto error;
            /* disable edges */
            data = NDP120_DSP_CONFIG_PDMCTL_POSEDGEENABLE_MASK_INSERT( data, 0);
            data = NDP120_DSP_CONFIG_PDMCTL_NEGEDGEENABLE_MASK_INSERT( data, 0);
            /* disable enable */
            data = NDP120_DSP_CONFIG_PDMCTL_ENABLE_MASK_INSERT(data, 0);

            s = ndp_mcu_write(NDP120_DSP_CONFIG_PDMCTL(config->interface), data);
            if (s) goto error;

            s = ndp_mcu_read(NDP120_CHIP_CONFIG_AUDCTRL(config->interface), &data);
            if (s) goto error;

            data = NDP120_CHIP_CONFIG_AUDCTRL_PCLK0_EN_MASK_INSERT(data, 0);
            data = NDP120_CHIP_CONFIG_AUDCTRL_PCLK1_EN_MASK_INSERT(data, 0);
            data = NDP120_CHIP_CONFIG_AUDCTRL_OE_MASK_INSERT(data, 0);
            data = NDP120_CHIP_CONFIG_AUDCTRL_IE_MASK_INSERT(data, 0);
            data = NDP120_CHIP_CONFIG_AUDCTRL_MODE_MASK_INSERT(data, 0);

            s = ndp_mcu_write(NDP120_CHIP_CONFIG_AUDCTRL(config->interface), data);
            if (s) goto error;

            DEBUG_PRINTF(">>> pdm clk set OFF\n");
            /* notify MCU to clean up inference state */
            s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
                    NDP_MBIN_REQUEST_RESET_INFERENCE_STATE, NULL);
            if (s) goto error;

        } else if (config->clk == SYNTIANT_NDP120_CONFIG_VALUE_PDM_CLK_ON) {

            uint32_t pdmctl;
            uint32_t audctrl;
            uint32_t msg = 0;

            /* At this point, NN synpkg could have been loaded, so issue dsp restart,
               before pdm enable */
            if (ndp->d.ndp120.dsp_fw_loaded) {
                s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
                                 NDP120_DSP_MB_H2D_RESTART, &msg);
                if (s) {
                    DEBUG_PRINTF("Error %d in sending DSP restart\n", s);
                    goto error;
                }
                if (msg) {
                    DEBUG_PRINTF("Error in completing DSP restart %s:%d\n",
                        __FILE__, __LINE__);
                    goto error;
                }
            }

            /* pdmcfg_a.pdm_bypass = 0 */
            /* pdmcfg_a.pdmsiltstage2bypass = 0 */
            s = ndp_mcu_read(NDP120_DSP_CONFIG_PDMCFG_A(config->interface), &data);
            if (s) goto error;
            data = NDP120_DSP_CONFIG_PDMCFG_A_PDMBYPASS_MASK_INSERT(data, 0);
            data = NDP120_DSP_CONFIG_PDMCFG_A_PDMFILTSTAGE2BYPASS_MASK_INSERT(data, 0);
            s = ndp_mcu_write(NDP120_DSP_CONFIG_PDMCFG_A(config->interface), data);
            if (s) goto error;

            /* read pdmctl and audctrl */
            s = ndp_mcu_read(NDP120_DSP_CONFIG_PDMCTL(config->interface), &pdmctl);
            if (s) goto error;
            s = ndp_mcu_read(NDP120_CHIP_CONFIG_AUDCTRL(config->interface), &audctrl);
            if (s) goto error;

            /* pdmctl.enable = 1, pdmctl.rstb = 1*/
            pdmctl = NDP120_DSP_CONFIG_PDMCTL_ENABLE_INSERT(pdmctl, 1);
            DEBUG_PRINTF("clk mode: %d\n", config->clk_mode);

        switch(config->clk_mode) {
            case SYNTIANT_NDP120_CONFIG_VALUE_PDM_CLK_MODE_EXTERNAL:

                audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PDMCLKOUTNEEDED_MASK_INSERT(audctrl, 0);
                audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PCLK0_EN_INSERT(audctrl, 1);
                audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PCLK1_EN_MASK_INSERT(audctrl, 0);
                audctrl = NDP120_CHIP_CONFIG_AUDCTRL_OE_MASK_INSERT(audctrl, 0);
                audctrl = NDP120_CHIP_CONFIG_AUDCTRL_IE_MASK_INSERT(audctrl, 0x5);
                audctrl = NDP120_CHIP_CONFIG_AUDCTRL_MODE_MASK_INSERT(audctrl, NDP120_CHIP_CONFIG_AUDCTRL_MODE_PDM_IN);

                switch (config->mode) {
                    case SYNTIANT_NDP120_CONFIG_VALUE_PDM_MODE_LEFT:
                        pdmctl = NDP120_DSP_CONFIG_PDMCTL_POSEDGEENABLE_INSERT(pdmctl, 1);
                        pdmctl = NDP120_DSP_CONFIG_PDMCTL_NEGEDGEENABLE_MASK_INSERT(pdmctl, 0);
                        break;
                    case SYNTIANT_NDP120_CONFIG_VALUE_PDM_MODE_RIGHT:
                        pdmctl = NDP120_DSP_CONFIG_PDMCTL_POSEDGEENABLE_MASK_INSERT(pdmctl, 0);
                        pdmctl = NDP120_DSP_CONFIG_PDMCTL_NEGEDGEENABLE_INSERT(pdmctl, 1);
                        break;
                    case SYNTIANT_NDP120_CONFIG_VALUE_PDM_MODE_STEREO:
                        pdmctl = NDP120_DSP_CONFIG_PDMCTL_POSEDGEENABLE_INSERT(pdmctl, 1);
                        pdmctl = NDP120_DSP_CONFIG_PDMCTL_NEGEDGEENABLE_INSERT(pdmctl, 1);
                        break;
                }
            break;
            case SYNTIANT_NDP120_CONFIG_VALUE_PDM_CLK_MODE_INTERNAL:

                /* audctrl */
                audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PDMCLKOUTNEEDED_INSERT(audctrl, 1);
                audctrl = NDP120_CHIP_CONFIG_AUDCTRL_MODE_MASK_INSERT(audctrl, NDP120_CHIP_CONFIG_AUDCTRL_MODE_PDM_OUT);
                audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PDMCLKOUTDIV_MASK_INSERT(audctrl, main_clk / 2 / config->pdm_rate);
                audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PCLK0_EN_INSERT(audctrl, 1);
                audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PCLK1_EN_MASK_INSERT(audctrl, 0);
                audctrl = NDP120_CHIP_CONFIG_AUDCTRL_OE_MASK_INSERT(audctrl, 1);
                audctrl = NDP120_CHIP_CONFIG_AUDCTRL_IE_MASK_INSERT(audctrl, 0x5);

                switch (config->mode) {
                    case SYNTIANT_NDP120_CONFIG_VALUE_PDM_MODE_LEFT:
                        pdmctl = NDP120_DSP_CONFIG_PDMCTL_POSEDGEENABLE_INSERT(pdmctl, 1);
                        pdmctl = NDP120_DSP_CONFIG_PDMCTL_NEGEDGEENABLE_MASK_INSERT(pdmctl, 0);
                        break;
                    case SYNTIANT_NDP120_CONFIG_VALUE_PDM_MODE_RIGHT:
                        pdmctl = NDP120_DSP_CONFIG_PDMCTL_POSEDGEENABLE_MASK_INSERT(pdmctl, 0);
                        pdmctl = NDP120_DSP_CONFIG_PDMCTL_NEGEDGEENABLE_INSERT(pdmctl, 1);
                        break;
                    case SYNTIANT_NDP120_CONFIG_VALUE_PDM_MODE_STEREO:
                        pdmctl = NDP120_DSP_CONFIG_PDMCTL_POSEDGEENABLE_INSERT(pdmctl, 1);
                        pdmctl = NDP120_DSP_CONFIG_PDMCTL_NEGEDGEENABLE_INSERT(pdmctl, 1);
                        break;
                }
                break;
            case SYNTIANT_NDP120_CONFIG_VALUE_PDM_CLK_MODE_DUAL_INTERNAL:
                /* audctrl */
                audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PDMCLKOUTNEEDED_INSERT(audctrl, 1);
                audctrl = NDP120_CHIP_CONFIG_AUDCTRL_MODE_MASK_INSERT(audctrl, NDP120_CHIP_CONFIG_AUDCTRL_MODE_PDM_OUT);
                audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PDMCLKOUTDIV_MASK_INSERT(audctrl, main_clk / 2 / config->pdm_rate);
                audctrl = NDP120_CHIP_CONFIG_AUDCTRL_OE_MASK_INSERT(audctrl, 3);
                audctrl = NDP120_CHIP_CONFIG_AUDCTRL_IE_MASK_INSERT(audctrl, 0x7);

                switch (config->mode) {
                    case SYNTIANT_NDP120_CONFIG_VALUE_PDM_MODE_LEFT:
                        pdmctl = NDP120_DSP_CONFIG_PDMCTL_POSEDGEENABLE_INSERT(pdmctl, 1);
                        pdmctl = NDP120_DSP_CONFIG_PDMCTL_NEGEDGEENABLE_MASK_INSERT(pdmctl, 0);
                        audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PCLK0_EN_INSERT(audctrl, 1);
                        audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PCLK1_EN_MASK_INSERT(audctrl, 0);
                        break;
                    case SYNTIANT_NDP120_CONFIG_VALUE_PDM_MODE_RIGHT:
                        pdmctl = NDP120_DSP_CONFIG_PDMCTL_POSEDGEENABLE_MASK_INSERT(pdmctl, 0);
                        pdmctl = NDP120_DSP_CONFIG_PDMCTL_NEGEDGEENABLE_INSERT(pdmctl, 1);
                        audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PCLK0_EN_MASK_INSERT(audctrl, 0);
                        audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PCLK1_EN_INSERT(audctrl, 1);
                        break;
                    case SYNTIANT_NDP120_CONFIG_VALUE_PDM_MODE_STEREO:
                        pdmctl = NDP120_DSP_CONFIG_PDMCTL_POSEDGEENABLE_INSERT(pdmctl, 1);
                        pdmctl = NDP120_DSP_CONFIG_PDMCTL_NEGEDGEENABLE_INSERT(pdmctl, 1);
                        audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PCLK0_EN_INSERT(audctrl, 1);
                        audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PCLK1_EN_INSERT(audctrl, 1);
                        break;
                }
                break;
            }

            s = ndp_mcu_write(NDP120_CHIP_CONFIG_AUDCTRL(config->interface), audctrl);
            if (s) goto error;
            s = ndp_mcu_write(NDP120_DSP_CONFIG_PDMCTL(config->interface), pdmctl);
            if (s) goto error;

            DEBUG_PRINTF(">>> pdm clk set ON\n");
        }

        s = ndp_mcu_write(SCRATCH_PDM_MAIN_CLK_AT_LAST_CONFIG_ADX, main_clk);
        if (s) goto error;
        valid |= SYNTIANT_CONFIG_PDM_MAIN_CLK_AT_LAST_CONFIG;

        s = toggle_all_pdmctl_update(ndp);
        if (s) goto error;

    } else {
        adx = NDP120_DSP_CONFIG_PDMCTL(config->interface);
        s = ndp_mcu_read(adx, &data);
        if (s) goto error;
        config->clk = NDP120_DSP_CONFIG_PDMCTL_ENABLE_EXTRACT(data);
        config->set |= SYNTIANT_NDP120_CONFIG_SET_PDM_CLK;

    }

    s = syntiant_ndp120_scratch_set_valid(ndp, valid);
    if (s) goto error;

error:
    return s;
}

int
syntiant_ndp120_config_pdm(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_pdm_t *config)
{
    int s0, s = SYNTIANT_NDP_ERROR_NONE;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_ndp120_config_pdm_no_sync(ndp, config);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int
syntiant_ndp120_config_nn_metadata_no_sync
(struct syntiant_ndp_device_s *ndp, syntiant_ndp120_nn_metadata_t * config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t metadata_addr = syntiant_ndp120_get_dsp_fw_pointer(ndp) +
        (uint32_t) offsetof(ndp120_dsp_fw_base_t, metadata);
    uint32_t nn_metadata_addr = 0;
    uint32_t offset;

    offset = (uint32_t)offsetof(ndp120_dnn_metadata_t, version);
    if (config->set & SYNTIANT_NDP120_SET_META_HEADER) {
        s = ndp_mcu_write_block(
            metadata_addr + offset, &config->header, sizeof(config->header));
        if (s) goto error;
    } else {
        s = ndp_mcu_read_block(
            metadata_addr + offset, &config->header, sizeof(config->header));
        if (s) goto error;
    }

    offset = (uint32_t)offsetof(ndp120_dnn_metadata_t, nn_metadata);
    nn_metadata_addr = metadata_addr + offset;

    if (config->header.version == NDP120_DNN_METADATA_VERSION_V1) {

        offset = (uint32_t)(offsetof(
                                ndp120_dnn_network_metadata_t, v1.layers_per_nn)
            + config->nn_idx * sizeof(ndp120_dnn_network_metadata_t));
        if (config->set & SYNTIANT_NDP120_SET_LAYER_PER_NN) {
            s = ndp_mcu_write(nn_metadata_addr + offset, config->layers_per_nn);
            if (s) goto error;
        } else {
            s = ndp_mcu_read(nn_metadata_addr + offset, &config->layers_per_nn);
            if (s) goto error;
        }

        offset = (uint32_t)(offsetof(
                                ndp120_dnn_network_metadata_t, v1.is_nn_cached)
            + config->nn_idx * sizeof(ndp120_dnn_network_metadata_t));
        if (config->set & SYNTIANT_NDP120_SET_IS_NN_CACHED) {
            s = ndp_mcu_write(nn_metadata_addr + offset, config->is_nn_cached);
            if (s) goto error;
        } else {
            s = ndp_mcu_read(nn_metadata_addr + offset, &config->is_nn_cached);
            if (s) goto error;
        }

        offset = (uint32_t)(offsetof(ndp120_dnn_network_metadata_t,
                                v1.input_layer_isa_idx)
            + config->nn_idx * sizeof(ndp120_dnn_network_metadata_t));
        if (config->set & SYNTIANT_NDP120_SET_INPUT_ISA_IDX) {
            s = ndp_mcu_write(
                nn_metadata_addr + offset, config->nn_input_isa_idx);
            if (s) goto error;
        } else {
            s = ndp_mcu_read(
                nn_metadata_addr + offset, &config->nn_input_isa_idx);
            if (s) goto error;
        }

        offset = (uint32_t)(offsetof(ndp120_dnn_network_metadata_t,
                                v1.output_layer_isa_idx)
            + config->nn_idx * sizeof(ndp120_dnn_network_metadata_t));
        if (config->set & SYNTIANT_NDP120_SET_OUTPUT_ISA_IDX) {
            s = ndp_mcu_write(
                nn_metadata_addr + offset, config->nn_output_isa_idx);
            if (s) goto error;
        } else {
            s = ndp_mcu_read(
                nn_metadata_addr + offset, &config->nn_output_isa_idx);
            if (s) goto error;
        }

        offset = (uint32_t)(offsetof(ndp120_dnn_network_metadata_t,
                                v1.input_layer_type)
            + config->nn_idx * sizeof(ndp120_dnn_network_metadata_t));
        if (config->set & SYNTIANT_NDP120_SET_INPUT_LAYER_TYPE) {
            s = ndp_mcu_write(
                nn_metadata_addr + offset, config->nn_input_layer_type);
            if (s) goto error;
        } else {
            s = ndp_mcu_read(
                nn_metadata_addr + offset, &config->nn_input_layer_type);
            if (s) goto error;
        }

        offset = (uint32_t)(offsetof(ndp120_dnn_network_metadata_t,
                                v1.input_layer_size)
            + config->nn_idx * sizeof(ndp120_dnn_network_metadata_t));
        if (config->set & SYNTIANT_NDP120_SET_INPUT_SIZE) {
            s = ndp_mcu_write_block(nn_metadata_addr + offset,
                &config->nn_input_layer_size,
                sizeof(config->nn_input_layer_size));
            if (s) goto error;
        } else {
            s = ndp_mcu_read_block(nn_metadata_addr + offset,
                &config->nn_input_layer_size,
                sizeof(config->nn_input_layer_size));
            if (s) goto error;
        }

        offset = (uint32_t)(offsetof(
                                ndp120_dnn_network_metadata_t, v1.input_coords)
            + config->nn_idx * sizeof(ndp120_dnn_network_metadata_t)
            + config->layer_idx
                * member_size(
                    ndp120_dnn_network_metadata_t, v1.input_coords[0]));
        if (config->set & SYNTIANT_NDP120_SET_INPUT_COORD) {
            s = ndp_mcu_write(nn_metadata_addr + offset, config->input_coord);
            if (s) goto error;
        } else {
            s = ndp_mcu_read(nn_metadata_addr + offset, &config->input_coord);
            if (s) goto error;
        }

        offset = (uint32_t)(offsetof(
                                ndp120_dnn_network_metadata_t, v1.output_coords)
            + config->nn_idx * sizeof(ndp120_dnn_network_metadata_t)
            + config->layer_idx
                * member_size(
                    ndp120_dnn_network_metadata_t, v1.output_coords[0]));
        if (config->set & SYNTIANT_NDP120_SET_OUTPUT_COORD) {
            s = ndp_mcu_write(nn_metadata_addr + offset, config->output_coord);
            if (s) goto error;
        } else {
            s = ndp_mcu_read(nn_metadata_addr + offset, &config->output_coord);
            if (s) goto error;
        }

        offset = (uint32_t)(offsetof(ndp120_dnn_network_metadata_t,
                                v1.cache_instructions)
            + config->nn_idx * sizeof(ndp120_dnn_network_metadata_t)
            + config->layer_idx
                * member_size(
                    ndp120_dnn_network_metadata_t, v1.cache_instructions[0]));
        if (config->set & SYNTIANT_NDP120_SET_CACHE_INST) {
            s = ndp_mcu_write_block(nn_metadata_addr + offset,
                &config->cache_inst, sizeof(config->cache_inst));
            if (s) goto error;
        } else {
            s = ndp_mcu_read_block(nn_metadata_addr + offset,
                &config->cache_inst, sizeof(config->cache_inst));
            if (s) goto error;
        }
    } else {

        offset
            = (uint32_t)(offsetof(ndp120_dnn_network_metadata_t, v2.num_layers)
                + config->nn_idx * sizeof(ndp120_dnn_network_metadata_t));
        if (config->set & SYNTIANT_NDP120_SET_META_BASE) {
            s = ndp_mcu_write_block(nn_metadata_addr + offset,
                &config->meta_base, sizeof(config->meta_base));
            if (s) goto error;
        } else {
            s = ndp_mcu_read_block(nn_metadata_addr + offset,
                &config->meta_base, sizeof(config->meta_base));
            if (s) goto error;
        }

        offset = (uint32_t)(offsetof(ndp120_dnn_network_metadata_t, v2.layers)
            + config->nn_idx * sizeof(ndp120_dnn_network_metadata_t));
        if (config->set & SYNTIANT_NDP120_SET_LAYER_INDICES) {
            s = ndp_mcu_write_block(nn_metadata_addr + offset,
                config->layer_indices, sizeof(config->layer_indices));
            if (s) goto error;
        } else {
            s = ndp_mcu_read_block(nn_metadata_addr + offset,
                config->layer_indices, sizeof(config->layer_indices));
            if (s) goto error;
        }

        offset = (uint32_t)(offsetof(ndp120_dnn_network_metadata_t, v2.inputs)
            + config->nn_idx * sizeof(ndp120_dnn_network_metadata_t));
        if (config->set & SYNTIANT_NDP120_SET_INPUT_INDICES) {
            s = ndp_mcu_write_block(nn_metadata_addr + offset,
                config->input_indices, sizeof(config->input_indices));
            if (s) goto error;
        } else {
            s = ndp_mcu_read_block(nn_metadata_addr + offset,
                config->input_indices, sizeof(config->input_indices));
            if (s) goto error;
        }

        offset = (uint32_t)(offsetof(ndp120_dnn_network_metadata_t, v2.outputs)
            + config->nn_idx * sizeof(ndp120_dnn_network_metadata_t));
        if (config->set & SYNTIANT_NDP120_SET_OUTPUT_INDICES) {
            s = ndp_mcu_write_block(nn_metadata_addr + offset,
                config->output_indices, sizeof(config->output_indices));
            if (s) goto error;
        } else {
            s = ndp_mcu_read_block(nn_metadata_addr + offset,
                config->output_indices, sizeof(config->output_indices));
            if (s) goto error;
        }

        offset = (uint32_t)(offsetof(ndp120_dnn_network_metadata_t,
                                v1.cache_instructions)
            + config->nn_idx * sizeof(ndp120_dnn_network_metadata_t)
            + config->layer_idx
                * member_size(
                    ndp120_dnn_network_metadata_t, v2.cache_instructions[0]));
        if (config->set & SYNTIANT_NDP120_SET_CACHE_INST) {
            s = ndp_mcu_write_block(nn_metadata_addr + offset,
                &config->cache_inst, sizeof(config->cache_inst));
            if (s) goto error;
        } else {
            s = ndp_mcu_read_block(nn_metadata_addr + offset,
                &config->cache_inst, sizeof(config->cache_inst));
            if (s) goto error;
        }
    }

error:
    return s;
}

int
syntiant_ndp120_config_mcu_orchestrator
(struct syntiant_ndp_device_s * ndp, syntiant_ndp120_mcu_orch_t * config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    int s0;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_ndp120_config_mcu_orchestrator_no_sync(ndp, config);
    if(s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;

}

int
syntiant_ndp120_config_mcu_orchestrator_no_sync
(struct syntiant_ndp_device_s * ndp, syntiant_ndp120_mcu_orch_t * config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t graph_addr = ndp->d.ndp120.mcu_fw_orchestrator_graph_addr;
    struct ndp120_nn_graph nn;
    ndp120_nno_node_t *node;

    s = ndp_mcu_read_block(graph_addr, &nn, sizeof(nn));
    if (s) goto error;

    if (config->set & SYNTIANT_NDP120_SET_NUM_NODES) {
        nn.num_nodes = config->num_nodes;
    } else {
        config->num_nodes = nn.num_nodes;
    }
    if (config->set & SYNTIANT_NDP120_SET_FLOWMAP) {
        nn.flowmap = config->flowmap;
    } else {
        config->flowmap = nn.flowmap;
    }
    if (config->set & SYNTIANT_NDP120_SET_ENABLE_CASCADE) {
        nn.enable_cascade = config->enable_cascade;
        nn.nn_output = (uint8_t) (config->node_idx & 0xff);
    } else {
        config->enable_cascade = nn.enable_cascade;
        nn.nn_output = 0xff; /*enable host notification from any NN*/
    }

    if (config->node_idx >= MAX_NDP120_NNGRAPH_NODES) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    node = &nn.nn_graph[config->node_idx];
    if (config->set & SYNTIANT_NDP120_SET_NODE) {
        memset(node, 0, sizeof (*node));
        node->id = config->id;
        node->flowset_id = config->flow_id;
        node->status = config->status;
        node->type = config->type;
        node->action = config->action;
        node->num_inputs = config->num_inputs;
        node->num_outputs = config->num_outputs;
        node->timeout = config->orch_timeout;
        node->preroll = config->preroll;
        node->src_buffer = config->src_buffer;
        memcpy(&node->input, &config->input_edges, sizeof(node->input));
        memcpy(&node->next_ids, &config->next_ids, sizeof(node->next_ids));
    } else {
        config->id = node->id;
        config->flow_id = node->flowset_id;
        config->status = node->status;
        config->type = node->type;
        config->action = node->action;
        config->num_inputs = node->num_inputs;
        config->num_outputs = node->num_outputs;
        config->orch_timeout = node->timeout;
        config->preroll = node->preroll;
        config->src_buffer = node->src_buffer;
        memcpy(&config->input_edges, &node->input, sizeof(config->input_edges));
        memcpy(&config->next_ids, &node->next_ids, sizeof(config->next_ids));
    }

    if (config->set) {
        s = ndp_mcu_write_block(graph_addr, &nn, sizeof(nn));
        if (s) goto error;
    }

error:
    return s;
}


int syntiant_ndp120_dsp_flow_setup_reset(ndp120_dsp_data_flow_setup_t *setup)
{
    memset(setup, 0, sizeof(*setup));
    return SYNTIANT_NDP_ERROR_NONE;
}

static int flow_get_chan_and_len(ndp120_dsp_data_flow_setup_t *setup, int src, ndp120_dsp_data_flow_rule_t **src_chan, unsigned int *src_len) {
    int s = SYNTIANT_NDP_ERROR_NONE;
    switch (src) {
    case NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO:
        *src_chan = setup->src_pcm_audio;
        *src_len = ARRAY_LEN(setup->src_pcm_audio);
        break;

    case NDP120_DSP_DATA_FLOW_SRC_TYPE_FUNCTION:
        *src_chan = setup->src_function;
        *src_len = ARRAY_LEN(setup->src_function);
        break;

    case NDP120_DSP_DATA_FLOW_SRC_TYPE_NN:
        *src_chan = setup->src_nn;
        *src_len = ARRAY_LEN(setup->src_nn);
        break;

    case NDP120_DSP_DATA_FLOW_SRC_TYPE_MCU:
        *src_chan = setup->src_mcu;
        *src_len = ARRAY_LEN(setup->src_mcu);
        break;

    case NDP120_DSP_DATA_FLOW_SRC_TYPE_SENSOR:
        *src_chan = setup->src_sensor;
        *src_len = ARRAY_LEN(setup->src_sensor);
        break;

    default:
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    error:
    return s;
}

static void
compact_flow_rules(ndp120_dsp_data_flow_setup_t *setup) {
    ndp120_dsp_data_flow_setup_t temp;
    unsigned i, src_len;
    int src_type, set_id;
    int s;
    ndp120_dsp_data_flow_rule_t *rule;
    ndp120_dsp_data_flow_rule_t *chan;

    memcpy(&temp, setup, sizeof(temp));
    syntiant_ndp120_dsp_flow_setup_reset(setup);

    /* do two things here:
     * 1) Remove gaps in rules
     * 2) Group together sets while not changing the rule order in those sets.
     */
    for (src_type = 0; src_type < NDP120_DSP_DATA_FLOW_SRC_TYPE_COUNT; src_type++) {
        s = flow_get_chan_and_len(&temp, src_type, &chan, &src_len);
        if (!s) {
            do {
                set_id = -1;
                for (i = 0; i < src_len; i++) {
                    rule = &chan[i];
                    if (!NDP120_DSP_FLOW_RULE_IS_VALID(*rule)) continue;
                    if (set_id == -1) set_id = rule->set_id;
                    if (rule->set_id == set_id) {
                        syntiant_ndp120_dsp_flow_setup_add_rule(setup, rule, src_type);
                        NDP120_DSP_FLOW_RULE_INVALIDATE(*rule);
                    }
                }
            } while(set_id !=-1);
        }
    }
}

int
syntiant_ndp120_dsp_flow_setup_del_rule(ndp120_dsp_data_flow_setup_t *setup,
    const ndp120_dsp_data_flow_rule_t *rule, int src_type) {
    ndp120_dsp_data_flow_rule_t *src_chan;
    unsigned int i, src_len;
    int deleted = 0;
    int s = SYNTIANT_NDP_ERROR_NONE;
    s = flow_get_chan_and_len(setup, src_type, &src_chan, &src_len);
    if (s) goto error;

    for(i = 0; i < src_len; ++i) {
        if (!NDP120_DSP_FLOW_RULE_IS_VALID(src_chan[i])) continue;
        /*DEBUG_PRINTF("i: %d src_param: %d, dst_type: %d, dst_param: %d\n", i, src_chan[i].src_param, src_chan[i].dst_type, src_chan[i].dst_param);*/
        if(src_chan[i].set_id == rule->set_id &&
           src_chan[i].src_param == rule->src_param &&
           src_chan[i].dst_type == rule->dst_type &&
           src_chan[i].dst_param == rule->dst_param ) {
               NDP120_DSP_FLOW_RULE_INVALIDATE(src_chan[i]);
               deleted = 1;
               break;
           }
    }

    if(deleted) {
           compact_flow_rules(setup);
    }


error:
    return s;
}

int
syntiant_ndp120_dsp_flow_setup_get_rules(struct syntiant_ndp_device_s *ndp, unsigned int flowset_id,
    ndp120_dsp_data_flow_setup_t *filtered_flow_setup)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint8_t idx;
    ndp120_dsp_data_flow_setup_t complete_flow_setup = {0};

    s = syntiant_ndp120_dsp_flow_setup_get(ndp, &complete_flow_setup);
    if (s) {
        goto error;
    }

    for (idx = 0; idx < NDP120_PCM_DATA_FLOW_RULE_MAX; idx++) {
        if ((complete_flow_setup.src_pcm_audio[idx].set_id == flowset_id)) {
            filtered_flow_setup->src_pcm_audio[idx] = complete_flow_setup.src_pcm_audio[idx];
        }
    }
    for (idx = 0; idx < NDP120_FUNC_DATA_FLOW_RULE_MAX; idx++) {
        if ((complete_flow_setup.src_function[idx].set_id == flowset_id)) {
            filtered_flow_setup->src_function[idx] = complete_flow_setup.src_function[idx];
        }
    }
    for (idx = 0; idx < NDP120_NN_DATA_FLOW_RULE_MAX; idx++) {
        if ((complete_flow_setup.src_nn[idx].set_id == flowset_id)) {
            filtered_flow_setup->src_nn[idx] = complete_flow_setup.src_nn[idx];
        }
    }
    for (idx = 0; idx < NDP120_MCU_DATA_FLOW_RULE_MAX; idx++) {
        if ((complete_flow_setup.src_mcu[idx].set_id == flowset_id)) {
            filtered_flow_setup->src_mcu[idx] = complete_flow_setup.src_mcu[idx];
        }
    }
    for (idx = 0; idx < NDP120_SENSOR_DATA_FLOW_RULE_MAX; idx++) {
        if ((complete_flow_setup.src_sensor[idx].set_id == flowset_id)) {
            filtered_flow_setup->src_sensor[idx] = complete_flow_setup.src_sensor[idx];
        }
    }

error:
    return s;
}

int
syntiant_ndp120_dsp_flow_setup_add_rule(ndp120_dsp_data_flow_setup_t *setup,
    const ndp120_dsp_data_flow_rule_t *rule, int src_type)
{
    ndp120_dsp_data_flow_rule_t *src_chan;
    unsigned int src_len, i;
    int s = SYNTIANT_NDP_ERROR_NONE;
    int added = 0;
    s = flow_get_chan_and_len(setup, src_type, &src_chan, &src_len);
    if (s) goto error;

    for(i = 0; i < src_len; ++i) {
        if (NDP120_DSP_FLOW_RULE_IS_VALID(src_chan[i])) continue;
        src_chan[i] = *rule;
        added = 1;
        break;
    }
    if (!added) {
        s = SYNTIANT_NDP_ERROR_NOMEM;
        goto error;
    }

error:
    return s;
}

int
syntiant_ndp120_dsp_flow_setup_apply(struct syntiant_ndp_device_s *ndp,
                                     ndp120_dsp_data_flow_setup_t *setup)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    int s0;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    s = syntiant_ndp120_dsp_flow_setup_apply_no_sync(ndp, setup);
    if (s) goto error;

    /* DSP restart */
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp, NDP120_DSP_MB_H2D_RESTART,
                                               NULL);
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int
syntiant_ndp120_dsp_flow_setup_apply_no_sync(struct syntiant_ndp_device_s *ndp,
    ndp120_dsp_data_flow_setup_t *setup)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t fw_st_adx = ndp->d.ndp120.dsp_fw_state_addr;

    if (!fw_st_adx) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }
    compact_flow_rules(setup);

    s = ndp_mcu_write_block((uint32_t) (fw_st_adx +
            offsetof(ndp120_dsp_fw_base_t, data_flow)), (void*)setup, sizeof(*setup));
    if(s) goto error;

error:
    return s;
}

int
syntiant_ndp120_dsp_flow_setup_get(
    struct syntiant_ndp_device_s *ndp, ndp120_dsp_data_flow_setup_t *setup)
{
    int s0;
    int s = SYNTIANT_NDP_ERROR_NONE;
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;


    s = syntiant_ndp120_dsp_flow_setup_get_no_sync(ndp, setup);
    if(s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int
syntiant_ndp120_dsp_flow_setup_get_no_sync(
    struct syntiant_ndp_device_s *ndp, ndp120_dsp_data_flow_setup_t *setup)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t fw_st_adx = ndp->d.ndp120.dsp_fw_state_addr;

    if (!fw_st_adx) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = ndp_mcu_read_block((uint32_t) (fw_st_adx +
           offsetof(ndp120_dsp_fw_base_t, data_flow)), (void*)setup, sizeof(*setup));
    if(s) goto error;

error:
    return s;
}

int
syntiant_ndp120_dsp_flow_get_put_set_id(struct syntiant_ndp_device_s *ndp, int *set_id)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    int s0;
    uint32_t adx, payload_in_MSB = 0;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;


    adx = syntiant_ndp120_get_dsp_fw_pointer(ndp);
    if (!adx) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    adx += (uint32_t) offsetof(ndp120_dsp_fw_base_t, data_flow_current_set_id);

    if (*set_id < 0) {
        s = ndp_mcu_read(adx, set_id);
        if (s) goto error;
    } else if (*set_id <= 255) {
        adx = NDP120_DSP_MB_H2D_ADDR;
        payload_in_MSB = NDP120_H2D_MB_SET_PAYLOAD(payload_in_MSB,
                                                        (uint32_t)*set_id);
        s = ndp_mcu_write_block(adx, &payload_in_MSB, sizeof(uint32_t));
        if (s) goto error;

        /* Send MB command to DSP to set current_set_id */
        s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
            NDP120_DSP_MB_H2D_EXT_SET_ID, NULL);
        if (s) goto error;
    } else {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }
error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int
syntiant_ndp120_orch_host_notification_network(struct syntiant_ndp_device_s *ndp, int *nn_number)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t graph_addr = ndp->d.ndp120.mcu_fw_orchestrator_graph_addr;
    struct ndp120_nn_graph nn;

    s = ndp_mcu_read_block(graph_addr, &nn, sizeof(nn));
    if (s) goto error;

    nn.nn_output = (uint8_t)*nn_number;

    s = ndp_mcu_write_block(graph_addr, &nn, sizeof(nn));
    if (s) goto error;
error:
    return s;
}

#if SYNTIANT_NDP120_DEBUG
#define COLUMN_DUMP(SRC_TYPE) \
    flow_get_chan_and_len(setup, SRC_TYPE, &src_chan, &src_len); \
    for(i=0;i<src_len;++i) { \
        rule = &src_chan[i]; \
        if (!NDP120_DSP_FLOW_RULE_IS_VALID(*rule)) continue; \
        DEBUG_PRINTF("[%d] %s%d-->%s%d\n", \
        rule->set_id, \
        NDP120_DSP_DATA_FLOW_SRC_TYPE_STR(SRC_TYPE), rule->src_param, \
        NDP120_DSP_DATA_FLOW_RULE_DST_STR(*rule), rule->dst_param \
        ); \
    }
void syntiant_ndp120_dsp_flow_setup_dump(ndp120_dsp_data_flow_setup_t *setup) {
    unsigned int i, src_len;
    ndp120_dsp_data_flow_rule_t *src_chan, *rule;

    COLUMN_DUMP(NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO);
    COLUMN_DUMP(NDP120_DSP_DATA_FLOW_SRC_TYPE_FUNCTION);
    COLUMN_DUMP(NDP120_DSP_DATA_FLOW_SRC_TYPE_NN);
}
#else
void syntiant_ndp120_dsp_flow_setup_dump(ndp120_dsp_data_flow_setup_t *setup) {
    (void)setup;
}
#endif

/*---Experimental feature---*/
int syntiant_ndp120_config_auto_clock_scaling(
    struct syntiant_ndp_device_s *ndp, uint32_t enable) {
    int s0,s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t payload_in_MSB = 0;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;

    payload_in_MSB = NDP120_H2D_MB_SET_PAYLOAD(payload_in_MSB, enable);
    s = syntiant_ndp120_write_block(ndp, 1, NDP120_DSP_MB_H2D_ADDR,
        &payload_in_MSB, sizeof(uint32_t));
    if (s) goto error;

    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
        NDP120_DSP_MB_H2D_EXT_AUTO_CLK_SCALING, NULL);
    if (s) goto error;

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

#if 0

static void clear_i2c_state(void){
    /* clear state */
    i2c_state.status  = 0;
    i2c_state.phase   = 0;
    i2c_state.num_bytes = 0;
    i2c_state.device_addr = 0;
    i2c_state.timeout_count = 0;
    i2c_state.timeout_limit = 2000000;
}
#endif

static int i2c_reset(struct syntiant_ndp_device_s *ndp) {
    int s;
    uint32_t data;
    /* Enable the interface */
    s = ndp_mcu_read(NDP120_CHIP_CONFIG_I2CCFG0, &data);
    if (s) goto done;
    /* hw_enable has to be 0;
       1 means i2c is "literally means the I2C block is
       completely under control of a HW state machine"
       Per David Garrett, 3 Feb 2022 */
    data = NDP120_CHIP_CONFIG_I2CCFG0_HW_ENABLE_MASK_INSERT(data, 0);
    data = NDP120_CHIP_CONFIG_I2CCFG0_RESET_MASK_INSERT(data, 1);
    s = ndp_mcu_write(NDP120_CHIP_CONFIG_I2CCFG0, data);
    if (s) goto done;
    data = NDP120_CHIP_CONFIG_I2CCFG0_RESET_MASK_INSERT(data, 0);
    s = ndp_mcu_write(NDP120_CHIP_CONFIG_I2CCFG0, data);
    if (s) goto done;

done:
    return s;
}

#define int_ceil(x,y) (((x) + (y) - 1) / (y))

#define I2C_GLITCH_FILTER_NANOSECONDS 50

int syntiant_ndp120_config_i2c(
    struct syntiant_ndp_device_s *ndp,
    syntiant_ndp120_config_i2c_t *config)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint32_t main_clk_freq = 0, cur_delay, num_stages, divider_a, divider_b, data;

    DEBUG_PRINTF("->> syntiant_ndp120_config_i2c...\n");
    s = syntiant_ndp120_get_main_clk_freq_no_sync(ndp, &main_clk_freq);
    if(s) goto done;
    if (main_clk_freq == 0) {
        s = SYNTIANT_NDP_ERROR_CONFIG;
        goto done;
    }

    if (config->get) {
        /* read cur freq */
        s = ndp_mcu_read(NDP120_I2C_CONFIG_CR, &data);
        if (s) goto done;
        divider_a = NDP120_I2C_CONFIG_CR_DIV_A_EXTRACT(data);

        /*divider_a = main_clk_freq / (config->clk_rate * 22 + 1);*/
        if (divider_a) {
            config->clk_rate = main_clk_freq / (divider_a * 22 + 1);
        } else {
            config->clk_rate = 0;
        }

        s = ndp_mcu_read(NDP120_CHIP_CONFIG_I2CCFG0, &data);
        if (s) goto done;
        config->enable = !!NDP120_CHIP_CONFIG_I2CCFG0_ENABLE_INTERFACE_EXTRACT(data);

        goto done;
    }

    DEBUG_PRINTF("  enable=%d\n", config->enable);
    DEBUG_PRINTF("clk_rate=%d\n", config->clk_rate);

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_I2C_ENABLE) {
        if (config->enable) {
            /* Enable the interface */
            s = i2c_reset(ndp);
            if (s) {
                DEBUG_PRINTF(">>> i2c_reset failed s=%d\n", s);
                goto done;
            }

            s = ndp_mcu_read(NDP120_CHIP_CONFIG_I2CCFG0, &data);
            if (s) goto done;
            /* data = D(NDP120_CHIP_CONFIG_I2CCFG0); */
            data = NDP120_CHIP_CONFIG_I2CCFG0_ENABLE_INTERFACE_MASK_INSERT(data, 1);
            data = NDP120_CHIP_CONFIG_I2CCFG0_CLK_ENABLE_MASK_INSERT(data, 1);
            data = NDP120_CHIP_CONFIG_I2CCFG0_PULLUP_MODE_MASK_INSERT(data, NDP120_CHIP_CONFIG_I2CCFG0_PULLUP_MODE_DISABLE);
            s = ndp_mcu_write(NDP120_CHIP_CONFIG_I2CCFG0, data);
            if (s) goto done;

            s = ndp_mcu_read(NDP120_I2C_CONFIG_CR, &data);
            if (s) goto done;
            data = NDP120_I2C_CONFIG_CR_MS_MASK_INSERT(data, 1);
            data = NDP120_I2C_CONFIG_CR_NEA_MASK_INSERT(data, 1);
            data = NDP120_I2C_CONFIG_CR_ACKEN_MASK_INSERT(data, 1);
            data = NDP120_I2C_CONFIG_CR_CLRFIFO_MASK_INSERT(data, 1);
            DEBUG_PRINTF("data=%#08x\n", data);
            s = ndp_mcu_write(NDP120_I2C_CONFIG_CR, data);
            if (s) goto done;

        } else {
            /* Disable the interface */
            s = ndp_mcu_read(NDP120_CHIP_CONFIG_I2CCFG0, &data);
            if (s) goto done;
            data = NDP120_CHIP_CONFIG_I2CCFG0_ENABLE_INTERFACE_MASK_INSERT(data, 0);
            data = NDP120_CHIP_CONFIG_I2CCFG0_HW_ENABLE_MASK_INSERT(data, 0);
            data = NDP120_CHIP_CONFIG_I2CCFG0_CLK_ENABLE_MASK_INSERT(data, 0);
            s = ndp_mcu_write(NDP120_CHIP_CONFIG_I2CCFG0, data);
            if (s) goto done;
        }
    }

    if (config->set & SYNTIANT_NDP120_CONFIG_SET_I2C_CLK_RATE) {
        DEBUG_PRINTF("main_clk_freq=%d clk_rate=%d\n", main_clk_freq, config->clk_rate);
        /*divider_a = main_clk_freq / (config->clk_rate * 22 + 1);*/
        divider_a = int_ceil(main_clk_freq, config->clk_rate * 22 + 1);
        divider_b = 0;

        /* Turn on the I2C Controller mode */
        /* Config I2CS */
        s = ndp_mcu_read(NDP120_I2C_CONFIG_CR, &data);
        if (s) goto done;
        data = NDP120_I2C_CONFIG_CR_DIV_A_MASK_INSERT(data, divider_a);
        data = NDP120_I2C_CONFIG_CR_DIV_B_MASK_INSERT(data, divider_b);
        DEBUG_PRINTF("data=%#08x\n", data);
        s = ndp_mcu_write(NDP120_I2C_CONFIG_CR, data);
        if (s) goto done;

        /* Set I2C Glitch Filter */
        /* cur_delay = (uint32_t)((uint32_t)1e9 / main_clk_freq / 2); */
        cur_delay = (uint32_t)1e9 / main_clk_freq;
        cur_delay >>= 1; /* divide by 2 */
        num_stages = cur_delay < I2C_GLITCH_FILTER_NANOSECONDS  ? (I2C_GLITCH_FILTER_NANOSECONDS - 1) / cur_delay : 0;

        s = ndp_mcu_read(NDP120_CHIP_CONFIG_I2CSHADOWGFCR, &data);
        if (s) goto done;
        data = NDP120_CHIP_CONFIG_I2CSHADOWGFCR_GFILTER_DEPTH_MASK_INSERT(data, num_stages);
        s = ndp_mcu_write(NDP120_CHIP_CONFIG_I2CSHADOWGFCR, data);
        if (s) goto done;
    }

done:
    return s;
}

static void
syntiant_ndp120_dsp_pick_flowset_ids(ndp120_dsp_data_flow_rule_t *src_rules,
    uint8_t num_rules, uint8_t *flow_map)
{
    int i;
    ndp120_dsp_data_flow_rule_t *rule;

    for (i = 0; i < num_rules; i++) {
        rule = (src_rules + i);
        if (rule->dst_type != NDP120_DSP_DATA_FLOW_DST_TYPE_NONE &&
            rule->set_id != SYNTIANT_NDP120_DSP_FLOWSET_DISABLE_ALL) {
            *flow_map |= (uint8_t) (1 << rule->set_id);
        }
    }
}

int
syntiant_ndp120_dsp_get_flowsets(
    struct syntiant_ndp_device_s *ndp,
    struct syntiant_ndp120_flowsets *flowset)
{
    int s0, i, k = 0;
    uint8_t flow_map = 0, mask = 0, pos = 0;
    uint32_t rule_adx, rule_size, current_flowset_id;
    uint32_t fw_st_adx = ndp->d.ndp120.dsp_fw_state_addr;
    ndp120_dsp_data_flow_rule_t src_rules[NDP120_FUNC_DATA_FLOW_RULE_MAX];

    int s = SYNTIANT_NDP_ERROR_NONE;
    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;


    if (!fw_st_adx) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    fw_st_adx += (uint32_t) offsetof(ndp120_dsp_fw_base_t, data_flow);

    if (!flowset) goto error;
    memset(flowset, 0, sizeof(*flowset));

    for (i = 0; i < SYNTIANT_NDP120_MAX_FLOW_SETS; i++) {
        flowset->flowset_ids[i] = SYNTIANT_NDP120_DSP_FLOWSET_DISABLE_ALL;
    }

    rule_size = sizeof(src_rules[0]);
    memset(src_rules, 0, sizeof(src_rules));
    /* read flow rules with PCM as src */
    s = ndp_mcu_read_block(fw_st_adx, (void*)src_rules, rule_size *
          NDP120_FUNC_DATA_FLOW_RULE_MAX);
    if(s) goto error;
    syntiant_ndp120_dsp_pick_flowset_ids(src_rules,
        NDP120_PCM_DATA_FLOW_RULE_MAX, &flow_map);

    /* read flow rules with FUNC as src */
    rule_adx = fw_st_adx + (uint32_t) offsetof(ndp120_dsp_data_flow_setup_t,
                  src_function);
    memset(src_rules, 0, sizeof(src_rules));
    s = ndp_mcu_read_block(rule_adx, (void*)src_rules, rule_size *
          NDP120_FUNC_DATA_FLOW_RULE_MAX);
    if(s) goto error;
    syntiant_ndp120_dsp_pick_flowset_ids(src_rules,
        NDP120_FUNC_DATA_FLOW_RULE_MAX, &flow_map);

    /* read flow rules with NN as src */
    rule_adx = fw_st_adx + (uint32_t) offsetof(ndp120_dsp_data_flow_setup_t,
                  src_nn);
    memset(src_rules, 0, sizeof(src_rules));
    s = ndp_mcu_read_block(rule_adx, (void*)src_rules, rule_size *
          NDP120_NN_DATA_FLOW_RULE_MAX);
    if(s) goto error;
    syntiant_ndp120_dsp_pick_flowset_ids(src_rules,
        NDP120_NN_DATA_FLOW_RULE_MAX, &flow_map);

    /* read flow rules with MCU as src */
    rule_adx = fw_st_adx + (uint32_t) offsetof(ndp120_dsp_data_flow_setup_t,
                  src_mcu);
    memset(src_rules, 0, sizeof(src_rules));
    s = ndp_mcu_read_block(rule_adx, (void*)src_rules, rule_size *
          NDP120_MCU_DATA_FLOW_RULE_MAX);
    if(s) goto error;
    syntiant_ndp120_dsp_pick_flowset_ids(src_rules,
        NDP120_MCU_DATA_FLOW_RULE_MAX, &flow_map);

    /* read current active flowset id */
    rule_adx = ndp->d.ndp120.dsp_fw_state_addr +
            (uint32_t) offsetof(ndp120_dsp_fw_base_t, data_flow_current_set_id);
    s = ndp_mcu_read(rule_adx, &current_flowset_id);
    if (s) goto error;

    flowset->active_flowset_id = (uint8_t) current_flowset_id;
    for (i = 0; i < SYNTIANT_NDP120_MAX_FLOW_SETS; i++) {
        mask = (uint8_t) (1U << i);
        if (flow_map & mask) {
            flowset->flowset_ids[k++] = pos;
            flowset->num_flowsets++;
        }
        pos++;
    }

error:
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}

int syntiant_ndp120_init_sensors(struct syntiant_ndp_device_s *ndp)
{
    int s0, s = SYNTIANT_NDP_ERROR_NONE;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) return s;
    s = syntiant_ndp120_do_mailbox_req_no_sync(ndp,
                                 NDP120_DSP_MB_H2D_EXT_SENSOR_INIT, NULL);
    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
    return s;
}
