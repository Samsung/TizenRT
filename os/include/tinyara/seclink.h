#ifndef __SECLINK_H__
#define __SECLINK_H__

#include <stdint.h>
#include <sys/ioctl.h>
#include <tinyara/security_hal.h>

#define SECLINK_OK                               0
#define SECLINK_ERROR                            -1

#define SECLINK_MEM_MAX_SIZE                     4096
#define SECLINK_MEM_PRIV_MAX_SIZE                256

/*  common */
#define SECLINKIOC_COMMON                       _SECLINKIOC(0x00)
#define SECLINKIOC_INIT                         _SECLINKIOC((SECLINKIOC_COMMON | 0x00))
#define SECLINKIOC_DEINIT                       _SECLINKIOC((SECLINKIOC_COMMON | 0x01))

/*  Crypto */
#define SECLINKIOC_CRYPTO                       _SECLINKIOC(0x10)
#define SECLINKIOC_AESENCRYPT                   _SECLINKIOC((SECLINKIOC_CRYPTO | 0x00))
#define SECLINKIOC_AESDECRYPT                   _SECLINKIOC((SECLINKIOC_CRYPTO | 0x01))
#define SECLINKIOC_RSAENCRYPT                   _SECLINKIOC((SECLINKIOC_CRYPTO | 0x02))
#define SECLINKIOC_RSADECRYPT                   _SECLINKIOC((SECLINKIOC_CRYPTO | 0x03))

/*  Authenticate */
#define SECLINKIOC_AUTH                         _SECLINKIOC(0x20)
#define SECLINKIOC_GENERATERANDOM               _SECLINKIOC((SECLINKIOC_AUTH | 0x00))
#define SECLINKIOC_GETHASH                      _SECLINKIOC((SECLINKIOC_AUTH | 0x01))
#define SECLINKIOC_GETHMAC                      _SECLINKIOC((SECLINKIOC_AUTH | 0x02))
#define SECLINKIOC_RSASIGNMD                    _SECLINKIOC((SECLINKIOC_AUTH | 0x03))
#define SECLINKIOC_RSAVERIFYMD                  _SECLINKIOC((SECLINKIOC_AUTH | 0x04))
#define SECLINKIOC_ECDSASIGNMD                  _SECLINKIOC((SECLINKIOC_AUTH | 0x05))
#define SECLINKIOC_ECDSAVERIFYMD                _SECLINKIOC((SECLINKIOC_AUTH | 0x06))
#define SECLINKIOC_DHGENERATEPARAM              _SECLINKIOC((SECLINKIOC_AUTH | 0x07))
#define SECLINKIOC_DHCOMPUTESHAREDSECRET        _SECLINKIOC((SECLINKIOC_AUTH | 0x08))
#define SECLINKIOC_ECDHCOMPUTESHAREDSECRET      _SECLINKIOC((SECLINKIOC_AUTH | 0x09))
#define SECLINKIOC_SETCERTIFICATE               _SECLINKIOC((SECLINKIOC_AUTH | 0x0a))
#define SECLINKIOC_GETCERTIFICATE               _SECLINKIOC((SECLINKIOC_AUTH | 0x0b))
#define SECLINKIOC_REMOVECERTIFICATE            _SECLINKIOC((SECLINKIOC_AUTH | 0x0c))
#define SECLINKIOC_GETFACTORY_KEY               _SECLINKIOC((SECLINKIOC_AUTH | 0x0d))
#define SECLINKIOC_GETFACTORY_CERT              _SECLINKIOC((SECLINKIOC_AUTH | 0x0e))
#define SECLINKIOC_GETFACTORY_DATA              _SECLINKIOC((SECLINKIOC_AUTH | 0x0f))

/*  Secure Storage */
#define SECLINKIOC_SS                           _SECLINKIOC(0x40)
#define SECLINKIOC_WRITESTORAGE                 _SECLINKIOC((SECLINKIOC_SS | 0x00))
#define SECLINKIOC_READSTORAGE                  _SECLINKIOC((SECLINKIOC_SS | 0x01))
#define SECLINKIOC_DELETESTORAGE                _SECLINKIOC((SECLINKIOC_SS | 0x02))

/*  Key manager */
#define SECLINKIOC_KEYMGR                       _SECLINKIOC(0x0080)
#define SECLINKIOC_SETKEY                       _SECLINKIOC((SECLINKIOC_KEYMGR | 0x0000))
#define SECLINKIOC_GETKEY                       _SECLINKIOC((SECLINKIOC_KEYMGR | 0x0001))
#define SECLINKIOC_REMOVEKEY                    _SECLINKIOC((SECLINKIOC_KEYMGR | 0x0002))
#define SECLINKIOC_GENERATEKEY                  _SECLINKIOC((SECLINKIOC_KEYMGR | 0x0003))

/*
#define SECLINK_HAL_INIT                         0x0001
#define SECLINK_HAL_DEINIT                       0x0002

#define SECLINK_KEY                              0x0010
#define SECLINK_HAL_SETKEY                       0x0011
#define SECLINK_HAL_GETKEY                       0x0012
#define SECLINK_HAL_REMOVEKEY                    0x0013
#define SECLINK_HAL_GENERATEKEY                  0x0014

#define SECLINK_AUTH                             0x0020
#define SECLINK_HAL_GENERATERANDOM               0x0021
#define SECLINK_HAL_GETHASH                      0x0022
#define SECLINK_HAL_GETHMAC                      0x0023
#define SECLINK_HAL_RSASIGNMD                    0x0024
#define SECLINK_HAL_RSAVERIFYMD                  0x0025
#define SECLINK_HAL_ECDSASIGNMD                  0x0026
#define SECLINK_HAL_ECDSAVERIFYMD                0x0027
#define SECLINK_HAL_DHGENERATEPARAM              0x0028
#define SECLINK_HAL_DHCOMPUTESHAREDSECRET        0x0029
#define SECLINK_HAL_ECDHCOMPUTESHAREDSECRET      0x002a
#define SECLINK_HAL_SETCERTIFICATE               0x002b
#define SECLINK_HAL_GETCERTIFICATE               0x002c
#define SECLINK_HAL_REMOVECERTIFICATE            0x002d
#define SECLINK_HAL_GETFACTORY_KEY               0x002e
#define SECLINK_HAL_GETFACTORY_CERT              0x0030
#define SECLINK_HAL_GETFACTORY_DATA              0x0031

#define SECLINK_CRYPTO                           0x0040
#define SECLINK_HAL_AESENCRYPT                   0x0041
#define SECLINK_HAL_AESDECRYPT                   0x0042
#define SECLINK_HAL_RSAENCRYPT                   0x0043
#define SECLINK_HAL_RSADECRYPT                   0x0044

#define SECLINK_SS                               0x0080
#define SECLINK_HAL_WRITESTORAGE                 0x0081
#define SECLINK_HAL_READSTORAGE                  0x0082
#define SECLINK_HAL_DELETESTORAGE                0x0083
*/

struct _seclink_s_;
typedef struct _seclink_s_ *sl_ctx;

typedef struct _seclink_mempool_s {
	void *mem;
	void *mem_priv;
} seclink_mem;

struct seclink_init_param {
	uint32_t i2c_port;
	uint32_t gpio;
};

struct seclink_key_info {
	hal_key_type mode;
	uint32_t key_idx;
	hal_data *key;
	hal_data *prikey;
};

struct seclink_auth_info {
	union {
		hal_hash_type hash_type;
		hal_hmac_type hmac_type;
		hal_rsa_mode rsa_type;
		hal_ecdsa_mode ecdsa_type;
		uint32_t random_len;
	} auth_type;
	uint32_t key_idx;
	hal_data *data;
	union {
		hal_data *data;
		hal_dh_data *dh_param;
		hal_ecdh_data *ecdh_param
	} auth_data;
};

struct seclink_crypto_info {
	uint32_t key_idx;
	hal_data *input;
	hal_data *output;
	hal_aes_param *aes_param;
	hal_rsa_mode *rsa_mode;
};

struct seclink_ss_info {
	uint32_t key_idx;
	hal_data *data;
};

struct seclink_comm_info {
	uint8_t *priv;
};
struct seclink_req {
	union {
		struct seclink_key_info *key;
		struct seclink_auth_info *auth;
		struct seclink_crypto_info *crypto;
		struct seclink_ss_info *ss;
		struct seclink_comm_info *comm;
	} req_type;
	struct seclink_init_param *params;
	int32_t res;
};

/*  Common */
int sl_init(sl_ctx *hnd);
void sl_deinit(sl_ctx hnd);

/*  key manager */
int sl_set_key(sl_ctx hnd, hal_key_type mode, uint32_t key_idx, hal_data *key, hal_data *prikey);

int sl_get_key(sl_ctx hnd, hal_key_type mode, uint32_t key_idx, _OUT_ hal_data *key);

int sl_remove_key(sl_ctx hnd, hal_key_type mode, uint32_t key_idx);

int sl_generate_key(sl_ctx hnd, hal_key_type mode, uint32_t key_idx);


/*  Authenticate */
int sl_generate_random(sl_ctx hnd, uint32_t len, _OUT_ hal_data *random);

int sl_get_hash(sl_ctx hnd, hal_hash_type mode, hal_data *input, _OUT_ hal_data *hash);

int sl_get_hmac(sl_ctx hnd, hal_hmac_type mode, hal_data *input, uint32_t key_idx, _OUT_ hal_data *hmac);

int sl_rsa_sign_md(sl_ctx hnd, hal_rsa_mode mode, hal_data *hash, uint32_t key_idx, _OUT_ hal_data *sign);

int sl_rsa_verify_md(sl_ctx hnd, hal_rsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx);

int sl_ecdsa_sign_md(sl_ctx hnd, hal_ecdsa_mode mode, hal_data *hash, uint32_t key_idx, _OUT_ hal_data *sign);

int sl_ecdsa_verify_md(sl_ctx hnd, hal_ecdsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx);

int sl_dh_generate_param(sl_ctx hnd, uint32_t dh_idx, _INOUT_ hal_dh_data *dh_param);

int sl_dh_compute_shared_secret(sl_ctx hnd, hal_dh_data *dh_param, uint32_t dh_idx, _OUT_ hal_data *shared_secret);

int sl_ecdh_compute_shared_secret(sl_ctx hnd, hal_ecdh_data *ecdh_mode, uint32_t key_idx, _OUT_ hal_data *shared_secret);

int sl_set_certificate(sl_ctx hnd, uint32_t cert_idx, hal_data *cert_in);

int sl_get_certificate(sl_ctx hnd, uint32_t cert_idx, _OUT_ hal_data *cert_out);

int sl_remove_certificate(sl_ctx hnd, uint32_t cert_idx);

int sl_get_factory_key(sl_ctx hnd, uint32_t key_idx, hal_data *key);

int sl_get_factory_cert(sl_ctx hnd, uint32_t cert_idx, hal_data *cert);

int sl_get_factory_data(sl_ctx hnd, uint32_t data_idx, hal_data *data);


/*  Crypto */
int sl_aes_encrypt(sl_ctx hnd, hal_data *dec_data, hal_aes_param *aes_param, uint32_t key_idx, _OUT_ hal_data *enc_data);

int sl_aes_decrypt(sl_ctx hnd, hal_data *enc_data, hal_aes_param *aes_param, uint32_t key_idx, _OUT_ hal_data *dec_data);

int sl_rsa_encrypt(sl_ctx hnd, hal_data *dec_data, hal_rsa_mode *rsa_mode, uint32_t key_idx, _OUT_ hal_data *enc_data);

int sl_rsa_decrypt(sl_ctx hnd, hal_data *enc_data, hal_rsa_mode *rsa_mode, uint32_t key_idx, _OUT_ hal_data *dec_data);


/*  Secure Storage */
int sl_write_storage(sl_ctx hnd, uint32_t ss_idx, hal_data *data);

int sl_read_storage(sl_ctx hnd, uint32_t ss_idx, _OUT_ hal_data *data);

int sl_delete_storage(sl_ctx hnd, uint32_t ss_idx);

#endif // __SECLINK_H__
