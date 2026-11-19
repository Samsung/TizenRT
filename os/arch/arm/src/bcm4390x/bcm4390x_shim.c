/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/src/bcm4390x/bcm4390x_shim.c
 *
 * Compatibility shims for the prebuilt libbcmarch.a: the library was built
 * against the pre-NuttX-12.13.0 kernel headers and references kernel APIs
 * that were renamed or removed.  This file provides thin wrappers where a
 * 12.13.0 equivalent exists and stubs where the API was removed.
 *
 * NOTE: this only guarantees that the image links.  The prebuilt library
 * was compiled against the old kernel ABI, so its runtime behavior is not
 * guaranteed and is out of the scope of the board port.
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <errno.h>
#include <syslog.h>
#include <assert.h>

#include <tinyara/init.h>
#include <tinyara/mtd/mtd.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/* os_start() was renamed to nx_start() in 12.13.0 */

void os_start(void)
{
	nx_start();
}

/* up_assert() was renamed to _assert() in 12.13.0 */

void up_assert(FAR const char *filename, int linenum, FAR const char *msg, FAR void *regs)
{
	_assert(filename, linenum, msg, regs);
}

/* get_errno_ptr() was renamed to __errno() in 12.13.0 */

FAR int *get_errno_ptr(void)
{
	return __errno();
}

/* lowsyslog() was removed in 12.13.0; forward to vsyslog() */

void lowsyslog(int priority, FAR const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vsyslog(priority, fmt, ap);
	va_end(ap);
}

/* mksmartfs() was removed in 12.13.0 with no in-kernel replacement */

int mksmartfs(FAR const char *pathname, uint8_t nrootdirs, bool format)
{
	return -ENOSYS;
}

/* mtd_register() was removed in 12.13.0 */

int mtd_register(FAR struct mtd_dev_s *mtd, FAR const char *name)
{
	return -ENOSYS;
}

/* i2c_uioregister() is built only for CONFIG_I2C_SLAVE_DRIVER and still
 * uses the removed struct i2c_dev_s in 12.13.0; stub it out.
 */

int i2c_uioregister(FAR const char *path, FAR struct i2c_master_s *dev)
{
	return -ENOSYS;
}

/* bcm4390x_wwd_init() is provided by the WICED external library
 * (CONFIG_WL_BCM4390X), which is not part of this configuration.
 */

int bcm4390x_wwd_init(void)
{
	return 0;
}

/* nuttx_platform_irq_demuxer() was a platform hook of the old Cypress
 * board support that is not part of the public tree; the prebuilt library
 * references it, so provide a stub.
 */

void nuttx_platform_irq_demuxer(void)
{
}

/* sched_process_timer() was renamed to nxsched_process_timer() in 12.13.0 */

void sched_process_timer(void)
{
	nxsched_process_timer();
}

/* The BCM4390x has no in-tree serial driver; the console is handled by the
 * prebuilt library, so arm_serialinit() is an empty stub.
 */

void arm_serialinit(void)
{
}

/* gpio_register() lived in os/drivers/gpio/gpio.c which was removed when
 * the GPIO framework moved to drivers/ioexpander in 12.13.0; stub it out.
 */

int gpio_register(unsigned int minor, FAR struct gpio_lowerhalf_s *lower)
{
	return -ENOSYS;
}

/****************************************************************************
 * MPU helpers
 *
 * mpu_allocregion()/mpu_log2regionceil()/mpu_subregion() are provided by
 * armv7-r/arm_mpu.c only for CONFIG_BUILD_PROTECTED.  Provide the minimal
 * self-contained implementations for the flat build (copied from the
 * pre-12.13.0 armv7-r/arm_mpu.c).
 ****************************************************************************/

static const uint8_t g_ms_regionmask[9] = {
	0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x00
};

static const uint8_t g_ls_regionmask[9] = {
	0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff
};

static inline uint32_t mpu_subregion_ms(size_t size, uint8_t l2size)
{
	unsigned int nsrs;
	uint32_t asize;
	uint32_t mask;

	if (l2size < 32) {
		mask = ((1 << l2size) - 1) >> 3;
	} else {
		mask = 0x1fffffff;
	}

	asize = (size + mask) & ~mask;
	nsrs = asize >> (l2size - 3);
	return g_ms_regionmask[nsrs];
}

static inline uint32_t mpu_subregion_ls(size_t offset, uint8_t l2size)
{
	unsigned int nsrs;
	uint32_t aoffset;
	uint32_t mask;

	if (l2size < 32) {
		mask = ((1 << l2size) - 1) >> 3;
	} else {
		mask = 0x1fffffff;
	}

	aoffset = (offset + mask) & ~mask;
	nsrs = aoffset >> (l2size - 3);
	return g_ls_regionmask[nsrs];
}

uint32_t mpu_subregion(uintptr_t base, size_t size, uint8_t l2size)
{
	uint32_t mask;
	size_t offset;
	uint32_t ret;

	if (l2size < 8) {
		return 0;
	}

	mask = (1 << l2size) - 1;
	offset = base & mask;

	ret = mpu_subregion_ms(size + offset, l2size);
	ret |= mpu_subregion_ls(offset, l2size);
	return ret;
}

unsigned int mpu_allocregion(void)
{
	static unsigned int g_region;

	return g_region++;
}

uint8_t mpu_log2regionceil(size_t size)
{
	uint8_t l2size;

	/* The minimum permitted region size is 32 bytes (log2(32) = 5) */

	for (l2size = 5; l2size < 32 && size > (1 << l2size); l2size++) ;

	return l2size;
}
