/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#pragma once

hal_key_type alt_get_keytype(unsigned int curve);
hal_ecdsa_curve alt_get_curve(mbedtls_ecp_group_id curve);
int alt_gen_key(sl_ctx shnd, hal_key_type key_type, int from);
int alt_set_key(sl_ctx shnd,
					   hal_key_type key_type,
					   hal_data *pubkey,
					   hal_data *prikey, int from);
void alt_print_buffer(char *data, int buf_len, const char *message);
