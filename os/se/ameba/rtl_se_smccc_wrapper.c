/**************************************************************************************************
*
* Copyright 2023 Samsung Electronics All Rights Reserved.
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
* Decription: For BL33 (Non Secure) access to BL32 (Secure) via Secure Monitor Call Service
***************************************************************************************************/

#include "rtl_se_crypto_function.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * struct arm_smccc_res - Result from SMC/HVC call
 * @a0-a3 result values from registers 0 to 3
 */
struct arm_smccc_res {
	unsigned long a0;
	unsigned long a1;
	unsigned long a2;
	unsigned long a3;
};

/**
 * struct arm_smccc_quirk - Contains quirk information
 * @id: quirk identification
 * @state: quirk specific information
 * @a6: Qualcomm quirk entry for returning post-smc call contents of a6
 */
struct arm_smccc_quirk {
	int	id;
	union {
		unsigned long a6;
	} state;
};

/**
 * __arm_smccc_smc() - make SMC calls
 * @a0-a7: arguments passed in registers 0 to 7
 * @res: result values from registers 0 to 3
 * @quirk: points to an arm_smccc_quirk, or NULL when no quirks are required.
 *
 * This function is used to make SMC calls following SMC Calling Convention.
 * The content of the supplied param are copied to registers 0 to 7 prior
 * to the SMC instruction. The return values are updated with the content
 * from register 0 to 3 on return from the SMC instruction.  An optional
 * quirk structure provides vendor specific behavior.
 */

extern void __arm_smccc_smc(unsigned long a0, unsigned long a1,
							unsigned long a2, unsigned long a3, unsigned long a4,
							unsigned long a5, unsigned long a6, unsigned long a7,
							struct arm_smccc_res *res, struct arm_smccc_quirk *quirk);

#define arm_smccc_smc(...) __arm_smccc_smc(__VA_ARGS__, NULL)

static unsigned long invoke_securetest(unsigned long function_id,
									unsigned long arg0, unsigned long arg1,
									unsigned long arg2, unsigned long arg3)
{
	struct arm_smccc_res res;

	arm_smccc_smc(function_id, arg0, arg1, arg2, arg3, 0, 0, 0, &res);

	//printf("\n ==== Output SMCCC: [function_id:%x] %x %x %x %x ==== \n", function_id, res.a0, res.a1, res.a2, res.a3);

	return res.a0;
}

/**
 * Common
 */
int ameba_hal_init(hal_init_param *params, factory_struc *input_data, ns_passin_struc *ns_passin)
{
	int ret = 0;
	ret = invoke_securetest(0x82000010, (uint32_t)params, (uint32_t)input_data, (uint32_t)ns_passin, 0);
	return ret;
}

int ameba_hal_deinit(void)
{
	int ret = 0;
	ret = invoke_securetest(0x82000011, 0, 0, 0, 0);
	return ret;
}

int ameba_hal_get_status(void)
{
	int ret = 0;
	ret = invoke_securetest(0x82000013, 0, 0, 0, 0);
	return ret;
}

/**
 * Key Manager
 */
int ameba_hal_set_key(hal_key_type mode, uint32_t key_idx, inout_struc *pub_prikey)
{
	int ret = 0;
	ret = invoke_securetest(0x82000014, (uint32_t)mode, key_idx, (uint32_t)pub_prikey, 0);
	return ret;
}

int ameba_hal_get_key(hal_key_type mode, uint32_t key_idx, hal_data *key)
{
	int ret = 0;
	ret = invoke_securetest(0x82000015, (uint32_t)mode, key_idx, (uint32_t)key, 0);
	return ret;
}

int ameba_hal_remove_key(hal_key_type mode, uint32_t key_idx)
{
	int ret = 0;
	ret = invoke_securetest(0x82000016, (uint32_t)mode, key_idx, 0, 0);
	return ret;
}

int ameba_hal_generate_key(hal_key_type mode, uint32_t key_idx)
{
	int ret = 0;
	ret = invoke_securetest(0x82000017, (uint32_t)mode, key_idx, 0, 0);
	return ret;
}

/**
 * Authenticate
 */
int ameba_hal_generate_random(uint32_t len, hal_data *random)
{
	int ret = 0;
	ret = invoke_securetest(0x82000018, len, (uint32_t)random, 0, 0);
	return ret;
}

int ameba_hal_get_hash(hal_hash_type mode, hal_data *input, hal_data *hash)
{
	int ret = 0;
	ret = invoke_securetest(0x82000019, (uint32_t)mode, (uint32_t)input, (uint32_t)hash, 0);
	return ret;
}

int ameba_hal_get_hmac(hal_hmac_type mode, inout_struc *in_output, uint32_t key_idx)
{
	int ret = 0;
	ret = invoke_securetest(0x8200001A, (uint32_t)mode, (uint32_t)in_output, key_idx, 0);
	return ret;
}

int ameba_hal_rsa_sign_md(hal_rsa_mode mode, inout_struc *in_output, uint32_t key_idx)
{
	int ret = 0;
	hal_rsa_mode *in_modeptr;
	in_modeptr = &mode;

	ret = invoke_securetest(0x8200001B, (uint32_t)in_modeptr, (uint32_t)in_output, key_idx, 0);
	return ret;
}

int ameba_hal_rsa_verify_md(hal_rsa_mode mode, inout_struc *in_output, uint32_t key_idx)
{
	int ret = 0;
	hal_rsa_mode *in_modeptr;
	in_modeptr = &mode;

	ret = invoke_securetest(0x8200001C, (uint32_t)in_modeptr, (uint32_t)in_output, key_idx, 0);
	return ret;
}

int ameba_hal_ecdsa_sign_md(hal_ecdsa_mode mode, inout_struc *in_output, uint32_t key_idx)
{
	int ret = 0;
	hal_ecdsa_mode *in_modeptr;
	in_modeptr = &mode;

	ret = invoke_securetest(0x8200001D, (uint32_t)in_modeptr, (uint32_t)in_output, key_idx, 0);
	return ret;
}

int ameba_hal_ecdsa_verify_md(hal_ecdsa_mode mode, inout_struc *in_output, uint32_t key_idx)
{
	int ret = 0;
	hal_ecdsa_mode *in_modeptr;
	in_modeptr = &mode;

	ret = invoke_securetest(0x8200001E, (uint32_t)in_modeptr, (uint32_t)in_output, key_idx, 0);
	return ret;
}

int ameba_hal_dh_generate_param(uint32_t dh_idx, hal_dh_data *dh_param)
{
	int ret = 0;
	ret = invoke_securetest(0x8200001F, dh_idx, (uint32_t)dh_param, 0, 0);
	return ret;
}

int ameba_hal_dh_compute_shared_secret(hal_dh_data *dh_param, uint32_t dh_idx, hal_data *shared_secret)
{
	int ret = 0;
	ret = invoke_securetest(0x82000020, (uint32_t)dh_param, dh_idx, (uint32_t)shared_secret, 0);
	return ret;
}

int ameba_hal_ecdh_compute_shared_secret(hal_ecdh_data *ecdh_param, uint32_t key_idx, hal_data *shared_secret)
{
	int ret = 0;
	ret = invoke_securetest(0x82000021, (uint32_t)ecdh_param, key_idx, (uint32_t)shared_secret, 0);
	return ret;
}

int ameba_hal_set_certificate(uint32_t cert_idx, hal_data *cert_in)
{
	int ret = 0;
	ret = invoke_securetest(0x82000022, cert_idx, (uint32_t)cert_in, 0, 0);
	return ret;
}

int ameba_hal_get_certificate(uint32_t cert_idx, hal_data *cert_out)
{
	int ret = 0;
	ret = invoke_securetest(0x82000023, cert_idx, (uint32_t)cert_out, 0, 0);
	return ret;
}

int ameba_hal_remove_certificate(uint32_t cert_idx)
{
	int ret = 0;
	ret = invoke_securetest(0x82000024, cert_idx, 0, 0, 0);
	return ret;
}

int ameba_hal_get_factory_key(uint32_t key_idx, hal_data *key)
{
	int ret = 0;
	ret = invoke_securetest(0x82000025, key_idx, (uint32_t)key, 0, 0);
	return ret;
}

int ameba_hal_get_factory_cert(uint32_t cert_idx, hal_data *cert)
{
	int ret = 0;
	ret = invoke_securetest(0x82000026, cert_idx, (uint32_t)cert, 0, 0);
	return ret;
}

int ameba_hal_get_factory_data(uint32_t data_idx, hal_data *data)
{
	int ret = 0;
	ret = invoke_securetest(0x82000027, data_idx, (uint32_t)data, 0, 0);
	return ret;
}

/**
 * Crypto
 */
int ameba_hal_aes_encrypt(inout_struc *in_output, hal_aes_param *aes_param, uint32_t key_idx)
{
	int ret = 0;
	ret = invoke_securetest(0x82000028, (uint32_t)in_output, (uint32_t)aes_param, key_idx, 0);
	return ret;
}

int ameba_hal_aes_decrypt(inout_struc *in_output, hal_aes_param *aes_param, uint32_t key_idx)
{
	int ret = 0;
	ret = invoke_securetest(0x82000029, (uint32_t)in_output, (uint32_t)aes_param, key_idx, 0);
	return ret;
}

int ameba_hal_rsa_encrypt(inout_struc *in_output, hal_rsa_mode *rsa_mode, uint32_t key_idx)
{
	int ret = 0;
	ret = invoke_securetest(0x8200002A, (uint32_t)in_output, (uint32_t)rsa_mode, key_idx, 0);
	return ret;
}

int ameba_hal_rsa_decrypt(inout_struc *in_output, hal_rsa_mode *rsa_mode, uint32_t key_idx)
{
	int ret = 0;
	ret = invoke_securetest(0x8200002B, (uint32_t)in_output, (uint32_t)rsa_mode, key_idx, 0);
	return ret;
}

/**
 * Secure Storage
 */
int ameba_hal_write_storage(uint32_t ss_idx, hal_data *data)
{
	int ret = 0;
	ret = invoke_securetest(0x8200002C, ss_idx, (uint32_t)data, 0, 0);
	return ret;
}

int ameba_hal_read_storage(uint32_t ss_idx, hal_data *data)
{
	int ret = 0;
	ret = invoke_securetest(0x8200002D, ss_idx, (uint32_t)data, 0, 0);
	return ret;
}

int ameba_hal_delete_storage(uint32_t ss_idx)
{
	int ret = 0;
	ret = invoke_securetest(0x8200002E, ss_idx, 0, 0, 0);
	return ret;
}

int ameba_hal_write_factory_key(uint32_t key_idx, hal_data *key)
{
	int ret = 0;
	ret = invoke_securetest(0x8200002F, key_idx, (uint32_t)key, 0, 0);
	return ret;
}

int ameba_hal_write_factory_cert(uint32_t cert_idx, hal_data *cert)
{
	int ret = 0;
	ret = invoke_securetest(0x82000030, cert_idx, (uint32_t)cert, 0, 0);
	return ret;
}

int ameba_hal_write_protect(uint32_t status)
{
	int ret = 0;
	ret = invoke_securetest(0x82000031, status, 0, 0, 0);
	return ret;
}

int ameba_hal_SignatureVerify(uint32_t *imagehdr_addr)
{
	int ret = 0;
	ret = invoke_securetest(0x82000032, (uint32_t)imagehdr_addr, 0, 0, 0);
	return ret;
}

/* Kernel Image Verification */
int ameba_SBOOT_Validate_Algorithm(OTA_SIG_CHECK_ADAPTER *OTA_adapter)
{
	int ret = 0;
	ret = invoke_securetest(0x82000034, (uint32_t)OTA_adapter, 0, 0, 0);
	return ret;
} 
 
int ameba_SBOOT_Validate_Signature(OTA_SIG_CHECK_ADAPTER *OTA_adapter)
{
	int ret = 0;
	ret = invoke_securetest(0x82000035, (uint32_t)OTA_adapter, 0, 0, 0);
	return ret;
}

int ameba_SBOOT_Validate_ImgHash(OTA_SIG_CHECK_ADAPTER *OTA_adapter)
{
	int ret = 0;
	ret = invoke_securetest(0x82000036, (uint32_t)OTA_adapter, 0, 0, 0);
	return ret;
}

int ameba_SBOOT_Validate_PubKey(OTA_SIG_CHECK_ADAPTER *OTA_adapter)
{
	int ret = 0;
	ret = invoke_securetest(0x82000037, (uint32_t)OTA_adapter, 0, 0, 0);
	return ret;
}

/* Secure Key Derive Function */
int ameba_Secure_KeyDeriveFunc(secure_kdf_struc *KFD_info)
{
	int ret = 0;
	ret = invoke_securetest(0x8200003E, (uint32_t)KFD_info, 0, 0, 0);
	return ret;
}

/* Secure FTL */
int ameba_ftl_secure_init(uint32_t key_addr)
{
	int ret = 0;
	ret = invoke_securetest(0x82000040, key_addr, 0, 0, 0);
	return ret;
}

int ameba_ftl_save_to_storage(uint8_t *tmp_buff, void *pdata, uint16_t offset, uint16_t size)
{
	int ret = 0;
	ret = invoke_securetest(0x82000041, (unsigned long)tmp_buff, (unsigned long)pdata, offset, size);
	return ret;
}

int ameba_ftl_load_from_storage(void *pdata, uint16_t offset, uint16_t size)
{
	int ret = 0;
	ret = invoke_securetest(0x82000042, (unsigned long)pdata, offset, size, 0);
	return ret;
}


