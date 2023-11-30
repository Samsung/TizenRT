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


#if 0 // not using this
typedef unsigned char Boolean;

struct eap_method {
	/**
	 * vendor - EAP Vendor-ID (EAP_VENDOR_*) (0 = IETF)
	 */
	int vendor;

	/**
	 * method - EAP type number (EAP_TYPE_*)
	 */
	EapType method;

	/**
	 * name - Name of the method (e.g., "TLS")
	 */
	const char *name;

	/**
	 * init - Initialize an EAP method
	 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
	 * Returns: Pointer to allocated private data, or %NULL on failure
	 *
	 * This function is used to initialize the EAP method explicitly
	 * instead of using METHOD_INIT state as specific in RFC 4137. The
	 * method is expected to initialize it method-specific state and return
	 * a pointer that will be used as the priv argument to other calls.
	 */
	void *(*init)();

	/**
	 * deinit - Deinitialize an EAP method
	 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
	 * @priv: Pointer to private EAP method data from eap_method::init()
	 *
	 * Deinitialize the EAP method and free any allocated private data.
	 */
	void (*deinit)(void *priv);

	/**
	 * process - Process an EAP request
	 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
	 * @priv: Pointer to private EAP method data from eap_method::init()
	 * @ret: Return values from EAP request validation and processing
	 * @reqData: EAP request to be processed (eapReqData)
	 * Returns: Pointer to allocated EAP response packet (eapRespData)
	 *
	 * This function is a combination of m.check(), m.process(), and
	 * m.buildResp() procedures defined in section 4.4 of RFC 4137 In other
	 * words, this function validates the incoming request, processes it,
	 * and build a response packet. m.check() and m.process() return values
	 * are returned through struct eap_method_ret *ret variable. Caller is
	 * responsible for freeing the returned EAP response packet.
	 */
	struct wpabuf *(*process)(void *priv,
							  //struct eap_method_ret *ret,
							  const struct wpabuf *reqData);

	/**
	 * isKeyAvailable - Find out whether EAP method has keying material
	 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
	 * @priv: Pointer to private EAP method data from eap_method::init()
	 * Returns: %TRUE if key material (eapKeyData) is available
	 */
	//Boolean (*isKeyAvailable)(struct eap_sm *sm, void *priv);

	/**
	 * getKey - Get EAP method specific keying material (eapKeyData)
	 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
	 * @priv: Pointer to private EAP method data from eap_method::init()
	 * @len: Pointer to variable to store key length (eapKeyDataLen)
	 * Returns: Keying material (eapKeyData) or %NULL if not available
	 *
	 * This function can be used to get the keying material from the EAP
	 * method. The key may already be stored in the method-specific private
	 * data or this function may derive the key.
	 */
	//u8 * (*getKey)(struct eap_sm *sm, void *priv, size_t *len);

	/**
	 * get_status - Get EAP method status
	 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
	 * @priv: Pointer to private EAP method data from eap_method::init()
	 * @buf: Buffer for status information
	 * @buflen: Maximum buffer length
	 * @verbose: Whether to include verbose status information
	 * Returns: Number of bytes written to buf
	 *
	 * Query EAP method for status information. This function fills in a
	 * text area with current status information from the EAP method. If
	 * the buffer (buf) is not large enough, status information will be
	 * truncated to fit the buffer.
	 */
	//int (*get_status)(struct eap_sm *sm, void *priv, char *buf,
	//		  size_t buflen, int verbose);

	/**
	 * has_reauth_data - Whether method is ready for fast reauthentication
	 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
	 * @priv: Pointer to private EAP method data from eap_method::init()
	 * Returns: %TRUE or %FALSE based on whether fast reauthentication is
	 * possible
	 *
	 * This function is an optional handler that only EAP methods
	 * supporting fast re-authentication need to implement.
	 */
	//Boolean (*has_reauth_data)(struct eap_sm *sm, void *priv);

	/**
	 * deinit_for_reauth - Release data that is not needed for fast re-auth
	 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
	 * @priv: Pointer to private EAP method data from eap_method::init()
	 *
	 * This function is an optional handler that only EAP methods
	 * supporting fast re-authentication need to implement. This is called
	 * when authentication has been completed and EAP state machine is
	 * requesting that enough state information is maintained for fast
	 * re-authentication
	 */
	//void (*deinit_for_reauth)(struct eap_sm *sm, void *priv);

	/**
	 * init_for_reauth - Prepare for start of fast re-authentication
	 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
	 * @priv: Pointer to private EAP method data from eap_method::init()
	 *
	 * This function is an optional handler that only EAP methods
	 * supporting fast re-authentication need to implement. This is called
	 * when EAP authentication is started and EAP state machine is
	 * requesting fast re-authentication to be used.
	 */
	//void * (*init_for_reauth)(struct eap_sm *sm, void *priv);

	/**
	 * get_identity - Get method specific identity for re-authentication
	 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
	 * @priv: Pointer to private EAP method data from eap_method::init()
	 * @len: Length of the returned identity
	 * Returns: Pointer to the method specific identity or %NULL if default
	 * identity is to be used
	 *
	 * This function is an optional handler that only EAP methods
	 * that use method specific identity need to implement.
	 */
	//const u8 * (*get_identity)(struct eap_sm *sm, void *priv, size_t *len);

	/**
	 * free - Free EAP method data
	 * @method: Pointer to the method data registered with
	 * eap_peer_method_register().
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

#define EAP_PEER_METHOD_INTERFACE_VERSION 1
	/**
	 * version - Version of the EAP peer method interface
	 *
	 * The EAP peer method implementation should set this variable to
	 * EAP_PEER_METHOD_INTERFACE_VERSION. This is used to verify that the
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

#ifdef CONFIG_DYNAMIC_EAP_METHODS
	/**
	 * dl_handle - Handle for the dynamic library
	 *
	 * This variable is used internally in the EAP method registration code
	 * to store a handle for the dynamic library. If the method is linked
	 * in statically, this is %NULL.
	 */
	void *dl_handle;
#endif /* CONFIG_DYNAMIC_EAP_METHODS */

	/**
	 * get_emsk - Get EAP method specific keying extended material (EMSK)
	 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
	 * @priv: Pointer to private EAP method data from eap_method::init()
	 * @len: Pointer to a variable to store EMSK length
	 * Returns: EMSK or %NULL if not available
	 *
	 * This function can be used to get the extended keying material from
	 * the EAP method. The key may already be stored in the method-specific
	 * private data or this function may derive the key.
	 */
	//u8 * (*get_emsk)(struct eap_sm *sm, void *priv, size_t *len);

	/**
	 * getSessionId - Get EAP method specific Session-Id
	 * @sm: Pointer to EAP state machine allocated with eap_peer_sm_init()
	 * @priv: Pointer to private EAP method data from eap_method::init()
	 * @len: Pointer to a variable to store Session-Id length
	 * Returns: Session-Id or %NULL if not available
	 *
	 * This function can be used to get the Session-Id from the EAP method.
	 * The Session-Id may already be stored in the method-specific private
	 * data or this function may derive the Session-Id.
	 */
	//u8 * (*getSessionId)(struct eap_sm *sm, void *priv, size_t *len);
};
#endif

struct eap_server_method {
	int vendor;
	EapType method;
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
const u8 *eap_hdr_validate(int vendor, EapType eap_type,
						   const struct wpabuf *msg, size_t *plen);
struct wpabuf *eap_msg_alloc(int vendor, EapType type, size_t payload_len,
							 u8 code, u8 identifier);
void eap_update_len(struct wpabuf *msg);
u8 eap_get_id(const struct wpabuf *msg);
EapType eap_get_type(const struct wpabuf *msg);

#endif /* EAP_COMMON_H */
