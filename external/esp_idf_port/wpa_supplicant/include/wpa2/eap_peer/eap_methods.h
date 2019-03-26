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
 * EAP peer: Method registration
 * Copyright (c) 2004-2007, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef EAP_METHODS_H
#define EAP_METHODS_H

#include "eap_defs.h"
#include "eap_config.h"

const struct eap_method * eap_peer_get_eap_method(int vendor, EapType method);
const struct eap_method * eap_peer_get_methods(size_t *count);

u32 eap_get_phase2_type(const char *name, int *vendor);
struct eap_method_type * eap_get_phase2_types(struct eap_peer_config *config,
					      size_t *count);

struct eap_method * eap_peer_method_alloc(int verdor, EapType method,
					  const char *name);

void eap_peer_method_free(struct eap_method *method);
int eap_peer_method_register(struct eap_method *method);

void eap_peer_unregister_methods(void);

//int eap_peer_md5_register(void);
int eap_peer_tls_register(void);
int eap_peer_peap_register(void);
int eap_peer_ttls_register(void);
int eap_peer_mschapv2_register(void);

void eap_peer_unregister_methods(void);
int eap_peer_register_methods(void);

#endif /* EAP_METHODS_H */
