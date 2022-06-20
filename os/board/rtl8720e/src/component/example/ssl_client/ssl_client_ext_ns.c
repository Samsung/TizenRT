#include "FreeRTOS.h"
#include "platform_opts.h"

#include "mbedtls/config.h"
#include "mbedtls/platform.h"
#include "mbedtls/ssl.h"

#define SSL_VERIFY_CLIENT
#define SSL_VERIFY_SERVER

#ifdef SSL_VERIFY_CLIENT
static mbedtls_x509_crt *_cli_crt = NULL;
static mbedtls_pk_context *_clikey_rsa = NULL;
#endif

#ifdef SSL_VERIFY_SERVER
static mbedtls_x509_crt *_ca_crt = NULL;
#endif

extern mbedtls_pk_context *NS_ENTRY secure_mbedtls_pk_parse_key(void);
extern void NS_ENTRY secure_mbedtls_pk_free(mbedtls_pk_context *pk);
extern mbedtls_x509_crt *NS_ENTRY secure_mbedtls_x509_crt_parse(void);
extern mbedtls_x509_crt *NS_ENTRY secure_mbedtls_x509_crt_parse_ca(void);
extern void NS_ENTRY secure_mbedtls_x509_crt_free(mbedtls_x509_crt *crt);
extern int NS_ENTRY secure_mbedtls_ssl_conf_own_cert(mbedtls_ssl_config *conf, mbedtls_x509_crt *own_cert, mbedtls_pk_context *pk_key);
extern void NS_ENTRY secure_mbedtls_ssl_conf_verify(mbedtls_ssl_config *conf, void *p_vrfy);

void ssl_client_ext_free(void)
{
#ifdef SSL_VERIFY_CLIENT
	if (_cli_crt) {
		secure_mbedtls_x509_crt_free(_cli_crt);
		_cli_crt = NULL;
	}

	if (_clikey_rsa) {
		secure_mbedtls_pk_free(_clikey_rsa);
		_clikey_rsa = NULL;
	}
#endif
#ifdef SSL_VERIFY_SERVER
	if (_ca_crt) {
		secure_mbedtls_x509_crt_free(_ca_crt);
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
	_cli_crt = secure_mbedtls_x509_crt_parse();
	if (_cli_crt == NULL) {
		return -1;
	}

	_clikey_rsa = secure_mbedtls_pk_parse_key();
	if (_clikey_rsa == NULL) {
		return -1;
	}

	secure_mbedtls_ssl_conf_own_cert(conf, _cli_crt, _clikey_rsa);
#endif
#ifdef SSL_VERIFY_SERVER
	_ca_crt = secure_mbedtls_x509_crt_parse_ca();
	if (_ca_crt == NULL) {
		return -1;
	}

	mbedtls_ssl_conf_ca_chain(conf, _ca_crt, NULL);
	mbedtls_ssl_conf_authmode(conf, MBEDTLS_SSL_VERIFY_REQUIRED);
	secure_mbedtls_ssl_conf_verify(conf, NULL);
#endif
	return 0;
}

