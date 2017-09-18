/****************************************************************************
 * drivers/i2c/i2c_ioctrl.c
 *
 *   Copyright (C) 2016,2017 SAMSUNG ELECTRONICS CO., LTD. All rights reserved.
 *   Author: ByoungTae Cho <bt.cho@samsung.com>
 *   Author: Tomasz Wozniak <t.wozniak@samsung.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/i2c.h>
#include <tinyara/fs/ioctl.h>

/****************************************************************************
 * Name: i2c_ioctrl
 *
 * Description:
 *   IO control on I2C device.
 *
 * TODO: validate pointers
 ****************************************************************************/

int i2c_ioctrl(FAR struct i2c_dev_s *dev, int cmd, unsigned long arg)
{
	int32_t ret;
	int addr;
	FAR struct i2c_rdwr_ioctl_data_s *rdwr;
	int32_t freq;
#ifdef CONFIG_I2C_ARTIK_EXTENSIONS
	int atomic;
#endif

	switch (cmd) {
	case I2C_SLAVE:
	case I2C_SLAVE_FORCE:
		addr = *((int *)arg);
		ret = I2C_SETADDRESS(dev, addr, -1);
		break;
	case I2C_TENBIT:
		/* 0 for 7 bit addrs, != 0 for 10 bit */
		if (arg == 0) {
			ret = I2C_SETADDRESS(dev, -1, 7);
		} else {
			ret = I2C_SETADDRESS(dev, -1, 10);
		}
		break;
	case I2C_RDWR:
		rdwr = (struct i2c_rdwr_ioctl_data_s *)(arg);
		ret = I2C_TRANSFER(dev, rdwr->msgs, rdwr->nmsgs);
		break;

	case I2C_FREQUENCY:
		freq = *((uint32_t *)arg);
		ret = I2C_SETFREQUENCY(dev, freq);
		break;
#ifdef CONFIG_I2C_ARTIK_EXTENSIONS
	case I2C_ATOMICITY:
		atomic = *((uint32_t *)arg) & I2C_ATOMIC_FLAG;
		ret = I2C_SETATOMIC(dev, atomic);
		break;
#endif

	default:
		dbg("Unknown cmd(%x)\n", cmd);
		ret = -EINVAL;
		break;
	}

	return ret;
}
