/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef _SECURITY_API_KEYMGR_H__
#define _SECURITY_API_KEYMGR_H__

#include "security_common.h"

/**
 * Key Manager
 */

security_error keymgr_generate_key(security_handle hnd, security_key_type algo, const char *key_name);
security_error keymgr_set_key(security_handle hnd, security_key_type algo, const char *key_name, security_data *pubkey, security_data *prikey);
security_error keymgr_get_key(security_handle hnd, security_key_type algo, const char *key_name, security_data *pubkey_x, security_data *pubkey_y);
security_error keymgr_remove_key(security_handle hnd, security_key_type algo, const char *key_name);

#endif // _SECURITY_API_KEYMGR_H__
