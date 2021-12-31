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
// command, type, handler
SL_AUTH_TEST_POOL("ecdh", SL_AUTH_TYPE_ECDH, sl_handle_auth_ecdh)
SL_AUTH_TEST_POOL("ecdhs", SL_AUTH_TYPE_ECDHS, sl_handle_auth_setkey_ecdh)
SL_AUTH_TEST_POOL("x25519", SL_AUTH_TYPE_X25519, sl_handle_auth_x25519)
SL_AUTH_TEST_POOL("x25519s", SL_AUTH_TYPE_X25519S, sl_handle_auth_setkey_x25519)
SL_AUTH_TEST_POOL("ecdsav", SL_AUTH_TYPE_ECDSA_VERIFY, sl_handle_auth_ecdsa_verify)
SL_AUTH_TEST_POOL("ecdsas", SL_AUTH_TYPE_ECDSA_SIGNATURE, sl_handle_auth_ecdsa_signature)
SL_AUTH_TEST_POOL("ed25519v", SL_AUTH_TYPE_ED25519_VERIFY, sl_handle_auth_ed25519_verify)
SL_AUTH_TEST_POOL("ed25519s", SL_AUTH_TYPE_ED25519_SIGNATURE, sl_handle_auth_ed25519_signature)
SL_AUTH_TEST_POOL("random", SL_AUTH_TYPE_GEN_RANDOM, sl_handle_auth_gen_random)
SL_AUTH_TEST_POOL("set_cert", SL_AUTH_TYPE_SET_CERT, sl_handle_auth_set_cert)
SL_AUTH_TEST_POOL("get_cert", SL_AUTH_TYPE_GET_CERT, sl_handle_auth_get_cert)
SL_AUTH_TEST_POOL("remove_cert", SL_AUTH_TYPE_REMOVE_CERT, sl_handle_auth_remove_cert)
