/*
Copyright (c) 2010-2020 Roger Light <roger@atchoo.org>

All rights reserved. This program and the accompanying materials
are made available under the terms of the Eclipse Public License 2.0
and Eclipse Distribution License v1.0 which accompany this distribution.

The Eclipse Public License is available at
   https://www.eclipse.org/legal/epl-2.0/
and the Eclipse Distribution License is available at
  http://www.eclipse.org/org/documents/edl-v10.php.

SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause

Contributors:
   Roger Light - initial implementation and documentation.
*/

#include "config.h"

#ifndef WIN32
#  include <strings.h>
#endif

#include <string.h>

#ifdef WITH_TLS
#  ifdef WIN32
#    include <winsock2.h>
#  endif
#  include <openssl/engine.h>
#endif

#include "mosquitto.h"
#include "mosquitto_internal.h"
#include "memory_mosq.h"
#include "misc_mosq.h"
#include "mqtt_protocol.h"
#include "util_mosq.h"
#include "will_mosq.h"


int mosquitto_will_set(struct mosquitto *mosq, const char *topic, int payloadlen, const void *payload, int qos, bool retain)
{
	return mosquitto_will_set_v5(mosq, topic, payloadlen, payload, qos, retain, NULL);
}


int mosquitto_will_set_v5(struct mosquitto *mosq, const char *topic, int payloadlen, const void *payload, int qos, bool retain, mosquitto_property *properties)
{
	int rc;

	if(!mosq) return MOSQ_ERR_INVAL;

	if(properties){
		rc = mosquitto_property_check_all(CMD_WILL, properties);
		if(rc) return rc;
	}

	return will__set(mosq, topic, payloadlen, payload, qos, retain, properties);
}


int mosquitto_will_clear(struct mosquitto *mosq)
{
	if(!mosq) return MOSQ_ERR_INVAL;
	return will__clear(mosq);
}


int mosquitto_username_pw_set(struct mosquitto *mosq, const char *username, const char *password)
{
	size_t slen;

	if(!mosq) return MOSQ_ERR_INVAL;

	if(mosq->protocol == mosq_p_mqtt311 || mosq->protocol == mosq_p_mqtt31){
		if(password != NULL && username == NULL){
			return MOSQ_ERR_INVAL;
		}
	}

	mosquitto__free(mosq->username);
	mosq->username = NULL;

	mosquitto__free(mosq->password);
	mosq->password = NULL;

	if(username){
		slen = strlen(username);
		if(slen > UINT16_MAX){
			return MOSQ_ERR_INVAL;
		}
		if(mosquitto_validate_utf8(username, (int)slen)){
			return MOSQ_ERR_MALFORMED_UTF8;
		}
		mosq->username = mosquitto__strdup(username);
		if(!mosq->username) return MOSQ_ERR_NOMEM;
	}

	if(password){
		mosq->password = mosquitto__strdup(password);
		if(!mosq->password){
			mosquitto__free(mosq->username);
			mosq->username = NULL;
			return MOSQ_ERR_NOMEM;
		}
	}
	return MOSQ_ERR_SUCCESS;
}


int mosquitto_reconnect_delay_set(struct mosquitto *mosq, unsigned int reconnect_delay, unsigned int reconnect_delay_max, bool reconnect_exponential_backoff)
{
	if(!mosq) return MOSQ_ERR_INVAL;

	if(reconnect_delay == 0) reconnect_delay = 1;

	mosq->reconnect_delay = reconnect_delay;
	mosq->reconnect_delay_max = reconnect_delay_max;
	mosq->reconnect_exponential_backoff = reconnect_exponential_backoff;

	return MOSQ_ERR_SUCCESS;
}


int mosquitto_tls_set(struct mosquitto *mosq, const char *cafile, const char *capath, const char *certfile, const char *keyfile, int (*pw_callback)(char *buf, int size, int rwflag, void *userdata))
{
#ifdef WITH_TLS
	FILE *fptr;

	if(!mosq || (!cafile && !capath) || (certfile && !keyfile) || (!certfile && keyfile)) return MOSQ_ERR_INVAL;

	mosquitto__free(mosq->tls_cafile);
	mosq->tls_cafile = NULL;
	if(cafile){
		fptr = mosquitto__fopen(cafile, "rt", false);
		if(fptr){
			fclose(fptr);
		}else{
			return MOSQ_ERR_INVAL;
		}
		mosq->tls_cafile = mosquitto__strdup(cafile);

		if(!mosq->tls_cafile){
			return MOSQ_ERR_NOMEM;
		}
	}

	mosquitto__free(mosq->tls_capath);
	mosq->tls_capath = NULL;
	if(capath){
		mosq->tls_capath = mosquitto__strdup(capath);
		if(!mosq->tls_capath){
			return MOSQ_ERR_NOMEM;
		}
	}

	mosquitto__free(mosq->tls_certfile);
	mosq->tls_certfile = NULL;
	if(certfile){
		fptr = mosquitto__fopen(certfile, "rt", false);
		if(fptr){
			fclose(fptr);
		}else{
			mosquitto__free(mosq->tls_cafile);
			mosq->tls_cafile = NULL;

			mosquitto__free(mosq->tls_capath);
			mosq->tls_capath = NULL;
			return MOSQ_ERR_INVAL;
		}
		mosq->tls_certfile = mosquitto__strdup(certfile);
		if(!mosq->tls_certfile){
			return MOSQ_ERR_NOMEM;
		}
	}

	mosquitto__free(mosq->tls_keyfile);
	mosq->tls_keyfile = NULL;
	if(keyfile){
		fptr = mosquitto__fopen(keyfile, "rt", false);
		if(fptr){
			fclose(fptr);
		}else{
			mosquitto__free(mosq->tls_cafile);
			mosq->tls_cafile = NULL;

			mosquitto__free(mosq->tls_capath);
			mosq->tls_capath = NULL;

			mosquitto__free(mosq->tls_certfile);
			mosq->tls_certfile = NULL;
			return MOSQ_ERR_INVAL;
		}
		mosq->tls_keyfile = mosquitto__strdup(keyfile);
		if(!mosq->tls_keyfile){
			return MOSQ_ERR_NOMEM;
		}
	}

	mosq->tls_pw_callback = pw_callback;


	return MOSQ_ERR_SUCCESS;
#else
#ifdef WITH_MBEDTLS
	if (!mosq || !cafile) {
		return MOSQ_ERR_INVAL;
	}

	mosq->tls_ca_cert = (char *)cafile;
	mosq->tls_cert = (char *)certfile;
	mosq->tls_key = (char *)keyfile;

	mosq->cert = malloc(sizeof(mbedtls_x509_crt));
	mosq->pkey = malloc(sizeof(mbedtls_pk_context));
	mosq->entropy = malloc(sizeof(mbedtls_entropy_context));
	mosq->ctr_drbg = malloc(sizeof(mbedtls_ctr_drbg_context));
	mosq->cache = malloc(sizeof(mbedtls_ssl_cache_context));

	if (!mosq->cert || !mosq->pkey || !mosq->entropy || !mosq->ctr_drbg || !mosq->cache) {
		return MOSQ_ERR_NOMEM;
	}

	/* 0. Initialize tls sub params */
	mbedtls_x509_crt_init(mosq->cert);
	mbedtls_pk_init(mosq->pkey);
	mbedtls_entropy_init(mosq->entropy);
	mbedtls_ctr_drbg_init(mosq->ctr_drbg);
	mbedtls_ssl_cache_init(mosq->cache);

	/* 1. Load the certificates and private RSA key */
	if (mosq->tls_cert) {
		if (mbedtls_x509_crt_parse(mosq->cert, (const unsigned char *)mosq->tls_cert, mosq->tls_cert_len) != 0) {
			log__printf(mosq, MOSQ_LOG_ERR, "Error: mbedtls_x509_crt_parse fail");
			return MOSQ_ERR_TLS;
		}
	}

	if (mbedtls_x509_crt_parse(mosq->cert, (const unsigned char *)mosq->tls_ca_cert, mosq->tls_ca_cert_len) != 0) {
		log__printf(mosq, MOSQ_LOG_ERR, "Error: mbedtls_x509_crt_parse_ca fail");
		return MOSQ_ERR_TLS;
	}

	if (mosq->tls_key) {
		if (mbedtls_pk_parse_key(mosq->pkey, (const unsigned char *)mosq->tls_key, mosq->tls_key_len, NULL, 0) != 0) {
			log__printf(mosq, MOSQ_LOG_ERR, "Error: mbedtls_pk_parse_key fail");
			return MOSQ_ERR_TLS;
		}
	}

	if (mbedtls_ctr_drbg_seed(mosq->ctr_drbg, mbedtls_entropy_func, mosq->entropy, NULL, 0) != 0) {
		log__printf(mosq, MOSQ_LOG_ERR, "Error: mbedtls_ctr_drbg_seed fail");
		return MOSQ_ERR_TLS;
	}

	/* 2. Set ssl */
	mosq->ssl = malloc(sizeof(mbedtls_ssl_config));

	if (!mosq->ssl) {
		return MOSQ_ERR_NOMEM;
	}
#if defined(MBEDTLS_SSL_SESSION_TICKETS)
	mbedtls_ssl_conf_session_tickets(mosq->ssl, 0);
#endif
	mbedtls_ssl_config_init(mosq->ssl);

	if (mbedtls_ssl_config_defaults(mosq->ssl, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT) != 0) {
		log__printf(mosq, MOSQ_LOG_ERR, "Error: mbedtls_ssl_config_defaults fail");
		return MOSQ_ERR_TLS;
	}
	mbedtls_ssl_conf_rng(mosq->ssl, mbedtls_ctr_drbg_random, mosq->ctr_drbg);
	if (mosq->tls_cert && mosq->tls_key) {
		mbedtls_ssl_conf_ca_chain(mosq->ssl, ((mbedtls_x509_crt *)mosq->cert)->next, NULL);

		if (mbedtls_ssl_conf_own_cert(mosq->ssl, mosq->cert, mosq->pkey) != 0) {
			log__printf(mosq, MOSQ_LOG_ERR, "Error: mbedtls_ssl_conf_own_cert fail");
			return MOSQ_ERR_TLS;
		}
	} else if (mosq->tls_cert) {
		mbedtls_ssl_conf_ca_chain(mosq->ssl, ((mbedtls_x509_crt *)mosq->cert)->next, NULL);

		if (mbedtls_ssl_conf_own_cert(mosq->ssl, mosq->cert, NULL) != 0) {
			log__printf(mosq, MOSQ_LOG_ERR, "Error: mbedtls_ssl_conf_own_cert fail");
			return MOSQ_ERR_TLS;
		}
	} else {
		mbedtls_ssl_conf_ca_chain(mosq->ssl, ((mbedtls_x509_crt *)mosq->cert), NULL);
	}

	mbedtls_ssl_conf_authmode(mosq->ssl, mosq->tls_cert_reqs);

#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_cache_init(mosq->cache);
	mbedtls_ssl_conf_session_cache(mosq->ssl, mosq->cache, mbedtls_ssl_cache_get, mbedtls_ssl_cache_set);
#endif
	mosq->mbedtls_state = mosq_mbedtls_state_enabled;
	return MOSQ_ERR_SUCCESS;
#else
	UNUSED(mosq);
	UNUSED(cafile);
	UNUSED(capath);
	UNUSED(certfile);
	UNUSED(keyfile);
	UNUSED(pw_callback);

	return MOSQ_ERR_NOT_SUPPORTED;
#endif
#endif
}


int mosquitto_tls_opts_set(struct mosquitto *mosq, int cert_reqs, const char *tls_version, const char *ciphers)
{
#ifdef WITH_TLS
	if(!mosq) return MOSQ_ERR_INVAL;

	mosq->tls_cert_reqs = cert_reqs;
	if(tls_version){
		if(!strcasecmp(tls_version, "tlsv1.3")
				|| !strcasecmp(tls_version, "tlsv1.2")
				|| !strcasecmp(tls_version, "tlsv1.1")){

			mosq->tls_version = mosquitto__strdup(tls_version);
			if(!mosq->tls_version) return MOSQ_ERR_NOMEM;
		}else{
			return MOSQ_ERR_INVAL;
		}
	}else{
		mosq->tls_version = mosquitto__strdup("tlsv1.2");
		if(!mosq->tls_version) return MOSQ_ERR_NOMEM;
	}
	if(ciphers){
		mosq->tls_ciphers = mosquitto__strdup(ciphers);
		if(!mosq->tls_ciphers) return MOSQ_ERR_NOMEM;
	}else{
		mosq->tls_ciphers = NULL;
	}


	return MOSQ_ERR_SUCCESS;
#else
	UNUSED(mosq);
	UNUSED(cert_reqs);
	UNUSED(tls_version);
	UNUSED(ciphers);

	return MOSQ_ERR_NOT_SUPPORTED;
#endif
}


int mosquitto_tls_insecure_set(struct mosquitto *mosq, bool value)
{
#ifdef WITH_TLS
	if(!mosq) return MOSQ_ERR_INVAL;
	mosq->tls_insecure = value;
	return MOSQ_ERR_SUCCESS;
#else
#ifdef WITH_MBEDTLS
	if (!mosq) {
		return MOSQ_ERR_INVAL;
	}
	mosq->tls_insecure = value;
	return MOSQ_ERR_SUCCESS;
#else
	UNUSED(mosq);
	UNUSED(value);
#endif
	return MOSQ_ERR_NOT_SUPPORTED;
#endif
}


int mosquitto_string_option(struct mosquitto *mosq, enum mosq_opt_t option, const char *value)
{
#ifdef WITH_TLS
	ENGINE *eng;
	char *str;
#endif

	if(!mosq) return MOSQ_ERR_INVAL;

	switch(option){
		case MOSQ_OPT_TLS_ENGINE:
#if defined(WITH_TLS) && !defined(OPENSSL_NO_ENGINE)
			eng = ENGINE_by_id(value);
			if(!eng){
				return MOSQ_ERR_INVAL;
			}
			ENGINE_free(eng); /* release the structural reference from ENGINE_by_id() */
			mosq->tls_engine = mosquitto__strdup(value);
			if(!mosq->tls_engine){
				return MOSQ_ERR_NOMEM;
			}
			return MOSQ_ERR_SUCCESS;
#else
			return MOSQ_ERR_NOT_SUPPORTED;
#endif
			break;

		case MOSQ_OPT_TLS_KEYFORM:
#ifdef WITH_TLS
			if(!value) return MOSQ_ERR_INVAL;
			if(!strcasecmp(value, "pem")){
				mosq->tls_keyform = mosq_k_pem;
			}else if (!strcasecmp(value, "engine")){
				mosq->tls_keyform = mosq_k_engine;
			}else{
				return MOSQ_ERR_INVAL;
			}
			return MOSQ_ERR_SUCCESS;
#else
			return MOSQ_ERR_NOT_SUPPORTED;
#endif
			break;


		case MOSQ_OPT_TLS_ENGINE_KPASS_SHA1:
#ifdef WITH_TLS
			if(mosquitto__hex2bin_sha1(value, (unsigned char**)&str) != MOSQ_ERR_SUCCESS){
				return MOSQ_ERR_INVAL;
			}
			mosq->tls_engine_kpass_sha1 = str;
			return MOSQ_ERR_SUCCESS;
#else
			return MOSQ_ERR_NOT_SUPPORTED;
#endif
			break;

		case MOSQ_OPT_TLS_ALPN:
#ifdef WITH_TLS
			mosq->tls_alpn = mosquitto__strdup(value);
			if(!mosq->tls_alpn){
				return MOSQ_ERR_NOMEM;
			}
			return MOSQ_ERR_SUCCESS;
#else
			return MOSQ_ERR_NOT_SUPPORTED;
#endif
			break;

		case MOSQ_OPT_BIND_ADDRESS:
			mosquitto__free(mosq->bind_address);
			if(value){
				mosq->bind_address = mosquitto__strdup(value);
				if(mosq->bind_address){
					return MOSQ_ERR_SUCCESS;
				}else{
					return MOSQ_ERR_NOMEM;
				}
			}else{
				return MOSQ_ERR_SUCCESS;
			}


		default:
			return MOSQ_ERR_INVAL;
	}
}


int mosquitto_tls_psk_set(struct mosquitto *mosq, const char *psk, const char *identity, const char *ciphers)
{
#ifdef FINAL_WITH_TLS_PSK
	if(!mosq || !psk || !identity) return MOSQ_ERR_INVAL;

	/* Check for hex only digits */
	if(strspn(psk, "0123456789abcdefABCDEF") < strlen(psk)){
		return MOSQ_ERR_INVAL;
	}
	mosq->tls_psk = mosquitto__strdup(psk);
	if(!mosq->tls_psk) return MOSQ_ERR_NOMEM;

	mosq->tls_psk_identity = mosquitto__strdup(identity);
	if(!mosq->tls_psk_identity){
		mosquitto__free(mosq->tls_psk);
		return MOSQ_ERR_NOMEM;
	}
	if(ciphers){
		mosq->tls_ciphers = mosquitto__strdup(ciphers);
		if(!mosq->tls_ciphers) return MOSQ_ERR_NOMEM;
	}else{
		mosq->tls_ciphers = NULL;
	}

	return MOSQ_ERR_SUCCESS;
#else
	UNUSED(mosq);
	UNUSED(psk);
	UNUSED(identity);
	UNUSED(ciphers);

	return MOSQ_ERR_NOT_SUPPORTED;
#endif
}


int mosquitto_opts_set(struct mosquitto *mosq, enum mosq_opt_t option, void *value)
{
	int ival;

	if(!mosq) return MOSQ_ERR_INVAL;

	switch(option){
		case MOSQ_OPT_PROTOCOL_VERSION:
			if(value == NULL){
				return MOSQ_ERR_INVAL;
			}
			ival = *((int *)value);
			return mosquitto_int_option(mosq, option, ival);
		case MOSQ_OPT_SSL_CTX:
			return mosquitto_void_option(mosq, option, value);
		default:
			return MOSQ_ERR_INVAL;
	}
	return MOSQ_ERR_SUCCESS;
}


int mosquitto_int_option(struct mosquitto *mosq, enum mosq_opt_t option, int value)
{
	if(!mosq) return MOSQ_ERR_INVAL;

	switch(option){
		case MOSQ_OPT_PROTOCOL_VERSION:
			if(value == MQTT_PROTOCOL_V31){
				mosq->protocol = mosq_p_mqtt31;
			}else if(value == MQTT_PROTOCOL_V311){
				mosq->protocol = mosq_p_mqtt311;
			}else if(value == MQTT_PROTOCOL_V5){
				mosq->protocol = mosq_p_mqtt5;
			}else{
				return MOSQ_ERR_INVAL;
			}
			break;

		case MOSQ_OPT_RECEIVE_MAXIMUM:
			if(value < 0 || value > UINT16_MAX){
				return MOSQ_ERR_INVAL;
			}
			if(value == 0){
				mosq->msgs_in.inflight_maximum = UINT16_MAX;
			}else{
				mosq->msgs_in.inflight_maximum = (uint16_t)value;
			}
			break;

		case MOSQ_OPT_SEND_MAXIMUM:
			if(value < 0 || value > UINT16_MAX){
				return MOSQ_ERR_INVAL;
			}
			if(value == 0){
				mosq->msgs_out.inflight_maximum = UINT16_MAX;
			}else{
				mosq->msgs_out.inflight_maximum = (uint16_t)value;
			}
			break;

		case MOSQ_OPT_SSL_CTX_WITH_DEFAULTS:
#if defined(WITH_TLS) && OPENSSL_VERSION_NUMBER >= 0x10100000L
			if(value){
				mosq->ssl_ctx_defaults = true;
			}else{
				mosq->ssl_ctx_defaults = false;
			}
			break;
#else
			return MOSQ_ERR_NOT_SUPPORTED;
#endif

		case MOSQ_OPT_TLS_USE_OS_CERTS:
#ifdef WITH_TLS
			if(value){
				mosq->tls_use_os_certs = true;
			}else{
				mosq->tls_use_os_certs = false;
			}
			break;
#else
			return MOSQ_ERR_NOT_SUPPORTED;
#endif

		case MOSQ_OPT_TLS_OCSP_REQUIRED:
#ifdef WITH_TLS
			mosq->tls_ocsp_required = (bool)value;
#else
			return MOSQ_ERR_NOT_SUPPORTED;
#endif
			break;

		case MOSQ_OPT_TCP_NODELAY:
			mosq->tcp_nodelay = (bool)value;
			break;

		default:
			return MOSQ_ERR_INVAL;
	}
	return MOSQ_ERR_SUCCESS;
}


int mosquitto_void_option(struct mosquitto *mosq, enum mosq_opt_t option, void *value)
{
	if(!mosq) return MOSQ_ERR_INVAL;

	switch(option){
		case MOSQ_OPT_SSL_CTX:
#ifdef WITH_TLS
			mosq->user_ssl_ctx = (SSL_CTX *)value;
			if(mosq->user_ssl_ctx){
#if (OPENSSL_VERSION_NUMBER >= 0x10100000L)
				SSL_CTX_up_ref(mosq->user_ssl_ctx);
#else
				CRYPTO_add(&(mosq->user_ssl_ctx)->references, 1, CRYPTO_LOCK_SSL_CTX);
#endif
			}
			break;
#else
			return MOSQ_ERR_NOT_SUPPORTED;
#endif
		default:
			return MOSQ_ERR_INVAL;
	}
	return MOSQ_ERR_SUCCESS;
}


void mosquitto_user_data_set(struct mosquitto *mosq, void *userdata)
{
	if(mosq){
		mosq->userdata = userdata;
	}
}

void *mosquitto_userdata(struct mosquitto *mosq)
{
	return mosq->userdata;
}
