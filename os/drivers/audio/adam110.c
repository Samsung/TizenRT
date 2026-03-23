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
/*******************************************************************************
* Modifications:
* Copyright 2026 ABOV Semiconductor Co., Ltd.
*
* This file has been modified by ABOV Semiconductor.
*
* Modifications include:
*   - Implementing the interface logic for the ADAM110
*
* Modifications by ABOV Semiconductor are licensed under the BSD-3-Clause license.
* The original portions of this file remain licensed under the Apache License, Version 2.0.
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation and/or
* other materials provided with the distribution.
* 
 * 3. Neither the name of the copyright holder nor the names of its contributors
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
* 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <tinyara/audio/i2s.h>
#include <tinyara/gpio.h>
#include <tinyara/kmalloc.h>
#include <tinyara/kthread.h>
#include <tinyara/wqueue.h>
#include <tinyara/audio/adam110.h>

#include "adam110.h"

#ifdef CONFIG_PM
#include <tinyara/pm/pm.h>
#endif


/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int adam110_getcaps(FAR struct audio_lowerhalf_s *dev, int type, FAR struct audio_caps_s *caps);
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int adam110_configure(FAR struct audio_lowerhalf_s *dev, FAR void *session, FAR const struct audio_caps_s *caps);
#else
static int adam110_configure(FAR struct audio_lowerhalf_s *dev, FAR const struct audio_caps_s *caps);
#endif
static int adam110_shutdown(FAR struct audio_lowerhalf_s *dev);

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int adam110_start(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int adam110_start(FAR struct audio_lowerhalf_s *dev);
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int adam110_stop(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int adam110_stop(FAR struct audio_lowerhalf_s *dev);
#endif
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int adam110_pause(FAR struct audio_lowerhalf_s *dev, FAR void *session);
static int adam110_resume(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int adam110_pause(FAR struct audio_lowerhalf_s *dev);
static int adam110_resume(FAR struct audio_lowerhalf_s *dev);
#endif
#endif
static int adam110_enqueuebuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb);
static int adam110_cancelbuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb);
static int adam110_ioctl(FAR struct audio_lowerhalf_s *dev, int cmd, unsigned long arg);
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int adam110_reserve(FAR struct audio_lowerhalf_s *dev, FAR void **session);
#else
static int adam110_reserve(FAR struct audio_lowerhalf_s *dev);
#endif
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int adam110_release(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int adam110_release(FAR struct audio_lowerhalf_s *dev);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct audio_ops_s g_audioops = {
	adam110_getcaps,                /* getcaps        */
	adam110_configure,              /* configure      */
	adam110_shutdown,               /* shutdown       */
	adam110_start,                  /* start          */
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
	adam110_stop,                   /* stop           */
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
	adam110_pause,                  /* pause          */
	adam110_resume,                 /* resume         */
#endif
	NULL,                           /* allocbuffer    */
	NULL,                           /* freebuffer     */
	adam110_enqueuebuffer,          /* enqueue_buffer */
	adam110_cancelbuffer,           /* cancel_buffer  */
	adam110_ioctl,                  /* ioctl          */
	NULL,                           /* read           */
	NULL,                           /* write          */
	adam110_reserve,                /* reserve        */
	adam110_release                 /* release        */
};

static struct adam110_dev_s *g_adam110;
#ifdef CONFIG_PM
static void adam110_pm_notify(struct pm_callback_s *cb, enum pm_state_e pmstate);
static int adam110_pm_prepare(struct pm_callback_s *cb, enum pm_state_e pmstate);

static struct pm_callback_s g_pmndpcb =
{
	.notify  = adam110_pm_notify,
	.prepare = adam110_pm_prepare,
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static uint8_t adam110_calculate_checksum(uint8_t *pkt, uint32_t size);
static void adam110_spi_exchange(FAR struct adam110_dev_s *dev, uint8_t *txpkt, int txlen, uint8_t *rxpkt, int rxlen);
static int adam110_verify_packet(t_proto_pkt *pkt);
static int adam110_spi_command_xfer(FAR struct adam110_dev_s *dev, t_proto_pkt *txpkt, t_proto_pkt *rxpkt);
static int adam110_send_cmd(FAR struct adam110_dev_s *dev, uint8_t op,
                                    uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4,
                                    t_proto_pkt *rx_out);


#define adam110_givesem(s)    sem_post(s)

#define ADAM110_RESET(dev, rx_ptr)           adam110_send_cmd(dev, DM_RESET, 0, 0, 0, 0, rx_ptr)
#define ADAM110_GET_KEEP_ALIVE(dev, rx_ptr)  adam110_send_cmd(dev, DM_GET_KEEP_ALIVE, 0, 0, 0, 0, rx_ptr)
#define ADAM110_GET_STATE(dev, rx_ptr)       adam110_send_cmd(dev, DM_GET_STATE, 0, 0, 0, 0, rx_ptr)

#define ADAM110_GET_FW_VER(dev, rx_ptr) \
    adam110_send_cmd(dev, FW_GET_VER, 0, 0, 0, 0, rx_ptr)

#define ADAM110_GET_EVENT(dev, rx_ptr) \
    adam110_send_cmd(dev, DM_GET_EVENT, 0, 0, 0, 0, rx_ptr)

#define ADAM110_GET_SEAMLESS(dev, rx_ptr) \
    adam110_send_cmd(dev, AUD_GET_SEAMLESS_DATA, 0, 0, 0, 0, rx_ptr)

#define ADAM110_GET_AUDIO(dev, rx_ptr) \
    adam110_send_cmd(dev, AUD_GET_PREP_DATA, 0, 0, 0, 0, rx_ptr)

#define ADAM110_AI_CHECK_XMIT(dev, h_size, l_size, rx_ptr) \
    adam110_send_cmd(dev, AC_UPDATE_CHK_XMIT, h_size, l_size, 0, 0, rx_ptr)

#define ADAM110_SET_I2S(dev, rate, width, rx_ptr) \
    adam110_send_cmd(dev, AUD_I2S_SET_CONFIG, (uint8_t)(rate), (uint8_t)(width), 0, 0, rx_ptr)

#define ADAM110_SET_AEC(dev, enable, rx_ptr) \
    adam110_send_cmd(dev, AUD_I2S_AEC_CTRL, (uint8_t)(enable), 0, 0, 0, rx_ptr)

#define ADAM110_AI_UPDATE_START(dev, rx_ptr) \
    adam110_send_cmd(dev, AC_UPDATE_MODEL, 0, 0, 0, 0, rx_ptr)

#define ADAM110_AI_UPDATE_RSLT(dev, rx_ptr) \
    adam110_send_cmd(dev, AC_UPDATE_RSLT, 0, 0, 0, 0, rx_ptr)

#define ADAM110_SET_MODEL(dev) \
	adam110_send_model(dev)

#define ADAM110_SET_FIRMWARE(dev) \
	adam110_send_firmware(dev)

#define ADAM110_GET_AUDIOBUFFER(dev, type, buf_ptr, size) \
	adam110_get_audiobuffer(dev, type, buf_ptr, size)

#define ADAM110_SET_INTR(dev, data, enable, rx_ptr) \
    adam110_send_cmd(dev, AUD_INT_EN, (uint8_t)(data), (uint8_t)(enable), 0, 0, rx_ptr)

#define ADAM110_AI_SET_THD(dev, thd_level, rx_ptr) \
    adam110_send_cmd(dev, AC_THD_ADJ, (uint8_t)(thd_level), 0, 0, 0, rx_ptr)

#define ADAM110_AI_SET_INTR(dev, model, enable, rx_ptr) \
    adam110_send_cmd(dev, AC_INT_EN, (uint8_t)(model), (uint8_t)(enable), 0, 0, rx_ptr)

#define ADAM110_SET_FW_BOOT_MODE(dev, rx_ptr) \
    adam110_send_cmd(dev, FW_ENTER_BOOT_MODE, 0, 0, 0, 0, rx_ptr)

#define ADAM110_GET_FW_BOOT_MODE(dev, rx_ptr) \
    adam110_send_cmd(dev, FW_CHK_BOOT_MODE, 0, 0, 0, 0, rx_ptr)

#define ADAM110_SET_FW_ERASE(dev, rx_ptr) \
    adam110_send_cmd(dev, FW_ERASE, 0, 0, 0, 0, rx_ptr)

#define ADAM110_SET_FW_UPDATE(dev, rx_ptr) \
    adam110_send_cmd(dev, FW_UPDATE, 0, 0, 0, 0, rx_ptr)

#define ADAM110_SET_FW_CHECKSUM(dev, chk_h, chk_l, rx_ptr) \
    adam110_send_cmd(dev, FW_UPDATE_RSLT_CAL, chk_h, chk_l, 0, 0, rx_ptr)

#define ADAM110_GET_FW_CHECKSUM(dev, rx_ptr) \
    adam110_send_cmd(dev, FW_UPDATE_RSLT, 0, 0, 0, 0, rx_ptr)

static void adam110_spi_lock(FAR struct adam110_dev_s *dev)
{
	(void)SPI_LOCK(dev->spi, true);

	SPI_SETMODE(dev->spi, dev->lower->spi_config.mode);
	SPI_SETFREQUENCY(dev->spi, dev->lower->spi_config.freq);
	SPI_SETBITS(dev->spi, dev->lower->spi_config.bpw);
}

static void adam110_spi_unlock(FAR struct adam110_dev_s *dev)
{
	(void)SPI_LOCK(dev->spi, false);
}

static void adam110_spi_exchange(FAR struct adam110_dev_s *dev, uint8_t *txpkt, int txlen, uint8_t *rxpkt, int rxlen)
{
	adam110_spi_lock(dev);
	SPI_SELECT(dev->spi, 0, true);
	if (txpkt != NULL) {
		SPI_SNDBLOCK(dev->spi, (void *)txpkt, txlen);
		up_udelay(ADAM110_TXRX_DELAY);
	}
	
	if (rxpkt != NULL) {
		SPI_RECVBLOCK(dev->spi, (void *)rxpkt, rxlen);
	}

	SPI_SELECT(dev->spi, 0, false);
	adam110_spi_unlock(dev);
}

static uint8_t adam110_calculate_checksum(uint8_t *data, uint32_t size) 
{
	uint8_t checksum = 0;
	for (uint32_t i = 0; i < size; i++) {
		checksum += data[i];
	}
	return checksum;
}

static uint16_t adam110_calculate_checksum16(int fd)
{
	uint16_t checksum = 0;
    uint32_t bytes_read_total = 0;
    uint8_t buffer[128];
    ssize_t nread;

    while ((nread = read(fd, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < nread; i++) {
            checksum += (uint16_t)buffer[i];
        }
        bytes_read_total += nread;
    }

    if (ADAM110_FLASH_TOTAL_SIZE > bytes_read_total) {
        uint32_t padding_count = ADAM110_FLASH_TOTAL_SIZE - bytes_read_total;
        checksum += (uint16_t)(padding_count * 0xFF);
    }

    return checksum;

}

static int adam110_verify_packet(t_proto_pkt *pkt) 
{
	if (pkt->header != PKT_HEADER_RECV) {
		auddbg("[E] pkt rxheader: 0x%x\n", pkt->header);
		return RSLT_SYNC_ERR;
	}

	uint8_t cal_checksum = adam110_calculate_checksum((uint8_t *)pkt, sizeof(t_proto_pkt) - 1);
	if (cal_checksum != pkt->checksum) {
		auddbg("[E] pkt chsum : 0x%x|0x%x|0x%x|0x%x|0x%x|0x%x|0x%x\n", pkt->header, pkt->op, pkt->parm1, pkt->parm2, pkt->parm3, pkt->parm4, pkt->checksum);
	return RSLT_CHKSUM_ERR;
	}

	return RSLT_SUCCESS;
}

static int adam110_spi_command_xfer(FAR struct adam110_dev_s *dev, t_proto_pkt *txpkt, t_proto_pkt *rxpkt)
{
	txpkt->header = PKT_HEADER_SEND;
	txpkt->checksum = adam110_calculate_checksum((uint8_t *)txpkt, sizeof(t_proto_pkt) - 1);

	adam110_spi_exchange(dev, (uint8_t *)txpkt, sizeof(t_proto_pkt),
                              (uint8_t *)rxpkt, (rxpkt != NULL) ? sizeof(t_proto_pkt) : 0);

	if (rxpkt == NULL) {
		return RSLT_SUCCESS;
	}

	int result = adam110_verify_packet(rxpkt);
	if (result != RSLT_SUCCESS) {
		auddbg("[E] packet verification : %d\n", result);
		return result;
	}

	if (rxpkt->op == RSLT_ERROR) {
		auddbg("[E] result : %d\n", result);
		return -EIO;
	}

	return RSLT_SUCCESS;
}

static int adam110_send_cmd(FAR struct adam110_dev_s *dev, uint8_t op,
                            uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4,
                            t_proto_pkt *rx_out)
{
	t_proto_pkt tx;

	memset(&tx, 0, sizeof(t_proto_pkt));
	tx.header = PKT_HEADER_SEND;
	tx.op     = op;
	tx.parm1  = p1;
	tx.parm2  = p2;
	tx.parm3  = p3;
	tx.parm4  = p4;

	return adam110_spi_command_xfer(dev, &tx, rx_out);
}

static int adam110_send_model(FAR struct adam110_dev_s *dev)
{
	t_proto_pkt rxpkt;
	int fd;
	ssize_t nread;
	uint32_t sent_size = 0;
	uint8_t chunk_buf[ADAM110_MODEL_CHUNK_SIZE + 1]; // 256 + 1 (checksum)
	int retry_remain;

	fd = open(dsp_package, O_RDONLY);
	if (fd < 0) {
		auddbg("[E] Failed to open DSP package: %s (errno: %d)\n", dsp_package, errno);
		return -ENOENT;
	}

	if (ADAM110_AI_UPDATE_START(dev, &rxpkt) != RSLT_SUCCESS) {
		auddbg("[E] Update start failed.\n");
		close(fd);
		return -EINVAL;
	}

	up_udelay(ADAM110_TXRX_DELAY * 100);
	while (1) {
		nread = read(fd, chunk_buf, ADAM110_MODEL_CHUNK_SIZE);
		if (nread == 0) {
			break; 
		}
        
		if (nread < 0) {
			auddbg("[E] File read error : (errno: %d)\n", errno);
			close(fd);
			return -EIO;
		}

		uint8_t p1, p2;
		if (nread == ADAM110_MODEL_CHUNK_SIZE) {
			p1 = 0x01;
			p2 = 0x00;
		} else {
			uint16_t actual_len = (uint16_t)(nread);
			p1 = (uint8_t)((actual_len >> 8) & 0xFF);
			p2 = (uint8_t)(actual_len & 0xFF);
		}

		retry_remain = ADAM110_MODEL_RETRY_CNT;
		while (retry_remain > 0) {
			int ret = ADAM110_AI_CHECK_XMIT(dev, p1, p2, &rxpkt);

			if (rxpkt.op == RSLT_SUCCESS) {
				chunk_buf[nread] = adam110_calculate_checksum(chunk_buf, nread);
				adam110_spi_exchange(dev, chunk_buf, nread + 1, NULL, 0);
				sent_size += nread;
				up_udelay(ADAM110_TXRX_DELAY);
				break;
			} else if (rxpkt.op == RSLT_BUF_FULL) {
				retry_remain--;
				if (retry_remain <= 0) {
					auddbg("[E] model download timeout.\n");
					close(fd);
					return -ETIMEDOUT;
				}
				up_udelay(ADAM110_TXRX_DELAY * 300);
			} else {
				auddbg("[E] model xmit ready failed : %d.\n", ret);
				close(fd);
				return -EIO;
			}
		}
	}
	close(fd);

	up_udelay(10*1000);

	if (ADAM110_AI_UPDATE_RSLT(dev, &rxpkt) != RSLT_SUCCESS) {
		auddbg("[E] Model update result failed.\n");
		return -EINVAL;
	}

	auddbg("[I] Model update success (Total: %u bytes)\n", sent_size);

	return OK;
}

static int adam110_send_firmware(FAR struct adam110_dev_s *dev)
{

	t_proto_pkt rxpkt;
	uint32_t sent_size = 0, fw_size = 0;
	int fd;
	struct stat st;
	ssize_t nread;
	uint8_t fw_buf[ADAM110_FW_HEADER_SIZE];
	uint8_t chunk_buf[ADAM110_FW_CHUNK_SIZE + 2]; //2(header+command) +128
	int ret = OK;
	uint32_t fw_ver;

	/* check f/w version */
	ret = ADAM110_GET_FW_VER(dev, &rxpkt);
	if (ret != RSLT_SUCCESS) {
		return -EINVAL;
	} 
	
    fw_ver = rxpkt.parm1 << 24 | rxpkt.parm2 << 16 | rxpkt.parm3 << 8 | rxpkt.parm4;
	auddbg("FW ver:0x%8x\n",fw_ver);

	fd = open(mcu_package, O_RDONLY);
	if (fd < 0) {
		auddbg("[E] Failed to open MCU package: %s (errno: %d)\n", mcu_package, errno);
		return -ENOENT;
	}

	if (fstat(fd, &st) != 0) {
		close(fd);
		return -EINVAL;
	}

	fw_size = (uint32_t)st.st_size - ADAM110_FW_HEADER_SIZE;

	nread = read(fd, fw_buf, ADAM110_FW_HEADER_SIZE);
	if (nread == 0) {
		auddbg("[E] Failed to read MCU package header: %s (errno: %d)\n", mcu_package, errno);
		close(fd);
		return -ENOENT;
	}

	/* check vendor id */
    if (ADAM110_FW_VENDOR_ID != fw_buf[ADAM110_FW_HEADER_VENDOR_OFFSET]) {
		auddbg("[E] Wrong F/W.\n");
		close(fd);
		return -EINVAL;
	}

	/* check fw version */
	uint32_t img_fw_ver = fw_buf[ADAM110_FW_HEADER_VER_MAJOR_OFFSET] << 24 | \
						  fw_buf[ADAM110_FW_HEADER_VER_MINOR_OFFSET] << 16 | \
						  fw_buf[ADAM110_FW_HEADER_VER_PATCH_H_OFFSET] << 8 | \
						  fw_buf[ADAM110_FW_HEADER_VER_PATCH_L_OFFSET];
	if (fw_ver == img_fw_ver) {
		auddbg("[I] Same F/W.\n");
		close(fd);
		return OK;
	}

	auddbg("IMG FW ver:0x%8x\n",img_fw_ver);

	/* check new fw image checksum16 */
	uint16_t img_header_checksum = fw_buf[ADAM110_FW_HEADER_CHKSUM_H_OFFSET] << 8 | \
								   fw_buf[ADAM110_FW_HEADER_CHKSUM_L_OFFSET];
    uint16_t img_checksum = adam110_calculate_checksum16(fd);
	if (img_header_checksum != img_checksum) {
		auddbg("[E] F/W image checksum failed.(0x%8x:0x%8x)\n", img_header_checksum, img_checksum);
		close(fd);
		return -EINVAL;
	}

	if (fw_ver != 0xB004C0DE) {
		ret = ADAM110_SET_FW_BOOT_MODE(dev, &rxpkt);
		if (ret != RSLT_SUCCESS) {
			auddbg("[E] Enter boot mode failed.\n");
			close(fd);
			return -EINVAL;
		}

		up_udelay(100*1000);
		g_adam110->lower->reset();
		up_udelay(500*1000);
	}

	ret = ADAM110_GET_FW_BOOT_MODE(dev, &rxpkt);
	if ((ret != RSLT_SUCCESS) ||
		(rxpkt.parm1 << 24 | rxpkt.parm2 << 16 | rxpkt.parm3 << 8 | rxpkt.parm4) != ADAM110_FW_BOOT_MODE_OK) {
		auddbg("[E] Enter boot mode failed.\n");
		close(fd);
		return -EINVAL;
	}

	ret = ADAM110_SET_FW_ERASE(dev, &rxpkt);
	if (ret != RSLT_SUCCESS) {
		auddbg("[E] Erase failed.\n");
		close(fd);
		return -EINVAL;
	}

	up_udelay(ADAM110_FW_ERASE_WAITTIME);

	if (lseek(fd, ADAM110_FW_HEADER_SIZE, SEEK_SET) == (off_t)-1) {
		auddbg("[E] Failed to seek to data start (16B).\n");
		close(fd);
	    return -EIO;
	}

	while (sent_size < fw_size) {
		ret = ADAM110_SET_FW_UPDATE(dev, &rxpkt);
		if (ret != RSLT_SUCCESS) {
			close(fd);
			return -EINVAL;
		}

		up_udelay(ADAM110_FW_UPDATE_WAITTIME);

		if (rxpkt.op == RSLT_SUCCESS) {
			memset(chunk_buf, 0xff, sizeof(chunk_buf));
			chunk_buf[0] = PKT_HEADER_SEND;
			chunk_buf[1] = FW_GET_UPDATE_DATA;
			nread = read(fd, &chunk_buf[ADAM110_FW_DATA_HEADER], ADAM110_FW_CHUNK_SIZE);
	        if (nread < 0) {
				auddbg("[E] File read error.\n");
				close(fd);
	            return -EIO;
			}
			adam110_spi_exchange(dev, chunk_buf, ADAM110_FW_CHUNK_SIZE + ADAM110_FW_DATA_HEADER, NULL, 0); /* must send 130byte (under 128byte need padding) */
			sent_size += nread;
			up_udelay(ADAM110_FW_WRITE_WAITTIME);
		} else {
			auddbg("[E] FW write failed.\n");
			return -EIO;
		}
	}

	close(fd);

	/* checsum */
	ret = ADAM110_SET_FW_CHECKSUM(dev, (uint8_t)(img_checksum >> 8 & 0xFF), (uint8_t)(img_checksum & 0xFF), &rxpkt);
	if (rxpkt.op != RSLT_SUCCESS) {
		auddbg("[E] FW chsum re failed.\n");
		return -EINVAL;
	}

	up_udelay(ADAM110_FW_CHKSUM_WAITTIME);

	ret = ADAM110_GET_FW_CHECKSUM(dev, &rxpkt);
	if (rxpkt.op != RSLT_SUCCESS) {
		auddbg("[E] Get FW chsum re failed.\n");
		return -EINVAL;
	}

	if (rxpkt.parm3 == ADAM110_FW_CHKSUM_FAIL) {
		auddbg("[E] FW chsum failed.\n");
		return -EINVAL;
	}

	auddbg("[I] F/W updated (0x%x.0x%x.0x%x)\n", (uint8_t)(img_fw_ver >> 24), (uint8_t)(img_fw_ver >> 16), (uint16_t)(img_fw_ver & 0xFFFF));
	return OK;
}

static int adam110_get_audiobuffer(FAR struct adam110_dev_s *dev, ai_data_type_t type, uint8_t *buffer, uint32_t size)
{
	int ret = 0;
	switch(type) {
		case AI_DATA_TYPE_SEAMLESS:
			ret = ADAM110_GET_SEAMLESS(dev, NULL);
			break;
		case AI_DATA_TYPE_AUDIO:
			ret = ADAM110_GET_AUDIO(dev, NULL);
			break;
		default:
			auddbg("[E] invalid data type\n");
			ret = -EINVAL;

	}

	if (ret == RSLT_SUCCESS) {
		up_udelay(ADAM110_TXRX_DELAY);
		adam110_spi_exchange(dev, NULL, 0, buffer, size);
	}

	return ret;
}

static FAR struct ap_buffer_s *pcm_waitq_take_one(FAR struct adam110_dev_s *priv)
{
	struct ap_buffer_s *apb;

	apb = (struct ap_buffer_s *)sq_remfirst(&priv->pendq);
	return apb;
}

/************************************************************************************
 * Name: adam110_takesem
 *
 * Description:
 *  Take a semaphore count, handling the nasty EINTR return if we are interrupted
 *  by a signal.
 *
 ************************************************************************************/
static void adam110_takesem(sem_t *sem)
{
	int ret;
	do 
	{
		ret = sem_wait(sem);
		DEBUGASSERT(ret == 0 || errno == EINTR);
	} while (ret < 0);
}

#ifdef CONFIG_AUDIO_ADAM110_ALIVE_CHECK
static int adam110_check_aliveness(FAR struct adam110_dev_s *priv)
{
	int ret = OK;
	int retry = ADAM110_RETRY_CNT;
	t_proto_pkt rxpkt;

	/* check keep alive from adam110 */
	while (retry-- > 0) {
		if (ADAM110_GET_KEEP_ALIVE(priv, &rxpkt) == RSLT_SUCCESS) {
			ret = OK;
			break;
		} else {
			ret = -EINVAL;
			priv->lower->reset();
			up_udelay(ADAM110_HW_RST_WAIT);
			priv->fw_loaded = false;
		}
	}

	/* if reset, re-send model */
    if (ret == OK && priv->fw_loaded == false) {
        ret = ADAM110_SET_MODEL(priv);
        if (ret != RSLT_SUCCESS) {
            auddbg("[E] Model send fail!\n");
            ret = -EINVAL;
        } else {
			priv->fw_loaded = true;
			ret = OK;
		}
    }

	return ret;
}

static int adam110_app_device_alive_check(int argc, FAR char *argv[])
{
	FAR struct adam110_dev_s *priv = (struct adam110_dev_s *)g_adam110;
	int ret = OK;

	while(1) {
		adam110_takesem(&priv->devsem);
#ifdef CONFIG_PM
		(void)pm_suspend(priv->pm_domain);
#endif
		ret = adam110_check_aliveness(priv);
		if (ret != OK) {
			auddbg("[E] Adam110 check aliveness failed\n");
		}
#ifdef CONFIG_PM
		(void)pm_resume(priv->pm_domain);
#endif
		adam110_givesem(&priv->devsem);
		usleep(ADAM110_ALIVENESS_CHECK_PERIOD_US);
	}

	return OK;
}
#endif

static void adam110_work_handler(void *arg)
{
	FAR struct adam110_dev_s *priv = (struct adam110_dev_s *)arg;
	struct audio_msg_s msg;
	t_proto_pkt txpkt, rxpkt;
	int ret = 0;

	bool send_kd_msg = false;
	bool send_pcm_cb = false;
	FAR struct ap_buffer_s *pcm_apb = NULL;

	memset(&msg, 0x00, sizeof(msg));
	memset(&txpkt, 0x00, sizeof(txpkt));
	memset(&rxpkt, 0x00, sizeof(rxpkt));

	adam110_takesem(&priv->devsem);

	ret = ADAM110_GET_KEEP_ALIVE(priv, &rxpkt);
	if (ret != RSLT_SUCCESS) {
		auddbg("[E] Keep alive failed.\n");
		goto out_unlock;
	}

	ret = ADAM110_GET_EVENT(priv, &rxpkt);
	if (ret != RSLT_SUCCESS) {
		auddbg("[E] Get event failed.\n");
		goto out_unlock;
	}

	if (rxpkt.parm2 == AI_DATA_TYPE_SEAMLESS) {
		uint32_t data_size = 0;
		int is_final_packet = 0;
		int recvsize = 0;
    
		recvsize = (uint32_t)(rxpkt.parm3 << 8 | rxpkt.parm4);

		if (recvsize > 0) {
			data_size = (recvsize < 3840) ? recvsize : 3840;
        
			is_final_packet = (data_size < 3840) ? 1 : 0;

			ret = ADAM110_GET_AUDIOBUFFER(priv, rxpkt.parm2, s_temp_chunk, data_size + 1);
			if (ret == OK) {
				uint8_t cal_sum = adam110_calculate_checksum(s_temp_chunk, data_size);
				uint8_t recv_sum = s_temp_chunk[data_size];

				if (cal_sum == recv_sum) {
					uint8_t *dest_ptr = (uint8_t *)&s_rxsmlbuf[priv->keyword_bytes_left];
					memcpy(dest_ptr, s_temp_chunk, data_size);

					priv->keyword_bytes_left += data_size;
				} else {
					auddbg("[E] KD checksum calc:0x%02x, Recv:0x%02x\n", cal_sum, recv_sum);
					goto out_unlock;
				}
			}
		}

		if (is_final_packet && priv->keyword_bytes_left == priv->keyword_bytes) {
			msg.msgId = AUDIO_MSG_KD;
			msg.u.pPtr = (void *)s_rxsmlbuf;

			if (priv->dev.process_mq != NULL && priv->kd_enabled == true) {
				audvdbg("[I] Final Total:%u\n", priv->keyword_bytes_left);
            
				priv->keyword_buffer = (uint8_t *)s_rxsmlbuf;
				send_kd_msg = true;
			}
		}
		goto out_unlock_then_mq;
	}


	if (rxpkt.parm2 == AI_DATA_TYPE_AUDIO) {
		uint16_t pcm_size = (uint16_t)(rxpkt.parm3 << 8 | rxpkt.parm4);

		if (!priv->running || !priv->recording) {
			auddbg("[PCM] drop after stop: running=%d recording=%d size=%u\n",
				   priv->running, priv->recording, pcm_size);
			goto out_unlock;
		}

		if (pcm_size > 0) {
			if (pcm_size > ADAM110_RX_MAX_SIZE) {
				auddbg("[PCM] pcm_size(%u) > tmp(%u)\n", pcm_size, ADAM110_RX_MAX_SIZE);
				pcm_size = ADAM110_RX_MAX_SIZE;
			}

			ret = ADAM110_GET_AUDIOBUFFER(priv, rxpkt.parm2, s_temp_chunk, pcm_size + 1);
			if (ret == OK) {
				uint8_t cal_sum = adam110_calculate_checksum(s_temp_chunk, pcm_size);
				uint8_t recv_sum = s_temp_chunk[pcm_size];

				if (cal_sum == recv_sum) {
					pcm_apb = pcm_waitq_take_one(priv);

					if (pcm_apb) {
						uint32_t copy_len = (pcm_size < pcm_apb->nmaxbytes) ? pcm_size : pcm_apb->nmaxbytes;
						memcpy(pcm_apb->samp, s_temp_chunk, copy_len);
						pcm_apb->curbyte = 0;
						pcm_apb->nbytes = copy_len;
						send_pcm_cb = true;

						if (pcm_size > pcm_apb->nmaxbytes) {
							auddbg("pcm truncated: pcm=%u pcm_apb=%u drop=%u\n", pcm_size, pcm_apb->nmaxbytes, pcm_size - pcm_apb->nmaxbytes);
						}
					}
					else {
						auddbg("no waiting pcm_apb, drop pcm=%u\n", pcm_size);
					}
				}
				else {
					auddbg("[PCM] checksum mismatch calc=0x%02x recv=0x%02x size=%u\n", cal_sum, recv_sum, pcm_size);
				}
			}
        }
		goto out_unlock_then_cb;
    }
	goto out_unlock;

out_unlock_then_mq:
	adam110_givesem(&priv->devsem);
	priv->lower->irq_enable(true);

	if (send_kd_msg) {
		int ret_mq = mq_send(priv->dev.process_mq, (FAR const char *)&msg, sizeof(msg), 100);
		if (ret_mq < 0) {
			auddbg("[E] MQ:ret=%d, errno=%d\n", ret_mq, errno);
		}
	}	
	return;

out_unlock_then_cb:
	adam110_givesem(&priv->devsem);
	priv->lower->irq_enable(true);

	if (send_pcm_cb && pcm_apb) {
#ifdef CONFIG_AUDIO_MULTI_SESSION
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_DEQUEUE, pcm_apb, OK, NULL);
#else
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_DEQUEUE, pcm_apb, OK);
#endif
	}
	return;

out_unlock:
	adam110_givesem(&priv->devsem);
	priv->lower->irq_enable(true);

}

static void adam110_interrupt_dispatch(FAR void *arg)
{
	FAR struct adam110_dev_s *priv = (struct adam110_dev_s *)arg;
	int ret;
#ifdef CONFIG_PM
	pm_timedsuspend(priv->pm_domain, 10000);
#endif
	ret = work_queue(HPWORK, &adam110_work, adam110_work_handler, (void *)priv, 0);
	if (ret != 0) {
		auddbg("adam110 work_queue fail\n");
	}
}

static int adam110_setMute(FAR struct adam110_dev_s *priv, bool mute)
{
	int ret = 0;
	t_proto_pkt rxpkt;

	audvdbg("[I] mute : %d\n", mute);

	if (!priv->init_done) {
		return 0;
	}
	if (mute) {
		ret = ADAM110_SET_INTR(priv, AI_DATA_TYPE_SEAMLESS, false, &rxpkt);
		if (ret != 0) {
			auddbg("adam110 kd stop failed ret : %d\n", ret);
			return ret;
		}
#ifdef CONFIG_AUDIO_MULTI_SESSION
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_MICMUTE, NULL, OK, NULL);
#else
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_MICMUTE, NULL, OK);
#endif
	} else {
		ret = ADAM110_SET_INTR(priv, AI_DATA_TYPE_SEAMLESS, true, &rxpkt);
		if (ret != 0) {
			auddbg("adam110 kd start failed ret : %d\n", ret);
			return ret;
		}
#ifdef CONFIG_AUDIO_MULTI_SESSION
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_MICUNMUTE, NULL, OK, NULL);
#else
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_MICUNMUTE, NULL, OK);
#endif
	}
	return ret;
}

/****************************************************************************
 * Name: adam110_getcaps
 *
 * Description: Get the audio device capabilities
 *
 ****************************************************************************/

static int adam110_getcaps(FAR struct audio_lowerhalf_s *dev, int type, FAR struct audio_caps_s *caps)
{

	FAR struct adam110_dev_s *priv = (struct adam110_dev_s *)dev;
	audvdbg("[I] type=%d\n", type);

	/* Validate the structure */

	DEBUGASSERT(caps->ac_len >= sizeof(struct audio_caps_s));
	audvdbg("[I] type=%d ac_type=%d\n", type, caps->ac_type);

	/* Fill in the caller's structure based on requested info */

	caps->ac_format.hw = 0;
	caps->ac_controls.w = 0;

	switch (caps->ac_type) {
		/* Caller is querying for the types of units we support */

	case AUDIO_TYPE_QUERY:

		/* Provide our overall capabilities.  The interfacing software
		 * must then call us back for specific info for each capability.
		 */

		caps->ac_channels = 1;	/* Mono output */

		switch (caps->ac_subtype) {
		case AUDIO_TYPE_QUERY:
			/* We don't decode any formats!  Only something above us in
			 * the audio stream can perform decoding on our behalf.
			 */

			/* The types of audio units we implement */

			caps->ac_controls.b[0] = AUDIO_TYPE_OUTPUT | AUDIO_TYPE_FEATURE | AUDIO_TYPE_PROCESSING;
			break;

		default:
			caps->ac_controls.b[0] = AUDIO_SUBFMT_END;
			break;
		}
		break;

		/* Provide capabilities of our INPUT & OUTPUT unit */

	case AUDIO_TYPE_INPUT:
		caps->ac_channels = 1;

		switch (caps->ac_subtype) {
		case AUDIO_TYPE_QUERY:

			/* Report the Sample rates we support */

			caps->ac_controls.b[0] = AUDIO_SAMP_RATE_TYPE_16K;
			break;

		default:
			break;
		}

		break;

		/* Provide capabilities of our FEATURE units */

	case AUDIO_TYPE_FEATURE:
	
		switch(caps->ac_subtype) {
		case AUDIO_FU_INP_GAIN:
			return -ENOSYS;
		case AUDIO_FU_MUTE:
			adam110_takesem(&priv->devsem);
			caps->ac_controls.b[0] = priv->mute;
			adam110_givesem(&priv->devsem);
		default:
			break;
		}

		break;

		/* Provide capabilities of our PROCESSING unit */

	case AUDIO_TYPE_PROCESSING:
		audvdbg("\n\t[I] AUDIO_TYPE_PROCESSING, type:%d, subtype:%d\n", caps->ac_type, caps->ac_subtype);

		switch (caps->ac_subtype) {
		case AUDIO_PU_UNDEF:
			caps->ac_controls.b[0] = 
#ifdef CONFIG_AUDIO_SPEECH_DETECT_FEATURES
			AUDIO_PU_SPEECH_DETECT |
#endif
			AUDIO_PU_UNDEF;
			break;

		case AUDIO_PU_SPEECH_DETECT:
			audvdbg("\n\t[I] ADAM110, AUDIO_PU_SPPECH_DETECT, ac_controls.b[0] = %d\n", caps->ac_controls.b[0]);
			/* Provide capabilities of our Speech Detect */
			caps->ac_controls.b[0] =
#ifdef CONFIG_AUDIO_SPEECH_DETECT_FEATURES
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
			AUDIO_SD_KEYWORD_DETECT |
#ifdef CONFIG_AUDIO_ADAM110_AEC_SUPPORT
			AUDIO_SD_AEC |
#endif
#endif
#endif
			AUDIO_SD_UNDEF;
			break;

		case AUDIO_PU_KD_SENSITIVITY: {
			adam110_takesem(&priv->devsem);
			caps->ac_controls.w = priv->sensitivity;
			adam110_givesem(&priv->devsem);
		}
			break;
		default:

			/* Other types of processing uint we don't support */
			break;
		}

		break;

		/* All others we don't support */

	default:

		/* Zero out the fields to indicate no support */

		caps->ac_subtype = 0;
		caps->ac_channels = 0;

		break;
	}

	/* Return the length of the audio_caps_s struct for validation of
	 * proper Audio device type.
	 */

	audvdbg("[I] Return : %d\n", caps->ac_len);
	return caps->ac_len;
}

/****************************************************************************
 * Name: adam110_configure
 *
 * Description:
 *   Configure the audio device for the specified  mode of operation.
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int adam110_configure(FAR struct audio_lowerhalf_s *dev, FAR void *session, FAR const struct audio_caps_s *caps)
#else
static int adam110_configure(FAR struct audio_lowerhalf_s *dev, FAR const struct audio_caps_s *caps)
#endif
{
	int ret = OK;
	FAR struct adam110_dev_s *priv = (FAR struct adam110_dev_s *)dev;

	DEBUGASSERT(priv && caps);
	audvdbg("[I} ac_type: %d\n", caps->ac_type);

	/* Process the configure operation */

	switch (caps->ac_type) {
	case AUDIO_TYPE_INPUT:
		audvdbg("[I]  AUDIO_TYPE_INPUT\n");
		break;
	case AUDIO_TYPE_FEATURE:
		audvdbg("[I]  AUDIO_TYPE_FEATURE\n");
		/* Process based on Feature Unit */
		switch (caps->ac_format.hw) {
		case AUDIO_FU_INP_GAIN: {
			return -ENOSYS;
		}
		break;
		case AUDIO_FU_MUTE: {
			bool mute = caps->ac_controls.b[0];
			audvdbg("[I] mute: 0x%x\n", mute);
			adam110_takesem(&priv->devsem);
			/* set HW mute */
			ret = adam110_setMute(priv, mute);
			if (ret != 0) {
				auddbg("[E} adam110_setMute failed ret : %d\n", ret);
				adam110_givesem(&priv->devsem);
				return ret;
			}
			priv->mute = mute;
			adam110_givesem(&priv->devsem);

		}
		break;
		default:
			auddbg("[E] Unrecognized feature unit\n");
			break;
		}
		break;
	case AUDIO_TYPE_PROCESSING:
		audvdbg("[I]  AUDIO_PU_SPEECH_DETECT\n");
		switch (caps->ac_subtype) {
		case AUDIO_PU_SPEECH_DETECT:
			switch(caps->ac_controls.w) {
#ifdef CONFIG_AUDIO_SPEECH_DETECT_FEATURES
			case AUDIO_SD_KEYWORD_DETECT:
			case AUDIO_SD_LOCAL:
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
#else
				ret = -EINVAL;
#endif
				break;
			}
#endif
			break;
		case AUDIO_PU_KD_SENSITIVITY: {
			uint16_t sensitivity = caps->ac_controls.w;
			uint8_t level = AI_MODEL_THD_LOW;
			t_proto_pkt rxpkt;
			if (sensitivity >= 0xAAAA)
				level = AI_MODEL_THD_HIGH;
			else if (sensitivity >= 0x5555)
				level = AI_MODEL_THD_MID;
			adam110_takesem(&priv->devsem);
			ADAM110_AI_SET_THD(priv, level, &rxpkt);
			priv->sensitivity = sensitivity;
			adam110_givesem(&priv->devsem);
		}
		break;
		default:
			auddbg("[E] Unrecognized feature unit\n");
			break;
		}
		break;
	default:
		audvdbg("[I] adam110_configure received unknown ac_type 0x%x\n", caps->ac_type);
		ret = -EINVAL;
		break;
	}

	return ret;
}

/****************************************************************************
 * Name: adam110_shutdown
 *
 * Description:
 *   Shutdown the driver and put it in the lowest power state possible.
 *
 ****************************************************************************/

static int adam110_shutdown(FAR struct audio_lowerhalf_s *dev)
{
	audvdbg("[I] Return OK\n");
	return OK;
}

#ifdef CONFIG_AUDIO_ENDPOINT_DETECT
#define AUDIO_MSG_EPD              10
#endif

#ifdef CONFIG_AUDIO_KEYWORD_DETECT
#define AUDIO_MSG_KD               11
#endif

/****************************************************************************
 * Name: adam110_spi_unregisterprocess
 *
 * Description: 
 *   Register the process (epd, wakeup etc.).
 *
 ****************************************************************************/
static int adam110_spi_unregisterprocess(FAR struct audio_lowerhalf_s *dev)
{
	FAR struct adam110_dev_s *priv = (FAR struct adam110_dev_s *)dev;
	int ret = OK;

	adam110_takesem(&priv->devsem);
	if (priv->dev.process_mq != NULL) {
		priv->dev.process_mq = NULL;
	} else {
		auddbg("[E] mq is null\n");
		ret = -ENOENT;
	}
	adam110_givesem(&priv->devsem);
	return ret;
}

/****************************************************************************
 * Name: adam110_spi_registerprocess
 *
 * Description:
 *   Register the process (epd, wakeup etc.).
 *
 ****************************************************************************/
static int adam110_spi_registerprocess(FAR struct audio_lowerhalf_s *dev, mqd_t mq)
{
	FAR struct adam110_dev_s *priv = (FAR struct adam110_dev_s *)dev;

	audvdbg("[I] Entry\n");
	adam110_takesem(&priv->devsem);
	if (priv->dev.process_mq == NULL) {
		priv->dev.process_mq = mq;
		adam110_givesem(&priv->devsem);
		return OK;
	}
	adam110_givesem(&priv->devsem);
	auddbg("[E] already registered!!\n");
	return -EBUSY;
}

/****************************************************************************
 * Name: adam110_start
 *
 * Description:
 *   Start the configured operation (audio streaming, volume enabled, etc.).
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int adam110_start(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int adam110_start(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct adam110_dev_s *priv = (FAR struct adam110_dev_s *)dev;
	t_proto_pkt rxpkt;

	if (priv->running) {
		return 0;
	}

	if (priv->mute) {
		return -ESTRPIPE;
	}

	audvdbg("[I] adam110_start Entry\n");
	adam110_takesem(&priv->devsem);
	priv->running = true;
	priv->recording = true;

	ADAM110_SET_INTR(priv, AI_DATA_TYPE_AUDIO, true, &rxpkt);

	adam110_givesem(&priv->devsem);
	return 0;
}

/****************************************************************************
 * Name: adam110_stop
 *
 * Description: Stop the configured operation (audio streaming, volume
 *              disabled, etc.).
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int adam110_stop(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int adam110_stop(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct adam110_dev_s *priv = (FAR struct adam110_dev_s *)dev;
	t_proto_pkt rxpkt;

	if (!priv) {
		return -EINVAL;
	}

	audvdbg("[I] adam110_stop Entry\n");
	adam110_takesem(&priv->devsem);
	priv->running = false;
	priv->recording = false;
	ADAM110_SET_INTR(priv, AI_DATA_TYPE_AUDIO, false, &rxpkt);

	FAR struct ap_buffer_s *apb;
	int flushed = 0;

	while ((apb = (FAR struct ap_buffer_s *)sq_remfirst(&priv->pendq)) != NULL) {
		apb->nbytes = 0;
		apb->curbyte = 0;
		flushed++;
	}

	auddbg("[I] stop flushed pendq=%d\n", flushed);

	adam110_givesem(&priv->devsem);
	return 0;
}
#endif

/****************************************************************************
 * Name: adam110_pause
 *
 * Description: Pauses the playback.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int adam110_pause(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int adam110_pause(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct adam110_dev_s *priv = (FAR struct adam110_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}

	adam110_takesem(&priv->devsem);
	adam110_givesem(&priv->devsem);
	return 0;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */

/****************************************************************************
 * Name: adam110_resume
 *
 * Description: Resumes the playback.
 *
 ****************************************************************************/
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int adam110_resume(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int adam110_resume(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct adam110_dev_s *priv = (FAR struct adam110_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}

	adam110_takesem(&priv->devsem);
	adam110_givesem(&priv->devsem);
	return 0;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */

/****************************************************************************
 * Name: adam110_enqueuebuffer
 *
 * Description: Enqueue an Audio Pipeline Buffer for playback/ processing.
 *
 ****************************************************************************/
static int adam110_enqueuebuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb)
{
	FAR struct adam110_dev_s *priv = (FAR struct adam110_dev_s *)dev;

	if (!priv || !apb) {
		return -EINVAL;
	}

	adam110_takesem(&priv->devsem);
	sq_addlast((sq_entry_t *)&apb->dq_entry, &priv->pendq);
	auddbg("[I] enqueue added buf %p nmax=%u running=%d\n", apb, apb->nmaxbytes, priv->running);	
	adam110_givesem(&priv->devsem);
	return OK;
}

/****************************************************************************
 * Name: adam110_cancelbuffer
 *
 * Description: Called when an enqueued buffer is being cancelled.
 *
 ****************************************************************************/
static int adam110_cancelbuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb)
{
	audvdbg("[I] apb=%p curbyte=%d nbytes=%d, return OK\n", apb, apb->curbyte, apb->nbytes);
	return OK;
}

/****************************************************************************
 * Name: adam110_ioctl
 *
 * Description: Perform a device ioctl
 *
 ****************************************************************************/

static int adam110_ioctl(FAR struct audio_lowerhalf_s *dev, int cmd, unsigned long arg)
{
	FAR struct adam110_dev_s *priv = (FAR struct adam110_dev_s *)dev;
	FAR struct ap_buffer_info_s *bufinfo;
	t_proto_pkt rxpkt;
	int ret = OK;

	if (!priv) {
		return -EINVAL;
	}

	audvdbg("[I] cmd=%d arg=%ld\n", cmd, arg);

	/* Deal with ioctls passed from the upper-half driver */

	switch (cmd) {
		/* Check for AUDIOIOC_HWRESET ioctl.  This ioctl is passed straight
		 * through from the upper-half audio driver.
		 */
	case AUDIOIOC_PREPARE: {
		/* nothing to prepare... */
	}
	break;

	/* Report our preferred buffer size and quantity */

	case AUDIOIOC_GETBUFFERINFO: {
		audvdbg("[I] AUDIOIOC_GETBUFFERINFO:\n");
		adam110_takesem(&priv->devsem);

		bufinfo = (FAR struct ap_buffer_info_s *)arg;
		bufinfo->buffer_size = priv->sample_size;
		bufinfo->nbuffers = CONFIG_ADAM110_NUM_BUFFERS;

		auddbg("[I] buffer_size : %d nbuffers : %d\n",
				bufinfo->buffer_size, bufinfo->nbuffers);

		adam110_givesem(&priv->devsem);
	}
	break;
	case AUDIOIOC_REGISTERPROCESS: {
#ifdef CONFIG_AUDIO_PROCESSING_FEATURES
		ret = adam110_spi_registerprocess(dev, (mqd_t)arg);
		if (ret != OK) {
			auddbg("[E] Process start failed ret : %d\n", ret);
			return ret;
		}
#else
		auddbg("[E] Register process failed - Device doesn't support\n");
		ret = -EINVAL;
#endif
	}
	break;
	case AUDIOIOC_UNREGISTERPROCESS: {
#ifdef CONFIG_AUDIO_PROCESSING_FEATURES
		ret = adam110_spi_unregisterprocess(dev);
		if (ret != OK) {
			auddbg("[E] Process start failed ret : %d\n", ret);
			return ret;
		}
#else
		auddbg("[E] UnRegister process failed - Device doesn't support\n");
		ret = -EINVAL;
#endif
	}
	break;

	case AUDIOIOC_STARTPROCESS: {
		audvdbg("[I] Set start process!!\n");
#ifdef CONFIG_AUDIO_PROCESSING_FEATURES
		switch ((uint8_t)arg) {
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
		case AUDIO_SD_LOCAL:
		case AUDIO_SD_KEYWORD_DETECT: {
			if (priv->kd_enabled == false) {
				priv->kd_enabled = true;
				ret = ADAM110_SET_INTR(priv, AI_DATA_TYPE_SEAMLESS, priv->kd_enabled, &rxpkt);
				if (ret != RSLT_SUCCESS) {
					auddbg("KD enable failed.\n");
				}
			}
		}
		break;
#endif
		case AUDIO_SD_AEC: {
#ifdef CONFIG_AUDIO_ADAM110_AEC_SUPPORT
			/* aec_enable function */
			ADAM110_SET_AEC(priv, true, &rxpkt);
#endif
		}
		break;
		default: {
			/* DO Nothing for now */
		}
		break;
		}
#else
		auddbg("[E] Start process failed - Device doesn't support\n");
		ret = -EINVAL;
#endif
	}
	break;
	case AUDIOIOC_STOPPROCESS: {
		audvdbg("[I] Set stop process!!\n");
#ifdef CONFIG_AUDIO_PROCESSING_FEATURES
		switch ((uint8_t)arg) {
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
		case AUDIO_SD_KEYWORD_DETECT: {
			if (priv->kd_enabled == true) {
				/* stop keyword detect */
				priv->kd_enabled = false;
				ADAM110_SET_INTR(priv, AI_DATA_TYPE_SEAMLESS, priv->kd_enabled, &rxpkt);
			}
		}
		break;
#endif
		}
#else
		auddbg("[E] Start process failed - Device doesn't support\n");
		ret = -EINVAL;
#endif
	}
	break;
#ifdef CONFIG_AUDIO_PROCESSING_FEATURES
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
	case AUDIOIOC_GETKDBUFSIZE: {
		*(uint32_t *)arg = priv->keyword_bytes;
	}
	break;
	case AUDIOIOC_GETKDDATA: {
		memcpy((uint8_t *)arg, priv->keyword_buffer, priv->keyword_bytes);
		priv->keyword_bytes_left = 0;
	}
	break;
#endif
#endif
	case AUDIOIOC_ENABLEDMIC: {
		if (priv->lower && priv->lower->set_dmic) {
			bool enable = (bool)arg;
			priv->lower->set_dmic(enable);
			ret = OK;
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case AUDIOIOC_CHANGEKD: {
		if (((arg & AUDIO_NN_MODEL_MASK) > AUDIO_NN_MODEL_MAX) ||
				((arg & AUDIO_NN_MODEL_LANG_MASK) > AUDIO_NN_MODEL_LANG_MAX)) {
			return -EINVAL;
		}

		if (priv->running) {
			return -EBUSY;
		}

		uint8_t kd_num = arg;
		if (arg > 2)
			ret = -EINVAL;
		else {
			ret = ADAM110_AI_SET_INTR(priv, (kd_num + 1), true, &rxpkt);
			if (ret != RSLT_SUCCESS) {
				auddbg("Model enable failed.\n");
			}
		}
	}
	break;
	default:
		audvdbg("[I] adam110_ioctl received unknown cmd 0x%x\n", cmd);
		ret = -EINVAL;
		break;
	}

	return ret;
}

/****************************************************************************
 * Name: adam110_reserve
 *
 * Description: Reserves a session (the only one we have).
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int adam110_reserve(FAR struct audio_lowerhalf_s *dev, FAR void **session)
#else
static int adam110_reserve(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct adam110_dev_s *priv = (FAR struct adam110_dev_s *)dev;
	int ret = OK;

	adam110_takesem(&priv->devsem);
	if (priv->reserved) {
		ret = -EBUSY;
	} else {
#ifdef CONFIG_AUDIO_MULTI_SESSION
		*session = NULL;
#endif
		priv->running = false;
		priv->reserved = true;
	}
	adam110_givesem(&priv->devsem);

	return ret;
}

/****************************************************************************
 * Name: adam110_release
 *
 * Description: Releases the session (the only one we have).
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int adam110_release(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int adam110_release(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct adam110_dev_s *priv = (FAR struct adam110_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}

	adam110_takesem(&priv->devsem);
	if (priv->running) {
		priv->running = false;
	}
	priv->reserved = false;
	adam110_givesem(&priv->devsem);

	return OK;
}

#ifdef CONFIG_PM
/****************************************************************************
 * Name: adam110_pm_notify
 *
 * Description:
 *   Notify the driver of new power state. This callback is called after
 *   all drivers have had the opportunity to prepare for the new power state.
 *
 ****************************************************************************/

static void adam110_pm_notify(struct pm_callback_s *cb, enum pm_state_e state)
{
	/* Currently PM follows the state changes as follows,
	 * On boot, we are in PM_NORMAL. After that we only use PM_NORMAL and PM_SLEEP
	 * on boot : PM_NORMAL -> PM_SLEEP, from there on
	 * PM_SLEEP -> PM_NORMAL -> PM_SLEEP -> PM_NORMAL........
	 */
	switch (state) {
	case(PM_SLEEP): {
		audvdbg("[I] Entering SLEEP\n");
#ifdef CONFIG_AUDIO_ADAM110_AEC_SUPPORT
		t_proto_pkt rxpkt;
		ADAM110_SET_AEC(g_adam110, false, &rxpkt);
#endif
	}
	break;
	default: {
		/* Nothing to do */
		audvdbg("[I] Default case\n");
	}
	break;
	}
}

/****************************************************************************
 * Name: adam110_pm_prepare
 *
 * Description:
 *   Request the driver to prepare for a new power state. This is a warning
 *   that the system is about to enter into a new power state. The driver
 *   should begin whatever operations that may be required to enter power
 *   state. The driver may abort the state change mode by returning a
 *   non-zero value from the callback function.
 *
 ****************************************************************************/

static int adam110_pm_prepare(struct pm_callback_s *cb, enum pm_state_e state)
{
	audvdbg("[I] entry\n");
	return OK;
}
#endif	/* End of CONFIG_PM */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: adam110_lowerhalf_initialize
 *
 * Description:
 *   Initialize the adam110 device.
 *
 * Input Parameters:
 *   spi     - An SPI driver instance
 *   lower   - Persistent board configuration data
 *
 * Returned Value:
 *   A new lower half audio interface for the AI-ADAM device is returned
 *   on success; NULL is returned on failure.
 *
 ****************************************************************************/
FAR struct audio_lowerhalf_s *adam110_lowerhalf_initialize(FAR struct spi_dev_s *spi, FAR struct adam110_lower_s *lower)
{
	FAR struct adam110_dev_s *priv;
	int ret = OK;
	t_proto_pkt rxpkt;
	int retry = ADAM110_RETRY_CNT;

	priv = (FAR struct adam110_dev_s *)kmm_zalloc(sizeof(struct adam110_dev_s));
	if (!priv) {
		auddbg("[E] Adam110 priv alloc failed.\n");
		return NULL;
    }

	priv->init_done = false;
	priv->dev.ops = &g_audioops;
	priv->spi = spi;
	priv->mic_gain = ADAM110_MIC_GAIN_DEFAULT;
	priv->sensitivity = ADAM110_DEFAULT_SENSITIVITY;
	priv->sample_size = ADAM110_DEFAULT_SAMPLE_SIZE;

	sq_init(&priv->pendq);
	sem_init(&priv->devsem, 0, 1);

	priv->lower = lower;
	priv->recording = false;
	priv->mute = false;
	priv->dev.process_mq = NULL;
	priv->keyword_bytes = ADAM110_KEYWORD_DATA_SIZE;

#ifdef CONFIG_PM
    /* only used during pm callbacks */
	g_adam110 = priv;

	priv->pm_domain = pm_domain_register("adam110");
	DEBUGASSERT(priv->pm_domain >= 0);
#endif


	adam110_takesem(&priv->devsem);

	/* keep alive */
	while (retry-- > 0) {
		if (ADAM110_GET_KEEP_ALIVE(priv, &rxpkt) == RSLT_SUCCESS) {
			ret = OK;
			break;
		} else {
			ret = -EINVAL;
		}
		priv->lower->reset();
		up_udelay(ADAM110_HW_RST_WAIT);
	}

	/* firmware update */
	retry = ADAM110_RETRY_CNT;
	while (retry-- > 0) {
		ret = ADAM110_SET_FIRMWARE(priv);
		if (ret == RSLT_SUCCESS) {
			ret = OK;
		} else {
			ret = -EINVAL;
		}
		priv->lower->reset();
		up_udelay(ADAM110_HW_RST_WAIT);
		if (ret == OK) {
			if (ADAM110_GET_KEEP_ALIVE(priv, &rxpkt) == RSLT_SUCCESS) {
				break;
			} else {
				ret = -EINVAL;
			}
		}
	}

	/* model update */
    if (ret == OK) {
        ret = ADAM110_SET_MODEL(priv);
        if (ret != RSLT_SUCCESS) {
            auddbg("[E] Model send fail!\n");
            ret = -EINVAL;
        } else {
			priv->fw_loaded = true;
		}
    }

	adam110_givesem(&priv->devsem);

    g_adam110 = priv;

#ifdef CONFIG_AUDIO_ADAM110_ALIVE_CHECK
	pid_t pid = kernel_thread("alive_check", 100, 1024, adam110_app_device_alive_check, NULL);
	if (pid < 0) {
		auddbg("Device alive check thread creatiopn failed\n");
		ret = -EINVAL;
	}
#endif

	if (ret != OK) {
		kmm_free(priv);
		g_adam110 = NULL;
		auddbg("[E] Model send fail!\n");
		return NULL;
	}

#ifdef CONFIG_PM
	/* register callbacks only if ADAM110 init is done */
	ret = pm_register(&g_pmndpcb);
	DEBUGASSERT(ret == OK);
#endif

	priv->lower->attach(adam110_interrupt_dispatch, (void *)priv); /* board adam110 gpio handler registration */

	priv->init_done = true;

	return &priv->dev;
}
