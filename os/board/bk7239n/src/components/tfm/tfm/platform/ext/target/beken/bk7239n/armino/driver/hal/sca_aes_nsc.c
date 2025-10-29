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
#include "psa/crypto.h"
#include "mbedtls/aes.h"
#include "psa/crypto_sizes.h"
#include "psa/crypto_values.h"
#include "dubhe_gcm.h"
#include "dubhe_sca.h"
#include "tfm_secure_api.h"
#include "tfm_sca_aes_nsc.h"
#include "tfm_secure_storage.h"
#include "dubhe_trng.h"

#define SCA_AES_DEBUG   (0)

static record_sca_aes_key_info_t s_key_info[TOTAL_KEY_STORAGE_INDEX_NUM] = {0};

/* Calling this API in tfm_s.bin to trigger the compiler to generate NSC symbols in tfm_s_veneers.o.
 **/
void psa_sca_aes_nsc_stub(void)
{
	return;
}

__tfm_psa_secure_gateway_no_naked_attributes__ psa_status_t psa_sca_aes_generate_key(aes_key_type key_type, uint32_t key_index)
{
#if SCA_AES_DEBUG
	printf("line :%d ,key_type :%d, key_index:0x%x-0x%x \r\n", __LINE__,key_type, key_index,s_key_info[key_index].key_idx);
#endif
    if (key_index < (SS_RAM_INDEX_START))
    {
        printf("Key index %u for factory key\n", key_index);
        return SCA_AES_INVALID_ARGS;
    }

	//double_check key_data is exsit
	if(s_key_info[key_index].key_idx != 0)
	{
		printf("Key index %u  and key_data is alreay exsit  \r\n", key_index);
		return SCA_AES_SUCCESS;
	}

	switch (key_type)
	{
		case AES_KEY_128:
		    s_key_info[key_index].key_bits = AES_KEY_128_BITS;
			break;
		case AES_KEY_192:
		    s_key_info[key_index].key_bits = AES_KEY_192_BITS;
			break;
		case AES_KEY_256:
		    s_key_info[key_index].key_bits = AES_KEY_256_BITS;
			break;
		default:
			return SCA_AES_INVALID_ARGS;
	}

    s_key_info[key_index].key_idx = key_index;
	arm_ce_seed_read(s_key_info[key_index].key_data, (s_key_info[key_index].key_bits/8));

#if SCA_AES_DEBUG
	printf("key_idx:0x%x, key_type:0x%x, key_bits:0x%x", key_index, key_type, s_key_info[key_index].key_bits);
	for(int i = 0;i< s_key_info[key_index].key_bits/8; i++)
	{
		printf("key_data[%d] =0x%x \r\n", i, s_key_info[key_index].key_data[i]);
	}
#endif
	return SCA_AES_SUCCESS;
}

__tfm_psa_secure_gateway_no_naked_attributes__ psa_status_t psa_sca_aes_set_key(aes_key_type key_type, uint32_t key_idx, key_data_t *key)
{
	uint32_t get_key_len = 0;

	if((key == NULL) || (key->data == NULL) || (key->data_len == 0)||(key_idx >= TOTAL_KEY_STORAGE_INDEX_NUM))
	{
		return SCA_AES_INVALID_ARGS;
	}

#if SCA_AES_DEBUG
	printf("line :%d ,key_type :%d, key_idx:0x%x \r\n", __LINE__,key_type, key_idx);
#endif

	switch (key_type)
	{
		case AES_KEY_128:
			if(key->data_len != AES_KEY_128_BITS/8)
			{
				return SCA_AES_INVALID_ARGS;
			}
			break;
		case AES_KEY_192:
			if(key->data_len != AES_KEY_192_BITS/8)
			{
				return SCA_AES_INVALID_ARGS;
			}
			break;
		case AES_KEY_256:
			if(key->data_len != AES_KEY_256_BITS/8)
			{
				return SCA_AES_INVALID_ARGS;
			}
			break;
		default:
			break;
	}

	if(key_idx >= (SS_RAM_INDEX_START))
	{
		memcpy(s_key_info[key_idx].key_data, key->data, key->data_len);
	}
	else
	{
		return SCA_AES_INVALID_ARGS;
	}

	s_key_info[key_idx].key_bits = (key->data_len *8);
	s_key_info[key_idx].key_idx = key_idx;

#if SCA_AES_DEBUG
	printf("key_idx:0x%x, key_type:0x%x, key_len:0x%x \r\n", key_idx, key_type, key->data_len);
	printf("get_key_len:0x%x \r\n", get_key_len);
	for(int i = 0;i< key->data_len; i++)
	{
		printf("set_key_data[%d] =0x%x \r\n", i, s_key_info[key_idx].key_data[i]);
	}
#endif
	return SCA_AES_SUCCESS;
}

__tfm_psa_secure_gateway_no_naked_attributes__ psa_status_t psa_sca_aes_remove_key(aes_key_type key_type, uint32_t key_index)
{
	uint32_t key_len =0;

#if SCA_AES_DEBUG
	printf("line :%d, key_bits :%d, key_index:0x%x \r\n", __LINE__,key_type, key_index);
#endif

	if(key_index >= (SS_RAM_INDEX_START))
	{
		s_key_info[key_index].key_idx = 0;
	}
	else
	{
		return SCA_AES_INVALID_ARGS;
	}

	return SCA_AES_SUCCESS;
}

psa_status_t psa_sca_aes_get_flash_key(uint32_t key_index)
{
	psa_status_t ret_val = SCA_AES_FAIL;
	uint32_t get_key_len = 0;

#if SCA_AES_DEBUG
	printf("line :%d, key_index :0x%x \r\n", __LINE__,key_index);
#endif
	if(key_index < (SS_RAM_INDEX_START))
	{
		ret_val = ss_read(SS_TYPE_IS_KEY, key_index, s_key_info[key_index].key_data, &(get_key_len));
		if(ret_val != 0)
		{
			return ret_val;
		}
	}
	else
	{
		ret_val = SCA_AES_INVALID_ARGS;
	}

	s_key_info[key_index].key_bits = (get_key_len *8);
	s_key_info[key_index].key_idx = key_index;

	return ret_val;
}

__tfm_psa_secure_gateway_no_naked_attributes__ psa_status_t psa_sca_aes_ecb_crypt(sca_aes_context_t *sca_aes_ctx, uint8_t*input, size_t in_len, uint8_t*output)
{
	psa_status_t ret_val = SCA_AES_FAIL;
	uint8_t *psa_input_text = NULL;
	uint8_t *psa_output_text= NULL;
	mbedtls_aes_context ctx;
	uint32_t key_indx = 0;

	if((sca_aes_ctx == NULL)||(output == NULL)||(input == NULL))
	{
		return SCA_AES_INVALID_ARGS;
	}

	key_indx = sca_aes_ctx->key_idx;

#if SCA_AES_DEBUG
	printf("line :%d ,sca_aes_ctx->key_idx :0x%x \r\n", __LINE__,sca_aes_ctx->key_idx);
	printf("sca_aes_ctx->mode :%d \r\n", sca_aes_ctx->mode);
	printf("s_key_info[sca_aes_ctx->key_idx].key_idx :0x%x \r\n", s_key_info[sca_aes_ctx->key_idx].key_idx);
	printf("in_len:0x%x \r\n", in_len);
	printf("key_bits:0x%x \r\n", s_key_info[key_indx].key_bits);
#endif

	if(in_len % SCA_AES_BLOCK_SIZE !=0)
	{
		return SCA_AES_LENGTH_NO_ALIGN;
	}

	psa_input_text = os_malloc(in_len);
	if(psa_input_text == NULL)
	{
		return SCA_AES_MALLOC_FAIL;
	}
	os_memset(psa_input_text, 0 , in_len);
	os_memcpy(psa_input_text, input, in_len);

	psa_output_text = os_malloc(in_len);
	if(psa_output_text == NULL)
	{
		os_free(psa_input_text);
		return SCA_AES_MALLOC_FAIL;
	}
	os_memset(psa_output_text, 0 , in_len);

	if(key_indx < (SS_RAM_INDEX_START))
	{
		ret_val = psa_sca_aes_get_flash_key(key_indx);
		if(ret_val != 0)
		{
			printf("line :%d ,ret_val :%d get flash key fail \r\n", __LINE__,ret_val);
			goto exit;
		}
	}

	dubhe_aes_init(&ctx);
	if(sca_aes_ctx->mode == SCA_AES_ECB_DECRYPT_MODE)
	{
		ret_val =  dubhe_aes_setkey_dec( &ctx, s_key_info[key_indx].key_data, s_key_info[key_indx].key_bits);
	}
	else if(sca_aes_ctx->mode == SCA_AES_ECB_ENCRYPT_MODE)
	{
		ret_val =  dubhe_aes_setkey_enc( &ctx, s_key_info[key_indx].key_data, s_key_info[key_indx].key_bits);
	}
	else{
		printf("line :%d ,sca_aes_ctx->mode :0x%x \r\n", __LINE__,sca_aes_ctx->mode);
		ret_val = SCA_AES_INVALID_ARGS;
		goto exit;
	}

	if(ret_val != 0)
	{
		printf("line :%d ,ret_val :%d \r\n", __LINE__,ret_val);
		goto exit;
	}

	ret_val =  dubhe_aes_crypt_ecb(&ctx, sca_aes_ctx->mode, psa_input_text, psa_output_text);
	if(ret_val != 0)
	{
		printf("line :%d ,ret_val :%d \r\n", __LINE__,ret_val);
		goto exit;
	}

#if SCA_AES_DEBUG
	for(int i =0; i<in_len;i++)
	{
		printf("line :%d ,psa_output_text[%d] :%x \r\n", __LINE__,i,psa_output_text[i]);
	}
#endif

	os_memcpy(output, psa_output_text, in_len);
	os_memcpy(&(sca_aes_ctx->out_length), &in_len , sizeof(size_t));

exit:
	os_free(psa_input_text);
	os_free(psa_output_text);
	dubhe_aes_free(&ctx);

	return ret_val;
}

__tfm_psa_secure_gateway_no_naked_attributes__ psa_status_t psa_sca_aes_cbc_crypt(sca_aes_context_t *sca_aes_ctx, uint8_t*input, size_t in_len, uint8_t*output)
{
	psa_status_t ret_val = SCA_AES_FAIL;
	uint8_t *psa_input_text = NULL;
	uint8_t *psa_output_text= NULL;
	uint8_t *psa_iv         = NULL;
	mbedtls_aes_context ctx;
	uint32_t key_indx = 0;

	if((sca_aes_ctx == NULL)||(output == NULL)||(input == NULL)||(sca_aes_ctx->iv == NULL))
	{
		return SCA_AES_INVALID_ARGS;
	}

	key_indx = sca_aes_ctx->key_idx;

#if SCA_AES_DEBUG
	printf("line :%d ,sca_aes_ctx->key_idx :0x%x \r\n", __LINE__,sca_aes_ctx->key_idx);
	printf("sca_aes_ctx->mode :%d \r\n", sca_aes_ctx->mode);
	printf("key_idx:0x%x, in_len:0x%x \r\n", s_key_info[sca_aes_ctx->key_idx].key_idx, in_len);
	printf("s_key_info[key_indx].key_bits:0x%x \r\n", s_key_info[key_indx].key_bits);
#endif
	if(sca_aes_ctx->iv_len != SCA_AES_IV_LEN)
	{
		printf("line :%d, iv len not match \r\n",__LINE__);
		return SCA_AES_INVALID_ARGS;
	}

	if(in_len % SCA_AES_BLOCK_SIZE !=0)
	{
		return SCA_AES_LENGTH_NO_ALIGN;
	}

	psa_input_text = os_malloc(in_len);
	if(psa_input_text == NULL)
	{
		return SCA_AES_MALLOC_FAIL;
	}
	os_memset(psa_input_text, 0 , in_len);
	os_memcpy(psa_input_text, input, in_len);

	psa_output_text = os_malloc(in_len);
	if(psa_output_text == NULL)
	{
		os_free(psa_input_text);
		return SCA_AES_MALLOC_FAIL;
	}
	os_memset(psa_output_text, 0 , in_len);

	psa_iv = os_malloc(sca_aes_ctx->iv_len);
	if(psa_iv == NULL)
	{
		os_free(psa_input_text);
		os_free(psa_output_text);
		return SCA_AES_MALLOC_FAIL;
	}
	os_memset(psa_iv, 0 , sca_aes_ctx->iv_len);
	os_memcpy(psa_iv, sca_aes_ctx->iv, sca_aes_ctx->iv_len);

	if(key_indx < (SS_RAM_INDEX_START))
	{
		ret_val = psa_sca_aes_get_flash_key(key_indx);
		if(ret_val != 0)
		{
			printf("line :%d ,ret_val :%d get flash key fail \r\n", __LINE__,ret_val);
			goto exit;
		}
	}

	dubhe_aes_init(&ctx);
	if(sca_aes_ctx->mode == SCA_AES_CBC_DECRYPT_MODE)
	{
		ret_val =  dubhe_aes_setkey_dec( &ctx, s_key_info[key_indx].key_data, s_key_info[key_indx].key_bits);
	}
	else if(sca_aes_ctx->mode == SCA_AES_CBC_ENCRYPT_MODE)
	{
		ret_val =  dubhe_aes_setkey_enc( &ctx, s_key_info[key_indx].key_data, s_key_info[key_indx].key_bits);
	}
	else{
		printf("line :%d ,sca_aes_ctx->mode :0x%x \r\n", __LINE__,sca_aes_ctx->mode);
		ret_val = SCA_AES_INVALID_ARGS;
		goto exit;
	}

	if(ret_val != 0)
	{
		printf("line :%d ,ret_val :%d \r\n", __LINE__,ret_val);
		goto exit;
	}
	sca_aes_ctx->mode -= SCA_AES_CBC_DECRYPT_MODE;
	ret_val =  dubhe_aes_crypt_cbc(&ctx, sca_aes_ctx->mode,in_len, psa_iv, psa_input_text, psa_output_text);
	if(ret_val != 0)
	{
		printf("line :%d ,ret_val :%d \r\n", __LINE__,ret_val);
		goto exit;
	}

#if SCA_AES_DEBUG
	for(int i =0; i<in_len;i++)
	{
		printf("line :%d ,psa_output_text[%d] :%x \r\n", __LINE__,i,psa_output_text[i]);
	}
#endif
	os_memcpy(output, psa_output_text, in_len);
	os_memcpy(&(sca_aes_ctx->out_length), &in_len , sizeof(size_t));

exit:
	os_free(psa_input_text);
	os_free(psa_output_text);
	os_free(psa_iv);
	dubhe_aes_free(&ctx);

	return ret_val;
}

__tfm_psa_secure_gateway_no_naked_attributes__ psa_status_t psa_sca_aes_ctr_crypt(sca_aes_context_t *sca_aes_ctx, uint8_t*input, size_t in_len, uint8_t*output)
{
	psa_status_t ret_val       = SCA_AES_FAIL;
	uint8_t *psa_input_text    = NULL;
	uint8_t *psa_output_text   = NULL;
    uint8_t *psa_nonce_counter = NULL;
    uint8_t *psa_stream_block  = NULL;
	uint32_t psa_nc_off        = 0;
	uint32_t key_indx          = 0;
	mbedtls_aes_context ctx;

	if((sca_aes_ctx == NULL)||(output == NULL)||(input == NULL)||
	(sca_aes_ctx->nonce_counter == NULL)||(sca_aes_ctx->stream_block == NULL) ||(sca_aes_ctx->nc_off == NULL))
	{
		return SCA_AES_INVALID_ARGS;
	}

	key_indx = sca_aes_ctx->key_idx;
	psa_nc_off = *(sca_aes_ctx->nc_off);

#if SCA_AES_DEBUG
	printf("line :%d ,sca_aes_ctx->key_idx :0x%x \r\n", __LINE__,sca_aes_ctx->key_idx);
	printf("sca_aes_ctx->mode :%d \r\n",sca_aes_ctx->mode);
	printf("key_idx:0x%x, in_len:0x%x \r\n", key_indx, in_len);
	printf("s_key_info[key_indx].key_bits:0x%x \r\n", s_key_info[key_indx].key_bits);
	printf("psa_nc_off:0x%x \r\n", psa_nc_off);
#endif

	psa_input_text = os_malloc(in_len);
	if(psa_input_text == NULL)
	{
		return SCA_AES_MALLOC_FAIL;
	}
	os_memset(psa_input_text, 0 , in_len);
	os_memcpy(psa_input_text, input, in_len);

	psa_output_text = os_malloc(in_len);
	if(psa_output_text == NULL)
	{
		os_free(psa_input_text);
		return SCA_AES_MALLOC_FAIL;
	}
	os_memset(psa_output_text, 0 , in_len);

	psa_nonce_counter = os_malloc(SCA_AES_NONCE_COUNTER_LEN);
	if(psa_nonce_counter == NULL)
	{
		os_free(psa_input_text);
		os_free(psa_output_text);
		return SCA_AES_MALLOC_FAIL;
	}
	os_memset(psa_nonce_counter, 0 , SCA_AES_NONCE_COUNTER_LEN);
	os_memcpy(psa_nonce_counter, sca_aes_ctx->nonce_counter, SCA_AES_NONCE_COUNTER_LEN);

	psa_stream_block = os_malloc(SCA_AES_STREAM_BLOCK_LEN);
	if(psa_stream_block == NULL)
	{
		os_free(psa_input_text);
		os_free(psa_output_text);
		os_free(psa_nonce_counter);
		return SCA_AES_MALLOC_FAIL;
	}
	os_memset(psa_stream_block, 0 , SCA_AES_STREAM_BLOCK_LEN);
	os_memcpy(psa_stream_block, sca_aes_ctx->stream_block, SCA_AES_STREAM_BLOCK_LEN);

	if(key_indx < (SS_RAM_INDEX_START))
	{
		ret_val = psa_sca_aes_get_flash_key(key_indx);
		if(ret_val != 0)
		{
			printf("line :%d ,ret_val :%d get flash key fail \r\n", __LINE__,ret_val);
			goto exit;
		}
	}

	dubhe_aes_init(&ctx);
	if((sca_aes_ctx->mode == SCA_AES_CTR_DECRYPT_MODE) ||(sca_aes_ctx->mode == SCA_AES_CTR_ENCRYPT_MODE))
	{
		ret_val =  dubhe_aes_setkey_enc( &ctx, s_key_info[key_indx].key_data, s_key_info[key_indx].key_bits);
	}
	else{
		printf("line :%d ,sca_aes_ctx->mode :%d \r\n", __LINE__,sca_aes_ctx->mode);
		ret_val = SCA_AES_INVALID_ARGS;
		goto exit;
	}

	if(ret_val != 0)
	{
		printf("line :%d ,ret_val :%d \r\n", __LINE__,ret_val);
		goto exit;
	}

	ret_val =  dubhe_aes_crypt_ctr(&ctx, in_len, &psa_nc_off,psa_nonce_counter, psa_stream_block, psa_input_text, psa_output_text);
	if(ret_val != 0)
	{
		printf("line :%d ,ret_val :%d \r\n", __LINE__,ret_val);
		goto exit;
	}

#if SCA_AES_DEBUG
	for(int i =0; i<in_len; i++)
	{
		printf("line :%d ,psa_output_text[%d] :%x \r\n", __LINE__,i,psa_output_text[i]);
	}
	for(int j =0; j<in_len; j++)
	{
		printf("line :%d ,psa_stream_block[%d] :%x \r\n", __LINE__,j,psa_stream_block[j]);
	}
	printf("line :%d ,psa_nc_off :%d \r\n", __LINE__,psa_nc_off);
#endif

	os_memcpy(output, psa_output_text, in_len);
	os_memcpy(&(sca_aes_ctx->out_length), &in_len , sizeof(size_t));
	os_memcpy(sca_aes_ctx->nonce_counter, psa_nonce_counter, SCA_AES_NONCE_COUNTER_LEN);
	os_memcpy(sca_aes_ctx->stream_block, psa_stream_block, SCA_AES_STREAM_BLOCK_LEN);
	*(sca_aes_ctx->nc_off) = psa_nc_off;

exit:
	os_free(psa_input_text);
	os_free(psa_output_text);
	os_free(psa_nonce_counter);
	os_free(psa_stream_block);
	dubhe_aes_free(&ctx);

	return ret_val;
}


__tfm_psa_secure_gateway_no_naked_attributes__ psa_status_t psa_sca_aes_cfb_crypt(sca_aes_context_t *sca_aes_ctx, uint8_t*input, size_t in_len, uint8_t*output)
{
	psa_status_t ret_val       = SCA_AES_FAIL;
	uint8_t *psa_input_text    = NULL;
	uint8_t *psa_output_text   = NULL;
    uint8_t *psa_iv            = NULL;
	uint32_t psa_iv_off        = 0;
	uint32_t key_indx          = 0;
	mbedtls_aes_context ctx;

	if((sca_aes_ctx == NULL)||(output == NULL)||(input == NULL)||(sca_aes_ctx->iv == NULL)||(sca_aes_ctx->iv_offset == NULL))
	{
		return SCA_AES_INVALID_ARGS;
	}

	key_indx = sca_aes_ctx->key_idx;
	psa_iv_off = *(sca_aes_ctx->iv_offset);
#if SCA_AES_DEBUG
	printf("line :%d ,sca_aes_ctx->key_idx :0x%x \r\n", __LINE__,sca_aes_ctx->key_idx);
	printf("sca_aes_ctx->mode :%d \r\n", sca_aes_ctx->mode);
	printf("key_idx:0x%x, in_len:0x%x \r\n", key_indx, in_len);
	printf("s_key_info[key_indx].key_bits:0x%x \r\n", s_key_info[key_indx].key_bits);
	printf("psa_iv_off:0x%x \r\n", psa_iv_off);
#endif

	if(sca_aes_ctx->iv_len != SCA_AES_IV_LEN)
	{
		printf("line :%d, iv len not match \r\n",__LINE__);
		return SCA_AES_INVALID_ARGS;
	}

	psa_input_text = os_malloc(in_len);
	if(psa_input_text == NULL)
	{
		return SCA_AES_MALLOC_FAIL;
	}
	os_memset(psa_input_text, 0 , in_len);
	os_memcpy(psa_input_text, input, in_len);

	psa_output_text = os_malloc(in_len);
	if(psa_output_text == NULL)
	{
		os_free(psa_input_text);
		return SCA_AES_MALLOC_FAIL;
	}
	os_memset(psa_output_text, 0 , in_len);

	psa_iv = os_malloc(SCA_AES_IV_LEN);
	if(psa_iv == NULL)
	{
		os_free(psa_input_text);
		os_free(psa_output_text);
		return SCA_AES_MALLOC_FAIL;
	}
	os_memset(psa_iv, 0 , SCA_AES_IV_LEN);
	os_memcpy(psa_iv, sca_aes_ctx->iv, SCA_AES_IV_LEN);

	if(key_indx < (SS_RAM_INDEX_START))
	{
		ret_val = psa_sca_aes_get_flash_key(key_indx);
		if(ret_val != 0)
		{
			printf("line :%d ,ret_val :%d get flash key fail \r\n", __LINE__,ret_val);
			goto exit;
		}
	}

	dubhe_aes_init(&ctx);
	if((sca_aes_ctx->mode == SCA_AES_CFB_DECRYPT_MODE) ||(sca_aes_ctx->mode == SCA_AES_CFB_ENCRYPT_MODE))
	{
		ret_val =  dubhe_aes_setkey_enc( &ctx, s_key_info[key_indx].key_data, s_key_info[key_indx].key_bits);
	}
	else{
		printf("line :%d ,sca_aes_ctx->mode :%d \r\n", __LINE__,sca_aes_ctx->mode);
		ret_val = SCA_AES_INVALID_ARGS;
		goto exit;
	}

	if(ret_val != 0)
	{
		printf("line :%d ,ret_val :%d \r\n", __LINE__,ret_val);
		goto exit;
	}

	sca_aes_ctx->mode -= SCA_AES_CFB_DECRYPT_MODE;
	ret_val =  dubhe_aes_crypt_cfb128(&ctx, sca_aes_ctx->mode, in_len, &psa_iv_off, psa_iv, psa_input_text, psa_output_text);
	if(ret_val != 0)
	{
		printf("line :%d ,ret_val :%d \r\n", __LINE__,ret_val);
		goto exit;
	}
#if SCA_AES_DEBUG
	for(int i =0; i<in_len; i++)
	{
		printf("line :%d ,psa_output_text[%d] :%x \r\n", __LINE__,i,psa_output_text[i]);
	}
	printf("line :%d ,psa_iv_off :%d \r\n", __LINE__,psa_iv_off);
#endif

	os_memcpy(output, psa_output_text, in_len);
	os_memcpy(&(sca_aes_ctx->out_length), &in_len , sizeof(size_t));
	*(sca_aes_ctx->iv_offset) = psa_iv_off;

exit:
	os_free(psa_input_text);
	os_free(psa_output_text);
	os_free(psa_iv);
	dubhe_aes_free(&ctx);

	return ret_val;
}

__tfm_psa_secure_gateway_no_naked_attributes__  psa_status_t armino_hal_psa_gcm_encrypt(gcm_type_t *gcm_param)
{

	arm_ce_gcm_ctx_t gcm_ctx;
	int32          ret_val;
	unsigned char* psa_plain_text  = NULL;
	unsigned char  psa_iv[AES_GCM_IV_LENGTH]   = {0};
	unsigned char  tag[AES_GCM_KEY_LENGTH/2];
    uint32_t key_len = AES_GCM_KEY_LENGTH;

	if(gcm_param == NULL || gcm_param->iv == NULL || gcm_param->dec_data == NULL || gcm_param->tag == NULL)
	{
		printf("line :%d ,gcm_param or iv or dec_data or tag is NULL \r\n", __LINE__);
		return SCA_AES_INVALID_ARGS;
	}

	if (gcm_param->dec_data_len % AES_GCM_BLOCK_SIZE !=0) {
		printf("dec_data_len %d not align 16\r\n", gcm_param->dec_data_len);
		return AES_GCM_LENGTH_NO_16_ALIGH;
	}

	psa_plain_text = os_malloc(gcm_param->dec_data_len);

	if (psa_plain_text == NULL) {
		printf("malloc psa_plain_text fail\r\n");
		return AES_GCM_MALLOC_FAIL;
	}

	memset(psa_plain_text, 0, gcm_param->dec_data_len);

	memcpy(psa_plain_text, gcm_param->dec_data, gcm_param->dec_data_len);

	memcpy(psa_iv, gcm_param->iv, AES_GCM_IV_LENGTH);

	uint32_t key_idx = gcm_param->key_id;
	if (key_idx < (SS_RAM_INDEX_START)) {
		ret_val = psa_sca_aes_get_flash_key(key_idx);
		if (ret_val != 0) {
			printf("line :%d ,ret_val :%d get flash key fail \r\n", __LINE__,ret_val);
			goto exit;
		}
	}

	ret_val = arm_ce_gcm_init(&gcm_ctx, ARM_CE_AES_GCM);
	if (ret_val != 0) {
		os_free(psa_plain_text);
		return ret_val;
	}

	ret_val = arm_ce_gcm_set_key(&gcm_ctx,  s_key_info[key_idx].key_data, s_key_info[key_idx].key_bits);
	if(ret_val != 0)
	{
		printf("set key failed: %d\r\n", ret_val);
		goto exit;
	}

	ret_val = arm_ce_gcm_start(&gcm_ctx, DBH_GCM_ENCRYPT, psa_iv, AES_GCM_IV_LENGTH);
	if(ret_val != 0)
	{
		printf("start failed: %d\r\n", ret_val);
		goto exit;
	}

	ret_val= arm_ce_gcm_update_aad(&gcm_ctx, gcm_param->aad_len, gcm_param->aad);
	if(ret_val !=0)
	{
		printf("update aad failed: %d\r\n", ret_val);
		goto exit;
	}

	ret_val = arm_ce_gcm_update(&gcm_ctx, gcm_param->dec_data_len, psa_plain_text, psa_plain_text);
	if(ret_val != 0)
	{
		printf("update failed: %d\r\n", ret_val);
		goto exit;
	}

	ret_val = arm_ce_gcm_update(&gcm_ctx, 0, NULL, NULL);
	if(ret_val != 0)
	{
		printf("update again failed: %d\r\n", ret_val);
		goto exit;
	}

	ret_val = arm_ce_gcm_finish(&gcm_ctx, tag, sizeof(tag));
	if(ret_val != 0)
	{
		printf("finish failed: %d\r\n", ret_val);
		goto exit;
	}

	memcpy(gcm_param->enc_data, psa_plain_text, gcm_param->dec_data_len);
	memcpy(gcm_param->tag, tag + AES_GCM_KEY_LENGTH/2 - sizeof(tag), sizeof(tag));
	gcm_param->tag_len = sizeof(tag);

exit:
	arm_ce_gcm_free(&gcm_ctx);
	os_free(psa_plain_text);

	return ret_val;

}

__tfm_psa_secure_gateway_no_naked_attributes__  psa_status_t armino_hal_psa_gcm_decrypt(gcm_type_t *gcm_param)
{
	arm_ce_gcm_ctx_t gcm_ctx;
	int32  ret_val;
	unsigned char* psa_encrypted_text = NULL;
	unsigned char  psa_key[AES_GCM_KEY_LENGTH] = {0};
	unsigned char  psa_iv[AES_GCM_IV_LENGTH]   = {0};
	unsigned char  tag[AES_GCM_KEY_LENGTH/2];
    uint32_t key_len = AES_GCM_KEY_LENGTH;

	if(gcm_param == NULL || gcm_param->iv == NULL || gcm_param->enc_data == NULL)
	{
		printf("line :%d ,gcm_param is NULL \r\n", __LINE__);
		return SCA_AES_INVALID_ARGS;
	}

	if(gcm_param->enc_data_len % AES_GCM_BLOCK_SIZE !=0)
	{
		printf("enc_data_len %d not align 16\r\n", gcm_param->enc_data_len);
		return AES_GCM_LENGTH_NO_16_ALIGH;
	}

	psa_encrypted_text = os_malloc(gcm_param->enc_data_len);
	if(psa_encrypted_text == NULL)
	{
		printf("malloc psa_encrypted_text fail\r\n");
		return AES_GCM_MALLOC_FAIL;
	}

	memset(psa_encrypted_text, 0 , gcm_param->enc_data_len);

	memcpy(psa_encrypted_text, gcm_param->enc_data, gcm_param->enc_data_len);

	memcpy(psa_iv, gcm_param->iv, AES_GCM_IV_LENGTH);

	uint32_t key_idx = gcm_param->key_id;
	if (key_idx < (SS_RAM_INDEX_START)) {
		ret_val = psa_sca_aes_get_flash_key(key_idx);
		if (ret_val != 0) {
			printf("line :%d ,ret_val :%d get flash key fail \r\n", __LINE__,ret_val);
			goto exit;
		}
	}

	ret_val = arm_ce_gcm_init(&gcm_ctx, ARM_CE_AES_GCM);
	if(ret_val != 0)
	{
		printf("init failed: %d\r\n", ret_val);
		os_free(psa_encrypted_text);
		return ret_val;
	}

	ret_val = arm_ce_gcm_set_key(&gcm_ctx,  s_key_info[key_idx].key_data, s_key_info[key_idx].key_bits);
	if(ret_val != 0)
	{
		printf("set key failed: %d\r\n", ret_val);
		goto exit;
	}

	ret_val = arm_ce_gcm_start(&gcm_ctx, DBH_GCM_DECRYPT, psa_iv, AES_GCM_IV_LENGTH);
	if(ret_val != 0)
	{
		printf("start failed: %d\r\n", ret_val);
		goto exit;
	}

	ret_val= arm_ce_gcm_update_aad(&gcm_ctx, gcm_param->aad_len, gcm_param->aad);
	if(ret_val !=0)
	{
		printf("update aad failed: %d\r\n", ret_val);
		goto exit;
	}

	ret_val = arm_ce_gcm_update(&gcm_ctx, gcm_param->enc_data_len, psa_encrypted_text, psa_encrypted_text);
	if(ret_val != 0)
	{
		printf("update failed: %d\r\n", ret_val);
		goto exit;
	}

	ret_val = arm_ce_gcm_update(&gcm_ctx, 0, NULL, NULL);
	if(ret_val != 0)
	{
		printf("update again failed: %d\r\n", ret_val);
		goto exit;
	}

	ret_val = arm_ce_gcm_finish(&gcm_ctx, tag, sizeof(tag));
	if(ret_val != 0)
	{
		printf("finish failed: %d\r\n", ret_val);
		goto exit;
	}

	memcpy(gcm_param->dec_data, psa_encrypted_text, gcm_param->enc_data_len);
	memcpy(gcm_param->tag, tag, AES_GCM_KEY_LENGTH/2);

exit:
	arm_ce_gcm_free(&gcm_ctx);
	os_free(psa_encrypted_text);

	return ret_val;
}
