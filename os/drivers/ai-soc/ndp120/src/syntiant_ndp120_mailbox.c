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
#include <syntiant-firmware/ndp120_mb.h>

#include <syntiant_ilib/syntiant_ndp120_mailbox.h>
#include <syntiant_ilib/syntiant_ndp120.h>
#include <syntiant_ilib/syntiant_ndp_error.h>

typedef struct {
    uint8_t op;
    const char *name;
} syntiant_ndp120_mb_names_t;

#if SYNTIANT_NDP120_DEBUG
static const syntiant_ndp120_mb_names_t syntiant_ndp120_mcu_mb_error_names[] = {
    { NDP_MB_ERROR_NONE, "NONE" },
    { NDP_MB_ERROR_UNEXPECTED, "UNEXPECTED" },
    { NDP_MB_ERROR_PACKAGE_MAGIC_TLV, "MAGIC_TLV" },
    { NDP_MB_ERROR_PACKAGE_FW_SIZE, "FW_SIZE" },
    { NDP_MB_ERROR_PACKAGE_INTEGRITY, "INTEGRITY" },
    { NDP_MB_ERROR_PACKAGE_MISSING_FW, "MISSING_FW" },
    { NDP_MB_ERROR_PACKAGE_FORMAT, "PACKAGE_FORMAT" },
    { NDP_MB_ERROR_AUTH, "AUTH" },
    { 0xFF, NULL },
};
#endif

#if SYNTIANT_NDP120_DEBUG
static const syntiant_ndp120_mb_names_t syntiant_ndp120_mcu_mb_op_names[] = {
    { SYNTIANT_NDP120_MB_MCU_NOP, "NOP" },
    { SYNTIANT_NDP120_MB_MCU_CONT, "CONT" },
    { SYNTIANT_NDP120_MB_MCU_MATCH, "MATCH" },
    { SYNTIANT_NDP120_MB_MCU_DATA, "DATA" },
    { SYNTIANT_NDP120_MB_MCU_SECURE_CMD, "SECURE_EXEC_CMD" },
    { SYNTIANT_NDP120_MB_MCU_FLASHBOOOT_CMD, "FLASHBOOT_CMD" },
    { SYNTIANT_NDP120_MB_PH_HEAP_ALLOCATION, "PH_HEAP_ALLOCATION" },
    { SYNTIANT_NDP120_MB_MCU_MIADDR, "MIADDR" },
    { SYNTIANT_NDP120_MB_MCU_LOAD, "LOAD" },
    { SYNTIANT_NDP120_MB_MCU_WATERMARK, "WATERMARK" },
    { SYNTIANT_NDP120_MB_MCU_RUNNING, "RUNNING" },
    { SYNTIANT_NDP120_MB_MCU_BOOTING, "BOOTING" },
    { SYNTIANT_NDP120_MB_MCU_RUNNING_TEST, "RUNNING_TEST" },
    { SYNTIANT_NDP120_MB_MCU_LOAD_DONE, "LOAD_DONE" },
    { SYNTIANT_NDP120_MB_OTHER_LOAD_DONE, "OTHER_PKG_LOAD_DONE" },
    { SYNTIANT_NDP120_MB_MCU_GPIO_CHANGE_LOW, "GPIO_LOW" },
    { SYNTIANT_NDP120_MB_MCU_GPIO_CHANGE_HIGH, "GPIO_HIGH" },
    { SYNTIANT_NDP120_MB_MCU_HEARTBEAT, "HEARTBEAT" },

    { SYNTIANT_NDP120_MB_MCU_BOOT_FROM_FLASH, "BOOT_FROM_FLASH" },
    { SYNTIANT_NDP120_MB_MCU_H2D_EXT_MB, "Extended H2D MB" },

    /* these are duplicates of some below because they
       go out via mbin, and come back via watermarkint */
    { NDP120_DSP_MB_H2D_RESTART, "RESTART"},
    { NDP120_DSP_MB_H2D_ADX_UPPER, "ADX_UPPER"},
    { NDP120_DSP_MB_H2D_ADX_LOWER, "ADX_LOWER"},
    { NDP120_DSP_MB_H2D_EXT, "EXT"},
    { NDP120_DSP_MB_H2D_REQUEST_NOP, "REQUEST_NOP"},
    { 0xFF, NULL },
};
#endif

#if SYNTIANT_NDP120_DEBUG
static const syntiant_ndp120_mb_names_t syntiant_ndp120_dsp_mb_op_names[] = {
    { NDP120_DSP_MB_D2H_WATERMARK, "WATERMARK" },
    { NDP120_DSP_MB_D2H_EXTRACT_READY, "EXTRACT_READY" },
    { NDP120_DSP_MB_D2H_RUNNING, "RUNNING" },
    { NDP120_DSP_MB_D2H_DEBUG, "DEBUG" },
    { NDP120_DSP_MB_D2H_UNKNOWN_MIC_STATE, "UNKNOWN_MIC_STATE" },
    { NDP120_DSP_MB_H2D_RESTART, "RESTART" },
    { NDP120_DSP_MB_H2D_ADX_UPPER, "ADX_UPPER" },
    { NDP120_DSP_MB_H2D_ADX_LOWER, "ADX_LOWER" },
    { NDP120_DSP_MB_H2D_EXT, "EXT" },
    { NDP120_DSP_MB_H2D_REQUEST_NOP, "REQUEST_NOP"},
    { 0xFF, NULL}
};
#endif


#if SYNTIANT_NDP120_DEBUG
static const char *syntiant_ndp120_find_mb_name(
    uint8_t op, const syntiant_ndp120_mb_names_t *p)
{
    for ( ; p->name; p++) {
        if (p->op == op) return p->name;
    }
    return "<UNKNOWN>";
}
#endif

#if SYNTIANT_NDP120_DEBUG
const char *syntiant_ndp120_dsp_mb_op_decoder(uint8_t msg)
{
    return syntiant_ndp120_find_mb_name(msg, syntiant_ndp120_dsp_mb_op_names);
}
#endif

#if SYNTIANT_NDP120_DEBUG
const char *syntiant_ndp120_mcu_mb_op_decoder(uint8_t op)
{
    return syntiant_ndp120_find_mb_name(op, syntiant_ndp120_mcu_mb_op_names);
}
#endif

#if SYNTIANT_NDP120_DEBUG
const char *syntiant_ndp120_mcu_mb_error_decoder(uint8_t error)
{
    return syntiant_ndp120_find_mb_name(error, syntiant_ndp120_mcu_mb_error_names);
}
#endif

#if SYNTIANT_NDP120_DEBUG
void syntiant_ndp120_mcu_mb_debug_print(const char *string, uint8_t data)
{
    if (data & SYNTIANT_NDP120_MB_MCU_DATA_MASK) {
        uint8_t op = data & (SYNTIANT_NDP120_MB_MCU_DATA_MASK - 1);
        DEBUG_PRINTF("0x%02X (DATA: 0x%02X) %s\n", data, op, string);
    } else if ((data & SYNTIANT_NDP120_MB_MCU_ERROR_MASK) &&
               (data & (SYNTIANT_NDP120_MB_MCU_ERROR_MASK - 1))) {
        uint8_t op = data & (SYNTIANT_NDP120_MB_MCU_ERROR_MASK - 1);
        const char *name = syntiant_ndp120_mcu_mb_error_decoder(op);
        DEBUG_PRINTF("ERROR: %s (0x%02X) %s\n", name, data, string);
    } else {
        uint8_t op = data & 0x7f;
        const char *name = syntiant_ndp120_mcu_mb_op_decoder(op);
        /* format shall contain %x followed by %s */
        DEBUG_PRINTF("0x%02X (%s) %s\n", data, name, string);
    }
}
#endif
