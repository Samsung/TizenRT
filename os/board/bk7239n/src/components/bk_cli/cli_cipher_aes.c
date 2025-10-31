// Copyright 2020-2025 Beken
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

#include <os/os.h>
#include "cli.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tinyara/config.h>
#include <tinyara/security_hal.h>

#define CIPHER_OUTPUT_LEN (128)

#define AES_LOGI(fmt, ...) CLI_LOGI(fmt, ##__VA_ARGS__)

extern int armino_hal_aes_encrypt(hal_data *dec_data, hal_aes_param *aes_param, uint32_t key_idx, hal_data *enc_data);
extern int armino_hal_aes_decrypt(hal_data *enc_data, hal_aes_param *aes_param, uint32_t key_idx, hal_data *dec_data);
extern int armino_hal_set_key(hal_key_type mode, uint32_t key_idx, hal_data *key, hal_data *prikey);
extern int armino_hal_generate_key(hal_key_type mode, uint32_t key_idx);
extern int armino_hal_remove_key(hal_key_type mode, uint32_t key_idx);
// 测试数据
static uint8_t s_test_128key[] = {
		0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
		0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
		}; // 128-bit key

static uint8_t s_test_192key[] = {
		0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
		0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
		0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
		}; // 192-bit key

static uint8_t s_test_256key[] = {
		0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
		0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
		0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
		0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
		}; // 256-bit key

static uint8_t s_test_iv[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    }; // 16-byte IV

static  uint8_t s_nonce_counter[] = {
	  0x53, 0x69, 0x6E, 0x67, 0x6C, 0x65, 0x20, 0x62,
      0x6C, 0x6F, 0x63, 0x6B, 0x20, 0x6D, 0x73, 0x67 };

static  uint8_t s_stream_block[] = {
      0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };

static uint8_t s_test_aad[] = {0x00, 0x01, 0x02, 0x03, 0x04};

typedef enum {
	AES_ECB_NOPAD = 0,
	AES_CBC_NOPAD,
	AES_CTR,
	AES_CFB128,
	AES_M1_PAD,
	AES_M2_PAD,
	AES_PKCS5_PAD,
    AES_PKCS7_PAD,
	AES_GCM,
	AES_UNKNOWN,
} aes_algo;


static void cli_aes_cipher_help(void)
{
	CLI_LOGI("aes_cipher aes_cipher {key_op|mode|alg}\n");
	CLI_LOGI("key_op {1:generate_key}; {2:set_key} \n");
	CLI_LOGI("mode {0:aes_128bits}; {1:aes_192bits}; {2:aes_256bits} \n");
	CLI_LOGI("alg {0:ECB_NOPAD}; {1:CBC_NOPAD}; {2:AES_CTR};{3:AES_CFB128};{4:AES_M1_PAD};{5:AES_M2_PAD};{6:AES_PKCS5_PAD};{7:AES_PKCS7_PAD};{8:AES_GCM}\n");
	CLI_LOGI("key_index {flash_key_index:0-31};{ram_key_index:32-63} \n");
}

static void cli_aes_cipher_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret;
	uint32_t key_index = 0;
	uint8_t input_data[] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x05, 0x06, 0x07,0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0f, 0x10};
	//uint8_t input_data[] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x05, 0x06, 0x07,0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0f};
    size_t input_len = sizeof(input_data);
    uint8_t output_data[CIPHER_OUTPUT_LEN] = {0};
	uint8_t tag[16] = {0};
    size_t output_len = 0;
	hal_data key_data ;
	uint32_t nonce_off = 0;
	uint32_t iv_off = 0;

	if (argc < 2) {
		cli_aes_cipher_help();
		return;
	}
	AES_LOGI("aes_cipher test enter,line:%d \r\n", __LINE__);

	uint8_t key_op = os_strtoul(argv[1], NULL, 10);
	AES_LOGI("line:%d key_op :%d\r\n", __LINE__,key_op);

	hal_key_type key_mode = os_strtoul(argv[2], NULL, 10);
	AES_LOGI("line:%d key_mode :%d\r\n", __LINE__,key_mode);

 	aes_algo alg = os_strtoul(argv[3], NULL, 10);
	AES_LOGI("line:%d alg :%d\r\n", __LINE__,alg);

    key_index = os_strtoul(argv[4], NULL, 10);
	AES_LOGI("line:%d key_index :%d\r\n", __LINE__,key_index);

	if(key_mode == HAL_KEY_AES_128)
	{
		key_data.data = s_test_128key;
		key_data.data_len =sizeof(s_test_128key);
	}
	else if(key_mode == HAL_KEY_AES_192)
	{
		key_data.data = s_test_192key;
		key_data.data_len =sizeof(s_test_192key);
	}
	else if(key_mode == HAL_KEY_AES_256)
	{
		key_data.data = s_test_256key;
		key_data.data_len =sizeof(s_test_256key);
	}
	else
	{
		AES_LOGI("key_mode:%d is not support \r\n", key_mode);
		return;
	}

	hal_aes_param cipher_aes_param ={
		.iv = s_test_iv,
		.iv_len = sizeof(s_test_iv),
	};

	hal_gcm_param gcm_param ={
		.iv = s_test_iv,
		.iv_len = sizeof(s_test_iv),
	};

	if(alg == AES_ECB_NOPAD)
	{
		cipher_aes_param.mode = HAL_AES_ECB_NOPAD;
	}
	else if(alg == AES_CBC_NOPAD)
	{
		cipher_aes_param.mode = HAL_AES_CBC_NOPAD;
	}
	else if(alg == AES_CTR)
	{
		cipher_aes_param.nonce_counter = os_malloc(16);
		if(cipher_aes_param.nonce_counter !=NULL)
		{
			os_memcpy( cipher_aes_param.nonce_counter,  s_nonce_counter, 16);
		}

		cipher_aes_param.stream_block = os_malloc(16);
		if(cipher_aes_param.stream_block !=NULL)
		{
			os_memcpy( cipher_aes_param.stream_block,  s_stream_block, 16);
		}
		else
		{
			goto exit;
		}

		cipher_aes_param.nc_off = os_malloc(4);
		if(cipher_aes_param.nc_off !=NULL)
		{
			os_memcpy( cipher_aes_param.nc_off,  &nonce_off, 4);
		}
		else
		{
			goto exit;
		}

		cipher_aes_param.mode = HAL_AES_CTR;
	}
	else if(alg == AES_CFB128)
	{
		cipher_aes_param.iv_offset = os_malloc(4);
		if(cipher_aes_param.iv_offset !=NULL)
		{
			os_memcpy( cipher_aes_param.iv_offset,  &iv_off, 4);
		}
		else
		{
			goto exit;
		}

		cipher_aes_param.mode = HAL_AES_CFB128;
	}
	else if(alg == AES_M1_PAD)
	{
		cipher_aes_param.mode = HAL_AES_CBC_ISO9797_M1;
	}
	else if(alg == AES_M2_PAD)
	{
		cipher_aes_param.mode = HAL_AES_CBC_ISO9797_M2;
	}
	else if(alg == AES_PKCS5_PAD)
	{
		cipher_aes_param.mode = HAL_AES_CBC_PKCS5;
	}
	else if(alg == AES_PKCS7_PAD)
	{
		cipher_aes_param.mode = HAL_AES_CBC_PKCS7;
	}
	else if(alg == AES_GCM)
	{
		gcm_param.cipher = HAL_GCM_AES;
		gcm_param.aad = s_test_aad;
		gcm_param.aad_len = 5;
		gcm_param.tag = tag;
		gcm_param.tag_len = sizeof(tag);
	}
	else
	{
		AES_LOGI("alg:%d is not support \r\n", alg);
		cipher_aes_param.mode = HAL_AES_UNKNOWN;
		return;
	}

	AES_LOGI("line:%d, cipher_aes_param.mode:%d  \r\n", __LINE__, cipher_aes_param.mode);

	hal_data input_hal_data = {.data_len = input_len};
	input_hal_data.data = os_malloc(128);
	if(input_hal_data.data == NULL)
	{
		goto exit;
	}
	os_memcpy( input_hal_data.data,  input_data, input_hal_data.data_len);
	hal_data output_hal_data = {.data_len = CIPHER_OUTPUT_LEN};
	output_hal_data.data = os_malloc(128);
	if(output_hal_data.data == NULL)
	{
		goto exit;
	}
	os_memcpy( output_hal_data.data,  output_data, 128);
	AES_LOGI("line:%d,key_len:%d,iv_len:%d,input_len:%d \r\n", __LINE__, key_data.data_len, cipher_aes_param.iv_len, input_hal_data.data_len);

	if(key_op == 1) //GENERATE_KEY_OP
	{
		AES_LOGI("do generate key \r\n");
		ret = armino_hal_generate_key(key_mode, key_index);
		if(ret != 0)
		{
			AES_LOGI("generate key failed, ret:%d \r\n", ret);
			goto exit;
		}
	}
	else if(key_op == 2) //SET_KEY_OP
	{
		AES_LOGI("do set key,key_len:%d \r\n", key_data.data_len);
		if(key_index >=32)
		{
			ret = armino_hal_set_key(key_mode, key_index, &key_data, NULL);
			if(ret != 0)
			{
				AES_LOGI("set key failed, ret:%d \r\n", ret);
				goto exit;
			}
		}
	}
	else
	{
		AES_LOGI("key_op:%d is not support \r\n", key_op);
		goto exit;
	}
	//执行加密操作
	if (alg == AES_GCM) {
		ret = armino_hal_gcm_encrypt(&input_hal_data, &gcm_param, key_index, &output_hal_data);
	} else {
		ret = armino_hal_aes_encrypt(&input_hal_data, &cipher_aes_param, key_index, &output_hal_data);
	}

	if(ret != 0) {
		AES_LOGI("encrypt failed, ret:%d \r\n", ret);
		goto exit;
	}
	AES_LOGI("after encrypted the outlen :%d \r\n", output_hal_data.data_len);

	uint8_t *out_data = (uint8_t*)output_hal_data.data;
	for(int i = 0; i < output_hal_data.data_len; i++ )
	{
		AES_LOGI("output_hal_data.data[%d]:%x \r\n", i, out_data[i]);
	}

	//执行减密操作

	memset(input_hal_data.data, 0, input_len);
	if (alg == AES_CTR){
		AES_LOGI("cipher_aes_param.nc_off:%x\r\n", *cipher_aes_param.nc_off);	
		os_memcpy(cipher_aes_param.nonce_counter, s_nonce_counter, 16); 
		os_memcpy(cipher_aes_param.stream_block , s_stream_block , 16);
		os_memset(cipher_aes_param.nc_off , 0, sizeof(uint32_t));	
		AES_LOGI("cipher_aes_param.iv_offset:%x \r\n",  *cipher_aes_param.nc_off);
		for(int i = 0; i< 16; i++)
	{
		AES_LOGI("nonce_counter:%x \r\n",  (uint8_t*)cipher_aes_param.nonce_counter[i]);
	}
	for(int ij = 0; ij< 16; ij++)
	{
		AES_LOGI("stream_block:%x -- 0x%x \r\n",  (uint8_t*)cipher_aes_param.stream_block[ij], s_stream_block[ij]);
	}
	}else if(alg == AES_CFB128){
		AES_LOGI("cipher_aes_param.iv_offset:%x \r\n",  *cipher_aes_param.iv_offset);
		os_memset(cipher_aes_param.iv_offset, 0, sizeof(uint32_t));	
		AES_LOGI("cipher_aes_param.iv_offset:%x \r\n",  *cipher_aes_param.iv_offset);
	}else{
	}

	if (alg == AES_GCM) {
		ret = armino_hal_gcm_decrypt(&output_hal_data, &gcm_param, key_index, &input_hal_data);
	} else {
		ret = armino_hal_aes_decrypt(&output_hal_data, &cipher_aes_param, key_index, &input_hal_data);
	}

	if(ret != 0)
	{
		AES_LOGI("decrypt failed, ret:%d \r\n", ret);
		goto exit;
	}

	AES_LOGI("after decrypted, the output_len:%d \r\n", input_hal_data.data_len);

	uint8_t *out_datax = (uint8_t*)input_hal_data.data;
	for(int j = 0; j < input_hal_data.data_len; j++ )
	{
		AES_LOGI("input_hal_data.data[%d]:%x \r\n", j, out_datax[j]);
	}

	AES_LOGI("compare_buffers,input_len:%d,input_hal_data.data_len :%d \r\n", input_len, input_hal_data.data_len);

	if(os_memcmp(input_data, input_hal_data.data, input_len) != 0)
	{
		AES_LOGI("AES cipher failed, ret:%d \r\n", ret);
		goto exit;
	}
	else
	{
		armino_hal_remove_key(key_mode , key_index);
		AES_LOGI("AES cipher success !!!!\r\n");
	}
exit:
	if(!cipher_aes_param.nonce_counter){
		os_free(cipher_aes_param.nonce_counter);
		cipher_aes_param.nonce_counter = NULL;
	}

	if(!cipher_aes_param.stream_block){
		os_free(cipher_aes_param.stream_block);
		cipher_aes_param.stream_block = NULL;
	}

	if(!cipher_aes_param.nc_off){
		os_free(cipher_aes_param.nc_off);
		cipher_aes_param.nc_off = NULL;
	}

	if(!cipher_aes_param.iv_offset){
		os_free(cipher_aes_param.iv_offset);
		cipher_aes_param.iv_offset = NULL;
	}

	if(!input_hal_data.data){
		os_free(input_hal_data.data);
		input_hal_data.data = NULL;
	}

	if(!output_hal_data.data){
		os_free(output_hal_data.data);
		output_hal_data.data = NULL;
	}

}

static const struct cli_command s_cipher_aes_commands[] = {
	{"aes_cipher", "aes_cipher {key_op|mode|alg|key_index}", cli_aes_cipher_cmd},
};

#define AES_CMD_CNT (sizeof(s_cipher_aes_commands) / sizeof(struct cli_command))

int cli_cipher_aes_init(void)
{
	return cli_register_commands(s_cipher_aes_commands, AES_CMD_CNT);
}


