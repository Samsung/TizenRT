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
/*
 * Copyright (c) 1997-2007  The Stanford SRP Authentication Project
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT SHALL STANFORD BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Redistributions in source or binary form must retain an intact copy
 * of this copyright notice.
 */
#include <tls/hap/t_defines.h>
#include <tls/hap/srp.h>
#include <tls/hap/t_sha.h>

/*
 * SRP-6 has two minor refinements relative to SRP-3/RFC2945:
 * 1. The verifier is multipled by three in the server's
 *    calculation for B.
 * 2. The value of u is taken as the hash of A and B,
 *    instead of the top 32 bits of the hash of B.
 *    This eliminates the old restriction where the
 *    server had to receive A before it could send B.
 */

/*
 * The RFC2945 server keeps track of the running hash state via
 * SHACTX structures pointed to by the meth_data pointer.
 * The "hash" member is the hash value that will be sent to the
 * other side; the "ckhash" member is the hash value expected
 * from the other side.  The server also keeps two more "old"
 * hash states, for backwards-compatibility.
 */
struct server_meth_st {
	SHACTX hash;
	SHACTX ckhash;
	SHACTX oldhash;
	SHACTX oldckhash;
	unsigned char k[RFC2945_KEY_LEN];
	unsigned char r[RFC2945_RESP_LEN];
};

#define SERVER_CTXP(srp)    ((struct server_meth_st *)(srp)->meth_data)

static SRP_RESULT srp6_server_init(SRP *srp)
{
	srp->magic = SRP_MAGIC_SERVER;
	srp->flags = SRP_FLAG_MOD_ACCEL;
	srp->meth_data = malloc(sizeof(struct server_meth_st));
	SHAInit(&SERVER_CTXP(srp)->hash);
	SHAInit(&SERVER_CTXP(srp)->ckhash);
	SHAInit(&SERVER_CTXP(srp)->oldhash);
	SHAInit(&SERVER_CTXP(srp)->oldckhash);
	return SRP_SUCCESS;
}

static SRP_RESULT srp6a_server_init(SRP *srp)
{
	srp->magic = SRP_MAGIC_SERVER;
	srp->flags = SRP_FLAG_MOD_ACCEL | SRP_FLAG_LEFT_PAD;
	srp->meth_data = malloc(sizeof(struct server_meth_st));
	SHAInit(&SERVER_CTXP(srp)->hash);
	SHAInit(&SERVER_CTXP(srp)->ckhash);
	SHAInit(&SERVER_CTXP(srp)->oldhash);
	SHAInit(&SERVER_CTXP(srp)->oldckhash);
	return SRP_SUCCESS;
}

static SRP_RESULT srp6_server_finish(SRP *srp)
{
	if (srp->meth_data) {
		memset(srp->meth_data, 0, sizeof(struct server_meth_st));
		free(srp->meth_data);
	}
	return SRP_SUCCESS;
}

static SRP_RESULT srp6_server_params(SRP *srp, const unsigned char *modulus, int modlen, const unsigned char *generator, int genlen, const unsigned char *salt, int saltlen)
{
	unsigned char buf1[SHA_DIGESTSIZE], buf2[SHA_DIGESTSIZE];
	SHACTX ctxt;
	int i;

	/* Fields set by SRP_set_params */

	/* Update hash state */
	SHAInit(&ctxt);
	SHAUpdate(&ctxt, modulus, modlen);
	SHAFinal(buf1, &ctxt);		/* buf1 = H(modulus) */

	SHAInit(&ctxt);
	SHAUpdate(&ctxt, generator, genlen);
	SHAFinal(buf2, &ctxt);		/* buf2 = H(generator) */

	for (i = 0; i < sizeof(buf1); ++i) {
		buf1[i] ^= buf2[i];    /* buf1 = H(modulus) XOR H(generator) */
	}

	/* ckhash: H(N) xor H(g) */
	SHAUpdate(&SERVER_CTXP(srp)->ckhash, buf1, sizeof(buf1));

	SHAInit(&ctxt);
	SHAUpdate(&ctxt, (const unsigned char *)(srp->username->data), srp->username->length);
	SHAFinal(buf1, &ctxt);		/* buf1 = H(user) */

	/* ckhash: (H(N) xor H(g)) | H(U) */
	SHAUpdate(&SERVER_CTXP(srp)->ckhash, buf1, sizeof(buf1));

	/* ckhash: (H(N) xor H(g)) | H(U) | s */
	SHAUpdate(&SERVER_CTXP(srp)->ckhash, salt, saltlen);

	return SRP_SUCCESS;
}

static SRP_RESULT srp6_server_auth(SRP *srp, const unsigned char *a, int alen)
{
	/* On the server, the authenticator is the verifier */
	srp->verifier = BigIntegerFromBytes(a, alen);

	return SRP_SUCCESS;
}

/* Normally this method isn't called, except maybe by test programs */
static SRP_RESULT srp6_server_passwd(SRP *srp, const unsigned char *p, int plen)
{
	SHACTX ctxt;
	unsigned char dig[SHA_DIGESTSIZE];

	SHAInit(&ctxt);
	SHAUpdate(&ctxt, (const unsigned char *)(srp->username->data), srp->username->length);
	SHAUpdate(&ctxt, (const unsigned char *)":", 1);
	SHAUpdate(&ctxt, p, plen);
	SHAFinal(dig, &ctxt);		/* dig = H(U | ":" | P) */

	SHAInit(&ctxt);
	SHAUpdate(&ctxt, (const unsigned char *)(srp->salt->data), srp->salt->length);
	SHAUpdate(&ctxt, dig, sizeof(dig));
	SHAFinal(dig, &ctxt);		/* dig = H(s | H(U | ":" | P)) */
	memset(&ctxt, 0, sizeof(ctxt));

	srp->password = BigIntegerFromBytes(dig, sizeof(dig));
	memset(dig, 0, sizeof(dig));

	/* verifier = g^x mod N */
	srp->verifier = BigIntegerFromInt(0);
	BigIntegerModExp(srp->verifier, srp->generator, srp->password, srp->modulus, srp->bctx, srp->accel);

	return SRP_SUCCESS;
}

/* NOTE: this clobbers k */
static SRP_RESULT srp6_server_genpub_ex(SRP *srp, cstr **result, BigInteger k)
{
	cstr *bstr;
	int slen = (SRP_get_secret_bits(BigIntegerBitLen(srp->modulus)) + 7) / 8;

	if (result == NULL) {
		bstr = cstr_new();
	} else {
		if (*result == NULL) {
			*result = cstr_new();
		}
		bstr = *result;
	}

	cstr_set_length(bstr, BigIntegerByteLen(srp->modulus));
	t_random((unsigned char *)(bstr->data), slen);
	srp->secret = BigIntegerFromBytes((const unsigned char *)(bstr->data), slen);
	srp->pubkey = BigIntegerFromInt(0);

	/* B = kv + g^b mod n (blinding) */
	BigIntegerMul(srp->pubkey, k, srp->verifier, srp->bctx);
	BigIntegerModExp(k, srp->generator, srp->secret, srp->modulus, srp->bctx, srp->accel);
	BigIntegerAdd(k, k, srp->pubkey);
	BigIntegerMod(srp->pubkey, k, srp->modulus, srp->bctx);

	BigIntegerToCstr(srp->pubkey, bstr);

	/* oldckhash: B */
	SHAUpdate(&SERVER_CTXP(srp)->oldckhash, (const unsigned char *)(bstr->data), bstr->length);

	if (result == NULL) {		/* bstr was a temporary */
		cstr_clear_free(bstr);
	}

	return SRP_SUCCESS;
}

static SRP_RESULT srp6_server_genpub(SRP *srp, cstr **result)
{
	SRP_RESULT ret;
	BigInteger k;

	k = BigIntegerFromInt(3);
	ret = srp6_server_genpub_ex(srp, result, k);
	BigIntegerClearFree(k);
	return ret;
}

static SRP_RESULT srp6a_server_genpub(SRP *srp, cstr **result)
{
	SRP_RESULT ret;
	BigInteger k;
	cstr *s;
	SHACTX ctxt;
	unsigned char dig[SHA_DIGESTSIZE];

	SHAInit(&ctxt);
	s = cstr_new();
	BigIntegerToCstr(srp->modulus, s);
	SHAUpdate(&ctxt, (const unsigned char *)(s->data), s->length);
	if (srp->flags & SRP_FLAG_LEFT_PAD) {
		BigIntegerToCstrEx(srp->generator, s, s->length);
	} else {
		BigIntegerToCstr(srp->generator, s);
	}
	SHAUpdate(&ctxt, (const unsigned char *)(s->data), s->length);
	SHAFinal(dig, &ctxt);
	cstr_free(s);

	k = BigIntegerFromBytes(dig, SHA_DIGESTSIZE);
	if (BigIntegerCmpInt(k, 0) == 0) {
		ret = SRP_ERROR;
	} else {
		ret = srp6_server_genpub_ex(srp, result, k);
	}
	BigIntegerClearFree(k);
	return ret;
}

static SRP_RESULT srp6_server_key(SRP *srp, cstr **result, const unsigned char *pubkey, int pubkeylen)
{
	cstr *s;
	BigInteger t1, t2, t3;
	SHACTX ctxt;
	unsigned char dig[SHA_DIGESTSIZE];
	int modlen;

	modlen = BigIntegerByteLen(srp->modulus);
	if (pubkeylen > modlen) {
		return SRP_ERROR;
	}

	/* ckhash: (H(N) xor H(g)) | H(U) | s | A */
	SHAUpdate(&SERVER_CTXP(srp)->ckhash, pubkey, pubkeylen);

	s = cstr_new();
	BigIntegerToCstr(srp->pubkey, s);	/* get encoding of B */

	/* ckhash: (H(N) xor H(g)) | H(U) | s | A | B */
	SHAUpdate(&SERVER_CTXP(srp)->ckhash, (const unsigned char *)(s->data), s->length);

	/* hash: A */
	SHAUpdate(&SERVER_CTXP(srp)->hash, pubkey, pubkeylen);
	/* oldhash: A */
	SHAUpdate(&SERVER_CTXP(srp)->oldhash, pubkey, pubkeylen);

	/* Compute u = H(pubkey || mypubkey) */
	SHAInit(&ctxt);
	if (srp->flags & SRP_FLAG_LEFT_PAD) {
		if (pubkeylen < modlen) {
			cstr_set_length(s, modlen);
			memcpy(s->data + (modlen - pubkeylen), pubkey, pubkeylen);
			memset(s->data, 0, modlen - pubkeylen);
			SHAUpdate(&ctxt, (const unsigned char *)(s->data), modlen);
			BigIntegerToCstrEx(srp->pubkey, s, modlen);
		} else {
			SHAUpdate(&ctxt, pubkey, pubkeylen);
			if (s->length < modlen) {
				BigIntegerToCstrEx(srp->pubkey, s, modlen);
			}
		}
	} else {
		SHAUpdate(&ctxt, pubkey, pubkeylen);
	}
	SHAUpdate(&ctxt, (const unsigned char *)(s->data), s->length);
	SHAFinal(dig, &ctxt);		/* dig = H(A || B) */
	srp->u = BigIntegerFromBytes(dig, SHA_DIGESTSIZE);

	/* compute A*v^u */
	t1 = BigIntegerFromInt(0);
	BigIntegerModExp(t1, srp->verifier, srp->u, srp->modulus, srp->bctx, srp->accel);	/* t1 = v^u */
	t2 = BigIntegerFromBytes(pubkey, pubkeylen);	/* t2 = A */
	t3 = BigIntegerFromInt(0);
	BigIntegerModMul(t3, t2, t1, srp->modulus, srp->bctx);	/* t3 = A*v^u (mod N) */
	BigIntegerFree(t2);

	if (BigIntegerCmpInt(t3, 1) <= 0) {	/* Reject A*v^u == 0,1 (mod N) */
		BigIntegerClearFree(t1);
		BigIntegerClearFree(t3);
		cstr_free(s);
		return SRP_ERROR;
	}

	BigIntegerAddInt(t1, t3, 1);
	if (BigIntegerCmp(t1, srp->modulus) == 0) {	/* Reject A*v^u == -1 (mod N) */
		BigIntegerClearFree(t1);
		BigIntegerClearFree(t3);
		cstr_free(s);
		return SRP_ERROR;
	}

	srp->key = BigIntegerFromInt(0);
	BigIntegerModExp(srp->key, t3, srp->secret, srp->modulus, srp->bctx, srp->accel);	/* (Av^u)^b */
	BigIntegerClearFree(t1);
	BigIntegerClearFree(t3);

	/* convert srp->key into session key, update hashes */
	SHACTX sctx;
	BigIntegerToCstr(srp->key, s);
	SHAInit(&sctx);
	SHAUpdate(&sctx, (const unsigned char *)(s->data), s->length);
	SHAFinal(SERVER_CTXP(srp)->k, &sctx);
	cstr_clear_free(s);

	/* ckhash: (H(N) xor H(g)) | H(U) | s | A | B | K */
	SHAUpdate(&SERVER_CTXP(srp)->ckhash, SERVER_CTXP(srp)->k, RFC2945_KEY_LEN);
	/* ckhash: (H(N) xor H(g)) | H(U) | s | A | B | K | ex_data */
	if (srp->ex_data->length > 0) {
		SHAUpdate(&SERVER_CTXP(srp)->ckhash, (const unsigned char *)(srp->ex_data->data), srp->ex_data->length);
	}

	/* oldhash: A | K */
	SHAUpdate(&SERVER_CTXP(srp)->oldhash, SERVER_CTXP(srp)->k, RFC2945_KEY_LEN);
	/* oldckhash: B | K */
	SHAUpdate(&SERVER_CTXP(srp)->oldckhash, SERVER_CTXP(srp)->k, RFC2945_KEY_LEN);

	if (result) {
		if (*result == NULL) {
			*result = cstr_new();
		}
		cstr_setn(*result, (const char *)(SERVER_CTXP(srp)->k), RFC2945_KEY_LEN);
	}

	return SRP_SUCCESS;
}

static SRP_RESULT srp6_server_verify(SRP *srp, const unsigned char *proof, int prooflen)
{
	unsigned char expected[SHA_DIGESTSIZE];

	SHAFinal(expected, &SERVER_CTXP(srp)->oldckhash);
	if (prooflen == RFC2945_RESP_LEN && memcmp(expected, proof, prooflen) == 0) {
		SHAFinal(SERVER_CTXP(srp)->r, &SERVER_CTXP(srp)->oldhash);
		return SRP_SUCCESS;
	}
	SHAFinal(expected, &SERVER_CTXP(srp)->ckhash);
	if (prooflen == RFC2945_RESP_LEN && memcmp(expected, proof, prooflen) == 0) {
		/* hash: A | M | K */
		SHAUpdate(&SERVER_CTXP(srp)->hash, expected, sizeof(expected));
		SHAUpdate(&SERVER_CTXP(srp)->hash, SERVER_CTXP(srp)->k, RFC2945_KEY_LEN);
		SHAFinal(SERVER_CTXP(srp)->r, &SERVER_CTXP(srp)->hash);
		return SRP_SUCCESS;
	}
	return SRP_ERROR;
}

static SRP_RESULT srp6_server_respond(SRP *srp, cstr **proof)
{
	if (proof == NULL) {
		return SRP_ERROR;
	}

	if (*proof == NULL) {
		*proof = cstr_new();
	}

	cstr_set_length(*proof, RFC2945_RESP_LEN);
	memcpy((*proof)->data, SERVER_CTXP(srp)->r, RFC2945_RESP_LEN);
	return SRP_SUCCESS;
}

static SRP_METHOD srp6_server_meth = {
	"SRP-6 server (tjw)",
	srp6_server_init,
	srp6_server_finish,
	srp6_server_params,
	srp6_server_auth,
	srp6_server_passwd,
	srp6_server_genpub,
	srp6_server_key,
	srp6_server_verify,
	srp6_server_respond,
	NULL
};

_TYPE(SRP_METHOD *)
SRP6_server_method()
{
	return &srp6_server_meth;
}

static SRP_METHOD srp6a_server_meth = {
	"SRP-6a server (tjw)",
	srp6a_server_init,
	srp6_server_finish,
	srp6_server_params,
	srp6_server_auth,
	srp6_server_passwd,
	srp6a_server_genpub,
	srp6_server_key,
	srp6_server_verify,
	srp6_server_respond,
	NULL
};

_TYPE(SRP_METHOD *)
SRP6a_server_method()
{
	return &srp6a_server_meth;
}
