/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
#include "Configuration.h"

typedef SHACTX ed25519_hash_context;

static void ed25519_hash_init(ed25519_hash_context *ctx)
{
	SHAInit(ctx);
}

static void ed25519_hash_update(ed25519_hash_context *ctx, const uint8_t *in, size_t inlen)
{
	SHAUpdate(ctx, in, inlen);
}

static void ed25519_hash_final(ed25519_hash_context *ctx, uint8_t *hash)
{
	SHAFinal(hash, ctx);
}

static void ed25519_hash(uint8_t *hash, const uint8_t *in, size_t inlen)
{
	SHA512(in, inlen, hash);
}
