/*
 *  Header files of eap_protocol_handler
 *
 */

#ifndef _EAP_PROTOCOL_HANDLER_H_
#define _EAP_PROTOCOL_HANDLER_H_

#include "utils/os.h"

/*
// supported eap methods
enum{
	NONE,
	PEAP,
	TLS,
	TTLS
};
*/

struct rtw_eap_context {
	/* EAP State Machine */
//	int retryCnt;
//	unsigned char eapProcessDone;
	unsigned char eapIsProcessing;
	u8 isKeySet;

	int eap_method;
	void *eap_data;
	struct eap_sm *eap_sm;

};

//extern struct rtw_peap_context g_peap_context;

void eap_sm_deinit(void);
int get_eap_phase(void);
int get_eap_ctx_method(void);
int set_eap_peap_method(void);
int set_eap_tls_method(void);
int set_eap_ttls_method(void);

void eap_eapol_start_hdl(u8 *evt_info);
void eap_eapol_recvd_hdl(u8 *buf, s32 buf_len);
#endif
