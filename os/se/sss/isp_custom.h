/*!
 *  @file       isp_custom.h
 *  @brief      Headerfile : util functions to support memset, memcpy, memcmp
 *  @author     joohwani.kim
 *  @version    v0.1 : 2017.8.24 Init. release version
 */
#ifndef ISP_CUSTOM_H_
#define ISP_CUSTOM_H_

#include "isp_oid.h"
#include "isp_define.h"

// Secure Storage
#define SECURE_STORAGE_TYPE_FACTORYKEY_KEY			(0x00)
#define SECURE_STORAGE_TYPE_FACTORYKEY_DATA			(0x01)

#define SECURE_STORAGE_TYPE_KEY_AES				(0x0A)
#define SECURE_STORAGE_TYPE_KEY_HMAC			(0x0B)
#define SECURE_STORAGE_TYPE_KEY_RSA				(0x0C)
#define SECURE_STORAGE_TYPE_KEY_DH				(0x0D)
#define SECURE_STORAGE_TYPE_KEY_ECC				(0x0E)
#define SECURE_STORAGE_TYPE_DATA				(0x10)
#define SECURE_STORAGE_TYPE_CERT				(0x11)

#define FACTORYKEY_ARTIK_PSK					(0x80000100)
#define FACTORYKEY_ARTIK_DEVICE					(0x00010120)
#define FACTORYKEY_ARTIK_CERT					(0x00010122)

// AES Mode
#define AES_ECB_MODE		(0x0008)
#define AES_CBC_MODE		(0x0108)
#define AES_CTR_MODE		(0x0208)
#define AES_XTS_MODE		(0x0308)
#define AES_CCM_MODE		(0x1008)
#define AES_GCM_MODE		(0x1108)
#define AES_KW_MODE		(0x1208)

#define SSTORAGE_DATA_SLOT_INDEX_MAX		(32)

// ======================================
// Structure
// ======================================
//! @struct sECC_KEY
//! @brief struct of ECDSA Key parameter
struct sECC_KEY {
	unsigned int curve;
	unsigned char *privatekey;
	unsigned char *publickey_x;
	unsigned char x_byte_len;
	unsigned char *publickey_y;
	unsigned char y_byte_len;
};

//! @struct sECC_SIGN
//! @brief struct of ECDSA Signature
struct sECC_SIGN {
	unsigned int sign_type;
	unsigned char *r;
	unsigned char r_byte_len;
	unsigned char *s;
	unsigned char s_byte_len;
};

//! @struct sRSA_KEY
//! @brief struct of RSA Key parameter
struct sRSA_KEY {
	unsigned int object_id;
	unsigned char *modules;
	unsigned int modules_byte_len;
	unsigned char *publickey;
	unsigned int publickey_byte_len;
	unsigned char *privatekey;
};

//! @struct sRSA_SIGN
//! @brief struct of RSA Signature
struct sRSA_SIGN {
	unsigned int signature_byte_len;
	unsigned char *signature;
	unsigned int salt_byte_len;
	unsigned int alg_type;
};

//! @struct sAES_PARAM
//! @brief struct of AES parameter
struct sAES_PARAM {
	unsigned char *pu8Plaintext;
	unsigned int u32Plaintext_byte_len;
	unsigned char *pu8Ciphertext;
	unsigned int u32Ciphertext_byte_len;

	unsigned char *pu8iv;
	unsigned int u32iv_byte_len;

	unsigned char *pu8aad;
	unsigned int u32aad_byte_len;

	unsigned char *pu8Tag;
	unsigned int u32Tag_byte_len;

	unsigned int u32Mode;
};

//! @struct sHMAC_MSG
//! @brief struct of message for HMAC
struct sHMAC_MSG {
	unsigned int addr_low;
	unsigned int addr_high;
	unsigned int descriptor_byte_len;
	unsigned int msg_byte_len;
	unsigned int msg_type;
};

//! @struct sHASH_MSG
//! @brief struct of message for Hash
struct sHASH_MSG {
	unsigned int addr_low;
	unsigned int addr_high;
	unsigned int descriptor_byte_len;
	unsigned int msg_byte_len;
	unsigned int msg_type;
};

//! @struct sDH_PARAM
//! @brief struct of DH parameter
struct sDH_PARAM {
	unsigned int object_id;
	unsigned int modules_p_byte_len;
	unsigned char *modules_p;
	unsigned int generator_g_byte_len;
	unsigned char *generator_g;
	unsigned int order_q_byte_len;
	unsigned char *order_q;
	unsigned int publickey_byte_len;
	unsigned char *publickey;
};


// ======================================
// Function
// ======================================

/*! @fn         isp_get_status()
 *	@ingroup	SECURITY_ISP
 *	@brief		status function
 *  @version    v0.50 : 2016.8.13 Init. release version
 *	@retval		SUCCESS
 *	@retval		ERROR_SYSTEM_MAILBOX_BUSY
 */
int isp_get_status(void);


/*! @fn         isp_clear(unsigned int type)
 *	@ingroup	SECURITY_ISP
 *	@brief		clear function
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @param		type                    clear type
 *	@retval		SUCCESS
 *	@retval		Others(!=0)	fail - error from sub-function
 */
int isp_clear(unsigned int type);

/*! @fn         isp_read_cert(unsigned char *data, unsigned int *data_byte_len, unsigned int index)
 *	@ingroup    SECURITY_ISP
 *	@brief      read cert function for secure storage
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.51 : 2016.12.29 Use Pre-Defined Variable for the readability
 *  @param[out] data             : array of data
 *  @param[out] data_byte_len    : length of data
 *  @param[in]  index            : slot index of secure storage
 *	@retval		SUCCESS		: Success
 *	@retval		Others(!=0)	: fail - error from sub-function
 */
int isp_read_cert(unsigned char *data, unsigned int *data_byte_len, unsigned int index);

/*! @fn         isp_set_factorykey_data(unsigned char *data, unsigned int data_byte_len, unsigned int data_id)
 *	@ingroup    SECURITY_ISP
 *	@brief      write data function for secure storage
 *  @version    v0.50 : 2016.12.29 Init. release version
 *  @param[in]  data             : array of data
 *  @param[in]  data_byte_len    : length of data
 *  @param[in]  data_id          : id of data
 *	@retval		SUCCESS		: Success
 *	@retval		Others(!=0)	: fail - error from sub-function
 */
int isp_get_factorykey_data(unsigned char *data, unsigned int *data_byte_len, unsigned int data_id);

/*! @fn			isp_generate_random(unsigned int *random, unsigned int wlen)
 *	@ingroup	SECURITY_ISP
 *	@brief		rng function
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @param[out] random             : array of random number
 *  @param[in]  wlen               : word length of random number to be generated
 *	@retval		SUCCESS            : Success
 *	@retval		Others(!=0)        : fail - error from sub-function
 */
int isp_generate_random(unsigned int *random, unsigned int wlen);

/*! @fn         isp_ecdsa_sign_md_securekey(struct sECC_SIGN * ecc_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index)
 *	@ingroup    SECURITY_ISP
 *	@brief      ecdsa function for generating signature
 *  @version    v0.10 : 2016.12.23 Init. release version
 *  @param[out] ecc_sign     : struct of signature
 *  @param[in]  msg_digest           : array of hashed_message
 *  @param[in]  msg_digest_byte_len  : length of hashed_message with byte unit
 *  @param[in]  key_index     : slot # in secure storage
 *	@retval     SUCCESS       : Success
 *	@retval     Others(!=0)   : fail - error from sub-function
 */
int isp_ecdsa_sign_md_securekey(struct sECC_SIGN *ecc_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index);

/*!
 *	@ingroup    SECURITY_ISP
 *	@brief      ecdh function for computing shared secret using secure key storage
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.60 : 2016.12.27 Change factorykey id parameter, Set Null input for object_id
 *  @param[out] shared_secret : array of shared secret
 *  @param[out] shared_secret_byte_len : length of shared_secret_byte_len
 *  @param[in]  ecc_publickey : struct of ecc key
 *  @param[in]  key_index     : slot # in secure storage
 *	@retval     SUCCESS       : Success
 *	@retval     Others(!=0)   : fail - error from sub-function
 */
int isp_compute_ecdh_securekey(unsigned char *shared_secret, unsigned int *shared_secret_byte_len, struct sECC_KEY ecc_publickey, unsigned int key_index);

/*! @fn         isp_write_cert(unsigned char *data, unsigned int data_byte_len, unsigned int index)
 *	@ingroup    SECURITY_ISP
 *	@brief      write cert function for secure storage
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.51 : 2016.12.29 Use Pre-Defined Variable for the readability
 *  @param[in]  data             : array of data
 *  @param[in]  data_byte_len    : length of data
 *  @param[in]  index            : slot index of secure storage
 *	@retval		SUCCESS		: Success
 *	@retval		Others(!=0)	: fail - error from sub-function
 */
int isp_write_storage(unsigned char *data, unsigned int data_byte_len, unsigned int index);

/*! @fn         isp_read_cert(unsigned char *data, unsigned int *data_byte_len, unsigned int index)
 *	@ingroup    SECURITY_ISP
 *	@brief      read cert function for secure storage
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.51 : 2016.12.29 Use Pre-Defined Variable for the readability
 *  @param[out] data             : array of data
 *  @param[out] data_byte_len    : length of data
 *  @param[in]  index            : slot index of secure storage
 *	@retval		SUCCESS		: Success
 *	@retval		Others(!=0)	: fail - error from sub-function
 */
int isp_read_storage(unsigned char *data, unsigned int *data_byte_len, unsigned int index);

/*! @fn         int isp_get_factorykey_data(unsigned char *data, unsigned int *data_byte_len, unsigned int data_id)
 *	@ingroup    SECURITY_ISP
 *	@brief      read data function for secure storage
 *  @version    v0.50 : 2016.12.29 Init. release version
 *  @param[out] data             : array of data
 *  @param[out] data_byte_len    : length of data
 *  @param[in]  data_id          : id of data
 *	@retval		SUCCESS		: Success
 *	@retval		Others(!=0)	: fail - error from sub-function
 */
int isp_get_factorykey_data(unsigned char *data, unsigned int *data_byte_len, unsigned int data_id);

/*! @fn         isp_ecdsa_get_publickey_securekey(struct sECC_KEY *ecc_publickey, unsigned int key_index, unsigned int object_id)
 *	@ingroup    SECURITY_ISP
 *	@brief      ecc function for get publickey to secure storage
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.60 : 2016.12.23 Change factorykey id parameter, Set Null input for object_id
 *  @param[out] ecc_publickey : public key from slot # in secure storage
 *  @param[in]  key_index     : slot # in secure storage
 *  @param[in]  object_id     : selection of algorithm
 *	@retval     SUCCESS       : Success
 *	@retval     Others(!=0)   : fail - error from sub-function
 */
int isp_ecdsa_get_publickey_securekey(struct sECC_KEY *ecc_publickey, unsigned int key_index, unsigned int object_id);

/*! @fn         isp_aes_generate_key_securekey(unsigned int key_byte_len, unsigned int key_index)
 *	@ingroup	SECURITY_ISP
 *	@brief		aes key generation function to store in secure storage
 *  @version	v0.10 : 2016.7.29
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @param[in]  key_byte_len  : byte length of key
 *  @param[in]  key_index : slot index of secure storage
 *	@retval		SUCCESS		: Success
 *	@retval		Others(!=0)	: fail - error from sub-function
 */
int isp_aes_generate_key_securekey(unsigned int key_byte_len, unsigned int key_index);

/*! @fn         isp_hmac_generate_key_securekey(unsigned int key_byte_len, unsigned int key_index)
 *	@ingroup	SECURITY_ISP
 *	@brief		hmac key generation function to store in secure storage
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @param[in]  key_byte_len  : byte length of key
 *  @param[in]  key_index : slot index of secure storage
 *	@retval		SUCCESS		: Success
 *	@retval		Others(!=0)	: fail - error from sub-function
 */
int isp_hmac_generate_key_securekey(unsigned int key_byte_len, unsigned int key_index);

/*! @fn         isp_rsa_generate_key_securekey(unsigned key_index, unsigned object_id, unsigned int pukey_e)
 *	@ingroup    SECURITY_ISP
 *	@brief      rsa function for verfication of signature
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @param[in]  key_index          : secure storage index
 *  @param[in]  object_id          : algorithm selection
 *  @param[in]  pukey_e            : input pukey under 32bits-length (if 0, pukey will be randomly generated)
 *	@retval     SUCCESS            : Success
 *	@retval     Others(!=0)        : fail - error from sub-function
 */
int isp_rsa_generate_key_securekey(unsigned key_index, unsigned object_id, unsigned int pukey_e);

/*! @fn         isp_ecdsa_generate_key_securekey(unsigned int key_index, unsigned int object_id)
 *	@ingroup    SECURITY_ISP
 *	@brief      ecdsa function for generating key to secure storage
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @param[in]  key_index     : slot # in secure storage
 *  @param[in]  object_id     : selection of algorithm
 *	@retval     SUCCESS       : Success
 *	@retval     Others(!=0)   : fail - error from sub-function
 */
int isp_ecdsa_generate_key_securekey(unsigned int key_index, unsigned int object_id);

/*! @fn         isp_set_securekey( unsigned char *data, unsigned int data_byte_len, unsigned int key_type, unsigned int index)
 *	@ingroup    SECURITY_ISP
 *	@brief      set securekey function
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.51 : 2016.12.29 Use Pre-Defined Variable for the readability
 *  @param[in]  data                 : array of data
 *  @param[in]  data_byte_len        : length of data
 *  @param[in]  key_type             : type of key
 *  @param[in]  index                : slot index of secure storage
 *	@retval		SUCCESS              : Success
 *	@retval		Others(!=0)          : fail - error from sub-function
 */
int isp_set_securekey(unsigned char *data, unsigned int data_byte_len, unsigned int key_type, unsigned int index);

/*! @fn         isp_remove_key(unsigned int key_type, unsigned int key_index)
 *	@ingroup    SECURITY_ISP
 *	@brief      delete securekey function
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.60 : 2016.12.29 Use Pre-Defined Variable for the readability, Secure Storage Map
 *  @param[in]  key_type : type of key
 *  @param[in]  key_index: slot index of secure storage
 *	@retval		SUCCESS		: Success
 *	@retval		Others(!=0)	: fail - error from sub-function
 */
int isp_remove_key(unsigned int key_type, unsigned int index);

/*! @fn         isp_aes_encrypt_securekey(struct sAES_PARAM * aes_param, unsigned int key_index)
 *	@ingroup    SECURITY_ISP
 *	@brief      aes encryption function using securekey
 *  @version    v0.10 : 2016.7.29
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.51 : 2016.12.29 Use Pre-Defined Variable for the readability
 *  @param[in]  aes_param               : struct of aes parameters
 *  @param[in]  key_index               : slot index of secure storage
 *	@retval		SUCCESS		: Success
 *	@retval		Others(!=0)	: fail - error from sub-function
 */
int isp_aes_encrypt_securekey(struct sAES_PARAM *aes_param, unsigned int key_index);

/*! @fn         isp_aes_decrypt_securekey(struct sAES_PARAM * aes_param, unsigned int key_index)
 *	@ingroup    SECURITY_ISP
 *	@brief      aes decryption function using securekey
 *  @version    v0.10 : 2016.7.29
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.51 : 2016.12.29 Use Pre-Defined Variable for the readability
 *  @param[in]  aes_param               : struct of aes parameters
 *  @param[in]  key_index               : slot index of secure storage
 *	@retval		SUCCESS		: Success
 *	@retval		Others(!=0)	: fail - error from sub-function
 */
int isp_aes_decrypt_securekey(struct sAES_PARAM *aes_param, unsigned int key_index);

/*! @fn			isp_hmac_securekey(unsigned char * mac, struct sHMAC_MSG * hmac_msg, unsigned int object_id, unsigned int key_index)
 *	@ingroup	SECURITY_ISP
 *	@brief		hmac function for key stored in secure storage slot
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.60 : 2016.12.27 Set Null input for object_id
 *  @param[out] mac                 : mac output
 *  @param[in]  hmac_msg            : struct sHMAC_MSG
 *  @param[in]  object_id           : algorithm selection
 *  @param[in]  key_index           : key index
 *	@retval		SUCCESS             : Success
 *	@retval		Others(!=0)         : fail - error from sub-function
 */
int isp_hmac_securekey(unsigned char *mac, struct sHMAC_MSG *hmac_msg, unsigned int object_id, unsigned int key_index);

/*! @fn         isp_hash(unsigned char * hash, struct sHASH_MSG * hash_msg, unsigned int object_id)
 *	@ingroup	SECURITY_ISP
 *	@brief		hash function
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @param[out] hash          : hash output
 *  @param[in]  hash_msg      : struct of message
 *  @param[in]  object_id     : algorithm selection
 *	@retval		SUCCESS       : Success
 *	@retval		Others(!=0)   : fail - error from sub-function
 */
int isp_hash(unsigned char *hash, struct sHASH_MSG *hash_msg, unsigned int object_id);

/*! @fn		isp_dh_generate_keypair_userparam_securestorage(struct sDH_PARAM *i_dh_param, unsigned int dh_param_index)
 *	@ingroup    SECURITY_ISP
 *	@brief      dh function for generate pvkey & pukey pair from user input parameter p & g
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @param[out] i_dh_param        : struct of DH param = {p, g, pukey}
 *  @param[in]  dh_param_index: slot # in secure storage for p, g, pvkey
 *	@retval     SUCCESS       : Success
 *	@retval     Others(!=0)   : fail - error from sub-function
 */
int isp_dh_generate_keypair_userparam_securestorage(struct sDH_PARAM *i_dh_param, unsigned int dh_param_index);

/*! @fn         isp_dh_compute_shared_secret_securekey(unsigned char *shared_secret, struct sDH_KEY* dh_publickey, unsigned int key_index)
 *	@ingroup    SECURITY_ISP
 *	@brief      dh function for compute shared secret
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.60 : 2016.12.27 Set Null input for object_id
 *  @param[out] shared_secret : array of shared secret
 *  @param[in]  dh_publickey  : struct of DH publickey
 *  @param[in]  key_index     : slot # in secure storage
 *	@retval     SUCCESS       : Success
 *	@retval     Others(!=0)   : fail - error from sub-function
 */
int isp_dh_compute_shared_secret_securekey(unsigned char *shared_secret, unsigned int *shared_secret_byte_len, struct sDH_PARAM dh_publickey, unsigned int key_index);

/*! @fn         isp_rsa_decrypt_securekey(unsigned char *output, unsigned int *output_byte_len, unsigned char *input, unsigned int input_byte_len, unsigned int key_index)
 *	@ingroup    SECURITY_ISP
 *	@brief      rsa function for decryption using securekey
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.60 : 2016.12.27 Change factorykey id parameter, Set Null input for object_id
 *  @param[out] output            : array of output message
 *  @param[out] output_byte_len   : length of output message
 *  @param[in]  input             : array of input message
 *  @param[in]  input_byte_len    : length of input message
 *  @param[in]  key_index         : slot # in secure storage
 *	@retval     SUCCESS           : Success
 *	@retval     Others(!=0)       : fail - error from sub-function
 */
int isp_rsa_decrypt_securekey(unsigned char *output, unsigned int *output_byte_len, unsigned char *input, unsigned int input_byte_len, unsigned int key_index);

/*! @fn         isp_rsa_encrypt_securekey(unsigned char *output, unsigned char *input, unsigned int input_byte_len, unsigned int key_index)
 *	@ingroup    SECURITY_ISP
 *	@brief      rsa function for encryption using securekey
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.60 : 2016.12.27 Change factorykey id parameter, Set Null input for object_id
 *  @param[out] output            : array of output message
 *  @param[in]  input             : array of input message
 *  @param[in]  input_byte_len    : length of input message
 *  @param[in]  key_index         : slot # in secure storage
 *	@retval     SUCCESS           : Success
 *	@retval     Others(!=0)       : fail - error from sub-function
 */
int isp_rsa_encrypt_securekey(unsigned char *output, unsigned int *output_byte_len, unsigned char *input, unsigned int input_byte_len, unsigned int key_index);

/*! @fn         isp_rsa_sign_md_securekey(struct sRSA_SIGN *rsa_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index)
 *	@ingroup    SECURITY_ISP
 *	@brief      rsa function for generating signature using securekey
 *  @version    v0.10 : 2016.12.23 Init. release version
 *  @param[in]  rsa_sign          : struct of rsa signature
 *  @param[in]  msg_digest               : array of hashed_message
 *  @param[in]  msg_digest_byte_len      : byte length of hashed_message
 *  @param[in]  key_index         : slot # in secure storage
 *	@retval     SUCCESS           : Success
 *	@retval     Others(!=0)       : fail - error from sub-function
 */
int isp_rsa_sign_md_securekey(struct sRSA_SIGN *rsa_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index);

/*! @fn         isp_rsa_verify_md_securekey(struct sRSA_SIGN *rsa_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index)
 *	@ingroup    SECURITY_ISP
 *	@brief      rsa function for verification of signature using securekey
 *  @version    v0.10 : 2016.12.23 Init. release version
 *  @param[in]  rsa_sign          : struct of rsa signature
 *  @param[in]  msg_digest               : array of hashed_message
 *  @param[in]  msg_digest_byte_len      : byte length of hashed_message
 *  @param[in]  key_index         : slot # in secure storage
 *	@retval     SUCCESS           : Success
 *	@retval     Others(!=0)       : fail - error from sub-function
 */
int isp_rsa_verify_md_securekey(struct sRSA_SIGN *rsa_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index);

/*! @fn         isp_ecdsa_verify_md_securekey(struct sECC_SIGN * ecc_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index)
 *	@ingroup    SECURITY_ISP
 *	@brief      ecdsa function for generating signature
 *  @version    v0.10 : 2016.12.23 Init. release version
 *  @param[out] ecc_sign     : struct of signature
 *  @param[in]  msg_digest           : array of hashed_message
 *  @param[in]  msg_digest_byte_len      : length of hashed_message with byte unit
 *  @param[in]  key_index     : slot # in secure storage
 *	@retval     SUCCESS       : Success
 *	@retval     Others(!=0)   : fail - error from sub-function
 */
int isp_ecdsa_verify_md_securekey(struct sECC_SIGN *ecc_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index);

/*! @fn         isp_write_cert(unsigned char *data, unsigned int data_byte_len, unsigned int index)
 *	@ingroup    SECURITY_ISP
 *	@brief      write cert function for secure storage
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.51 : 2016.12.29 Use Pre-Defined Variable for the readability
 *  @param[in]  data             : array of data
 *  @param[in]  data_byte_len    : length of data
 *  @param[in]  index            : slot index of secure storage
 *	@retval		SUCCESS		: Success
 *	@retval		Others(!=0)	: fail - error from sub-function
 */
int isp_write_cert(unsigned char *data, unsigned int data_byte_len, unsigned int index);

/*! @fn         isp_read_cert(unsigned char *data, unsigned int *data_byte_len, unsigned int index)
 *	@ingroup    SECURITY_ISP
 *	@brief      read cert function for secure storage
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.51 : 2016.12.29 Use Pre-Defined Variable for the readability
 *  @param[out] data             : array of data
 *  @param[out] data_byte_len    : length of data
 *  @param[in]  index            : slot index of secure storage
 *	@retval		SUCCESS		: Success
 *	@retval		Others(!=0)	: fail - error from sub-function
 */
int isp_read_cert(unsigned char *data, unsigned int *data_byte_len, unsigned int index);

/*!
 *	@ingroup    SECURITY_ISP
 *	@brief      ecdsa function for generating key to secure storage
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @param[in]  object_id     : selection of algorithm
 *  @param[out] encryptedkey  : encrypted key data
 *	@retval     SUCCESS       : Success
 *	@retval     Others(!=0)   : fail - error from sub-function
 */
int isp_ecdsa_generate_key_encryptedkey(unsigned int object_id, unsigned char *encryptedkey);

/*!
 *	@ingroup    SECURITY_ISP
 *	@brief      ecc function for get publickey to secure storage
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.60 : 2016.12.23 Change factorykey id parameter, Set Null input for object_id
 *  @param[out] ecc_publickey : public key from slot # in secure storage
 *  @param[in]  object_id     : selection of algorithm
 *  @param[in]  encryptedkey  : encrypted key data
 *	@retval     SUCCESS       : Success
 *	@retval     Others(!=0)   : fail - error from sub-function
 */
int isp_ecdsa_get_publickey_encryptedkey(struct sECC_KEY *ecc_publickey, unsigned int object_id, unsigned char *encryptedkey);

/*!
 *	@ingroup    SECURITY_ISP
 *	@brief      ecdh function for computing shared secret using userkey input
 *  @version    v0.00 : 2017.1.19 Init. release version
 *  @param[out] shared_secret : array of shared secret
 *  @param[out] shared_secret_byte_len : length of shared_secret_byte_len
 *  @param[in]  ecc_publickey : struct of ecc key
 *  @param[in]  encryptedkey  : encrypted key data
 *	@retval     SUCCESS       : Success
 *	@retval     Others(!=0)   : fail - error from sub-function
 */
int isp_compute_ecdh_encryptedkey(unsigned char *shared_secret, unsigned int *shared_secret_byte_len, struct sECC_KEY ecc_publickey, unsigned char *encryptedkey);

/*!
 *	@ingroup    SECURITY_ISP
 *	@brief      dh function for generate pvkey & pukey pair from user input parameter p & g
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @param[out] i_dh_param        : struct of DH param = {p, g, pukey}
 *  @param[out]  encryptedkey     : encrypted key for p, g, pvkey
 *	@retval     SUCCESS       : Success
 *	@retval     Others(!=0)   : fail - error from sub-function
 */
int isp_dh_generate_keypair_userparam_encryptedkey(struct sDH_PARAM *i_dh_param, unsigned char *encryptedkey);

/*!
 *	@ingroup    SECURITY_ISP
 *	@brief      dh function for compute shared secret
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.60 : 2016.12.27 Set Null input for object_id
 *  @param[out] shared_secret : array of shared secret
 *  @param[in]  dh_publickey  : struct of DH publickey
 *  @param[in]  encryptedkey  : encrypted key data
 *	@retval     SUCCESS       : Success
 *	@retval     Others(!=0)   : fail - error from sub-function
 */
int isp_dh_compute_shared_secret_encryptedkey(unsigned char *shared_secret, unsigned int *shared_secret_byte_len, struct sDH_PARAM dh_publickey, unsigned char *encryptedkey);

/*!
 *	@ingroup    SECURITY_ISP
 *	@brief      set securekey function
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.51 : 2016.12.29 Use Pre-Defined Variable for the readability
 *  @param[in]  data                 : array of data
 *  @param[in]  data_byte_len        : length of data
 *  @param[in]  key_type             : type of key
 *  @param[out] encryptedkey         : encrypted key data
 *	@retval		SUCCESS              : Success
 *	@retval		Others(!=0)          : fail - error from sub-function
 */
int isp_set_encryptedkey(unsigned char *key, unsigned int key_byte_len, unsigned int key_type, unsigned char *encryptedkey);

/*! @fn         isp_ecdsa_verify_md_securekey(struct sECC_SIGN * ecc_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned int key_index)
 *	@ingroup    SECURITY_ISP
 *	@brief      ecdsa function for generating signature
 *  @version    v0.10 : 2016.12.23 Init. release version
 *  @param[out] ecc_sign     : struct of signature
 *  @param[in]  msg_digest           : array of hashed_message
 *  @param[in]  msg_digest_byte_len      : length of hashed_message with byte unit
 *  @param[in]  encryptedkey     : encrypted key data
 *	@retval     SUCCESS       : Success
 *	@retval     Others(!=0)   : fail - error from sub-function
 */
int isp_ecdsa_verify_md_encryptedkey(struct sECC_SIGN *ecc_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned char *encryptedkey);

/*! @fn         isp_rsa_verify_md_encryptedkey(struct sRSA_SIGN *rsa_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned char *encryptedkey)
 *	@ingroup    SECURITY_ISP
 *	@brief      rsa function for verification of signature using securekey
 *  @version    v0.10 : 2016.12.23 Init. release version
 *  @param[in]  rsa_sign          : struct of rsa signature
 *  @param[in]  msg_digest               : array of hashed_message
 *  @param[in]  msg_digest_byte_len      : byte length of hashed_message
 *  @param[in]  encryptedkey          : encrypted key data
 *	@retval     SUCCESS           : Success
 *	@retval     Others(!=0)       : fail - error from sub-function
 */
int isp_rsa_verify_md_encryptedkey(struct sRSA_SIGN *rsa_sign, unsigned char *msg_digest, unsigned int msg_digest_byte_len, unsigned char *encryptedkey);

/*! @fn         isp_rsa_encrypt_encryptedkey(unsigned char *output, unsigned char *input, unsigned int input_byte_len, unsigned int key_index)
 *	@ingroup    SECURITY_ISP
 *	@brief      rsa function for encryption using securekey
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.60 : 2016.12.27 Change factorykey id parameter, Set Null input for object_id
 *  @param[out] output            : array of output message
 *  @param[in]  input             : array of input message
 *  @param[in]  input_byte_len    : length of input message
 *  @param[in]  encryptedkey      : encrypted key data
 *	@retval     SUCCESS           : Success
 *	@retval     Others(!=0)       : fail - error from sub-function
 */
int isp_rsa_encrypt_encryptedkey(unsigned char *output, unsigned int *output_byte_len, unsigned char *input, unsigned int input_byte_len, unsigned char *encryptedkey);
#endif							/* ISP_CUSTOM_H_ */
