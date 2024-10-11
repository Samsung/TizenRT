/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <debug.h>
#include <assert.h>
#include <errno.h>
#include <tinyara/i2c.h>
#include <tinyara/gpio.h>
#include <tinyara/audio/audio.h>
#include <tinyara/audio/i2s.h>
#include <tinyara/sensors/sensor.h>
#include <tinyara/sensors/ais25ba.h>

/*#include "objects.h"
#include "gpio_irq_api.h"
#include "PinNames.h"
#include "gpio_api.h"*/
#include "amebasmart_i2s.h"
#define PIN_LOW 0
#define PIN_HIGH 1

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* i2c config */
#if CONFIG_RTL8730E_BOARD_REVISION >= 6
#define AIS25BA_I2C_PORT		0
#else
#define AIS25BA_I2C_PORT		0
#endif

#define AIS25BA_I2C_FREQ		100000
#define AIS25BA_I2C_ADDRLEN		7
#define AIS25BA_I2C_ADDR		0x19
#define AIS25BA_I2S_PORT                2

/*other pin config */
#define AIS25BA_GPIO_RESET_PIN		PA_5
#if CONFIG_RTL8730E_BOARD_REVISION >= 6
#define AIS25BA_GPIO_I2C_PIN PA_4
#else
#define AIS25BA_GPIO_I2C_PIN PA_2
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct rtl8730e_ais25ba_s {
	//struct lcd_touch_config touch_config;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
//extern FAR struct i2s_dev_s *amebasmart_i2s_initialize(uint16_t port, bool is_reinit);

static void rtl8730e_verify_sensor(struct i2c_dev_s *i2c, struct i2c_config_s config);
static void rtl8730e_ctrl_tdm(struct i2c_dev_s *i2c, struct i2c_config_s config);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static struct rtl8730e_ais25ba_s g_rtl8730e_ais25ba_priv0 = {
};

static struct ais25ba_dev_s g_ais25ba_dev0 = {
        .i2c = NULL,
        .i2c_config = {
                .frequency = AIS25BA_I2C_FREQ,
                .address = AIS25BA_I2C_ADDR,
                .addrlen = AIS25BA_I2C_ADDRLEN,
        },
        .priv = &g_rtl8730e_ais25ba_priv0,
};


/****************************************************************************
 * Private Functions
 ****************************************************************************/
static float ais25ba_from_raw_to_mg(int16_t lsb)
{
	return ((float)lsb) * 0.122f;
}

static void rtl8730e_verify_sensor(struct i2c_dev_s *i2c, struct i2c_config_s config)
{
	int ret = 0;
	int reg[1];
	uint8_t data[1];
	reg[0] = 0x0F;					//WHO_AM_I
	lldbg("add %8x, freq %d, len%d\n", config.address, config.frequency, config.addrlen);
	ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
	if (ret == 1) {
		i2c_read(i2c, &config, (uint8_t *)data, 1);
		lldbg("data read is %8x\n", data[0]); // this should be 0x20
	}
}

static void rtl8730e_ctrl_tdm(struct i2c_dev_s *i2c, struct i2c_config_s config)
{
	int ret = 0;
	int reg[2];
	uint8_t data[2];
	reg[0] = 0x26;			//		CTRL_REG_1
	reg[1] = 0x00;			//PD : normal mode - enabling device
	ret = i2c_write(i2c, &config, (uint8_t *)reg, 2);
	lldbg("ret ctrl reg1 %d\n", ret);
	
	reg[0] = 0x2F;			//		CTRL_REG_2
	reg[1] = 0xE1;			//auto_odr : 1
	ret = i2c_write(i2c, &config, (uint8_t *)reg, 2);
	lldbg("ret ctrl reg2 %d\n", ret);
	
	reg[0] = 0x30;				//accelerometer FS(full scale) selection  CTRL_REG_FS
	reg[1] = 0x00;				//FS = 3.85g  ---> 0.122 sensitivity
	ret = i2c_write(i2c, &config, (uint8_t *)reg, 2);
	lldbg("ret fs = %d\n", ret);
	/*if (ret == 1) {
		i2c_read(i2c, &config, (uint8_t *)data, 2);
		lldbg("data read is %8x\n", data[0]); 
	}*/
	
	reg[0] = 0x2E;			//tdm ctrl register			TDM_CTRL_REG
	reg[1] = 0x02;			//WCLK = 16KHz , enable TDM, no delayed configuration, valid data, tdm mapping 0
	ret = i2c_write(i2c, &config, (uint8_t *)reg, 2);
	lldbg("ret tdm ctrl= %d\n", ret);
	/*if (ret == 1) {
		i2c_read(i2c, &config, (uint8_t *)data, 2);
		lldbg("data read is %8x\n", data[0]); 
	}*/
}

/*static void ais25ba_gpio_reset()
{
	GPIO_WriteBit(ais25ba_GPIO_RESET_PIN, PIN_HIGH);
	DelayMs(500);
	GPIO_WriteBit(ais25ba_GPIO_RESET_PIN, PIN_LOW);
	DelayMs(500);
	GPIO_WriteBit(ais25ba_GPIO_RESET_PIN, PIN_HIGH);
	DelayMs(500);
	return;
}

static void ais25ba_gpio_init()
{
	Pinmux_Config(ais25ba_GPIO_RESET_PIN, PINMUX_FUNCTION_GPIO);
	GPIO_InitTypeDef TouchResetPin;
	TouchResetPin.GPIO_Pin = ais25ba_GPIO_RESET_PIN;
	TouchResetPin.GPIO_PuPd = GPIO_PuPd_NOPULL;
	TouchResetPin.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(&TouchResetPin);
}*/

void ais25ba_callback(FAR struct i2s_dev_s *dev, FAR struct ap_buffer_s *apb, FAR void *arg, int result)
{
	lldbg("in callback..........................................\n");
	lldbg("apb=%p nbytes=%d result=%d\n", apb, apb->nbytes, result);

        /* REVISIT: If you want this to actually do something other than
         * test I2S data transfer, then this is the point where you would
         * want to let some application know that the transfer has complete.
         */

        /* Release our reference to the audio buffer.  Hopefully it will be freed
         * now.
         */
	/*uint8_t *ptr;
	uint8_t data;
	int j = 0;
	for (ptr = apb->samp; j < 1; j++) {
                 data = ptr[j];
                        j++;
				if(data !=NULL) {
               lldbg("data %d\n", data);
				}
    }*/

	/*int8_t x1 = 0;
	int8_t x2 = 0;
	int16_t x = x1 << 8 || x2;
	float x_f = ais25ba_from_raw_to_mg(x);
	for (i = 0U; i < 3U; i++)
  {
    data->xl.raw[i] = (int16_t) tdm_stream[i + offset];
    data->xl.mg[i] = ais25ba_from_raw_to_mg(data->xl.raw[i]);
  }
        lldbg("Freeing apb=%p crefs=%d\n", apb, apb->crefs);
        apb_free(apb);*/

}

static void read_i2s(struct i2s_dev_s *i2s)
{
	struct ap_buffer_s *apb;
	//apb_reference(&apb);
	struct audio_buf_desc_s desc;
	desc.numbytes = 256;
        desc.u.ppBuffer = &apb;

        int ret = apb_alloc(&desc);
	if (ret < 0) {
		lldbg("alloc fail\n");
		return;
	}
	ret = I2S_RECEIVE(i2s, apb, ais25ba_callback, NULL, 100);/* 100 ms timeout for read data */
	if (ret < 0) {
		lldbg("ERROR: I2S_RECEIVE returned: %d\n", ret);
		kmm_free(apb);
	//	apb_free(&apb);
		return;
	}
}
/****************************************************************************
 * Public Functions
 ****************************************************************************/

void rtl8730e_ais25ba_initialize()
{
	FAR struct i2c_dev_s *i2c;
	struct i2s_dev_s *i2s;
	i2s = amebasmart_i2s_initialize(AIS25BA_I2S_PORT, 0);
	if (!i2s) {
		lldbg("ERROR: Failed to initialize I2S\n");
	}
	int ret = 0;

	i2c = up_i2cinitialize(AIS25BA_I2C_PORT);
	if (!i2c) {
		lldbg("ERROR: Failed to initialize I2C\n");
	}
	/*struct i2c_config_s config;
	config.frequency = AIS25BA_I2C_FREQ;
	config.address = AIS25BA_I2C_ADDR;
	config.addrlen = AIS25BA_I2C_ADDRLEN;
	rtl8730e_verify_sensor(i2c, config);
	rtl8730e_ctrl_tdm(i2c, config);*/
	//read_i2s(i2s);
	g_ais25ba_dev0.i2c = i2c;
	g_ais25ba_dev0.i2s = i2s;
	ret = ais25ba_initialize("/dev/sensor1", &g_ais25ba_dev0);
	if (ret < 0) {
		lldbg("ERROR: MEMS ais25ba driver register fail\n");
		return;
	}
	lldbg("MEMS ais25ba driver register success\n");
}
