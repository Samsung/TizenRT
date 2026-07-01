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

#define RESERVE_LEN 1

#define HCI_PATCH_FRAG_SIZE 252

typedef enum {
	OPCODE_PATCH_SNIPPETS = 0x01,
	OPCODE_DUMMY_HEADER = 0x02,
	OPCODE_SECURITY_HEADER = 0x03,
	OPCODE_OTA_FLAG = 0x04,
	OPCODE_CONTROLLER_RSVD = 0x08
} SECTION_OPCODE;

struct PATCH_NODE {
	struct list_head list;
	uint8_t eco;
	uint8_t priority;
	uint8_t key_id;
	uint8_t reserve;
	uint32_t payload_len;
	uint8_t *payload;
	uint32_t sent_payload_len;
};

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
	struct PATCH_NODE head_node;
};

static struct PATCH_INFO *patch_info = NULL;
static uint8_t patch_key_id = 0;

static uint8_t hci_patch_download_init(void)
{
	patch_info = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct PATCH_INFO));
	if (!patch_info) {
		return HCI_FAIL;
	}

	memset(patch_info, 0, sizeof(struct PATCH_INFO));
	INIT_LIST_HEAD(&patch_info->head_node.list);

	return HCI_SUCCESS;
}

static void hci_patch_download_done(void)
{
	struct list_head *pos, *next;
	struct PATCH_NODE *node;

	list_for_each_safe(pos, next, &patch_info->head_node.list) {
		node = list_entry(pos, struct PATCH_NODE, list);
		list_del_init(pos);
		osif_mem_free(node);
	}

	if (patch_info) {
		osif_mem_free(patch_info);
	}
	patch_info = NULL;
}

static void _insert_patch_queue(struct list_head *head, struct PATCH_NODE *p_patch_node)
{
	struct list_head *pos, *next;
	struct PATCH_NODE *node;

	if (!head || !p_patch_node) {
		return;
	}

	list_for_each_safe(pos, next, head) {
		node = list_entry(pos, struct PATCH_NODE, list);
		if (node->priority >= p_patch_node->priority) {
			break;
		}
	}

	__list_add(&p_patch_node->list, pos->prev, pos);
}

static void _parse_patch_section(uint8_t *p_buf, uint32_t *p_fw_len, SECTION_OPCODE opcode, bool *p_found_security_header,
								 struct PATCH_NODE *p_patch_node_head)
{
	struct PATCH_NODE *patch_node;
	uint16_t number, reserve;
	uint8_t *position;
	uint8_t eco;
	uint32_t payload_len;

	LE_TO_UINT16(number, p_buf);

	position = p_buf + sizeof(number) + sizeof(reserve);
	for (uint16_t i = 0; i < number; i++) {
		eco = *(position);
		LE_TO_UINT32(payload_len, position + sizeof(patch_node->eco) + sizeof(patch_node->priority) +
					 sizeof(patch_node->key_id) + sizeof(patch_node->reserve));

		if (eco == hci_patch_get_chipid()) {
			patch_node = (struct PATCH_NODE *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct PATCH_NODE));
			if (patch_node == NULL) {
				BT_LOGE("patch_node allocate fail!\r\n");
				return;
			}
			memset(patch_node, 0, sizeof(struct PATCH_NODE));

			patch_node->eco = eco;
			patch_node->priority = *(position + sizeof(patch_node->eco));
			if (opcode == OPCODE_SECURITY_HEADER) {
				patch_node->key_id = *(position + sizeof(patch_node->eco) + sizeof(patch_node->priority));
			}
			patch_node->payload_len = payload_len;
			patch_node->payload = position + sizeof(patch_node->eco) + sizeof(patch_node->priority) +
								  sizeof(patch_node->key_id) + sizeof(patch_node->reserve) + sizeof(patch_node->payload_len);

			if (opcode == OPCODE_PATCH_SNIPPETS || opcode == OPCODE_DUMMY_HEADER) {
				_insert_patch_queue(&p_patch_node_head->list, patch_node);
				*p_fw_len += payload_len;
			} else if (opcode == OPCODE_SECURITY_HEADER) {
				if (patch_node->key_id == patch_key_id) {
					_insert_patch_queue(&p_patch_node_head->list, patch_node);
					*p_fw_len += payload_len;
					*p_found_security_header = true;
				} else {
					BT_LOGE("patch_node->key_id = 0x%x mismatch patch_key_id = 0x%x\r\n", patch_node->key_id, patch_key_id);
					osif_mem_free(patch_node);
				}
			}
		}

		position += sizeof(patch_node->eco) + sizeof(patch_node->priority) +
					sizeof(patch_node->key_id) + sizeof(patch_node->reserve) + sizeof(patch_node->payload_len) + payload_len;
	}
}

static uint32_t _parse_patch(uint8_t *p_buf, struct PATCH_NODE *p_patch_node_head)
{
	uint32_t i;
	uint32_t section_num;
	uint8_t *p_section;
	uint32_t opcode, length;
	uint32_t fw_len = 0;
	bool found_security_header = false;

	LE_TO_UINT32(section_num, p_buf);

	if (section_num == 0) {
		BT_LOGE("Section num error!\r\n");
		return 0;
	} else {
		p_section = p_buf + sizeof(section_num);
		for (i = 0; i < section_num; i++) {
			LE_TO_UINT32(opcode, p_section);
			LE_TO_UINT32(length, p_section + sizeof(opcode));

			switch (opcode) {
			case OPCODE_PATCH_SNIPPETS:
				_parse_patch_section(p_section + sizeof(opcode) + sizeof(length), &fw_len, OPCODE_PATCH_SNIPPETS, NULL, p_patch_node_head);
				break;
			case OPCODE_DUMMY_HEADER:
				if (patch_key_id != 0) {
					BT_LOGE("patch_key_id = 0x%x, ignore\r\n", patch_key_id);
					break;
				}
				_parse_patch_section(p_section + sizeof(opcode) + sizeof(length), &fw_len, OPCODE_DUMMY_HEADER, NULL, p_patch_node_head);
				break;
			case OPCODE_SECURITY_HEADER:
				if (patch_key_id == 0) {
					BT_LOGE("patch_key_id = 0x%x, ignore\r\n", patch_key_id);
					break;
				}
				_parse_patch_section(p_section + sizeof(opcode) + sizeof(length), &fw_len, OPCODE_SECURITY_HEADER, &found_security_header, p_patch_node_head);
				break;
			case OPCODE_OTA_FLAG:
				BT_LOGE("OTA flag not support\r\n");
				break;
			case OPCODE_CONTROLLER_RSVD:
				break;
			default:
				BT_LOGE("Unknown opcode 0x%x\r\n", opcode);
				break;
			}
			p_section += sizeof(opcode) + sizeof(length) + length;
		}

		/* if has key id but not found security header, parse dummy header again */
		if (patch_key_id != 0 && found_security_header == false) {
			p_section = p_buf + sizeof(section_num);
			for (i = 0; i < section_num; i++) {
				LE_TO_UINT32(opcode, p_section);
				LE_TO_UINT32(length, p_section + sizeof(opcode));

				if (opcode == OPCODE_DUMMY_HEADER) {
					_parse_patch_section(p_section + sizeof(opcode) + sizeof(length), &fw_len, OPCODE_DUMMY_HEADER, NULL, p_patch_node_head);
				}
				p_section += sizeof(opcode) + sizeof(length) + length;
			}
		}
	}

	return fw_len;
}

static uint8_t _get_patch_info(uint8_t *p_patch, uint32_t patch_len)
{
	struct PATCH_INFO *info = patch_info;
	uint32_t version_date, version_time;
	uint32_t fw_len;
	uint8_t sig_len = 8;

	info->patch_buf = p_patch;
	info->patch_len = patch_len;

	LE_TO_UINT32(version_date, info->patch_buf + sig_len);
	LE_TO_UINT32(version_time, info->patch_buf + sig_len + sizeof(version_date));
	BT_LOGA("FW Version: %08d%06d\r\n", version_date, version_time);

	fw_len = _parse_patch(info->patch_buf + sig_len + sizeof(version_date) + sizeof(version_time), &info->head_node);
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
	uint8_t remain_len = data_len;
	uint8_t sending_len = 0;
	uint32_t total_node_len = 0;
	struct list_head *pos, *next;
	struct PATCH_NODE *node;

	/* first byte is index */
	if (info->cur_index >= 0x80) {
		cmd_buf[0] = (info->cur_index - 0x80) % 0x7f + 1;
	} else {
		cmd_buf[0] = info->cur_index % 0x80;
	}
	if (info->cur_index == info->end_index) {
		cmd_buf[0] |= 0x80;
	}

	list_for_each_safe(pos, next, &info->head_node.list) {
		node = list_entry(pos, struct PATCH_NODE, list);
		total_node_len += node->payload_len;

		/* Find the patch node need to be send */
		if (info->sent_fw_len < total_node_len) {
			if (info->sent_fw_len + remain_len < total_node_len) {
				sending_len = remain_len;
				memcpy(data_buf + data_len - remain_len, node->payload + node->sent_payload_len, sending_len);
				info->sent_fw_len += sending_len;
				node->sent_payload_len += sending_len;
				remain_len -= sending_len;
				/* data_buf is already full, break the loop */
				break;
			} else {
				sending_len = node->payload_len - node->sent_payload_len;
				memcpy(data_buf + data_len - remain_len, node->payload + node->sent_payload_len, sending_len);
				info->sent_fw_len += sending_len;
				node->sent_payload_len += sending_len;
				remain_len -= sending_len;
				if (node->payload_len != node->sent_payload_len) {
					BT_LOGE("Patch node has not been sent completely! payload_len = %d, sent_payload_len = %d\r\n", node->payload_len, node->sent_payload_len);
					return HCI_FAIL;
				}
				/* data_buf is not full, jump to the next patch node */
			}
		}
	}

	if (remain_len > 0) {
		if (info->fw_len != info->sent_fw_len) {
			BT_LOGE("Firmware has not been sent completely! fw_len = %d, sent_fw_len = %d\r\n", info->fw_len, info->sent_fw_len);
			return HCI_FAIL;
		}

		/* Add config data after firmware */
		memcpy(data_buf + data_len - remain_len, info->config_buf + info->sent_config_len, remain_len);
		info->sent_config_len += remain_len;
	}

	if (info->cur_index == info->end_index) {
		if (info->config_len != info->sent_config_len) {
			BT_LOGE("Config data has not been sent completely! config_len = %d, sent_config_len = %d\r\n", info->config_len, info->sent_config_len);
			return HCI_FAIL;
		}
	} else {
		info->cur_index++;
	}

	return HCI_SUCCESS;
}

uint8_t hci_patch_download_v2(uint16_t opcode, uint8_t *p_patch, uint32_t patch_len)
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