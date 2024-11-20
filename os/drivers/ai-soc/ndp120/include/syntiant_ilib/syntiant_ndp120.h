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
#ifndef SYNTIANT_NDP120_H
#define SYNTIANT_NDP120_H

#ifdef __cplusplus
extern "C" {
#endif

#include <syntiant_ilib/syntiant_ndp.h>
#include <syntiant_packager/syntiant_package_consts.h>
#include <syntiant-firmware/ndp120_firmware.h>
#include <syntiant-firmware/ndp120_mb.h>
#include <syntiant-firmware/ndp120_orchestrator.h>
#include <syntiant_ilib/ndp120_regs.h>
#include <syntiant-dsp-firmware/ndp120_dsp_fw_state.h>
#include <syntiant-dsp-firmware/ndp120_serial.h>
#include <syntiant_ilib/syntiant_ndp120_ph.h>
#include <syntiant-firmware/ndp120_ph.h>

#define SYNTIANT_NDP120_DEBUG 0
#define SYNTIANT_NDP120_DEBUG_SIMULATOR 0
#define SYNTIANT_NDP120_DEBUG_MB 0
#define SYNTIANT_NDP120_DEBUG_POLL 0
#define SYNTIANT_NDP120_DEBUG_NN_META 0
#define SYNTIANT_NDP120_DEBUG_POLL_MATCH 0
#define SYNTIANT_NDP120_DEBUG_PH  0
#define SYNTIANT_NDP120_DEBUG_SEND 0
#define SYNTIANT_NDP120_DEBUG_SECURE 0
#define SYNTIANT_NDP120_DEBUG_NO_TOUCH  0
#define SYNTIANT_NDP120_DEBUG_FLASH_BOOT  0
#define SYNTIANT_NDP120_HEXDUMP 0

#define PCM_AUDIO_SAMPLE_WIDTH_BITS 16
#define PCM_AUDIO_SAMPLE_WIDTH_BYTES (PCM_AUDIO_SAMPLE_WIDTH_BITS/8)

#define SYNTIANT_NDP120_AUDIO_SAMPLE_RATE 16000
#define SYNTIANT_NDP120_AUDIO_SAMPLES_PER_WORD 2
#define SYNTIANT_NDP120_MIN_AUT0_CLK_SCALING_SPI_SPEED 100000
#define SYNTIANT_NDP120_MAX_AUTO_CLK_SCALING_SPI_SPEED 512000
#define SYNTIANT_NDP120_DEFAULT_SPI_SPEED 1000000

enum flash_boot_mode_ {
    SYNTIANT_NDP120_HOST_BOOT   = 0,
    SYNTIANT_NDP120_SERIAL_BOOT = 1
};

enum syntiant_ndp120_flashboot_mb_cmds {
    SYNTIANT_NDP120_MAIN_CLK_CONFIG = 0x602,
    SYNTIANT_NDP120_PDM_CLK_START = 0x603,
    SYNTIANT_NDP120_PDM_CLK_STOP = 0x604
};

#define SYNTIANT_NDP120_DSP_ALGO_CONFIG_AREA_SIZE_ALIGN  (4)

/* NDP120 supported clock frequencies */
enum syntiant_ndp120_clk_freq_e {
    SYNTINAT_NDP120_LOWPOWER_MODE_10p752MHz = 10752000,
    SYNTINAT_NDP120_LOWPOWER_MODE_15p360MHz = 15360000,
    SYNTIANT_NDP120_LOWPOWER_MODE_16p896MHz = 16896000,
    SYNTINAT_NDP120_LOWPOWER_MODE_21p504MHz = 21504000,
    SYNTINAT_NDP120_LOWPOWER_MODE_24p000MHz = 24000000,
    SYNTINAT_NDP120_LOWPOWER_MODE_30p720MHz = 30720000,
    SYNTINAT_NDP120_NORMALPOWER_MODE_49p152MHz = 49152000,
    SYNTINAT_NDP120_NORMALPOWER_MODE_55p286MHz = 55296000,
    SYNTINAT_NDP120_NORMALPOWER_MODE_61p440MHz = 61440000,
    SYNTINAT_NDP120_HIGHPOWER_MODE_76p800MHz = 76800000,
    SYNTINAT_NDP120_HIGHPOWER_MODE_98p304MHz = 98304000
};

/**
 * @brief EMA memory assist settings
 */
enum syntiant_ndp120_ema_settings_e {
    SYNTIANT_NDP120_EMA_NONE = 0,   /* no change */
    SYNTIANT_NDP120_EMA_4 = 4,  /* EMA 4: 0.90V supported 15.36-22MHz*/
    SYNTIANT_NDP120_EMA_7 = 7   /* EMA 7: 1V,1.1V supported 22-100MHz */
};

enum syntiant_ndp120_spi_mcu_space_selector {
    SYNTIANT_NDP120_SPI = 0,
    SYNTIANT_NDP120_MCU = 1
};

enum syntiant_ndp120_audio_channel_cnt_e{
    SINGLE_CHANNEL = 1,
    TWO_CHANNELS = 2,
    THREE_CHANNELS = 3,
    FOUR_CHANNELS = 4
};

#define OP_SIZE(mcu)    ((mcu) ? 4 : 1)

#define ndp_spi_read_block(reg, data, len) \
    syntiant_ndp120_read_block(ndp, SYNTIANT_NDP120_SPI, reg, data, len)
#define ndp_mcu_read_block(reg, data, len) \
    syntiant_ndp120_read_block(ndp, SYNTIANT_NDP120_MCU, reg, data, len)
#define ndp_spi_write_block(reg, data, len) \
    syntiant_ndp120_write_block(ndp, SYNTIANT_NDP120_SPI, reg, data, len)
#define ndp_mcu_write_block(reg, data, len) \
    syntiant_ndp120_write_block(ndp, SYNTIANT_NDP120_MCU, reg, data, len)

#define ndp_spi_read(reg, data) syntiant_ndp120_read(ndp, SYNTIANT_NDP120_SPI, reg, data)
#define ndp_mcu_read(reg, data) syntiant_ndp120_read(ndp, SYNTIANT_NDP120_MCU, reg, data)
#define ndp_spi_write(reg, data) syntiant_ndp120_write(ndp, SYNTIANT_NDP120_SPI, reg, data)
#define ndp_mcu_write(reg, data) syntiant_ndp120_write(ndp, SYNTIANT_NDP120_MCU, reg, data)

#define BUFDELTA(CUR, LAST, BUFSIZE) ((CUR) + ((CUR) < (LAST) ? (BUFSIZE) : 0) - (LAST))

int DEBUG_PRINTF(const char * fmt, ...);

#define ARRAY_LEN(x) (sizeof(x)/sizeof((x)[0]))

#define SYNTIANT_NDP_SENSOR_ID_NAMES                                        \
    {                                                                       \
        "none", "bmi160", "vm3011", "edge_int", "da217", "kx120", "mc3419", \
            "gpio_debounce", "spl_threshold", "t5838", "icm42670" \
    }
#define SYNTIANT_NDP_SENSOR_ID_NAME(id)                                       \
    ((NDP_FW_SENSOR_ID_LAST < (id))  \
            ? "*unknown*"                                                      \
            : syntiant_ndp_sensor_id_names[id])

#define NDP120_DNN_ISA_COMP0_ACTIVATION_LINEAR 0x0U
#define NDP120_DNN_ISA_COMP0_ACTIVATION_LINEAR_16 0x1U
#define NDP120_DNN_ISA_COMP0_ACTIVATION_RELU 0x2U
#define NDP120_DNN_ISA_COMP0_ACTIVATION_TANH0 0x4U
#define NDP120_DNN_ISA_COMP0_ACTIVATION_TANH1 0x5U
#define NDP120_DNN_ISA_COMP0_ACTIVATION_SIGMOID0 0x6U
#define NDP120_DNN_ISA_COMP0_ACTIVATION_SIGMOID1 0x7U

/**
 * @file syntiant_ndp120.h
 * @date 2019-10-11
 * @brief Interface to Syntiant NDP120 chip-specific interface library functions
 *
 * \section config_api Using the config API
 * A number of config API functions have a signature in the form of:
 * \code{.c}
 * int syntiant_ndp120_config_farrow(struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_farrow_t *config);
 * \endcode
 * Where the `config` struct looks like:
 * \code{.c}
 * typedef struct syntiant_ndp120_config_farrow_s {
 *     uint32_t get;
 *     uint32_t set;
 *     uint32_t interface;
 *     uint32_t bypass;
 *     uint32_t phase_step;
 * } syntiant_ndp120_config_farrow_t;
 * \endcode
 * All config structs contain a `get` and a `set` field which control the
 * operation of the function call. The `get` field is used a boolean value and
 * the `set` field uses bits to control which value(s) to set. For the above
 * example the value for the `set` can be a bitwise union of
 * `SYNTIANT_NDP120_CONFIG_SET_FARROW_BYPASS` and
 * `SYNTIANT_NDP120_CONFIG_SET_FARROW_PHASE_STEP`.
 *
 * The values for `get` and `set` should be mutually exclusive, i.e. only one
 * should be non-zero.
 *
 * Other fields in the struct are either values that read or written to or are
 * used as an option. In the example above, `interface` needs to be set for both
 * `get` & `set` commands (an option), but `bypass` and `phase_step` are values
 * that are either read or written.
 *
 * When using `set` the other fields in the struct should be set to their
 * desired values. When the function returns their values **should not be read**.
 * In order to determine if the `set` was successful, the return code from the
 * function should be read.
 *
 * To manually verify that a config value has been set, the function
 * needs to called twice, once with `set` and again with `get`. E.g:
 * \code{.c}
 * syntiant_ndp120_config_farrow_t farrow_config = { 0 };
 * farrow_config.set = SYNTIANT_NDP120_CONFIG_SET_FARROW_BYPASS;
 * farrow_config.bypass = 1;
 * int s = syntiant_ndp120_config_farrow(ndp, &farrow_config);
 * if (s) {
 *     // handle error... exit/return/etc
 * }
 * // to double check the value...
 * memset(&farrow_config, 0, sizeof(farrow_config));
 * farrow_config.get = 1;
 * s = syntiant_ndp120_config_farrow(ndp, &farrow_config);
 * if (s) {
 *     // handle error
 * }
 * printf("farrow bypass is %d\n", farrow_config.bypass);
 * \endcode
 */

/**
 * @brief NDP120 firmware posterior handler set flags
 */
enum syntiant_ndp120_posterior_config_action_type_e {
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_ACTION_TYPE_MATCH = 0,
    /**< report a match in the summary and go to state 0 */
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_ACTION_TYPE_STATE = 1,
    /**< go to state @c action_state */
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_ACTION_TYPE_STAY = 2,
    /**< stay in the current state and leave timer running */
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_ACTION_TYPE_MAX
    = ((uint32_t)(SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_ACTION_TYPE_STAY << 1) - 1)
};

/**
 * @brief NDP120 configuration data
 */

enum {
    /* clk.src */
    SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_REFSEL            = 0x1,
    SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_CLKSEL            = 0x2,
    SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_EXTSEL            = 0x4,
    SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_FORCE_EXT         = 0x8,
    SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_FORCE_INT         = 0x10,
    SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_FORCE_DNN_CLK     = 0x20,
    SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_FORCE_AESOTP_CLK  = 0x40,

    /* clk.div */
    SYNTIANT_NDP120_CONFIG_SET_CLK_DIV_MCUCLKDIV         = 0x1,

    /* clk.pll */
    SYNTIANT_NDP120_CONFIG_SET_CLK_PLL_PRESET            = 0x01,

    /* clk.fll */
    SYNTIANT_NDP120_CONFIG_SET_CLK_FLL_PRESET            = 0x01,

    /* clk.ext_freq */
    SYNTIANT_NDP120_CONFIG_SET_CLK_EXT_FREQ_HZ           = 0x01,

    /* clk.xtal */
    SYNTIANT_NDP120_CONFIG_SET_CLK_XTAL_OUT              = 0x01,
    SYNTIANT_NDP120_CONFIG_SET_CLK_XTAL_OSC              = 0x02

};

enum syntiant_ndp_fw_state {
    SYNTIANT_NDP_MCU_FW_ALIVE = 1,
    SYNTIANT_NDP_DSP_FW_ALIVE = 2,
    SYNTIANT_NDP_BOTH_FW_ALIVE =  SYNTIANT_NDP_MCU_FW_ALIVE |
                                  SYNTIANT_NDP_DSP_FW_ALIVE,
    SYNTIANT_NDP_BOTH_FW_DEAD = !SYNTIANT_NDP_BOTH_FW_ALIVE
};

enum syntiant_ndp_load_state {
    SYNTIANT_NDP_MCU_FW_LOADED = 1,
    SYNTIANT_NDP_DSP_FW_LOADED = 2,
    SYNTIANT_NDP_NN_LOADED = 4,
    SYNTIANT_NDP_ALL_LOADED = SYNTIANT_NDP_MCU_FW_LOADED |
                              SYNTIANT_NDP_DSP_FW_LOADED |
                              SYNTIANT_NDP_NN_LOADED,
    SYNTIANT_NDP_FW_NOT_LOADED = !SYNTIANT_NDP_ALL_LOADED
};

enum {
    SYNTIANT_NDP120_MAIN_CLK_SRC_EXT = 1,
    SYNTIANT_NDP120_MAIN_CLK_SRC_PLL = 2,
    SYNTIANT_NDP120_MAIN_CLK_SRC_FLL = 3
};

enum {
    SYNTIANT_NDP120_REF_CLK_PAD = 0,
    SYNTIANT_NDP120_REF_CLK_XTAL = 1
};

enum {
    /* gpio */
    SYNTIANT_NDP120_CONFIG_SET_GPIO_DIR                  = 0x01,
    SYNTIANT_NDP120_CONFIG_SET_GPIO_VALUE                = 0x02,
    SYNTIANT_NDP120_CONFIG_VALUE_GPIO_DIR_OUT            = 0x00,
    SYNTIANT_NDP120_CONFIG_VALUE_GPIO_DIR_IN             = 0x01
};

enum {
    /* PDM setters */
    SYNTIANT_NDP120_CONFIG_SET_PDM_SAMPLE_RATE            = 0x0001,
    SYNTIANT_NDP120_CONFIG_SET_PDM_PDM_RATE               = 0x0002,
    SYNTIANT_NDP120_CONFIG_SET_PDM_CLK_MODE               = 0x0004,
    SYNTIANT_NDP120_CONFIG_SET_PDM_MODE                   = 0x0008,
    SYNTIANT_NDP120_CONFIG_SET_PDM_CLK                    = 0x0010
};

enum {
    /* PDM mode */
    SYNTIANT_NDP120_CONFIG_VALUE_PDM_MODE_NONE,
    SYNTIANT_NDP120_CONFIG_VALUE_PDM_MODE_LEFT,
    SYNTIANT_NDP120_CONFIG_VALUE_PDM_MODE_RIGHT,
    SYNTIANT_NDP120_CONFIG_VALUE_PDM_MODE_STEREO
};

enum {
    /* PDM clock mode */
    SYNTIANT_NDP120_CONFIG_VALUE_PDM_CLK_MODE_EXTERNAL,
    SYNTIANT_NDP120_CONFIG_VALUE_PDM_CLK_MODE_INTERNAL,
    SYNTIANT_NDP120_CONFIG_VALUE_PDM_CLK_MODE_DUAL_INTERNAL
};

enum {
    /* PDM clock stop/start */
    SYNTIANT_NDP120_CONFIG_VALUE_PDM_CLK_OFF,
    SYNTIANT_NDP120_CONFIG_VALUE_PDM_CLK_ON
};

enum {
    /* vad mic control */
    SYNTIANT_NDP120_CONFIG_VALUE_VAD_MIC_MODE_VAD_OFF = 0x0, /* vad off */
    SYNTIANT_NDP120_CONFIG_VALUE_VAD_MIC_MODE_VAD_AADA = 0x1, /* vad aada on */
	SYNTIANT_NDP120_CONFIG_VALUE_VAD_MIC_MODE_VAD_AADD2 = 0x2 /* vad aadd2 on */
};

#define BOOTLOADER_OPEN_RAM_WINDOW_LOWER    (0x20007c00U)
#define BOOTLOADER_OPEN_RAM_WINDOW_UPPER    (0x20008000U)
#define MCU_OPEN_RAM_WINDOW_LOWER           (NDP120_MCU_OPEN_RAM_BGN)
#define MCU_OPEN_RAM_WINDOW_UPPER           (NDP120_MCU_OPEN_RAM_END)
#define NDP120_FLASHBOOT_OPEN_RAM_MATCH_RESULTS (0x20007B94U)

typedef struct syntiant_ndp120_config_pdm_s {
        uint32_t get;
        uint32_t set;
        uint32_t interface;
        uint32_t sample_rate;
        uint32_t pdm_rate;
        uint32_t clk_mode;
        uint32_t mode;
        uint32_t clk; /* off, on */
} syntiant_ndp120_config_pdm_t;

/* mic setters */
enum {
    SYNTIANT_NDP120_CONFIG_MIC_SETTINGS = 1
};

typedef struct syntiant_ndp120_config_mic_s {
        uint32_t get;
        uint32_t set;
        uint8_t mic;
        int8_t sensitivity;
        uint8_t delay;
        uint8_t pad;
} syntiant_ndp120_config_mic_t;

/* decimation setters */
enum {
    SYNTIANT_NDP120_CONFIG_SET_DECIMATION_INSHIFT = 1,
    SYNTIANT_NDP120_CONFIG_SET_DECIMATION_OUTSHIFT = 2
};

typedef struct syntiant_ndp120_config_decimation_s {
        uint32_t get;
        uint32_t set;
        uint32_t mic;
        uint32_t inshift;
        uint32_t outshift;
} syntiant_ndp120_config_decimation_t;

/* gain setters */
enum {
    SYNTIANT_NDP120_CONFIG_SET_GAIN_DCREMOVALMODE = 1,
    SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCSHIFTDIR = 2,
    SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCSHIFTCNT = 4,
    SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCFINEGRAINMUL = 8,
    SYNTIANT_NDP120_CONFIG_SET_GAIN_ZCGAINCHANGE = 16,
    SYNTIANT_NDP120_CONFIG_SET_GAIN_DCSTATICREMVAL = 32
};
/* gain enums */
enum {
    SYNTIANT_NDP120_CONFIG_VALUE_GAIN_DCREMOVALMODE_OFF = NDP120_DSP_CONFIG_PDMCFG_B_DCREMOVALMODE_OFF,
    SYNTIANT_NDP120_CONFIG_VALUE_GAIN_DCREMOVALMODE_STATIC = NDP120_DSP_CONFIG_PDMCFG_B_DCREMOVALMODE_STATIC,
    SYNTIANT_NDP120_CONFIG_VALUE_GAIN_DCREMOVALMODE_ON = NDP120_DSP_CONFIG_PDMCFG_B_DCREMOVALMODE_ON,
    SYNTIANT_NDP120_CONFIG_VALUE_GAIN_AGCSHIFTDIR_LEFT = 0,
    SYNTIANT_NDP120_CONFIG_VALUE_GAIN_AGCSHIFTDIR_RIGHT = 1
};

typedef struct syntiant_ndp120_config_gain_s {
        uint32_t get;
        uint32_t set;
        uint32_t mic;
        uint32_t dcremovalmode;
        uint32_t dcstaticremval;
        uint32_t agcshiftdir;
        uint32_t agcshiftcnt;
        uint32_t agcfinegrainmul;
        uint32_t zcgainchange;
} syntiant_ndp120_config_gain_t;

/* tank setters */
enum {
    SYNTIANT_NDP120_CONFIG_SET_TANK_SAMPLETANK_MSEC = 1
};

/* tank config */
typedef struct syntiant_ndp120_config_tank_s {
        uint32_t get;
        uint32_t set;
        uint32_t sampletank_msec;
        uint32_t sampletank_mem_type;
} syntiant_ndp120_config_tank_t;

/* farrow setters */
enum {
    SYNTIANT_NDP120_CONFIG_SET_FARROW_BYPASS         = 0x01,
    SYNTIANT_NDP120_CONFIG_SET_FARROW_PHASE_STEP     = 0x02
};

typedef struct syntiant_ndp120_config_farrow_s {
        uint32_t get;
        uint32_t set;
        uint32_t interface;
        uint32_t bypass;
        uint32_t phase_step;
} syntiant_ndp120_config_farrow_t;

/* butterworth filter coefficients */
enum {
    SYNTIANT_NDP120_CONFIG_SET_BWCOEF_A1 = 1,
    SYNTIANT_NDP120_CONFIG_SET_BWCOEF_A2 = 2,
    SYNTIANT_NDP120_CONFIG_SET_BWCOEF_B0 = 4,
    SYNTIANT_NDP120_CONFIG_SET_BWCOEF_B1 = 8,
    SYNTIANT_NDP120_CONFIG_SET_BWCOEF_B2 = 16
};

typedef struct syntiant_ndp120_config_bwcoef_s {
    uint32_t get;
    uint32_t set;
    uint32_t mic;
    uint32_t a1;
    uint32_t a2;
    uint32_t b0;
    uint32_t b1;
    uint32_t b2;
} syntiant_ndp120_config_bwcoef_t;

enum syntiant_ndp120_debug_extract_type_e {
    SYNTIANT_NDP120_DEBUG_EXTRACT_TYPE_FW_STATE = 0x00,
    SYNTIANT_NDP120_DEBUG_EXTRACT_TYPE_PH_STATE = 0x02,
    SYNTIANT_NDP120_DEBUG_EXTRACT_NN_NUM = 0x04,
    SYNTIANT_NDP120_DEBUG_EXTRACT_TYPE_DSP_FW_STATE = 0x08,
    SYNTIANT_NDP120_DEBUG_EXTRACT_TYPE_LAST
        = SYNTIANT_NDP120_DEBUG_EXTRACT_TYPE_DSP_FW_STATE
};

enum syntiant_ndp120_clock_frequency_type {
   SYNTIANT_NDP120_CLOCK_MODE_PLL_0P9V_32768_10P752MHZ = 0,
   SYNTIANT_NDP120_CLOCK_MODE_PLL_0P9V_32768_15P36MHZ = 1,
   SYNTIANT_NDP120_CLOCK_MODE_PLL_0P9V_32768_21P504MHZ = 2,
   SYNTIANT_NDP120_CLOCK_MODE_PLL_0P9V_4P096MHZ_15P36MHZ = 3,
   SYNTIANT_NDP120_CLOCK_MODE_PLL_0P9V_4P096MHZ_21P504MHZ = 4,
   SYNTIANT_NDP120_CLOCK_MODE_PLL_1V_32768_49P152MHZ = 5,
   SYNTIANT_NDP120_CLOCK_MODE_PLL_1V_32768_55P296MHZ = 6,
   SYNTIANT_NDP120_CLOCK_MODE_PLL_1V_4P096MHZ_49P152MHZ = 7,
   SYNTIANT_NDP120_CLOCK_MODE_PLL_1P1V_32768_55P296MHZ = 8,
   SYNTIANT_NDP120_CLOCK_MODE_PLL_1P1V_32768_76P8MHZ = 9,
   SYNTIANT_NDP120_CLOCK_MODE_PLL_1P1V_32768_98P304MHZ = 10,
   SYNTIANT_NDP120_CLOCK_MODE_PLL_1P1V_512000_98P304MHZ = 11,
   SYNTIANT_NDP120_CLOCK_MODE_PLL_1P1V_4P096MHZ_76P8MHZ = 12,
   SYNTIANT_NDP120_CLOCK_MODE_PLL_1P1V_4P096MHZ_98P304MHZ = 13,
   SYNTIANT_NDP120_CLOCK_MODE_FLL_0P9V_32768_15P36MHZ = 14,
   SYNTIANT_NDP120_CLOCK_MODE_FLL_0P9V_32768_16P896MHZ = 15,
   SYNTIANT_NDP120_CLOCK_MODE_15P36MHZ = 20,
   SYNTIANT_NDP120_CLOCK_MODE_21P504MHZ = 21,
   SYNTIANT_NDP120_CLOCK_MODE_NULL = -1
};

typedef void (*ndp120_debug_callback_ptr_t)(char * dbg);
extern ndp120_debug_callback_ptr_t ndp120_debug_callback;
void syntiant_ndp120_set_debug_callback(ndp120_debug_callback_ptr_t p);

/* UNDER REVIEW
 *
 * The below was added for completeness
 * in parity with 10x code, and may or may
 * not be appropriate here.  It's here
 * as a reminder of what _might_ need
 * to be implemented.
 */
/*TODO create a struct abstracting out the common config between 120 and 10x
 * and embed that in 120 config
 */

/**
 * @brief NDP120 configuration data
 */
typedef struct syntiant_ndp120_config_misc_s {
    unsigned int set;              /**< configuration variable set flags */
    unsigned int set1;             /**< configuration variable set flags */
    int get; /**< read all configuration state (touching the chip) */
    /* fields returning valid info even when set == 0 && get == 0 */

    unsigned int image_frame_size;

    /* audio input buffer used frames (read only) */
    unsigned int image_buffer_used;

    /* samples per freq domain frame (read only) */
    unsigned int audio_frame_size;

    /* samples per freq domain step (read only) */
    unsigned int audio_frame_step;

    /* bytes per sample (read only) */
    unsigned int audio_sample_size_bytes;

    /* input source for dnn processing */
    unsigned int input;

    /* streaming features per DNN frame */
    unsigned int dnn_frame_size;

    /* audio input buffer used frames (read only) */
    unsigned int audio_buffer_used;

    /* match interrupt for every frame on/off */
    unsigned int match_per_frame_on;

    /* DSP system sample rate */
    uint32_t sample_rate;

    /* Minimum SPL threshold for match */
    uint32_t spl_thresh;

    /* configure interrupt auto clearance frame count */
    unsigned int frame_count;

    /* configure match clearance setting */
    unsigned int clear_match;

    /* count number of interrupt clearances*/
    unsigned int int_clear_count;

    /* fifo number with 4 being SPI_FIFO */
    uint32_t fifo_number;

    /* fifo_threshold (RW) */
    uint32_t fifo_threshold;

    /* hw_threshold (RO) */
    uint32_t hw_threshold;

    /* neural network number*/
    uint16_t nn_num;

    /* softmax scale factor */
    int16_t quantized_sf;

    /* secondary validation fields */
    uint32_t threshold;
    uint32_t ratio_threshold;
    uint8_t sec_val_type;
    uint8_t num_occurrences;
    uint16_t frame_range_low;
    uint16_t frame_range_high;
    uint16_t num_frames;
    uint8_t notification_type;

    uint8_t pad[3];
    /* vad mic sensor mode */
    uint32_t vad_mode;
} syntiant_ndp120_config_misc_t;


#if 1

/* UNDER REVIEW
 *
 * The below was added for completeness
 * in parity with 10x code, and may or may
 * not be appropriate here.  It's here
 * as a reminder of what _might_ need
 * to be implemented.
 */
/**
 * @brief DNN input sources
 */
enum syntiant_ndp120_config_dnn_input_e {
    SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_NONE = 0,
    /**< DNN input disabled */
    SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_PDM0 = 1,
    /**< PDM microphone, falling clock */
    SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_PDM1 = 2,
    /**< PDM microphone, rising clock */
    SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_PDM_SUM = 3,
    /**< Sum of both PDM microphones */
    SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_I2S_LEFT = 4,
    /**< I2S left input */
    SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_I2S_RIGHT = 5,
    /**< I2S right input */
    SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_I2S_SUM = 6,
    /**< I2S left + right inputs */
    SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_I2S_MONO = 7,
    /**< I2S 1 channel input */
    SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_I2S_DIRECT = 8,
    /**< I2S interface direct feature input (bypass frequency block) */
    SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_SPI = 9,
    /**< SPI interface PCM */
    SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_SPI_DIRECT = 10,
    /**< SPI interface direct feature input (bypass frequency block) */
    SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_MAX
    = SYNTIANT_NDP120_CONFIG_SET_DNN_INPUT_SPI_DIRECT
};
#endif

/* UNDER REVIEW
 *
 * The below was added for completeness
 * in parity with 10x code, and may or may
 * not be appropriate here.  It's here
 * as a reminder of what _might_ need
 * to be implemented.
 */
/**
 * @brief configuration variable set flags
 * @note this struct is sefined the same way in packager file board_TLVs.
 */
enum syntiant_ndp120_config_set_e {
    SYNTIANT_NDP120_CONFIG_SET_MISC_INPUT = 0x1,
    SYNTIANT_NDP120_CONFIG_SET_MISC_MATCH_PER_FRAME_ON = 0x2,
    SYNTIANT_NDP120_CONFIG_SET_MISC_SPL_THRESH = 0x4,
    SYNTIANT_NDP120_CONFIG_SET_MISC_SAMPLE_RATE = 0x8,
    SYNTIANT_NDP120_CONFIG_SET_MISC_FIFO_THRESHOLD = 0x10,
    SYNTIANT_NDP120_CONFIG_SET_MISC_INT_CLEAR = 0x20,
    SYNTIANT_NDP120_CONFIG_SET_MISC_SOFTMAX = 0x40,
    SYNTIANT_NDP120_CONFIG_SET_MISC_SEC_VAL_TYPE = 0x80,
    SYNTIANT_NDP120_CONFIG_SET_MISC_SEC_VAL_CONFIG = 0x100,
    SYNTIANT_NDP120_CONFIG_SET_MISC_VAD_MODE = 0x200,
    SYNTIANT_NDP120_CONFIG_SET_MISC_EXTRACT_READY = 0x400,
    SYNTIANT_NDP120_CONFIG_SET_MISC_ALL_M = 0x7FF
};

/**
 * @brief sensor status variable flags
 */
enum syntiant_ndp120_sensor_status_e {
    SYNTIANT_NDP120_SENSOR_STATUS_INT_COUNT = 0x1,
    SYNTIANT_NDP120_SENSOR_STATUS_ALL_M = 0x2
};

/*
See this doc:
https://docs.google.com/document/d/1autU3UJELTtHqzormwvVYPYPKA0ffQeu6I493j1T4B4/edit#bookmark=id.rwwtxlttjb8b
*/

/* I2S config values */


/*
Notes:
    mode:

    SYNTIANT_NDP120_CONFIG_VALUE_I2S_MODE_STANDARD = 0,
    SYNTIANT_NDP120_CONFIG_VALUE_I2S_MODE_BURST_DNN = 1,  not implemented ndp120
    SYNTIANT_NDP120_CONFIG_VALUE_I2S_MODE_TDM = 2,
    SYNTIANT_NDP120_CONFIG_VALUE_I2S_MODE_PDM = 3,        not implemented ndp120

    packed: All samples from either frame are put into the left (fifo0 or fifo2)

    msb_index: set to location of desired MSB from counting from L

    */

enum {
    /* mode */
    SYNTIANT_NDP120_CONFIG_VALUE_I2S_MODE_STANDARD = 0,
    SYNTIANT_NDP120_CONFIG_VALUE_I2S_MODE_BURST_DNN = 1,
    SYNTIANT_NDP120_CONFIG_VALUE_I2S_MODE_TDM = 2,
    SYNTIANT_NDP120_CONFIG_VALUE_I2S_MODE_PDM = 3,

    /* clk edge */
    SYNTIANT_NDP120_CONFIG_VALUE_I2S_EDGE_POS = 0,
    SYNTIANT_NDP120_CONFIG_VALUE_I2S_EDGE_NEG = 1,

    /* out mode */
    SYNTIANT_NDP120_CONFIG_VALUE_I2S_AUD2_OUT_MODE_MASTER = 0,
    SYNTIANT_NDP120_CONFIG_VALUE_I2S_AUD2_OUT_MODE_SLAVE = 1,

    /* out data source type */
    SYNTIANT_NDP120_CONFIG_VALUE_I2S_AUD2_SRC_TYPE_PCM_AUDIO = NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO,
    SYNTIANT_NDP120_CONFIG_VALUE_I2S_AUD2_SRC_TYPE_FUNCTION = NDP120_DSP_DATA_FLOW_SRC_TYPE_FUNCTION,

    /* delayed flop sensitivity */
    SYNTIANT_NDP120_CONFIG_VALUE_I2S_DELAYED_FLOP_SENSITIVITY_NORMAL = 0,
    SYNTIANT_NDP120_CONFIG_VALUE_I2S_DELAYED_FLOP_SENSITIVITY_DELAYED = 1
};

enum {
    SYNTIANT_NDP120_CONFIG_SET_I2S_MODE                          = 0x0001,
    SYNTIANT_NDP120_CONFIG_SET_I2S_FRAMESIZE                     = 0x0002,
    SYNTIANT_NDP120_CONFIG_SET_I2S_SAMPLESIZE                    = 0x0004,
    SYNTIANT_NDP120_CONFIG_SET_I2S_MSB_INDEX                     = 0x0008,
    SYNTIANT_NDP120_CONFIG_SET_I2S_PACKED                        = 0x0010,
    SYNTIANT_NDP120_CONFIG_SET_I2S_EDGE                          = 0x0020,
    SYNTIANT_NDP120_CONFIG_SET_I2S_DELAYED_FLOP_SENSITIVITY      = 0x0040,
    SYNTIANT_NDP120_CONFIG_SET_I2S_AUDIO_OUT_FS_EXT_ENABLE       = 0x0080,
    SYNTIANT_NDP120_CONFIG_SET_I2S_DUAL_CHANNEL_TDM              = 0x0100,
    SYNTIANT_NDP120_CONFIG_SET_I2S_LEFTCHENABLE                  = 0x0200,
    SYNTIANT_NDP120_CONFIG_SET_I2S_RIGHTCHENABLE                 = 0x0400,
    SYNTIANT_NDP120_CONFIG_SET_I2S_AUD2CLKOUTNEEDED              = 0x0800,
    SYNTIANT_NDP120_CONFIG_SET_I2S_AUD2_OUT_MODE                 = 0x1000,
    SYNTIANT_NDP120_CONFIG_SET_I2S_AUD2_SRC_TYPE                 = 0x2000,
    SYNTIANT_NDP120_CONFIG_SET_I2S_AUD2_SRC_PARAM                = 0x4000,
    SYNTIANT_NDP120_CONFIG_SET_I2S_DISABLE                       = 0x8000
};


typedef struct syntiant_ndp120_config_i2s_s {
    uint32_t get;
    uint32_t set;
    uint32_t interface;
    uint32_t freq; /* sample rate */

    uint32_t mode;
    uint32_t framesize;
    uint32_t samplesize;
    uint32_t msb_index;
    uint32_t packed;
    uint32_t edge;
    uint32_t delayed_flop_sensitivity;
    uint32_t audio_out_fs_ext_enable;
    uint32_t dual_channel_tdm;
    uint32_t leftchenable;
    uint32_t rightchenable;
    uint32_t aud2clkoutneeded;

    uint32_t aud2_out_mode;
    uint32_t aud2_src_type;
    uint32_t aud2_src_param;

} syntiant_ndp120_config_i2s_t;

typedef struct syntiant_ndp120_config_audio_sync_s {
    uint32_t mode;
    uint32_t ref_chan;
    uint32_t adj_chan;
    uint32_t sample_count_offset;
    int32_t chan_delay_ticks; /* positive = ref before adj, negative = adj before ref */
} syntiant_ndp120_config_audio_sync_t;

typedef struct syntiant_ndp120_config_notify_s {
    uint32_t notify_on_sample_ready;
} syntiant_ndp120_config_notify_t;

typedef struct syntiant_ndp120_config_filter_s {
        uint32_t get;
        uint32_t set;
        /* fixme

            --high-pass-cutoff
            --high-pass-coef
            --remove-dc
            --remove-dc
        */
} syntiant_ndp120_config_filter_t;

typedef struct syntiant_ndp120_config_clk_src_s {
            uint32_t get;
            uint32_t set;

            uint32_t refsel;    /* chip_config.clkctl1.refsel */
            uint32_t clksel;    /* chip_config.clkctl1.clksel */
            uint32_t extsel;    /* chip_config.clkctl1.extsel */
            uint32_t force_ext; /* spi.ctl.extsel */
            uint32_t force_int; /* spi.ctl.intsel */

            /* "clock gate for dnn" */
            uint32_t force_dnn_clk; /* chip_config.clkctl0.force_dnn_clk */
            /* "clock gate for aesotp" */
            uint32_t
                force_aesotp_clk; /* chip_config.clkctl0.force_aesotp_clk */

} syntiant_ndp120_config_clk_src_t;

typedef struct syntiant_ndp120_config_clk_xtal_s {
            uint32_t get;
            uint32_t set;
            uint32_t out;
            uint32_t osc;
}syntiant_ndp120_config_clk_xtal_t;

typedef struct syntiant_ndp120_config_clk_pll_s {
            uint32_t get;
            uint32_t set;
            uint32_t preset;
            uint32_t locked;
            uint32_t no_switch;
} syntiant_ndp120_config_clk_pll_t;

typedef struct syntiant_ndp120_config_clk_fll_s {
            uint32_t get;
            uint32_t set;
            uint32_t preset;
            uint32_t locked;
} syntiant_ndp120_config_clk_fll_t;

typedef struct syntiant_ndp120_config_clk_div_s {
            uint32_t get;
            uint32_t set;
            uint32_t mcuclkdiv;         /* chip_config.clkctl0 */
} syntiant_ndp120_config_clk_div_t;


typedef struct {
    uint32_t set;
    uint32_t gpio_num;
    uint32_t dir;
    uint32_t value;
} syntiant_ndp120_config_gpio_t;

typedef struct syntiant_ndp120_config_sensor_s {
    uint32_t get;
    uint32_t set;

    /* sensor interface GPIO bit roles */
    uint8_t gpio_role[NDP120_FW_GPIO_MAX];
    uint8_t sensor_id[SYNTIANT_CONFIG_SENSOR_SENSORS]; /* sensor id */

    /* sensor interface type */
    uint8_t sensor_interface[SYNTIANT_CONFIG_SENSOR_SENSORS];

    /* sensor i2c address or SPI select gpio # */
    uint8_t sensor_interface_address[SYNTIANT_CONFIG_SENSOR_SENSORS];

    /* true if sensor presents an interrupt input */
    uint8_t sensor_int[SYNTIANT_CONFIG_SENSOR_SENSORS];
    uint8_t sensor_int1[SYNTIANT_CONFIG_SENSOR_SENSORS]; /* second interrupt */

    /* sensor interrupt pin gpio */
    uint8_t sensor_int_gpio[SYNTIANT_CONFIG_SENSOR_SENSORS];
    uint8_t sensor_int_gpio1[SYNTIANT_CONFIG_SENSOR_SENSORS]; /* 2nd GPIO */
    uint8_t sensor_axes[SYNTIANT_CONFIG_SENSOR_SENSORS]; /* sensor axis count */

    /* store sensor axis in holding tank */
    uint8_t sensor_axis_tank[SYNTIANT_CONFIG_SENSOR_SENSORS]
        [SYNTIANT_CONFIG_SENSOR_AXES];

    /* store sensor axis in input vector */
    uint8_t sensor_axis_input[SYNTIANT_CONFIG_SENSOR_SENSORS]
        [SYNTIANT_CONFIG_SENSOR_AXES];
    /* sensor parameters */
    uint8_t sensor_parameter[SYNTIANT_CONFIG_SENSOR_SENSORS]
        [SYNTIANT_CONFIG_SENSOR_PARAMETERS];
} syntiant_ndp120_config_sensor_t;


typedef struct syntiant_ndp120_sensor_status_s {
    uint32_t flag;
    uint32_t sensor_num;  /* 0 ~ 3 */
    uint32_t sensor_type;   /* 1 ~ 9 */
    uint32_t int_count;   /* sensor int count */
    union {
        ndp_sensor_vm3011_state_t sensor_vm3011;
        ndp_sensor_t5838_state_t sensor_t5838;
    } u;
} syntiant_ndp120_sensor_status_t;

/**
 * @brief NDP10x firmware posterior handler set flags
 */
enum syntiant_ndp120_posterior_config_set_e {
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_STATES = 0x0001,
    /**< set the number of states */
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_CLASSES = 0x0002,
    /**< set the number of classes */
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_SM_QUEUE_SIZE = 0x0004,
    /**< set the number of frames to smooth probabilities over */
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_TIMEOUT = 0x0008,
    /**< set the state timeout */
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_TIMEOUT_ACTION = 0x0010,
    /**< set the state timeout action */
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_THRESHOLD = 0x0020,
    /**< set the class active threshold */
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_WINDOW = 0x0040,
    /**< set the class match window */
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_BACKOFF = 0x0080,
    /**< set the class match backoff timer */
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ENABLE = 0x0100,
    /**< set whether the posterior is enabled or not **/
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ACTION = 0x0200,
    /**< set the class action*/
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_PH_TYPE = 0x0400,
    /**< set the posterior handler type */
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_NUM_PH = 0x0800,
    /**< set the number of posterior handlers */
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ADAPTIVE_THRESHOLD = 0x1000,
    /**< set the class scp adaptive threshold */
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ADAPTIVE_FRAMES = 0x2000,
    /**< set adaptive frames */
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ADAPTIVE_DENOMIATOR = 0x4000,
    /**< set adaptive denominator */
    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ADAPTIVE_MAX_UPDATES = 0x8000,
    /**< set adaptive max updates */

    SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ALL_M
    = ((uint32_t)(SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_ADAPTIVE_MAX_UPDATES << 1) - 1)
};

enum syntiant_ndp120_config_tank_input_e {
    SYNTIANT_NDP120_CONFIG_SET_MISC_TANK_INPUT_NONE = 0
    /**< Tank input disabled */
};

/**
 * @brief NDP120 status and debugging set flags
 */
enum syntiant_ndp120_status_set_e {
    SYNTIANT_NDP120_STATUS_SET_CLEAR = 0x01,
    /**< clear status state (counters, etc.) */
    SYNTIANT_NDP120_STATUS_SET_MAILBOX_TRACE = 0x02,
    /**< set or clear mailbox tracing */
    SYNTIANT_NDP120_STATUS_SET_ALL_M
    = ((uint32_t) (SYNTIANT_NDP120_STATUS_SET_MAILBOX_TRACE << 1) - 1)
};

/**
 * @brief NDP120 status and debug mailbox directions
 */
enum syntiant_ndp120_status_mailbox_direction_e {
    SYNTIANT_NDP120_STATUS_MAILBOX_DIRECTION_HOST_TO_MCU = 0,
    SYNTIANT_NDP120_STATUS_MAILBOX_DIRECTION_MCU_TO_HOST = 1,
    SYNTIANT_NDP120_STATUS_MAILBOX_DIRECTION_MAX
        = SYNTIANT_NDP120_STATUS_MAILBOX_DIRECTION_MCU_TO_HOST
};

/**
 * @brief NDP120 status and debugging information
 */
struct syntiant_ndp120_status_s {
    unsigned int set;               /**< set flags */
    int mailbox_trace;              /**< enable mailbox tracing */
    uint32_t h2m_mailbox_req;       /**< host to mcu mailbox requests */
    uint32_t h2m_mailbox_rsp;       /**< host to mcu mailbox responses */
    uint32_t h2m_mailbox_unexpected;
    /**< host to mcu mailbox unexpected messages */
    uint32_t h2m_mailbox_error;
    /**< host to mcu mailbox protocol errors */
    uint32_t m2h_mailbox_req;       /**< mcu to host mailbox requests */
    uint32_t m2h_mailbox_rsp;       /**< mcu to host mailbox responses */
    uint32_t m2h_mailbox_unexpected;
    /**< mcu to host mcu mailbox unexpected messages */
    uint32_t m2h_mailbox_error;
    /**< mcu to host mcu mailbox protocol errors */
    uint32_t missed_frames;         /**< missed MATCH reports */
};

/**
 * @brief NDP120 metadata set
 */
enum syntiant_ndp120_nn_metadata_set_e {
    SYNTIANT_NDP120_SET_META_HEADER = 0x0001,
    SYNTIANT_NDP120_SET_LAYER_PER_NN = 0x0002,
    SYNTIANT_NDP120_SET_IS_NN_CACHED = 0x0004,
    SYNTIANT_NDP120_SET_INPUT_ISA_IDX = 0x0008,
    SYNTIANT_NDP120_SET_OUTPUT_ISA_IDX = 0x0010,
    SYNTIANT_NDP120_SET_INPUT_LAYER_TYPE = 0x0020,
    SYNTIANT_NDP120_SET_INPUT_SIZE = 0x0040,
    SYNTIANT_NDP120_SET_INPUT_COORD = 0x0080,
    SYNTIANT_NDP120_SET_OUTPUT_COORD = 0x0100,
    SYNTIANT_NDP120_SET_CACHE_INST = 0x0200,
    SYNTIANT_NDP120_SET_META_BASE = 0x0400,
    SYNTIANT_NDP120_SET_LAYER_INDICES = 0x800,
    SYNTIANT_NDP120_SET_INPUT_INDICES = 0x1000,
    SYNTIANT_NDP120_SET_OUTPUT_INDICES = 0x2000
};

typedef struct {
    uint8_t version;  /**< version for metadata */
    uint8_t nn_num;   /**< number of neural networks */
    uint8_t dnn_mode; /**< mode for each dnn network */
    uint8_t padding;  /**< padding */
} syntiant_ndp120_nn_metadata_header_t;

/**
 * @brief NDP120 neural network metadata information
 */
typedef struct {
    unsigned int set;                  /**< set flags */
    uint32_t nn_idx;
    /**< the index of network for which the following would be configured */
    uint32_t layer_idx;
    /**< the index of the layer in the network for which input, output and cache
         instructions need to be written. */
    syntiant_ndp120_nn_metadata_header_t header; /**< header for metadata*/
    uint32_t layers_per_nn;             /**< number rof layers per network */
    uint32_t is_nn_cached;              /**< is the network cached */
    uint32_t nn_input_isa_idx;          /**< input layer instruction index */
    uint32_t nn_output_isa_idx;         /**< output layer instruction index */
    uint32_t nn_input_layer_type;       /**< the type of input layer */
    uint32_t nn_input_layer_size[3];    /**< input layer size x, y, z */
    uint32_t input_coord;               /**< input coordinates of a layer */
    uint32_t output_coord;              /**< output coordinates of a layer */

    struct {
        uint8_t num_layers;
        uint8_t num_inputs;
        uint8_t num_outputs;
        uint8_t is_cached;
    } meta_base;

    uint8_t layer_indices[NDP120_DNN_MAX_NUM_LAYERS];
    uint8_t input_indices[NDP120_DNN_MAX_NUM_INPUTS];
    uint8_t output_indices[NDP120_DNN_MAX_NUM_OUTPUTS];
    struct {
        uint32_t input_base_coord_max;
        uint32_t output_base_coord_max;
        uint32_t input_base_coord_add;
        uint16_t input_offset_add;
        uint16_t input_offset_max;
        uint16_t output_base_coord_add;
        uint16_t output_base_coord_stride;
    } cache_inst;                       /**< cache instruction params */
} syntiant_ndp120_nn_metadata_t;

/**
 * @brief NDP120 MCU orchestrator set enum
 */
enum syntiant_ndp120_mcu_orch_set_e {
    SYNTIANT_NDP120_SET_NUM_NODES = 0x0001,
    SYNTIANT_NDP120_SET_NODE = 0x0002,
    SYNTIANT_NDP120_SET_FLOWMAP = 0x0004,
    SYNTIANT_NDP120_SET_ENABLE_CASCADE = 0x0008
};

/**
 * @brief NDP120 neural network metadata information
 */
typedef struct {
    uint32_t set;
    uint32_t node_idx;
    uint8_t num_nodes;
    uint8_t id;
    uint8_t flow_id;
    uint8_t status;
    uint8_t type;
    uint8_t action;
    uint8_t num_inputs;
    uint8_t num_outputs;
    uint8_t input_edges[4];
    uint8_t next_ids[4];
    uint8_t flowmap;
    uint8_t preroll;
    uint16_t orch_timeout;
    uint8_t enable_cascade;
    uint8_t src_buffer;
    uint8_t rsvd[2];
} syntiant_ndp120_mcu_orch_t;

enum pll_presets_voltage_e {
    PLL_PRESET_OP_VOLTAGE_0p9,
    PLL_PRESET_OP_VOLTAGE_1p0,
    PLL_PRESET_OP_VOLTAGE_1p1
};

enum pll_preset_value_constants_e {
    PLL_PRESET_VALUE_LEN = 21
};

typedef struct {
    uint16_t offset;
    uint16_t value;
} ndp120_pll_preset_value_t;

typedef struct {
    const char *name;
    int operating_voltage;
    uint32_t input_freq;
    uint32_t output_freq;
    ndp120_pll_preset_value_t *values;
} ndp120_pll_preset_t;

typedef struct {
    const char *name;
    int operating_voltage;
    uint32_t input_freq;
    uint32_t output_freq;
    uint32_t pdm_freq;
} ndp120_fll_preset_t;

typedef struct {
    uint32_t heap_total;
    uint32_t heap_available;
    uint32_t dnn_available_mem;
} ndp120_dsp_mem_state_t;

typedef struct {
    uint32_t heap_total;
    uint32_t heap_available;
} ndp120_mcu_mem_state_t;

typedef struct {
    ndp120_dsp_mem_state_t dsp_state;
    ndp120_mcu_mem_state_t mcu_state;
} ndp120_dsp_mcu_mem_state_t;

extern const unsigned ndp120_pll_presets_elements;
extern const unsigned ndp120_fll_presets_elements;

extern const unsigned ndp200_pll_presets_elements;
extern const unsigned ndp200_fll_presets_elements;

/**
 * @brief retrieve NDP120 status and debugging information
 *
 * Configuration variables with the corresponding
 * @c SYNTIANT_NDP120_STATUS_SET_* bit set in @c debug->set will
 * be updated.  After performing any requested updates, the current state
 * will be returned in @c status.
 *
 * @param ndp NDP state object
 * @param status status data object
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_status(struct syntiant_ndp_device_s *ndp,
                                  struct syntiant_ndp120_status_s *status);

/**
 * @brief init NDP120 dsp pcm related ring buffers
 *
 * Initialize pcm sample, function buffer and annotation buffer
 * to zero if reset flag set to 1 or update these buffer pointers from dsp fw base.
 * The read status of buffer pointers will be returned.
 *
 * @param ndp NDP state object
 * @param reset Flag to reset the buffers to zero
  @return a @c SYNTIANT_NDP_ERROR_ code
 */
int
syntiant_ndp120_init_ring_buffer_pointers(struct syntiant_ndp_device_s *ndp, int reset);

/**
 * @brief NDP120 read block of data.
 *
 * Reads the count number of bytes of data from the address specified by address field.
 *
 * @param ndp NDP state object
 * @param mcu indicates whether we are reading  mcu address or not.
 * @param address indicates the location from where the data is to be read.
 * @param value gets updated with the data.
 * @param count indicates the length of data.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int
syntiant_ndp120_read_block(struct syntiant_ndp_device_s *ndp, int mcu,
    uint32_t address, void *value, unsigned int count);

/**
 * @brief NDP120 write block of data.
 *
 * Writes the count number of bytes of data from the param value into the address specified by address field.
 *
 * @param ndp NDP state object
 * @param mcu indicates whether we are writing into mcu address or not.
 * @param address indicates the location to where the data is to be written.
 * @param value indicates the data.
 * @param count indicates the length of data.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int
syntiant_ndp120_write_block(struct syntiant_ndp_device_s *ndp, int mcu,
    uint32_t address, const void *value, unsigned int count);

/**
 * @brief NDP120 write data.
 *
 * Writes the data from the param value into the address specified by address field.
 *
 * @param ndp NDP state object
 * @param mcu indicates whether we are writing into mcu address or not.
 * @param address indicates the location to where the data is to be written.
 * @param value indicates the data.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int
syntiant_ndp120_write(struct syntiant_ndp_device_s *ndp, int mcu,
    uint32_t address, uint32_t value);

/*
 * @brief NDP120 MCU and DSP memory states
 *
 * Used to get the available heap memories of MCU and DSP
 *
 * @param ndp NDP state object
 * @param mcu_dsp_mem_info - gets updated with the current values of
 * dsp and mcu firmware
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int
syntiant_ndp120_get_memory_status(struct syntiant_ndp_device_s *ndp,
    ndp120_dsp_mcu_mem_state_t *mcu_dsp_mem_info);

/**
 * @brief NDP120 read data.
 *
 * Reads data from the address specified by address field.
 *
 * @param ndp NDP state object
 * @param mcu indicates whether we are reading  mcu address or not.
 * @param address indicates the location from where the data is to be read.
 * @param value gets updated with the data.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_read(struct syntiant_ndp_device_s *ndp, int mcu,
    uint32_t address, void *value);


/**
 * @brief NDP120 finalize parsing action
 *
 * Taking care of any remaining parsing actions before checking
 * checksum.
 *
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_finalize_parse_actions(void);

/**
 * @brief NDP120 halt mcu.
 *
 * Halts M0
 *
 * @param ndp NDP state object
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_halt_mcu(struct syntiant_ndp_device_s *ndp);

/**
 * @brief NDP120 mailbox mbin send.
 *
 * Sends mailbox from SPI to MCU by writing into register NDP120_SPI_MBIN.
 *
 * @param ndp NDP state object
 * @param data indicates data to be written into register NDP120_SPI_MBIN.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_mbin_send(struct syntiant_ndp_device_s *ndp, uint8_t data);

/**
 * @brief NDP120 mailbox mbin response get.
 *
 * Reads mailbox mbin response from mcu by reading the register NDP120_SPI_MBIN_RESP.
 *
 * @param ndp NDP state object
 * @param data indicates response written by mcu.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_mbin_resp_get(
    struct syntiant_ndp_device_s *ndp, uint8_t *data);

/**
 * @brief NDP120 mailbox request.
 *
 * Send mailbox request from the host to mcu or dsp. And updates resp param with mbin_resp
 * or watermarkint from structure syntiant_ndp120_mb_state_s depending on if its mcu or not.
 *
 * @param ndp NDP state object
 * @param req represents enum values indicating different requests of mcu and dsp.
 * @param resp gets updated with response if address is passed.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_do_mailbox_req(
    struct syntiant_ndp_device_s *ndp, uint8_t req, uint32_t *resp);

/**
 * @brief PDM clock execution mode
 */
enum syntiant_ndp120_pdm_clk_exe_mode_e {
    SYNTIANT_NDP120_SEC_PDM_CLK_START_CLEAN  = 0x01,
     /**< Start clock. Clean start*/
    SYNTIANT_NDP120_SEC_PDM_CLK_START_RESUME = 0x02,
     /**< Resume clock */
    SYNTIANT_NDP120_SEC_PDM_CLK_START_PAUSE  = 0x03
    /**< Pause clock */
};

/**
 * @brief secure input config mode
 */
enum syntiant_ndp120_input_config_mode_e {
    SYNTIANT_NDP120_GET_INPUT_CONFIG = 0,
    SYNTIANT_NDP120_INPUT_CONFIG_PDM = 1,
    SYNTIANT_NDP120_INPUT_CONFIG_SPI = 16
};

#define SYNTIANT_NDP120_SEC_DSP_RESTART           (NDP120_DSP_MB_H2D_RESTART)

#define NDP120_RAM_SECURE_CMD_ADDR                (MCU_OPEN_RAM_WINDOW_LOWER)
#define SYNTIANT_NDP120_SEC_MAIN_CLK_CONFIG       (NDP_MBIN_REQUEST_SECURE_MAIN_CLK)
#define SYNTIANT_NDP120_SEC_PDM_CLK_START         (NDP_MBIN_REQUEST_SECURE_START)
#define SYNTIANT_NDP120_SEC_PDM_CLK_STOP          (NDP_MBIN_REQUEST_SECURE_STOP)
#define SYNTIANT_NDP120_SEC_GET_INFO              (NDP_MBIN_REQUEST_SECURE_GET_INFO)
#define SYNTIANT_NDP120_SEC_GET_NN_GRAPH          (NDP_MBIN_REQUEST_SECURE_GET_NN_GRAPH)
#define SYNTIANT_NDP120_SEC_GET_DEBUG             (NDP_MBIN_REQUEST_SECURE_GET_DEBUG)
#define SYNTIANT_NDP120_SEC_GET_DSP_FLOW          (NDP_MBIN_REQUEST_SECURE_GET_DSP_FLOW)
#define SYNTIANT_NDP120_SEC_GET_AUDIO_CHUNK_SIZE  (NDP_MBIN_REQUEST_SECURE_GET_AUDIO_CHUNK_SIZE)
#define SYNTIANT_NDP120_SEC_INPUT_CONFIG          (NDP_MBIN_REQUEST_SECURE_INPUT_CONFIG)
#define SYNTIANT_NDP120_SEC_SPI_DIRECT            (NDP_MBIN_REQUEST_SECURE_SPI_DIRECT)
#define SYNTIANT_NDP120_SEC_SWITCH_FLOW_SET_ID    (NDP_MBIN_REQUEST_SECURE_SWITCH_FLOW_SET)
#define SYNTIANT_NDP120_SEC_GET_SNSR_SMP_SIZE     (NDP_MBIN_REQUEST_SECURE_GET_SENSOR_SAMPLE_SIZE)
#define SYNTIANT_NDP120_SEC_CFG_MISC_INTERRUPTS   (NDP_MBIN_REQUEST_SECURE_CONFIG_MISC_INTERRUPTS)
#define SYNTIANT_NDP120_SEC_POSTERIOR_CONFIG      (NDP_MBIN_REQUEST_SECURE_POSTERIOR_CONFIG)
#define SYNTIANT_NDP120_SEC_ALGO_CONFIG_AREA      (NDP_MBIN_REQUEST_SECURE_ALGO_CONFIG_AREA)
#define SYNTIANT_NDP120_SEC_INIT_EXTRACTION       (NDP_MBIN_REQUEST_SECURE_INIT_EXTRACTION)
#define SYNTIANT_NDP120_SEC_INIT_RING_BUF_PTRS    (NDP_MBIN_REQUEST_SECURE_INIT_RING_BUF_PTRS)
#define SYNTIANT_NDP120_SEC_POSTERIOR_HANDLER     (NDP_MBIN_REQUEST_SECURE_POSTERIOR_HANDLER)

/**
 * @brief NDP120 set the PDM clock in secure execution mode
 *
 * Start, Stop, or Resume the PDM clock execution.
 *
 * @param ndp NDP state object
 * @param execution_mode - syntiant_ndp120_pdm_clk_exe_mode_e
 *          Indicates whether this is to start, stop, or resume.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_pdm_clock_exe_mode(
    struct syntiant_ndp_device_s *ndp,
    enum syntiant_ndp120_pdm_clk_exe_mode_e execution_mode);

struct syntiant_ndp120_secure_clk_config_data {
    uint32_t src;       /**< enum indicating ext, fll, or pll */
    uint32_t ref_freq;  /**< input frequency, valid only for fll or pll */
    uint32_t ref_type;  /**< clkpad or xtalin, valid only for fll or pll */
    uint32_t core_freq; /**< core clock frequency */
    uint32_t voltage;   /**< core voltage */
    ndp120_pll_preset_value_t preset_value[PLL_PRESET_VALUE_LEN]; /**< pll only */
};

/**
 * @brief NDP120 configure clock in secure execution mode.
 *
 * Start, Stop, or Resume the PDM clock execution.
 *
 * @param ndp NDP state object
 * @param data for secure clock configuration
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_secure_clock_cfg(struct syntiant_ndp_device_s *ndp,
    const struct syntiant_ndp120_secure_clk_config_data* cfg);

/**
 * @brief NDP120 miscellaneous configurations.
 *
 * Set the configurations like audio_frame_size , audio_frame_step etc and update their
 * values in structure syntiant_ndp120_config_misc_t.
 *
 * @param ndp NDP state object
 * @param config structure gets its variables updated, see \ref config_api
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_misc(struct syntiant_ndp_device_s *ndp,
            syntiant_ndp120_config_misc_t *config);

/**
 * @brief NDP120 get sensor status.
 *
 * Get sensor status like interrupt count, etc
 *
 * @param ndp NDP state object
 * @param sensor status struct
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_get_sensor_status(struct syntiant_ndp_device_s *ndp,
       syntiant_ndp120_sensor_status_t *sensor_status);
/**
 * @brief NDP120 debug extract data.
 *
 * Used to extract the debug information about mcu firmware state, posterior handling state,
 * dsp firmware state etc depending on enum value indicated by param type.
 *
 * @param ndp NDP state object
 * @param type indicates the type of information to extract.
 * @param data gets updated with the data.
 * @param len indicates the length of the data.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_debug_extract(struct syntiant_ndp_device_s *ndp, int type,
        void *data, unsigned int *len);

/**
 * @brief NDP120 get last neural network id.
 *
 * Used to get the id of the last network ran.
 *
 * @param ndp NDP state object
 * @return the id  of the last neural network ran.
 */
uint32_t syntiant_get_last_network_id(struct syntiant_ndp_device_s *ndp);

/**
 * @brief NDP120 test function.
 *
 * Used to verify the cyclic redundancy check(crc) and validity of scratch.
 *
 * @param ndp NDP state object
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_test_function(struct syntiant_ndp_device_s *ndp);

int syntiant_ndp120_is_a0(struct syntiant_ndp_device_s *ndp);

/**
 * @brief Set main clock in DSP FW state
 *
 * This writes the configured effective clock rate of the
 * main clock to the DSP FW state, so that it can accurately
 * use internal timers
 *
 * @param ndp NDP state object
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_update_clock_rate_in_dsp(struct syntiant_ndp_device_s *ndp);

/**
 * @brief Set DSP heartbeat interval
 *
 * Set the heartbeat interval in milliseconds. This will cause the
 * DSP to send a "heartbeat" message to the MCU.
 *
 * Set to `0` to disable.
 *
 * @param ndp NDP state object
 * @param interval_ms milliseconds between heartbeats, `0` to disable
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_set_heartbeat_interval_ms(struct syntiant_ndp_device_s *ndp, uint32_t interval_ms);

/* config related */

/**
 * @brief NDP120 get secure boot status
 *
 * Return non-zero if status is secured boot mode.
 *
 * @param ndp NDP state object
 * @param is_secured returns non-zero if boot configuration is secured
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int
syntiant_ndp120_get_secured_status(struct syntiant_ndp_device_s *ndp,
    int *is_secured);

/**
 * @brief NDP120 configure notification on sample ready
 *
 * Send notification for every sample to the host.
 *
 * @param ndp NDP state object
 * @param enable Turn ON or OFF the notification
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_notify_on_sample_ready(struct syntiant_ndp_device_s *ndp, uint32_t enable);

/**
 * @brief NDP120 Get sample ready interrupt state
 *
 * Determine if sample ready interrupt has been turned on
 *
 * @param ndp NDP state object
 * @param enable Turn ON or OFF the notification
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_get_sample_ready(struct syntiant_ndp_device_s *ndp,
        uint32_t *enable);
/**
 * @brief NDP120 configure PDM params
 *
 * Configure PDM params like sample rate, pdm rate, pdm clk mode etc.
 *
 * @param ndp NDP state object
 * @param config contains configuration for setting pdm params, see \ref config_api
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_pdm(struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_pdm_t *config);

/**
 * @brief NDP120 configure I2S
 *
 * Configure I2S mode, freq, framesize e.t.c
 *
 * @param ndp NDP state object
 * @param config contains configuration for setting I2S params, see \ref config_api
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_i2s(struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_i2s_t *config);

/**
 * @brief NDP120 configure mic settings (sensitivity)
 *
 * Configures the mic sensitivity for specified mic
 *
 * @param ndp NDP state object
 * @param config contains configuration for setting mic settings (sensitivity)
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_mic(struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_mic_t *config);

/**
 * @brief NDP120 configure decimation
 *
 * Configures the decimation factor for pcm
 *
 * @param ndp NDP state object
 * @param config contains configuration for setting decimation params, see \ref config_api
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_decimation(struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_decimation_t *config);

/**
 * @brief NDP120 configure gain
 *
 * Configures the gain factor for pcm
 *
 * @param ndp NDP state object
 * @param config contains configuration for setting gain params, see \ref config_api
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_gain(struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_gain_t *config);

/**
 * @brief NDP120 configure farrow
 *
 * Configures the farrow factor for pcm
 *
 * @param ndp NDP state object
 * @param config contains configuration for setting farrow params, see \ref config_api
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_farrow(struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_farrow_t *config);

/**
 * @brief NDP120 configure butterworth filter
 *
 * Configures the butterworth filter coefficients for pcm
 *
 * @param ndp NDP state object
 * @param config contains configuration for setting filter coefficients, see \ref config_api
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_bwcoef(struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_bwcoef_t *config);

/**
 * @brief NDP120 configure clock source
 *
 * Configures the clock source of the device
 *
 * @param ndp NDP state object
 * @param config contains configuration for setting src clk params, see \ref config_api
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_clk_src(struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_clk_src_t *config);

/**
 * @brief NDP120 configure crystal clock
 *
 * Configures the crystal clock frequency of the device
 *
 * @param ndp NDP state object
 * @param config contains configuration for setting crystal clk params, see \ref config_api
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_clk_xtal(struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_clk_xtal_t *config);

/**
 * @brief NDP120 configure fll clock
 *
 * Configures the fll clock frequency of the device
 *
 * @param ndp NDP state object
 * @param config contains configuration for setting fll clk params, see \ref config_api
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_clk_fll(struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_clk_fll_t *config);

/**
 * @brief NDP120 configure pll clock params
 *
 * Configures the pll clock parameters of the device
 *
 * @param ndp NDP state object
 * @param config contains configuration for setting pll clk params, see \ref config_api
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_clk_pll( struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_clk_pll_t *config);

/**
 * @brief NDP120 clock divider configuration
 *
 * Clock divider configuration of the mcu
 *
 * @param ndp NDP state object
 * @param config contains configuration for setting clk divider params, see \ref config_api
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_clk_div( struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_clk_div_t *config);

/**
 * @brief NDP120 configure gpio
 *
 * Configures the gpio's of the device
 *
 * @param ndp NDP state object
 * @param config contains configuration for setting gpio params, see \ref config_api
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_gpio(struct syntiant_ndp_device_s *ndp,
    const syntiant_ndp120_config_gpio_t *config);

int syntiant_ndp120_get_put_main_clk_src(struct syntiant_ndp_device_s *ndp, uint32_t *src);

/**
 * @brief NDP120 configure the external clock frequency
 *
 * Reads and then writes the external clock frequency of the device
 *
 * @param ndp NDP state object
 * @param ext_freq contains configuration for setting ext clk freq params
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_get_put_ext_clk_freq(struct syntiant_ndp_device_s *ndp, uint32_t *ext_freq);

/**
 * @brief NDP120 configure main clock frequency
 *
 * Configures the clock frequencies of ext clk freq or fll clk freq or pll clk freq
 *
 * @param ndp NDP state object
 * @param freq contains configuration for setting main clk params
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_get_main_clk_freq(struct syntiant_ndp_device_s *ndp, uint32_t *freq);

/**
 * @brief NDP120 configure pll clk freq
 *
 * Configures the pll clock frequency of the device
 *
 * @param ndp NDP state object
 * @param freq contains configuration for setting pll params
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_get_put_pll_clk_freq(struct syntiant_ndp_device_s *ndp, uint32_t *freq);

/**
 * @brief NDP120 get dsp fw address
 *
 * Gets the 32bit dsp firmware state address
 *
 * @param ndp NDP state object
 * @return the dsp firmware state address
 */
uint32_t syntiant_ndp120_get_dsp_fw_pointer(struct syntiant_ndp_device_s *ndp);

/**
 * @brief NDP120 get mcu fw address
 *
 * Gets the 32bit mcu firmware state address
 *
 * @param ndp NDP state object
 * @return the mcu firmware state address
 */
uint32_t syntiant_ndp120_get_mcu_fw_pointer(struct syntiant_ndp_device_s *ndp);

/**
 * @brief NDP120 get mcu debug fw address
 *
 * Gets the 32bit mcu debug firmware state address
 *
 * @param ndp NDP state object
 * @return the mcu debug firmware state address
 */
uint32_t syntiant_ndp120_get_mcu_dbg_state_addr(struct syntiant_ndp_device_s *ndp);

/**
 * @brief NDP120 configure nn metadata
 *
 * Configures the NN metadata of the device into the structure
 * member metadata of ndp120_dsp_fw_base_t structure
 *
 * @param ndp NDP state object
 * @param config contains configuration for setting nn metadata params
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */

/**
 * @brief NDP120 configure mcu orchestration
 *
 * Configures the mcu orchestration of the device into the ndp120_nn_graph structure
 *
 * @param ndp NDP state object
 * @param config contains configuration for setting mcu orchestration params
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_mcu_orchestrator(struct syntiant_ndp_device_s *ndp, syntiant_ndp120_mcu_orch_t * config);

/* DSP flow related */

/**
 * @brief NDP120 dsp flow reset.
 *
 * Resets the existing flow rules by memset the structure ndp120_dsp_data_flow_setup_t to 0.
 *
 * @param setup indicates the structure containing all flow rules.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_dsp_flow_setup_reset(ndp120_dsp_data_flow_setup_t *setup);

/**
 * @brief NDP120 set the orchestrator host notification network.
 *
 * Sets the orchestrator to give the host notification from a particular network id
 * specified by nn_number.
 *
 * @param ndp NDP state object
 * @param nn_number indicates the network number from which the host notifications are required.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_orch_host_notification_network(struct syntiant_ndp_device_s *ndp, int *nn_number);

/**
 * @brief NDP120 dsp add new flow rule.
 *
 * Adds the new flow rule specified by the param rule and update that rule in
 * structure ndp120_dsp_data_flow_setup_t specified by param setup.
 *
 * @param setup indicates the local structure containing existing flow rules.
 * @param rule indicates the flow rule to be added.
 * @param src indicates the source type like PCM, FUNC, NN, MCU.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_dsp_flow_setup_add_rule(ndp120_dsp_data_flow_setup_t *setup, const ndp120_dsp_data_flow_rule_t *rule, int src);

/**
 * @brief NDP120 dsp delete flow rule.
 *
 * Deletes the existing flow rule specified by the param rule and delete that rule in
 * structure ndp120_dsp_data_flow_setup_t specified by param setup.
 *
 * @param setup indicates the local structure containing existing flow rules.
 * @param rule indicates the flow rule to be deleted.
 * @param src indicates the source type like PCM, FUNC, NN, MCU.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_dsp_flow_setup_del_rule(ndp120_dsp_data_flow_setup_t *setup, const ndp120_dsp_data_flow_rule_t *rule, int src);

/**
 * @brief NDP120 dsp get flow rules.
 *
 * Get the existing flow rules for the flow set id by the param rule and delete that rule in
 * structure ndp120_dsp_data_flow_setup_t specified by param setup.
 *
 * @param ndp ndp NDP state object.
 * @param flowset_id indicates the flowset id
 * @param filtered_flow_setup indicates the flow setup rules for the given flowset_id.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_dsp_flow_setup_get_rules(struct syntiant_ndp_device_s *ndp, unsigned int flowset_id, ndp120_dsp_data_flow_setup_t *filtered_flow_setup);

/**
 * @brief NDP120 dsp flow rules apply.
 *
 * Write the flow rules that are stored in the local structure setup
 * to the structure ndp120_dsp_fw_base_t present in the dsp firmware struture.
 *
 * @param ndp NDP state object.
 * @param ndp setup indicates the structure containing existing flow rules.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_dsp_flow_setup_apply(struct syntiant_ndp_device_s *ndp, ndp120_dsp_data_flow_setup_t *setup);

/**
 * @brief NDP120 dsp flow rule get.
 *
 * Read the flow rules from the structure ndp120_dsp_fw_base_t present in the dsp firmware struture
 * and writes them in the local structure setup.
 *
 * @param ndp NDP state object.
 * @param setup gets updated with existing flow rules present in dsp firmware.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_dsp_flow_setup_get(struct syntiant_ndp_device_s *ndp, ndp120_dsp_data_flow_setup_t *setup);

/**
 * @brief NDP120 dsp get or set flow set id.
 *
 * Used to get or set the active flow set id depending on param setid.
 * If set_id < 0 ,read the current active flow set id.
 * else set the active flow set id to id specified by set_id.
 *
 * @param ndp NDP state object.
 * @param set_id represents the flow set id.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_dsp_flow_get_put_set_id(struct syntiant_ndp_device_s *ndp, int *set_id);

/* only works if SYNTIANT_NDP120_DEBUG is set */
/**
 * @brief NDP120 dsp flow setup dump.
 *
 * Used to print the valid flow rules present in the structure setup if SYNTIANT_NDP120_DEBUG is enabled.
 *
 * @param ndp NDP state object.
 * @param setup indicates the structure containing the existing flow rules.
 */
void syntiant_ndp120_dsp_flow_setup_dump(ndp120_dsp_data_flow_setup_t *setup);

/* NN related */
int syntiant_ndp120_get_matched_network_id(struct syntiant_ndp_device_s *ndp, uint32_t *network);

/**
 * @brief NDP120 get number of networks.
 *
 * Get the number of neural networks present from the dsp firmware structure.
 *
 * @param ndp NDP state object.
 * @param num_networks gets updated with count represent the number of networks present.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_num_networks(struct syntiant_ndp_device_s *ndp, uint8_t *num_networks);

/**
 * @brief NDP120 get neural network graph.
 *
 * Returns the mcu firmware orchestrator graph address.
 *
 * @param ndp NDP state object.
 * @return uint32_t value representing the orchestrator graph address.
 */
uint32_t syntiant_ndp120_get_nn_graph(struct syntiant_ndp_device_s *ndp);

/**
 * @brief NDP120 reads the dnn state.
 *
 * Reads the dnn state of the requested neural network number.
 *
 * @param ndp NDP state object
 * @param nn_id indicates the neural network number.
 * @param dnn_state gets updated with dnn state of the requested network.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_read_dnn_state(struct syntiant_ndp_device_s *ndp,
        uint8_t nn_id, ndp120_dnn_network_state_t *dnn_state);

/**
 * @brief NDP120 get dsp counters.
 *
 * Get current values of dsp fimrware counters.
 *
 * @param ndp NDP state object
 * @param dsp_cnts gets updated with values of counters in dsp firmware.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_get_dsp_counters(struct syntiant_ndp_device_s *ndp,
        ndp120_dsp_counters_t *dsp_cnts);

/**
 * @brief NDP120 get dsp debug flag.
 *
 * Get the value of dsp debug flag.
 *
 * @param ndp NDP state object
 * @param debug_flag indicates the stautus of dsp debug.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_get_dsp_debug_flag(struct syntiant_ndp_device_s *ndp,
        uint32_t *debug_flag);

/**
 * @brief NDP120 set dsp debug flag.
 *
 * Set the value of dsp debug flag by sending MB cmd to DSP.
 *
 * @param ndp NDP state object
 * @param debug_flag indicates the value of dsp debug.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_set_dsp_debug_flag(struct syntiant_ndp_device_s *ndp,
        uint32_t debug_flag);
/**
 * @brief NDP120 get func buffer sample size.
 *
 * Used to get the sample size of func buffer present in dsp firmware.
 *
 * @param ndp NDP state object.
 * @param sample_size_bytes represents the sample size of func buffer.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_get_func_sample_size(struct syntiant_ndp_device_s *ndp, uint32_t *sample_size_bytes);

/**
 * @brief NDP120 get sensor buffer sample size.
 *
 * Used to get the sample size of sensor buffer present in dsp firmware.
 *
 * @param ndp NDP state object.
 * @param sample_size_bytes represents the sample size of sensor buffer.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_get_sensor_sample_size(struct syntiant_ndp_device_s *ndp,
    uint32_t *sensor_sample_size);

/**
 * @brief NDP120 enable disable sensor
 *
 * Enables or disbale the sensor by sending MB msg to the DSP firmware.
 *
 * @param ndp NDP state object.
 * @param sensor_id indicates the id of the sensor.
 * @param enable : 1 for enable, 0 for disable
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_enable_disable_sensor(struct syntiant_ndp_device_s *ndp,
    uint8_t sensor_id, int enable);

/**
 * @brief NDP120 get nn input type
 *
 * Gets input type for all Neural Networks
 *
 * @param ndp NDP state object.
 * @data gets updated with the nn input src type
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_get_nn_input_type(struct syntiant_ndp_device_s *ndp,
        syntiant_ndp120_flow_src_type_t *nn_input_type);

/* ALGO CONFIG */
/**
 * @brief NDP120 write algo configurations.
 *
 * Write the algo configuration specified by param data into the address
 * corresponding to algo config index specified by param index.
 *
 * @param ndp NDP state object.
 * @param index represents the algo config index value.
 * @data represents the data to be written.
 * @size indicates the length of the data.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_write_algo_config(struct syntiant_ndp_device_s *ndp,
    unsigned int index, void *data, unsigned int size);

/**
 * @brief NDP120 read algo configurations.
 *
 * Read the algo configuration into the param data from the address corresponding
 * to algo config index specified by param index.
 *
 * @param ndp NDP state object.
 * @param index represents the algo config index value.
 * @data gets updated with the data to be read.
 * @size indicates the length of the data.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_read_algo_config(struct syntiant_ndp_device_s *ndp,
    unsigned int index, void *data, unsigned int size);

/**
 * @brief NDP120 read or write dnn power configuration.
 *
 * Read or write the dnn power configuration.
 * If write is 1, then write the vaule specified by data into the structure member
 * dnn_params_power_cfg of structure ndp120_dsp_fw_base_t.
 * If write is 0, then read the structure member dnn_params_power_cfg of structure ndp120_dsp_fw_base_t
 * into the param data.
 *
 * @param ndp NDP state object.
 * @data represents the value.
 * @size indicates the length of the data.
 * @write indicates the read or write operation.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_read_write_dnn_power_cfg(struct syntiant_ndp_device_s *ndp,
        uint32_t *data, uint32_t size, int write);
/* AUDIO SYNC */

/**
 * @brief NDP120 write the dsp audio sync configuration.
 *
 * Write the dsp audio sync configurations specified by the param config
 * into the structure member audio_sync_config of dsp firmware base structure ndp120_dsp_fw_state_t.
 *
 * @param ndp NDP state object.
 * @param config represents the audio sync configuration.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_write_audio_sync_config(struct syntiant_ndp_device_s *ndp, ndp120_dsp_audio_sync_config_t *config);

/**
 * @brief NDP120 read the dsp audio sync configuration.
 *
 * Read the dsp audio sync configurations from the structure member audio_sync_config
 * of dsp firmware base structure ndp120_dsp_fw_state_t into param config.
 *
 * @param ndp NDP state object.
 * @param config represents the audio sync configuration.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_read_audio_sync_config(struct syntiant_ndp_device_s *ndp, ndp120_dsp_audio_sync_config_t *config);

/* DSP tanks config */

/**
 * @brief NDP120 write dsp sample buffers configurations.
 *
 * Write the sample buffers configurations like sample size, capacity etc specified by param config
 * into the dsp firmware state structure.
 *
 * @param ndp NDP state object.
 * @param config represents the dsp sample buffer configurations.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_write_sample_config(struct syntiant_ndp_device_s *ndp,
                                        const ndp120_dsp_config_t *config);

/**
 * @brief NDP120 read dsp sample buffers configurations.
 *
 * Read the sample buffers configurations like sample size, capacity etc from the dsp firmware state structure
 * into the param config.
 *
 * @param ndp NDP state object.
 * @param config represents the dsp sample buffer configurations.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_read_sample_config(struct syntiant_ndp_device_s *ndp,
                                       ndp120_dsp_config_t *config);

/**
 * @brief NDP120 read dsp memory state.
 *
 * Read the dsp memory state from the structure member mem_state of dsp firmware base structure.
 *
 * @param ndp NDP state object.
 * @param memory_state represents the dsp memory state configurations.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_read_dsp_tank_memory(struct syntiant_ndp_device_s *ndp,
                                 ndp120_dsp_memory_state_t  *memory_state);

/**
 * @brief NDP120 set or get dsp tank memory configurations.
 *
 * Read or write the dap tank memory configurations specified by structure members
 * ndp120_dsp_config_t, ndp120_dsp_memory_state_t of dsp firmware base structure.
 *
 * @param ndp NDP state object.
 * @param config represents dsp tank memory configurations, see \ref config_api
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_dsp_tank_memory(struct syntiant_ndp_device_s *ndp,
                                       syntiant_ndp120_config_tank_t *config);

/**
 * @brief NDP120 checks the flow rules.
 *
 * Checks the flow rules and configure, based on the input type either SPI or PDM

 * @param ndp NDP state object.
 * @param input_type represents either SPI or PDM
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_flow_rules(struct syntiant_ndp_device_s *ndp,
                            enum syntiant_ndp120_input_config_mode_e input_type);


/* driver functions */

/**
 * @brief NDP120 initialization
 *
 * Initialize NDP120 in requested modes like NO_TOUCH, RESET, RESTART.
 *
 * @param ndp       NDP state object
 * @param init_mode indicating type of the mode.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_init(
    struct syntiant_ndp_device_s *ndp, enum syntiant_ndp_init_mode_e init_mode);

/**
 * @brief NDP120 uninitialization
 *
 * Uninitialize NDP120 in requested modes like NO_TOUCH, RESET, RESTART.
 *
 * @param ndp       NDP state object
 * @param init_mode indicating type of the mode.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_uninit(
    struct syntiant_ndp_device_s *ndp, enum syntiant_ndp_init_mode_e init_mode);

/**
 * @brief Get op_size.
 *
 * Get size depending on mcu. If mcu is 1 then size is 4 else 1.
 *
 * @param ndp NDP state object
 * @param mcu indicates whether it is mcu or not.
 * @param size gets updated based on mcu.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_op_size(
    struct syntiant_ndp_device_s *ndp, int mcu, unsigned int *size);

/**
 * @brief NDP120 interrupts enable
 *
 * Enables the differrent NDP120 interrupts by writing into SPI_INTCTL register.
 *
 * @param ndp NDP state object
 * @param causes indicates the mask of interrupts you want to enable.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_interrupts(struct syntiant_ndp_device_s *ndp, int *causes);

/**
 * @brief NDP120 Poll for interrupts.
 *
 * Check for interrupts triggering and updates notifications param corresponding to interrupt triggerred.
 *
 * @param ndp NDP state object
 * @param notifications indicates bitmask of interrupts triggered.
 * @param clear if clear param is 1, then updates the SPI_INTSTS register.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_poll(
    struct syntiant_ndp_device_s *ndp, uint32_t *notifications, int clear);

/**
 * @brief NDP120 Poll notification.
 *
 * Poll for specific notifications specified by notification_mask.
 *
 * @param ndp NDP state object
 * @param notification_mask spcifies the bitmask of notifications.
 * @param clear if clear param is 1, then updates the SPI_INTSTS register.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_poll_notification(
    struct syntiant_ndp_device_s *ndp, uint32_t notification_mask);

/**
 * @brief NDP120 load packages.
 *
 * Loads the length number of chunk data into device.
 *
 * @param ndp NDP state object
 * @param chunk inicates the particular chunk number of data.
 * @param len indicates the length of the chunk.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_load(struct syntiant_ndp_device_s *ndp, void *chunk, int len);

/**
 * @brief NDP120 get configuration.
 *
 * Gets the coniguration of the device like device id, device family id, firmware version etc.
 *
 * @param ndp NDP state object
 * @param config is structure of these configuration params.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_get_config(
    struct syntiant_ndp_device_s *ndp, struct syntiant_ndp_config_s *config);

/**
 * @brief NDP120 send data.
 *
 * Writes sample data into sample register SPI_SAMPLE.
 *
 * @param ndp NDP state object
 * @param data represents 8 bit of data.
 * @param length indicates the length of data.
 * @param type indicates the type whether it is NDP_SEND_DATA_TYPE_STREAMING or not.
 * @param offset.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_send_data(
    struct syntiant_ndp_device_s *ndp, const uint8_t *data,
    unsigned int len, int type, uint32_t offset);

/**
 * @brief NDP120 extract data.
 *
 * Extract data from the dsp firmware buffers.
 *
 * @param ndp NDP state object
 * @param type indicates data types like audio(input/annotated), feature.
 * @param from indicates from where to extract like FROM_MATCH, FROM_NEWEST, FROM_OLDEST, FROM_UNREAD.
 * @param data represents the buffer and gets updated with data.
 * @param lenp indicates the length of data.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_extract_data(
    struct syntiant_ndp_device_s *ndp, int type, int from, uint8_t *data, unsigned int *lenp);


/**
 * @brief NDP120 get match summary.
 *
 * Gets the match summary from mcu firmware state structure or from register NDP120_SPI_MATCHSTS.
 *
 * @param ndp NDP state object
 * @param summary indicates summary of particular label match detection.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_get_match_summary(
    struct syntiant_ndp_device_s *ndp, uint32_t *summary);


/**
 * @brief NDP120 get match binary.
 *
 * Gets the binary match summary from the result structure "ndp120_result_s"
 * in mcu firmware structure "ndp120_fw_state_s".
 *
 * @param ndp NDP state object
 * @param matches gets updated with the summary of the matches.
 * @param len indicates the length of data.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_get_match_binary(
    struct syntiant_ndp_device_s *ndp, uint8_t *matches, unsigned int len);

/**
 * @brief NDP120 get match strength.
 *
 * Get the match strength from the result structure "ndp120_result_s"
 * in mcu firmware structure "ndp120_fw_state_s".
 *
 * @param ndp NDP state object
 * @param strengths gets updated with the strength value read from the mcu fw structure.
 * @param len indicates the length.
 * @param type indicates types of strength like NDP_STRENGTH_RAW, NDP_STRENGTH_SOFTMAX.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_get_match_strength(
    struct syntiant_ndp_device_s *ndp, uint8_t *strengths,
    unsigned int len, int type);

int syntiant_ndp120_get_num_activations(
    struct syntiant_ndp_device_s *ndp, unsigned int *len);

/**
 * @brief NDP120 get Activation type
 *
 * Get the Activation size from the result structure "ndp120_result_s"
 * in mcu firmware structure "ndp120_fw_state_s".
 *
 * @param ndp NDP state object
 * @param act_size is activation size in bytes
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_get_activation_size_bytes(struct syntiant_ndp_device_s *ndp,
    uint8_t *act_size);

/**
 * @brief NDP120 get network match strength.
 *
 * Get the match strength from the result structure "ndp120_result_s"
 * in mcu firmware structure "ndp120_fw_state_s", based on
 * specified network ID
 *
 * @param ndp NDP state object
 * @param strengths gets updated with the strength value read from the mcu fw structure.
 * @param network ID
 * @param len indicates the length.
 * @param type indicates types of strength like NDP_STRENGTH_RAW, NDP_STRENGTH_SOFTMAX.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_get_network_match_strength(
    struct syntiant_ndp_device_s *ndp, uint8_t *strengths, int net_id,
    unsigned int len, int type);


enum syntiant_ndp120_config_other_e {
    SYNTIANT_NDP120_CONFIG_OTHER_PDM,
    SYNTIANT_NDP120_CONFIG_OTHER_CLK_SRC,
    SYNTIANT_NDP120_CONFIG_OTHER_CLK_PLL,
    SYNTIANT_NDP120_CONFIG_OTHER_CLK_FLL,
    SYNTIANT_NDP120_CONFIG_OTHER_DECIMATION,
    SYNTIANT_NDP120_CONFIG_OTHER_GAIN,
    SYNTIANT_NDP120_CONFIG_OTHER_DSP_RESTART,
    SYNTIANT_NDP120_CONFIG_OTHER_CONFIG_I2S,
    SYNTIANT_NDP120_CONFIG_OTHER_CONFIG_AUDIO_SYNC,
    SYNTIANT_NDP120_CONFIG_OTHER_CONFIG_NOTIFY,
    SYNTIANT_NDP120_CONFIG_OTHER_CONFIG_ENABLE_DISABLE_FLOWSET,
    SYNTIANT_NDP120_CONFIG_OTHER_CONFIG_POSTERIOR_HANDLER,
    SYNTIANT_NDP120_CONFIG_OTHER_DSP_ALGO_SET_CONFIG,
    SYNTIANT_NDP120_CONFIG_OTHER_DSP_ALGO_ATTACH_CONFIG,
    SYNTIANT_NDP120_CONFIG_OTHER_CLK_XTAL,
    SYNTIANT_NDP120_CONFIG_OTHER_GET_LAST_NN_ID
};

/* for NDP Linux Driver SYNTIANT_NDP120_CONFIG_OTHER */
typedef struct syntiant_ndp120_config_other_s {
    union syntiant_ndp120_config_other_u {
        syntiant_ndp120_config_pdm_t ndp120_config_pdm;
        syntiant_ndp120_config_clk_src_t ndp120_config_clk_src;
        syntiant_ndp120_config_clk_pll_t ndp120_config_clk_pll;
        syntiant_ndp120_config_clk_fll_t ndp120_config_clk_fll;
        syntiant_ndp120_config_clk_xtal_t ndp120_config_clk_xtal;
        syntiant_ndp120_config_decimation_t ndp120_config_decimation;
        syntiant_ndp120_config_gain_t ndp120_config_gain;
        syntiant_ndp120_config_i2s_t  ndp120_config_i2s;
        syntiant_ndp120_config_audio_sync_t ndp120_config_audio_sync;
        syntiant_ndp120_config_notify_t ndp120_config_notify;
        int ndp120_config_flowset_id;
        syntiant_ndp120_posterior_config_t ndp120_posterior_config;
        int last_network_id;
    } config_other_u;
    struct syntiant_ndp120_dsp_algo_config_t {
        uint32_t idx;
        uint32_t data[64];
        uint32_t dsp_algo_size;
        uint32_t dsp_algo_read;
        ndp120_dsp_algo_t dsp_algo_state_data[NDP120_DSP_ALGO_MAX_COUNT];
    } dsp_algo_config_struct;
    enum syntiant_ndp120_config_other_e config_other_action;
} syntiant_ndp120_config_other_t;

/* for NDP Linux Driver SYNTIANT_NDP120_CONFIG_FLOW */
enum syntiant_ndp120_flow_setup_e {
    SYNTIANT_NDP120_FLOW_SETUP_RESET,
    SYNTIANT_NDP120_FLOW_SETUP_ADD_RULE,
    SYNTIANT_NDP120_FLOW_SETUP_APPLY,
    SYNTIANT_NDP120_FLOW_SETUP_DEL_RULE,
    SYNTIANT_NDP120_FLOW_RULE_GET
};

typedef struct syntiant_ndp120_flow_setup_s {
    enum syntiant_ndp120_flow_setup_e action;
    uint32_t src_type;
    ndp120_dsp_data_flow_rule_t rule;
    ndp120_dsp_data_flow_setup_t flow_setup;
} syntiant_ndp120_flow_setup_t;

enum syntiant_ndp120_flowsets_e {
    SYNTIANT_NDP120_MAX_FLOW_SETS = 6
};

/** @brief Flowset data to manage DSP flows
 */
struct syntiant_ndp120_flowsets {
    uint8_t num_flowsets;
    /**< number of DSP flowsets */
    uint8_t active_flowset_id;
    /**< active flowset id */
    uint8_t flowset_ids[SYNTIANT_NDP120_MAX_FLOW_SETS];
    /**< flowset ids */
};

/* I2C */

/* set bit flags */
enum {
    SYNTIANT_NDP120_CONFIG_SET_I2C_ENABLE = 1,
    SYNTIANT_NDP120_CONFIG_SET_I2C_CLK_RATE = 2
};

typedef struct syntiant_ndp120_config_i2c_s {
    uint32_t get;
    uint32_t set;
    uint32_t enable;
    uint32_t clk_rate;
} syntiant_ndp120_config_i2c_t;

/**
 * @brief NDP120 configure i2c.
 *
 * Get the i2c configurations by reading the registers SYNTIANT_NDP120_CHIP_CONFIG_I2CCFG0,SYNTIANT_SYNTIANT_NDP120_I2C_CONFIG_CR.
 * Set the i2c configurations by writing into the registers SYNTIANT_NDP120_CHIP_CONFIG_I2CCFG0,SYNTIANT_NDP120_I2C_CONFIG_CR.
 *
 * @param ndp NDP state object
 * @param config structure indicates the i2c configurations operations.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_i2c(
    struct syntiant_ndp_device_s *ndp, syntiant_ndp120_config_i2c_t *config);

/**
 * @brief NDP120 Read Write algo bindings.
 *
 * Read or write algo binding configurations specified by the structure member 'algos'
 * of the dsp firmware base structue.
 *
 * @param ndp NDP state object
 * @param data represents algo binding configurations.
 * @param size indicates the length of the data.
 * @param read indicates read or write operation.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_read_write_algo_bindings(struct syntiant_ndp_device_s *ndp,
    void *data, unsigned int size, int read);

struct syntiant_ndp120_secure_info {
    char *fw_version;                           /**< MCU fw version */
    char *dsp_fw_version;                       /**< DSP fw version */
    char *pkg_version;                          /**< pkg version */
    char *labels;                               /**< NN labels */
    uint8_t *pbi;                               /**< PBI version */
    unsigned int fw_version_len;                /**< MCU fw version length */
    unsigned int dsp_fw_version_len;            /**< DSP fw version length */
    unsigned int pkg_version_len;               /**< pkg version length */
    unsigned int labels_len;                    /**< NN labels length */
    unsigned int total_nn;                      /**< Total deployed NNs */
    unsigned int *scale_factor;                 /**< Scale factor of each NN*/
    unsigned int *sensor_info;                  /**< sensor info for
                                                  NDP_FW_SENSOR_MAX sensors */
};

struct syntiant_ndp120_secure_debug {
   struct ndp120_debug_cnt_s *mcu_dbg_cnt;
   ndp120_dsp_counters_t *dsp_cnt;
};

/**
 *  @brief NDP120 Mailbox command for secured device
 *
 *  Send device start, stop, or clock configuration data to MCU firmware via
 *  mailbox event.
 *
 *  @param ndp NDP state object
 *  @param cmd represents the mailbox message to be sent
 *  @param data represents if any additional data needed for the device
 *  @param length represents the length of the payload
 *  @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_send_secure_command(struct syntiant_ndp_device_s *ndp,
        uint8_t cmd, void *data, uint32_t length);


/** @brief Get all the version strings, labels, total NNs from the secured device
 * @param ndp NDP state object
 * @param sec_data represents secured data to be fetched from the device
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_secure_get_info(struct syntiant_ndp_device_s *ndp,
        struct syntiant_ndp120_secure_info *sec_data);

/** @brief Get NN graph from the secured device
 * @param ndp NDP state object
 * @param graph NN graph object
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_secure_get_nn_graph(struct syntiant_ndp_device_s *ndp,
        struct ndp120_nn_graph *graph);

/**
 * @brief Get dsp flow rules from the secured device
 * @param ndp NDP state object
 * @param flow_set gets updated with current flow rules
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_secure_get_dsp_flow(struct syntiant_ndp_device_s *ndp,
        ndp120_dsp_data_flow_setup_t *flow_setup);

/**
 * @brief NDP120 secure get audio chunk size
 *
 * @param ndp NDP state object
 * @param audio_chunk_size caller provided integer pointer for fetching chunk size
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_secure_get_audio_chunk_size(struct syntiant_ndp_device_s *ndp,
        uint32_t *audio_chunk_size);

/**
 * @brief NDP120 secure get debug gets the debug counter values
 *
 * @param ndp NDP state object
 * @param mcu_dsp_dbg_cnts gets updated with the current values in the
 * both mcu and dsp firmware.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_secure_get_debug(struct syntiant_ndp_device_s *ndp,
        struct syntiant_ndp120_secure_debug *mcu_dsp_dbg_cnts);

/**
 * @brief NDP120 secure input config sets the input mode of the dsp.
 *
 * @param ndp NDP state object
 * @param input_mode indicates the input mode like PDM or SPI.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_secure_input_config(struct syntiant_ndp_device_s *ndp,
        uint32_t input_mode);

/** @brief NDP120 spi direct config in secure mode
 * @param ndp NDP state object
 * @param threshod_bytes indicates the threshold bytes for SPI FIFO in dsp.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_secure_spi_direct_config(struct syntiant_ndp_device_s *ndp,
             uint32_t threshold_bytes, uint32_t *fifo_threshold_value);

/** @brief Switch to an active flowset among all the available flowsets
 * @param ndp NDP stateobject
 * @param flow set value
 * @param input mode
 */
int syntiant_ndp120_secure_switch_flowset(struct syntiant_ndp_device_s *ndp,
        uint32_t flow_set_id, uint32_t *input_mode);

/** @brief posterior params config
 * @param ndp NDP stateobject
 * @param struct for specific ph/state/class to configure
 */
int syntiant_ndp120_secure_posterior_config(struct syntiant_ndp_device_s *ndp,
    struct syntiant_ndp120_secure_posterior_config_s *config);

/** @brief custom posterior handler
 * @param ndp NDP stateobject
 * @param struct for arbitrary posterior handler data
 */
int syntiant_ndp120_secure_custom_posterior_handler(
    struct syntiant_ndp_device_s *ndp,
    struct syntiant_posterior_handler_s *cfg);

/** @brief modify algo config area
 * @param ndp NDP stateobject
 * @param struct for modifying algo config area
 */
int syntiant_ndp120_secure_algo_config_area(struct syntiant_ndp_device_s *ndp,
    struct syntiant_algo_area_s *cfg);

/** @brief config tank size
 * @param ndp NDP stateobject
 * @param tank size in msec
 */
int syntiant_ndp120_secure_config_tank_size(struct syntiant_ndp_device_s *ndp,
             uint32_t pcm_tank_size_in_msec);

/** @brief send DSP restart to the device for reinitialization of DSP firmware
 * @param ndp NDP stateobject
 */
int syntiant_ndp120_dsp_restart(struct syntiant_ndp_device_s *ndp);

/** @brief Get DSP flow sets
 * @param ndp NDP state obkect
 * @param flowset Caller supplied flowset object for storing flow rule sets
 */
int syntiant_ndp120_dsp_get_flowsets(struct syntiant_ndp_device_s *ndp,
              struct syntiant_ndp120_flowsets *flowset);

const char *syntiant_ndp120_get_config_devtype(
              uint32_t device_id, enum pbi_version_chip_type_t_ *chip_type);

/** @brief custom posterior handler
 *  @param ndp NDP state object
 *  @param cfg struct for arbitrary posterior handler data
 *  @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_custom_posterior_handler(
    struct syntiant_ndp_device_s *ndp,
    struct syntiant_posterior_handler_s *cfg);

/** @brief Allocate an algo config area in DSP firmware
 *  @param ndp NDP state obkect
 *  @param algo_config_index index of algo config area, [0-3]
 *  @param algo_config_area_size size of the algo config area (must be aligned
 *  to 16bytes)
 */
int syntiant_ndp120_alloc_algo_config_area(struct syntiant_ndp_device_s *ndp,
              uint8_t algo_config_index, uint32_t algo_config_area_size);

/** @brief Get sizes for all the algo config areas
 * @param ndp NDP state obkect
 * @param algo_config_sizes array to hold the algo config area sizes
 * @param num_algo_config_areas number of algo config areas
 */
int syntiant_ndp120_get_algo_config_area_sizes(struct syntiant_ndp_device_s *ndp,
              uint32_t *algo_config_sizes, uint8_t num_algo_config_areas);

/** @brief Erase all the configured algo config areas from device memory
 * @param ndp NDP state object
 */
int syntiant_ndp120_erase_algo_config_areas(struct syntiant_ndp_device_s *ndp);

/** @brief Add multi config params to algo config area
 * @param ndp NDP state object
 * @param rule indicates the flow rule
 * @param input_data array holding the multi config params
 * @param input_count number of multi config params in array parsed
 */
int syntiant_ndp120_add_flow_input(struct syntiant_ndp_device_s *ndp,
    const ndp120_dsp_data_flow_rule_t *rule,
    syntiant_algo_multi_config_params_t *input_data, uint8_t input_count);

/** @brief Set dnn run relay
 * @param ndp NDP state object
 * @param rule indicates the flow rule
 * @param src_type indicates the src type of the flow rule
 * @param delay, NN run will be delayed by the mutliple of FIFO interrupt occurrences
        specified by the delay parameter
 */
int syntiant_ndp120_set_get_dnn_run_delay(struct syntiant_ndp_device_s *ndp,
    const ndp120_dsp_data_flow_rule_t *rule, syntiant_ndp120_flow_src_type_t
    src_type, uint32_t *delay, uint32_t set);

/**
 * @brief NDP120 enable/disable barge-in.
 *
 * Enables/disables the barge-in by sending MB cmd to DSP.
 *
 * @param ndp NDP state object
 * @param enable indicates enable/disable.
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_enable_disable_barge_in(struct syntiant_ndp_device_s *ndp,
        int enable);

/** @brief Get Core2 device type
 * @param ndp NDP state obkect
 */
int syntiant_ndp120_handle_image(struct syntiant_ndp_device_s *ndp);

#ifdef SYNTIANT_NDP120_DEBUG_NO_TOUCH
void syntiant_ndp120_set_fw_bases(struct syntiant_ndp_device_s *ndp,
                        uint32_t mcu_fw_base, uint32_t dsp_fw_base,
                        uint32_t mcu_fw_dbg_state_base);
#endif

/** @brief set/get driver boot from flash flag
 * @param ndp NDP state object
 * @param set true for set, otherwise get
 */
int syntiant_ndp_boot_from_flash_flag(struct syntiant_ndp_device_s *ndp, int set);

/** @brief Boot MCU FW from flash
 * @param ndp NDP state obkect
 */
int syntiant_ndp120_boot_from_flash(struct syntiant_ndp_device_s *ndp);

/** @brief PDM clock start mode
 *  @param ndp NDP state object
 *  @param execution_mode, one of syntiant_ndp120_pdm_clk_exe_mode_e
 */
int syntiant_ndp120_pdm_clock_exe_mode_flashboot(
    struct syntiant_ndp_device_s *ndp,
    enum syntiant_ndp120_pdm_clk_exe_mode_e execution_mode);

/** @brief Clock configuration for boot from flash mode
 *  @param ndp NDP state object
 *  @cfg clock configuration object
 */
int syntiant_ndp120_clock_cfg_flashboot(struct syntiant_ndp_device_s *ndp,
    const struct syntiant_ndp120_secure_clk_config_data* cfg);

/** @brief Read from device scratch area
 *  @param ndp NDP state object
 *  @param in buffer to hold read data
 *  @param length size of the read operation
 */
int syntiant_ndp120_read_scratch(struct syntiant_ndp_device_s *ndp, void* in,
    uint32_t length);

int syntiant_ndp120_secure_get_sensor_sample_size(
    struct syntiant_ndp_device_s *ndp, uint32_t *sensor_sample_size);

int syntiant_ndp120_secure_config_misc_interrupts(
    struct syntiant_ndp_device_s *ndp, uint32_t interrupt, uint32_t enable);

/** @brief Opens up device area second window for host extraction
 *  @param ndp NDP state object
 */
int syntiant_ndp120_secure_init_extraction(struct syntiant_ndp_device_s *ndp);

/**
 * @brief ---Experimental feature---
 * @brief NDP120 configure auto clock scaling
 *
 * @param ndp NDP state object
 * @param enable : 1 for enable, 0 for disable
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_config_auto_clock_scaling(struct syntiant_ndp_device_s *ndp,
    uint32_t enable);

/** @brief Instruct DSP to initialize sensors without
 *         initializing gpios
 *  @param ndp NDP state object
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_init_sensors(struct syntiant_ndp_device_s *ndp);

/**
 * @brief get runtime audio parameters as computed by audio algorithms
 * @param ndp NDP state object
 * @param aparms computed audio parameters
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_get_audio_params(struct syntiant_ndp_device_s *ndp,
                                  struct ndp120_dsp_audio_params_s *aparams);
int syntiant_ndp120_get_extract_sample_size(struct syntiant_ndp_device_s *ndp,
    int spi_speed, uint32_t sample_size, int channels, uint32_t *extract_size);
int syntiant_ndp120_get_active_configurations(struct syntiant_ndp_device_s *ndp,
    int *active_channels);
int syntiant_ndp120_dsp_get_info(struct syntiant_ndp_device_s *ndp);

/**
 * @brief get a posterior handler type for a given network
 * @param ndp NDP state object
 * @param nn_id network id
 * @param ph_type posterior type of a given network
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
int syntiant_ndp120_get_posterior_type(struct syntiant_ndp_device_s *ndp,
                                  uint32_t nn_id, uint32_t *ph_type);

/** @brief Set get input size for func or NN
 * @param ndp NDP state object
 * @param rule indicates the flow rule
 * @param src_type indicates the src type of the flow rule
 * @param input_size size to be configured
 * @param set : 1 for set, 0 for get
 */
int syntiant_ndp120_set_get_input_size(struct syntiant_ndp_device_s *ndp,
    const ndp120_dsp_data_flow_rule_t *rule, syntiant_ndp120_flow_src_type_t
    src_type, uint32_t *input_size, uint32_t set);

/**
 * @brief get the health status of MCU and DSP firmware
 * @param ndp NDP state object
 * @param state indicates the status of firmware
 * @param wait_period indicates the waiting period between successive reads of
 * firmware state counters
 */
int syntiant_ndp120_check_fw(struct syntiant_ndp_device_s *ndp,
    enum syntiant_ndp_fw_state *state, uint32_t wait_period);

/**
 * @brief get the firmware loaded status
 * @param ndp NDP state object
 * @param state indicates the firmware load status
 */
void syntiant_ndp120_get_fw_load_status(struct syntiant_ndp_device_s *ndp,
    enum syntiant_ndp_load_state *state);

#ifdef __cplusplus
}
#endif

#endif
