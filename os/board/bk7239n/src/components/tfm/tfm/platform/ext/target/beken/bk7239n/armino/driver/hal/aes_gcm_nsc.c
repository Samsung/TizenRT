// Copyright 2022-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!
//

#include <stdio.h>
#include <string.h>
#include <os/os.h>
#include <os/mem.h>
#include "tfm_secure_api.h"
#include "dubhe_gcm.h"
#include "dubhe_sca.h" 
#include "tfm_aes_gcm_nsc.h"
#include "_otp.h"
#include "otp_s.h"

//#include <components/log.h>

#define GCM_TAG "GCM"

#define GCM_LOGI(...) BK_LOGI(GCM_TAG, ##__VA_ARGS__)
#define GCM_LOGW(...) BK_LOGW(GCM_TAG, ##__VA_ARGS__)
#define GCM_LOGE(...) BK_LOGE(GCM_TAG, ##__VA_ARGS__)
#define GCM_LOGD(...) BK_LOGD(GCM_TAG, ##__VA_ARGS__)

/* Calling this API in tfm_s.bin to trigger the compiler to generate NSC symbols in tfm_s_veneers.o.
 **/
 

#define AES_GCM_KEY_SHIFT_BITS  (3)
#define AES_GCM_DEBUG_LOG       (0)

static unsigned char s_aes_gcm_iv_val[AES_GCM_IV_LENGTH] ={0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB};
static unsigned char s_aes_gcm_key_val[AES_GCM_KEY_LENGTH];

void psa_aes_gcm_nsc_stub(void)
{
	return;
}

psa_status_t psa_read_randomkey_from_otp2(uint8_t item_id)
{
	uint32_t ret  = 0;
	uint32_t size = 0;
	
	OTP_ACTIVE();

	size = otp_map[1][item_id].allocated_size;

	ret = otp_read(2,item_id,s_aes_gcm_key_val,size);

	otp_sleep();

	return ret;
}

#if AES_GCM_DEBUG_LOG
psa_status_t psa_write_randomkey_to_otp2(uint8_t item_id)
{
	uint32_t ret  = 0;
	uint32_t size = 0;

	OTP_ACTIVE();

	size = otp_map[1][item_id].allocated_size;

	ret = otp_update(2,item_id,s_aes_gcm_key_val,size);

	otp_sleep();

	return ret;
}
#endif

__tfm_psa_secure_gateway_no_naked_attributes__  psa_status_t psa_aes_gcm_decrypt(aes_gcm_context_t *ex_ctx, unsigned char*input, size_t in_len, unsigned char*output)
{
	arm_ce_gcm_ctx_t gcm_ctx;
	int32  ret_val;
	unsigned char* psa_encrypted_text = NULL;
	unsigned char  psa_key[AES_GCM_KEY_LENGTH] = {0};
	unsigned char  psa_iv[AES_GCM_IV_LENGTH]   = {0};
	unsigned char  tag[AES_GCM_KEY_LENGTH/2];

#if AES_GCM_DEBUG_LOG
	GCM_LOGD("padded_ciphertext_len :0x%x \r\n", in_len);
#endif

	if((ex_ctx == NULL)||(output == NULL)||(input == NULL)||(in_len == 0))
	{
		return AES_GCM_PARAMETER_FAIL;
	}

	if(in_len % AES_GCM_BLOCK_SIZE !=0)
	{
		return AES_GCM_LENGTH_NO_16_ALIGH;
	}

	psa_encrypted_text = os_malloc(in_len);
	if(psa_encrypted_text == NULL)
	{
		return AES_GCM_MALLOC_FAIL;
	}

	ret_val = psa_read_randomkey_from_otp2(ex_ctx->item_id);
	if(ret_val != 0)
	{
		os_free(psa_encrypted_text);
		return ret_val;
	}

	memset(psa_encrypted_text, 0 , in_len);

	memcpy(psa_encrypted_text, input, in_len);

	memcpy(psa_key, s_aes_gcm_key_val, AES_GCM_KEY_LENGTH);

	memcpy(psa_iv, s_aes_gcm_iv_val, AES_GCM_IV_LENGTH);

	ret_val = arm_ce_gcm_init(&gcm_ctx, ARM_CE_AES_GCM);
	if(ret_val != 0)
	{
		os_free(psa_encrypted_text);
		return ret_val;
	}

	ret_val = arm_ce_gcm_set_key(&gcm_ctx,  psa_key, (AES_GCM_KEY_LENGTH<<AES_GCM_KEY_SHIFT_BITS));
	if(ret_val != 0)
	{
		goto exit;
	}

	ret_val = arm_ce_gcm_start(&gcm_ctx, DBH_GCM_DECRYPT, psa_iv, AES_GCM_IV_LENGTH);
	if(ret_val != 0)
	{
		goto exit;
	}

	ret_val = arm_ce_gcm_update(&gcm_ctx, in_len, psa_encrypted_text, psa_encrypted_text);
	if(ret_val != 0)
	{
		goto exit;
	}

	ret_val = arm_ce_gcm_update(&gcm_ctx, 0, NULL, NULL);
	if(ret_val != 0)
	{
		goto exit;
	}

	ret_val = arm_ce_gcm_finish(&gcm_ctx, tag, sizeof(tag));
	if(ret_val != 0)
	{
		goto exit;
	}

#if AES_GCM_DEBUG_LOG
	GCM_LOGD("unpadding_len :0x%x \r\n",in_len);
	for (int i = 0; i < in_len; i++)
	{
		GCM_LOGD("0x%x ", psa_encrypted_text[i]);
	}
#endif

	memcpy(output, psa_encrypted_text, in_len);
	memcpy(&(ex_ctx->out_length), &in_len , sizeof(size_t));

exit:
	arm_ce_gcm_free(&gcm_ctx);
	os_free(psa_encrypted_text);
	
	return ret_val;
}

__tfm_psa_secure_gateway_no_naked_attributes__  psa_status_t psa_aes_gcm_encrypt(aes_gcm_context_t *ex_ctx, unsigned char*input, size_t in_len, unsigned char*output)
{
	arm_ce_gcm_ctx_t gcm_ctx;
	int32          ret_val;
	unsigned char* psa_plain_text  = NULL;
	unsigned char  psa_key[AES_GCM_KEY_LENGTH] = {0};
	unsigned char  psa_iv[AES_GCM_IV_LENGTH]   = {0};
	unsigned char  tag[AES_GCM_KEY_LENGTH/2];

	if((ex_ctx == NULL)||(input == NULL)||(output == NULL)||(in_len == 0))
	{
		return AES_GCM_PARAMETER_FAIL;
	}

	if(in_len % AES_GCM_BLOCK_SIZE !=0)
	{
		return AES_GCM_LENGTH_NO_16_ALIGH;
	}

#if AES_GCM_DEBUG_LOG
	GCM_LOGD("\r\n plaintext_length :0x%x , item_id :%d \r\n", in_len, ex_ctx->item_id);
#endif

	psa_plain_text = os_malloc(in_len);

	if(psa_plain_text == NULL)
	{
		return AES_GCM_MALLOC_FAIL;
	}

	ret_val = psa_read_randomkey_from_otp2(ex_ctx->item_id);
	if(ret_val != 0)
	{
		GCM_LOGE("\r\n get randomkey fail! \r\n");
		os_free(psa_plain_text);
		return ret_val;
	}

	memset(psa_plain_text, 0, in_len);

	memcpy(psa_plain_text, input, in_len);

	memcpy(psa_key, s_aes_gcm_key_val, AES_GCM_KEY_LENGTH);

	memcpy(psa_iv, s_aes_gcm_iv_val, AES_GCM_IV_LENGTH);

	ret_val = arm_ce_gcm_init(&gcm_ctx, ARM_CE_AES_GCM);
	if(ret_val != 0)
	{
		os_free(psa_plain_text);
		return ret_val;
	}

	ret_val = arm_ce_gcm_set_key(&gcm_ctx,  psa_key, (AES_GCM_KEY_LENGTH<<AES_GCM_KEY_SHIFT_BITS));
	if(ret_val != 0)
	{
		goto exit;
	}

	ret_val = arm_ce_gcm_start(&gcm_ctx, DBH_GCM_ENCRYPT, psa_iv, AES_GCM_IV_LENGTH);
	if(ret_val != 0)
	{
		goto exit;
	}

	ret_val = arm_ce_gcm_update(&gcm_ctx, in_len, psa_plain_text, psa_plain_text);
	if(ret_val != 0)
	{
		goto exit;
	}

	ret_val = arm_ce_gcm_update(&gcm_ctx, 0, NULL, NULL);
	if(ret_val != 0)
	{
		goto exit;
	}

	ret_val = arm_ce_gcm_finish(&gcm_ctx, tag, sizeof(tag));
	if(ret_val != 0)
	{
		goto exit;
	}

#if AES_GCM_DEBUG_LOG
	for (int i = 0; i < in_len; i++)
	{
		GCM_LOGD("0x%x ", psa_plain_text[i]);
	}
#endif
	memcpy(output, psa_plain_text, in_len);

	memcpy(&(ex_ctx->out_length), &in_len , sizeof(size_t));

exit:
	arm_ce_gcm_free(&gcm_ctx);
	os_free(psa_plain_text);
	
	return ret_val;
}
