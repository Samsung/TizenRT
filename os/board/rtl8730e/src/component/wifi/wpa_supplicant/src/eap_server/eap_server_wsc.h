#ifndef EAP_SERVER_WSC_H
#define EAP_SERVER_WSC_H

#ifdef FAIL
#undef FAIL
#endif

#if 0
enum wsc_state { WAIT_START, MESG, FRAG_ACK, WAIT_FRAG_ACK, DONE, FAIL };


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
};
#endif
int eap_server_wsc_register(void);

#endif /* EAP_WSC_H */
