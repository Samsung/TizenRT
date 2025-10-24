// Copyright 2022-2023 Beken
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

#include "hal_config.h"
#include "prro_hw.h"
#include "prro_hal.h"
#include "prro_ll.h"

static const prro_map_t s_ap_map[] = PRRO_AP_MAP_ENTITY;
static const prro_map_t s_sec_map[] = PRRO_SEC_MAP_ENTITY;

#define TAG "prro_hal"
#define PRRO_HAL_LOGE BK_LOGE
#define PRRO_HAL_LOGD BK_LOGD

#define PRIV_TYPE_TO_BITV(type) ((type) == ((uint8_t)PRRO_PRIVILEGED) ? 1 : 0)
#define SEC_TYPE_TO_BITV(type) ((type) == ((uint8_t)PRRO_NON_SECURE) ? 1 : 0)
#define BITV_TO_SEC_TYPE(bitv) ((bitv) == 1 ? PRRO_NON_SECURE : PRRO_SECURE)

bk_err_t prro_hal_init(void)
{
	prro_ll_init();
	return BK_OK;
}

static uint32_t config_bits_to_reg_bits(prro_map_t *map, uint32_t map_idx_start, uint32_t map_idx_max, uint32_t config_bits)
{
	uint32_t valid_entry_in_map = map_idx_max - map_idx_start;
	uint32_t reg_value = 0;
	uint32_t reg_id = 0;

	if (valid_entry_in_map > 32) {
		PRRO_HAL_LOGE(TAG, "more than 32 entries\r\n");
	}

	for (uint32_t bit_idx = 0; bit_idx < valid_entry_in_map; bit_idx++) {
		uint32_t map_idx = bit_idx + map_idx_start;

		if (bit_idx == 0) {
			reg_id = map[map_idx].reg;
		} else if ((bit_idx != 0) && (reg_id != map[map_idx].reg)) { //All bits should in the same register
			PRRO_HAL_LOGE(TAG, "bits not in the same reg\r\n");
		}

		if (config_bits & BIT(bit_idx)) {
			reg_value |= BIT(bit_idx);
		}
	}

	return reg_value;
}

bk_err_t prro_hal_set_privilege(prro_dev_t dev, prro_privilege_type_t privilege_type)
{
	PRRO_AP_MAP(ap_map);

	if (dev >= PRRO_AP_MAP_COUNT) {
		PRRO_HAL_LOGE(TAG, "invalid dev=%d\r\n", dev);
		return BK_ERR_PRRO_DEV_ID;
	}

	if (dev != ap_map[dev].dev) {
		PRRO_HAL_LOGE(TAG, "device=%d not match map index\n", dev);
		return BK_ERR_PRRO_MAP_TABLE;
	}

	PRRO_HAL_LOGD(TAG, "set priv dev%x to %s, reg=%x, bit=%d\r\n",
		dev, (privilege_type == PRRO_PRIVILEGED) ? "privilege" : "non_privilege", ap_map[dev].reg, ap_map[dev].bit);

	if (ap_map[dev].reg == PRRO_INVALID_REG) {
		PRRO_HAL_LOGE(TAG, "invalid dev=%d\r\n", dev);
		return BK_ERR_PRRO_DEV_ID;
	}

	prro_ll_set_reg_bit(ap_map[dev].reg, ap_map[dev].bit, PRIV_TYPE_TO_BITV(privilege_type));

	return BK_OK;
}

bk_err_t prro_hal_set_ahb_dev_privilege(uint32_t ahb_dev_privilege_bits)
{
	PRRO_AP_MAP(ap_map);
	uint32_t reg_bits = config_bits_to_reg_bits(ap_map, PRRO_DEV_AHB_AON, PRRO_DEV_AHB_MAX, ahb_dev_privilege_bits);
	PRRO_HAL_LOGD(TAG, "set privilege ahb, reg_bits=%x\r\n", reg_bits);
	prro_ll_set_ahb_cfg_ap(reg_bits);
	return BK_OK;
}

bk_err_t prro_hal_set_apb_dev_privilege(uint32_t apb_dev_privilege_bits)
{
	PRRO_AP_MAP(ap_map);
	uint32_t reg_bits = config_bits_to_reg_bits(ap_map, PRRO_DEV_APB_WDT, PRRO_DEV_APB_MAX, apb_dev_privilege_bits);
	PRRO_HAL_LOGD(TAG, "set privilege apb, reg_bits=%x\r\n", reg_bits);
	prro_ll_set_apb_cfg_ap(reg_bits);
	return BK_OK;
}


static bk_err_t hal_set_gpio_secure(prro_dev_t dev, prro_secure_type_t secure_type)
{
	uint32_t gpio_bit = dev - PRRO_DEV_GPIO_0;

	PRRO_HAL_LOGD(TAG, "set gpio%d to %s\r\n", dev, secure_type == PRRO_SECURE ? "s" : "ns");
	if (gpio_bit <= 31) {
		prro_ll_set_bit_aon_gpio_nonsec0(gpio_bit, SEC_TYPE_TO_BITV(secure_type));
	} else {
		prro_ll_set_bit_aon_gpio_nonsec1(gpio_bit - 32, SEC_TYPE_TO_BITV(secure_type));
	}
	return BK_OK;
}

bk_err_t prro_hal_set_secure(prro_dev_t dev, prro_secure_type_t secure_type)
{
	if (dev >= PRRO_DEV_GPIO_0) {
		return hal_set_gpio_secure(dev, secure_type);
	}

	PRRO_SEC_MAP(sec_map);

	if (dev >= PRRO_SEC_MAP_COUNT) {
		PRRO_HAL_LOGE(TAG, "invalid dev=%d\r\n", dev);
		return BK_ERR_PRRO_DEV_ID;
	}

	if (dev != sec_map[dev].dev) {
		PRRO_HAL_LOGE(TAG, "device=%d not match map index\n", dev);
		return BK_ERR_PRRO_MAP_TABLE;
	}

	PRRO_HAL_LOGD(TAG, "set dev=%d to %s, reg=%x, bit=%d\r\n",
		dev, secure_type == PRRO_SECURE ? "s" : "ns", sec_map[dev].reg, sec_map[dev].bit);

	if (sec_map[dev].reg == PRRO_INVALID_REG) {
		PRRO_HAL_LOGE(TAG, "invalid dev=%d\r\n", dev);
		return BK_ERR_PRRO_DEV_ID;
	}

	prro_ll_set_reg_bit(sec_map[dev].reg, sec_map[dev].bit, SEC_TYPE_TO_BITV(secure_type));
	return BK_OK;

}

prro_secure_type_t prro_hal_get_secure(prro_dev_t dev)
{
	PRRO_SEC_MAP(sec_map);

	uint32_t bit_val = prro_ll_get_reg_bit(sec_map[dev].reg, sec_map[dev].bit);
	return BITV_TO_SEC_TYPE(bit_val);
}

bk_err_t prro_hal_set_gpios_secure(uint32_t group0_bits, uint32_t group1_bits)
{
	PRRO_HAL_LOGD(TAG, "set gpio 0~31 to %x, 32~64 to %x\r\n", group0_bits, group1_bits);
	prro_ll_set_aon_gpio_nonsec0(group0_bits);
	prro_ll_set_aon_gpio_nonsec1(group1_bits);
	return BK_OK;
}

bk_err_t prro_hal_set_ahb_dev_secure(uint32_t ahb_dev_secure_bits)
{
	PRRO_SEC_MAP(sec_map);
	uint32_t reg_bits = config_bits_to_reg_bits(sec_map, PRRO_DEV_AHB_AON, PRRO_DEV_AHB_MAX, ahb_dev_secure_bits);
	PRRO_HAL_LOGD(TAG, "set ahb secure, reg_bits=%x\r\n", reg_bits);
	prro_ll_set_ahb_cfg_nsec(reg_bits);
	return BK_OK;
}

bk_err_t prro_hal_set_apb_dev_secure(uint32_t apb_dev_secure_bits)
{
	PRRO_SEC_MAP(sec_map);
	uint32_t reg_bits = config_bits_to_reg_bits(sec_map, PRRO_DEV_APB_WDT, PRRO_DEV_APB_MAX, apb_dev_secure_bits);
	PRRO_HAL_LOGD(TAG, "set apb secure, reg_bits=%x\r\n", reg_bits);
	prro_ll_set_apb_cfg_nsec(reg_bits);
	return BK_OK;
}

//TODO
bk_err_t prro_hal_set_hnonsec_dev_secure(uint32_t hnonsec_dev_secure_bits)
{
	return BK_OK;
}

