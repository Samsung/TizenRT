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

#include <tinyara/config.h>

#include <string.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/bluetooth/bluetooth.h>
#include <tinyara/bluetooth/conn.h>
#include <tinyara/wqueue.h>
#include <tinyara/bluetooth/bt_core.h>
#include <tinyara/bluetooth/bt_hci.h>
#include <tinyara/random.h>
#include <tinyara/bluetooth/iob/iob.h>

#include "bt_buf.h"
#include "bt_keys.h"
#include "bt_conn.h"
#include "bt_l2cap.h"
#include "bt_hcicore.h"

#ifdef CONFIG_BLUETOOTH_NULL
#include <tinyara/bluetooth/bt_null.h>
#endif

extern struct bt_dev_s g_btdev;

static bt_le_scan_cb_t *scan_dev_found_cb;

struct bt_ad {
	const struct bt_data *data;
	size_t len;
};

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* Work structures: One for high priority and one for low priority work */

static struct work_s g_lowp_work;

static bt_ready_cb_t ready_cb;

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
		bt_atomic_setbit(g_btdev.flags, BT_DEV_ADVERTISING);
	} else {
		bt_atomic_clrbit(g_btdev.flags, BT_DEV_ADVERTISING);
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
					ndbg("Too big advertising data");
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

#ifdef CONFIG_BLUETOOTH_NULL
static int write_local_name(const char *name)
{
	struct bt_buf_s *buf;
	struct hci_cp_write_local_name_s *name_cp;
	int err;

	/* Set local name */
	buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_LOCAL_NAME, sizeof(*name_cp));
	if (!buf) {
		return -ENOBUFS;
	}

	name_cp = bt_buf_extend(buf, sizeof(*name_cp));
	strncpy((char *)name_cp->local_name, name, sizeof(name_cp->local_name));

	err = bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_LOCAL_NAME, buf, NULL);
	if (err) {
		return err;
	}

	return 0;
}
#endif

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
		struct bt_data data[] = { BT_DATA(BT_DATA_NAME_COMPLETE,
						name, strlen(name)) };

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

#ifdef CONFIG_BLUETOOTH_NULL
static void read_local_name_complete(FAR struct bt_buf_s *buf)
{
	FAR struct hci_rp_read_local_name_s *rp = (FAR void *)buf->data;

	nvdbg("status %u\n", rp->status);

	memcpy(g_btdev.local_name, rp->local_name, HCI_MAX_NAME_LENGTH);
}

static int read_local_name(void)
{
	struct bt_buf_s *rsp;
	int err;

	err = bt_hci_cmd_send_sync(BT_HCI_OP_READ_LOCAL_NAME, NULL, &rsp);
	if (err) {
		ndbg("ERROR:  bt_hci_cmd_send_sync failed: %d\n", err);
		return err;
	}

	read_local_name_complete(rsp);

	return 0;
}
#endif

const char *bt_get_name(void)
{
#ifdef CONFIG_BLUETOOTH_NULL
	read_local_name();
#endif
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
	char *hci_version;

	nvdbg("Identity%s: %s", g_btdev.id_count > 1 ? "[0]" : "", bt_addr_le_str(&g_btdev.id_addr[0]));

	for (i = 1; i < g_btdev.id_count; i++) {
		nvdbg("Identity[%d]: %s", i, bt_addr_le_str(&g_btdev.id_addr[i]));
	}

	hci_version = (char *)ver_str(g_btdev.hci_version);

	nvdbg("HCI: version %s (0x%02x) revision 0x%04x, manufacturer 0x%04x \n", hci_version, g_btdev.hci_version, g_btdev.hci_revision, g_btdev.manufacturer);
}

static void bt_finalize_init(void)
{
	bt_atomic_setbit(g_btdev.flags, BT_DEV_READY);
#if 0
	if (IS_ENABLED(CONFIG_BT_OBSERVER)) {
		bt_ble_scan_update(false);
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
	int ret = 0;

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

	if (work_available(&g_lowp_work)) {
		err = work_queue(LPWORK, &g_lowp_work, init_work, NULL, 0);
		if (err < 0) {
			ndbg("ERROR:  Failed to schedule HPWORK: %d\n", err);
		}
	}
}

static void bt_common_init(void)
{
	/* It initializes the buf, iob and tx queue and thread.
	   Initialise it before the radio opens */

	bt_buf_initialize();

	iob_initialize();

	cmd_queue_init();
}

int bt_enable(bt_ready_cb_t cb)
{
	int err;

	FAR const struct bt_driver_s *bt_drv = g_btdev.btdev;

	if (!bt_drv) {
		ndbg("No HCI driver registered");
		return -ENODEV;
	}

	if (bt_atomic_testsetbit(g_btdev.flags, BT_DEV_ENABLE)) {
		return -EALREADY;
	}

	bt_common_init();

	bt_set_name(CONFIG_BT_DEVICE_NAME);

	ready_cb = cb;

	err = bt_drv->open(bt_drv);
	if (err) {
		ndbg("HCI driver open failed (%d)", err);
		return err;
	}

	if (!cb) {
		return bt_init();
	}

	k_work_submit();

	return 0;
}

static int bt_rand(void *buf, size_t len)
{
#ifdef CONFIG_CRYPTO_RANDOM_POOL
	getrandom(buf, len);
#endif

	return 0;
}

static inline int create_random_addr(bt_addr_le_t *addr)
{
	addr->type = BT_ADDR_LE_RANDOM;

#if 0
	/* TODO implement the bt_rand */
	return bt_rand(addr->val, 6);
#endif
	return 0;
}

int bt_addr_le_create_static(bt_addr_le_t *addr)
{
	int err;

	err = create_random_addr(addr);
	if (err) {
		return err;
	}

	BT_ADDR_SET_STATIC(addr);

	return 0;
}

int bt_set_id_addr(const bt_addr_le_t *addr)
{
	bt_addr_le_t non_const_addr;

	if (bt_atomic_testbit(g_btdev.flags, BT_DEV_READY)) {
		ndbg("Setting identity not allowed after bt_enable()");
		return -EBUSY;
	}

	bt_addr_le_copy(&non_const_addr, addr);

	return bt_id_create(&non_const_addr, NULL);
}

void bt_id_get(bt_addr_le_t *addrs, size_t *count)
{
	// TODO: need to implement
	return;
}

static int id_find(const bt_addr_le_t *addr)
{
	uint8_t id;

	for (id = 0U; id < g_btdev.id_count; id++) {
		if (!bt_addr_le_cmp(addr, &g_btdev.id_addr[id])) {
			return id;
		}
	}

	return -ENOENT;
}

static void id_create(uint8_t id, bt_addr_le_t *addr, uint8_t *irk)
{
	if (addr && bt_addr_le_cmp(addr, BT_ADDR_LE_ANY)) {
		bt_addr_le_copy(&g_btdev.id_addr[id], addr);
	} else {
		bt_addr_le_t new_addr;

		do {
			bt_addr_le_create_static(&new_addr);
			/* Make sure we didn't generate a duplicate */
		} while (id_find(&new_addr) >= 0);

		bt_addr_le_copy(&g_btdev.id_addr[id], &new_addr);

		if (addr) {
			bt_addr_le_copy(addr, &g_btdev.id_addr[id]);
		}
	}

#if 0
#if defined(CONFIG_BT_PRIVACY)
	{
		u8_t zero_irk[16] = { 0 };

		if (irk && memcmp(irk, zero_irk, 16)) {
			memcpy(&bt_dev.irk[id], irk, 16);
		} else {
			bt_rand(&bt_dev.irk[id], 16);
			if (irk) {
				memcpy(irk, &bt_dev.irk[id], 16);
			}
		}
	}
#endif
	/* Only store if stack was already initialized. Before initialization
	 * we don't know the flash content, so it's potentially harmful to
	 * try to write anything there.
	 */
	if (IS_ENABLED(CONFIG_BT_SETTINGS) && atomic_test_bit(bt_dev.flags, BT_DEV_READY)) {
		bt_settings_save_id();
	}
#endif
}

int bt_id_create(bt_addr_le_t *addr, uint8_t *irk)
{
	int new_id;

	if (addr && bt_addr_le_cmp(addr, BT_ADDR_LE_ANY)) {
		if (addr->type != BT_ADDR_LE_RANDOM || !BT_ADDR_IS_STATIC(addr)) {
			ndbg("Only static random identity address supported");
			return -EINVAL;
		}

		if (id_find(addr) >= 0) {
			return -EALREADY;
		}
	}
#if 0
	if (!IS_ENABLED(CONFIG_BT_PRIVACY) && irk) {
		return -EINVAL;
	}
#endif
	if (g_btdev.id_count == ARRAY_SIZE(g_btdev.id_addr)) {
		return -ENOMEM;
	}

	new_id = g_btdev.id_count++;
	if (new_id == BT_ID_DEFAULT && !bt_atomic_testbit(g_btdev.flags, BT_DEV_READY)) {
		bt_atomic_setbit(g_btdev.flags, BT_DEV_USER_ID_ADDR);
	}

	id_create(new_id, addr, irk);

	return new_id;
}

int bt_id_reset(uint8_t id, bt_addr_le_t *addr, uint8_t *irk)
{
	if (addr && bt_addr_le_cmp(addr, BT_ADDR_LE_ANY)) {
		if (addr->type != BT_ADDR_LE_RANDOM || !BT_ADDR_IS_STATIC(addr)) {
			ndbg("Only static random identity address supported");
			return -EINVAL;
		}

		if (id_find(addr) >= 0) {
			return -EALREADY;
		}
	}
#if 0
	if (!IS_ENABLED(CONFIG_BT_PRIVACY) && irk) {
		return -EINVAL;
	}
#endif
	if (id == BT_ID_DEFAULT || id >= g_btdev.id_count) {
		return -EINVAL;
	}

	if (id == g_btdev.adv_id && bt_atomic_testbit(g_btdev.flags, BT_DEV_ADVERTISING)) {
		return -EBUSY;
	}
#if 0
	if (IS_ENABLED(CONFIG_BT_CONN) &&
#endif
	if (bt_addr_le_cmp(&g_btdev.id_addr[id], BT_ADDR_LE_ANY)) {
		int err;
		err = bt_unpair(id, NULL);
			if (err) {
				return err;
			}
		}

	id_create(id, addr, irk); return id;
}

int bt_id_delete(uint8_t id)
{
	if (id == BT_ID_DEFAULT || id >= g_btdev.id_count) {
		return -EINVAL;
	}

	if (!bt_addr_le_cmp(&g_btdev.id_addr[id], BT_ADDR_LE_ANY)) {
		return -EALREADY;
	}

	if (id == g_btdev.adv_id && bt_atomic_testbit(g_btdev.flags, BT_DEV_ADVERTISING)) {
		return -EBUSY;
	}

#if 0
	if (IS_ENABLED(CONFIG_BT_CONN))
#endif
	{
		int err;
		err = bt_unpair(id, NULL);
		if (err) {
			return err;
		}
	}

#if 0
#if defined(CONFIG_BT_PRIVACY)
	(void)memset(bt_dev.irk[id], 0, 16);
#endif
#endif
	bt_addr_le_copy(&g_btdev.id_addr[id], BT_ADDR_LE_ANY);
	if (id == g_btdev.id_count - 1) {
		g_btdev.id_count--;
	}

#if 0
	if (IS_ENABLED(CONFIG_BT_SETTINGS) && atomic_test_bit(bt_dev.flags, BT_DEV_READY)) {
		bt_settings_save_id();
	}
#endif
	return 0;
}

static int set_random_address(const bt_addr_t *addr)
{
	struct bt_buf_s *buf;
	bt_addr_t *r_addr;
	int err;
	nvdbg("%s", bt_addr_str(addr));
	/* Do nothing if we already have the right address */
	if (!bt_addr_cmp(addr, (bt_addr_t *) g_btdev.random_addr.val)) {
		return 0;
	}

	buf = bt_hci_cmd_create(BT_HCI_OP_LE_SET_RANDOM_ADDRESS, sizeof(*addr));
	if (!buf) {
		return -ENOBUFS;
	}

	r_addr = bt_buf_extend(buf, sizeof(bt_addr_t));
	memcpy(r_addr, addr, sizeof(bt_addr_t));
	err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_SET_RANDOM_ADDRESS, buf, NULL);
	if (err) {
		return err;
	}

	bt_addr_copy((bt_addr_t *) g_btdev.random_addr.val, addr);
	g_btdev.random_addr.type = BT_ADDR_LE_RANDOM;
	return 0;
}

static inline bool ad_has_name(const struct bt_data *ad, size_t ad_len)
{
	int i;
	for (i = 0; i < ad_len; i++) {
		if (ad[i].type == BT_DATA_NAME_COMPLETE || ad[i].type == BT_DATA_NAME_SHORTENED) {
			return true;
		}
	}

	return false;
}

static int le_adv_update(const struct bt_data *ad, size_t ad_len, const struct bt_data *sd, size_t sd_len, bool connectable, bool use_name)
{
	struct bt_ad d[2] = {
	};
	int err;
	d[0].data = ad;
	d[0].len = ad_len;
	err = set_ad(BT_HCI_OP_LE_SET_ADV_DATA, d, 1);
	if (err) {
		return err;
	}

	d[0].data = sd;
	d[0].len = sd_len;
	if (use_name) {
		const char *name;
		if (sd) {
			/* Cannot use name if name is already set */
			if (ad_has_name(sd, sd_len)) {
				return -EINVAL;
			}
		}

		name = bt_get_name();
		d[1].data = (&(struct bt_data)BT_DATA(BT_DATA_NAME_COMPLETE, name, strlen(name)));
		d[1].len = 1;
	}

	/*
	 * We need to set SCAN_RSP when enabling advertising type that
	 * allows for Scan Requests.
	 *
	 * If any data was not provided but we enable connectable
	 * undirected advertising sd needs to be cleared from values set
	 * by previous calls.
	 * Clearing sd is done by calling set_ad() with NULL data and
	 * zero len.
	 * So following condition check is unusual but correct.
	 */
	if (d[0].data || d[1].data || connectable) {
		err = set_ad(BT_HCI_OP_LE_SET_SCAN_RSP_DATA, d, 2);
		if (err) {
			return err;
		}
	}

	return 0;
}

static bool valid_adv_param(const struct bt_le_adv_param *param, bool dir_adv)
{
	/* TODO amit id_addr and adv_id to check more on this */
	if (param->id >= g_btdev.id_count || !bt_addr_le_cmp(&g_btdev.id_addr[param->id], BT_ADDR_LE_ANY)) {
		return false;
	}

	if (!(param->options & BT_LE_ADV_OPT_CONNECTABLE)) {
		/*
		 * BT Core 4.2 [Vol 2, Part E, 7.8.5]
		 * The Advertising_Interval_Min and Advertising_Interval_Max
		 * shall not be set to less than 0x00A0 (100 ms) if the
		 * Advertising_Type is set to ADV_SCAN_IND or ADV_NONCONN_IND.
		 */
		if (g_btdev.hci_version < BT_HCI_VERSION_5_0 && param->interval_min < 0x00a0) {
			return false;
		}
	}

	if ((param->options & BT_LE_ADV_OPT_DIR_MODE_LOW_DUTY) || !dir_adv) {
		if (param->interval_min > param->interval_max || param->interval_min < 0x0020 || param->interval_max > 0x4000) {
			return false;
		}
	}

	return true;
}

#if 0
#if defined(CONFIG_BT_PRIVACY)
/* this function sets new RPA only if current one is no longer valid */
static int le_set_private_addr(u8_t id)
{
	bt_addr_t rpa;
	int err;
	/* check if RPA is valid */
	if (atomic_test_bit(bt_dev.flags, BT_DEV_RPA_VALID)) {
		return 0;
	}

	err = bt_rpa_create(bt_dev.irk[id], &rpa);
	if (!err) {
		err = set_random_address(&rpa);
		if (!err) {
			atomic_set_bit(bt_dev.flags, BT_DEV_RPA_VALID);
		}
	}

	/* restart timer even if failed to set new RPA */
	k_delayed_work_submit(&bt_dev.rpa_update, RPA_TIMEOUT);
	return err;
}
#endif
#endif

static int le_set_private_addr(uint8_t id)
{
	bt_addr_t nrpa;
	int err;
	err = bt_rand(nrpa.val, sizeof(nrpa.val));
	if (err) {
		return err;
	}

	nrpa.val[5] &= 0x3f;
	return set_random_address(&nrpa);
}

static int bt_le_adv_start_internal(const struct bt_le_adv_param *param, const struct bt_data *ad, size_t ad_len, const struct bt_data *sd, size_t sd_len, const bt_addr_le_t *peer)
{
	FAR struct bt_hci_cp_le_set_adv_parameters_s set_param;
	FAR struct bt_hci_cp_le_set_adv_parameters_s *set_adv_param;
	const bt_addr_le_t *id_addr;
	FAR struct bt_buf_s *buf;
	bool dir_adv = (peer != NULL);
	int err = 0;
	if (!bt_atomic_testbit(g_btdev.flags, BT_DEV_READY)) {
		return -EAGAIN;
	}

	if (!valid_adv_param(param, dir_adv)) {
		return -EINVAL;
	}

	if (bt_atomic_testbit(g_btdev.flags, BT_DEV_ADVERTISING)) {
		return -EALREADY;
	}

	if (!dir_adv) {
		err = le_adv_update(ad, ad_len, sd, sd_len, param->options & BT_LE_ADV_OPT_CONNECTABLE, param->options & BT_LE_ADV_OPT_USE_NAME);
		if (err) {
			return err;
		}
	}

	(void)memset(&set_param, 0, sizeof(set_param));
	set_param.min_interval = BT_HOST2LE16(param->interval_min);
	set_param.max_interval = BT_HOST2LE16(param->interval_max);
	set_param.channel_map = 0x07;
	if (g_btdev.adv_id != param->id) {
		bt_atomic_clrbit(g_btdev.flags, BT_DEV_RPA_VALID);
	}

	/* Set which local identity address we're advertising with */
	g_btdev.adv_id = param->id;
	id_addr = &g_btdev.id_addr[param->id];
	if (param->options & BT_LE_ADV_OPT_CONNECTABLE) {
#if 0
		if (IS_ENABLED(CONFIG_BT_PRIVACY) && !(param->options & BT_LE_ADV_OPT_USE_IDENTITY)) {
			err = le_set_private_addr(param->id);
		}
		if (err) {
			return err;
		}

		if (BT_FEAT_LE_PRIVACY(bt_dev.le.features)) {
			set_param.own_addr_type = BT_HCI_OWN_ADDR_RPA_OR_RANDOM;
		} else {
			set_param.own_addr_type = BT_ADDR_LE_RANDOM;
		}
#endif
		if (!(param->options & BT_LE_ADV_OPT_USE_IDENTITY)) {
			err = le_set_private_addr(param->id);
			if (err) {
				return err;
			}

			set_param.own_addr_type = BT_ADDR_LE_RANDOM;
		} else {
			/*
			 * If Static Random address is used as Identity
			 * address we need to restore it before advertising
			 * is enabled. Otherwise NRPA used for active scan
			 * could be used for advertising.
			 */
			if (id_addr->type == BT_ADDR_LE_RANDOM) {
				set_random_address((bt_addr_t *) id_addr->val);
			}

			set_param.own_addr_type = id_addr->type;
		}

		if (dir_adv) {
			if (param->options & BT_LE_ADV_OPT_DIR_MODE_LOW_DUTY) {
				set_param.type = BT_LE_ADV_DIRECT_IND_LOW_DUTY;
			} else {
				set_param.type = BT_LE_ADV_DIRECT_IND;
			}

			bt_addr_le_copy(&set_param.direct_addr, peer);
#if 0
			if (IS_ENABLED(CONFIG_BT_SMP) && !IS_ENABLED(CONFIG_BT_PRIVACY) && BT_FEAT_LE_PRIVACY(bt_dev.le.features) && (param->options & BT_LE_ADV_OPT_DIR_ADDR_RPA)) {
				/* This will not use RPA for our own address
				 * since we have set zeroed out the local IRK.
				 */
				set_param.own_addr_type |= BT_HCI_OWN_ADDR_RPA_MASK;
			}
#endif
		} else {
			set_param.type = BT_LE_ADV_IND;
		}
	} else {
		if (param->options & BT_LE_ADV_OPT_USE_IDENTITY) {
			if (id_addr->type == BT_ADDR_LE_RANDOM) {
				err = set_random_address((bt_addr_t *) id_addr->val);
			}

			set_param.own_addr_type = id_addr->type;
		} else {
			err = le_set_private_addr(param->id);
			set_param.own_addr_type = BT_ADDR_LE_RANDOM;
		}

		if (err) {
			return err;
		}

		if (sd) {
			set_param.type = BT_LE_ADV_SCAN_IND;
		} else {
			set_param.type = BT_LE_ADV_NONCONN_IND;
		}
	}

	buf = bt_hci_cmd_create(BT_HCI_OP_LE_SET_ADV_PARAMETERS, sizeof(*set_adv_param));
	if (!buf) {
		return -ENOBUFS;
	}

	set_adv_param = bt_buf_extend(buf, sizeof(*set_adv_param));
	memset(set_adv_param, 0, sizeof(*set_adv_param));
	memcpy(set_adv_param, &set_param, sizeof(*set_adv_param));
	err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_SET_ADV_PARAMETERS, buf, NULL);
	if (err) {
		return err;
	}

	err = set_advertise_enable(true);
	if (err) {
		return err;
	}

	if (!(param->options & BT_LE_ADV_OPT_ONE_TIME)) {
		bt_atomic_setbit(g_btdev.flags, BT_DEV_KEEP_ADVERTISING);
	}

	if (param->options & BT_LE_ADV_OPT_USE_NAME) {
		bt_atomic_setbit(g_btdev.flags, BT_DEV_ADVERTISING_NAME);
	}

	if (param->options & BT_LE_ADV_OPT_CONNECTABLE) {
		bt_atomic_setbit(g_btdev.flags, BT_DEV_ADVERTISING_CONNECTABLE);
	}

	return 0;
}

int bt_le_adv_start(const struct bt_le_adv_param *param, const struct bt_data *ad, size_t ad_len, const struct bt_data *sd, size_t sd_len)
{
	if (param->options & BT_LE_ADV_OPT_DIR_MODE_LOW_DUTY) {
		return -EINVAL;
	}

	return bt_le_adv_start_internal(param, ad, ad_len, sd, sd_len, NULL);
}

int bt_le_adv_update_data(const struct bt_data *ad, size_t ad_len, const struct bt_data *sd, size_t sd_len)
{
	bool connectable, use_name;
	if (!bt_atomic_testbit(g_btdev.flags, BT_DEV_ADVERTISING)) {
		return -EAGAIN;
	}

	connectable = bt_atomic_testbit(g_btdev.flags, BT_DEV_ADVERTISING_CONNECTABLE);
	use_name = bt_atomic_testbit(g_btdev.flags, BT_DEV_ADVERTISING_NAME);
	return le_adv_update(ad, ad_len, sd, sd_len, connectable, use_name);
}

int bt_le_adv_stop(void)
{
	int err;
	/* Make sure advertising is not re-enabled later even if it's not
	 * currently enabled (i.e. BT_DEV_ADVERTISING is not set).
	 */
	bt_atomic_clrbit(g_btdev.flags, BT_DEV_KEEP_ADVERTISING);
	if (!bt_atomic_testbit(g_btdev.flags, BT_DEV_ADVERTISING)) {
		return 0;
	}

	err = set_advertise_enable(false);
	if (err) {
		return err;
	}
#if 0
	if (!IS_ENABLED(CONFIG_BT_PRIVACY))
#endif
	{
		/* If active scan is ongoing set NRPA */
		if (bt_atomic_testbit(g_btdev.flags, BT_DEV_SCANNING) && bt_atomic_testbit(g_btdev.flags, BT_DEV_ACTIVE_SCAN)) {
			le_set_private_addr(g_btdev.adv_id);
		}
	}

	return 0;
}

int bt_le_scan_stop(void)
{
	/* Return if active scanning is already disabled */
	if (!bt_atomic_testclrbit(g_btdev.flags, BT_DEV_EXPLICIT_SCAN)) {
		return -EALREADY;
	}

	scan_dev_found_cb = NULL;
	return bt_ble_scan_update(false);
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
   } */

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

static bool valid_le_scan_param(const struct bt_le_scan_param *param)
{
	if (param->type != BT_LE_SCAN_PASSIVE && param->type != BT_LE_SCAN_ACTIVE) {
		return false;
	}

	if (param->filter_dup != BT_LE_SCAN_FILTER_DUP_DISABLE && param->filter_dup != BT_LE_SCAN_FILTER_DUP_ENABLE) {
		return false;
	}

	if (param->interval < 0x0004 || param->interval > 0x4000) {
		return false;
	}

	if (param->window < 0x0004 || param->window > 0x4000) {
		return false;
	}

	if (param->window > param->interval) {
		return false;
	}

	return true;
}

static int set_le_scan_enable(uint8_t enable)
{
	FAR struct bt_hci_cp_le_set_scan_enable_s *cp;
	FAR struct bt_buf_s *buf;
	int err;
	buf = bt_hci_cmd_create(BT_HCI_OP_LE_SET_SCAN_ENABLE, sizeof(*cp));
	if (!buf) {
		return -ENOBUFS;
	}

	cp = bt_buf_extend(buf, sizeof(*cp));
	if (enable == BT_LE_SCAN_ENABLE) {
		cp->filter_dup = bt_atomic_testbit(g_btdev.flags, BT_DEV_SCAN_FILTER_DUP);
	} else {
		cp->filter_dup = BT_LE_SCAN_FILTER_DUP_DISABLE;
	}

	cp->enable = enable;
	err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_SET_SCAN_ENABLE, buf, NULL);
	if (err) {
		return err;
	}

	/* setting the BT_DEV_SCANNING flag */
	if (enable) {
		bt_atomic_setbit(g_btdev.flags, BT_DEV_SCANNING);
	} else {
		bt_atomic_clrbit(g_btdev.flags, BT_DEV_SCANNING);
	}
	return 0;
}

static int start_le_scan(uint8_t scan_type, uint16_t interval, uint16_t window)
{
	FAR struct bt_hci_cp_le_set_scan_params_s *set_param;
	FAR struct bt_buf_s *buf;
	int err;
	buf = bt_hci_cmd_create(BT_HCI_OP_LE_SET_SCAN_PARAMS, sizeof(*set_param));
	if (buf == NULL) {
		ndbg("ERROR:  Failed to create buffer\n");
		return -ENOBUFS;
	}

	set_param = bt_buf_extend(buf, sizeof(*set_param));
	memset(set_param, 0, sizeof(*set_param));
	set_param->scan_type = scan_type;
	/* for the rest parameters apply default values according to
	 *  spec 4.2, vol2, part E, 7.8.10
	 */
	set_param->interval = BT_HOST2LE16(interval);
	set_param->window = BT_HOST2LE16(window);
	set_param->filter_policy = 0x00;
	set_param->addr_type = 0x00;
#if 0
	if (IS_ENABLED(CONFIG_BT_PRIVACY)) {
		err = le_set_private_addr(BT_ID_DEFAULT);
		if (err) {
			return err;
		}

		if (BT_FEAT_LE_PRIVACY(bt_dev.le.features)) {
			set_param.addr_type = BT_HCI_OWN_ADDR_RPA_OR_RANDOM;
		} else {
			set_param.addr_type = BT_ADDR_LE_RANDOM;
		}
	} else {
		set_param.addr_type = bt_dev.id_addr[0].type;
		/* Use NRPA unless identity has been explicitly requested
		 * (through Kconfig), or if there is no advertising ongoing.
		 */
		if (!IS_ENABLED(CONFIG_BT_SCAN_WITH_IDENTITY) && scan_type == BT_HCI_LE_SCAN_ACTIVE && !atomic_test_bit(bt_dev.flags, BT_DEV_ADVERTISING)) {
			err = le_set_private_addr(BT_ID_DEFAULT);
			if (err) {
				return err;
			}

			set_param.addr_type = BT_ADDR_LE_RANDOM;
		} else if (set_param.addr_type == BT_ADDR_LE_RANDOM) {
			set_random_address(&bt_dev.id_addr[0].a);
		}
	}
#endif

	bt_hci_cmd_send(BT_HCI_OP_LE_SET_SCAN_PARAMS, buf);
	err = set_le_scan_enable(BT_LE_SCAN_ENABLE);
	if (err) {
		return err;
	}

	if (scan_type == BT_LE_SCAN_ACTIVE) {
		bt_atomic_setbit(g_btdev.flags, BT_DEV_ACTIVE_SCAN);
	} else {
		bt_atomic_clrbit(g_btdev.flags, BT_DEV_ACTIVE_SCAN);
	}

	return 0;
}

int bt_ble_scan_update(bool fast_scan)
{
	if (bt_atomic_testbit(g_btdev.flags, BT_DEV_EXPLICIT_SCAN)) {
		return 0;
	}

	if (bt_atomic_testbit(g_btdev.flags, BT_DEV_SCANNING)) {
		int err;
		err = set_le_scan_enable(BT_LE_SCAN_DISABLE);
		if (err) {
			return err;
		}
	}

#if 0
	if (IS_ENABLED(CONFIG_BT_CENTRAL))
#endif
	{
		uint16_t interval, window;
		FAR struct bt_conn_s *conn = NULL;
		/* don't restart scan if we have pending connection */
		conn = bt_conn_lookup_state(NULL, BT_CONN_CONNECT);
		if (conn) {
			bt_conn_release(conn);
			return 0;
		}

		conn = bt_conn_lookup_state(NULL, BT_CONN_CONNECT_SCAN);
		if (!conn) {
			return 0;
		}

		bt_atomic_setbit(g_btdev.flags, BT_DEV_SCAN_FILTER_DUP);
		bt_conn_release(conn);
		if (fast_scan) {
			interval = BT_GAP_SCAN_FAST_INTERVAL;
			window = BT_GAP_SCAN_FAST_WINDOW;
		} else {
			interval = CONFIG_BT_BACKGROUND_SCAN_INTERVAL;
			window = CONFIG_BT_BACKGROUND_SCAN_WINDOW;
		}

		return start_le_scan(BT_LE_SCAN_PASSIVE, interval, window);
	}

	return 0;
}

static void ble_check_pending_conn(FAR const bt_addr_le_t *addr, uint8_t evtype, FAR struct bt_keys_s *keys)
{
	FAR struct bt_conn_s *conn = NULL;
	/* No connections are allowed during explicit scanning */
	if (bt_atomic_testbit(g_btdev.flags, BT_DEV_EXPLICIT_SCAN)) {
		return;
	}

	/* Return if event is not connectible */
	if (evtype != BT_LE_ADV_IND && evtype != BT_LE_ADV_DIRECT_IND) {
		return;
	}

	if (keys) {
		conn = bt_conn_lookup_state(&keys->addr, BT_CONN_CONNECT_SCAN);
	} else {
		conn = bt_conn_lookup_state(addr, BT_CONN_CONNECT_SCAN);
	}

	if (!conn) {
		return;
	}

	if (bt_atomic_testbit(g_btdev.flags, BT_DEV_SCANNING) && set_le_scan_enable(BT_LE_SCAN_DISABLE)) {
		goto failed;
	}

	if (hci_le_create_conn(addr)) {
		goto failed;
	}

	bt_conn_set_state(conn, BT_CONN_CONNECT);
	bt_conn_release(conn);
	return;
failed:
	bt_conn_set_state(conn, BT_CONN_DISCONNECTED);
	bt_conn_release(conn);
	bt_ble_scan_update(false);
}

void ble_adv_report(FAR struct bt_buf_s *buf)
{
	FAR struct bt_hci_ev_le_advertising_info_s *info;
	uint8_t num_reports = buf->data[0];
	nvdbg("Adv number of reports %u\n", num_reports);
	info = bt_buf_consume(buf, sizeof(num_reports));
	while (num_reports--) {
		int8_t rssi = info->data[info->length];
		FAR struct bt_keys_s *keys;
		bt_addr_le_t addr;
		nvdbg("%s event %u, len %u, rssi %d dBm\n", bt_addr_le_str(&info->addr), info->evt_type, info->length, rssi);
		keys = bt_keys_find_irk(&info->addr);
		if (keys) {
			bt_addr_le_copy(&addr, &keys->addr);
			nvdbg("Identity %s matched RPA %s\n", bt_addr_le_str(&keys->addr), bt_addr_le_str(&info->addr));
		} else {
			bt_addr_le_copy(&addr, &info->addr);
		}

		if (scan_dev_found_cb) {
			struct net_buf_simple net_buf;
			net_buf.data = info->data;
			net_buf.len = info->length;
			scan_dev_found_cb(&addr, rssi, info->evt_type, &net_buf);
		}

		ble_check_pending_conn(&info->addr, info->evt_type, keys);
		/* Get next report iteration by moving pointer to right offset in buf
		 * according to spec 4.2, Vol 2, Part E, 7.7.65.2.
		 */
		info = bt_buf_consume(buf, sizeof(*info) + info->length + sizeof(rssi));
	}
}

int bt_le_scan_start(const struct bt_le_scan_param *param, bt_le_scan_cb_t cb)
{
	int err;
	if (!bt_atomic_testbit(g_btdev.flags, BT_DEV_READY)) {
		return -EAGAIN;
	}

	/* Check that the parameters have valid values */
	if (!valid_le_scan_param(param)) {
		return -EINVAL;
	}

	/* Return if active scan is already enabled */
	if (bt_atomic_testsetbit(g_btdev.flags, BT_DEV_EXPLICIT_SCAN)) {
		return -EALREADY;
	}

	if (bt_atomic_testbit(g_btdev.flags, BT_DEV_SCANNING)) {
		err = set_le_scan_enable(BT_LE_SCAN_DISABLE);
		if (err) {
			bt_atomic_clrbit(g_btdev.flags, BT_DEV_EXPLICIT_SCAN);
			return err;
		}
	}

	if (param->filter_dup) {
		bt_atomic_setbit(g_btdev.flags, BT_DEV_SCAN_FILTER_DUP);
	} else {
		bt_atomic_clrbit(g_btdev.flags, BT_DEV_SCAN_FILTER_DUP);
	}

	err = start_le_scan(param->type, param->interval, param->window);
	if (err) {
		bt_atomic_clrbit(g_btdev.flags, BT_DEV_EXPLICIT_SCAN);
		return err;
	}

	scan_dev_found_cb = cb;
	return 0;
}
