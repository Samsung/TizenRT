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
#include <tinyara/audio/i2s.h>
#include <tinyara/gpio.h>
#include <tinyara/kmalloc.h>
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

#ifdef CONFIG_PM
static struct adam110_dev_s *g_adam110;
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
static void adam110_spi_exchange(FAR struct spi_dev_s *spi, uint8_t *txpkt, int txlen, uint8_t *rxpkt, int rxlen);
static int adam110_verify_packet(t_proto_pkt *pkt);
static int adam110_spi_command_xfer(FAR struct spi_dev_s *spi, t_proto_pkt *txpkt, t_proto_pkt *rxpkt);
static int adam110_send_cmd(FAR struct spi_dev_s *spi, uint8_t op,
                                    uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4,
                                    t_proto_pkt *rx_out);


#define adam110_givesem(s)    sem_post(s)

#define ADAM110_RESET(spi, rx_ptr)           adam110_send_cmd(spi, DM_RESET, 0, 0, 0, 0, rx_ptr)
#define ADAM110_GET_KEEP_ALIVE(spi, rx_ptr)  adam110_send_cmd(spi, DM_GET_KEEP_ALIVE, 0, 0, 0, 0, rx_ptr)
#define ADAM110_GET_STATE(spi, rx_ptr)       adam110_send_cmd(spi, DM_GET_STATE, 0, 0, 0, 0, rx_ptr)
#define ADAM110_ENTER_BOOT(spi, rx_ptr)      adam110_send_cmd(spi, FW_ENTER_BOOT_MODE, 0, 0, 0, 0, rx_ptr)

#define ADAM110_GET_FW_VER(spi, rx_ptr) \
    adam110_send_cmd(spi, FW_GET_VER, 0, 0, 0, 0, rx_ptr)

#define ADAM110_GET_EVENT(spi, rx_ptr) \
    adam110_send_cmd(spi, DM_GET_EVENT, 0, 0, 0, 0, rx_ptr)

#define ADAM110_GET_SEAMLESS(spi, rx_ptr) \
    adam110_send_cmd(spi, AUD_GET_SEAMLESS_DATA, 0, 0, 0, 0, rx_ptr)

#define ADAM110_GET_AUDIO(spi, rx_ptr) \
    adam110_send_cmd(spi, AUD_GET_PREP_DATA, 0, 0, 0, 0, rx_ptr)

#define ADAM110_AI_CHECK_XMIT(spi, h_size, l_size, rx_ptr) \
    adam110_send_cmd(spi, AC_UPDATE_CHK_XMIT, h_size, l_size, 0, 0, rx_ptr)

#define ADAM110_SET_I2S(spi, rate, width, rx_ptr) \
    adam110_send_cmd(spi, AUD_I2S_SET_CONFIG, (uint8_t)(rate), (uint8_t)(width), 0, 0, rx_ptr)

#define ADAM110_SET_AEC(spi, enable, rx_ptr) \
    adam110_send_cmd(spi, AUD_I2S_AEC_CTRL, (uint8_t)(enable), 0, 0, 0, rx_ptr)

#define ADAM110_AI_UPDATE_START(spi, rx_ptr) \
    adam110_send_cmd(spi, AC_UPDATE_MODEL, 0, 0, 0, 0, rx_ptr)

#define ADAM110_SET_MODEL(spi) \
	adam110_send_model(spi) 

#define ADAM110_SET_FIRMWARE(spi) \
	adam110_send_firmware(spi) 

#define ADAM110_GET_KDBUFFER(spi, buf_ptr, size) \
	adam110_get_kdbuffer(spi, buf_ptr, size) 

#define ADAM110_GET_AUDIOBUFFER(spi, buf_ptr, size) \
	adam110_get_audiobuffer(spi, buf_ptr, size) 

#define ADAM110_AI_SET_THD(spi, thd_level, rx_ptr) \
    adam110_send_cmd(spi, AC_THD_ADJ, (uint8_t)(thd_level), 0, 0, 0, rx_ptr)

#define ADAM110_AI_SET_INTR(spi, model, enable, rx_ptr) \
    adam110_send_cmd(spi, DM_SET_INTR, model, (uint8_t)(enable), 0, 0, rx_ptr)

static void adam110_spi_exchange(FAR struct spi_dev_s *spi, uint8_t *txpkt, int txlen, uint8_t *rxpkt, int rxlen)
{
	if (txpkt != NULL) {
		SPI_SNDBLOCK(spi, (void *)txpkt, txlen);
		up_udelay(ADAM110_TXRX_DELAY);
	}
	
	if (rxpkt != NULL) {
		SPI_RECVBLOCK(spi, (void *)rxpkt, rxlen);
	}
}

static uint8_t adam110_calculate_checksum(uint8_t *data, uint32_t size) 
{
	uint8_t checksum = 0;
	for (uint32_t i = 0; i < size; i++) {
	checksum += data[i];
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

static int adam110_spi_command_xfer(FAR struct spi_dev_s *spi, t_proto_pkt *txpkt, t_proto_pkt *rxpkt)
{
	txpkt->header = PKT_HEADER_SEND;
	txpkt->checksum = adam110_calculate_checksum((uint8_t *)txpkt, sizeof(t_proto_pkt) - 1);

	adam110_spi_exchange(spi, (uint8_t *)txpkt, sizeof(t_proto_pkt),
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

static int adam110_send_cmd(FAR struct spi_dev_s *spi, uint8_t op,
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

	return adam110_spi_command_xfer(spi, &tx, rx_out);
}

static int adam110_send_model(FAR struct spi_dev_s *spi)
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

	if (ADAM110_AI_UPDATE_START(spi, &rxpkt) != RSLT_SUCCESS) {
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
			p2 = 0x01;
		} else {
			uint16_t actual_len = (uint16_t)(nread + 1);
			p1 = (uint8_t)((actual_len >> 8) & 0xFF);
			p2 = (uint8_t)(actual_len & 0xFF);
		}

		retry_remain = ADAM110_MODEL_RETRY_CNT;
		while (retry_remain > 0) {
			int ret = ADAM110_AI_CHECK_XMIT(spi, p1, p2, &rxpkt);

			if (rxpkt.op == RSLT_SUCCESS) {
				chunk_buf[nread] = adam110_calculate_checksum(chunk_buf, nread);
				SPI_SNDBLOCK(spi, chunk_buf, nread + 1);

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
	audvdbg("[I] Model update success (Total: %u bytes)\n", sent_size);
	return OK;
}

static int adam110_send_firmware(FAR struct spi_dev_s *spi)
{
	t_proto_pkt rxpkt;
	int fd;
	ssize_t nread;
	uint32_t sent_size = 0;
	uint8_t chunk_buf[ADAM110_MODEL_CHUNK_SIZE + 1]; // 256 + 1 (checksum)
	uint8_t header[6];
	int retry_remain;
	uint32_t cur_fw_ver = 0, img_fw_ver = 0;

	/* check f/w version */
	int ret = ADAM110_GET_FW_VER(spi, &rxpkt);
	if (ret != RSLT_SUCCESS) {
		return -EINVAL;
	} 
	
    cur_fw_ver = ((uint32_t)rxpkt.parm1 << 24) | ((uint32_t)rxpkt.parm2 << 16) | ((uint32_t)rxpkt.parm3 << 8) | ((uint32_t)rxpkt.parm4);
    
    audvdbg("Cur FW ver: %d.%d.%d (0x%08X)\n", rxpkt.parm1, rxpkt.parm2, (rxpkt.parm3 << 8 | rxpkt.parm4), cur_fw_ver);
	return OK;

#ifdef NEED_IMPL_FIRMWARE_UPDATE
    fd = open(mcu_package, O_RDONLY);
	if (fd < 0) {
		auddbg("[E] Open failed %s (%d)\n", mcu_package, errno);
		return -ENOENT;
	}

	nread = read(fd, header, sizeof(header));

	close(fd);

	if (nread < (ssize_t)sizeof(header)) {
		auddbg("[E] Header read failed (read:%zd)\n", nread);
		return (nread < 0) ? -errno : -EIO;
	}

	img_fw_ver = ((uint32_t)header[0] << 24) | ((uint32_t)header[1] << 16) | ((uint32_t)header[2] << 8) | ((uint32_t)header[3]);

	audvdbg("IMG FW ver: %d.%d.%d (0x%08X)\n", header[0], header[1], (header[2] << 8 | header[3]), img_fw_ver);

	if (img_fw_ver <= cur_fw_ver) {
		audvdbg("[I] Skip update (Current is latest)\n");
		return OK;
	}

	audvdbg("[I] Update starting...\n");

	if (ADAM110_AI_UPDATE_START(spi, &rxpkt) != RSLT_SUCCESS) {
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
			close(fd);
			return -EIO;
		}

		uint8_t p1, p2;
		if (nread == ADAM110_MODEL_CHUNK_SIZE) {
			p1 = 0x01;
			p2 = 0x01;
		} else {
			uint16_t actual_len = (uint16_t)(nread + 1);
			p1 = (uint8_t)((actual_len >> 8) & 0xFF);
			p2 = (uint8_t)(actual_len & 0xFF);
		}

		retry_remain = ADAM110_MODEL_RETRY_CNT;
		while (retry_remain > 0) {
			int ret = ADAM110_AI_CHECK_XMIT(spi, p1, p2, &rxpkt);

			if (rxpkt.op == RSLT_SUCCESS) {
				chunk_buf[nread] = adam110_calculate_checksum(chunk_buf, nread);
				SPI_SNDBLOCK(spi, chunk_buf, nread + 1);

				sent_size += nread;
				up_udelay(ADAM110_TXRX_DELAY);
				break; 
			} else if (rxpkt.op == RSLT_BUF_FULL) {
				retry_remain--;
				if (retry_remain <= 0) {
					close(fd);
					return -ETIMEDOUT;
				}
				up_udelay(ADAM110_TXRX_DELAY * 300);
			} else {
				close(fd);
				return -EIO;
			}
		}
	}

	close(fd);
	return OK;
#endif

}

static int adam110_get_kdbuffer(FAR struct spi_dev_s *spi, uint8_t *buffer, uint32_t size)
{
	int ret = 0;
	ret = ADAM110_GET_SEAMLESS(spi, NULL);
	if (ret == RSLT_SUCCESS) {
		up_udelay(ADAM110_TXRX_DELAY);
		SPI_RECVBLOCK(spi, buffer, size);
	}
	return ret;
}

static int adam110_get_audiobuffer(FAR struct spi_dev_s *spi, uint8_t *buffer, uint32_t size)
{
	int ret = 0;
	ret = ADAM110_GET_AUDIO(spi, NULL);
	if (ret == RSLT_SUCCESS) {
		up_udelay(ADAM110_TXRX_DELAY);
	  	SPI_RECVBLOCK(spi, buffer, size);
	}
	return ret;
}

#ifdef USE_HOST_RINGBUFFER
static void pcm_ring_log_overflow(FAR struct adam110_dev_s *p,
                                  uint32_t incoming,
                                  uint32_t drop,
                                  uint32_t fill_before)
{
	p->pcm_ovf_count++;
	p->pcm_drop_bytes_total += drop;

	if ((p->pcm_ovf_count == 1) || ((p->pcm_ovf_count % 10) == 0)) {
		auddbg("[HOST_RING] OVERFLOW #%lu: incoming=%lu drop=%lu fill_before=%lu ring=%lu drop_total=%lu\n",
				(unsigned long)p->pcm_ovf_count,
				(unsigned long)incoming,
				(unsigned long)drop,
				(unsigned long)fill_before,
				(unsigned long)p->pcm_ring_size,
				(unsigned long)p->pcm_drop_bytes_total);
    }
}

static void pcm_ring_push(FAR struct adam110_dev_s *p, const uint8_t *src, uint32_t n)
{
	if (!p || !src || n == 0)
		return;

	/* if incoming bigger than ring: keep only last ring_size bytes */
	if (n > p->pcm_ring_size) {
		src += (n - p->pcm_ring_size);
		n = p->pcm_ring_size;
	}

	uint32_t fill_before = p->pcm_fill;

	/* drop oldest if overflow */
	if (p->pcm_fill + n > p->pcm_ring_size) {
		uint32_t drop = (p->pcm_fill + n) - p->pcm_ring_size;
		pcm_ring_log_overflow(p, n, drop, fill_before);

		p->pcm_r = (p->pcm_r + drop) % p->pcm_ring_size;
		p->pcm_fill -= drop;
	}

	uint32_t first = p->pcm_ring_size - p->pcm_w;
	uint32_t w1 = (n < first) ? n : first;

	memcpy(p->pcm_ring + p->pcm_w, src, w1);
	if (n > w1) {
		memcpy(p->pcm_ring, src + w1, n - w1);
	}

	p->pcm_w = (p->pcm_w + n) % p->pcm_ring_size;
	p->pcm_fill += n;
}

static uint32_t pcm_ring_pop(FAR struct adam110_dev_s *p, uint8_t *dst, uint32_t n)
{
	if (!p || !dst || n == 0 || p->pcm_fill == 0)
		return 0;

	if (n > p->pcm_fill)
		n = p->pcm_fill;

	uint32_t first = p->pcm_ring_size - p->pcm_r;
	uint32_t r1 = (n < first) ? n : first;

	memcpy(dst, p->pcm_ring + p->pcm_r, r1);
	if (n > r1) {
		memcpy(dst + r1, p->pcm_ring, n - r1);
	}

	p->pcm_r = (p->pcm_r + n) % p->pcm_ring_size;
	p->pcm_fill -= n;

	return n;
}

static FAR struct ap_buffer_s * try_fill_one_apb(FAR struct adam110_dev_s *priv)
{
	struct ap_buffer_s *apb;

	apb = (struct ap_buffer_s *)sq_remfirst(&priv->pcm_apb_waitq);
	if (!apb)
		return NULL;

	uint32_t got = pcm_ring_pop(priv, apb->samp, apb->nmaxbytes);

	apb->curbyte = 0;
	apb->nbytes = got;

	return apb;
}
#endif

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

static void adam110_work_handler(void *arg)
{
	FAR struct adam110_dev_s *priv = (struct adam110_dev_s *)arg;
	struct audio_msg_s msg;
	t_proto_pkt txpkt, rxpkt;
	int ret = 0;

	memset(&txpkt, 0x00, sizeof(txpkt));
	memset(&rxpkt, 0x00, sizeof(rxpkt));

	adam110_takesem(&priv->devsem);

	ret = ADAM110_GET_KEEP_ALIVE(priv->spi, &rxpkt);
	if (ret != RSLT_SUCCESS) {
		auddbg("[E] Keep alive failed.\n");
		adam110_givesem(&priv->devsem);
		return;	
	}

	ret = ADAM110_GET_EVENT(priv->spi, &rxpkt);
	if (ret != RSLT_SUCCESS) {
		auddbg("[E] Get event failed.\n");
		adam110_givesem(&priv->devsem);
		return;	
	}

	if (rxpkt.parm2 == AI_DATA_TYPE_SEAMLESS) {
		uint32_t data_size = 0;
		int is_final_packet = 0;
    
		priv->keyword_bytes_left = (uint32_t)(rxpkt.parm3 << 8 | rxpkt.parm4);

		if (priv->keyword_bytes_left > 0) {
			data_size = (priv->keyword_bytes_left < 3840) ? priv->keyword_bytes_left : 3840;
        
			is_final_packet = (data_size < 3840) ? 1 : 0;

			ret = ADAM110_GET_KDBUFFER(priv->spi, s_temp_chunk, data_size + 1);
			if (ret == OK) {
				uint8_t cal_sum = adam110_calculate_checksum(s_temp_chunk, data_size);
				uint8_t recv_sum = s_temp_chunk[data_size];

				if (cal_sum == recv_sum) {
					uint8_t *dest_ptr = (uint8_t *)&s_rxsmlbuf[priv->keyword_bytes];
					memcpy(dest_ptr, s_temp_chunk, data_size);

					priv->keyword_bytes += data_size;
				} else {
					auddbg("[E] KD checksum calc:0x%02x, Recv:0x%02x\n", cal_sum, recv_sum);
					adam110_givesem(&priv->devsem);
					return;
				}
			}
		}

		if (is_final_packet && priv->keyword_bytes > 0) {
			msg.msgId = AUDIO_MSG_KD;
			msg.u.pPtr = (void *)s_rxsmlbuf;

			if (priv->dev.process_mq != NULL && priv->kd_enabled == true) {
				audvdbg("[I] Final Total:%u\n", priv->keyword_bytes);
            
				priv->keyword_buffer = (uint8_t *)s_rxsmlbuf;
				int ret_mq = mq_send(priv->dev.process_mq, (FAR const char *)&msg, sizeof(msg), 100);
            
				if (ret_mq < 0) {
					auddbg("[E] MQ:ret=%d, errno=%d\n", ret_mq, errno);
				}
			}
		}
	}

#ifdef USE_HOST_RINGBUFFER
	if (rxpkt.parm2 == AI_DATA_TYPE_AUDIO) {
		uint16_t pcm_size = (uint16_t)(rxpkt.parm3 << 8 | rxpkt.parm4);

		if (pcm_size > 0) {
			if (pcm_size > ADAM110_RX_MAX_SIZE) {
				auddbg("[HOST_RING] pcm_size(%d) > tmp(%d)\n", pcm_size, ADAM110_RX_MAX_SIZE);
				pcm_size = ADAM110_RX_MAX_SIZE;
			}

			ret = ADAM110_GET_AUDIOBUFFER(priv->spi, s_temp_chunk, pcm_size + 1);
			if (ret == OK) {
				uint8_t cal_sum = adam110_calculate_checksum(s_temp_chunk, pcm_size);
				uint8_t recv_sum = s_temp_chunk[pcm_size];

				if (cal_sum == recv_sum) {
					adam110_takesem(&priv->pcmsem);
					pcm_ring_push(priv, s_temp_chunk, pcm_size);
					adam110_givesem(&priv->pcmsem);
				}
			}

			while (1) {
				FAR struct ap_buffer_s *apb = NULL;
				adam110_takesem(&priv->pcmsem);
				if (priv->pcm_fill > 0 && sq_peek(&priv->pcm_apb_waitq) != NULL) {
					apb = try_fill_one_apb(priv);
				}
				adam110_givesem(&priv->pcmsem);

				if (!apb)
					break;

#ifdef CONFIG_AUDIO_MULTI_SESSION
				priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_DEQUEUE, apb, OK, NULL);
#else
				priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_DEQUEUE, apb, OK);
#endif
			}
        }
    }
#endif
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
		ret = ADAM110_AI_SET_INTR(priv->spi, AI_MODEL_SEAMLESS, false, &rxpkt);
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
		ret = ADAM110_AI_SET_INTR(priv->spi, AI_MODEL_SEAMLESS, true, &rxpkt);
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
			ADAM110_AI_SET_THD(priv->spi, level, &rxpkt);
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

	/* Enqueue buffers (enqueueed before the start of alc) to lower layer */
	sq_entry_t *tmp = NULL;
	sq_queue_t *q = &priv->pendq;
	for (tmp = sq_peek(q); tmp; tmp = sq_next(tmp)) {
		adam110_enqueuebuffer(dev, (struct ap_buffer_s *)tmp);
	}

	/* Remove audio buffers from pending queue here */
	while ((tmp = sq_remfirst(&priv->pendq)) != NULL) {
		apb_free((struct ap_buffer_s *)tmp);
	}

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

	if (!priv) {
		return -EINVAL;
	}

	audvdbg("[I] adam110_stop Entry\n");
	adam110_takesem(&priv->devsem);
	priv->running = false;
	priv->recording = false;
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

	if (!priv->running) {
		adam110_takesem(&priv->devsem);
		sq_addlast((sq_entry_t *)&apb->dq_entry, &priv->pendq);
		audvdbg("[I] enquene added buf 0x%x\n", apb);
		adam110_givesem(&priv->devsem);
		return 0;
	}
#ifdef USE_HOST_RINGBUFFER
	adam110_takesem(&priv->pcmsem);

	if (priv->pcm_fill > 0) {
		uint32_t got = pcm_ring_pop(priv, apb->samp, apb->nmaxbytes);
		apb->curbyte = 0;
		apb->nbytes = got;
		adam110_givesem(&priv->pcmsem);

#ifdef CONFIG_AUDIO_MULTI_SESSION
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_DEQUEUE, apb, OK, NULL);
#else
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_DEQUEUE, apb, OK);
#endif
		return OK;
	}

	sq_addlast((sq_entry_t *)&apb->dq_entry, &priv->pcm_apb_waitq);
	adam110_givesem(&priv->pcmsem);
	return OK;
#else
	audvdbg("[I] Return OK\n");
	return OK;
#endif
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

		audvdbg("[I] buffer_size : %d nbuffers : %d\n",
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
				ADAM110_AI_SET_INTR(priv->spi, AI_MODEL_SEAMLESS, priv->kd_enabled, &rxpkt);
			}
		}
		break;
#endif
		case AUDIO_SD_AEC: {
#ifdef CONFIG_AUDIO_ADAM110_AEC_SUPPORT
			/* aec_enable function */
			ADAM110_SET_AEC(priv->spi, true, &rxpkt);
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
				ADAM110_AI_SET_INTR(priv->spi, AI_MODEL_SEAMLESS, priv->kd_enabled, &rxpkt);
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
		priv->keyword_bytes = 0;
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
			ADAM110_AI_SET_INTR(priv->spi, (kd_num + 1), true, &rxpkt);
			ret = OK;
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
		ADAM110_SET_AEC(g_adam110->spi, false, &rxpkt);
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
	uint32_t cur_fw_ver = 0, fw_ver = 0;

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

#ifdef USE_HOST_RINGBUFFER
	priv->pcm_ring_size = ADAM110_PCM_RING_SIZE_4SEC;
	priv->pcm_ring = (uint8_t *)kmm_malloc(priv->pcm_ring_size);
	if (!priv->pcm_ring) {
		kmm_free(priv);
		auddbg("[E] Adam110 ring alloc failed.\n");
		return NULL;
	}
	priv->pcm_w = 0;
	priv->pcm_r = 0;
	priv->pcm_fill = 0;
	priv->pcm_ovf_count = 0;
	priv->pcm_drop_bytes_total = 0;

	sq_init(&priv->pcm_apb_waitq);
	sem_init(&priv->pcmsem, 0, 1);
#endif

#ifdef CONFIG_PM
    /* only used during pm callbacks */
	g_adam110 = priv;

	priv->pm_domain = pm_domain_register("adam110");
	DEBUGASSERT(priv->pm_domain >= 0);
#endif

	adam110_takesem(&priv->devsem);

	while (retry-- > 0) {
		if (ADAM110_GET_KEEP_ALIVE(spi, &rxpkt) == RSLT_SUCCESS) {
			ret = OK;
			break;
		}
		priv->lower->reset();
		up_udelay(ADAM110_HW_RST_WAIT);
	}

	if (ret == OK) {
		ret = ADAM110_SET_FIRMWARE(spi);
		if (ret != RSLT_SUCCESS) {
			auddbg("[E] FW update fail!\n");
			ret = -EINVAL;
		}
	}

    if (ret == OK) {
        ret = ADAM110_SET_MODEL(spi);
        if (ret != RSLT_SUCCESS) {
            auddbg("[E] Model send fail!\n");
            ret = -EINVAL;
        }
    }

	adam110_givesem(&priv->devsem);

	if (ret != OK) {
#ifdef USE_HOST_RINGBUFFER
		if (priv->pcm_ring) {
			kmm_free(priv->pcm_ring);
		}
#endif
		free(priv);
		auddbg("[E] Model send fail!\n");
		return NULL;
	}
#ifdef CONFIG_PM
	/* register callbacks only if NDP init is done */
    g_adam110 = priv;

	ret = pm_register(&g_pmndpcb);
	DEBUGASSERT(ret == OK);
#endif

	priv->lower->attach(adam110_interrupt_dispatch, (void *)priv); /* board adam110 gpio handler registration */

	priv->init_done = true;

	return &priv->dev;
}
