/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License\n");
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

#define SL_AUTH_USAGE                              \
	"\n authentication test\n"                     \
	"    TASH> sl_test auth [algorithm] [count]\n" \
	"    algorithm: \n"                            \
	"        all: run all authentication TC\n"     \
	"        ecdsav: get ecdsa verification"       \
	"        ecdsas: get ecdsa signature"          \
	"        ed25519v: get ed25519 verification"   \
	"        ed25519s: get ed25519 signature"      \
	"        ecdh: compute ecdh"                   \
	"        x25519: compute x25519"

#define SL_SS_USAGE                              \
	"\n secure storage test\n"                   \
	"    TASH> sl_test ss [operation] [count]\n" \
	"    operation: \n"                          \
	"        all: run all secure storage TC\n"   \
	"        write: test write operation\n"      \
	"        read: test read operation\n"        \
	"        delete: test delete operation\n"

#define SL_CRYPTO_USAGE                              \
	"\n crypto test (Not supported yet)\n"

#define SL_KEY_USAGE													\
	"\n key test (Not supported yet)\n"

#define SL_USAGE                            \
	"\n usage: sl_test [options]\n"         \
	"    run all tests\n"                   \
	"    TASH> sl_test all\n" \
	SL_AUTH_USAGE \
	SL_SS_USAGE\
	SL_CRYPTO_USAGE\
	SL_KEY_USAGE
