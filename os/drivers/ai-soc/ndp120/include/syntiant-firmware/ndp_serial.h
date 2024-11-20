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

#ifndef NDP_SERIAL_H
#define NDP_SERIAL_H


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief GPIO bit roles
 */
enum NDP_FW_GPIO_ROLE_e {
    NDP_FW_GPIO_ROLE_NONE = 0,    /* firmware will not touch   */
    NDP_FW_GPIO_ROLE_IDATA = 1,   /* I2C data                  */
    NDP_FW_GPIO_ROLE_ICLK = 2,    /* I2C clock                 */
    NDP_FW_GPIO_ROLE_MMISO = 3,   /* SPI master MISO           */
    NDP_FW_GPIO_ROLE_MMOSI = 4,   /* SPI master MOSI           */
    NDP_FW_GPIO_ROLE_MSCLK = 5,   /* SPI master clock          */
    NDP_FW_GPIO_ROLE_MSSEL = 6,   /* SPI slave select          */
    NDP_FW_GPIO_ROLE_I2SCLK = 7,  /* I2S emulator bit clock    */
    NDP_FW_GPIO_ROLE_INTEF = 8,   /* falling edge interrupt    */
    NDP_FW_GPIO_ROLE_INTER = 9,   /* rising edge interrupt     */
    NDP_FW_GPIO_ROLE_INTLL = 10,  /* low level interrupt       */
    NDP_FW_GPIO_ROLE_INTLH = 11,  /* high level interrupt      */
    NDP_FW_GPIO_ROLE_OUT   = 12   /* output                    */
};

/**
 * @brief sensor driver device type identifier
 */
enum NDP_FW_SENSOR_ID_e {
    NDP_FW_SENSOR_ID_NONE = 0,          /* no sensor                                            */
    NDP_FW_SENSOR_ID_BMI160 = 1,        /* Bosch BMI160 (6 or 10 axis)                          */
    NDP_FW_SENSOR_ID_VM3011 = 2,        /* VM3011 energy detecting microphone                   */
    NDP_FW_SENSOR_ID_EDGE_INT = 3,      /* Generic edge interrupt sensor such as LiteOn LTR559  */
    NDP_FW_SENSOR_ID_DA217 = 4,         /* Miramems DA217 motion sensor                         */
    NDP_FW_SENSOR_ID_KX120 = 5,         /* Kionix KX120 motion sensor                           */
    NDP_FW_SENSOR_ID_MC3419 = 6,        /* Kionix MC3419 motion sensor                          */
    NDP_FW_SENSOR_ID_GPIO_DEBOUNCE = 7, /* GPIO pin that requires debouncing                    */
    NDP_FW_SENSOR_ID_SPL_THRESHOLD = 8, /* Toggle pin output on SPL threshold                   */
    NDP_FW_SENSOR_ID_T5838 = 9,         /* TDK T5838 mic                                        */
    NDP_FW_SENSOR_ID_ICM42670 = 10,     /* TDK ICM-42670 IMU                                    */
    NDP_FW_SENSOR_ID_PAG7920 = 11,      /* Pixart image sensor                                  */
    NDP_FW_SENSOR_ID_LAST = NDP_FW_SENSOR_ID_PAG7920
};

enum ndp_firmware_serial_constants_e {
    NDP_FW_SERIAL_BYTE_MAX = 12,
    NDP_FW_SERIAL_CONTROL_RUN_SHIFT = 0,
    NDP_FW_SERIAL_CONTROL_RUN_MASK = 0x1 << NDP_FW_SERIAL_CONTROL_RUN_SHIFT,
    NDP_FW_SERIAL_CONTROL_CONTINUE_SHIFT = 1,
    NDP_FW_SERIAL_CONTROL_CONTINUE_MASK = 0x1 << NDP_FW_SERIAL_CONTROL_CONTINUE_SHIFT,
    NDP_FW_SERIAL_CONTROL_STATUS_SHIFT = 6,
    NDP_FW_SERIAL_CONTROL_STATUS_MASK = 0x3 << NDP_FW_SERIAL_CONTROL_STATUS_SHIFT,
    NDP_FW_SERIAL_CONTROL_STATUS_SUCCESS = 0,
    NDP_FW_SERIAL_CONTROL_STATUS_TIMEOUT = 1,
    NDP_FW_SERIAL_CONTROL_STATUS_ERROR = 2,
    NDP_FW_SERIAL_CONTROL_ADDRESS_SHIFT = 8,
    NDP_FW_SERIAL_CONTROL_ADDRESS_MASK = 0xff << NDP_FW_SERIAL_CONTROL_ADDRESS_SHIFT,
    NDP_FW_SERIAL_CONTROL_OUTLEN_SHIFT = 16,
    NDP_FW_SERIAL_CONTROL_OUTLEN_MASK = 0x3f << NDP_FW_SERIAL_CONTROL_OUTLEN_SHIFT,
    NDP_FW_SERIAL_CONTROL_INLEN_SHIFT = 22,
    NDP_FW_SERIAL_CONTROL_INLEN_MASK = 0x3f << NDP_FW_SERIAL_CONTROL_INLEN_SHIFT,

    NDP_FW_SERIAL_ADDRESS_I2C_ADDRESS_MASK = 0x7f,
    NDP_FW_SERIAL_ADDRESS_I2C_ADDRESS_SHIFT = 0,
    NDP_FW_SERIAL_ADDRESS_SPI_SELECT_GPIO_MASK = 0x1f,
    NDP_FW_SERIAL_ADDRESS_SPI_SELECT_GPIO_SHIFT = 0,
    NDP_FW_SERIAL_ADDRESS_SPI_MODE_MASK = 0x60,
    NDP_FW_SERIAL_ADDRESS_SPI_MODE_SHIFT = 5,
    NDP_FW_SERIAL_ADDRESS_I2C_MASK = 0x80,
    NDP_FW_SERIAL_ADDRESS_I2C_SHIFT = 7,

    NDP_FW_SENSOR_MAX = 4,

    NDP10X_FW_GPIO_MAX = 8,
    NDP120_FW_GPIO_MAX = 32,

    NDP_FW_SENSOR_AXIS_MAX = 15,
    NDP_FW_SENSOR_CONTROL_ID_MASK = 0xff,
    NDP_FW_SENSOR_CONTROL_ID_SHIFT = 0,
    NDP_FW_SENSOR_CONTROL_ADDRESS_MASK = 0xff00,
    NDP_FW_SENSOR_CONTROL_ADDRESS_SHIFT = 8,
    NDP_FW_SENSOR_CONTROL_INT_GPIO_MASK = 0x1f0000,
    NDP_FW_SENSOR_CONTROL_INT_GPIO_SHIFT = 16,

    /* bits 21-27 not used */
    NDP_FW_SENSOR_CONTROL_AXES_MASK = 0x3C000000,
    NDP_FW_SENSOR_CONTROL_AXES_SHIFT = 26,

    NDP_FW_SENSOR_ENABLE_TANK_SHIFT = 0,
    NDP_FW_SENSOR_ENABLE_TANK_MASK = 0x7fff,
    NDP_FW_SENSOR_ENABLE_INPUT_MASK = 0x7fff0000,
    NDP_FW_SENSOR_ENABLE_INPUT_SHIFT = 16,

    NDP_FW_BMI160_PARAMETERS_TEST_ISIZE = 12,
    NDP_FW_BMI160_PARAMETERS_TEST_ISIZE_SCALE = 16,
    NDP_FW_BMI160_PARAMETERS_INT_PASSTHROUGH = 13,
    NDP_FW_BMI160_PARAMETERS_INT_PASSTHROUGH_TEST = 0xff,
    NDP_FW_BMI160_PARAMETERS_FLUSH_THRESHOLD = 14,
    NDP_FW_BMI160_PARAMETERS_DNN_INPUT_THRESHOLD = 15,

    NDP_FW_BMI160_TEST_BASE = 0x20012800,

    NDP_FW_VM3011_PARAMETERS_NORMAL_CLOCK_DIV = 0,
    NDP_FW_VM3011_PARAMETERS_TIMEOUT_COUNT = 1,
    NDP_FW_VM3011_PARAMETERS_CURRENT_THRESHOLD = 2,
    NDP_FW_VM3011_PARAMETERS_PGA_MAX_GAIN = 3,
    NDP_FW_VM3011_PARAMETERS_MATCH_MIN_SPL = 4,
    NDP_FW_VM3011_PARAMETERS_TEST_SPL = 5,

    NDP_FW_NDP_PARAMETERS_EDGE_INT_ID = 0,

    NDP_FW_DA217_PARAMETERS_PICK_UP_ID = 0,
    NDP_FW_DA217_PARAMETERS_SIGNIFICANT_MOTION_ID = 1,
    NDP_FW_DA217_PARAMETERS_TILT_ID = 2,
    NDP_FW_DA217_PARAMETERS_DOUBLE_TAP_ID = 3,
    NDP_FW_DA217_PARAMETERS_FREE_FALL_ID = 4,
    NDP_FW_DA217_PARAMETERS_STEP_DETECT_ID = 5,

    NDP_FW_DA217_EVENT_TYPES = 6,
    NDP_FW_DA217_PARAMETERS_TEST_TRIGGER = 15,
    NDP_FW_DA217_NUM_AXES = 3,
    NDP_FW_DA217_BYTES_PER_AXIS = 2,
    NDP_FW_DA217_BYTES_PER_READ = NDP_FW_DA217_NUM_AXES * NDP_FW_DA217_BYTES_PER_AXIS + 2,
    NDP_FW_DA217_MAIN_BUF_ENTRIES = 128,
    NDP_FW_DA217_RAW_BUF_ENTRIES = 64,
    NDP_FW_DA217_RAW_BUF_ENTRIES_IN_POWER_2 = 6,
    NDP_FW_DA217_RAW_SAMPLE_RING_BUF_SIZE = NDP_FW_DA217_RAW_BUF_ENTRIES * NDP_FW_DA217_BYTES_PER_READ,

    NDP_FW_ACCEL_PARAMETERS_PICK_UP_ID = 0,
    NDP_FW_ACCEL_PARAMETERS_SIGNIFICANT_MOTION_ID = 1,
    NDP_FW_ACCEL_PARAMETERS_TILT_ID = 2,
    NDP_FW_ACCEL_PARAMETERS_DOUBLE_TAP_ID = 3,
    NDP_FW_ACCEL_PARAMETERS_FREE_FALL_ID = 4,
    NDP_FW_ACCEL_PARAMETERS_STEP_DETECT_ID = 5,

    NDP_FW_ACCEL_EVENT_TYPES = 6,
    NDP_FW_ACCEL_PARAMETERS_TEST_TRIGGER = 15,
    NDP_FW_ACCEL_NUM_AXES = 3,
    NDP_FW_ACCEL_BYTES_PER_AXIS = 2,
    NDP_FW_ACCEL_BYTES_PER_READ = NDP_FW_ACCEL_NUM_AXES * NDP_FW_ACCEL_BYTES_PER_AXIS + 2,
    NDP_FW_ACCEL_MAIN_BUF_ENTRIES = 256,
    NDP_FW_ACCEL_RAW_BUF_ENTRIES = 8,
    NDP_FW_ACCEL_RAW_BUF_ENTRIES_IN_POWER_2 = 3,
    NDP_FW_ACCEL_RAW_SAMPLE_RING_BUF_SIZE = NDP_FW_ACCEL_RAW_BUF_ENTRIES * NDP_FW_ACCEL_BYTES_PER_READ,
    NDP_FW_SENSOR_MAX_INPUT_MEMBERS = 128
};

typedef enum ndp_sensor_driver_cause_e {
    NDP_SENSOR_DRIVER_CAUSE_DNN = 0,
    NDP_SENSOR_DRIVER_CAUSE_INT = 1,
    NDP_SENSOR_DRIVER_CAUSE_TIMER = 2
} ndp_sensor_driver_cause_t;


/* currently define KX120. Will need to unify buffer sizes
   across all motion sensors */
#define  KX120

#ifdef KX120
#define NDP_RAW_SAMPLE_RING_BUF_SIZE   NDP_FW_ACCEL_RAW_SAMPLE_RING_BUF_SIZE
#define NDP_MAIN_BUF_ENTRIES           NDP_FW_ACCEL_MAIN_BUF_ENTRIES
#define NDP_ACCEL_NUM_AXES             NDP_FW_ACCEL_NUM_AXES
#else
#define NDP_RAW_SAMPLE_RING_BUF_SIZE   NDP_FW_DA217_RAW_SAMPLE_RING_BUF_SIZE
#define NDP_MAIN_BUF_ENTRIES           NDP_FW_DA217_MAIN_BUF_ENTRIES
#define NDP_ACCEL_NUM_AXES             NDP_FW_DA217_NUM_AXES
#endif


/**
 * @brief serial communications control area
 */
typedef struct ndp_fw_serial_s {
    uint32_t control; /** address, outlen, inlen, finish */
    uint32_t data[NDP_FW_SERIAL_BYTE_MAX / 4]; /** serial data buffer */
} ndp_fw_serial_t;

#define SERIAL_DATA8(s) ((uint8_t *) (s)->data)
#define SERIAL_DATA16(s) ((uint16_t *) (s)->data)
#define SERIAL_DATA32(s) ((s)->data)

/**
 * @brief attached sensor configuration
 */
typedef struct ndp_fw_sensor_configuration_s {
    uint32_t control;   /** sensor control fields */
    uint32_t enable;    /** tank and input enable bits */
    uint8_t parameter[NDP_FW_SENSOR_AXIS_MAX + 1];
    /** per-driver configuration, e.g. axis input shift */
} ndp_fw_sensor_configuration_t;

typedef struct ndp_sensor_vm3011_state_s {
    uint32_t current_timeout;
    uint32_t current_state;
    uint32_t current_threshold;
    uint32_t pga_max_gain;
    uint32_t test_spl;
    int32_t clear_sum;
    uint32_t two_frame_sum;
    uint32_t max_avg;
    uint32_t previous_sum;
} ndp_sensor_vm3011_state_t;

typedef struct ndp_sensor_t5838_state_s {
    uint32_t aad_event_time;
    uint32_t process_mode;
    uint32_t prev_reg_2f_2e_2b_2a; /* [31:0] = 0x2f, 0x2e, 0x2b, 0x2a */
    uint32_t curr_reg_2f_2e_2b_2a;
    uint32_t prev_reg_33_32_31_30; /* [31:0] = 0x33, 0x32, 0x31, 0x30 */
    uint32_t curr_reg_33_32_31_30;
    uint32_t prev_reg_36_35;       /* [15:0] = 0x36, 0x35 */
    uint32_t curr_reg_36_35;
    uint32_t current_state;
    /* bits [1:0] mode: 0 = vad off; 1 = aada; 2 = aadd2 */
    uint32_t vad_control;
    int ack_received;
} ndp_sensor_t5838_state_t;

#ifdef __cplusplus
}
#endif
#endif
