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

#include <stdlib.h>
#include <common/bk_include.h>
#include <os/os.h>
#include <os/mem.h>
#include <driver/ckmn.h>
#include "ckmn_driver.h"
#include "ckmn_hal.h"
#include "sys_driver.h"

typedef struct {
	ckmn_hal_t hal;
} ckmn_driver_t;

#define CKMN_RETURN_ON_NOT_INIT() do {\
		if (!s_ckmn_driver_is_init) {\
			return BK_ERR_CKMN_DRIVER_NOT_INIT;\
		}\
	} while(0)

#define CKMN_RETURN_ON_INVALID_INT(type) do {\
		if ((type) >= CKMN_INT_MAX) {\
			return BK_ERR_CKMN_INT_TYPE;\
		}\
	} while(0)

#define CKMN_RETURN_ON_CKEST_BUSY() do {\
		if ((REG_READ(CKMN_RC32K_CTRL_ADDR) >> CKMN_RC32K_CTRL_ENABLE_POS) & CKMN_RC32K_CTRL_ENABLE_MASK ||\
			(REG_READ(CKMN_INTR_ADDR) >> CKMN_CKEST_INTR_STATUS_POS) & CKMN_CKEST_INTR_STATUS_MASK) {\
			return BK_ERR_CKMN_CKEST_BUSY;\
		}\
	} while(0)

static ckmn_driver_t s_ckmn = {0};
static ckmn_isr_t s_ckmn_isr[CKMN_INT_MAX] = {NULL};
static bool s_ckmn_driver_is_init = false;

bk_err_t bk_ckmn_register_isr(ckmn_int_type_t int_type, ckmn_isr_t int_isr)
{
	CKMN_RETURN_ON_NOT_INIT();
	CKMN_RETURN_ON_INVALID_INT(int_type);
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	s_ckmn_isr[int_type] = int_isr;
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

bk_err_t bk_ckmn_unregister_isr(ckmn_int_type_t int_type)
{
	CKMN_RETURN_ON_NOT_INIT();
	CKMN_RETURN_ON_INVALID_INT(int_type);
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	s_ckmn_isr[int_type] = 0;
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

static void ckmn_isr(void)
{
	ckmn_hal_t *hal = &s_ckmn.hal;
	if (ckmn_hal_get_ckest_intr_status(hal)) {
		CKMN_LOGD("ckmn_isr CKEST FINISH TRIGGERED!\r\n");
		//NOTES:clear intrrupt in condition because maybe multi-core(two CPU) access one CKMN
		//it can't cleared peer-side channels status.
		if (s_ckmn_isr[CKMN_INT_CKEST]) {
			CKMN_LOGD("ckmn_isr CLSET_finish_isr!\r\n");
			ckmn_hal_clear_ckest_intr_status(hal);
			s_ckmn_isr[CKMN_INT_CKEST]();
		}
	}

	if (ckmn_hal_get_cor26m_intr_status(hal)) {
		CKMN_LOGD("ckmn_isr cor26m_intr TRIGGERED!\r\n");
		if (s_ckmn_isr[CKMN_INT_26M]) {
			CKMN_LOGD("ckmn_isr cor26m_isr!\r\n");
			ckmn_hal_clear_cor26m_intr_status(hal);
			s_ckmn_isr[CKMN_INT_26M]();
		}
	}

	if (ckmn_hal_get_cor32k_intr_status(hal)) {
		CKMN_LOGD("ckmn_isr cor32k_intr TRIGGERED!\r\n");
		if (s_ckmn_isr[CKMN_INT_32K]) {
			CKMN_LOGD("ckmn_isr cor32k_isr!\r\n");
			ckmn_hal_clear_cor32k_intr_status(hal);
			s_ckmn_isr[CKMN_INT_32K]();
		}
	}
}

bk_err_t bk_ckmn_set_rc32k_count(uint32_t count)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_set_rc32k_count(&s_ckmn.hal, count);

	return BK_OK;
}

bk_err_t bk_ckmn_get_rc32k_count(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	return ckmn_hal_get_rc32k_count(&s_ckmn.hal);
}

bk_err_t bk_ckmn_ckest_enable(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_rc32k_enable(&s_ckmn.hal);
	ckmn_hal_rc32k_intr_enable(&s_ckmn.hal);

	return BK_OK;
}

bk_err_t bk_ckmn_ckest_disable(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_rc32k_intr_disable(&s_ckmn.hal);
	ckmn_hal_rc32k_disable(&s_ckmn.hal);

	return BK_OK;
}

bk_err_t bk_ckmn_ckest_check_busy(void)
{
	CKMN_RETURN_ON_CKEST_BUSY();
	return BK_OK;
}

bk_err_t bk_ckmn_get_rc26m_count(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	return ckmn_hal_get_rc26m_count(&s_ckmn.hal);
}

bk_err_t bk_ckmn_set_26m_target(ckmn_acc_t target)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_set_26m_target(&s_ckmn.hal, target);

	return BK_OK;
}

bk_err_t bk_ckmn_enable_autosw_26m(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_autosw_26m_enable(&s_ckmn.hal);
	ckmn_hal_corr_26m_enable(&s_ckmn.hal);

	return BK_OK;
}

bk_err_t bk_ckmn_disable_autosw_26m(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_autosw_26m_disable(&s_ckmn.hal);
	ckmn_hal_corr_26m_disable(&s_ckmn.hal);

	return BK_OK;
}
bk_err_t bk_ckmn_enable_corr_26m(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_corr_26m_enable(&s_ckmn.hal);

	return BK_OK;
}

bk_err_t bk_ckmn_disable_corr_26m(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_corr_26m_disable(&s_ckmn.hal);

	return BK_OK;
}

bk_err_t bk_ckmn_set_32k_target(ckmn_acc_t target)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_set_32k_target(&s_ckmn.hal, target);

	return BK_OK;
}
bk_err_t bk_ckmn_enable_autosw_32k(void) {
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_autosw_32k_enable(&s_ckmn.hal);
	ckmn_hal_corr_32k_enable(&s_ckmn.hal);

	return BK_OK;
}
bk_err_t bk_ckmn_disable_autosw_32k(void) {
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_autosw_32k_disable(&s_ckmn.hal);
	ckmn_hal_corr_32k_disable(&s_ckmn.hal);

	return BK_OK;
}
bk_err_t bk_ckmn_enable_corr_32k(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_corr_32k_enable(&s_ckmn.hal);

	return BK_OK;
}

bk_err_t bk_ckmn_disable_corr_32k(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_corr_32k_disable(&s_ckmn.hal);

	return BK_OK;
}

bk_err_t bk_ckmn_cor26m_intr_enable(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_cor26m_intr_enable(&s_ckmn.hal);

	return BK_OK;
}

bk_err_t bk_ckmn_cor26m_intr_disable(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_cor26m_intr_disable(&s_ckmn.hal);

	return BK_OK;
}

bk_err_t bk_ckmn_cor32k_intr_enable(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_cor32k_intr_enable(&s_ckmn.hal);

	return BK_OK;
}

bk_err_t bk_ckmn_cor32k_intr_disable(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_cor32k_intr_disable(&s_ckmn.hal);

	return BK_OK;
}

//reg int_status:

bk_err_t bk_ckmn_get_intr_status(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	return ckmn_hal_get_intr_status(&s_ckmn.hal);
}

bk_err_t bk_ckmn_clear_ckest_intr_status(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_clear_ckest_intr_status(&s_ckmn.hal);

	return BK_OK;
}
bk_err_t bk_ckmn_clear_cor26m_intr_status(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_clear_cor26m_intr_status(&s_ckmn.hal);

	return BK_OK;
}

bk_err_t bk_ckmn_clear_cor32k_intr_status(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_clear_cor32k_intr_status(&s_ckmn.hal);

	return BK_OK;
}

bk_err_t bk_ckmn_get_ckest_intr_status(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	return ckmn_hal_get_ckest_intr_status(&s_ckmn.hal);
}

bk_err_t bk_ckmn_get_cor26m_intr_status(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	return ckmn_hal_get_cor26m_intr_status(&s_ckmn.hal);
}

bk_err_t bk_ckmn_get_cor32k_intr_status(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	return ckmn_hal_get_cor32k_intr_status(&s_ckmn.hal);
}

bk_err_t bk_ckmn_soft_reset(void)
{
	CKMN_RETURN_ON_NOT_INIT();
	ckmn_hal_set_soft_reset(&s_ckmn.hal);

	return BK_OK;
}

bk_err_t bk_ckmn_driver_init(void)
{
	if (s_ckmn_driver_is_init) {
		return BK_OK;
	}

	os_memset(&s_ckmn, 0, sizeof(s_ckmn));
	os_memset(&s_ckmn_isr, 0, sizeof(s_ckmn_isr));

	bk_int_isr_register(INT_SRC_CKMN, ckmn_isr, NULL);

	ckmn_hal_init(&s_ckmn.hal);

	sys_drv_int_enable(CKMN_INTERRUPT_CTRL_BIT);
	s_ckmn_driver_is_init = true;

#if defined(CONFIG_CKMN_TEST)
        int bk_ckmn_register_cli_test_feature(void);
        bk_ckmn_register_cli_test_feature();
#endif

	return BK_OK;
}

bk_err_t bk_ckmn_driver_deinit(void)
{
	if (!s_ckmn_driver_is_init) {
		return BK_OK;
	}

	s_ckmn_driver_is_init = false;
	sys_drv_int_disable(CKMN_INTERRUPT_CTRL_BIT);
	bk_int_isr_unregister(INT_SRC_CKMN);

	return BK_OK;
}

