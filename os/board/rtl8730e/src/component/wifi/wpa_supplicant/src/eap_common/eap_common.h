/*
 * EAP common peer/server definitions
 * Copyright (c) 2004-2012, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef EAP_COMMON_H
#define EAP_COMMON_H

#include "utils/wpabuf.h"
#include "eap_defs.h"

struct eap_server_method {
	int vendor;
	enum EapType method;
	const char *name;

	void *(*init)(char *identity, char identity_len);
	//void * (*initPickUp)(struct eap_sm *sm);
	void (*reset)(void *priv);

	struct wpabuf *(*buildReq)(void *priv, u8 id);
	int (*getTimeout)(void *priv);
	unsigned char (*check)(void *priv,
						   struct wpabuf *respData);
	void (*process)(void *priv,
					struct wpabuf *respData);
	unsigned char (*isDone)(void *priv);
	u8 *(*getKey)(void *priv, size_t *len);
	/* isSuccess is not specified in draft-ietf-eap-statemachine-05.txt,
	 * but it is useful in implementing Policy.getDecision() */
	unsigned char (*isSuccess)(void *priv);

	/**
	 * free - Free EAP method data
	 * @method: Pointer to the method data registered with
	 * eap_server_method_register().
	 *
	 * This function will be called when the EAP method is being
	 * unregistered. If the EAP method allocated resources during
	 * registration (e.g., allocated struct eap_method), they should be
	 * freed in this function. No other method functions will be called
	 * after this call. If this function is not defined (i.e., function
	 * pointer is %NULL), a default handler is used to release the method
	 * data with free(method). This is suitable for most cases.
	 */
	//void (*free)(struct eap_method *method);

#define EAP_SERVER_METHOD_INTERFACE_VERSION 1
	/**
	 * version - Version of the EAP server method interface
	 *
	 * The EAP server method implementation should set this variable to
	 * EAP_SERVER_METHOD_INTERFACE_VERSION. This is used to verify that the
	 * EAP method is using supported API version when using dynamically
	 * loadable EAP methods.
	 */
	int version;

	/**
	 * next - Pointer to the next EAP method
	 *
	 * This variable is used internally in the EAP method registration code
	 * to create a linked list of registered EAP methods.
	 */
	//struct eap_method *next;

	/**
	 * get_emsk - Get EAP method specific keying extended material (EMSK)
	 * @sm: Pointer to EAP state machine allocated with eap_sm_init()
	 * @priv: Pointer to private EAP method data from eap_method::init()
	 * @len: Pointer to a variable to store EMSK length
	 * Returns: EMSK or %NULL if not available
	 *
	 * This function can be used to get the extended keying material from
	 * the EAP method. The key may already be stored in the method-specific
	 * private data or this function may derive the key.
	 */
	//u8 * (*get_emsk)(struct eap_sm *sm, void *priv, size_t *len);
};

int eap_hdr_len_valid(const struct wpabuf *msg, size_t min_payload);
const u8 *eap_hdr_validate(int vendor, enum EapType eap_type,
						   const struct wpabuf *msg, size_t *plen);
struct wpabuf *eap_msg_alloc(int vendor, enum EapType type, size_t payload_len,
							 u8 code, u8 identifier);
void eap_update_len(struct wpabuf *msg);
u8 eap_get_id(const struct wpabuf *msg);
enum EapType eap_get_type(const struct wpabuf *msg);

#endif /* EAP_COMMON_H */
