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
#include <components/system.h>
#include <driver/prro.h>
#include <os/os.h>
#include <os/mem.h>
#include "prro_driver.h"
#include "prro_hal.h"


typedef struct {
	prro_hal_t hal;
} prro_driver_t;

#define PRRO_RETURN_ON_DRIVER_NOT_INIT() do {\
	if (!s_prro_driver_is_init) {\
		PRRO_LOGE("PRRO driver not init\r\n");\
		return BK_ERR_PRRO_DRIVER_NOT_INIT;\
	}\
} while(0)

#define PRRO_RETURN_ON_INVALID_ID(id) do {\
	if (((id) < 0) || ((id) >= PRRO_DEV_MAX)) {\
		PRRO_LOGE("Invalid device id=%d\r\n", (id));\
		return BK_ERR_PRRO_DEV_ID;\
	}\
} while(0)

static prro_driver_t s_prro = {0};
static bool s_prro_driver_is_init = false;

bk_err_t bk_prro_driver_init(void)
{
	if (s_prro_driver_is_init) {
		return BK_OK;
	}

	os_memset(&s_prro, 0, sizeof(s_prro));
	prro_hal_init();
	s_prro_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_prro_driver_deinit(void)
{
	if (!s_prro_driver_is_init) {
		return BK_OK;
	}
	os_memset(&s_prro, 0, sizeof(s_prro));
	s_prro_driver_is_init = false;

	return BK_OK;
}

bk_err_t bk_prro_set_secure(prro_dev_t dev, prro_secure_type_t secure_type)
{
	PRRO_RETURN_ON_DRIVER_NOT_INIT();
	PRRO_RETURN_ON_INVALID_ID(dev);
	PRRO_LOGD("set dev%d to %s\r\n", dev, secure_type == PRRO_SECURE ? "s" : "ns");
	return prro_hal_set_secure(dev, secure_type);
}

prro_secure_type_t bk_prro_get_secure(prro_dev_t dev)
{
	//PRRO_RETURN_ON_DRIVER_NOT_INIT();
	//PRRO_RETURN_ON_INVALID_ID(dev);
	return prro_hal_get_secure(dev);
}
#if 0//code size optimize
bk_err_t bk_prro_set_privilege(prro_dev_t dev, prro_privilege_type_t privilege_type)
{
	PRRO_RETURN_ON_DRIVER_NOT_INIT();
	PRRO_RETURN_ON_INVALID_ID(dev);
	return prro_hal_set_privilege(dev, privilege_type);
}

bk_err_t bk_prro_set_ahb_dev_privilege(uint32_t ahb_priv_bits)
{
	PRRO_RETURN_ON_DRIVER_NOT_INIT();
	PRRO_LOGD("set ahb priv: %x\r\n", ahb_priv_bits);
	prro_hal_set_ahb_dev_privilege(ahb_priv_bits);
	return BK_OK;
}

bk_err_t bk_prro_set_apb_dev_privilege(uint32_t apb_priv_bits)
{
	PRRO_RETURN_ON_DRIVER_NOT_INIT();
	PRRO_LOGD("set apb priv: %x\r\n", apb_priv_bits);
	prro_hal_set_apb_dev_privilege(apb_priv_bits);
	return BK_OK;
}

bk_err_t bk_prro_set_gpios_secure(uint32_t gpio_0_31_bits, uint32_t gpio_32_63_bits)
{
	PRRO_RETURN_ON_DRIVER_NOT_INIT();
	PRRO_LOGD("set gpio secure, gpio 0~31=%x, gpio 32~63=%x\r\n", gpio_0_31_bits, gpio_32_63_bits);
	prro_hal_set_gpios_secure(gpio_0_31_bits, gpio_32_63_bits);
	return BK_OK;
}

bk_err_t bk_prro_set_ahb_dev_secure(uint32_t ahb_dev_secure_bits)
{
	PRRO_RETURN_ON_DRIVER_NOT_INIT();
	PRRO_LOGD("set ahb secure, bits=%x\r\n", ahb_dev_secure_bits);
	prro_hal_set_ahb_dev_secure(ahb_dev_secure_bits);
	return BK_OK;
}

bk_err_t bk_prro_set_apb_dev_secure(uint32_t aph_dev_secure_bits)
{
	PRRO_RETURN_ON_DRIVER_NOT_INIT();
	PRRO_LOGD("set apb secure, bits=%x\r\n", aph_dev_secure_bits);
	prro_hal_set_apb_dev_secure(aph_dev_secure_bits);
	return BK_OK;
}

//TODO user better name!!!
bk_err_t bk_prro_set_hnonsec_dev_secure(uint32_t dev_secure_bits)
{
	PRRO_RETURN_ON_DRIVER_NOT_INIT();
	PRRO_LOGD("set hnonsec secure, bits=%x\r\n", dev_secure_bits);
	prro_hal_set_hnonsec_dev_secure(dev_secure_bits);
	return BK_OK;
}

bk_err_t bk_prro_set_hw_cmp_condition(prro_cmp_id_t id, uint32_t start, uint32_t end)
{
	return BK_OK;
}

bk_err_t bk_prro_set_hw_cmp_src(prro_cmp_id_t id, uint32_t src)
{
	return BK_OK;
}

bk_err_t bk_prro_set_hw_cmp_dst(prro_cmp_id_t id, uint32_t dst)
{
	return BK_OK;
}
#endif


