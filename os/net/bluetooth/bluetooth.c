/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Ported from the Intel/Zephyr zephyr/subsys/bluetooth/
 * where the code was released with a compatible 3-clause BSD license:
 *
 *   Copyright (c) 2016, Intel Corporation
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include <string.h>
#include <tinyara/bluetooth/bluetooth.h>
#include <tinyara/bluetooth/conn.h>
#include "bt_hcicore.h"
#include <errno.h>
#include <debug.h>
#include <tinyara/wqueue.h>

extern struct bt_dev_s g_btdev;

struct bt_ad {
	const struct bt_data *data;
	size_t len;
};

/* Work structures: One for high priority and one for low priority work */

static struct work_s g_hip_work;

static bt_ready_cb_t ready_cb;

FAR const char *bt_addr_le_str(FAR const bt_addr_le_t *addr)
{
	static char bufs[2][27];
	static uint8_t cur;
	FAR char *str;

	str = bufs[cur++];
	cur %= ARRAY_SIZE(bufs);
	bt_addr_le_to_str(addr, str, sizeof(bufs[cur]));

	return str;
}

static int set_advertise_enable(bool enable)
{
	struct bt_buf_s *buf;
	int err;

	buf = bt_hci_cmd_create(BT_HCI_OP_LE_SET_ADV_ENABLE, 1);
	if (!buf) {
		return -ENOBUFS;
	}

	memcpy(bt_buf_extend(buf, 1), &g_btdev.adv_enable, 1);

	err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_SET_ADV_ENABLE, buf, NULL);
	if (err) {
		return err;
	}

	g_btdev.adv_enable = enable;

	if (enable) {
		bt_atomic_testsetbit(g_btdev.flags, BT_DEV_ADVERTISING);
	} else {
		bt_atomic_testclrbit(g_btdev.flags, BT_DEV_ADVERTISING);
	}

	return 0;
}

static int set_ad(unsigned short hci_op, const struct bt_ad *ad, size_t ad_len)
{
	struct bt_hci_cp_le_set_adv_data_s *set_data;
	struct bt_buf_s *buf;
	int c, i;

	buf = bt_hci_cmd_create(hci_op, sizeof(*set_data));
	if (!buf) {
		return -ENOBUFS;
	}

	set_data = bt_buf_extend(buf, sizeof(*set_data));

	(void)memset(set_data, 0, sizeof(*set_data));

	for (c = 0; c < ad_len; c++) {
		const struct bt_data *data = ad[c].data;

		for (i = 0; i < ad[c].len; i++) {
			int len = data[i].data_len;
			unsigned char type = data[i].type;

			/* Check if ad fit in the remaining buffer */
			if (set_data->len + len + 2 > 31) {
				len = 31 - (set_data->len + 2);
				if (type != BT_DATA_NAME_COMPLETE || !len) {
					bt_buf_release(buf);
					BT_ERR("Too big advertising data");
					return -EINVAL;
				}
				type = BT_DATA_NAME_SHORTENED;
			}

			set_data->data[set_data->len++] = len + 1;
			set_data->data[set_data->len++] = type;

			memcpy(&set_data->data[set_data->len], data[i].data, len);
			set_data->len += len;
		}
	}

	return bt_hci_cmd_send_sync(hci_op, buf, NULL);
}

int bt_set_name(const char *name)
{
	size_t len = strlen(name);

	if (len >= sizeof(g_btdev.name)) {
		return -ENOMEM;
	}

	if (!strcmp(g_btdev.name, name)) {
		return 0;
	}

	strncpy(g_btdev.name, name, sizeof(g_btdev.name));

	/* Update advertising name if in use */

	if (bt_atomic_testbit(g_btdev.flags, BT_DEV_ADVERTISING_NAME)) {
		struct bt_data data[] = { BT_DATA(BT_DATA_NAME_COMPLETE, name,
											  strlen(name))
		};
		struct bt_ad sd = { data, ARRAY_SIZE(data) };

		set_ad(BT_HCI_OP_LE_SET_SCAN_RSP_DATA, &sd, 1);

		/* Make sure the new name is set */
		if (bt_atomic_testbit(g_btdev.flags, BT_DEV_ADVERTISING)) {
			set_advertise_enable(false);
			set_advertise_enable(true);
		}
	}

	return 0;
}

const char *bt_get_name(void)
{
	return g_btdev.name;
}

static const char *ver_str(uint8_t ver)
{
	const char *const str[] = {
		"1.0b", "1.1", "1.2", "2.0", "2.1", "3.0", "4.0", "4.1", "4.2",
		"5.0", "5.1",
	};

	if (ver < ARRAY_SIZE(str)) {
		return str[ver];
	}

	return "unknown";
}

static void bt_dev_show_info(void)
{
	int i;

	BT_INFO("Identity%s: %s", g_btdev.id_count > 1 ? "[0]" : "", bt_addr_le_str(&g_btdev.id_addr[0]));

	for (i = 1; i < g_btdev.id_count; i++) {
		BT_INFO("Identity[%d]: %s", i, bt_addr_le_str(&g_btdev.id_addr[i]));
	}

	BT_INFO("HCI: version %s (0x%02x) revision 0x%04x, manufacturer 0x%04x", ver_str(g_btdev.hci_version), g_btdev.hci_version, g_btdev.hci_revision, g_btdev.manufacturer);
}

static void bt_finalize_init(void)
{
	bt_atomic_testsetbit(g_btdev.flags, BT_DEV_READY);
#if 0
	if (IS_ENABLED(CONFIG_BT_OBSERVER)) {
		bt_le_scan_update(false);
	}
#endif
	bt_dev_show_info();
}

static int bt_conn_init(void)
{
	int err;

	err = bt_l2cap_init();
	if (err < 0) {
		ndbg("ERROR:  l2cap init failed: %d\n", err);
		return err;
	}

	return err;
}

static int bt_init(void)
{
	int ret;

	ret = hci_initialize();
	if (ret < 0) {
		ndbg("ERROR:  hci_initialize failed: %d\n", ret);
		return ret;
	}

	ret = bt_conn_init();
	if (ret < 0) {
		ndbg("ERROR:  bt_conn_init failed: %d\n", ret);
		return ret;
	}

	bt_finalize_init();

	return ret;
}

static void init_work(FAR void *arg)
{
	int err;

	err = bt_init();

	if (ready_cb) {
		ready_cb(err);
	}
}

static void k_work_submit(void)
{
	int err;

	if (work_available(&g_hip_work)) {
		err = work_queue(HPWORK, &g_hip_work, init_work, NULL, 0);
		if (err < 0) {
			ndbg("ERROR:  Failed to schedule HPWORK: %d\n", err);
		}
	}
}

int bt_enable(bt_ready_cb_t cb)
{
	int err;
	FAR const struct bt_driver_s *bt_drv = g_btdev.btdev;

	if (!bt_drv) {
		BT_ERR("No HCI driver registered");
		return -ENODEV;
	}

	if (bt_atomic_testbit(g_btdev.flags, BT_DEV_ENABLE)) {
		return -EALREADY;
	}

	bt_atomic_testsetbit(g_btdev.flags, BT_DEV_ENABLE);

	bt_set_name(CONFIG_BT_DEVICE_NAME);

	ready_cb = cb;

	err = bt_drv->open(bt_drv);
	if (err) {
		BT_ERR("HCI driver open failed (%d)", err);
		return err;
	}

	if (!cb) {
		return bt_init();
	}

	k_work_submit();

	return 0;
}

int bt_set_id_addr(const bt_addr_le_t *addr)
{
	// TODO: need to implement

	return 0;
}

void bt_id_get(bt_addr_le_t *addrs, size_t *count)
{
	// TODO: need to implement
	return;
}

int bt_id_create(bt_addr_le_t *addr, uint8_t *irk)
{
	// TODO: need to implement
	return 0;
}

int bt_id_reset(uint8_t id, bt_addr_le_t *addr, uint8_t *irk)
{
	// TODO: need to implement
	return 0;
}

int bt_id_delete(uint8_t id)
{
	// TODO: need to implement
	return 0;
}

int bt_le_adv_update_data(const struct bt_data *ad, size_t ad_len, const struct bt_data *sd, size_t sd_len)
{
	// TODO: need to implement
	return 0;
}

int bt_le_set_chan_map(uint8_t chan_map[5])
{
	// TODO: need to implement
	return 0;
}

/*
void bt_data_parse(struct net_buf_simple *ad, bool (*func)(struct bt_data *data, void *user_data), void *user_data)
{
	// TODO: need to implement
	return;
}
*/

int bt_le_oob_get_local(uint8_t id, struct bt_le_oob *oob)
{
	// TODO: need to implement
	return 0;
}

int bt_br_discovery_start(const struct bt_br_discovery_param *param, struct bt_br_discovery_result *results, size_t count, bt_br_discovery_cb_t cb)
{
	// TODO: need to implement
	return 0;
}

int bt_br_discovery_stop(void)
{
	// TODO: need to implement
	return 0;
}

int bt_br_oob_get_local(struct bt_br_oob *oob)
{
	// TODO: need to implement
	return 0;
}

int bt_br_set_discoverable(bool enable)
{
	// TODO: need to implement
	return 0;
}

int bt_br_set_connectable(bool enable)
{
	// TODO: need to implement
	return 0;
}

int bt_unpair(uint8_t id, const bt_addr_le_t *addr)
{
	// TODO: need to implement
	return 0;
}

void bt_foreach_bond(uint8_t id, void (*func)(const struct bt_bond_info *info, void *user_data), void *user_data)
{
	// TODO: need to implement
	return;
}

struct bt_conn *bt_conn_ref(struct bt_conn *conn)
{
	// TODO: need to implement
	return NULL;
}

void bt_conn_unref(struct bt_conn *conn)
{
	// TODO: need to implement
	return;
}

struct bt_conn *bt_conn_lookup_addr_le(uint8_t id, const bt_addr_le_t *peer)
{
	// TODO: need to implement
	return NULL;
}

const bt_addr_le_t *bt_conn_get_dst(const struct bt_conn *conn)
{
	// TODO: need to implement
	return NULL;
}

uint8_t bt_conn_index(struct bt_conn *conn)
{
	// TODO: need to implement
	return 0;
}

int bt_conn_get_info(const struct bt_conn *conn, struct bt_conn_info *info)
{
	// TODO: need to implement
	return 0;
}

int bt_conn_le_param_update(struct bt_conn *conn, const struct bt_le_conn_param *param)
{
	// TODO: need to implement
	return 0;
}

int bt_conn_disconnect(struct bt_conn *conn, uint8_t reason)
{
	// TODO: need to implement
	return 0;
}

struct bt_conn *bt_conn_create_le(const bt_addr_le_t *peer, const struct bt_le_conn_param *param)
{
	// TODO: need to implement
	return NULL;
}

int bt_le_set_auto_conn(const bt_addr_le_t *addr, const struct bt_le_conn_param *param)
{
	// TODO: need to implement
	return 0;
}

struct bt_conn *bt_conn_create_slave_le(const bt_addr_le_t *peer, const struct bt_le_adv_param *param)
{
	// TODO: need to implement
	return NULL;
}

int bt_conn_security(struct bt_conn *conn, bt_security_t sec)
{
	// TODO: need to implement
	return 0;
}

uint8_t bt_conn_enc_key_size(struct bt_conn *conn)
{
	// TODO: need to implement
	return 0;
}

/* void bt_conn_cb_register(struct bt_conn_cb *cb)
{
	// TODO: need to implement
	return;
}*/

void bt_set_bondable(bool enable)
{
	// TODO: need to implement
	return;
}

void bt_set_oob_data_flag(bool enable)
{
	// TODO: need to implement
	return;
}

int bt_le_oob_set_sc_data(struct bt_conn *conn, const struct bt_le_oob_sc_data *oobd_local, const struct bt_le_oob_sc_data *oobd_remote)
{
	// TODO: need to implement
	return 0;
}

int bt_le_oob_get_sc_data(struct bt_conn *conn, const struct bt_le_oob_sc_data **oobd_local, const struct bt_le_oob_sc_data **oobd_remote)
{
	// TODO: need to implement
	return 0;
}

int bt_passkey_set(unsigned int passkey)
{
	// TODO: need to implement
	return 0;
}

int bt_conn_auth_cb_register(const struct bt_conn_auth_cb *cb)
{
	// TODO: need to implement
	return 0;
}

int bt_conn_auth_passkey_entry(struct bt_conn *conn, unsigned int passkey)
{
	// TODO: need to implement
	return 0;
}

int bt_conn_auth_cancel(struct bt_conn *conn)
{
	// TODO: need to implement
	return 0;
}

int bt_conn_auth_passkey_confirm(struct bt_conn *conn)
{
	// TODO: need to implement
	return 0;
}

int bt_conn_auth_pairing_confirm(struct bt_conn *conn)
{
	// TODO: need to implement
	return 0;
}

int bt_conn_auth_pincode_entry(struct bt_conn *conn, const char *pin)
{
	// TODO: need to implement
	return 0;
}

struct bt_conn *bt_conn_create_br(const bt_addr_t *peer, const struct bt_br_conn_param *param)
{
	// TODO: need to implement
	return NULL;
}

struct bt_conn *bt_conn_create_sco(const bt_addr_t *peer)
{
	// TODO: need to implement
	return NULL;
}
