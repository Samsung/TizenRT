/*
 * EAP-WSC peer for Wi-Fi Protected Setup
 * Copyright (c) 2007-2009, 2012, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "utils/includes.h"
#include "utils/common.h"

#include "utils/uuid.h"
#include "eap_peer/eap_i.h"

#include "eap_common/eap_wsc_common.h"
#include "wps/wps.h"
#include "wps/wps_defs.h"


//#ifdef CONFIG_WPS

#ifdef FAIL
#undef FAIL
#endif

#if 0
struct eap_wsc_data {
	enum { WAIT_START, MESG, FRAG_ACK, WAIT_FRAG_ACK, DONE, FAIL } state ;
	int registrar;
	struct wpabuf *in_buf;
	struct wpabuf *out_buf;
	enum wsc_op_code in_op_code, out_op_code;
	size_t out_used;		//global used ?
	size_t fragment_size;
	struct wps_data *wps;		// allocate by wsc_init()
};
#endif

#ifndef CONFIG_NO_STDOUT_DEBUG
static const char *eap_wsc_state_txt(int state)
{
	switch (state) {
	case WAIT_START:
		return "WAIT_START";
	case MESG:
		return "MESG";
	case FRAG_ACK:
		return "FRAG_ACK";
	case WAIT_FRAG_ACK:
		return "WAIT_FRAG_ACK";
	case DONE:
		return "DONE";
	case FAIL:
		return "FAIL";
	default:
		return "?";
	}
}
#endif

// change the state machine and
// printf the debug message for from state to changed state
static void eap_wsc_state(struct eap_wsc_data *data, int state)
{
	wpa_printf(MSG_DEBUG, "EAP-WSC: %s -> %s", eap_wsc_state_txt(data->state),
			   eap_wsc_state_txt(state));
	data->state = (enum wsc_state)state;
}

// assume params = "pbc=1"
#if 0
static int eap_wsc_new_ap_settings(struct wps_credential *cred,
								   const char *params)
{
	const char *pos, *end;
	size_t len;

	os_memset(cred, 0, sizeof(*cred));

	pos = os_strstr(params, "new_ssid=");
	if (pos == NULL) {
		return 0;
	}
	pos += 9;
	//find out the first exist ' ' place in the string pos.
	end = os_strchr(pos, ' ');
	if (end == NULL) {
		len = os_strlen(pos);    // if no ' ' in string meaning all length be used
	} else {
		len = end - pos;
	}

	if ((len & 1) || len > 2 * sizeof(cred->ssid) //||
		/*hexstr2bin(pos, cred->ssid, len / 2)*/) {
		return -1;
	}
	cred->ssid_len = len / 2;

	// check if there is "new_auth=" in the string params.
	pos = os_strstr(params, "new_auth=");
	if (pos == NULL) {
		return -1;
	}
	if (os_strncmp(pos + 9, "OPEN", 4) == 0) {
		cred->auth_type = WPS_AUTH_OPEN;
	} else if (os_strncmp(pos + 9, "WPAPSK", 6) == 0) {
		cred->auth_type = WPS_AUTH_WPAPSK;	// tkip
	} else if (os_strncmp(pos + 9, "WPA2PSK", 7) == 0) {
		cred->auth_type = WPS_AUTH_WPA2PSK;	// aes
	} else {
		return -1;
	}

	pos = os_strstr(params, "new_encr=");
	if (pos == NULL) {
		return -1;
	}
	if (os_strncmp(pos + 9, "NONE", 4) == 0) {
		cred->encr_type = WPS_ENCR_NONE;
	} else if (os_strncmp(pos + 9, "WEP", 3) == 0) {
		cred->encr_type = WPS_ENCR_WEP;
	} else if (os_strncmp(pos + 9, "TKIP", 4) == 0) {
		cred->encr_type = WPS_ENCR_TKIP;
	} else if (os_strncmp(pos + 9, "CCMP", 4) == 0) {
		cred->encr_type = WPS_ENCR_AES;
	} else {
		return -1;
	}

	pos = os_strstr(params, "new_key=");
	if (pos == NULL) {
		return 0;
	}
	pos += 8;
	end = os_strchr(pos, ' ');
	if (end == NULL) {
		len = os_strlen(pos);
	} else {
		len = end - pos;
	}
	if ((len & 1) || len > 2 * sizeof(cred->key) //||
		/*hexstr2bin(pos, cred->key, len / 2)*/) {
		return -1;
	}
	cred->key_len = len / 2;

	return 1;
}
#endif
extern char wps_pin_code[];
// return struct eap_wsc_data *
extern unsigned char wps_password_id;
extern u16 config_method;
struct eap_method wsc_eap;

// add eap_sm argu
//static void *eap_wsc_init(void)
static void *eap_wsc_init(struct eap_sm *sm)
{
	/* To avoid gcc warnings */
	(void) sm;

	struct eap_wsc_data *data;
	size_t identity_len;
	int registrar;

	struct wps_config cfg;
	//const char *pos;
	//const char *phase1;
	//char phase1[5] = {'p','b','c','=','1'};
	//struct wps_context *wps;		// don't use it
	//struct wps_credential new_ap_settings;
	//int res;
	//int nfc = 0;


	// get config ID and the id length, assume
	// id = WFA-SimpleConfig-Enrollee-1-0, id_len = 29

	char identity[] = "WFA-SimpleConfig-Enrollee-1-0";
	identity_len = WSC_ID_ENROLLEE_LEN;

	if (identity_len == WSC_ID_REGISTRAR_LEN &&
		os_memcmp(identity, WSC_ID_REGISTRAR, WSC_ID_REGISTRAR_LEN) == _TRUE) {
		/* Supplicant is Registrar */
		registrar = 1;
		wpa_printf(MSG_INFO, "eap_wsc_init() registrar = 1, Supplicant is Registrar");
	} else if (identity_len == WSC_ID_ENROLLEE_LEN &&
			   os_memcmp(identity, WSC_ID_ENROLLEE, WSC_ID_ENROLLEE_LEN) == _TRUE) {
		/* Supplicant is Enrollee */
		registrar = 0;
		wpa_printf(MSG_INFO, "eap_wsc_init() registrar = 0, Supplicant is Enrollee");
	} else {
		wpa_printf(MSG_INFO, "eap_wsc_init() EAP-WSC: Unexpected identity");
		return NULL;
	}

	// struct eap_wsc_data !!!!! Carefully!!!!!
	data = (struct eap_wsc_data *)os_zalloc(sizeof(*data));
	if (data == NULL) {
		return NULL;
	}

	data->state = registrar ? MESG : WAIT_START;
	data->registrar = registrar;

	//  cfg.registrar = 0
	os_memset(&cfg, 0, sizeof(cfg));

	cfg.registrar = registrar;

	if (config_method & WPS_CONFIG_PUSHBUTTON) {
		cfg.pbc = 1;
		cfg.dev_pw_id = 0x0004;
		wpa_printf(MSG_DEBUG, "cfg.dev_pw_id = 0x%x", cfg.dev_pw_id);
	} else if ((config_method & WPS_CONFIG_KEYPAD) || (config_method & WPS_CONFIG_DISPLAY)) {
		cfg.pin = (const u8 *)wps_pin_code;
		cfg.pin_len = 8;
		cfg.dev_pw_id = 0x0000;
		wpa_printf(MSG_DEBUG, "cfg.dev_pw_id = 0x%x", cfg.dev_pw_id);
	} else {
		wpa_printf(MSG_INFO, "EAP-WSC: PIN or PBC not enable");
		//os_free(data, 0);
		//return NULL;
	}


	// check error cases !!!!
	if (cfg.pin == NULL && !cfg.pbc) {
		wpa_printf(MSG_INFO, "EAP-WSC: PIN or PBC not set in phase1 configuration data");
		os_free(data, 0);
		return NULL;
	}

	data->wps = wps_init(&cfg);
	if (data->wps == NULL) {
		os_free(data, 0);
		return NULL;
	}

	data->fragment_size = WSC_FRAGMENT_SIZE;

	return data;
}

// add eap_sm argu
//static void eap_wsc_deinit(void *priv)
static void eap_wsc_deinit(struct eap_sm *sm, void *priv)
{
	/* To avoid gcc warnings */
	(void) sm;

	struct eap_wsc_data *data = priv;
	if (data == NULL) {
		return;
	}
	wpabuf_free(data->in_buf);
	wpabuf_free(data->out_buf);
	wps_deinit(data->wps);
	//os_free(data->wps_ctx->network_key,0);
	//data->wps_ctx->network_key = NULL;
	os_free(data, 0);
}

// id = identifier of eap packet for match the reauest/response
static struct wpabuf *eap_wsc_build_msg(struct eap_wsc_data *data, u8 id)
{
	struct wpabuf *resp;
	u8 flags;
	size_t send_len, plen;


	flags = 0;
	send_len = wpabuf_len(data->out_buf) - data->out_used;

	if (2 + send_len > data->fragment_size) {
		send_len = data->fragment_size - 2;
		flags |= WSC_FLAGS_MF;
		if (data->out_used == 0) {
			flags |= WSC_FLAGS_LF;
			send_len -= 2;
		}
	}


	plen = 2 + send_len;
	if (flags & WSC_FLAGS_LF) {
		plen += 2;
	}
	// create hdr and fill content
	resp = eap_msg_alloc(EAP_VENDOR_WFA, (EapType)EAP_VENDOR_TYPE_WSC, plen,
						 EAP_CODE_RESPONSE, id);
	if (resp == NULL) {
		wpa_printf(MSG_WARNING, "resp == NULL!!!");
		return NULL;
	}
	// fill op-code and flags
	wpabuf_put_u8(resp, data->out_op_code); /* Op-Code */
	wpabuf_put_u8(resp, flags); 		/* Flags */
	if (flags & WSC_FLAGS_LF) {
		wpabuf_put_be16(resp, wpabuf_len(data->out_buf));
	}

	// copy all out_buf content to header(i.e resp)
	wpabuf_put_data(resp, wpabuf_head_u8(data->out_buf) + data->out_used,
					send_len);
	data->out_used += send_len;

	if (data->out_used == wpabuf_len(data->out_buf)) {
		wpa_printf(MSG_DEBUG, "EAP-WSC: Sending out %lu bytes message sent completely",
				   (unsigned long) send_len);

		wpabuf_free(data->out_buf);

		data->out_buf = NULL;
		data->out_used = 0;

		if ((data->state == 0x05 && data->out_op_code == WSC_ACK) ||
			data->out_op_code == WSC_NACK ||
			data->out_op_code == WSC_Done) {
			eap_wsc_state(data, FAIL);
		} else {
			eap_wsc_state(data, MESG);
		}
	} else {
		wpa_printf(MSG_INFO, "EAP-WSC: Sending out %lu bytes (%lu more to send)",
				   (unsigned long)send_len, (unsigned long)(wpabuf_len(data->out_buf) - data->out_used));
		eap_wsc_state(data, WAIT_FRAG_ACK);
	}

	return resp;
}


static int eap_wsc_process_cont(struct eap_wsc_data *data,
								const u8 *buf, size_t len, u8 op_code)
{
	/* Process continuation of a pending message */
	if (op_code != data->in_op_code) {
		wpa_printf(MSG_DEBUG, "EAP-WSC: Unexpected Op-Code %d in "
				   "fragment (expected %d)",
				   op_code, data->in_op_code);
		return -1;
	}

	if (len > wpabuf_tailroom(data->in_buf)) {
		wpa_printf(MSG_DEBUG, "EAP-WSC: Fragment overflow");
		eap_wsc_state(data, FAIL);
		return -1;
	}

	wpabuf_put_data(data->in_buf, buf, len);
	wpa_printf(MSG_DEBUG, "EAP-WSC: Received %lu bytes, waiting "
			   "for %lu bytes more", (unsigned long) len,
			   (unsigned long) wpabuf_tailroom(data->in_buf));

	return 0;
}


static struct wpabuf *eap_wsc_process_fragment(struct eap_wsc_data *data,
		u8 id, u8 flags, u8 op_code,
		u16 message_length,
		const u8 *buf, size_t len)
{
	/* Process a fragment that is not the last one of the message */
	if (data->in_buf == NULL && !(flags & WSC_FLAGS_LF)) {
		wpa_printf(MSG_DEBUG, "EAP-WSC: No Message Length field in a "
				   "fragmented packet");
#if 0
		ret->ignore = TRUE;
#endif
		return NULL;
	}

	if (data->in_buf == NULL) {
		/* First fragment of the message */
		data->in_buf = wpabuf_alloc(message_length);
		if (data->in_buf == NULL) {
			wpa_printf(MSG_DEBUG, "EAP-WSC: No memory for "
					   "message");
#if 0
			ret->ignore = TRUE;
#endif
			return NULL;
		}
		data->in_op_code = (enum wsc_op_code)op_code;
		wpabuf_put_data(data->in_buf, buf, len);
		wpa_printf(MSG_DEBUG, "EAP-WSC: Received %lu bytes in first "
				   "fragment, waiting for %lu bytes more",
				   (unsigned long) len,
				   (unsigned long) wpabuf_tailroom(data->in_buf));
	}

	return eap_wsc_build_frag_ack(id, EAP_CODE_RESPONSE);
}


/* wpabuf means the receiving packet place */ // NULL, wps_data, NULL, &reqData
// add eap_sm and eap_method_ret argu
//static struct wpabuf *eap_wsc_process(void *priv, const struct wpabuf *reqData)
static struct wpabuf *eap_wsc_process(struct eap_sm *sm, void *priv,
									  struct eap_method_ret *ret, const struct wpabuf *reqData)
{
	/* To avoid gcc warnings */
	(void) sm;
	(void) ret;
	struct eap_wsc_data *data = priv;
	const u8 *start, *pos, *end;
	size_t len;
	u8 op_code, flags, id;
	u16 message_length = 0;
	enum wps_process_res res;
	struct wpabuf tmpbuf;
	struct wpabuf *r;


	// verify the eap hdr content and get the addr of op-code in eap packet
	pos = eap_hdr_validate(EAP_VENDOR_WFA, (EapType)EAP_VENDOR_TYPE_WSC, reqData, &len);


	if (pos == NULL || len < 2) {
		wpa_printf(MSG_INFO, "pos == NULL || len < 2");
		return NULL;
	}

	// get identifier of eap packet for match the reauest/response
	id = eap_get_id(reqData);

	// the addr of op-code in eap packet
	start = pos;

	// compute the end addr of the eap packet
	end = start + len;

	// get eap op-code
	op_code = *pos++;
	// get eap flags
	flags = *pos++;

	// the pos now pointer the addr of Message Length in eap packet

	// check the WSC TLC attributes exist in eap packet or not
	if (flags & WSC_FLAGS_LF) {
		if (end - pos < 2) { // length < 2 ???
			wpa_printf(MSG_INFO, "EAP-WSC: EAP-WSC: Message underflow");
			return NULL;
		}
		// total length of WSC TLV attrubutes in the WSC message
		message_length = WPA_GET_BE16(pos);
		// get the addr of message data in eap packet
		pos += 2;

		if (message_length < end - pos) {
			wpa_printf(MSG_INFO, "EAP-WSC: Invalid Message Length");
			return NULL;
		}
		// pos now is point to the addr of message data
	}

	wpa_printf(MSG_DEBUG, "EAP-WSC: Received packet: Op-Code %d Flags 0x%x Message Length %d",
			   op_code, flags, message_length);

	// WAIT_FRAG_ACK means MF Flag be set before, the packet be fragmentation
	// and already completet received so maybe we should to reassemble
	// multiple eap-ackets to one origonal packet.
	if (data->state == WAIT_FRAG_ACK) {
		if (op_code != WSC_FRAG_ACK) {
			wpa_printf(MSG_INFO, "EAP-WSC: Unexpected Op-Code %d in WAIT_FRAG_ACK state", op_code);
			return NULL;
		}

		wpa_printf(MSG_INFO, "EAP-WSC: Fragment Acknowledged");
		eap_wsc_state(data, MESG);
		return eap_wsc_build_msg(data, id);
	}

	// Check op-code belone into the required range
	// WSC_Start(0x01)~WSC_FRAG_ACK(0x06)
	if (op_code != WSC_ACK && op_code != WSC_NACK && op_code != WSC_MSG &&
		op_code != WSC_Done && op_code != WSC_Start) {
		wpa_printf(MSG_INFO, "EAP-WSC: Not Valid Op-Code %d", op_code);
		return NULL;
	}

	// First time entry point
	if (data->state == WAIT_START) {
		if (op_code != WSC_Start) {
			wpa_printf(MSG_INFO, "EAP-WSC: Unexpected Op-Code %d in WAIT_START state", op_code);
			return NULL;
		}
		wpa_printf(MSG_INFO, "EAP-WSC: Received start");
		eap_wsc_state(data, MESG);
		/* Start message has empty payload, skip processing */
		goto send_msg;
	} else if (op_code == WSC_Start) {
		wpa_printf(MSG_INFO, "EAP-WSC: Unexpected Op-Code %d", op_code);
		return NULL;
	}

	if (data->in_buf && eap_wsc_process_cont(data, pos, end - pos, op_code) < 0) {
		wpa_printf(MSG_INFO, "eap_wsc_process_cont < 0");
		return NULL;
	}

	if (flags & WSC_FLAGS_MF) {
		return eap_wsc_process_fragment(data, id, flags, op_code,
										message_length, pos,
										end - pos);
	}

	if (data->in_buf == NULL) {
		/* Wrap unfragmented messages as wpabuf without extra copy */
		wpabuf_set(&tmpbuf, pos, end - pos);
		data->in_buf = &tmpbuf;
	}

	// struct eap_wsc_data *data = priv(i.e wps_data);
	res = wps_process_msg(data->wps, (enum wsc_op_code)op_code, data->in_buf);

	wpa_printf(MSG_DEBUG, "EAP-WSC: WPS processing return res = %x", res);

	switch (res) {
	case WPS_DONE:
		wpa_printf(MSG_DEBUG, "EAP-WSC: WPS processing completed successfully - wait for EAP failure");
		eap_wsc_state(data, FAIL);
		break;
	case WPS_CONTINUE:
		wpa_printf(MSG_DEBUG, "EAP-WSC: WPS processing WPS_CONTINUE");
		eap_wsc_state(data, MESG);
		break;
	case WPS_FAILURE:
	case WPS_PENDING:
		wpa_printf(MSG_DEBUG, "EAP-WSC: WPS processing failed");
		eap_wsc_state(data, FAIL);
		break;
	}

	if (data->in_buf != &tmpbuf) {
		wpabuf_free(data->in_buf);
	}

	data->in_buf = NULL;

send_msg:
	if (data->out_buf == NULL) {
		data->out_buf = wps_get_msg(data->wps, &data->out_op_code);
		if (data->out_buf == NULL) {
			wpa_printf(MSG_INFO, "EAP-WSC: Failed to receive message from WPS");
			return NULL;
		}
		data->out_used = 0;
	}

	eap_wsc_state(data, MESG);

	// identifier of eap packet for match the reauest/response
	r = eap_wsc_build_msg(data, id);

	return r;
}

int eap_peer_wsc_register(void)
{

	wsc_eap.init = eap_wsc_init;
	wsc_eap.deinit = eap_wsc_deinit;
	wsc_eap.process = eap_wsc_process;

	return 0;
}
//#endif
