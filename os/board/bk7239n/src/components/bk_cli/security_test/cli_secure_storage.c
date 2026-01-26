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
#include "tfm_secure_storage.h"
#include <tinyara/security_hal.h>

#define FACTORY_KEY_MAX_INDEX (32)
#define SS_READ_ECP_KEY_BUF_SIZE (132)
#define SS_READ_BUF_SIZE     (4096)
#define SS_MAX_READ_BUF_SIZE (SS_READ_BUF_SIZE*3)

extern int hexstr2bin(const char *hex, u8 *buf, size_t len);
extern int armino_hal_write_storage(uint32_t ss_idx, hal_data *data);
extern int armino_hal_read_storage(uint32_t ss_idx, hal_data *data);
extern int armino_hal_delete_storage(uint32_t ss_idx);
extern int armino_hal_set_certificate(uint32_t cert_idx, hal_data *cert_in);
extern int armino_hal_get_certificate(uint32_t cert_idx, hal_data *cert_out);
extern int armino_hal_remove_certificate(uint32_t cert_idx);
extern int armino_hal_get_key(hal_key_type mode, uint32_t key_idx, hal_data *key_out);


static void cli_sec_help(void)
{
	CLI_LOGI("sec cert_write  {index} - Write certificate to secure storage\r\n");
	CLI_LOGI("sec cert_read   {index} - Read certificate from secure storage\r\n");
	CLI_LOGI("sec cert_delete {index} - Delete certificate from secure storage\r\n");
	CLI_LOGI("sec data_write  {index} - Write data to secure storage\r\n");
	CLI_LOGI("sec data_read   {index} - Read data from secure storage\r\n");
	CLI_LOGI("sec data_delete {index} - Delete data from secure storage\r\n");
	CLI_LOGI("sec key_read    {mode} {index} - Read key from secure storage\r\n");
}

void sec_ss_data_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_sec_help();
		return;
	}
	uint8_t *data = NULL;
	int ret = 0;
	hal_data data_in = {0};
	hal_data data_out = {0};

	if (os_strcmp(argv[1], "write") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t index = os_strtoul(argv[2], NULL, 10);
		uint32_t size = os_strtoul(argv[3], NULL, 10);
		data = (uint8_t *)os_malloc(size);
		if (data == NULL) {
			CLI_LOGE("Failed to allocate memory for data\r\n");
			return;
		}
		for (uint32_t i = 0; i < size; i++) {
			data[i] = i % 0x100;
		}
		CLI_LOGI("Writing data to index %u, size %u\r\n", index, size);
		data_in.data = data;
		data_in.data_len = size;
		ret = armino_hal_write_storage(index, &data_in);
		os_free(data);
		data = NULL;
		if (ret != SS_SUCCESS) {
			CLI_LOGE("Failed to write data, ret = %d\r\n", ret);
		}
	} else if (os_strcmp(argv[1], "read") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 3);
		uint32_t index = os_strtoul(argv[2], NULL, 10);
		uint32_t size = (index == 0) ? SS_MAX_READ_BUF_SIZE : SS_READ_BUF_SIZE;
		data = (uint8_t *)os_malloc(size);
		if (data == NULL) {
			CLI_LOGE("Failed to allocate memory for data\r\n");
			return;
		}
		os_memset(data, 0, size);
		CLI_LOGI("Reading data from index %u\r\n", index);
		data_out.data = data;
		data_out.data_len = size;
		ret = armino_hal_read_storage(index, &data_out);
		if (ret != SS_SUCCESS) {
			CLI_LOGE("Failed to read data, ret = %d\r\n", ret);
			os_free(data);
			data = NULL;
			return;
		} else {
			size = data_out.data_len;
			CLI_LOGI("Data read successfully, length = %u\r\n", size);
			CLI_LOGI("Data (hex):\r\n");
			for (uint32_t i = 0; i < size; i++) {
				CLI_LOGI("%02x", data[i]);
			}
			CLI_LOGI("\r\n");
			os_free(data);
			data = NULL;
		}
	} else if (os_strcmp(argv[1], "delete") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 3);
		uint32_t index = os_strtoul(argv[2], NULL, 10);
		CLI_LOGI("Deleting data from index %u\r\n", index);
		ret = armino_hal_delete_storage(index);
		if (ret != SS_SUCCESS) {
			CLI_LOGE("Failed to delete data, ret = %d\r\n", ret);
		} else {
			CLI_LOGI("Data deleted successfully\r\n");
		}
	} else {
		cli_sec_help();
		return;
	}
}

#define TEST_CA_CRT_RSA_SHA256_PEM                                         \
    "-----BEGIN CERTIFICATE-----\r\n"                                      \
    "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\r\n" \
    "-----END CERTIFICATE-----\r\n"                                        \

static const char test_cas_pem[] =
    TEST_CA_CRT_RSA_SHA256_PEM
    "";

void sec_ss_cert_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_sec_help();
		return;
	}
	uint8_t *data = NULL;
	int ret = 0;
	hal_data cert_in = {0};
	hal_data cert_out = {0};

	if (os_strcmp(argv[1], "write") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 3);
		uint32_t index = os_strtoul(argv[2], NULL, 10);
		uint32_t size = sizeof(test_cas_pem);
		CLI_LOGI("Writing data to index %d, size %d\r\n", index, size);
		data = (uint8_t *)os_malloc(size);
		if (data == NULL) {
			CLI_LOGE("Failed to allocate memory for data\r\n");
			return;
		}
		os_memset(data, 0, size);
		os_memcpy(data, test_cas_pem, size);
		cert_in.data = data;
		cert_in.data_len = size;
		ret = armino_hal_set_certificate(index, &cert_in);
		os_free(data);
		data = NULL;
		if (ret != SS_SUCCESS) {
			CLI_LOGE("Failed to write certificate, ret = %d\r\n", ret);
		} else {
			CLI_LOGI("Certificate written successfully\r\n");
		}
	} else if (os_strcmp(argv[1], "read") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 3);
		uint32_t index = os_strtoul(argv[2], NULL, 10);
		uint32_t size = SS_READ_BUF_SIZE;
		data = (uint8_t *)os_malloc(size);
		if (data == NULL) {
			CLI_LOGE("Failed to allocate memory for data\r\n");
			return;
		}
		os_memset(data, 0, size);
		CLI_LOGI("Reading data from index %u\r\n", index);
		cert_out.data = data;
		cert_out.data_len = size;
		ret = armino_hal_get_certificate(index, &cert_out);
		if (ret != SS_SUCCESS) {
			CLI_LOGE("Failed to read certificate, ret = %d\r\n", ret);
			os_free(data);
			data = NULL;
			return;
		} else {
			size = cert_out.data_len;
			CLI_LOGI("Certificate read successfully, length = %u\r\n", size);
			CLI_LOGI("Certificate (hex):\r\n");
			for (uint32_t i = 0; i < size; i++) {
				CLI_LOGI("%c", data[i]);
			}
			CLI_LOGI("\r\n");
			os_free(data);
			data = NULL;
		}
	} else if (os_strcmp(argv[1], "delete") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 3);
		uint32_t index = os_strtoul(argv[2], NULL, 10);
		CLI_LOGI("Deleting certificate from index %u\r\n", index);
		ret = armino_hal_remove_certificate(index);
		if (ret != SS_SUCCESS) {
			CLI_LOGE("Failed to delete certificate, ret = %d\r\n", ret);
		} else {
			CLI_LOGI("Certificate deleted successfully\r\n");
		}
	} else {
		cli_sec_help();
		return;
	}
}

void sec_ss_key_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = 0;
	uint8_t *key = NULL, *key_2 = NULL;
	uint32_t key_len = 0;
	hal_data key_out = {0};

	if (argc < 2) {
		cli_sec_help();
		return;
	}

	key = (uint8_t *)os_malloc(SS_READ_ECP_KEY_BUF_SIZE/2);
	if (key == NULL) {
		CLI_LOGE("Failed to allocate memory for key\r\n");
		return;
	}
	os_memset(key, 0, SS_READ_ECP_KEY_BUF_SIZE/2);

	key_2 = (uint8_t *)os_malloc(SS_READ_ECP_KEY_BUF_SIZE/2);
	if (key_2 == NULL) {
		CLI_LOGE("Failed to allocate memory for private key\r\n");
		return;
	}
	os_memset(key_2, 0, SS_READ_ECP_KEY_BUF_SIZE/2);

	if (os_strcmp(argv[1], "read") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		hal_key_type mode = os_strtoul(argv[2], NULL, 10);
		uint32_t index = os_strtoul(argv[3], NULL, 10);
		CLI_LOGI("Reading key from index %u\r\n", index);
		hal_data key_out = {0};

		key_out.data = key;
		key_out.data_len = SS_READ_ECP_KEY_BUF_SIZE/2;
		key_out.priv = key_2;
		key_out.priv_len = SS_READ_ECP_KEY_BUF_SIZE/2;
		ret = armino_hal_get_key(mode, index, &key_out);

		if (ret != SS_SUCCESS) {
			if (index < FACTORY_KEY_MAX_INDEX)
				CLI_LOGI("Factory key is not supported to export, ret = %d\r\n", ret);
			else
				CLI_LOGE("Failed to read key, ret = %d\r\n", ret);
			os_free(key);
			key = NULL;
			return;

		} else {
			key_len = key_out.data_len;
			CLI_LOGI("Key read successfully, length = %u\r\n", key_len);
			CLI_LOGI("Key (hex):\r\n");
			for (uint32_t i = 0; i < key_len; i++) {
				CLI_LOGI("%02x", key[i]);
			}
			CLI_LOGI("\r\n");

			key_len = key_out.priv_len;
			CLI_LOGI("key 2 (hex):\r\n");
			for (uint32_t i = 0; i < key_len; i++) {
				CLI_LOGI("%02x", key_2[i]);
			}
			CLI_LOGI("\r\n");

			os_free(key);
			key = NULL;
		}
	} else {
		cli_sec_help();
		return;
	}
}

#define SECURITY_STORAGE_CMD_CNT (sizeof(s_security_storage_commands) / sizeof(struct cli_command))
static const struct cli_command s_security_storage_commands[] = {
	{"ss_data", "ss_data {write|read|delete}", sec_ss_data_command },
	{"ss_cert", "ss_cert {write|read|delete}", sec_ss_cert_command },
	{"ss_key",  "ss_key {read}",               sec_ss_key_command  },
};

int cli_secure_storage_init(void)
{
	return cli_register_commands(s_security_storage_commands, SECURITY_STORAGE_CMD_CNT);
}
