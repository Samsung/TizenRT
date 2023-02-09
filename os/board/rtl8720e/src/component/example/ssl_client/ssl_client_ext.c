#include "FreeRTOS.h"
#include "platform_opts.h"

#include "mbedtls/config.h"
#include "mbedtls/platform.h"
#include "mbedtls/ssl.h"

//#define SSL_VERIFY_CLIENT
//#define SSL_VERIFY_SERVER

#ifdef SSL_VERIFY_CLIENT
static mbedtls_x509_crt *_cli_crt = NULL;
static mbedtls_pk_context *_clikey_rsa = NULL;

#if defined(configENABLE_TRUSTZONE) && (configENABLE_TRUSTZONE == 1) && defined(CONFIG_SSL_CLIENT_PRIVATE_IN_TZ) && (CONFIG_SSL_CLIENT_PRIVATE_IN_TZ == 1)
//client key is moved to secure zone
#else
static const unsigned char *test_client_key = \
		"-----BEGIN RSA PRIVATE KEY-----\r\n" \
		"MIICXAIBAAKBgQDxzEGPfEgtHIvwr2Q9uVwfe+HaP80ymn7v8ANHp6kvL870fnij\r\n" \
		"oJlcMlDbYE5lZmHA3Z6t4wSLh5k1rAFakGNDvl6G/mF45vSCAWgrN+JOqBVBSdQ0\r\n" \
		"iu+qlRwZMvE9yI68a9OLT8w1ijaFAGkh2p4jn9P1++t9Gf25B50wngZpNwIDAQAB\r\n" \
		"AoGADjCUcF00xyDzvW4tjcnA9EAK4wn3es7CQEZvitb2AMTnekWpwsB7/4McZ6Nh\r\n" \
		"NmYpum3HECFQdPfCL/omIjBzAruFHzx8Z0QAMN/cKIiQSd5Tx8MbKkOn8teHJe/u\r\n" \
		"C/wLu/Qtt4l/E0uqyIcW/cyYcnN1/oOBfZglOGTiJ3Q0QNECQQD/Y1FLhzkQ32Ib\r\n" \
		"Jc70ExPwvIvIu8jjFVyGXEnHtvRF478LgyPeLepiuvNVTTWpQs5bIqT2lRBrgfjO\r\n" \
		"B+mptDa9AkEA8mCZ1cFfGAQGvEGJFH7K+6nGwGa9eRyRdDEAmnz9Fx8TNu8bv3S0\r\n" \
		"rFYl2GdYc4DwGWWz91vxTHLtvzRW59OpAwJACBn/dzTn84gSlafiY83DISzqBkkm\r\n" \
		"u0VvNPfygzpjj2AFLHlRhvfP6qfhlkAnxqHO6Ac2UngyGCb8XJTVrcOAxQJAAb3t\r\n" \
		"/RpJq6uDvOCfspUCsnJS6lkSrYcmCHa9sV6KlxrzEWcAhQy6q65m0TpU6WeY0iXn\r\n" \
		"+DQh9NGyobgnk7zyrwJBAMkjC/a9aeUA9y/ePoY5gVzWjPBNKJJplDzJvDLANG5k\r\n" \
		"7zonQ5Fu2JEZTjh2p3Ev1X3SmhuEijHYoaiXFTUAjlE=\r\n" \
		"-----END RSA PRIVATE KEY-----\r\n";
#endif

static const unsigned char *test_client_cert = \
		"-----BEGIN CERTIFICATE-----\r\n" \
		"MIIC3zCCAkigAwIBAgIBAjANBgkqhkiG9w0BAQsFADB7MQswCQYDVQQGEwJDTjEL\r\n" \
		"MAkGA1UECAwCSlMxCzAJBgNVBAcMAlNaMRAwDgYDVQQKDAdSZWFsc2lsMRAwDgYD\r\n" \
		"VQQLDAdSZWFsdGVrMRAwDgYDVQQDDAdSZWFsc2lsMRwwGgYJKoZIhvcNAQkBFg1h\r\n" \
		"QHJlYWxzaWwuY29tMB4XDTE2MDkxNDA2MDg0N1oXDTE3MDkxNDA2MDg0N1owczEL\r\n" \
		"MAkGA1UEBhMCQ04xCzAJBgNVBAgMAkpTMRAwDgYDVQQKDAdSZWFsc2lsMRAwDgYD\r\n" \
		"VQQLDAdSZWFsdGVrMRUwEwYDVQQDDAwxOTIuMTY4LjEuNTIxHDAaBgkqhkiG9w0B\r\n" \
		"CQEWDWNAcmVhbHNpbC5jb20wgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJAoGBAPHM\r\n" \
		"QY98SC0ci/CvZD25XB974do/zTKafu/wA0enqS8vzvR+eKOgmVwyUNtgTmVmYcDd\r\n" \
		"nq3jBIuHmTWsAVqQY0O+Xob+YXjm9IIBaCs34k6oFUFJ1DSK76qVHBky8T3Ijrxr\r\n" \
		"04tPzDWKNoUAaSHaniOf0/X7630Z/bkHnTCeBmk3AgMBAAGjezB5MAkGA1UdEwQC\r\n" \
		"MAAwLAYJYIZIAYb4QgENBB8WHU9wZW5TU0wgR2VuZXJhdGVkIENlcnRpZmljYXRl\r\n" \
		"MB0GA1UdDgQWBBRrALVkL8aJIm4zSHfSHRlPZtPWUzAfBgNVHSMEGDAWgBSg0idX\r\n" \
		"+PDX/BDU0kIRPUDia1ntRTANBgkqhkiG9w0BAQsFAAOBgQCdo9WhM1cSV/wiHQAa\r\n" \
		"b7OTj4tIUbTpXzA7zv0Ib5VfmoTWALtGtyw4HeRRWEZxKuSjmsDrhNrmZb0pQ91z\r\n" \
		"WKsa8KmFPdFJOGkdUyGmCejJQnKBAGZv+LcW377z9v6zeTX2Hw0+RYpjIzVWNw/2\r\n" \
		"/OII2cfiuu3BUxx1jM2PwA2iTw==\r\n" \
		"-----END CERTIFICATE-----\r\n";
#endif

#ifdef SSL_VERIFY_SERVER
static mbedtls_x509_crt *_ca_crt = NULL;

static const unsigned char *test_ca_cert = \
		"-----BEGIN CERTIFICATE-----\r\n" \
		"MIICxDCCAi2gAwIBAgIJAO1U1TMWGa37MA0GCSqGSIb3DQEBCwUAMHsxCzAJBgNV\r\n" \
		"BAYTAkNOMQswCQYDVQQIDAJKUzELMAkGA1UEBwwCU1oxEDAOBgNVBAoMB1JlYWxz\r\n" \
		"aWwxEDAOBgNVBAsMB1JlYWx0ZWsxEDAOBgNVBAMMB1JlYWxzaWwxHDAaBgkqhkiG\r\n" \
		"9w0BCQEWDWFAcmVhbHNpbC5jb20wHhcNMTYwOTE0MDU1OTE5WhcNMTcwOTE0MDU1\r\n" \
		"OTE5WjB7MQswCQYDVQQGEwJDTjELMAkGA1UECAwCSlMxCzAJBgNVBAcMAlNaMRAw\r\n" \
		"DgYDVQQKDAdSZWFsc2lsMRAwDgYDVQQLDAdSZWFsdGVrMRAwDgYDVQQDDAdSZWFs\r\n" \
		"c2lsMRwwGgYJKoZIhvcNAQkBFg1hQHJlYWxzaWwuY29tMIGfMA0GCSqGSIb3DQEB\r\n" \
		"AQUAA4GNADCBiQKBgQDK1uJDOX1TQylVnzKNCgpTDos2oHekx65LZSrg/fB8Rknb\r\n" \
		"Zi9DXwi9ELCtROHZu5NUOGLjwNN/hqaW09WJrRAd5KrvKIxJZTRsNfPlCBBQOlh8\r\n" \
		"6aZj9TE+H88x4fFdJPxzMR3hTqtm2/CD0WSEIvhS+4NK8/3qiXiGFF8WYD8HYwID\r\n" \
		"AQABo1AwTjAdBgNVHQ4EFgQUoNInV/jw1/wQ1NJCET1A4mtZ7UUwHwYDVR0jBBgw\r\n" \
		"FoAUoNInV/jw1/wQ1NJCET1A4mtZ7UUwDAYDVR0TBAUwAwEB/zANBgkqhkiG9w0B\r\n" \
		"AQsFAAOBgQBxWiMOKuH6i0BD2Gp8nHGgpup5vSu7TKTf1lBnIvKnwfXGTmTI1TjB\r\n" \
		"d3pXaPB4ehLg/1FNiFJ4mV8+s2fnQfTYuti3XlsBccL08WhprECwjNY3ZoeAXIi3\r\n" \
		"VGXR2fkZ4jhrf2GHmLTrtvbiatX1bN6EVvzbemHpxCYoGJdibDyCZg==\r\n" \
		"-----END CERTIFICATE-----\r\n";

static int my_verify(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags)
{
	char buf[1024];
	((void) data);

	printf("Verify requested for (Depth %d):\n", depth);
	mbedtls_x509_crt_info(buf, sizeof(buf) - 1, "", crt);
	printf("%s", buf);

	if (((*flags) & MBEDTLS_X509_BADCERT_EXPIRED) != 0) {
		printf("server certificate has expired\n");
	}

	if (((*flags) & MBEDTLS_X509_BADCERT_REVOKED) != 0) {
		printf("  ! server certificate has been revoked\n");
	}

	if (((*flags) & MBEDTLS_X509_BADCERT_CN_MISMATCH) != 0) {
		printf("  ! CN mismatch\n");
	}

	if (((*flags) & MBEDTLS_X509_BADCERT_NOT_TRUSTED) != 0) {
		printf("  ! self-signed or not signed by a trusted CA\n");
	}

	if (((*flags) & MBEDTLS_X509_BADCRL_NOT_TRUSTED) != 0) {
		printf("  ! CRL not trusted\n");
	}

	if (((*flags) & MBEDTLS_X509_BADCRL_EXPIRED) != 0) {
		printf("  ! CRL expired\n");
	}

	if (((*flags) & MBEDTLS_X509_BADCERT_OTHER) != 0) {
		printf("  ! other (unknown) flag\n");
	}

	if ((*flags) == 0) {
		printf("  Certificate verified without error flags\n");
	}

	return (0);
}
#endif

int ssl_client_ext_init(void)
{
#ifdef SSL_VERIFY_CLIENT
	_cli_crt = (mbedtls_x509_crt *) mbedtls_calloc(1, sizeof(mbedtls_x509_crt));

	if (_cli_crt) {
		mbedtls_x509_crt_init(_cli_crt);
	} else {
		return -1;
	}
#if !defined(configENABLE_TRUSTZONE) || (configENABLE_TRUSTZONE == 0) || !defined(CONFIG_SSL_CLIENT_PRIVATE_IN_TZ) || (CONFIG_SSL_CLIENT_PRIVATE_IN_TZ == 0)
	_clikey_rsa = (mbedtls_pk_context *) mbedtls_calloc(1, sizeof(mbedtls_pk_context));

	if (_clikey_rsa) {
		mbedtls_pk_init(_clikey_rsa);
	} else {
		return -1;
	}
#endif
#endif
#ifdef SSL_VERIFY_SERVER
	_ca_crt = (mbedtls_x509_crt *) mbedtls_calloc(1, sizeof(mbedtls_x509_crt));

	if (_ca_crt) {
		mbedtls_x509_crt_init(_ca_crt);
	} else {
		return -1;
	}
#endif
	return 0;
}

void ssl_client_ext_free(void)
{
#ifdef SSL_VERIFY_CLIENT
	if (_cli_crt) {
		mbedtls_x509_crt_free(_cli_crt);
		mbedtls_free(_cli_crt);
		_cli_crt = NULL;
	}

	if (_clikey_rsa) {
#if defined(configENABLE_TRUSTZONE) && (configENABLE_TRUSTZONE == 1) && defined(CONFIG_SSL_CLIENT_PRIVATE_IN_TZ) && (CONFIG_SSL_CLIENT_PRIVATE_IN_TZ == 1)
		extern void NS_ENTRY secure_mbedtls_pk_free(mbedtls_pk_context * pk);
		secure_mbedtls_pk_free(_clikey_rsa);
#else
		mbedtls_pk_free(_clikey_rsa);
		mbedtls_free(_clikey_rsa);
#endif
		_clikey_rsa = NULL;
	}
#endif
#ifdef SSL_VERIFY_SERVER
	if (_ca_crt) {
		mbedtls_x509_crt_free(_ca_crt);
		mbedtls_free(_ca_crt);
		_ca_crt = NULL;
	}
#endif
}

int ssl_client_ext_setup(mbedtls_ssl_config *conf)
{
	/* To avoid gcc warnings */
	(void) conf;

	/* Since mbedtls crt and pk API will check string terminator to prevent non-null-terminated string, need to count string terminator to buffer length */
#ifdef SSL_VERIFY_CLIENT
	if (mbedtls_x509_crt_parse(_cli_crt, test_client_cert, strlen((char const *)test_client_cert) + 1) != 0) {
		return -1;
	}

#if defined(configENABLE_TRUSTZONE) && (configENABLE_TRUSTZONE == 1) && defined(CONFIG_SSL_CLIENT_PRIVATE_IN_TZ) && (CONFIG_SSL_CLIENT_PRIVATE_IN_TZ == 1)
	extern mbedtls_pk_context *NS_ENTRY secure_mbedtls_pk_parse_key(void);
	_clikey_rsa = secure_mbedtls_pk_parse_key();
	if (_clikey_rsa == NULL) {
		return -1;
	}
#else
#if defined(MBEDTLS_VERSION_NUMBER) && (MBEDTLS_VERSION_NUMBER == 0x03000000)
	if (mbedtls_pk_parse_key(_clikey_rsa, test_client_key, strlen((char const *)test_client_key) + 1, NULL, 0, NULL, NULL) != 0) {
#else
	if (mbedtls_pk_parse_key(_clikey_rsa, test_client_key, strlen((char const *)test_client_key) + 1, NULL, 0) != 0) {
#endif
		return -1;
	}
#endif

	mbedtls_ssl_conf_own_cert(conf, _cli_crt, _clikey_rsa);
#endif
#ifdef SSL_VERIFY_SERVER
	if (mbedtls_x509_crt_parse(_ca_crt, test_ca_cert, strlen((char const *)test_ca_cert) + 1) != 0) {
		return -1;
	}

	mbedtls_ssl_conf_ca_chain(conf, _ca_crt, NULL);
	mbedtls_ssl_conf_authmode(conf, MBEDTLS_SSL_VERIFY_REQUIRED);
	mbedtls_ssl_conf_verify(conf, my_verify, NULL);
#endif
	return 0;
}
