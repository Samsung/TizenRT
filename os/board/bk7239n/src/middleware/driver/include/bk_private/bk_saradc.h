// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define SARADC_FAILURE                (1)
#define SARADC_SUCCESS                (0)

#define SARADC_DEV_NAME                "saradc"

#define SARADC_CMD_MAGIC              (0xe290000)
enum
{
    SARADC_CMD_SET_MODE = SARADC_CMD_MAGIC + 1,
    SARADC_CMD_SET_CHANNEL,
    SARADC_CMD_SET_SAMPLE_RATE,
    SARADC_CMD_SET_WAITING_TIME,
    SARADC_CMD_SET_VALID_MODE,
    SARADC_CMD_CLEAR_INT,
    SARADC_CMD_SET_CLK_RATE,
    SARADC_CMD_RUN_OR_STOP_ADC,
    SARADC_CMD_SET_CAL_VAL,
    SARADC_CMD_SET_BYPASS_CALIB,
    SARADC_CMD_SET_SAT_CTRL,
    SARADC_CMD_PAUSE,
    SARADC_CMD_RESUME,
};

typedef enum
{
    SARADC_CALIBRATE_EXT_LOW,
    SARADC_CALIBRATE_LOW,
    SARADC_CALIBRATE_HIGH,
    SARADC_CALIBRATE_TEMP_CODE25,
    SARADC_CALIBRATE_TEMP_STEP10,
    SARADC_CALIBRATE_MAX,
} SARADC_MODE;

#define ADC_CONFIG_MODE_SLEEP           (0x00UL)
#define ADC_CONFIG_MODE_STEP            (0x01UL)
#define ADC_CONFIG_MODE_SOFT_CTRL       (0x02UL)
#define ADC_CONFIG_MODE_CONTINUE        (0x03UL)

#define ADC_CONFIG_MODE_4CLK_DELAY      (0x0UL)
#define ADC_CONFIG_MODE_8CLK_DELAY      (0x1UL)
#define ADC_CONFIG_MODE_SHOULD_OFF      (1 << 3)

typedef struct
{
    UINT16 *pData;
    volatile UINT8 current_sample_data_cnt;
    volatile UINT8 current_read_data_cnt;
    UINT8 data_buff_size;
    volatile UINT8 has_data; /* 1: has data      0: no data*/
    volatile UINT8 all_done; /* 1: all done      0: still sampling*/
    UINT8 channel;

    /* mode:     ADC mode
     * bit[0:1]: ADC operation mode
     *          00:  ADC power down mode
     *          01:  ADC one-step mode
     *          10:  ADC software control mode
     *          11:  ADC continuous mode
     * bit[2:2]: delay clk(adc setting)
     *           0: delay 4 clk
     *           1: delay 8 clk
     * bit[7:3]: reserved
     */
    UINT8 mode;
    void (*p_Int_Handler)(void);
    unsigned char pre_div;					// ADC pre-divide clk
    unsigned char samp_rate;				// ADC sample rate
    unsigned char filter;                   //ADC filter
} saradc_desc_t;

typedef struct
{
    UINT8 enable;
    UINT8 channel;
} saradc_chan_t;

typedef struct
{
    unsigned short val;
    SARADC_MODE mode;
} saradc_cal_val_t;

/*******************************************************************************
* Function Declarations
*******************************************************************************/
void saradc_disable(void);
void saradc_init(void);
void saradc_exit(void);
void saradc_isr(void);
void saradc_config_param_init_for_temp(saradc_desc_t * adc_config);
void saradc_config_param_init(saradc_desc_t * adc_config);
void saradc_ensure_close(void);
UINT32 saradc_check_busy(void);
UINT32 saradc_check_accuracy(void);
float saradc_calculate(UINT16 adc_val);
float bk_adc_data_calculate(UINT16 adc_val, UINT8 adc_chan);
UINT32 saradc_set_calibrate_val(uint16_t *value, SARADC_MODE mode);
UINT32 saradc_get_calibrate_val(uint16_t *value, SARADC_MODE mode);
#ifdef __cplusplus
}
#endif
