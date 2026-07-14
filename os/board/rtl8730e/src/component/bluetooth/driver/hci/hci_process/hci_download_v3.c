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

#define HCI_PATCH_FRAG_SIZE         252
#define HCI_PATCH_IMAGE_INFO_LEN    1024

/* section fomart: ... [regaddr(8bytes) regvalue(8bytes)] index(1byte) image_len(8bytes) payload */
#define REG_VALUE_POSITION(__payload, __idx)   ((__payload) - 9 - ((__idx) * 2 + 1) * 8)
#define REG_ADDR_POSITION(__payload, __idx)    ((__payload) - 9 - ((__idx) * 2 + 2) * 8)

enum {
	/* enum uses reverse order */
	SECURITY_HEADER_BUF_REMAIN_SIZE = 0x00,
	PATCH_START_ADDRESS,
	CURRENT_IMAGE_ID,
	CURRENT_IMAGE_DOWNLOAD_ADDRESS,
	SECTION_REG_NUM,
};

enum {
	OPCODE_PATCH_IMAGE = 0x01,
	OPCODE_PATCH_IMAGE_KR4, /* start from smartplus */
};

enum {
	IMGAE_ID_PLATFORM = 0xF000,
	IMGAE_ID_154 = 0xF001,
	IMGAE_ID_LOWERSTACK = 0xF002,
};

struct SECTION_NODE {
	struct list_head list;
	uint8_t index;
	uint32_t length;
	uint8_t *payload;
	uint16_t image_id;  /* always 0 when opcode=OPCODE_PATCH_IMAGE */
	uint8_t config_rule;/* always 0 when opcode=OPCODE_PATCH_IMAGE */
	uint8_t need_config;/* always 0 when opcode=OPCODE_PATCH_IMAGE */
};

struct SECTION_NODE section_head = {0};

static uint8_t hci_vendor_write(uint32_t addr, uint32_t value)
{
	/* OpCode: 0xFC62, Data Len: Cmd(12), Event(5) */
	uint8_t buf_raw[RESERVE_LEN + 12];
	uint8_t *buf = buf_raw + RESERVE_LEN;

	buf[2] = (uint8_t)(9);
	buf[3] = (uint8_t)(0x21);
	for (uint8_t i = 0; i < 4; i++) {
		buf[4 + i] = (uint8_t)(addr >> (8 * i));
		buf[8 + i] = (uint8_t)(value >> (8 * i));
	}

	BT_DUMPD("hci_vendor_write HCI CMD: ", buf, buf[2] + 3);

	return hci_sa_send_cmd_sync(0xFC62, buf, 12);
}

static uint32_t _parse_section(uint8_t *section)
{
	struct SECTION_NODE *node, *p, *n;
	uint8_t ic_cut, key_id, ota_en;
	uint32_t length;

	/* ChipID(2) ICcut(1) KeyID(1) OTA(1) ImageID(2) CfgRule(1) NeedCfg(1) */
	ic_cut = section[2];
	key_id = section[3];
	ota_en = section[4];

	if (key_id != 0 || ota_en != 0) {
		BT_LOGE("Mismatch key id / ota en!\r\n");
		return 0;
	}

	if (ic_cut != hci_patch_get_chipid()) {
		/* Drop section whose ic cut mismatch */
		BT_LOGD("Mismatch ic cut!\r\n");
		return 0;
	}

	LE_TO_UINT32(length, section + HCI_PATCH_IMAGE_INFO_LEN);
	if (length == 0) {
		/* Drop section whose length is 0 */
		BT_LOGD("Section data length is 0.\r\n");
		return 0;
	}

	node = (struct SECTION_NODE *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct SECTION_NODE));
	if (node == NULL) {
		BT_LOGE("Section node allocate fail!\r\n");
		return 0;
	}
	memset(node, 0, sizeof(struct SECTION_NODE));

	node->length = length;
	LE_TO_UINT16(node->image_id, section + 5);
	node->config_rule = section[7];
	node->need_config = section[8];
	node->index = section[HCI_PATCH_IMAGE_INFO_LEN - 1];
	node->payload = section + HCI_PATCH_IMAGE_INFO_LEN + 8;

	/* insert section to list in image, sorted by index */
	list_for_each_entry_safe(p, n, &section_head.list, list, struct SECTION_NODE) {
		if ((p->image_id > node->image_id) || (p->image_id == node->image_id && p->index > node->index)) {
			break;
		}
	}
	list_add_tail(&node->list, &p->list);

	return node->length;
}

static uint8_t hci_patch_parse(uint8_t *p_patch, uint32_t *p_opcode)
{
	uint32_t version_date, version_time;
	uint8_t *p_section;
	uint32_t fw_len = 0, section_num, opcode, payload_len, i;

	/* signature(8) data(4) time(4) rsvd(4) SectionNum(4) */
	LE_TO_UINT32(version_date, p_patch + 8);
	LE_TO_UINT32(version_time, p_patch + 12);
	BT_LOGA("FW Version: %08d%06d\r\n", version_date, version_time);

	LE_TO_UINT32(section_num, p_patch + 20);
	if (section_num == 0) {
		BT_LOGE("Section num 0!\r\n");
		return HCI_FAIL;
	}

	p_section = p_patch + 24;
	INIT_LIST_HEAD(&section_head.list);
	/* opcode(4) length(8) playload(length) */
	for (i = 0; i < section_num; i++) {
		LE_TO_UINT32(opcode, p_section);
		if (i == 0) {
			*p_opcode = opcode;
			BT_LOGD("Section opcode %d!\r\n", opcode);
		} else if (*p_opcode != opcode) {
			BT_LOGE("Opcode mismatch %d %d\r\n", *p_opcode, opcode);
			return HCI_FAIL;
		}

		fw_len += _parse_section(p_section + 12);
		LE_TO_UINT32(payload_len, p_section + 4);
		p_section += (12 + payload_len);
	}

	if (fw_len == 0) {
		BT_LOGE("Available patch not found!\r\n");
		return HCI_IGNORE;
	}
	BT_LOGA("FW Length: %d\r\n", fw_len);

	return HCI_SUCCESS;
}

static uint8_t _send_payload(uint8_t *data, uint32_t len)
{
	uint8_t buf_raw[RESERVE_LEN + 256];
	uint8_t *cmd = buf_raw + RESERVE_LEN;
	uint8_t send, idx = 0;
	uint32_t remain = len;

	while (remain) {
		idx++;
		idx = (idx & 0x80) ? 1 : idx; /* valid range 1~0x7F */
		send = (remain < HCI_PATCH_FRAG_SIZE) ? remain : HCI_PATCH_FRAG_SIZE;
		memcpy(&cmd[4], data + len - remain, send);
		remain -= send;
		cmd[2] = send + 1;
		cmd[3] = idx;
		if (HCI_SUCCESS != hci_sa_send_cmd_sync(0xFC20, cmd, send + 4)) {
			return HCI_FAIL;
		}
	}

	return HCI_SUCCESS;
}

static uint8_t _write_regs(struct SECTION_NODE *node)
{
	uint32_t addr, value;
	int i;

	/* select security node, which is needed after all sections are downloaded */
	for (i = SECTION_REG_NUM - 1; i >= 0; i--) {
		LE_TO_UINT32(addr, REG_ADDR_POSITION(node->payload, i));
		if (addr != 0 && addr != 0xffffffff) {
			LE_TO_UINT32(value, REG_VALUE_POSITION(node->payload, i));
			BT_LOGD("Write regs(%d): addr 0x%08x, value 0x%x\r\n", i, addr, value);
			if (HCI_SUCCESS != hci_vendor_write(addr, value)) {
				return HCI_FAIL;
			}
		}
	}

	return HCI_SUCCESS;
}

static void _write_reg_for_security_verify(void)
{
	struct SECTION_NODE *node, *n, *security_node = NULL;
	uint32_t addr, value, temp;

	/* select security node, which is needed after all sections are downloaded */
	list_for_each_entry_safe(node, n, &section_head.list, list, struct SECTION_NODE) {
		LE_TO_UINT32(addr, REG_ADDR_POSITION(node->payload, SECURITY_HEADER_BUF_REMAIN_SIZE));
		LE_TO_UINT32(value, REG_VALUE_POSITION(node->payload, SECURITY_HEADER_BUF_REMAIN_SIZE));
		if (addr != 0 && addr != 0xffffffff && value != 0) {
			if ((security_node == NULL) || (node->index > security_node->index)) {
				security_node = node;
			}
		}
	}

	if (security_node) {
		LE_TO_UINT32(addr, REG_ADDR_POSITION(security_node->payload, PATCH_START_ADDRESS));
		LE_TO_UINT32(temp, REG_VALUE_POSITION(security_node->payload, PATCH_START_ADDRESS));
		value = temp + security_node->length;
		LE_TO_UINT32(temp, REG_VALUE_POSITION(security_node->payload, SECURITY_HEADER_BUF_REMAIN_SIZE));
		value -= temp;

		hci_vendor_write(addr, value);
	}
}

uint8_t hci_patch_download_v3(uint8_t *p_patch)
{
	uint8_t ret = HCI_FAIL;
	struct SECTION_NODE *node, *n;
	uint8_t cmd[16];
	bool add_config = true;
	uint8_t *cfg;
	uint16_t cfg_len;
	uint32_t opcode, image_total_len = 0;

	if (HCI_SUCCESS != hci_patch_parse(p_patch, &opcode)) {
		goto dl_patch_done;
	}

	list_for_each_entry_safe(node, n, &section_head.list, list, struct SECTION_NODE) {
		BT_LOGD("download: imageID 0x%x, index %d, length %d, cfgRule %d, needCfg %d, \r\n", node->image_id, node->index, node->length, node->config_rule,
				node->need_config);
		/* write registers if address is valid */
		if (HCI_SUCCESS != _write_regs(node)) {
			goto dl_patch_done;
		}
		/* send section content to controller */
		if (HCI_SUCCESS != _send_payload(node->payload, node->length)) {
			goto dl_patch_done;
		}
		/* send config data
		 * config_rule & image_id are always 0 when opcode = 1 */
		if ((opcode == OPCODE_PATCH_IMAGE && add_config) || (node->config_rule && node->need_config)) {
			add_config = false;
			hci_platform_get_config(&cfg, &cfg_len); /* ToDo: get config by image id */
			if (HCI_SUCCESS != _send_payload(cfg, cfg_len)) {
				goto dl_patch_done;
			}
		}
		/* send 0xfc20 to indiacate all sections with this image ID are sent */
		if (opcode == OPCODE_PATCH_IMAGE_KR4) {
			image_total_len += node->length;
			if (node->image_id != list_entry(node->list.next, struct SECTION_NODE, list)->image_id) {
				BT_LOGA("FW Image(0x%04x) Length %d\r\n", node->image_id, image_total_len);
				image_total_len = 0;
				cmd[3] = 1;
				cmd[4] = 0x80;
				if (HCI_SUCCESS != hci_sa_send_cmd_sync(0xFC20, cmd + 1, 4)) {
					goto dl_patch_done;
				}
			}
		}
	}

	/* Last step: trigger controller to verify */
	if (opcode == OPCODE_PATCH_IMAGE) {
		_write_reg_for_security_verify();
		cmd[3] = 1;
		cmd[4] = 0x80;
		ret = hci_sa_send_cmd_sync(0xFC20, cmd + 1, 4);
	} else if (opcode == OPCODE_PATCH_IMAGE_KR4) {
		cmd[3] = 2;
		cmd[4] = 0x3;
		cmd[5] = 0xB2;
		ret = hci_sa_send_cmd_sync(0xFC8E, cmd + 1, 5);
	}

dl_patch_done:
	list_for_each_entry_safe(node, n, &section_head.list, list, struct SECTION_NODE) {
		osif_mem_free(node);
	}
	return ret;
}

#if defined(hci_platfrom_DOWNLOAD_PATCH_TO_SHRMEM) && hci_platfrom_DOWNLOAD_PATCH_TO_SHRMEM
#define BT_IMEM_HEADER_SIZE 32
#define PATCH_IMAGE_HDR_SIZE 785
uint8_t hci_patch_download_v3_to_share_mem(void)
{
	uint8_t ret = HCI_FAIL;
	struct SECTION_NODE *node, *n;
	uint32_t value, image_total_len = 0;
	uint8_t *p_patch;
	uint8_t *shrmem = 0;
	uint32_t opcode;
	uint32_t *imem = (uint32_t *)BT_SHRMEM_SYSADDR_IMEM;

	if (PATCH_VERSION_V3 != hci_patch_get_patch_version(&p_patch, NULL)) {
		BT_LOGE("Signature check success: Merge patch v1 not support\r\n");
		return HCI_FAIL;
	}

	if (HCI_SUCCESS != hci_patch_parse(p_patch, &opcode) || opcode != OPCODE_PATCH_IMAGE_KR4) {
		goto dl_patch_done;
	}

	memset(BT_SHRMEM_SYSADDR_IMEM, 0, BT_IMEM_HEADER_SIZE); /* First 32 bytes in IMEM is reserved */
	list_for_each_entry_safe(node, n, &section_head.list, list, struct SECTION_NODE) {
		BT_LOGD("download: imageID 0x%x, index %d, length %d, cfgRule %d, needCfg %d, \r\n", node->image_id, node->index, node->length, node->config_rule,
				node->need_config);

		/* first section with image ID */
		if (node->image_id != list_entry(node->list.prev, struct SECTION_NODE, list)->image_id) {
			image_total_len = 0;
			LE_TO_UINT32(value, REG_VALUE_POSITION(node->payload, PATCH_START_ADDRESS));
			shrmem = BT_SHRMEM_SYSADDR_IMEM + (value - (uint32_t)BT_SHRMEM_BTADDR_IMEM);
			node->payload += PATCH_IMAGE_HDR_SIZE;
			node->length -= PATCH_IMAGE_HDR_SIZE;
		}

		/* copy payload */
		memcpy(shrmem, node->payload, node->length);
		DCache_CleanInvalidate((u32)shrmem, node->length);
		shrmem += node->length;
		if (shrmem > BT_SHARE_MEM_PATCH_END) {
			BT_LOGE("Patch too long!!\r\n");
		}

		/* last section with image ID, calculate HASH */
		image_total_len += node->length;
		if (node->image_id != list_entry(node->list.next, struct SECTION_NODE, list)->image_id) {
			BT_LOGA("FW Image(0x%04x) Length %d\r\n", node->image_id, image_total_len);
			*imem |= BIT(node->image_id - IMGAE_ID_PLATFORM + 19); /* image valid bit */
		}
	}

	*imem |= BIT(6); /* indicate BT host is existed */
	DCache_CleanInvalidate((u32)imem, BT_IMEM_HEADER_SIZE);

	ret = HCI_SUCCESS;

dl_patch_done:
	list_for_each_entry_safe(node, n, &section_head.list, list, struct SECTION_NODE) {
		osif_mem_free(node);
	}
	return ret;
}
#endif