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

//#include <common/bk_include.h>
//#include <driver/vault.h>
#include <string.h>
#include "otp_hal.h"
#include "_otp.h"
#include "tfm_secure_api.h"


typedef struct {
	otp_hal_t hal;
} otp_driver_t;

static otp_driver_t s_otp = {0};

void psa_otp_nsc_stub(void)
{
	return 0;
}

static void bk_otp_init(otp_hal_t *hal)
{
	hal->hw = (otp_hw_t *)OTP_LL_REG_BASE();
	hal->hw2 = (otp2_hw_t *)OTP2_LL_REG_BASE();

	return BK_OK;
}

void otp_sleep()
{
	otp_hal_power_off(&s_otp.hal);
}

int otp_active()
{
	bk_otp_init(&s_otp.hal);
	return otp_hal_power_on(&s_otp.hal);
}

static uint32_t _otp_read_otp(uint32_t location)
{
	return otp_hal_read_otp(&s_otp.hal, location);
}
static uint32_t _otp2_read_otp(uint32_t location)
{
	return otp2_hal_read_otp(&s_otp.hal, location);
}

static void _otp_write_otp(uint32_t location, uint32_t value)
{
	otp_hal_write_otp(&s_otp.hal, location, value);
}

static void _otp2_write_otp(uint32_t location, uint32_t value)
{
	otp2_hal_write_otp(&s_otp.hal, location, value);
}

static uint32_t _otp_read_permission(uint32_t location)
{
	return otp_hal_read_permission(&s_otp.hal, location);
}

static uint32_t _otp2_read_permission(uint32_t location)
{
	return otp2_hal_read_permission(&s_otp.hal, location);
}

static void _otp_write_permission(uint32_t location, uint32_t permission)
{
	otp_hal_write_permission(&s_otp.hal, location, permission);
}

static void _otp2_write_permission(uint32_t location, uint32_t permission)
{
	otp2_hal_write_permission(&s_otp.hal, location, permission);
}

static uint32_t _otp_read_mask(uint32_t location)
{
	return otp_hal_read_mask(&s_otp.hal, location);
}

static uint32_t _otp2_read_mask(uint32_t location)
{
	return otp2_hal_read_mask(&s_otp.hal, location);
}

static void _otp_write_mask(uint32_t location, uint32_t mask)
{
	otp_hal_write_mask(&s_otp.hal, location, mask);
}

static void _otp2_write_mask(uint32_t location, uint32_t mask)
{
	otp2_hal_write_mask(&s_otp.hal, location, mask);
}

#define OTP_ACTIVE() \
	do { \
		int ret = otp_active(); \
		if(ret != 0) { \
			otp_sleep(); \
			return BK_ERR_OTP_INIT_FAIL;\
		} \
	} while(0);

#define OTP_SLEEP() otp_sleep();

typedef uint32_t (*otp_read_ptr)(uint32_t);
typedef void (*otp_write_ptr)(uint32_t, uint32_t);

uint32_t otp_read_permission(uint8_t map_id, uint32_t item)
{
	uint32_t location =0;

	if(map_id == 1)
	{
		location = otp_map_1[item].offset / 4;
	}
	else if(map_id == 2)
	{
		location = otp_map_2[item].offset / 4;
	}
	else
		return BK_FAIL;

	otp_read_ptr read_permission_fun[] = {_otp_read_permission,_otp2_read_permission};
	otp_read_ptr read_mask_fun[] = {_otp_read_mask, _otp2_read_mask};

	uint32_t permission = read_permission_fun[map_id-1](location);
	uint32_t mask = read_mask_fun[map_id-1](location);
	uint32_t ret_permission = max(permission, mask);
	return ret_permission;
}

bk_err_t otp_write_permission(uint8_t map_id, uint32_t item, otp_privilege_t permission)
{
	uint32_t location = 0;
	uint32_t size = 0;

	if(map_id == 1)
	{
		location = otp_map_1[item].offset / 4;
		size = otp_map_1[item].allocated_size;
	} else if(map_id == 2)
	{
		location = otp_map_2[item].offset / 4;
		size = otp_map_2[item].allocated_size;
	}
	else
		return BK_FAIL;

	otp_write_ptr write_permission_fun[] = {_otp_write_permission,_otp2_write_permission};

	if (permission != OTP_READ_ONLY || permission != OTP_NO_ACCESS) {
		return BK_ERR_OTP_PERMISSION_WRONG;
	}

	while( size > 0 ) {
		write_permission_fun[map_id-1](location, permission);
		location++;
		size-=4;
	}
	return BK_OK;
}

static bk_err_t otp_write_mask(uint8_t map_id, uint32_t item, otp_privilege_t permission)
{
	uint32_t location = 0;
	uint32_t size = 0;

	if(map_id == 1)
	{
		location = otp_map_1[item].offset / 4;
		size = otp_map_1[item].allocated_size;
	}
	else if(map_id == 1)
	{
		location = otp_map_2[item].offset / 4;
		size = otp_map_2[item].allocated_size;
	}
	else
		return BK_FAIL;

	otp_write_ptr write_mask_fun[] = {_otp_write_mask,_otp2_write_mask};

	if (permission != OTP_READ_ONLY && permission != OTP_NO_ACCESS) {
		return BK_ERR_OTP_PERMISSION_WRONG;
	}

	while( size > 0 ) {
		write_mask_fun[map_id-1](location, permission);
		location++;
		size-=4;
	}
	return BK_OK;
}

bk_err_t otp_read(uint8_t map_id, uint32_t item, uint8_t* buf, uint32_t size)
{
	uint32_t location = 0;
	uint32_t start = 0;

	if(map_id == 1)
	{
		location = otp_map_1[item].offset / 4;
		start = otp_map_1[item].offset % 4;
	}
	else if(map_id == 2)
	{
		location = otp_map_2[item].offset / 4;
		start = otp_map_2[item].offset % 4;
	}
	else
		return BK_FAIL;

	if( otp_read_permission(map_id, item) > OTP_READ_ONLY ) {
		return BK_ERR_NO_READ_PERMISSION;
	}
	if(map_id == 1)
	{
		if( size > otp_map_1[item].allocated_size ){
			return BK_ERR_OTP_ADDR_OUT_OF_RANGE;
		}
	}
	else
	{
		if( size > otp_map_2[item].allocated_size ){
			return BK_ERR_OTP_ADDR_OUT_OF_RANGE;
		}
	}

	otp_read_ptr read_fun[] = {_otp_read_otp,_otp2_read_otp};


	uint32_t value;

	while(size > 0) {
		value = read_fun[map_id-1](location);

		uint8_t * src_data = (uint8_t *)&value;
		int       cpy_cnt;

		src_data += start;

		cpy_cnt = (size >= (4 - start)) ? (4 - start) : size;

		switch( cpy_cnt ) {
			case 4:
				*buf++ = *src_data++;
			case 3:
				*buf++ = *src_data++;
			case 2:
				*buf++ = *src_data++;
			case 1:
				*buf++ = *src_data++;
		}

		size -= cpy_cnt;
		location++;
		start = 0;
	}
	return BK_OK;
}

bk_err_t otp_update(uint8_t map_id, uint32_t item, uint8_t* buf, uint32_t size)
{
	uint32_t location = 0;
	uint32_t start = 0;

	if(map_id == 1)
	{
		location = otp_map_1[item].offset / 4;
		start = otp_map_1[item].offset % 4;
	}
	else if(map_id == 2)
	{
		location = otp_map_2[item].offset / 4;
		start = otp_map_2[item].offset % 4;
	}
	else
		return BK_FAIL;

	if( otp_read_permission(map_id, item) > OTP_READ_WRITE ) {
		return BK_ERR_NO_WRITE_PERMISSION;
	}

	if(map_id == 1)
	{
		if( size > otp_map_1[item].allocated_size ){
			return BK_ERR_OTP_ADDR_OUT_OF_RANGE;
		}
	}
	else
	{
		if( size > otp_map_2[item].allocated_size ){
			return BK_ERR_OTP_ADDR_OUT_OF_RANGE;
		}
	}

	uint32_t value = 0;

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
		return BK_ERR_OTP_UPDATE_NOT_EQUAL;  // BK_ERR_OTP_CANNOT_WRTIE; ???
	}

	/* restore all variables. */
	if(map_id == 1)
	{
		location = otp_map_1[item].offset / 4;
		start = otp_map_1[item].offset % 4;
	}
	else if(map_id == 2)
	{
		location = otp_map_2[item].offset / 4;
		start = otp_map_2[item].offset % 4;
	}
	size = back_size;
	buf = back_buf;

	while(size > 0) {
		int cpy_cnt = (size >= (4 - start)) ? (4 - start) : size;

		value = 0;
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

		size -= cpy_cnt;
		location++;
		start = 0;
	}

	return BK_OK;
}

/**
 * obtain APB OTP value in little endian with item map_id:
 * 1. allowed start address of item not aligned
 * 2. allowed end address of item not aligned
 */
__tfm_psa_secure_gateway_no_naked_attributes__ bk_err_t bk_otp_read_nsc(uint32_t item)
{
	OTP_ACTIVE();
	uint32_t size = otp_map_1[item].allocated_size;
	uint8_t* buf = (uint8_t*)malloc(size*(sizeof(uint8_t)));
	if(buf == NULL)
		return BK_ERR_NO_MEM;
	bk_err_t ret = otp_read(2, item, buf, size);
	free(buf);
	buf = NULL;
	otp_sleep();
	return ret;
}


__tfm_psa_secure_gateway_no_naked_attributes__
bk_err_t bk_otp_read_swd_nsc(bool *swd_enabled)
{
    if (swd_enabled == NULL) {
        return BK_ERR_PARAM;
    }

    OTP_ACTIVE();

    uint32_t efuse_word = 0;
    bk_err_t ret = otp_read(1, OTP_EFUSE, (uint8_t *)&efuse_word, sizeof(efuse_word));

    otp_sleep();

    if (ret != BK_OK) {
        return ret;
    }

    *swd_enabled = ((efuse_word & (1u << 31)) == 0u);
    return BK_OK;
}

/**
 * update APB OTP value in little endian with item map_id:
 * 1. allowed start address of item not aligned
 * 2. allowed end address of item not aligned
 * 3. check overwritable before write, return BK_ERR_OTP_CANNOT_WRTIE if failed
 * 4. verify value after write, return BK_ERR_OTP_UPDATE_NOT_EQUAL if failed
 */
__tfm_psa_secure_gateway_no_naked_attributes__ bk_err_t bk_otp_update_nsc(uint8_t map_id, uint32_t item, uint8_t* buf, uint32_t size)
{
	OTP_ACTIVE();
	bk_err_t ret = otp_update(map_id, item, buf, size);
	otp_sleep();
	return ret;
}

__tfm_psa_secure_gateway_no_naked_attributes__ bk_err_t bk_otp_read_permission_nsc(uint8_t map_id, uint32_t item, uint32_t* permission)
{
	OTP_ACTIVE();
	*permission = otp_read_permission(map_id, item);
	otp_sleep();
	return BK_OK;
}

__tfm_psa_secure_gateway_no_naked_attributes__ bk_err_t bk_otp_write_permission_nsc(uint8_t map_id, uint32_t item, uint32_t permission)
{
	OTP_ACTIVE();
	bk_err_t ret = otp_write_permission(map_id, item, permission);
	otp_sleep();
	return ret;
}

__tfm_psa_secure_gateway_no_naked_attributes__ bk_err_t bk_otp_write_mask_nsc(uint8_t map_id, uint32_t item, uint32_t permission)
{
	OTP_ACTIVE();
	bk_err_t ret = otp_write_mask(map_id, item, permission);
	otp_sleep();
	return ret;
}