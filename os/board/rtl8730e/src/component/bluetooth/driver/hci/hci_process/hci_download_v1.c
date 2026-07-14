/*
 * Copyright (c) 2025 Realtek Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "osif.h"
#include "hci_platform.h"
#include "hci_controller.h"
#include "hci_transport.h"
#include "bt_debug.h"
#include "dlist.h"

#define USE_HCI_H4 1
#if defined(USE_HCI_H4) && USE_HCI_H4
#define RESERVE_LEN 1
#elif defined(USE_HCI_H5) && USE_HCI_H5
#define RESERVE_LEN 4
#else
#define RESERVE_LEN 0
#endif

#define HCI_PATCH_FRAG_SIZE 252

struct PATCH_INFO {
	uint32_t fw_len;
	uint32_t sent_fw_len;
	uint8_t *config_buf;
	uint16_t config_len;
	uint16_t sent_config_len;
	uint16_t cur_index;
	uint16_t end_index;
	uint8_t last_pkt;
	uint8_t *patch_buf;
	uint32_t patch_len;
};

static struct PATCH_INFO *patch_info = NULL;
static uint32_t lmp_subversion;

static uint8_t hci_patch_download_init(void)
{
	patch_info = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct PATCH_INFO));
	if (!patch_info) {
		return HCI_FAIL;
	}

	memset(patch_info, 0, sizeof(struct PATCH_INFO));

	return HCI_SUCCESS;
}

static void hci_patch_download_done(void)
{
	if (patch_info) {
		osif_mem_free(patch_info);
	}
	patch_info = NULL;
}

static uint32_t _parse_patch(uint8_t *p_buf)
{
	uint16_t patch_num, fw_chip_id, fw_len;
	uint32_t i, fw_offset;
	bool patch_found = false;

	LE_TO_UINT16(patch_num, p_buf);

	if (patch_num == 0) {
		BT_LOGE("Patch num error!\r\n");
		return 0;
	} else {
		for (i = 0; i < patch_num; i++) {
			LE_TO_UINT16(fw_chip_id, p_buf + 2 + 2 * i);
			if (fw_chip_id == hci_patch_get_chipid()) {
				LE_TO_UINT16(fw_len, p_buf + 2 + 2 * patch_num + 2 * i);
				LE_TO_UINT32(fw_offset, p_buf + 2 + 4 * patch_num + 4 * i);
				patch_found = true;
				break;
			}
		}

		if (patch_found == false) {
			BT_LOGE("No match patch!");
			return 0;
		}
	}
	patch_info->patch_buf = patch_info->patch_buf + fw_offset;
	return fw_len;
}

static uint8_t _get_patch_info(uint8_t *p_patch, uint32_t patch_len)
{
	struct PATCH_INFO *info = patch_info;
	uint16_t fw_len;
	uint8_t sig_len = 8;

	info->patch_buf = p_patch;
	info->patch_len = patch_len;

	LE_TO_UINT32(lmp_subversion, info->patch_buf + sig_len);
	BT_LOGA("LMP Subversion: %x\r\n", lmp_subversion);

	fw_len = _parse_patch(info->patch_buf + sig_len + sizeof(lmp_subversion));
	if (fw_len == 0) {
		BT_LOGE("Available patch not found!\r\n");
		return HCI_IGNORE;
	}
	BT_LOGA("FW Length: %d\r\n", fw_len);

	info->fw_len = fw_len;
	hci_platform_get_config(&info->config_buf, &info->config_len);

	/* Calculate patch info */
	info->end_index = (info->fw_len + info->config_len - 1) / HCI_PATCH_FRAG_SIZE;
	info->last_pkt = (info->fw_len + info->config_len) % HCI_PATCH_FRAG_SIZE;
	if (info->last_pkt == 0) {
		info->last_pkt = HCI_PATCH_FRAG_SIZE;
	}

	return HCI_SUCCESS;
}

static uint8_t hci_get_patch_cmd_len(uint8_t *cmd_len, uint8_t *p_patch, uint32_t patch_len)
{
	uint8_t ret;
	struct PATCH_INFO *info = patch_info;

	/* Download FW partial patch first time, get patch and info */
	if (0 == info->cur_index) {
		ret = _get_patch_info(p_patch, patch_len);
		if (HCI_SUCCESS != ret) {
			return ret;
		}
	}

	if (info->cur_index == info->end_index) {
		*cmd_len = info->last_pkt + 1;
		return HCI_SUCCESS;
	}

	*cmd_len = HCI_PATCH_FRAG_SIZE + 1;

	return HCI_SUCCESS;
}

static uint8_t hci_get_patch_cmd_buf(uint8_t *cmd_buf, uint8_t cmd_len)
{
	struct PATCH_INFO *info = patch_info;
	uint8_t *data_buf = &cmd_buf[1];
	uint8_t data_len = cmd_len - 1;
	uint8_t lmp_buf[4];
	uint8_t lmp_copy_length;

	lmp_buf[0] = (uint8_t)(lmp_subversion >> 0);
	lmp_buf[1] = (uint8_t)(lmp_subversion >> 8);
	lmp_buf[2] = (uint8_t)(lmp_subversion >> 16);
	lmp_buf[3] = (uint8_t)(lmp_subversion >> 24);

	/* first byte is index */
	if (info->cur_index >= 0x80) {
		cmd_buf[0] = (info->cur_index - 0x80) % 0x7f + 1;
	} else {
		cmd_buf[0] = info->cur_index % 0x80;
	}
	if (info->cur_index == info->end_index) {
		cmd_buf[0] |= 0x80;
	}

	if (info->sent_fw_len + data_len <= info->fw_len) {
		/* within fw patch domain */
		memcpy(data_buf, info->patch_buf + info->sent_fw_len, data_len);

		if (info->sent_fw_len + data_len > info->fw_len - 4) {
			/* need copy part lmp subversion */
			lmp_copy_length = 4 - (info->fw_len - info->sent_fw_len - data_len);
			memcpy(data_buf + data_len - lmp_copy_length, lmp_buf, lmp_copy_length);
		}
	} else if ((info->sent_fw_len < info->fw_len) && (info->sent_fw_len + data_len > info->fw_len)) {
		/* need copy fw patch domain and config domain */
		memcpy(data_buf, info->patch_buf + info->sent_fw_len, info->fw_len - info->sent_fw_len);

		if (info->sent_fw_len + data_len - info->fw_len < 4) {
			lmp_copy_length = info->sent_fw_len + data_len - info->fw_len;
		} else {
			lmp_copy_length = 4;
		}
		/* need copy lmp subversion */
		memcpy(data_buf + (info->fw_len - info->sent_fw_len) - lmp_copy_length, lmp_buf + (4 - lmp_copy_length), lmp_copy_length);

		memcpy(data_buf + (info->fw_len - info->sent_fw_len), info->config_buf,
			   data_len - (info->fw_len - info->sent_fw_len));
	} else {
		memcpy(data_buf, info->config_buf + (info->sent_fw_len - info->fw_len), data_len);
	}

	info->sent_fw_len += data_len;
	info->cur_index++;

	return HCI_SUCCESS;
}

uint8_t hci_patch_download_v1(uint16_t opcode, uint8_t *p_patch, uint32_t patch_len)
{
	/* OpCode: 0xFC20, Data Len: Cmd(256), Event(7) */
	uint8_t ret = HCI_SUCCESS;
	uint8_t buf_raw[RESERVE_LEN + 256];
	uint8_t *buf = buf_raw + RESERVE_LEN;

	ret = hci_patch_download_init();
	if (HCI_SUCCESS != ret) {
		goto dl_patch_done;
	}

	while (1) {
		buf[0] = (uint8_t)(opcode >> 0);
		buf[1] = (uint8_t)(opcode >> 8);
		ret = hci_get_patch_cmd_len(&buf[2], p_patch, patch_len);
		if (HCI_SUCCESS != ret) {
			goto dl_patch_done;
		}

		ret = hci_get_patch_cmd_buf(&buf[3], buf[2]);
		if (HCI_SUCCESS != ret) {
			goto dl_patch_done;
		}

		ret = hci_sa_send(HCI_CMD, buf, buf[2] + 3, true);
		if (HCI_SUCCESS != ret) {
			goto dl_patch_done;
		}

		/* Check Resp: OpCode and Status */
		if (buf[3] != (uint8_t)(opcode >> 0) || buf[4] != (uint8_t)(opcode >> 8) || buf[5] != 0x00) {
			goto dl_patch_done;
		}

		/* Check the last patch fragment */
		if (buf[6] & 0x80) {
			break;
		}
	}

dl_patch_done:
	hci_patch_download_done();

	return ret;
}