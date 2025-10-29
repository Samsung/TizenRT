#include <stddef.h>
#include <string.h>
#include "crypto_hw.h"
#include "psa/crypto_types.h"
#include "tfm_plat_crypto_keys.h"
#include "tfm_plat_otp.h"
#include "mbedtls/hkdf.h"

#define DUBHE_BASE_S   (0x4b110000)

extern int dubhe_driver_init( unsigned long dbh_base_addr );
extern void dubhe_driver_cleanup( void );

int crypto_hw_accelerator_init(void)
{
	return dubhe_driver_init(DUBHE_BASE_S);
}

int crypto_hw_accelerator_finish(void)
{
	dubhe_driver_cleanup();

	return 0;
}

int crypto_hw_accelerator_huk_derive_key(const uint8_t *label,
                                         size_t label_size,
                                         const uint8_t *context,
                                         size_t context_size,
                                         uint8_t *key,
                                         size_t key_size)
{
	uint8_t huk_buf[32];
	int err = -1;
	int mbedtls_err;

	if (key == NULL) {
		return -1;
	}

	if (label == NULL && label_size != 0) {
		return -1;
	}

	if (context == NULL && context_size != 0) {
		return -1;
	}

	err = tfm_plat_otp_read(PLAT_OTP_ID_HUK, sizeof(huk_buf), huk_buf);
	if (err != TFM_PLAT_ERR_SUCCESS) {
		goto out;
	}

	mbedtls_err = mbedtls_hkdf(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256),
							   label, label_size, huk_buf, sizeof(huk_buf),
							   context, context_size, key, key_size);
	if (mbedtls_err) {
		err = -1;
		goto out;
	}

out:
	memset(huk_buf, 0, sizeof(huk_buf));

	return err;
}

int crypto_hw_apply_debug_permissions(uint8_t *permissions_mask, uint32_t len)
{
	return 0;
}

