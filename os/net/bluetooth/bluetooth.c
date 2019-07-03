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

int bt_enable(bt_ready_cb_t cb)
{
	// TODO: need to implement
	return 0;
}

int bt_set_name(const char *name)
{
	// TODO: need to implement
	return 0;
}

const char *bt_get_name(void)
{
	// TODO: need to implement
	return NULL;
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

int bt_le_adv_start(const struct bt_le_adv_param *param, const struct bt_data *ad, size_t ad_len, const struct bt_data *sd, size_t sd_len)
{
	// TODO: need to implement
	return 0;
}

int bt_le_adv_update_data(const struct bt_data *ad, size_t ad_len, const struct bt_data *sd, size_t sd_len)
{
	// TODO: need to implement
	return 0;
}

int bt_le_adv_stop(void)
{
	// TODO: need to implement
	return 0;
}

int bt_le_scan_start(const struct bt_le_scan_param *param, bt_le_scan_cb_t cb)
{
	// TODO: need to implement
	return 0;
}

int bt_le_scan_stop(void)
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

void bt_conn_cb_register(struct bt_conn_cb *cb)
{
	// TODO: need to implement
	return;
}

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
