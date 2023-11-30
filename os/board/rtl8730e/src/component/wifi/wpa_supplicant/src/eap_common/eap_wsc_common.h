/*
 * EAP-WSC definitions for Wi-Fi Protected Setup
 * Copyright (c) 2007, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef EAP_WSC_COMMON_H
#define EAP_WSC_COMMON_H

#include "wps/wps.h"

#define EAP_VENDOR_TYPE_WSC 1

#define WSC_FLAGS_MF 0x01
#define WSC_FLAGS_LF 0x02

#define WSC_ID_REGISTRAR "WFA-SimpleConfig-Registrar-1-0"
#define WSC_ID_REGISTRAR_LEN 30
#define WSC_ID_ENROLLEE "WFA-SimpleConfig-Enrollee-1-0"
#define WSC_ID_ENROLLEE_LEN 29

#define WSC_FRAGMENT_SIZE 1400

#ifdef FAIL
#undef FAIL
#endif

enum wsc_state {
	WAIT_START = 0,
	START,
	MESG,
	FRAG_ACK,
	WAIT_FRAG_ACK,
	DONE,
	FAIL,
};

//move from eap_wsc.h and eap_server_wsc.h to here.
//enum wsc_state { WAIT_START, START, MESG, FRAG_ACK, WAIT_FRAG_ACK, DONE, FAIL };

struct eap_wsc_data {
	enum wsc_state state ;
	int registrar;
	struct wpabuf *in_buf;
	struct wpabuf *out_buf;
	enum wsc_op_code in_op_code, out_op_code;
	size_t out_used;		//global used ?
	size_t fragment_size;
	struct wps_data *wps;		// allocate by wsc_init()
	//struct wps_context *wps_ctx;	// non-use
	int ext_reg_timeout; //server must need it?
	struct wpabuf *last_reqdata;
	os_timer retrans_timer;
	int MaxRetrans;
};
struct wpabuf *eap_wsc_build_frag_ack(u8 id, u8 code);

#endif /* EAP_WSC_COMMON_H */
