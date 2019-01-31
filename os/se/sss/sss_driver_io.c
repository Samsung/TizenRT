/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <tinyara/fs/fs.h>
#include <tinyara/fs/mtd.h>
#include <tinyara/fs/ioctl.h>

#include "isp_driver_error.h"
#include "sss_driver_io.h"

#define S5J_SSS_RO_DEVICE_NAME "/dev/mtdblock1"

char *sss_get_flash_device_name(void)
{
	return S5J_SSS_RO_DEVICE_NAME;
}

int sss_ro_read(unsigned int start_offset, unsigned char *buf, unsigned int byte_size)
{
	int ret = OK;
#if defined(SSS_OVER_MTDFTL)
	struct mtd_geometry_s geo;
	FAR struct inode *pnode = NULL;
	char *devname = sss_get_flash_device_name();
	unsigned char *read_buf = NULL;
	unsigned int start_sector;
	unsigned int end_sector;
	unsigned int nsector;
	unsigned int end_offset = start_offset + byte_size;

	/* Check input bound */
	if (buf == NULL) {
		return ERROR_SSTORAGE_SFS_FREAD;
	}

	/* Open the mtd block device */
	ret = open_blockdriver(devname, 0, &pnode);
	if (ret < 0) {
		fdbg("Fail to open %s errno = %d\n", devname, errno);
		return ERROR_SSTORAGE_SFS_FOPEN;
	}

	ret = pnode->u.i_bops->ioctl(pnode, MTDIOC_GEOMETRY, (unsigned long)&geo);
	if (ret < 0) {
		fdbg("Fail to ioctl %s errno = %d\n", devname, errno);
		ret = ERROR_SSTORAGE_SFS_FOPEN;
		goto read_out;
	}

	/* Calculate the sector number what we sholuld read  */
	start_sector = start_offset / geo.blocksize;
	end_sector = end_offset / geo.blocksize;
	nsector = end_sector - start_sector + ((end_offset % geo.blocksize) ? (1) : (0));

	if (geo.erasesize * geo.neraseblocks < end_offset) {
		ret = ERROR_SSTORAGE_INVALID_DATA_LEN;
		goto read_out;
	}

	/* Allocate temporary read buffer */
	read_buf = (unsigned char *)malloc(nsector * geo.blocksize);
	if (read_buf == NULL) {
		fdbg("Fail to allocate memory\n");
		ret = ERROR_SSTORAGE_SFS_FREAD;
		goto read_out;
	}

	/* Read data */
	ret = pnode->u.i_bops->read(pnode, read_buf, start_sector, nsector);
	if (ret < 0) {
		fdbg("Fail to read start_sector = %d, nsector = %d, errno = %d\n", start_sector, nsector, errno);
		ret = ERROR_SSTORAGE_SFS_FREAD;
	} else {
		memcpy(buf, read_buf + (start_offset % geo.blocksize), byte_size);
		ret = 0;
	}

read_out:
	if (close_blockdriver(pnode)) {
		fdbg("Fail to close errno = %d\n", errno);
		ret = ERROR_SSTORAGE_SFS_FREAD;
	}

	if (read_buf) {
		free(read_buf);
	}
#endif
	return ret;
}

int sss_ro_write(unsigned int start_offset, unsigned char *buf, unsigned int byte_size)
{
	// Do not support yet.
	return OK;
}
