/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <tinyara/fs/fs.h>
#include <tinyara/fs/mtd.h>
#include <tinyara/fs/ioctl.h>

#include "isp_custom.h"
/*
 * Definition for handling pthread
 */
#define SSSRO_CHECK_OFFSET	0x1000
#define SSSRO_MAGIC		0x10186068
#define SSSRO_ENDMARK		0xFD
#define SSSRO_CHECKED		0x01
#define SSSRO_CHECK_SIZE	6

#define SSSRO_CHK_DEVICE_NAME	"/dev/mtdblock7"

#define SSSRO_CHK_OK	0
#define SSSRO_CHK_FAIL	-1

struct sssro_check {
	unsigned int magic;
	unsigned char check_data;
	unsigned char endmark;
};

int read_sssro_check(unsigned char *buf, unsigned int byte_size)
{
	int ret = 0;
	struct mtd_geometry_s geo;

	FAR struct inode *pnode = NULL;
	char *devname = SSSRO_CHK_DEVICE_NAME;
	unsigned char *read_buf = NULL;
	unsigned int start_sector;

	if (buf == NULL) {
		return SSSRO_CHK_FAIL;
	}

	ret = open_blockdriver(devname, 0, &pnode);
	if (ret < 0) {
		fdbg("Fail to open %s err = %d\n", devname, ret);
		return ret;
	}

	ret = pnode->u.i_bops->ioctl(pnode, MTDIOC_GEOMETRY, (unsigned long)&geo);
	if (ret < 0) {
		fdbg("Fail to ioctl %s err = %d\n", devname, ret);
		goto read_out;
	}

	read_buf = (unsigned char *)malloc(geo.blocksize);
	if (read_buf == NULL) {
		fdbg("Fail to allocate memory\n");
		ret = SSSRO_CHK_FAIL;
		goto read_out;
	}

	start_sector = SSSRO_CHECK_OFFSET / geo.blocksize;

	ret = pnode->u.i_bops->read(pnode, read_buf, start_sector, 1);
	if (ret < 0) {
		fdbg("Fail to read start_sector = %d, err = %d\n",
							start_sector, ret);
	} else {
		memcpy(buf, read_buf, byte_size);
		ret = SSSRO_CHK_OK;
	}

read_out:
	if (close_blockdriver(pnode)) {
		fdbg("Fail to close %s err = %d\n", devname, ret);
		ret = SSSRO_CHK_FAIL;
	}

	if (read_buf) {
		free(read_buf);
	}

	return ret;
}

int write_sssro_check(unsigned char *buf, unsigned int byte_size)
{
	int ret = 0;
	struct mtd_geometry_s geo;
	FAR struct inode *pnode = NULL;
	char *devname = SSSRO_CHK_DEVICE_NAME;
	unsigned int start_sector;

	if (buf == NULL) {
		return SSSRO_CHK_FAIL;
	}

	ret = open_blockdriver(devname, 0, &pnode);
	if (ret < 0) {
		fdbg("Fail to open %s err = %d\n", devname, ret);
		return ret;
	}

	ret = pnode->u.i_bops->ioctl(pnode, MTDIOC_GEOMETRY, (unsigned long)&geo);
	if (ret < 0) {
		fdbg("Fail to ioctl %s err = %d\n", devname, ret);
		goto write_out;
	}

	start_sector = SSSRO_CHECK_OFFSET / geo.blocksize;

	ret = pnode->u.i_bops->write(pnode, buf, start_sector, 1);
	if (ret < 0) {
		fdbg("Fail to write start_sector = %d, err = %d\n",
							start_sector, ret);
	} else {
		ret = SSSRO_CHK_OK;
	}

write_out:
	if (close_blockdriver(pnode)) {
		fdbg("Fail to close %s err = %d\n", devname, ret);
		ret = SSSRO_CHK_FAIL;
	}

	return ret;
}

int sssro_verify(void)
{
	int ret = 0;
	struct sssro_check data;
	unsigned int hash_size = 32;
	unsigned char hash[32] = { 0x01, };
	unsigned char r[68];
	unsigned char s[68];
	struct sECC_SIGN ecc_sign;

	ret = read_sssro_check((unsigned char *)&data, sizeof(struct sssro_check));
	if (ret) {
		goto err;
	}

	if ((data.magic == SSSRO_MAGIC) && (data.check_data == SSSRO_CHECKED)
				&& (data.endmark == SSSRO_ENDMARK)) {
		return SSSRO_CHK_OK;
	}

	data.magic = SSSRO_MAGIC;
	data.check_data = SSSRO_CHECKED;
	data.endmark = SSSRO_ENDMARK;

	memset(&ecc_sign, 0, sizeof(struct sECC_SIGN));

	ecc_sign.sign_type = OID_ECDSA_P256_SHA2_256;
	ecc_sign.r = r;
	ecc_sign.s = s;

	/*
	 * Verify ARTIK key
	 */
	ret = isp_ecdsa_sign_md_securekey(&ecc_sign, hash, hash_size,
						FACTORYKEY_ARTIK_DEVICE);
	if (ret) {
		goto err;
	}

	ret = isp_ecdsa_verify_md_securekey(&ecc_sign, hash, hash_size,
						FACTORYKEY_ARTIK_DEVICE);
	if (ret) {
		goto err;
	}

	ret = write_sssro_check((unsigned char *)&data, sizeof(struct sssro_check));
	if (ret) {
		goto err;
	}
	dbg("PASS\n");

	return SSSRO_CHK_OK;

err:
	dbg("FAIL (%d)\n", ret);

	return ret;
}
