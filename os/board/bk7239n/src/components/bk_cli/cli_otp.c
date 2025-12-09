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
#include <driver/otp.h>

#if defined(CONFIG_TFM_OTP_NSC)
#include "tfm_otp_nsc.h"
#endif

uint32_t findFirstZeroBit(uint8_t num)
{
	uint32_t positition = 0;
	while((num & 1) && (positition < 8)){
		num = num >> 1;
		++positition;
	}
	return positition;
}

extern int hexstr2bin(const char *hex, u8 *buf, size_t len);

static void cli_otp_help(void)
{
#if defined(CONFIG_OTP)
	CLI_LOGD("otp_driver init/deinit\r\n");
	CLI_LOGD("write data must be string like 01ab03, you can use pre-write to check data\r\n");

	CLI_LOGD("otp_apb self_test \r\n");
	CLI_LOGD("otp_apb read  [item_id][size] \r\n");
	CLI_LOGD("otp_apb write [item_id][size][data] \r\n");

	CLI_LOGD("otp_apb read_mask/read_permission [item_id] \r\n");
	CLI_LOGD("otp_apb write_mask/write_permission [item_id][mask/permission] \r\n");

	CLI_LOGD("otp_apb read_random [size] \r\n");
	CLI_LOGD("otp_apb pre-write  [size][data] \r\n");

	CLI_LOGD("otp_ahb read [item_id][size] \r\n");
	CLI_LOGD("otp_ahb write [item_id][size][data] \r\n");

	CLI_LOGD("otp_ahb read_permission [item_id] \r\n");
	CLI_LOGD("otp_ahb write_permission [item_id][mask/permission] \r\n");

	CLI_LOGD("otp_ahb read_random [size] \r\n");
	CLI_LOGD("otp_ahb pre-write  [size][data] \r\n");
#endif
#if defined(CONFIG_TFM_OTP_NSC)
	CLI_LOGD("otp_nsc read [item_id] \r\n");
	CLI_LOGD("otp_nsc apb_read [map_id][item_id][size] \r\n"),
	CLI_LOGD("otp_nsc write [map_id][item_id][size][data] \r\n");
	CLI_LOGD("otp_nsc read_permission/write_permission [map_id][item_id][permission] \r\n");
#endif
}

#if defined(CONFIG_OTP)
static void cli_otp_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_otp_help();
		return;
	}
	if (os_strcmp(argv[1], "init") == 0) {
		bk_otp_driver_init();
	} else if (os_strcmp(argv[1], "deinit") == 0){
		bk_otp_driver_deinit();
	} else {
		cli_otp_help();
	}
}

static void cli_otp_apb_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	extern bk_err_t bk_otp_fully_flow_test();

	if (argc < 2) {
		cli_otp_help();
		return;
	}

	if (os_strcmp(argv[1], "self_test") == 0){
#if defined(CONFIG_ATE_TEST)
		uint32_t ret = bk_otp_fully_flow_test();
		BK_RAW_LOGD(NULL, "ret = %u\r\n",ret);
#endif
		BK_RAW_LOGD(NULL,"Enable ATE_TEST\r\n");
	} else if (os_strcmp(argv[1], "read") == 0){
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		uint32_t size = os_strtoul(argv[3], NULL, 10);
		uint8_t* data = (uint8_t*)os_malloc(size*sizeof(uint8_t));
		if(data == NULL){
			BK_RAW_LOGD(NULL, "data os_malloc fail\r\n");
			return;
		}
		memset(data, 0xFF, size);
		int ret = bk_otp_apb_read(item,data,size);
		BK_RAW_LOGI(NULL, "read ret = %d\r\n",ret);
		if (ret != BK_OK) {
			os_free(data);
			return;
		}
		BK_RAW_LOGI(NULL, "data in little endian:\r\n");
		for(int i = 0;i < size; i++){
			BK_RAW_LOGD(NULL, "%x ",data[i]);
			if(i % 8 == 7) BK_RAW_LOGD(NULL,"\r\n");
		}
		os_free(data);
		data = NULL;
	} else if (os_strcmp(argv[1], "write") == 0){
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		uint32_t size = os_strtoul(argv[3], NULL, 10);
		uint8_t* data = (uint8_t*)os_malloc(size*sizeof(uint8_t));
		if(data == NULL){
			BK_RAW_LOGD(NULL, "data os_malloc fail\r\n");
			return;
		}
		memset(data, 0, size);
		cli_hexstr2bin(argv[4], data, size);
		BK_RAW_LOGD(NULL, "data to be written:\r\n");
		for(int i = 0;i < size; i++){
			BK_RAW_LOGD(NULL, "%x ",data[i]);
			if(i % 8 == 7) BK_RAW_LOGD(NULL, "\r\n");
		}
		uint32_t ret = bk_otp_apb_update(item,data,size);
		memset(data, 0xFF, size);
		bk_otp_apb_read(item,data,size);
		BK_RAW_LOGD(NULL, "\r\nwrite ret = %d, after write data:\r\n",ret);
		for(int i = 0;i < size; i++){
			BK_RAW_LOGD(NULL, "%x ",data[i]);
			if(i % 8 == 7) BK_RAW_LOGD(NULL, "\r\n");
		}
		os_free(data);
		data = NULL;
	} else if (os_strcmp(argv[1], "read_mask") == 0){
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		uint32_t value;
		value = bk_otp_apb_read_mask(item);
		BK_RAW_LOGD(NULL, "permission value = %#x\r\n",value);
	} else if (os_strcmp(argv[1], "write_mask") == 0){
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		otp_privilege_t permission = os_strtoul(argv[3], NULL, 16);
		uint32_t value;
		int ret = bk_otp_apb_write_mask(item, permission);
		value = bk_otp_apb_read_permission(item);
		BK_RAW_LOGD(NULL, "permission value = %#x,ret = %d\r\n",value, ret);
	} else if (os_strcmp(argv[1], "read_otp_s") == 0) {
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		int ret = bk_otp_read_security(item);
		int flag = bk_otp_read_security_flag();
		BK_RAW_LOGI(NULL, "read otp item:%d, security:%#x,flag;%#x\r\n", item, ret, flag);
	} else if (os_strcmp(argv[1], "write_otp_s") == 0) {
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		int ret = bk_otp_read_security(item);
		BK_RAW_LOGI(NULL, "befor write,read item %d : otp_security: %x\r\n", item, ret);
		bk_otp_write_security(item);
		bk_otp_enable_security_flag();
		ret = bk_otp_read_security(item);
		BK_RAW_LOGI(NULL, "after write,read item %d : otp_security: %x\r\n", item, ret);
	} else if (os_strcmp(argv[1], "read_puf_s") == 0) {
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		int ret = bk_otp_read_puf_security(item);
		int flag = bk_otp_read_security_flag();
		BK_RAW_LOGI(NULL, "read puf item:%d, security:%#x,flag;%#x\r\n", item, ret, flag);
	} else if (os_strcmp(argv[1], "write_puf_s") == 0) {
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		int ret = bk_otp_read_puf_security(item);
		BK_RAW_LOGI(NULL, "befor write,read item %d : puf security: %x\r\n", item, ret);
		bk_otp_write_puf_security(item);
		bk_otp_enable_security_flag();
		ret = bk_otp_read_puf_security(item);
		BK_RAW_LOGI(NULL, "after write,read item %d : puf security: %x\r\n", item, ret);
	}else if (os_strcmp(argv[1], "read_permission") == 0){
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		uint32_t value;
		value = bk_otp_apb_read_permission(item);
		BK_RAW_LOGD(NULL, "permission value = %#x\r\n",value);
	} else if (os_strcmp(argv[1], "write_permission") == 0){
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		otp_privilege_t permission = os_strtoul(argv[3], NULL, 16);
		uint32_t value;
		int ret = bk_otp_apb_write_permission(item, permission);
		value = bk_otp_apb_read_permission(item);
		BK_RAW_LOGD(NULL, "permission value = %#x,ret = %d\r\n",value, ret);
	} else if (os_strcmp(argv[1], "read_random") == 0){
		uint32_t size = os_strtoul(argv[2], NULL, 10);
		uint32_t* data = (uint32_t*)os_malloc(size*sizeof(uint32_t));
		if(data == NULL){
			BK_RAW_LOGD(NULL, "data os_malloc fail\r\n");
			return;
		}
		memset(data, 0, size);
		bk_otp_read_random_number(data, size);
		for(int i = 0;i < size; i++){
			BK_RAW_LOGD(NULL, "%x\r\n",data[i]);
		}
		os_free(data);
		data = NULL;
		BK_RAW_LOGI(NULL, "\r\n");
	} else if (os_strcmp(argv[1], "pre-write") == 0){
		uint32_t size = os_strtoul(argv[2], NULL, 10);
		uint8_t* data = (uint8_t*)os_malloc(size*sizeof(uint8_t));
		if(data == NULL){
			BK_RAW_LOGD(NULL, "data os_malloc fail\r\n");
			return;
		}
		memset(data, 0, size);
		hexstr2bin(argv[3], data, size);
		BK_RAW_LOGD(NULL, "please check write data:\r\n");
		for(int i = 0;i < size; i++){
			BK_RAW_LOGD(NULL, "%x ",data[i]);
			if(i % 8 == 7) BK_RAW_LOGD(NULL, "\r\n");
		}
		os_free(data);
		data = NULL;
	} else {
		cli_otp_help();
	}
}

static void cli_otp_ahb_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	extern otp_privilege_t bk_otp_ahb_read_permission(otp2_id_t item);
	extern bk_err_t bk_otp_ahb_write_permission(otp2_id_t item, otp_privilege_t permission);
	if (argc < 2) {
		cli_otp_help();
		return;
	}

	if (os_strcmp(argv[1], "read") == 0){
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		uint32_t size = os_strtoul(argv[3], NULL, 10);
		uint8_t* data = (uint8_t*)os_malloc(size*sizeof(uint8_t));
		if(data == NULL){
			BK_RAW_LOGD(NULL, "data os_malloc fail\r\n");
			return;
		}
		memset(data, 0xFF, size);
		int ret = bk_otp_ahb_read(item,data,size);
		BK_RAW_LOGD(NULL, "read ret = %d, data in little endian:\r\n",ret);
		for(int i = 0;i < size; i++){
			BK_RAW_LOGD(NULL, "%x ",data[i]);
			if(i % 8 == 7) BK_RAW_LOGD(NULL,"\r\n");
		}
		os_free(data);
		data = NULL;
	} else if (os_strcmp(argv[1], "write") == 0){
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		uint32_t size = os_strtoul(argv[3], NULL, 10);
		uint8_t* data = (uint8_t*)os_malloc(size*sizeof(uint8_t));
		if(data == NULL){
			BK_RAW_LOGD(NULL, "data os_malloc fail\r\n");
			return;
		}
		memset(data, 0, size);
		hexstr2bin(argv[4], data, size);
		BK_RAW_LOGD(NULL, "data to be written:\r\n");
		for(int i = 0;i < size; i++){
			BK_RAW_LOGD(NULL, "%x ",data[i]);
			if(i % 8 == 7) BK_RAW_LOGD(NULL, "\r\n");
		}
		uint32_t ret = bk_otp_ahb_update(item,data,size);
		bk_otp_ahb_read(item,data,size);
		BK_RAW_LOGD(NULL, "\r\nwrite ret = %d, after write data:\r\n",ret);
		for(int i = 0;i < size; i++){
			BK_RAW_LOGD(NULL, "%x ",data[i]);
			if(i % 8 == 7) BK_RAW_LOGD(NULL, "\r\n");
		}
		os_free(data);
		data = NULL;
	} else if (os_strcmp(argv[1], "read_permission") == 0){
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		uint32_t value;
		value = bk_otp_ahb_read_permission(item);
		BK_LOGD(NULL, "permission value = %#x\r\n",value);
	} else if (os_strcmp(argv[1], "write_permission") == 0){
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		otp_privilege_t permission = os_strtoul(argv[3], NULL, 16);
		uint32_t value;
		int ret = bk_otp_ahb_write_permission(item, permission);
		value = bk_otp_ahb_read_permission(item);
		BK_LOGD(NULL, "after write permission value = %#x,ret = %d\r\n",value, ret);
	} else if (os_strcmp(argv[1], "pre-write") == 0){
		uint32_t size = os_strtoul(argv[2], NULL, 10);
		uint8_t* data = (uint8_t*)os_malloc(size*sizeof(uint8_t));
		if(data == NULL){
			BK_RAW_LOGD(NULL, "data os_malloc fail\r\n");
			return;
		}
		memset(data, 0, size);
		hexstr2bin(argv[3], data, size);
		BK_RAW_LOGD(NULL, "please check write data:\r\n");
		for(int i = 0;i < size; i++){
			BK_RAW_LOGD(NULL, "%x ",data[i]);
			if(i % 8 == 7) BK_RAW_LOGD(NULL, "\r\n");
		}
		os_free(data);
		data = NULL;
	} else {
		cli_otp_help();
	}
}
#endif
#if defined(CONFIG_TFM_OTP_NSC)
static void cli_otp_nsc_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (os_strcmp(argv[1], "read") == 0) {
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		int ret = bk_otp_read_nsc(item);
		BK_RAW_LOGD(NULL, "read ret = %d\r\n",ret);
	} else if (os_strcmp(argv[1], "apb_read") == 0) {
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		uint32_t size = os_strtoul(argv[3], NULL, 10);
		uint8_t* data = (uint8_t*)os_malloc(size*sizeof(uint8_t));
		if (data == NULL) {
			return;
		}
		memset(data, 0xFF, size);
		int ret = bk_otp_apb_read_nsc(1, item, data,size);
		BK_RAW_LOGI(NULL, "read ret = %d\r\n",ret);
		if (ret != BK_OK) {
			os_free(data);
			return;
		}
		BK_RAW_LOGI(NULL, "data in little endian:\r\n");
		for(int i = 0;i < size; i++){
			BK_RAW_LOGI(NULL, "%x ",data[i]);
			if(i % 8 == 7) BK_RAW_LOGI(NULL,"\r\n");
		}
		os_free(data);
		data = NULL;
	} else if (os_strcmp(argv[1], "read_otp_s") == 0) {
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		int ret = bk_otp_read_security_nsc(item);
		int flag = bk_otp_read_security_flag();
		BK_RAW_LOGI(NULL, "read otp item:%d, security:%#x,flag;%#x\r\n", item, ret, flag);
	} else if (os_strcmp(argv[1], "write_otp_s") == 0) {
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		int ret = bk_otp_read_security_nsc(item);
		BK_RAW_LOGI(NULL, "befor write,read item %d : otp_security: %x\r\n", item, ret);
		bk_otp_write_security_nsc(item);
		bk_otp_enable_security_flag();
		ret = bk_otp_read_security_nsc(item);
		BK_RAW_LOGI(NULL, "after write,read item %d : otp_security: %x\r\n", item, ret);
	} else if (os_strcmp(argv[1], "read_puf_s") == 0) {
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		int ret = bk_otp_read_puf_security_nsc(item);
		int flag = bk_otp_read_security_flag();
		BK_RAW_LOGI(NULL, "read otp item:%d, security:%#x,flag;%#x\r\n", item, ret, flag);
	} else if (os_strcmp(argv[1], "write_puf_s") == 0) {
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		int ret = bk_otp_read_puf_security_nsc(item);
		BK_RAW_LOGI(NULL, "befor write,read item %d : puf security: %x\r\n", item, ret);
		bk_otp_write_puf_security_nsc(item);
		bk_otp_enable_security_flag();
		ret = bk_otp_read_puf_security_nsc(item);
		BK_RAW_LOGI(NULL, "after write,read item %d : puf security: %x\r\n", item, ret);
	} else if (os_strcmp(argv[1], "write") == 0) {
		uint32_t map_id = os_strtoul(argv[2], NULL, 10);
		uint32_t item = os_strtoul(argv[3], NULL, 10);
		uint32_t size = os_strtoul(argv[4], NULL, 10);
		uint8_t* data = (uint8_t*)os_malloc(size*sizeof(uint8_t));
		if (data == NULL) {
			return;
		}
		memset(data, 0, size);
		hexstr2bin(argv[5], data, size);
		BK_RAW_LOGD(NULL, "data to be written:\r\n");
		for(int i = 0;i < size; i++){
			BK_RAW_LOGD(NULL, "%x ",data[i]);
			if(i % 8 == 7) BK_RAW_LOGD(NULL, "\r\n");
		}
		int ret = bk_otp_update_nsc(map_id,item,data,size);
		BK_RAW_LOGD(NULL, "write ret = %d\r\n",ret);
		os_free(data);
		data = NULL;
	} else if (os_strcmp(argv[1], "read_permission") == 0) {
		uint32_t map_id = os_strtoul(argv[2], NULL, 10);
		uint32_t item = os_strtoul(argv[3], NULL, 10);
		uint32_t permission;
		int ret = bk_otp_read_permission_nsc(map_id, item, &permission);
		BK_RAW_LOGD(NULL, "read ret = %d, permission = %#x\r\n",ret, permission);
	} else if (os_strcmp(argv[1], "write_permission") == 0) {
		uint32_t map_id = os_strtoul(argv[2], NULL, 10);
		uint32_t item = os_strtoul(argv[3], NULL, 10);
		uint32_t permission = os_strtoul(argv[4], NULL, 16);
		int ret = bk_otp_write_permission_nsc(map_id, item, permission);
		bk_otp_read_permission_nsc(map_id, item, &permission);
		BK_RAW_LOGD(NULL, "write ret = %d, permission = %#x\r\n",ret, permission);
	} else if (os_strcmp(argv[1], "write_mask") == 0) {
		uint32_t map_id = os_strtoul(argv[2], NULL, 10);
		uint32_t item = os_strtoul(argv[3], NULL, 10);
		uint32_t permission = os_strtoul(argv[4], NULL, 16);
		int ret = bk_otp_write_mask_nsc(map_id, item, permission);
		bk_otp_read_permission_nsc(map_id, item, &permission);
		BK_RAW_LOGI(NULL, "write ret = %d, permission = %#x\r\n",ret, permission);
	} else {
		cli_otp_help();
	}
}
#endif

#if defined(CONFIG_FLASH_OTP)
static void cli_flash_otp_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (os_strcmp(argv[1], "init") == 0) {
		uint8_t section_id = os_strtoul(argv[2], NULL, 10);
		bk_flash_otp_init(section_id);
	} else if (os_strcmp(argv[1], "read") == 0) {
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		uint32_t size = os_strtoul(argv[3], NULL, 10);
		uint8_t* data = (uint8_t*)os_malloc(size*sizeof(uint8_t));
		if (data == NULL) {
			return;
		}
		memset(data, 0xFF, size);
		int ret = bk_flash_otp_read(item,data,size);
		BK_RAW_LOGD(NULL, "read ret = %d, data in little endian:\r\n",ret);
		for(int i = 0;i < size; i++){
			BK_RAW_LOGD(NULL, "%x ",data[i]);
			if(i % 8 == 7) BK_RAW_LOGD(NULL,"\r\n");
		}
		os_free(data);
		data = NULL;
	} else if (os_strcmp(argv[1], "write") == 0){
		uint32_t item = os_strtoul(argv[2], NULL, 10);
		uint32_t size = os_strtoul(argv[3], NULL, 10);
		uint8_t* data = (uint8_t*)os_malloc(size*sizeof(uint8_t));
		if (data == NULL) {
			return;
		}
		memset(data, 0xFF, size);
		hexstr2bin(argv[4], data, size);
		BK_RAW_LOGD(NULL, "data to be written:\r\n");
		for(int i = 0;i < size; i++){
			BK_RAW_LOGD(NULL, "%x ",data[i]);
			if(i % 8 == 7) BK_RAW_LOGD(NULL, "\r\n");
		}
		uint32_t ret = bk_flash_otp_update(item,data,size);
		bk_flash_otp_read(item,data,size);
		BK_RAW_LOGD(NULL, "\r\nwrite ret = %d, after write data:\r\n",ret);
		for(int i = 0;i < size; i++){
			BK_RAW_LOGD(NULL, "%x ",data[i]);
			if(i % 8 == 7) BK_RAW_LOGD(NULL, "\r\n");
		}
		os_free(data);
		data = NULL;
	} else if (os_strcmp(argv[1], "lock") == 0) {
		uint8_t section_id = os_strtoul(argv[2], NULL, 10);
		extern void bk_flash_lock_otp(uint8_t block_index);
		bk_flash_lock_otp(section_id);
	}
}
#endif

#define OTP_CMD_CNT (sizeof(s_otp_commands) / sizeof(struct cli_command))
static const struct cli_command s_otp_commands[] = {
#if defined(CONFIG_OTP)
	{"otp_driver", "otp driver {init/deinit}", cli_otp_driver_cmd},
	{"otp_apb", "otp_apb {read}", cli_otp_apb_cmd},
	{"otp_ahb", "otp_ahb {read}", cli_otp_ahb_cmd},
#endif
#if defined(CONFIG_TFM_OTP_NSC)
	{"otp_nsc","otp_nsc {read}", cli_otp_nsc_cmd},
#endif
#if defined(CONFIG_FLASH_OTP)
	{"flash_otp", "otp {read}", cli_flash_otp_cmd},
#endif
};

int cli_otp_init(void)
{
	return cli_register_commands(s_otp_commands, OTP_CMD_CNT);
}
