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
//#include <poll.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>

#include <tinyara/spi/spi.h>
#include <tinyara/i2c.h>
#include <tinyara/sensors/mlx90617.h>
//#include <tinyara/input/touchscreen.h>
//#include <tinyara/input/ist415.h>

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/
/****************************************************************************
 * Private Types
 ****************************************************************************/

/* MLX90617 Device */

struct mlx90617_dev_s
{
	/* I2C bus and address for device. */
	struct i2c_dev_s *i2c;
	struct i2c_config_s config;
	struct spi_dev_s *spi;
	//struct lcd_touch_config *lower;
	
	/* Configuration for device. */
	sem_t sem;
	int crefs;
};


/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int mlx90617_open(FAR struct file *filep);
static int mlx90617_close(FAR struct file *filep);
static ssize_t mlx90617_read(FAR struct file *filep, FAR char *buffer,
                            size_t buflen);
static ssize_t mlx90617_write(FAR struct file *filep, FAR char *buffer,
                            size_t buflen);
static ssize_t mlx90617_ioctl(FAR struct file *filep, FAR char *buffer,
                            size_t buflen);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static FAR struct mlx90617_dev_s g_mlx90617_priv;

/* File Operations exposed to NuttX Apps */
static const struct file_operations g_mlx90617_fileops =
{
  mlx90617_open,   /* open */
  mlx90617_close,  /* close */
  mlx90617_read,   /* read */
  mlx90617_write,           /* write */
  NULL,           /* seek */
  mlx90617_ioctl,          /*ioctl */
  NULL,          
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void mlx90617_read_i2c(struct i2c_dev_s *i2c,struct i2c_config_s config, uint8_t reg, int len, uint16_t *data)
{
	lldbg("%d %d %d\n", config.address, config.addrlen, config.frequency);
	uint8_t buffer[3];
    	if (i2c) {
#ifdef CONFIG_I2C_WRITEREAD
		int ret = i2c_writeread(i2c, &config, (uint8_t *)&reg, 1, buffer, len);
                printf("ret %d\n", ret);
#else
		int ret = i2c_write(i2c, &config, (uint8_t *)&reg, 1);
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

/************************************************************************************
 * Name: mlx90617_semtake
 ************************************************************************************/
static inline int mlx90617_semtake(FAR sem_t *sem, bool errout)
{
	/* Take a count from the semaphore, possibly waiting */
	if (sem_wait(sem) < 0) {
		/* EINTR is the only error that we expect */
		int errcode = get_errno();
		DEBUGASSERT(errcode == EINTR);
		if (errout) {
			return errcode;
		}
	}

	return 0;
}

/****************************************************************************
 * Name: mlx90617_semgive
 ****************************************************************************/

void mlx90617_semgive(sem_t *sem)
{
	sem_post(sem);
}

static int mlx90617_open(FAR struct file *filep)
{
	FAR struct mlx90617_dev_s *priv;
	priv = filep->f_inode->i_private;

	if (!priv) {
		return -EINVAL;
	}

	mlx90617_semtake(&priv->sem, false);
	priv->crefs++;
	DEBUGASSERT(priv->crefs > 0);
	mlx90617_semgive(&priv->sem);
}


static int mlx90617_close(FAR struct file *filep)
{
	FAR struct mlx90617_dev_s *priv;
	priv = filep->f_inode->i_private;

	if (!priv) {
		return -EINVAL;
	}

	mlx90617_semtake(&priv->sem, false);
	DEBUGASSERT(priv->crefs > 0);
	priv->crefs--;
	mlx90617_semgive(&priv->sem);
}

static ssize_t mlx90617_read(FAR struct file *filep, FAR char *buffer,
                            size_t buflen)
{
	FAR struct inode *inode;
	FAR struct mlx90617_dev_s *priv;
	size_t outlen;
	irqstate_t flags;
	int ret;
	uint8_t reg[2];
	uint16_t data[3];
	float Ta_N, To_N;
	inode = filep->f_inode;

	DEBUGASSERT(inode && inode->i_private);
	priv = inode->i_private;
	struct i2c_dev_s *i2c = priv->i2c;
	struct i2c_config_s config = priv->config;
	/* Wait for semaphore to prevent concurrent reads */

	mlx90617_semtake(&priv->sem, false);

	config.address = 0x5A;
	//config.address = (0xA0 >> 1);
	reg[0] = 0x06;
	//reg[0] = 0x23;
	mlx90617_read_i2c(i2c, config, reg, 3, data);
	Ta_N = (float)(data[0]) * 0.02 - 273.15;  // MLX90614 Ta
	printf("Ta_N 614%f\n", Ta_N);
	
	reg[0] = 0x07;
        mlx90617_read_i2c(i2c, config, reg, 3, data);
	To_N = (float)(data[0]) * 0.02 - 273.15;  // MLX90614 To
	printf("To_N 614%f\n", To_N);

	config.address = 0x5D;
	reg[0] = 0x06;
        mlx90617_read_i2c(i2c, config, reg, 3, data);
        Ta_N = (float)(data[0]) * 0.02 - 273.15;  // MLX90614 Ta
        printf("Ta_N 617%f\n", Ta_N);

        reg[0] = 0x07;
        mlx90617_read_i2c(i2c, config, reg, 3, data);
        To_N = (float)(data[0]) * 0.02 - 273.15;  // MLX90614 To
        printf("To_N 617%f\n", To_N);

	mlx90617_semgive(&priv->sem);
	return 0;
}

static ssize_t mlx90617_write(FAR struct file *filep, FAR char *buffer,
                            size_t buflen)
{
	return 0;
}

static ssize_t mlx90617_ioctl(FAR struct file *filep, FAR char *buffer,
                            size_t buflen)
{
	return 0;

}

int mlx90617_register(FAR const char *devpath,
                     FAR struct i2c_dev_s *i2c_dev,
                     FAR struct sensor_config *config, FAR struct spi_dev_s *spi)
{
	struct mlx90617_dev_s *priv = &g_mlx90617_priv;;
	int ret = 0;

	/* Setup device structure. */

	//priv->lower = config;
	priv->config = config->i2c_config;
	priv->i2c = i2c_dev;
	priv->spi = spi;
	sem_init(&priv->sem, 0, 1);

	ret = register_driver(devpath, &g_mlx90617_fileops, 0666, priv);
	if (ret < 0) {
		kmm_free(priv);
		sem_destroy(&priv->sem);
		printf("IR sensor Driver registration failed\n");
		return ret;
	}
	return 0;
}
