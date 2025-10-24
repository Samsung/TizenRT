// Copyright 2022-2023 Beken
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

#include <string.h>
#include <driver/es8311.h>
#include <driver/i2c.h>
#include <os/os.h>


#define ES8311_TAG "es8311"
#define ES8311_LOGI(...) BK_LOGI(ES8311_TAG, ##__VA_ARGS__)
#define ES8311_LOGW(...) BK_LOGW(ES8311_TAG, ##__VA_ARGS__)
#define ES8311_LOGE(...) BK_LOGE(ES8311_TAG, ##__VA_ARGS__)
#define ES8311_LOGD(...) BK_LOGD(ES8311_TAG, ##__VA_ARGS__)

/*
 *   ES8311_REGISTER NAME_REG_REGISTER ADDRESS
 */
#define ES8311_RESET_REG00              0x00  /*reset digital,csm,clock manager etc.*/

/*
 * Clock Scheme Register definition
 */
#define ES8311_CLK_MANAGER_REG01        0x01 /* select clk src for mclk, enable clock for codec */
#define ES8311_CLK_MANAGER_REG02        0x02 /* clk divider and clk multiplier */
#define ES8311_CLK_MANAGER_REG03        0x03 /* adc fsmode and osr  */
#define ES8311_CLK_MANAGER_REG04        0x04 /* dac osr */
#define ES8311_CLK_MANAGER_REG05        0x05 /* clk divier for adc and dac */
#define ES8311_CLK_MANAGER_REG06        0x06 /* bclk inverter and divider */
#define ES8311_CLK_MANAGER_REG07        0x07 /* tri-state, lrck divider */
#define ES8311_CLK_MANAGER_REG08        0x08 /* lrck divider */
/*
 * SDP
 */
#define ES8311_SDPIN_REG09              0x09 /* dac serial digital port */
#define ES8311_SDPOUT_REG0A             0x0A /* adc serial digital port */
/*
 * SYSTEM
 */
#define ES8311_SYSTEM_REG0B             0x0B /* system */
#define ES8311_SYSTEM_REG0C             0x0C /* system */
#define ES8311_SYSTEM_REG0D             0x0D /* system, power up/down */
#define ES8311_SYSTEM_REG0E             0x0E /* system, power up/down */
#define ES8311_SYSTEM_REG0F             0x0F /* system, low power */
#define ES8311_SYSTEM_REG10             0x10 /* system */
#define ES8311_SYSTEM_REG11             0x11 /* system */
#define ES8311_SYSTEM_REG12             0x12 /* system, Enable DAC */
#define ES8311_SYSTEM_REG13             0x13 /* system */
#define ES8311_SYSTEM_REG14             0x14 /* system, select DMIC, select analog pga gain */
/*
 * ADC
 */
#define ES8311_ADC_REG15                0x15 /* ADC, adc ramp rate, dmic sense */
#define ES8311_ADC_REG16                0x16 /* ADC */
#define ES8311_ADC_REG17                0x17 /* ADC, volume */
#define ES8311_ADC_REG18                0x18 /* ADC, alc enable and winsize */
#define ES8311_ADC_REG19                0x19 /* ADC, alc maxlevel */
#define ES8311_ADC_REG1A                0x1A /* ADC, alc automute */
#define ES8311_ADC_REG1B                0x1B /* ADC, alc automute, adc hpf s1 */
#define ES8311_ADC_REG1C                0x1C /* ADC, equalizer, hpf s2 */
/*
 * DAC
 */
#define ES8311_DAC_REG31                0x31 /* DAC, mute */
#define ES8311_DAC_REG32                0x32 /* DAC, volume */
#define ES8311_DAC_REG33                0x33 /* DAC, offset */
#define ES8311_DAC_REG34                0x34 /* DAC, drc enable, drc winsize */
#define ES8311_DAC_REG35                0x35 /* DAC, drc maxlevel, minilevel */
#define ES8311_DAC_REG37                0x37 /* DAC, ramprate */
/*
 *GPIO
 */
#define ES8311_GPIO_REG44               0x44 /* GPIO, dac2adc for test */
#define ES8311_GP_REG45                 0x45 /* GP CONTROL */
/*
 * CHIP
 */
#define ES8311_CHD1_REGFD               0xFD /* CHIP ID1 */
#define ES8311_CHD2_REGFE               0xFE /* CHIP ID2 */
#define ES8311_CHVER_REGFF              0xFF /* VERSION */
#define ES8311_CHD1_REGFD               0xFD /* CHIP ID1 */

#define ES8311_MAX_REGISTER             0xFF

/* ES8311 address
 * 0x32:CE=1;0x30:CE=0
 */
#define ES8311_ADDR         0x18 //0x30:CE=0

/*
 * to define the clock soure of MCLK
 */
#define FROM_MCLK_PIN       0
#define FROM_SCLK_PIN       1
#define ES8311_MCLK_SOURCE  FROM_MCLK_PIN

/*
 * to define whether to reverse the clock
 */
#define INVERT_MCLK         0 // do not invert
#define INVERT_SCLK         0

#define IS_DMIC             1 // Is it a digital microphone

#define MCLK_DIV_FRE        256

#define I2C_WRITE_WAIT_MAX_MS    (500)


extern void delay_ms(UINT32 ms_count);

/* codec hifi mclk clock divider coefficients */
static const struct mclk_div_coeff es8311_clk_div_table[] = {
    //mclk samp_rate lrck_div  bclk_div pre_div adc_div dac_div scale adc_osr  dac_osr
    /* 12.288MHz */
    {12288000, 8000 , 1536, 24, 0xA0, 0x00, 0x00, 0x24, 0x10, 0x10},
    {12288000, 12000, 1024, 16, 0x60, 0x00, 0x00, 0x24, 0x10, 0x10},
    {12288000, 16000, 768 , 12, 0x40, 0x00, 0x00, 0x24, 0x10, 0x10},
    {12288000, 24000, 512 , 8 , 0x20, 0x00, 0x00, 0x24, 0x10, 0x10},
    {12288000, 32000, 384 , 6 , 0x48, 0x00, 0x00, 0x24, 0x10, 0x10},
    {12288000, 48000, 256 , 4 , 0x00, 0x00, 0x00, 0x24, 0x10, 0x10},

    /* 11.2896MHz */
    {11289600, 11025, 1024, 16, 0x60, 0x00, 0x00, 0x24, 0x10, 0x10},
    {11289600, 22050, 512 , 8 , 0x20, 0x00, 0x00, 0x24, 0x10, 0x10},
    {11289600, 44100, 256 , 4 , 0x00, 0x00, 0x00, 0x24, 0x10, 0x10},
};

bk_err_t es8311_write_reg(uint8_t reg_addr, uint8_t data)
{
	//return i2c_bus_write_byte(i2c_handle, reg_addr, data);
	uint8_t reg_data = data;
	i2c_mem_param_t mem_param = {0};
	mem_param.dev_addr = ES8311_ADDR;
	mem_param.mem_addr = reg_addr;
	mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
	mem_param.data = (uint8_t *)&reg_data;
	mem_param.data_size = 1;
	mem_param.timeout_ms = I2C_WRITE_WAIT_MAX_MS;
	return bk_i2c_memory_write(I2C_ID_0, &mem_param);
}

int es8311_read_reg(uint8_t reg_addr)
{
	uint8_t data_buf = 0;
	i2c_mem_param_t mem_param = {0};
	mem_param.dev_addr = ES8311_ADDR;
	mem_param.mem_addr = reg_addr;
	mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
	mem_param.data = &data_buf;
	mem_param.data_size = 1;
	mem_param.timeout_ms = I2C_WRITE_WAIT_MAX_MS;
	bk_i2c_memory_read(I2C_ID_0, &mem_param);
	return (int)data_buf;
}

/*
* look for the coefficient in coeff_div[] table
*/
static int get_coeff(uint32_t mclk, uint32_t rate)
{
    for (int i = 0; i < (sizeof(es8311_clk_div_table) / sizeof(es8311_clk_div_table[0])); i++) {
        if (es8311_clk_div_table[i].samp_rate == rate && es8311_clk_div_table[i].mclk == mclk)
            return i;
    }
    return -1;
}

/*
* set es8311 dac mute or not
* if mute = 0, dac un-mute
* if mute = 1, dac mute
*/
static void es8311_mute(int mute)
{
    uint8_t regv;
    regv = es8311_read_reg(ES8311_DAC_REG31) & 0x9f;
    if (mute) {
        es8311_write_reg(ES8311_SYSTEM_REG12, 0x02);
        es8311_write_reg(ES8311_DAC_REG31, regv | 0x60);
        es8311_write_reg(ES8311_DAC_REG32, 0x00);
        es8311_write_reg(ES8311_DAC_REG37, 0x08);
    } else {
        es8311_write_reg(ES8311_DAC_REG31, regv);
        es8311_write_reg(ES8311_SYSTEM_REG12, 0x00);
    }
}

/*
* set es8311 into suspend mode
*/
static void es8311_suspend(void)
{
    ES8311_LOGI("Enter into es8311_suspend \n");
    es8311_write_reg(ES8311_DAC_REG32, 0x00);
    es8311_write_reg(ES8311_ADC_REG17, 0x00);
    es8311_write_reg(ES8311_SYSTEM_REG0E, 0xFF);
    es8311_write_reg(ES8311_SYSTEM_REG12, 0x02);
    es8311_write_reg(ES8311_SYSTEM_REG14, 0x00);
    es8311_write_reg(ES8311_SYSTEM_REG0D, 0xFA);
    es8311_write_reg(ES8311_ADC_REG15, 0x00);
    es8311_write_reg(ES8311_DAC_REG37, 0x08);
    es8311_write_reg(ES8311_GP_REG45, 0x01);
}

bk_err_t es8311_config_i2s_mode(i2s_work_mode_t mode)
{
    bk_err_t ret = BK_OK;
    uint8_t adc_iface = 0;
	uint8_t dac_iface = 0;
    dac_iface = es8311_read_reg(ES8311_SDPIN_REG09);
    adc_iface = es8311_read_reg(ES8311_SDPOUT_REG0A);
    switch (mode) {
        case I2S_WORK_MODE_I2S:
            ES8311_LOGD("ES8311 in I2S Format");
            dac_iface &= 0xFC;
            adc_iface &= 0xFC;
            break;
        case I2S_WORK_MODE_LEFTJUST:
        case I2S_WORK_MODE_RIGHTJUST:
            ES8311_LOGD("ES8311 in LJ Format");
            adc_iface &= 0xFC;
            dac_iface &= 0xFC;
            adc_iface |= 0x01;
            dac_iface |= 0x01;
            break;
        case I2S_WORK_MODE_SHORTFAMSYNC:
            ES8311_LOGD("ES8311 in DSP-B Format");
            adc_iface &= 0xDC;
            dac_iface &= 0xDC;
            adc_iface |= 0x23;
            dac_iface |= 0x23;
            break;
        default:
            dac_iface &= 0xFC;
            adc_iface &= 0xFC;
            break;
    }
    ret |= es8311_write_reg(ES8311_SDPIN_REG09, dac_iface);
    ret |= es8311_write_reg(ES8311_SDPOUT_REG0A, adc_iface);

    return ret;
}

bk_err_t es8311_set_bits_per_sample(i2s_data_width_t bits)
{
    bk_err_t ret = BK_OK;
    uint8_t adc_iface = 0, dac_iface = 0;
    dac_iface = es8311_read_reg(ES8311_SDPIN_REG09);
    adc_iface = es8311_read_reg(ES8311_SDPOUT_REG0A);
    switch (bits) {
        case I2S_DATA_WIDTH_16:
            dac_iface |= 0x0c;
            adc_iface |= 0x0c;
            break;
        case I2S_DATA_WIDTH_24:
            break;
        case I2S_DATA_WIDTH_32:
            dac_iface |= 0x10;
            adc_iface |= 0x10;
            break;
        default:
            dac_iface |= 0x0c;
            adc_iface |= 0x0c;
            break;

    }
    ret |= es8311_write_reg(ES8311_SDPIN_REG09, dac_iface);
    ret |= es8311_write_reg(ES8311_SDPOUT_REG0A, adc_iface);

    return ret;
}

bk_err_t es8311_set_i2s_role(i2s_role_t role)
{
    bk_err_t ret = BK_OK;
    uint8_t regv = 0;
    regv = es8311_read_reg(ES8311_RESET_REG00);
    switch (role) {
        case I2S_ROLE_SLAVE:
            regv &= 0XBF;
            break;
        case I2S_ROLE_MASTER:
			regv |= BIT(6);
            break;
        default:
			regv |= BIT(6);
            break;
    }
    ret |= es8311_write_reg(ES8311_RESET_REG00, regv);

    return ret;
}

bk_err_t es8311_codec_init(void)
{
	i2c_config_t i2c_cfg = {0};
	i2c_cfg.baud_rate = 400000;
	i2c_cfg.addr_mode = I2C_ADDR_MODE_7BIT;
	i2c_cfg.slave_addr = 0X18;

	bk_i2c_init(I2C_ID_0, &i2c_cfg);
	ES8311_LOGI("i2c(%d) init \n", I2C_ID_0);

	/* reset codec */
	es8311_write_reg(0x00, 0x1F);
	es8311_write_reg(0x45, 0x00);

    return BK_OK;
}


bk_err_t es8311_codec_deinit(void)
{

    return BK_OK;
}

bk_err_t es8311_codec_start(es8311_codec_config_t *cfg)
{
	bk_err_t ret= BK_OK;
	uint8_t clk_tb_num = 0;

    uint32_t mclk_val = 0;
    uint32_t samp_rate = 0;

    switch (cfg->i2s_cfg.samp_rate) {
		case ES8311_SAMP_RATE_8K:
			samp_rate = 8000;
			mclk_val = 12288000;
			break;

		case ES8311_SAMP_RATE_12K:
			samp_rate = 12000;
			mclk_val = 12288000;
			break;

		case ES8311_SAMP_RATE_16K:
			samp_rate = 16000;
			mclk_val = 12288000;
			break;

		case ES8311_SAMP_RATE_24K:
			samp_rate = 24000;
			mclk_val = 12288000;
			break;

		case ES8311_SAMP_RATE_32K:
			samp_rate = 32000;
			mclk_val = 12288000;
			break;

		case ES8311_SAMP_RATE_48K:
			samp_rate = 48000;
			mclk_val = 12288000;
			break;


		case ES8311_SAMP_RATE_11_025K:
			samp_rate = 11025;
			mclk_val = 11289600;
			break;

		case ES8311_SAMP_RATE_22_05K:
			samp_rate = 22050;
			mclk_val = 11289600;
			break;

		case ES8311_SAMP_RATE_44_1K:
			samp_rate = 44100;
			mclk_val = 11289600;
			break;

		default:
			return BK_FAIL;
	}

	clk_tb_num = get_coeff(mclk_val, samp_rate);
	ES8311_LOGI("clk_tb_num: %d \n", clk_tb_num);

	/* reset codec */
	ret |= es8311_write_reg(0x00, 0x1F);
	ret |= es8311_write_reg(0x45, 0x00);

	/* set adc/dac clk */
	ret |= es8311_write_reg(0x01, 0x30);
	ret |= es8311_write_reg(0x02, 0x10);
	ret |= es8311_write_reg(0x02, es8311_clk_div_table[clk_tb_num].pre_div);	//set digmclk, clock doubler
	ret |= es8311_write_reg(0x03, es8311_clk_div_table[clk_tb_num].adc_osr);	//ADC FDMODE, ADC_OSR
	ret |= es8311_write_reg(0x16, es8311_clk_div_table[clk_tb_num].scale);		//set ADC_SYNC mode
	ret |= es8311_write_reg(0x04, es8311_clk_div_table[clk_tb_num].dac_osr);	//DAC_OSR
	ret |= es8311_write_reg(0x05, (es8311_clk_div_table[clk_tb_num].adc_div<<4) | (es8311_clk_div_table[clk_tb_num].dac_div));	//set adc_mclk, dac_mclk

	/* set system power up */
	ret |= es8311_write_reg(0x06, es8311_clk_div_table[clk_tb_num].bclk_div - 1);	//power up stage A/B time
	ret |= es8311_write_reg(0x07, (uint8_t)((es8311_clk_div_table[clk_tb_num].bclk_div - 1)>>2));	//power up stage A/B time
	ret |= es8311_write_reg(0x08, (uint8_t)((es8311_clk_div_table[clk_tb_num].bclk_div - 1) | 0XFF));	//power up stage A/B time

	ret |= es8311_write_reg(0x0B, 0x7B);	//power up stage A/B time
	ret |= es8311_write_reg(0x0C, 0x8F);	//power up stage B/C time
	ret |= es8311_write_reg(0x10, 0x03);	//set vmid/ibias
	ret |= es8311_write_reg(0x11, 0x7B);	//set vsel
	ret |= es8311_write_reg(0x00, 0xC1);	//chip powerup slave mode. master mode(0xC0)
	ret |= es8311_set_i2s_role(cfg->i2s_cfg.role);

	rtos_delay_milliseconds(50);
	ret |= es8311_write_reg(0x01, 0x3A);	//power up digital

	/* set adc */
	ret |= es8311_write_reg(0x14, 0x40);	//dmic off, input selection, PGA gain

	/* set dac */
	ret |= es8311_write_reg(0x12, 0x00);

	/* enable HP drive */
	ret |= es8311_write_reg(0x13, 0x00);

	/* set adc/dac data format */
	//ret |= es8311_write_reg(0x0A, 0x0C);	//set adc data format = 24 bit i2s
	//ret |= es8311_write_reg(0x09, 0x00);	//set dac data format = 24 bit i2s
	ret |= es8311_config_i2s_mode(cfg->i2s_cfg.mode);
	ret |= es8311_set_bits_per_sample(cfg->i2s_cfg.bits);

	/* set low or normal power mode */
	ret |= es8311_write_reg(0x0E, 0x0A);	//enable analog pga/adc modulator
	ret |= es8311_write_reg(0x0F, 0xFE);	//normal power mode

	/* set adc */
	ret |= es8311_write_reg(0x15, 0x00);	//set softramp
	ret |= es8311_write_reg(0x1B, 0x0A);	//set adc hpf
	ret |= es8311_write_reg(0x1C, 0x6A);	//set ADC HPF, ADC_EQ bypass
	ret |= es8311_write_reg(0x37, 0x08);	//set adc digtal vol
	ret |= es8311_write_reg(0x17, 0xBF);	//set adc digtal vol

	return ret;
}


bk_err_t es8311_stop(void)
{
    bk_err_t ret = BK_OK;
    es8311_suspend();
    return ret;
}

bk_err_t es8311_codec_set_voice_volume(int volume)
{
    bk_err_t res = BK_OK;
    if (volume < 0) {
        volume = 0;
    } else if (volume > 100) {
        volume = 100;
    }
    int vol = (volume) * 2550 / 1000;
    ES8311_LOGD("SET: volume:%d", vol);
    es8311_write_reg(ES8311_DAC_REG32, vol);
    return res;
}

bk_err_t es8311_codec_get_voice_volume(int *volume)
{
    bk_err_t res = BK_OK;
    int regv = 0;
    regv = es8311_read_reg(ES8311_DAC_REG32);
    if (regv == BK_FAIL) {
        *volume = 0;
        res = BK_FAIL;
    } else {
        *volume = regv * 100 / 256;
    }
    ES8311_LOGD("GET: res:%d, volume:%d", regv, *volume);
    return res;
}

bk_err_t es8311_set_voice_mute(bool enable)
{
    ES8311_LOGD("Es8311SetVoiceMute volume:%d", enable);
    es8311_mute(enable);
    return BK_OK;
}

bk_err_t es8311_get_voice_mute(int *mute)
{
    bk_err_t res = BK_OK;
    uint8_t reg = 0;
    res = es8311_read_reg(ES8311_DAC_REG31);
    if (res != BK_FAIL) {
        reg = (res & 0x20) >> 5;
    }
    *mute = reg;
    return res;
}

bk_err_t es8311_set_mic_gain(es8311_mic_gain_t gain_db)
{
    bk_err_t res = BK_OK;
    res = es8311_write_reg(ES8311_ADC_REG16, gain_db); // MIC gain scale
    return res;
}

void es8311_read_all()
{
    for (int i = 0; i < 0x4A; i++) {//0x4A
        uint8_t reg = es8311_read_reg(i);
        ES8311_LOGI("REG%02x: 0x%02x\n", i, reg);
    }
}


/* official demo */
#if 0

void es8311_codec_init_demo(void)
{
	/* reset codec */
	es8311_write_reg(0x00, 0x1F);
	es8311_write_reg(0x45, 0x00);

	/* set adc/dac clk */
	es8311_write_reg(0x01, 0x30);	//set mclkin, clk_adc, clk_dac
	es8311_write_reg(0x02, 0x10);
	es8311_write_reg(0x02, 0x00);	//set digmclk, clock doubler
	es8311_write_reg(0x03, 0x10);	//ADC FDMODE, ADC_OSR
	es8311_write_reg(0x16, 0x24);	//set ADC_SYNC mode
	es8311_write_reg(0x04, 0x10);	//DAC_OSR
	es8311_write_reg(0x05, 0x00);	//set adc_mclk, dac_mclk

	/* set system power up */
	es8311_write_reg(0x0B, 0x00);	//power up stage A/B time
	es8311_write_reg(0x0C, 0x00);	//power up stage B/C time
	es8311_write_reg(0x10, 0x1F);	//set vmid/ibias
	es8311_write_reg(0x11, 0x7F);	//set vsel
	es8311_write_reg(0x00, 0x80);	//chip powerup slave mode. master mode(0xC0)
	rtos_delay_milliseconds(50);
	es8311_write_reg(0x0D, 0x01);	//power up analog
	es8311_write_reg(0x01, 0x3F);	//power up digital

	/* set adc */
	es8311_write_reg(0x14, 0x14);	//dmic off, input selection, PGA gain

	/* set dac */
	es8311_write_reg(0x12, 0x00);

	/* enable HP drive */
	es8311_write_reg(0x13, 0x10);

	/* set adc/dac data format */
	es8311_write_reg(0x0A, 0x00);	//set adc data format = 24 bit i2s
	es8311_write_reg(0x09, 0x00);	//set dac data format = 24 bit i2s

	/* set low or normal power mode */
	es8311_write_reg(0x0E, 0x02);	//enable analog pga/adc modulator
	es8311_write_reg(0x0F, 0x44);	//normal power mode

	/* set adc */
	es8311_write_reg(0x15, 0x40);	//set softramp
	es8311_write_reg(0x1B, 0x0A);	//set adc hpf
	es8311_write_reg(0x1C, 0x6A);	//set ADC HPF, ADC_EQ bypass
	es8311_write_reg(0x17, 0xBF);	//set adc digtal vol

	/* set dac */
	es8311_write_reg(0x37, 0x48);	//set dac softramp, disable DAC_EQ
	es8311_write_reg(0x32, 0xBF);

	/* set adc/dac data form */
	es8311_write_reg(0x44, 0x08);	//adc to dac disable, ADCDATA=ADC(L)+ADC(R)

	/* only set adc alc function for amic record */
	es8311_write_reg(0x16, 0x22);	//set adc gain scale up
	es8311_write_reg(0x17, 0xCF);	//set adc alc max gain
	es8311_write_reg(0x18, 0x87);	//set adc alc enable,alc_winsize
	es8311_write_reg(0x19, 0xFB);	//set adc alc target level
	es8311_write_reg(0x1A, 0x03);	//set adc_automute noise gate
	es8311_write_reg(0x1B, 0xEA);	//set adc_automute vol

	/* for low power and donot drive HP */
	es8311_write_reg(0x10, 0x0C);	//set vmid/ibias
	es8311_write_reg(0x11, 0x7B);	//set vsel
	es8311_write_reg(0x13, 0x00);	//disable HP drive
	es8311_write_reg(0x0E, 0x0A);	//LP vrefbuf
	es8311_write_reg(0x0F, 0xFF);	//set in LP mode
	es8311_write_reg(0x1B, 0x05);	//set adc hpf
	es8311_write_reg(0x1C, 0x45);	//set adc hpf, ADC_EQ bypass
}

void es8311_adc_init_demo(void)
{
	/* reset codec */
	es8311_write_reg(0x00, 0x1F);
	es8311_write_reg(0x45, 0x00);

	/* set adc/dac clk */
	es8311_write_reg(0x01, 0x30);	//set mclkin, clk_adc, clk_dac
	es8311_write_reg(0x02, 0x10);
	//es8311_write_reg(0x02, 0x48);	//set digmclk, clock doubler   32K
	//es8311_write_reg(0x02, 0x00);	//48K sample rate
	es8311_write_reg(0x02, 0x40);	//16K
	//es8311_write_reg(0x02, 0xA0);	//8K
	es8311_write_reg(0x03, 0x10);	//ADC FDMODE, ADC_OSR
	es8311_write_reg(0x16, 0x24);	//set ADC_SYNC mode
	es8311_write_reg(0x04, 0x10);	//DAC_OSR
	es8311_write_reg(0x05, 0x00);	//set adc_mclk, dac_mclk

	/* set system power up */
	es8311_write_reg(0x0B, 0x7B);	//power up stage A/B time
	es8311_write_reg(0x0C, 0x8F);	//power up stage B/C time
	es8311_write_reg(0x10, 0x03);	//set vmid/ibias
	es8311_write_reg(0x11, 0x7B);	//set vsel
	es8311_write_reg(0x00, 0x81);	//chip powerup, slave mode(0x81)
	//es8311_write_reg(0x00, 0xC1);	//chip powerup, master mode(0xC1)
	rtos_delay_milliseconds(50);
	//es8311_write_reg(0x0D, 0x0A);	//power up analog
	es8311_write_reg(0x0D, 0x09);	//power up analog
	es8311_write_reg(0x01, 0x3A);	//power up digital

	/* set adc */
	es8311_write_reg(0x14, 0x5A);	//dmic on, input selection, PGA gain

	/* set adc/dac data format */
	//es8311_write_reg(0x0A, 0x00);	//set adc data format = 24 bit i2s
	es8311_write_reg(0x0A, 0x0C);	//set adc data format = 16 bit i2s
	//es8311_write_reg(0x0A, 0x10);	//set adc data format = 32 bit i2s

	/* set low power mode */
	es8311_write_reg(0x0E, 0x0A);	//enable analog pga/adc modulator
	es8311_write_reg(0x0F, 0xF9);

	/* set adc */
	es8311_write_reg(0x15, 0xA0);	//set softramp
//	es8311_write_reg(0x15, 0xA1);	//set softramp

	es8311_write_reg(0x1B, 0x0A);	//set adc hpf

	/* adc alc function for amic record */
	es8311_write_reg(0x16, 0x22);	//set adc gain scale up
	es8311_write_reg(0x17, 0xCF);	//set adc alc max gain
	es8311_write_reg(0x18, 0x8A);	//set adc alc enable,alc_winsize
	es8311_write_reg(0x19, 0xFB);	//set adc alc target level
	es8311_write_reg(0x1A, 0x03);	//set adc_automute noise gate
	es8311_write_reg(0x1B, 0xEA);	//set adc_automute vol

	/* set adc/dac data form */
	es8311_write_reg(0x44, 0x08);	//adc to dac disable, ADCDATA=ADC(L)+ADC(R)
}

void es8311_codec_init_slave_demo(void)
{
	/* reset codec */
	//es8311_write_reg(0x00, 0x1F);
	es8311_write_reg(0x45, 0x00);

	/* set adc/dac clk */
	es8311_write_reg(0x01, 0x30);	//set mclkin, clk_adc, clk_dac
	es8311_write_reg(0x02, 0x10);
	es8311_write_reg(0x02, 0x40);	//set digmclk, clock doubler
	es8311_write_reg(0x03, 0x10);	//ADC FDMODE, ADC_OSR
	es8311_write_reg(0x16, 0x24);	//set ADC_SYNC mode
	es8311_write_reg(0x04, 0x10);	//DAC_OSR
	es8311_write_reg(0x05, 0x00);	//set adc_mclk, dac_mclk

	/* set system power up */
	es8311_write_reg(0x0B, 0x7B);	//power up stage A/B time
	es8311_write_reg(0x0C, 0x8F);	//power up stage B/C time
	es8311_write_reg(0x10, 0x03);	//set vmid/ibias
	es8311_write_reg(0x11, 0x7B);	//set vsel
	es8311_write_reg(0x00, 0x81);	//chip powerup slave mode. master mode(0xC0)
	rtos_delay_milliseconds(50);
	es8311_write_reg(0x01, 0x3A);	//power up digital

	/* set adc */
	es8311_write_reg(0x14, 0x40);	//dmic off, input selection, PGA gain

	/* set dac */
	es8311_write_reg(0x12, 0x00);

	/* enable HP drive */
	es8311_write_reg(0x13, 0x00);

	/* set adc/dac data format */
	es8311_write_reg(0x0A, 0x0C);	//set adc data format = 24 bit i2s
	es8311_write_reg(0x09, 0x00);	//set dac data format = 24 bit i2s

	/* set low or normal power mode */
	es8311_write_reg(0x0E, 0x0A);	//enable analog pga/adc modulator
	es8311_write_reg(0x0F, 0xFE);	//normal power mode

	/* set adc */
	es8311_write_reg(0x15, 0x00);	//set softramp
	es8311_write_reg(0x1B, 0x0A);	//set adc hpf
	es8311_write_reg(0x1C, 0x6A);	//set ADC HPF, ADC_EQ bypass
	es8311_write_reg(0x17, 0xBF);	//set adc digtal vol
}

void es8311_codec_init_master_demo(void)
{
	/* reset codec */
	//es8311_write_reg(0x00, 0x1F);
	es8311_write_reg(0x45, 0x00);

	/* set adc/dac clk */
	es8311_write_reg(0x01, 0x30);	//set mclkin, clk_adc, clk_dac
	es8311_write_reg(0x02, 0x10);
	es8311_write_reg(0x02, 0x40);	//set digmclk, clock doubler
	es8311_write_reg(0x03, 0x10);	//ADC FDMODE, ADC_OSR
	es8311_write_reg(0x16, 0x24);	//set ADC_SYNC mode
	es8311_write_reg(0x04, 0x10);	//DAC_OSR
	es8311_write_reg(0x05, 0x00);	//set adc_mclk, dac_mclk

	/* set system power up */
	es8311_write_reg(0x06, 0x0B);	//power up stage A/B time
	es8311_write_reg(0x07, 0x02);	//power up stage A/B time
	es8311_write_reg(0x08, 0xFF);	//power up stage A/B time

	es8311_write_reg(0x0B, 0x7B);	//power up stage A/B time
	es8311_write_reg(0x0C, 0x8F);	//power up stage B/C time
	es8311_write_reg(0x10, 0x03);	//set vmid/ibias
	es8311_write_reg(0x11, 0x7B);	//set vsel
	es8311_write_reg(0x00, 0xC1);	//chip powerup slave mode. master mode(0xC0)
	rtos_delay_milliseconds(50);
	es8311_write_reg(0x01, 0x3A);	//power up digital

	/* set adc */
	es8311_write_reg(0x14, 0x40);	//dmic off, input selection, PGA gain

	/* set dac */
	es8311_write_reg(0x12, 0x00);

	/* enable HP drive */
	es8311_write_reg(0x13, 0x00);

	/* set adc/dac data format */
	es8311_write_reg(0x0A, 0x0C);	//set adc data format = 24 bit i2s
	es8311_write_reg(0x09, 0x00);	//set dac data format = 24 bit i2s

	/* set low or normal power mode */
	es8311_write_reg(0x0E, 0x0A);	//enable analog pga/adc modulator
	es8311_write_reg(0x0F, 0xFE);	//normal power mode

	/* set adc */
	es8311_write_reg(0x15, 0x00);	//set softramp
	es8311_write_reg(0x1B, 0x0A);	//set adc hpf
	es8311_write_reg(0x1C, 0x6A);	//set ADC HPF, ADC_EQ bypass
	es8311_write_reg(0x37, 0x08);	//set adc digtal vol
	es8311_write_reg(0x17, 0xBF);	//set adc digtal vol
}

#endif
