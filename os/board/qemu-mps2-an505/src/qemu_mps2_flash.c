/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <tinyara/fs/ioctl.h>
#include <tinyara/fs/mtd.h>

#define QEMU_MPS2_FLASH_BLOCKSIZE   512
#define QEMU_MPS2_FLASH_ERASESIZE   4096
#define QEMU_MPS2_FLASH_ERASESTATE  0xff

struct qemu_mps2_flash_dev_s {
	struct mtd_dev_s mtd;
	FAR uint8_t *start;
	size_t neraseblocks;
};

static struct qemu_mps2_flash_dev_s g_qemu_mps2_flash;

static int qemu_mps2_flash_erase(FAR struct mtd_dev_s *dev, off_t startblock, size_t nblocks)
{
	FAR struct qemu_mps2_flash_dev_s *priv = (FAR struct qemu_mps2_flash_dev_s *)dev;
	size_t offset = startblock * QEMU_MPS2_FLASH_ERASESIZE;
	size_t nbytes = nblocks * QEMU_MPS2_FLASH_ERASESIZE;

	if (startblock + nblocks > priv->neraseblocks) {
		return -EINVAL;
	}

	memset(priv->start + offset, QEMU_MPS2_FLASH_ERASESTATE, nbytes);
	return (int)nblocks;
}

static ssize_t qemu_mps2_flash_bread(FAR struct mtd_dev_s *dev, off_t startblock,
				     size_t nblocks, FAR uint8_t *buf)
{
	FAR struct qemu_mps2_flash_dev_s *priv = (FAR struct qemu_mps2_flash_dev_s *)dev;
	size_t offset = startblock * QEMU_MPS2_FLASH_BLOCKSIZE;
	size_t nbytes = nblocks * QEMU_MPS2_FLASH_BLOCKSIZE;

	if ((offset + nbytes) > (priv->neraseblocks * QEMU_MPS2_FLASH_ERASESIZE)) {
		return -EINVAL;
	}

	memcpy(buf, priv->start + offset, nbytes);
	return (ssize_t)nblocks;
}

static ssize_t qemu_mps2_flash_bwrite(FAR struct mtd_dev_s *dev, off_t startblock,
				      size_t nblocks, FAR const uint8_t *buf)
{
	FAR struct qemu_mps2_flash_dev_s *priv = (FAR struct qemu_mps2_flash_dev_s *)dev;
	size_t offset = startblock * QEMU_MPS2_FLASH_BLOCKSIZE;
	size_t nbytes = nblocks * QEMU_MPS2_FLASH_BLOCKSIZE;

	if ((offset + nbytes) > (priv->neraseblocks * QEMU_MPS2_FLASH_ERASESIZE)) {
		return -EINVAL;
	}

	memcpy(priv->start + offset, buf, nbytes);
	return (ssize_t)nblocks;
}

static ssize_t qemu_mps2_flash_read(FAR struct mtd_dev_s *dev, off_t offset,
				    size_t nbytes, FAR uint8_t *buffer)
{
	FAR struct qemu_mps2_flash_dev_s *priv = (FAR struct qemu_mps2_flash_dev_s *)dev;

	if ((offset + nbytes) > (priv->neraseblocks * QEMU_MPS2_FLASH_ERASESIZE)) {
		return -EINVAL;
	}

	memcpy(buffer, priv->start + offset, nbytes);
	return (ssize_t)nbytes;
}

static ssize_t qemu_mps2_flash_write(FAR struct mtd_dev_s *dev, off_t offset,
				     size_t nbytes, FAR const uint8_t *buffer)
{
	FAR struct qemu_mps2_flash_dev_s *priv = (FAR struct qemu_mps2_flash_dev_s *)dev;

	if ((offset + nbytes) > (priv->neraseblocks * QEMU_MPS2_FLASH_ERASESIZE)) {
		return -EINVAL;
	}

	memcpy(priv->start + offset, buffer, nbytes);
	return (ssize_t)nbytes;
}

static int qemu_mps2_flash_ioctl(FAR struct mtd_dev_s *dev, int cmd, unsigned long arg)
{
	FAR struct qemu_mps2_flash_dev_s *priv = (FAR struct qemu_mps2_flash_dev_s *)dev;

	switch (cmd) {
	case MTDIOC_GEOMETRY:
		{
			FAR struct mtd_geometry_s *geo = (FAR struct mtd_geometry_s *)arg;
			if (!geo) {
				return -EINVAL;
			}

			geo->blocksize = QEMU_MPS2_FLASH_BLOCKSIZE;
			geo->erasesize = QEMU_MPS2_FLASH_ERASESIZE;
			geo->neraseblocks = priv->neraseblocks;
			return OK;
		}
	case MTDIOC_BULKERASE:
		memset(priv->start, QEMU_MPS2_FLASH_ERASESTATE,
		       priv->neraseblocks * QEMU_MPS2_FLASH_ERASESIZE);
		return OK;
	default:
		return -ENOTTY;
	}
}

FAR struct mtd_dev_s *qemu_mps2_flash_initialize(uintptr_t start, size_t size)
{
	FAR struct qemu_mps2_flash_dev_s *priv = &g_qemu_mps2_flash;

	if (size < QEMU_MPS2_FLASH_ERASESIZE) {
		return NULL;
	}

	priv->mtd.erase = qemu_mps2_flash_erase;
	priv->mtd.bread = qemu_mps2_flash_bread;
	priv->mtd.bwrite = qemu_mps2_flash_bwrite;
	priv->mtd.read = qemu_mps2_flash_read;
#ifdef CONFIG_MTD_BYTE_WRITE
	priv->mtd.write = qemu_mps2_flash_write;
#endif
	priv->mtd.ioctl = qemu_mps2_flash_ioctl;
	priv->start = (FAR uint8_t *)start;
	priv->neraseblocks = size / QEMU_MPS2_FLASH_ERASESIZE;

	return &priv->mtd;
}
