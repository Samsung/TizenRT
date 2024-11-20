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



/* --=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-= */
/*                                                                      */
/*                             IMPORTANT                                */
/*                                                                      */
/* This file is shared between syntiant-firmware and                    */
/* syntiant-dsp-firmware.  If you make changes in one repo              */
/* be sure to update it to the same thing in the other.                 */
/*                                                                      */
/* --=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-= */

#ifndef NDP120_SERIAL_H
#define NDP120_SERIAL_H

#include <syntiant-dsp-firmware/ndp_serial.h>

#ifdef __cplusplus
extern "C" {
#endif

enum ndp120_firmware_serial_constants_e {
    NDP_FW_SENSOR_CONTROL_INT_GPIO1_MASK = 0x3E00000,
    NDP_FW_SENSOR_CONTROL_INT_GPIO1_SHIFT = 21,

    NDP_FW_SERIAL_I2C_WRITE = 0,
    NDP_FW_SERIAL_I2C_READ = 1,

    NDP_FW_SENSOR_SERIAL_INTERFACE_I2C = 0,
    NDP_FW_SENSOR_SERIAL_INTERFACE_SPI0 = 1,
    NDP_FW_SENSOR_SERIAL_INTERFACE_SPI1 = 2,
    NDP_FW_SENSOR_SERIAL_INTERFACE_SPI2 = 3,
    NDP_FW_SENSOR_SERIAL_INTERFACE_SPI3 = 4,

    NDP_FW_SENSOR_PARAMETER_MAX = 16,
    NDP_FW_SENSOR_I2C_TRANSACTION_MAX = 16
};

typedef struct {
    uint32_t int_count;
    uint32_t enable_ctl;
    union {
        ndp_sensor_vm3011_state_t sensor_vm3011;
        ndp_sensor_t5838_state_t sensor_t5838;
    } u;
} ndp120_sensor_state_t;

#ifdef __cplusplus
}
#endif
#endif
