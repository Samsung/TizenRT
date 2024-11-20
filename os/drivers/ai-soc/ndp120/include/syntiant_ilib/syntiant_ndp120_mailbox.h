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
 * Syntiant NDP120 SPI mailbox protocol definitions
 */
#ifndef SYNTIANT_NDP120_MAILBOX_H
#define SYNTIANT_NDP120_MAILBOX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <syntiant-dsp-firmware/ndp120_dsp_mailbox.h>
#include <syntiant-firmware/ndp120_mb.h>

#define NDP120_H2D_PAYLOAD_MASK 0xFFFFFF00

#define NDP120_H2D_SET_MESSAGE(x, y)     ((x & NDP120_H2D_PAYLOAD_MASK) | y)

const char *syntiant_ndp120_dsp_mb_op_decoder(uint8_t msg);
const char *syntiant_ndp120_mcu_mb_op_decoder(uint8_t op);
const char *syntiant_ndp120_mcu_mb_error_decoder(uint8_t error);
void syntiant_ndp120_mcu_mb_debug_print(const char *string, uint8_t data);

enum syntiant_ndp120_mcu_mb_op_e {
    SYNTIANT_NDP120_MB_MCU_NOP              = 0x0,
    SYNTIANT_NDP120_MB_MCU_ACK              = SYNTIANT_NDP120_MB_MCU_NOP,
    SYNTIANT_NDP120_MB_MCU_CONT             = 0x1,
    SYNTIANT_NDP120_MB_MCU_MATCH            = 0x2,
    SYNTIANT_NDP120_MB_MCU_PREPARE          = NDP_MBIN_REQUEST_PREPARE,
    SYNTIANT_NDP120_MB_MCU_DATA             = 0x4,
    SYNTIANT_NDP120_MB_PH_HEAP_ALLOCATION   = NDP_MBIN_REQUEST_PH_HEAP_ALLOCATION,
    SYNTIANT_NDP120_MB_MCU_SECURE_CMD       = 0x5,
    SYNTIANT_NDP120_MB_MCU_FLASHBOOOT_CMD   = 0x6,
    SYNTIANT_NDP120_MB_MCU_MIADDR           = 0x8,
    SYNTIANT_NDP120_MB_MCU_LOAD             = 0x9,
    SYNTIANT_NDP120_MB_DSP_ADX_UPPER        = 0xB,
    SYNTIANT_NDP120_MB_DSP_ADX_LOWER        = 0xC,
    SYNTIANT_NDP120_MB_MCU_WATERMARK        = NDP120_DSP_MB_H2D_WATERMARK,
    SYNTIANT_NDP120_MB_MCU_H2D_EXT_MB       = 0xE,
    SYNTIANT_NDP120_MB_MCU_RUNNING          = 0x10,
    SYNTIANT_NDP120_MB_MCU_BOOTING          = 0x11,
    SYNTIANT_NDP120_MB_MCU_LOAD_DONE        = 0x12,
    SYNTIANT_NDP120_MB_MCU_RUNNING_TEST     = 0x13,
    SYNTIANT_NDP120_MB_OTHER_LOAD_DONE      = 0x14,
    SYNTIANT_NDP120_MB_MCU_HEARTBEAT        = 0x17,
    SYNTIANT_NDP120_MB_MCU_SECURE_EXTRACT   = 0x18,

    /* bootloader mb command */
    SYNTIANT_NDP120_MB_MCU_BOOT_FROM_FLASH  = 0x20
};

enum syntiant_ndp120_error_bit_masks {
    SYNTIANT_NDP120_MB_MCU_ERROR_MASK       = 0x20,
    SYNTIANT_NDP120_MB_MCU_DATA_MASK        = 0x40
};

enum syntiant_ndp120_mcu_network {
    SYNTIANT_NDP120_MB_MCU_NETWORK_MASK     = 0x40
};

enum {
    SYNTIANT_NDP120_MB_MCU_GPIO_CHANGE_LOW  = NDP120_DSP_MB_D2M_GPIO_CHANGE_LOW,
    SYNTIANT_NDP120_MB_MCU_GPIO_CHANGE_HIGH = NDP120_DSP_MB_D2M_GPIO_CHANGE_HIGH
};

#if 0
/* error codes (5-bit) */
enum syntiant_ndp120_mb_error_e {
    NDP_MB_ERROR_FAIL               = 0x0,
    NDP_MB_ERROR_UNEXPECTED         = 0x1,
    NDP_MB_ERROR_PACKAGE_MAGIC_TLV  = 0x2,
    NDP_MB_ERROR_PACKAGE_FW_SIZE    = 0x3,
    NDP_MB_ERROR_PACKAGE_INTEGRITY  = 0x4,
    NDP_MB_ERROR_PACKAGE_MISSING_FW = 0x5,
    NDP_MB_ERROR_PACKAGE_FORMAT     = 0x6,
    NDP_MB_ERROR_AUTH               = 0x7,
    SYNTIANT_NDP10x_MB_ERROR_AES    = 0x8
};
#endif

#ifdef __cplusplus
}
#endif

#endif
