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
 * TLSv1 credentials
 * Copyright (c) 2006-2007, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef TLSV1_CRED_H
#define TLSV1_CRED_H

struct tlsv1_credentials {
	struct x509_certificate *trusted_certs;
	struct x509_certificate *cert;
	struct crypto_private_key *key;

	/* Diffie-Hellman parameters */
	u8 *dh_p; /* prime */
	size_t dh_p_len;
	u8 *dh_g; /* generator */
	size_t dh_g_len;
};


struct tlsv1_credentials * tlsv1_cred_alloc(void);
void tlsv1_cred_free(struct tlsv1_credentials *cred);
int tlsv1_set_ca_cert(struct tlsv1_credentials *cred, const char *cert,
		      const u8 *cert_blob, size_t cert_blob_len,
		      const char *path);
int tlsv1_set_cert(struct tlsv1_credentials *cred, const char *cert,
		   const u8 *cert_blob, size_t cert_blob_len);
int tlsv1_set_private_key(struct tlsv1_credentials *cred,
			  const char *private_key,
			  const char *private_key_passwd,
			  const u8 *private_key_blob,
			  size_t private_key_blob_len);
int tlsv1_set_dhparams(struct tlsv1_credentials *cred, const char *dh_file,
		       const u8 *dh_blob, size_t dh_blob_len);

#endif /* TLSV1_CRED_H */
