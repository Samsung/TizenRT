/****************************************************************************
 * drivers/input/ist415.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>

#include <tinyara/i2c.h>
#include <tinyara/audio/audio.h>
#include <tinyara/audio/i2s.h>

#include <tinyara/sensors/sensor.h>
#include <tinyara/sensors/ais25ba.h>

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/
/****************************************************************************
 * Private Types
 ****************************************************************************/
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static ssize_t ais25ba_read(FAR struct sensor_upperhalf_s *dev, FAR char *buffer);
static void ais25ba_set_mclk(struct sensor_upperhalf_s *priv, int mclk);
static void ais25ba_set_bclk(struct sensor_upperhalf_s *priv, int bclk);
static void ais25ba_start(struct sensor_upperhalf_s *priv);
static void ais25ba_stop(struct sensor_upperhalf_s *priv);
static void ais25ba_setchannel_count(struct sensor_upperhalf_s *priv, int channel_count);
static void ais25ba_setbit_perchannel(struct sensor_upperhalf_s *priv, int bit_per_channel);
static void ais25ba_set_samprate(struct sensor_upperhalf_s *priv, int samp_rate);

/****************************************************************************
 * Private Data
 ****************************************************************************/
struct sensor_ops_s g_ais25ba_ops = {
        .sensor_read = ais25ba_read,
	.sensor_set_mclk = ais25ba_set_mclk,
	.sensor_set_bclk = ais25ba_set_bclk,
	.sensor_start = ais25ba_start,
	.sensor_stop = ais25ba_stop,
	.sensor_setchannel_count = ais25ba_setchannel_count,
	.sensor_setbit_perchannel = ais25ba_setbit_perchannel,
	.sensor_set_samprate = ais25ba_set_samprate,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void ais25ba_i2s_callback(FAR struct i2s_dev_s *dev, FAR struct ap_buffer_s *apb, FAR void *arg, int result)
{
        lldbg("in callback..........................................\n");
        lldbg("apb=%p nbytes=%d result=%d\n", apb, apb->nbytes, result);

}

static void ais25ba_set_mclk(struct sensor_upperhalf_s *priv, int mclk)
{
}

static void ais25ba_set_bclk(struct sensor_upperhalf_s *priv, int bclk)
{
}

static void ais25ba_start(struct sensor_upperhalf_s *upper)
{

	struct ais25ba_dev_s *priv = upper->priv;
	struct i2s_dev_s *i2s = priv->i2s;
	struct ap_buffer_s *apb;
        //apb_reference(&apb);
        struct audio_buf_desc_s desc;
        desc.numbytes = 64;
        desc.u.ppBuffer = &apb;

        int ret = apb_alloc(&desc);
        if (ret < 0) {
                printf("alloc fail\n");
                return;
        }
        ret = I2S_RECEIVE(i2s, apb, ais25ba_i2s_callback, NULL, 1000);/* 100 ms timeout for read data */
        printf("i2s receive return %d\n", ret);
        if (ret < 0) {
                printf("ERROR: I2S_RECEIVE returned: %d\n", ret);
        }

}

static void ais25ba_stop(struct sensor_upperhalf_s *upper)
{
	struct ais25ba_dev_s *priv = upper->priv;
	I2S_STOP(priv->i2s, I2S_RX);
}

static void ais25ba_setchannel_count(struct sensor_upperhalf_s *priv, int channel_count)
{
	
}

static void ais25ba_setbit_perchannel(struct sensor_upperhalf_s *upper, int bit_per_channel)
{
	struct ais25ba_dev_s *priv = upper->priv;

        I2S_RXDATAWIDTH(priv->i2s, bit_per_channel);

}

static void ais25ba_set_samprate(struct sensor_upperhalf_s *upper, int samp_rate)
{
	FAR struct ais25ba_dev_s *priv = upper->priv;
	printf("sample rate %d\n", samp_rate);
	I2S_RXSAMPLERATE(priv->i2s, samp_rate);
}

static void ais25ba_verify_sensor(struct i2c_dev_s *i2c, struct i2c_config_s config)
{
        int ret = 0;
        int reg[2];
        uint8_t data[2];
        reg[0] = 0x0F;                                  //WHO_AM_I
        printf("add %8x, freq %d, len%d\n", config.address, config.frequency, config.addrlen);
#ifdef CONFIG_I2C_WRITEREAD
	ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1);
	printf("ret %d data : %8x\n", ret, data[0]);
#else
        ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
        if (ret == 1) {
                i2c_read(i2c, &config, (uint8_t *)data, 1);
                printf("data read is %8x\n", data[0]); // this should be 0x20
        }
#endif
}

static void read_data(struct i2c_dev_s *i2c, struct i2c_config_s config)
{
        int ret = 0;
        uint8_t reg[2];
        uint8_t data[2];
#ifdef CONFIG_I2C_WRITEREAD
	reg[0] = 0x0B;
	ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1);
        printf("ret 0B data : %8x\n",  data[0]);
	
	reg[0] = 0x26;
        ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1);
        printf("ret 26 data : %8x\n",  data[0]);

	reg[0] = 0x2F;
        ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1);
        printf("ret 2F data : %8x\n",  data[0]);

	reg[0] = 0x30;
        ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1);
        printf("ret 30 data : %8x\n", data[0]);

	reg[0] = 0x2E;
        ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1);
        printf("ret 2E data : %8x\n", data[0]);

#else
	reg[0] = 0x0B;
	ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
        if (ret == 1) {
                i2c_read(i2c, &config, (uint8_t *)data, 1);
                printf("data read 0B : %8x\n", data[0]); // this should be 0x20
        }

	reg[0] = 0x26;
        ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
        if (ret == 1) {
                i2c_read(i2c, &config, (uint8_t *)data, 1);
                printf("data read 26 : %8x\n", data[0]); // this should be 0x20
        }

	reg[0] = 0x2F;
        ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
        if (ret == 1) {
                i2c_read(i2c, &config, (uint8_t *)data, 1);
                printf("data read 2F : %8x\n", data[0]); // this should be 0x20
        }

	reg[0] = 0x30;
        ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
        if (ret == 1) {
                i2c_read(i2c, &config, (uint8_t *)data, 1);
                printf("data read 30 : %8x\n", data[0]); // this should be 0x20
        }

	reg[0] = 0x2E;
        ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
        if (ret == 1) {
                i2c_read(i2c, &config, (uint8_t *)data, 1);
                printf("data read 2E : %8x\n", data[0]); // this should be 0x20
        }
#endif
}

static void write_data(struct i2c_dev_s *i2c, struct i2c_config_s config)
{

        int ret = 0;
        uint8_t reg[2];
        uint8_t data[2];
#ifdef CONFIG_I2C_WRITEREAD
        reg[0] = 0x26;
	reg[1] = 0x00;
        ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 2, data, 0);
        //printf("\nret 26 data : %8x\n",  data[0]);
	DelayMs(100);
	reg[0] = 0x2E;
        reg[1] = 0x32;
        ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 2, data, 0);
        //printf("\nret 2E data : %8x\n",  data[0]);
#else
	reg[0] = 0x26;
	reg[1] = 0x00;
	ret = i2c_write(i2c, &config, (uint8_t *)reg, 2);

	reg[0] = 0x2E;
	reg[1] = 0x32;
	ret = i2c_write(i2c, &config, (uint8_t *)reg, 2);
#endif
}

/*static void ais25ba_ctrl_tdm(struct i2c_dev_s *i2c, struct i2c_config_s config)
{
        int ret = 0;
        int reg[2];
        uint8_t data[2];

	reg[0] = 0x0B;
	//ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
	//ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1);
       // printf("ret self test %d data : 0b %8x\n", ret, data[0]);

        reg[0] = 0x26;                  //              CTRL_REG_1
        reg[1] = 0x00;                  //PD : normal mode - enabling device
        ret = i2c_write(i2c, &config, (uint8_t *)reg, 2);
	if (ret == 2) {
                ret = i2c_read(i2c, &config, (uint8_t *)data, 1);
                printf("data read 26 is %8x\n", data[0]); 
        }
	//printf("writing ctrl reg1 ret: %d\n", ret);
	//ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 2, data, 1);
        printf("ret ctrl reg1 %d data : 26 %8x\n", ret, data[0]);

        reg[0] = 0x2F;                  //              CTRL_REG_2
        reg[1] = 0xE1;                  //auto_odr : 1
	//ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 2, data, 1);
        ret = i2c_write(i2c, &config, (uint8_t *)reg, 2);
	if (ret == 2) {
		ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
		if (ret == 1) {
                ret = i2c_read(i2c, &config, (uint8_t *)data, 1);
                printf("data read 2F is %8x\n", data[0]); 
		}
        }
        printf("ret ctrl reg2 %d data : 2f %8x\n", ret, data[0]);

        reg[0] = 0x30;                          //accelerometer FS(full scale) selection  CTRL_REG_FS
        reg[1] = 0x00;                          //FS = 3.85g  ---> 0.122 sensitivity
	//ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 2, data, 1);
        ret = i2c_write(i2c, &config, (uint8_t *)reg, 2);
	if (ret == 2) {
                ret = i2c_read(i2c, &config, (uint8_t *)data, 1);
                printf("data read 30 is %8x\n", data[0]); 
        }
        printf("ret fs = %d data : 30 %8x\n", ret, data[0]);
        if (ret == 1) {
                i2c_read(i2c, &config, (uint8_t *)data, 2);
                lldbg("data read is %8x\n", data[0]);
        }

        reg[0] = 0x2E;                  //tdm ctrl register                     TDM_CTRL_REG
        reg[1] = 0x32;                  //WCLK = 16KHz , enable TDM, no delayed configuration, valid data, tdm mapping 0
	ret = i2c_write(i2c, &config, (uint8_t *)reg, 2);
	//printf("writing tdm ctrl ret: %d\n", ret);
        //ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 2, data, 1);
	if (ret == 2) {
		ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
                if (ret == 1) {
                ret = i2c_read(i2c, &config, (uint8_t *)data, 1);
                printf("data read 2e is %8x\n", data[0]); 
		}
        }
        printf("\nret tdm ctrl= %d data : 2e %8x\n", ret, data[0]);
        if (ret == 1) {
                i2c_read(i2c, &config, (uint8_t *)data, 2);
                lldbg("data read is %8x\n", data[0]);
        }
}*/

static void ais25ba_read_i2c(struct i2c_dev_s *i2c,struct i2c_config_s config, uint8_t reg[], int len, uint16_t *data)
{
	lldbg("%d %d %d\n", config.address, config.addrlen, config.frequency);
	uint8_t buffer[3];
    	if (i2c) {
#ifdef CONFIG_I2C_WRITEREAD
		int ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 1, buffer, len);
                printf("ret %d\n", ret);
#else
		int ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
		if (ret != 1) {
			printf("ERROR: i2c write not working\n");
			return;
		}
		ret = i2c_read(i2c, &config, buffer, len);
#endif
		data[0] = (uint16_t)(buffer[0] | (buffer[1] << 8));
		printf("value[0]: %8x | value[1]: %8x | value[2]: %8x | %8x ret: %d\n",
		buffer[0], buffer[1], buffer[2], ret);
	}
}

static int ais25ba_read_i2s(struct i2s_dev_s *i2s)
{
        struct ap_buffer_s *apb;
        //apb_reference(&apb);
        struct audio_buf_desc_s desc;
        desc.numbytes = 64;
        desc.u.ppBuffer = &apb;

        int ret = apb_alloc(&desc);
        if (ret < 0) {
                printf("alloc fail\n");
                return;
        }
        ret = I2S_RECEIVE(i2s, apb, ais25ba_i2s_callback, NULL, 1000);/* 100 ms timeout for read data */
	printf("i2s receive return %d\n", ret);
        if (ret < 0) {
                printf("ERROR: I2S_RECEIVE returned: %d\n", ret);
        }
	return ret;
}

static ssize_t ais25ba_read(FAR struct sensor_upperhalf_s *dev, FAR char *buffer)
{
	FAR struct ais25ba_dev_s *priv = dev->priv;
	size_t outlen;
	irqstate_t flags;
	int ret;
	uint8_t reg[2];
	uint16_t data[3];
	float Ta_N, To_N;

	struct i2c_dev_s *i2c = priv->i2c;
	struct i2s_dev_s *i2s = priv->i2s;
	struct i2c_config_s config = priv->i2c_config;
	/* Wait for semaphore to prevent concurrent reads */
	ais25ba_verify_sensor(i2c, config);
	read_data(i2c, config);
	DelayMs(2000);
	write_data(i2c, config);
	DelayMs(2000);
	read_data(i2c, config);
	/*ais25ba_ctrl_tdm(i2c, config);*/
	ais25ba_read_i2s(i2s);
	
	return OK;
}


int ais25ba_initialize(const char *devpath, struct ais25ba_dev_s *priv)
{
	int ret = 0;

	/* Setup device structure. */

	struct sensor_upperhalf_s *upper = (struct sensor_upperhalf_s *)kmm_zalloc(sizeof(struct sensor_upperhalf_s));
	upper->ops = &g_ais25ba_ops;
	upper->priv = priv;
	priv->upper = upper;

	printf("sensor registered success\n");
	return sensor_register(devpath, upper);
}
