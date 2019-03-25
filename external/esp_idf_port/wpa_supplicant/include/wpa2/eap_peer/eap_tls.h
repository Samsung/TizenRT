/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/*
 * EAP peer: EAP-TLS/PEAP/TTLS/FAST common functions
 * Copyright (c) 2004-2009, 2012, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef EAP_TLS_H
#define EAP_TLS_H

#include "eap_i.h"
#include "eap_common.h" 
#include "eap.h" 
#include "wpa/wpabuf.h" 

void * eap_tls_init(struct eap_sm *sm);
void eap_tls_deinit(struct eap_sm *sm, void *priv);
struct wpabuf * eap_tls_process(struct eap_sm *sm, void *priv,
                                       struct eap_method_ret *ret,
                                       const struct wpabuf *reqData);

u8 * eap_tls_getKey(struct eap_sm *sm, void *priv, size_t *len);

#endif /* EAP_TLS_H */
