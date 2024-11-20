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

/*#include <tinyara/config.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>*/
#include <unistd.h>
#include <dirent.h>
/*#include <poll.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>*/
#include <tinyara/i2c.h>
#include <tinyara/irq.h>
#include <tinyara/input/touchscreen.h>
#include <tinyara/input/ist415.h>
#include <ist415_update.h>
#include <tinyara/input/240619_AV_Display_V02010206.h>
//#include <tinyara/input/Model_Firmware.h>
//#include <tinyara/input/240626_AV_Display_V02030207.h>
/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/
#define IST415X_TOUCH_STATUS 0x20000000
#define TOUCH_STATUS_MAGIC 0x00000075
#define TOUCH_STATUS_MASK 0x000000FF
#define FIRMWARE_ARRAYNAME ist415x_fw
#define IC_POWEROFF_DELAY 350
#define IC_INITIAL_DELAY 300

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int ist415x_fwdn_checkfw(u32 chipId, unsigned char* fwBinary, unsigned long fwSize)
{
	u32 chipIdAddr;
	u32 fileChipID;
	
	chipIdAddr = CHIP_ID_ADDRESS;
	printf("chip id addr %d %d\n", chipIdAddr , fwSize);
	if (chipIdAddr > (fwSize - sizeof(u32)))
		return -1;
	
	fileChipID = *((u32 *)(fwBinary + chipIdAddr));
	printf("file chip id %d\n", fileChipID);
	
	if (fileChipID != chipId)
		return -1;
	
	return 0;
}

static int ist415x_i2c_isp_write(struct ist415_dev_s *priv, u8 sub1, u8 sub2, u32 addr, const u32 *pData, u32 len)
{
	s32 ret = -1;
	u32 i, j;
	u8 *pBuf = NULL;
	u8 cmd = IST415X_ISP_CMD;
	u32	byteLen;
	
	struct i2c_dev_s *i2c = priv->i2c;
	struct i2c_config_s config = priv->i2c_config;
	//printf("i2c write start\n");
	// ### cmd + regAddr + data length;
	byteLen = (sizeof(cmd) + sizeof(sub1) + sizeof(sub2) + sizeof(addr) + (len * sizeof(u32)));

	pBuf = (u8 *) kmm_calloc(byteLen, sizeof(u8));
	if (pBuf == NULL) {
		printf("mem alloc fail !!\r\n");
		return -1;
	}

	*pBuf = cmd;
	*(pBuf + sizeof(cmd)) = sub1;
	*(pBuf + sizeof(cmd) + sizeof(sub1)) = sub2;

	// ### Align the data for register address.
	for (i = 0; i < sizeof(addr); i++) {
		*(pBuf + (sizeof(cmd) + sizeof(sub1) + sizeof(sub2)) + i) = (u8)((addr >> (((sizeof(u32) - 1) - i) * 8)) & 0xFF);
	}

	// ### Align the data.
	for (i = 0; i < len; i++) {
		for (j = 0; j < sizeof(u32); j++) {
			*(pBuf + ((i * sizeof(u32)) + sizeof(cmd) + sizeof(sub1) + sizeof(sub2) + sizeof(addr)) + j) = (u8)((*(pData + i) >> (((sizeof(u32) - 1) - j) * 8)) & 0xFF);
		}
	}
	//printf("length i2c write...%d %d %d %d\n", sizeof(cmd), sizeof(addr), sizeof(u32), byteLen);
	if(i2c == NULL)
	{
		printf("global i2c is null\n");
		return -1;
	}
	ret = i2c_write(i2c, &config, (uint8_t *)pBuf, byteLen); 
	//ret = Portini2c_WriteByte(IST415X_DEV_ID, pBuf, byteLen, FALSE);
	if (ret != byteLen) {
		printf("i2c write fail !!\r\n");
		return -1;
	}

	if (pBuf) {
		free(pBuf);
	}
	//printf("i2c write end %d\n", ret);
	return ret;
}

static int ist415x_i2c_isp_read(struct ist415_dev_s *priv, u8 sub1, u8 sub2, u32 addr, u32 *pData, u32 len)
{
	s32 ret;
	u32 i, j;
	u8 *sendData;
	u32 sendDataLen;
	u8 cmd = IST415X_ISP_CMD;
	u32 byteLen = (len * sizeof(u32));
	u8 *pBuf;
	u32 tmpData;
	struct i2c_dev_s *i2c = priv->i2c;
	struct i2c_config_s config = priv->i2c_config;
	//printf("i2c read start\n");
	if (len == 0)
		return 0;

	pBuf = (u8 *)kmm_calloc(byteLen, sizeof(u8));
	if (pBuf == NULL) {
		printf("mem alloc fail !!\r\n");
		return -1;
	}

	sendDataLen = sizeof(cmd) + sizeof(sub1) + sizeof(sub2) + sizeof(addr);

	sendData = (u8 *)kmm_calloc(sendDataLen, sizeof(u8));
	if (sendData == NULL) {
		free(pBuf);
		sendData = NULL;
		printf("mem alloc fail !!\r\n");
		return -1;
	}

	*sendData = cmd;
	*(sendData + sizeof(cmd)) = sub1;
	*(sendData + sizeof(cmd) + sizeof(sub1)) = sub2;

	// ### Align the data for register address.
	for (i = 0; i < sizeof(addr); i++) {
		*(sendData + (sizeof(cmd) + sizeof(sub1) + sizeof(sub2)) + i) = (u8)((addr >> (((sizeof(u32) - 1) - i) * 8)) & 0xFF);
	}

	// ### I2C repeat start processing.
	ret = i2c_write(i2c, &config, (uint8_t *)sendData, sendDataLen);
	//ret = Portini2c_WriteByte(IST415X_DEV_ID, sendData, sendDataLen, TRUE);
	if (ret != sendDataLen) {
		printf("i2c write fail !!\r\n");
		return -1;
	} else {
		ret = i2c_read(i2c, &config, pBuf, byteLen);
		//ret = Portini2c_ReadByte(IST415X_DEV_ID, pBuf, byteLen);
		if (ret != byteLen) {
			printf("i2c read fail !!\r\n");
			return -1;
		}
		//printf("i2c read length %d return %d\n", byteLen, ret);

		// ### Align the data.
		for (i = 0; i < len; i++) {
			tmpData = 0;

			for(j=0; j<sizeof(u32); j++) {
				tmpData |= (u32)(*(pBuf + ((i * sizeof(u32)) + j)) << (((sizeof(u32) - 1) - j) * 8));
			}

			*(pData+i) = tmpData;
		}
	}

	if (pBuf) {
		free(pBuf);
		pBuf = NULL;
	}

	if (sendData) {
		free(sendData);
		sendData = NULL;
	}
	//printf("i2c read end %d\n", ret);
	return ret;
}

static int ist415x_i2c_readword(struct ist415_dev_s *priv, u32 regAddr, u32 *pData, u32 len)
{
	s32 ret;
	u32 i, j;
	u8 *sendData;
	u32 sendDataLen;
	u8 cmd = HCOM_SW_DA;
	u32 byteLen = (len * sizeof(u32));
	u8 *pBuf;
	u32 tmpData;
	struct i2c_dev_s *i2c = priv->i2c;
	struct i2c_config_s config = priv->i2c_config;

	if (len == 0)
		return 0;

	pBuf = (u8 *)kmm_calloc(byteLen, sizeof(u8));
	if (pBuf == NULL) {
		printf("mem alloc fail !!\r\n");
		return -1;
	}

	sendDataLen = sizeof(cmd) + sizeof(regAddr);

	sendData = (u8 *)kmm_calloc(sendDataLen, sizeof(u8));
	if (sendData == NULL) {
		free(pBuf);
		sendData = NULL;
		printf("mem alloc fail !!\r\n");
		return -1;
	}

	*sendData = cmd;

	// ### Align the data for register address.
	for (i = 0; i < sizeof(regAddr); i++) {
		*(sendData + sizeof(cmd) + i) = (u8)((regAddr >> (((sizeof(u32) - 1) - i) * 8)) & 0xFF);
	}

	// ### I2C repeat start processing.
	ret = i2c_write(i2c, &config, sendData, sendDataLen);
	//ret = Portini2c_WriteByte(IST415X_DEV_ID, sendData, sendDataLen, TRUE);
	if (ret != sendDataLen) {
		printf("i2c write fail !!\r\n");
		return -1;
	} else {
		ret = i2c_read(i2c, &config, pBuf, byteLen); 
		if (ret != byteLen) {
			printf("i2c read fail !!\r\n");
			return -1;
		}
		printf("pbuf data %d %d\n", pBuf, byteLen);
		// ### Align the data.
		for (i = 0; i < len; i++) {
			tmpData = 0;

			for(j=0; j<sizeof(u32); j++) {
				tmpData |= (u32)(*(pBuf + ((i * sizeof(u32)) + j)) << (((sizeof(u32) - 1) - j) * 8));
			}

			*(pData+i) = tmpData;
		}
	}

	if (pBuf) {
		free(pBuf);
		pBuf = NULL;
	}

	if (sendData) {
		free(sendData);
		sendData = NULL;
	}

	return ret;
}

void ist415x_fwdn_isp_on(struct ist415_dev_s *priv)
{
	ist415x_i2c_isp_write(priv, ISP_TARGET_ENTER, ISP_CMD_ENTER, 0, NULL, 0);

	DelayMs(1);
}

void ist415x_fwdn_isp_off(struct ist415_dev_s *priv)
{
        ist415x_i2c_isp_write(priv, ISP_TARGET_EXIT, ISP_CMD_EXIT, 0, NULL, 0);

        DelayMs(1);
}

static int ist415x_fwdn_comparefirmware(struct ist415_dev_s *priv, u8 *fwBinary, u32 fwDnStartAddr, u32 fwDnSize)
{
	u32 i;
	u32 startAddr;
	u8 aBufR[FLASH_PAGE_SIZE];
	u8 *pBufR, *pFwBin;
	u32 pageCnt;

	pFwBin = fwBinary;

	for (pageCnt = 0; pageCnt < (fwDnSize / FLASH_PAGE_SIZE); pageCnt++) {
		startAddr = fwDnStartAddr + (pageCnt * FLASH_PAGE_SIZE);
		pBufR = aBufR;
		if (ist415x_i2c_isp_read(priv, ISP_TARGET_MAIN, ISP_CMD_READ, startAddr, (u32 *)pBufR, FLASH_PAGE_SIZE / sizeof(u32)) == -1)
			return -1;

		for (i = 0; i < FLASH_PAGE_SIZE; i++) {
			if (*pFwBin++ != *pBufR++) {
				printf("  Fail! Addr:0x%04x, W:0x%02x, R:0x%02x\r\n", startAddr + i, *(pFwBin - 1), *(pBufR - 1));
				return -1;
			}
			//printf(" Pass! Addr:0x%04x, W:0x%02x, R:0x%02x\r\n", startAddr + i, *(pFwBin - 1), *(pBufR - 1));
		}

	}

	printf("Comparison Pass\r\n");

	return 0;
}

static int ist415x_fwdn_calculatecrc(void *pData, u32 size)
{
	u32 i;
	u32 crc = 0;
	u8 *pBuf = (u8*)pData;

	while (size--)
	{
		crc = crc ^ (*pBuf & 0x00FF);

		for (i = 0; i < 8; i++)
		{
			if (crc & 0x01)	{
				crc = (crc >> 1) ^ 0xA001;
			} else {
				crc = crc >> 1;
			}
		}

		pBuf++;
	}

	return crc;
}

static int ist415x_wait_isp(struct ist415_dev_s *priv, u8 waitMs)
{
	u32 status = 0;
	
	while (1) {
		DelayMs(1);
		ist415x_i2c_isp_read(priv, ISP_TARGET_STATUS, ISP_CMD_READ, 0, &status, 1);
		//printf("status %d\n", status);
		if (status == ISP_STATUS_DONE)
			break;
	
		if (waitMs-- == 0)
			return -1;
	}
	
	return 0;
}

static int ist415x_fwdn_getcrc(struct ist415_dev_s *priv, u32 startAddr, u32 size)
{
	u32 crc;

	if (ist415x_i2c_isp_write(priv, ISP_TARGET_MAIN, ISP_CMD_RUN_CRC, startAddr, &size, 1) == -1) {
		printf("crc run write error !!\r\n");
		return 0;
	}

	if (ist415x_wait_isp(priv, 30)) {
		printf("wait isp error !!\r\n");
		return 0;
	}

	if (ist415x_i2c_isp_read(priv, ISP_TARGET_MAIN, ISP_CMD_READ_CRC, 0, &crc, 1) == -1) {
		printf("read crc error !!\r\n");
		return 0;
	}

	return crc;
}

static int ist415x_fwdn_erasefirmware(struct ist415_dev_s *priv)
{
	u32 i;
	printf("flash total size %d , flash sector size %d\n", FLASH_TOTAL_SIZE, FLASH_SECTOR_SIZE);
	for (i = 0; i < (FLASH_TOTAL_SIZE / FLASH_SECTOR_SIZE); i++) {
		if (ist415x_i2c_isp_write(priv, ISP_TARGET_MAIN, ISP_CMD_SECTOR_ERASE, i * FLASH_SECTOR_SIZE, NULL, 0) == -1) {
			printf("error !!\r\n");
			return -1;
		}

		if (ist415x_wait_isp(priv, 30)) {
			return -1;
		}
	}

	return 0;
}

static int ist415x_fwdn_write(struct ist415_dev_s *priv, u32 flashAddr, u32 *pData, u32 sizeByte)
{
	u32 i;

	printf("  0x%04x", flashAddr);

	for (i = 0; i < (sizeByte / FLASH_SECTOR_SIZE); i++) {
		printf("\b\b\b\b\b\b\b\b\b\b\b\b");
		printf("  0x%08X    %08X\n", flashAddr, *pData);				// Progress Status

		if (ist415x_i2c_isp_write(priv, ISP_TARGET_MAIN, ISP_CMD_PROG, flashAddr, pData, FLASH_SECTOR_SIZE / sizeof(u32)) == -1) {
			printf("error !!\r\n");
			return -1;
		}

		flashAddr += FLASH_SECTOR_SIZE;
		pData += (FLASH_SECTOR_SIZE / sizeof(u32));
	}
	printf("\r\n");

	return 0;
}

static int ist415x_fwdn_writefirmware(struct ist415_dev_s *priv, u8 *fwBinary, u32 fwDnStartAddr, u32 fwDnSize, u32 fwDnChecksum)
{
        u32 getCrc;
	int ret = 0;
        // => Code Write
        ret = ist415x_fwdn_write(priv, fwDnStartAddr, (u32 *)fwBinary, fwDnSize);
	if (ret == -1) {
		printf("Firmware Write fail\n");
		return ret;	
	}
        // => Check CRC
        getCrc = ist415x_fwdn_getcrc(priv, fwDnStartAddr, fwDnSize);

        if (getCrc == fwDnChecksum) {
                printf("  Pass\r\n");
        } else {
                printf("  Fail! Write_CRC, Get_CRC : [0x%08X], [0x%08X]\r\n", fwDnChecksum, getCrc);
                return -1;
        }

        return ret;
}

/*int32_t read_firmware()
{
	char cwd[50];
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("Current working dir: %s\n", cwd);
   } else {
       printf("getcwd() error");
       return 1;
   }
	DIR *d;
  struct dirent *dir;
  d = opendir("/");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      printf("%s\n", dir->d_name);
    }
    closedir(d);
  }
   FILE *ptr;
	int ret = -1;
	char path[50] = {0};
	uint8_t buffer[20];
	int len = 16;
	sprintf(path, "%s", "240619_AV_Display_V02010206.bin");
	printf("file path %s\n", path);
	ptr = fopen(path, "rb");
	if (ptr == NULL) {
		printf("fopen failed\n");
		return ret;
	}

	ret = fread(buffer, len, 1, ptr);
	if (ret < 0) {
		printf("fread failed\n");
	} else {
		for(int i = 0; i<len; i++)
    			printf("%u ", buffer[i]);
	}
	fclose(ptr);
	return ret;
}*/

static int ist415x_ctrl_hw_reset(void)
{
	//Porting_IC_PowerOff(); - gpio
	//disable interrupt
	DelayMs(IC_POWEROFF_DELAY);

	//Porting_IC_PowerOn(); - gpio
	//enable interrupt
	DelayMs(IC_INITIAL_DELAY);

	/*int reg[1];
	reg[0] = HCOM_SENSE_OFF;
	if (i2c_write(i2c, &config, (uint8_t *)reg, 1) != 1) {
		printf("Error : F/W Hold failed after H/W Reset\r\n");
		return -1;
	}*/
	printf("hw sense off write success\n"); 
	DelayMs(1);

	return 0;
}

static int check_status(struct ist415_dev_s *priv)
{
	u32 touch_status = 0;

	int ret = ist415x_i2c_readword(priv, IST415X_TOUCH_STATUS, &touch_status, 1);
	printf("touch info 0x%08x\n", touch_status);

	if ((touch_status & TOUCH_STATUS_MASK) != TOUCH_STATUS_MAGIC) {
		printf("Touch status is not corrected (0x%08x)\n", touch_status);
	}
}

int ist415x_fwdn_fwupdate(u32 chipId, FAR struct ist415_dev_s *priv)
{
	u32 fwDnChecksum;
	//IST415x_FWDN_ISP_On();

	// App
	unsigned char *fwBinary = (unsigned char *)NULL;
	unsigned long fwSize = 0;
	fwBinary = FIRMWARE_ARRAYNAME;
	fwSize = sizeof(FIRMWARE_ARRAYNAME);
	//check_status();
	// => Check chipId
	printf("[Check Firmware]\r\n");
	if (ist415x_fwdn_checkfw(chipId, fwBinary, fwSize)) {
		printf("\tError : Invalid FW file, chipId=%u\r\n", chipId);
		goto FWDN_ERROR;
	}

	fwDnChecksum = ist415x_fwdn_calculatecrc(fwBinary, FLASH_TOTAL_SIZE);
	
	printf("check sum %d\n", fwDnChecksum);
	//check_status();
	ist415x_ctrl_hw_reset();
	
	printf("hw reset done\n");
	// Set ISP Mode
	ist415x_fwdn_isp_on(priv);
	printf("isp mode set\n");
	//check_status();
	// => Erase ROM
	printf("[ROM Erase]\r\n");

	if (ist415x_fwdn_erasefirmware(priv)) {
		printf("\tError Erasing ROM\r\n");
		goto FWDN_ERROR;
	}
	//check_status();
	// => Write App
	printf("[FW Write] %8x %d\r\n", FLASH_ADDR , FLASH_TOTAL_SIZE);
	if (ist415x_fwdn_writefirmware(priv, fwBinary, FLASH_ADDR, FLASH_TOTAL_SIZE, fwDnChecksum)) {
		printf("\tError Writing App\r\n");
		goto FWDN_ERROR;
	}

	// => App Read & Compare
	printf("[FW Read & Compare]\r\n");
	if (ist415x_fwdn_comparefirmware(priv, fwBinary, FLASH_ADDR, FLASH_TOTAL_SIZE)) {
		printf("\tError Comparing App\r\n");
		goto FWDN_ERROR;
	}
	//check_status();
	return 0;

FWDN_ERROR:

	return -1;
}

