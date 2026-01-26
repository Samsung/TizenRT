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
#include <common/bk_include.h>
#include <os/mem.h>
#include "otp_driver.h"
#include "otp_hal.h"
#include "CheckSumUtils.h"
#include "_otp.c"
#if defined(CONFIG_TFM_OTP_NSC)
#include "tfm_otp_nsc.h"
#endif
typedef struct {
	otp_hal_t hal;
} otp_driver_t;

/*the smallest unit length for configuration*/

/*              permission       secure_range             mask         */
/*    otp1        4 Byte            256 Byte             32 Byte       */
/*    otp2        128 Byte           NA                  128 Byte      */
/*    puf         4   Byte           32 Byte             32 Byte       */

static otp_driver_t s_otp = {0};
static bool s_otp_driver_is_init = false;
typedef uint32_t (*otp_read_ptr)(uint32_t);
typedef void (*otp_write_ptr)(uint32_t, uint32_t);

static bk_lock_t* otp_lock = NULL;

// TODO: Move the following code to a higher-level implementation
bk_lock_t* lock_init()
{
	bk_lock_t* bk_lock = (bk_lock_t*)os_malloc(sizeof(bk_lock_t));
	if (bk_lock == NULL) {
		return NULL;
	}
	rtos_init_mutex(&bk_lock->mutex);
	bk_lock->is_rtos_disable = false;
	bk_lock->int_level = 0;
	return bk_lock;
}

void lock_deinit(bk_lock_t* bk_lock)
{
	if (bk_lock != NULL) {
		os_free(bk_lock);
	}
}

void lock(bk_lock_t* bk_lock)
{
//	rtos_lock_mutex(&bk_lock->mutex);
	if (bk_lock->is_rtos_disable) {
		bk_lock->int_level = rtos_disable_int();
	}
}

void unlock(bk_lock_t* bk_lock)
{
//	rtos_unlock_mutex(&bk_lock->mutex);
	if (bk_lock->is_rtos_disable) {
		rtos_enable_int(bk_lock->int_level);
	}
}
//TODO end

bk_err_t bk_otp_driver_init(void)
{
	if(s_otp_driver_is_init) {
		return BK_OK;
	}
	otp_lock = lock_init();
	if (otp_lock == NULL) {
		return BK_FAIL;
	}
	s_otp.hal.hw = (otp_hw_t *)OTP_LL_REG_BASE();
	s_otp.hal.hw2 = (otp2_hw_t *)OTP2_LL_REG_BASE();

	s_otp_driver_is_init = true;

	return BK_OK;
}

void bk_otp_driver_deinit(void)
{
	s_otp.hal.hw = NULL;
	s_otp.hal.hw2 = NULL;
	lock_deinit(otp_lock);

	s_otp_driver_is_init = false;
	return;
}

static void otp_sleep()
{
#if defined(CONFIG_ATE_TEST)
	return ;
#endif
	otp_hal_power_off(&s_otp.hal);
}

static int otp_active()
{
#if defined(CONFIG_ATE_TEST)
	return 0;
#endif
	return otp_hal_power_on(&s_otp.hal);
}

int bk_otp_active(void)
{
	return otp_active();
}

int bk_otp_sleep(void)
{
	otp_sleep();
	return 0;
}

static int switch_map(uint8_t map_id)
{
	if(otp_map == NULL){
		if (map_id == 1) {
			otp_map = otp_map_1;
		} else if (map_id == 2) {
			otp_map = otp_map_2;
		} else {
			return -1;
		}
	} else {
		return -1;
	}
	return 0;
}

#define OTP_ACTIVE(map_id) \
	do { \
		lock(otp_lock);\
		int ret = switch_map(map_id); \
		if(ret != 0) { \
			return ret;\
		} \
		ret = otp_active(); \
		if(ret != 0) { \
			otp_sleep(); \
			otp_map = NULL; \
			return -1;\
		} \
	} while(0);

#define OTP_SLEEP() \
	do { \
		otp_sleep(); \
		otp_map = NULL; \
		unlock(otp_lock); \
	} while(0);

static uint32_t _otp_read_otp(uint32_t location)
{
	return otp_hal_read_otp(&s_otp.hal,location);
}
static uint32_t _otp2_read_otp(uint32_t location)
{
	return otp2_hal_read_otp(&s_otp.hal,location);
}

static void _otp2_write_otp(uint32_t location,uint32_t value)
{
	otp2_hal_write_otp(&s_otp.hal, location, value);
}

static void _otp_write_otp(uint32_t location,uint32_t value)
{
	otp_hal_write_otp(&s_otp.hal,location,value);
}

static uint32_t otp_read_status()
{
	return otp_hal_read_status(&s_otp.hal);
}

static uint32_t otp_get_failcnt()
{
	return otp_hal_get_failcnt(&s_otp.hal);
}

uint32_t otp_read_otp2_permission(uint32_t location)
{
	return otp_hal_read_otp2_permission(&s_otp.hal,location);
}

static void otp_set_lck_mask(uint32_t location)
{
	otp_hal_enable_mask_lck(&s_otp.hal);
}
static uint32_t otp_read_otp_mask(uint32_t location)
{
	return otp_hal_read_otp_mask(&s_otp.hal,location);
}
static void otp_zeroize_otp(uint32_t location)
{
	otp_hal_zeroize_otp(&s_otp.hal,location);
}
static void otp_zeroize_puf(uint32_t location)
{
	otp_hal_zeroize_puf(&s_otp.hal,location);
}
static uint32_t otp_get_otp_zeroized_flag(uint32_t location)
{
	return otp_hal_read_otp_zeroized_flag(&s_otp.hal,location);
}

static bool otp_check_crc8(uint8_t* buf, size_t buflen)
{
	CRC8_Context crc8;
	uint8_t cal_value;
	uint8_t store_value = buf[buflen - 1];
	CRC8_Init(&crc8);
	CRC8_Update(&crc8, buf, buflen - 1);
	CRC8_Final(&crc8, &cal_value);
	if (cal_value == store_value) {
		return true;
	} else {
		return false;
	}
}

static void otp_fill_crc8(uint8_t* buf, size_t buflen)
{
	CRC8_Context crc8;
	uint8_t fill_value;
	CRC8_Init(&crc8);
	CRC8_Update(&crc8, buf, buflen - 1);
	CRC8_Final(&crc8, &fill_value);
	buf[buflen - 1] = fill_value;
}

otp_privilege_t bk_otp_apb_read_permission(otp1_id_t item)
{
	OTP_ACTIVE(1)
	uint32_t location = otp_map[item].offset / 4;
	otp_privilege_t permission = otp_hal_read_otp_permission(&s_otp.hal,location);
	uint32_t mask = otp_hal_read_otp_mask(&s_otp.hal,location);
	otp_privilege_t ret_permission;
	if (mask != 0) {
		ret_permission =  OTP_NO_ACCESS;
	} else {
		ret_permission = permission;
	}

	OTP_SLEEP()
	return ret_permission;
}

otp_privilege_t bk_otp_ahb_read_permission(otp2_id_t item)
{
	OTP_ACTIVE(2)
	uint32_t location = otp_map[item].offset / 4;
	otp_privilege_t permission = otp_hal_read_otp2_permission(&s_otp.hal,location);
	uint32_t mask = otp_hal_read_otp2_mask(&s_otp.hal,location);
	otp_privilege_t ret_permission;
	if (mask != 0) {
		ret_permission =  OTP_NO_ACCESS;
	} else {
		ret_permission = permission;
	}

	OTP_SLEEP()
	return ret_permission;
}


bk_err_t bk_otp_ahb_write_permission(otp2_id_t item, otp_privilege_t permission)
{
	if (item >= OTP2_MAX_ID) {
		return BK_ERR_OTP_INDEX_WRONG;
	}
	if (permission != OTP_READ_ONLY) {
		return BK_ERR_OTP_PERMISSION_WRONG;
	}
	OTP_ACTIVE(2)
	uint32_t location = otp_map[item].offset / 4;
	int32_t size = otp_map[item].allocated_size;
	while(size > 0) {
		otp_hal_write_otp2_permission(&s_otp.hal, location, permission);
		location++;
		size-=4;
	}
	OTP_SLEEP()
	return BK_OK;
}

bk_err_t bk_otp_apb_write_permission(otp1_id_t item, otp_privilege_t permission)
{
	if (item >= OTP1_MAX_ID) {
		return BK_ERR_OTP_INDEX_WRONG;
	}
	if(permission != OTP_READ_ONLY && permission != OTP_NO_ACCESS){
		return BK_ERR_OTP_PERMISSION_WRONG;
	}
	OTP_ACTIVE(1)
	uint32_t location = otp_map[item].offset / 4;
	int32_t size = otp_map[item].allocated_size;
	while(size > 0) {
		otp_hal_write_otp_permission(&s_otp.hal, location, permission);
		location++;
		size-=4;
	}
	OTP_SLEEP()
	return BK_OK;
}

otp_privilege_t bk_otp_apb_read_mask(otp1_id_t item)
{
	OTP_ACTIVE(1)
	uint32_t location = otp_map[item].offset / 4;
	otp_privilege_t mask = otp_hal_read_otp_mask(&s_otp.hal,location);
	OTP_SLEEP()
	return mask;
}

bk_err_t bk_otp_apb_write_mask(otp1_id_t item, uint32_t mask)
{
	if (item >= OTP1_MAX_ID) {
		return BK_ERR_OTP_INDEX_WRONG;
	}
	OTP_ACTIVE(1)
	uint32_t location = otp_map[item].offset / 4;
	int32_t size = otp_map[item].allocated_size;
	while(size > 0) {
		otp_hal_write_otp_mask(&s_otp.hal, location, mask);
		location++;
		size-=4;
	}
	OTP_SLEEP()
	return BK_OK;
}

otp_privilege_t bk_otp_ahb_read_mask(otp2_id_t item)
{
	OTP_ACTIVE(2)
	uint32_t location = otp_map[item].offset / 4;
	otp_privilege_t mask = otp_hal_read_otp2_mask(&s_otp.hal,location);
	OTP_SLEEP()
	return mask;
}

bk_err_t bk_otp_ahb_write_mask(otp2_id_t item, uint32_t mask)
{
	if (item >= OTP2_MAX_ID) {
		return BK_ERR_OTP_INDEX_WRONG;
	}
	OTP_ACTIVE(2)
	uint32_t location = otp_map[item].offset / 4;
	int32_t size = otp_map[item].allocated_size;
	while(size > 0) {
		otp_hal_write_otp2_mask(&s_otp.hal, location, mask);
		location++;
		size-=4;
	}
	OTP_SLEEP()
	return BK_OK;
}

bk_err_t bk_otp_write_security(uint32_t item)
{
	OTP_ACTIVE(1);
	uint32_t location = otp_map[item].offset / 4;
	otp_hal_write_otp_security(&s_otp.hal, location);
	OTP_SLEEP();
	return BK_OK;
}

uint32_t bk_otp_read_security(uint32_t item)
{
	OTP_ACTIVE(1);
	uint32_t location = otp_map[item].offset / 4;
	uint32_t security_value = otp_hal_read_otp_security(&s_otp.hal, location);
	OTP_SLEEP();

	return security_value;
}

bk_err_t bk_otp_write_puf_security(uint32_t item)
{
	OTP_ACTIVE(1);
	uint32_t location = otp_map[item].offset / 4;
	otp_hal_write_puf_security(&s_otp.hal, location);
	OTP_SLEEP();
	return BK_OK;
}

uint32_t bk_otp_read_puf_security(uint32_t item)
{
	OTP_ACTIVE(1);
	uint32_t location = otp_map[item].offset / 4;
	uint32_t security_value = otp_hal_read_puf_security(&s_otp.hal, location);
	OTP_SLEEP();
	return security_value;
}

bk_err_t bk_otp_enable_security_flag(void)
{
	OTP_ACTIVE(1);
	otp_hal_enable_security_protection(&s_otp.hal);
	OTP_SLEEP();
	return BK_OK;
}

bk_err_t bk_otp_read_security_flag(void)
{
	OTP_ACTIVE(1);
	uint32_t flag = otp_hal_read_security_protection(&s_otp.hal);
	OTP_SLEEP();
	return flag;
}

static bk_err_t otp_read(uint8_t map_id, uint8_t item, uint8_t* buf, uint32_t size)
{
	bk_err_t ret = BK_OK;
	OTP_ACTIVE(map_id)

	uint8_t* read_buf = NULL;
	uint8_t* p_read_buf = NULL;
	uint32_t location = otp_map[item].offset / 4;
	uint32_t start = otp_map[item].offset % 4;
	uint32_t value;
	uint32_t read_size;
	uint32_t read_buf_len;
	otp_read_ptr read_fun[] = {_otp_read_otp,_otp2_read_otp};

	if (size > otp_map[item].allocated_size) {
		ret =  BK_ERR_OTP_ADDR_OUT_OF_RANGE;
		goto end;
	}

	if (otp_map[item].crc_en == OTP_NEED_CRC) {
		read_size = otp_map[item].allocated_size;
	} else if (otp_map[item].crc_en == OTP_NO_NEED_CRC) {
		read_size = size;
	} else {
		ret = BK_ERR_OTP_CRC_WRONG;
		goto end;
	}
	if (read_size == 0) {
		ret = BK_ERR_OTP_INDEX_WRONG;
		goto end;
	}
	read_buf_len = read_size;
	read_buf = (uint8_t*)os_zalloc(read_buf_len);
	if (read_buf == NULL) {
		ret = BK_ERR_NO_MEM;
		goto end;
	}
	p_read_buf = read_buf;

	while (read_size > 0) {
		value = read_fun[map_id-1](location);

		uint8_t * src_data = (uint8_t *)&value;
		int       cpy_cnt;

		src_data += start;

		cpy_cnt = (read_size >= (4 - start)) ? (4 - start) : read_size;

		switch ( cpy_cnt ) {
			case 4:
				*p_read_buf++ = *src_data++;
			case 3:
				*p_read_buf++ = *src_data++;
			case 2:
				*p_read_buf++ = *src_data++;
			case 1:
				*p_read_buf++ = *src_data++;
		}

		read_size -= cpy_cnt;
		location++;
		start = 0;
	}

	if (otp_map[item].crc_en == OTP_NEED_CRC) {
		uint8_t* zero_buf = os_zalloc(read_buf_len);
		if (os_memcmp(read_buf, zero_buf, read_buf_len) != 0){
			if (otp_check_crc8(read_buf, read_buf_len) == false) {
				os_free(zero_buf);
				ret = BK_ERR_OTP_CRC_WRONG;
				goto end;
			}
		}
		os_free(zero_buf);
	}

	os_memcpy(buf, read_buf, size);
end:
	if (read_buf != NULL) {
		os_free(read_buf);
	}
	OTP_SLEEP()
	return ret;
}

static bk_err_t otp_update(uint8_t map_id, uint8_t item, uint8_t* buf, uint32_t size)
{
	bk_err_t ret = BK_OK;
	OTP_ACTIVE(map_id)

	if (size > otp_map[item].allocated_size || size == 0) {
		ret = BK_ERR_OTP_ADDR_OUT_OF_RANGE;
		goto end;
	}
	if (otp_map[item].crc_en == OTP_NEED_CRC) {
		if (size != otp_map[item].allocated_size && buf[size - 1] != 0) {
			ret = BK_ERR_OTP_CRC_WRONG;
			goto end;
		}
		otp_fill_crc8(buf, otp_map[item].allocated_size);
	}
	uint32_t location = otp_map[item].offset / 4;
	uint32_t start = otp_map[item].offset % 4;
	uint32_t value = 0;
	uint32_t check_value = 0;
	otp_read_ptr read_fun[] = {_otp_read_otp,_otp2_read_otp};
	otp_write_ptr write_fun[] = {_otp_write_otp,_otp2_write_otp};

	uint8_t * dst_data;
	uint8_t * back_buf = buf;
	uint32_t  back_size = size;

	while(size > 0) {
		value = read_fun[map_id-1](location);

		int       cmp_cnt = (size >= (4 - start)) ? (4 - start) : size;
		uint32_t  cmp_result = 0;

		dst_data = (uint8_t *)&value;
		dst_data += start;

		/* the initial data of every OTP memory bit is 0. */
		/* OTP memory bit can be changed to 1, */
		/* once it is changed to 1, it can't be reset to 0. */
		/* so check the target bit value before write to OTP memory. */
		/* if try to change OTP bit from 1 to 0, return fail. */
		switch( cmp_cnt ) {
			case 4:
				cmp_result |= (~(*buf) & (*dst_data));
				buf++; dst_data++;
			case 3:
				cmp_result |= (~(*buf) & (*dst_data));
				buf++; dst_data++;
			case 2:
				cmp_result |= (~(*buf) & (*dst_data));
				buf++; dst_data++;
			case 1:
				cmp_result |= (~(*buf) & (*dst_data));
				buf++; dst_data++;
		}

		if(cmp_result != 0)
			break;

		size -= cmp_cnt;
		location++;
		start = 0;
	}

	if(size > 0) {
		ret = BK_ERR_OTP_UPDATE_NOT_EQUAL;
		goto end;
	}

	/* restore all variables. */
	location = otp_map[item].offset / 4;
	start = otp_map[item].offset % 4;
	size = back_size;
	buf = back_buf;

	while(size > 0) {
		int cpy_cnt = (size >= (4 - start)) ? (4 - start) : size;

		value = read_fun[map_id-1](location);
		dst_data = (uint8_t *)&value;
		dst_data += start;

		switch( cpy_cnt ) {
			case 4:
				*dst_data++ = *buf++;
			case 3:
				*dst_data++ = *buf++;
			case 2:
				*dst_data++ = *buf++;
			case 1:
				*dst_data++ = *buf++;
		}

		write_fun[map_id-1](location, value);
		check_value = read_fun[map_id-1](location);
		if (check_value != value) {
			ret = BK_ERR_OTP_UPDATE_NOT_EQUAL;
			goto end;
		}

		size -= cpy_cnt;
		location++;
		start = 0;
	}

end:
	OTP_SLEEP()
	return ret;
}

/**
 * obtain APB OTP value in little endian with item ID:
 * 1. allowed start address of item not aligned
 * 2. allowed end address of item not aligned
 */
bk_err_t bk_otp_apb_read(otp1_id_t item, uint8_t* buf, uint32_t size)
{
	if (item >= OTP1_MAX_ID) {
		return BK_ERR_OTP_INDEX_WRONG;
	}

	if (bk_otp_apb_read_permission(item) > OTP_READ_ONLY ) {
		return BK_ERR_NO_READ_PERMISSION;
	}
	return otp_read(1, item, buf, size);
}

/**
 * update APB OTP value in little endian with item ID:
 * 1. allowed start address of item not aligned
 * 2. allowed end address of item not aligned
 * 3. check overwritable before write, return BK_ERR_OTP_CANNOT_WRTIE if failed
 * 4. verify value after write, return BK_ERR_OTP_UPDATE_NOT_EQUAL if failed
 */
bk_err_t bk_otp_apb_update(otp1_id_t item, uint8_t* buf, uint32_t size)
{
	if (item >= OTP1_MAX_ID) {
		return BK_ERR_OTP_INDEX_WRONG;
	}
	if( bk_otp_apb_read_permission(item) != OTP_READ_WRITE ) {
		return BK_ERR_NO_WRITE_PERMISSION;
	}
	return otp_update(1, item, buf, size);
}

/**
 * obtain AHB OTP value with item ID:
 * 1. allowed start address of item not aligned
 * 2. allowed end address of item not aligned
 */
bk_err_t bk_otp_ahb_read(otp2_id_t item, uint8_t* buf, uint32_t size)
{
	if (item >= OTP2_MAX_ID) {
		return BK_ERR_OTP_INDEX_WRONG;
	}
	if( bk_otp_ahb_read_permission(item) > OTP_READ_ONLY ) {
		return BK_ERR_NO_READ_PERMISSION;
	}
	return otp_read(2, item, buf, size);
}

bk_err_t bk_otp_ahb_update(otp2_id_t item, uint8_t* buf, uint32_t size)
{
	if (item >= OTP2_MAX_ID) {
		return BK_ERR_OTP_INDEX_WRONG;
	}
	if( bk_otp_ahb_read_permission(item) != OTP_READ_WRITE ) {
		return BK_ERR_NO_WRITE_PERMISSION;
	}
	return otp_update(2, item, buf, size);

}

bk_err_t bk_otp_read_random_number(uint8* value, uint32_t size)
{
	OTP_ACTIVE(1)
	uint32_t word_len = size / 4;
	uint32_t byte_remain = size % 4;
	uint32_t value_word = 0;

	for(int i = 0; i < word_len; ++i){
		*(uint32_t*)value = otp_hal_read_random_number(&s_otp.hal);
		value += 4;
	}

	if(byte_remain > 0){
		value_word = otp_hal_read_random_number(&s_otp.hal);
		for(int i = 0; i < byte_remain; ++i){
			value[i] = (value_word >> (i * 8)) & 0xFF;
		}
	}

	OTP_SLEEP()
	return BK_OK;
}

/*Initial Phase,done at factory*/
#if defined(CONFIG_ATE_TEST)
static bk_err_t bk_otp_init_puf()
{
	if(otp_hal_read_enroll(&s_otp.hal) == 0xF){
		return BK_OK;
	} else if(otp_hal_read_enroll(&s_otp.hal) == 0x0){
		otp_hal_do_puf_enroll(&s_otp.hal);
	}
	uint32_t flag = otp_hal_read_status(&s_otp.hal);
	if(flag & OTP_ERROR){
		OTP_LOGE("puf enrollment fail.\r\n");
		return BK_ERR_OTP_OPERATION_ERROR;
	} else if(flag & OTP_WARNING) {
		OTP_LOGW("puf enrollment has been executed.\r\n");
	} else if(flag & OTP_WRONG) {
		OTP_LOGE("PTM data is not correctly set.\r\n");
		return BK_ERR_OTP_OPERATION_WRONG;
	} else if(flag & OTP_FORBID) {
		OTP_LOGE("TMlck is locked.\r\n");
		return BK_ERR_OTP_OPERATION_FORBID;
	}
	if(otp_hal_read_enroll(&s_otp.hal) != 0xF){
		OTP_LOGE("Error code:0x%x\r\n",otp_hal_read_status(&s_otp.hal));
		return BK_ERR_OTP_OPERATION_FAIL;
	}
	return BK_OK;
}

bk_err_t bk_otp_read_raw_entropy_output(uint32_t* value)
{
	otp_hal_set_fre_cont(&s_otp.hal, 1);
	*value = otp_hal_read_random_number(&s_otp.hal);
	otp_hal_set_fre_cont(&s_otp.hal, 0);
	return BK_OK;
}

static bk_err_t bk_otp_check_puf_quality()
{
	uint32_t flag = otp_hal_do_puf_quality_check(&s_otp.hal);
	if(flag & OTP_ERROR){
		OTP_LOGE("quality check fail.\r\n");
		return BK_ERR_OTP_OPERATION_ERROR;
	} else if(flag & OTP_WARNING) {
		OTP_LOGE("please init puf before quality check.\r\n");
		return BK_ERR_OTP_OPERATION_WARNING;
	} else if(flag & OTP_WRONG) {
		OTP_LOGE("PTM data is not correctly set.\r\n");
		return BK_ERR_OTP_OPERATION_WRONG;
	}
	return BK_OK;
}

static bk_err_t bk_otp_check_puf_health()
{
	uint32_t flag = otp_hal_do_puf_health_check(&s_otp.hal);
	while(flag & OTP_BUSY);
	flag = otp_hal_read_status(&s_otp.hal);
	if(flag & OTP_ERROR){
		OTP_LOGE("health check fail.\r\n");
		return BK_ERR_OTP_OPERATION_ERROR;
	} else if(flag & OTP_WARNING) {
		OTP_LOGE("please init puf before quality check.\r\n");
		return BK_ERR_OTP_OPERATION_WARNING;
	} else if(flag & OTP_WRONG) {
		OTP_LOGE("PTM data is not correctly set.\r\n");
		return BK_ERR_OTP_OPERATION_WRONG;
	}
	return BK_OK;
}

static bk_err_t bk_otp_do_auto_repair()
{
	uint32_t status = otp_hal_do_auto_repair(&s_otp.hal);
	uint32_t flag = status & 0x1F;
	uint32_t failcnt = (status & 0xF00) >> 8;

	if(failcnt != 0){
		OTP_LOGE("The total number of failed address is 0x%x.\r\n",failcnt);
	}
	if(failcnt > 4){
		return failcnt;
	}
	if(flag & OTP_ERROR){
		OTP_LOGE("auto repair fail.\r\n");
		return BK_ERR_OTP_OPERATION_ERROR;
	} else if(flag & OTP_WARNING) {
		OTP_LOGW("auto repair has been previously executed.\r\n");
	} else if(flag & OTP_WRONG) {
		OTP_LOGE("PTM data is not correctly set.\r\n");
		return BK_ERR_OTP_OPERATION_WRONG;
	} else if(flag & OTP_FORBID) {
		OTP_LOGE("TMLCK is locked.\r\n");
		return BK_ERR_OTP_OPERATION_FORBID;
	}
	return BK_OK;
}

static bk_err_t bk_otp_enable_shuffle_read()
{
	otp_hal_set_flag(&s_otp.hal,0x99);
	uint32_t flag = otp_hal_read_status(&s_otp.hal);
	if(flag & OTP_ERROR){
		OTP_LOGE("enable shuffle read fail.\r\n");
		return BK_ERR_OTP_OPERATION_ERROR;
	} else if(flag & OTP_WARNING) {
		OTP_LOGW("shuffle read has been re-programmed.\r\n");
	} else if(flag & OTP_WRONG) {
		OTP_LOGE("PTM data is not correctly set.\r\n");
		return BK_ERR_OTP_OPERATION_WRONG;
	} else if(flag & OTP_FORBID) {
		OTP_LOGE("TMLCK is locked.\r\n");
		return BK_ERR_OTP_OPERATION_FORBID;
	}
	volatile int delay = 1000;
	while(otp_hal_read_shfren(&s_otp.hal) != 0xF && delay){
		--delay;
	}
	otp_hal_power_off(&s_otp.hal);
	if(otp_hal_power_on(&s_otp.hal) != BK_OK){
		return BK_ERR_OTP_INIT_FAIL;
	}
	while(otp_hal_check_busy(&s_otp.hal));
	return BK_OK;
}

static bk_err_t bk_otp_enable_shuffle_write()
{
	otp_hal_set_flag(&s_otp.hal,0xc2);
	uint32_t flag = otp_hal_read_status(&s_otp.hal);
	while(flag & OTP_BUSY);
	flag = otp_hal_read_status(&s_otp.hal);
	if(flag & OTP_ERROR){
		OTP_LOGE("enable shuffle write fail.\r\n");
		return BK_ERR_OTP_OPERATION_ERROR;
	} else if(flag & OTP_WARNING) {
		OTP_LOGW("shuffle write has been re-programmed.\r\n");
	} else if(flag & OTP_WRONG) {
		OTP_LOGE("PTM data is not correctly set.\r\n");
		return BK_ERR_OTP_OPERATION_WRONG;
	} else if(flag & OTP_FORBID) {
		OTP_LOGE("TMLCK is locked.\r\n");
		return BK_ERR_OTP_OPERATION_FORBID;
	}
	volatile int delay = 1000;
	while(otp_hal_read_shfwen(&s_otp.hal) != 0xF && delay){
		--delay;
	}
	otp_hal_power_off(&s_otp.hal);
	if(otp_hal_power_on(&s_otp.hal) != BK_OK){
		return BK_ERR_OTP_INIT_FAIL;
	}
	while(otp_hal_check_busy(&s_otp.hal));
	return BK_OK;
}

static bk_err_t bk_otp_set_ptc_page(uint32_t page)
{
	otp_hal_set_ptc_page(&s_otp.hal,page);
	return BK_OK;
}

static bk_err_t bk_otp_test_page(uint32_t start, uint32_t end)
{
	uint32_t data = 0x5A5A5A5A;
	uint8_t bit = 0x1;
	for(uint32_t i = start;i < end;i++){
		otp_hal_write_test_row(&s_otp.hal,i,data);
		if(otp_hal_read_test_row(&s_otp.hal,i) != data){
			OTP_LOGE("test row %d = 0x%x,should be 0x%x.\r\n",i,otp_hal_read_test_row(&s_otp.hal,i),data);
			return BK_ERR_OTP_OPERATION_ERROR;
		}
		for(uint32_t j = 0; j < 4;j++){
			otp_hal_set_ptc_page(&s_otp.hal,j);
			otp_hal_write_test_column(&s_otp.hal,i,bit);
			if(otp_hal_read_test_column(&s_otp.hal,i) != bit){
				OTP_LOGE("test column %d  in page %d = 0x%x,should be 0x%x.\r\n",i,otp_hal_read_test_column(&s_otp.hal,i),bit);
				return BK_ERR_OTP_OPERATION_ERROR;
			}
		}
	}
	return BK_OK;
}

static bk_err_t bk_otp_off_margin_read()
{
	otp_hal_enable_off_margin_read(&s_otp.hal);
	if(bk_otp_test_page(8,12) == BK_OK){
		otp_hal_disable_off_margin_read(&s_otp.hal);
		return BK_OK;
	}
	OTP_LOGE("off margin read fail.\r\n");
	return BK_ERR_OTP_OPERATION_ERROR;
}

static bk_err_t bk_otp_on_margin_read()
{
	otp_hal_enable_on_margin_read(&s_otp.hal);
	if(bk_otp_test_page(12,16) == BK_OK){
		otp_hal_disable_on_margin_read(&s_otp.hal);
		return BK_OK;
	}
	OTP_LOGE("on margin read fail.\r\n");
	return BK_ERR_OTP_OPERATION_ERROR;
}

static bk_err_t bk_otp_initial_off_check()
{
	uint32_t flag = otp_hal_init_off_check(&s_otp.hal);
	if(flag & OTP_ERROR){
		OTP_LOGE("initial_off_check fail.\r\n");
		return BK_ERR_OTP_OPERATION_ERROR;
	} else if(flag & OTP_WRONG) {
		OTP_LOGE("PTM data is not correctly set.\r\n");
		return BK_ERR_OTP_OPERATION_WRONG;
	} else if(flag & OTP_FORBID) {
		OTP_LOGE("TMLCK is locked.\r\n");
		return BK_ERR_OTP_OPERATION_FORBID;
	}
	return BK_OK;
}

static bk_err_t bk_otp_enable_program_protect_function()
{
	if(otp_hal_read_pgmprt(&s_otp.hal) != 0xF){
		return BK_OK;
	}
	otp_hal_set_flag(&s_otp.hal,0xb6);
	uint32_t flag = otp_hal_read_status(&s_otp.hal);
	if(flag & OTP_ERROR){
		OTP_LOGE("program_protect fail.\r\n");
		return BK_ERR_OTP_OPERATION_ERROR;
	} else if(flag & OTP_WARNING) {
		OTP_LOGW("program_protect has been previously executed.\r\n");
	} else if(flag & OTP_WRONG) {
		OTP_LOGW("PTM data is not correctly set.\r\n");
		return BK_OK;
	} else if(flag & OTP_FORBID) {
		OTP_LOGE("TMLCK is locked.\r\n");
		return BK_ERR_OTP_OPERATION_FORBID;
	}
	if(otp_hal_read_pgmprt(&s_otp.hal) != 0xF){
		OTP_LOGE("program_protect fail.\r\n");
		return BK_ERR_OTP_OPERATION_FAIL;
	}
	return BK_OK;
}

static bk_err_t bk_otp_enable_test_mode_lock_function()
{
	if(otp_hal_read_tmlck(&s_otp.hal) == 0xF){
		return BK_OK;
	}
	otp_hal_set_flag(&s_otp.hal,0x71);
	uint32_t flag = otp_hal_read_status(&s_otp.hal);
	if(flag & OTP_ERROR){
		OTP_LOGE("test_mode_lock fail.\r\n");
		return BK_ERR_OTP_OPERATION_ERROR;
	} else if(flag & OTP_WARNING) {
		OTP_LOGW("test_mode_lock has been previously executed.\r\n");
	} else if(flag & OTP_WRONG) {
		OTP_LOGE("PTM data is not correctly set.\r\n");
		return BK_ERR_OTP_OPERATION_WRONG;
	} else if(flag & OTP_FORBID) {
		OTP_LOGE("TMLCK is locked.\r\n");
		return BK_ERR_OTP_OPERATION_FORBID;
	}
	if(otp_hal_read_tmlck(&s_otp.hal) != 0xF){
		OTP_LOGE("test_mode_lock fail.\r\n");
		return BK_ERR_OTP_OPERATION_FAIL;
	}
	return BK_OK;
}

uint32_t bk_otp_fully_flow_test()
{
	OTP_ACTIVE(1)
	uint32_t ret;
	uint32_t retry_count = 10;
	uint32_t fail_code = 0;
	/*test mode lck means test has been success*/
	if(otp_hal_read_tmlck(&s_otp.hal) == 0xF){
		return BK_OK;
	}

	ret = bk_otp_init_puf();
	if(ret != BK_OK){
		fail_code |= 1 << 0;
		goto exit;
	}
	ret = bk_otp_check_puf_quality();
	if(ret != BK_OK){
		fail_code |= 1 << 1;
		goto exit;
	}
	ret = bk_otp_check_puf_health();
	if(ret != BK_OK){
		fail_code |= 1 << 2;
		goto exit;
	}
	ret = bk_otp_do_auto_repair();
	if(ret != BK_OK){
		fail_code |= 1 << 3;
		if (ret > 0)
			fail_code |= ret << 12;
		goto exit;
	}

	while(otp_hal_read_shfwen(&s_otp.hal) != 0xF && retry_count){
		ret = bk_otp_enable_shuffle_write();
		--retry_count;
		if(ret != BK_OK){
			fail_code |= 1 << 4;
			goto exit;
		}
	}
	if(otp_hal_read_shfwen(&s_otp.hal) != 0xF){
		OTP_LOGE("enable shuffle write fail.\r\n");
		ret = BK_ERR_OTP_OPERATION_FAIL;
		goto exit;
	}

	retry_count = 10;
	while(otp_hal_read_shfren(&s_otp.hal) != 0xF && retry_count){
		ret = bk_otp_enable_shuffle_read();
		--retry_count;
		if(ret != BK_OK){
			fail_code |= 1 << 5;
			goto exit;
		}
	}

	if(otp_hal_read_shfren(&s_otp.hal) != 0xF){
		OTP_LOGE("enable shuffle read fail.\r\n");
		ret = BK_ERR_OTP_OPERATION_FAIL;
		goto exit;
	}

	ret = bk_otp_test_page(0,8);
	if(ret != BK_OK){
		fail_code |= 1 << 6;
		goto exit;
	}
	ret = bk_otp_off_margin_read();
	if(ret != BK_OK){
		fail_code |= 1 << 7;
		goto exit;
	}
	ret = bk_otp_on_margin_read();
	if(ret != BK_OK){
		fail_code |= 1 << 8;
		goto exit;
	}
	ret = bk_otp_initial_off_check();
	if(ret != BK_OK){
		fail_code |= 1 << 9;
		goto exit;
	}
	ret = bk_otp_enable_program_protect_function();
	if(ret != BK_OK){
		fail_code |= 1 << 10;
		goto exit;
	}
	ret = bk_otp_enable_test_mode_lock_function();
	if(ret != BK_OK){
		fail_code |= 1 << 11;
		goto exit;
	}

exit:
	OTP_SLEEP()
	switch (ret)
	{
		case BK_ERR_OTP_OPERATION_ERROR:
			fail_code |= 1 << 16;
			break;

		case BK_ERR_OTP_OPERATION_WRONG:
			fail_code |= 1 << 17;
			break;

		case BK_ERR_OTP_OPERATION_FORBID:
			fail_code |= 1 << 18;
			break;

		case BK_ERR_OTP_OPERATION_FAIL:
			fail_code |= 1 << 19;
			break;

		case BK_ERR_OTP_INIT_FAIL:
			fail_code |= 1 << 20;
			break;

		default:
			break;
	}
	return fail_code;
}
#endif /*CONFIG_ATE_TEST*/
