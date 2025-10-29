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
#include <common/bk_compiler.h>
#include <os/mem.h>
#include "icu_driver.h"
#include "dma_driver.h"
#include "dma_hal.h"
#include <driver/dma.h>
#include "bk_general_dma.h"
#include <driver/int.h>
#include "sys_driver.h"

#if defined(CONFIG_CACHE_ENABLE)
#include "cache.h"
#endif

#include "bk_misc.h"

#include <os/mem.h>

#ifdef CONFIG_FREERTOS_SMP
#include "spinlock.h"
static volatile spinlock_t dma_spin_lock = SPIN_LOCK_INIT;
#endif // CONFIG_FREERTOS_SMP

static inline uint32_t dma_enter_critical()
{
	uint32_t flags = rtos_disable_int();

#ifdef CONFIG_FREERTOS_SMP
	spin_lock(&dma_spin_lock);
#endif // CONFIG_FREERTOS_SMP

	return flags;
}

static inline void dma_exit_critical(uint32_t flags)
{
#ifdef CONFIG_FREERTOS_SMP
	spin_unlock(&dma_spin_lock);
#endif // CONFIG_FREERTOS_SMP

	rtos_enable_int(flags);
}

#define DMA_CPU_MASTER		0
#define DMA_CPU_SLAVE1		1

static void dma_isr(void) ;	//dma-hw-0
static void dma1_isr(void) ;	//dma-hw-1

typedef struct {
    dma_hal_t hal;
    uint32_t id_init_bits;
} dma_driver_t;

static dma_driver_t s_dma[SOC_DMA_UNIT_NUM] = {0};
static dma_isr_t s_dma_finish_isr[SOC_DMA_UNIT_NUM][SOC_DMA_CHAN_NUM_PER_UNIT] = {NULL};
static dma_isr_t s_dma_half_finish_isr[SOC_DMA_UNIT_NUM][SOC_DMA_CHAN_NUM_PER_UNIT] = {NULL};
static dma_isr_t s_dma_bus_err_isr[SOC_DMA_UNIT_NUM][SOC_DMA_CHAN_NUM_PER_UNIT] = {NULL};

static bool s_dma_driver_is_init = false;
static dma_chnl_pool_t s_dma_chnl_pool = {0};

#define DMA_RETURN_ON_NOT_INIT() do {\
        if (!s_dma_driver_is_init) {\
            return BK_ERR_DMA_NOT_INIT;\
        }\
    } while(0)

#define DMA_RETURN_ON_INVALID_ID(channel) do {\
        if (((channel) < CONFIG_DMA_LOGIC_CHAN_ID_MIN) || ((channel) >= CONFIG_DMA_LOGIC_CHAN_ID_MIN + CONFIG_DMA_LOGIC_CHAN_CNT)) {\
            return BK_ERR_DMA_ID;\
        }\
    } while(0)

#define DMA_RETURN_ON_ID_NOT_INIT(dma_num,id) do {\
        if (!(s_dma[dma_num].id_init_bits & BIT((id)))) {\
            return BK_ERR_DMA_ID_NOT_INIT;\
        }\
    } while(0)

#define DMA_LOG_ON_ID_IS_STARTED(dma_num,channel) do {\
        if (dma_hal_is_id_started(&s_dma[dma_num].hal, (channel))) {\
        }\
    } while(0)

#define DMA_RETURN_ON_INVALID_ADDR(start_addr, end_addr) do {\
        if ((0 < (end_addr)) && ((end_addr) < (start_addr))) {\
            return BK_ERR_DMA_INVALID_ADDR;\
        }\
    } while(0)

static void dma_id_to_hw_id_ch(dma_id_t id,uint8_t *dma_num,uint8_t *channle)
{
    if(dma_num){
        *dma_num = id / SOC_DMA_CHAN_NUM_PER_UNIT;
    }
    if(channle){
        *channle = id % SOC_DMA_CHAN_NUM_PER_UNIT;
    }
}

static void dma_id_init_common(dma_id_t id)
{
    uint8_t dma_num;
    uint8_t channel;
    dma_id_to_hw_id_ch(id, &dma_num, &channel);
    s_dma[dma_num].id_init_bits |= BIT(channel);
}

static void dma_id_deinit_common(dma_id_t id)
{
	uint8_t dma_num;
	uint8_t dma_channel;
	dma_id_to_hw_id_ch(id, &dma_num, &dma_channel);
    s_dma[dma_num].id_init_bits &= ~BIT(dma_channel);
    dma_hal_stop_common(&s_dma[dma_num].hal, dma_channel);
    dma_hal_reset_config_to_default(&s_dma[dma_num].hal, dma_channel);
}

static void dma_id_enable_interrupt_common(dma_id_t id)
{

#if (defined(CONFIG_SYSTEM_CTRL))
	#if defined(CONFIG_SPE)
	sys_drv_int_enable(GDMA_INTERRUPT_CTRL_BIT);
	sys_drv_int_enable(DMA1_SEC_INTERRUPT_CTRL_BIT);
	#elif (CONFIG_SPE == 1)
	sys_drv_int_enable(GDMA_INTERRUPT_CTRL_BIT);
	sys_drv_int_group2_enable(DMA1_SEC_INTERRUPT_CTRL_BIT);
	#else
	sys_drv_int_enable(DMA0_NSEC_INTERRUPT_CTRL_BIT);
	sys_drv_int_enable(DMA1_NSEC_INTERRUPT_CTRL_BIT);
	#endif
#else
    icu_enable_dma_interrupt();
#endif
}

/* used internally, called in context of interrupt disabled. */
u8 dma_chnl_alloc(u32 user_id)
{
	u8 chnl_id;

    for(chnl_id = CONFIG_DMA_LOGIC_CHAN_ID_MIN; chnl_id < CONFIG_DMA_LOGIC_CHAN_ID_MIN + CONFIG_DMA_LOGIC_CHAN_CNT; chnl_id++)
	{
		if((s_dma_chnl_pool.chnl_bitmap & (0x01 << chnl_id)) == 0)
		{
			s_dma_chnl_pool.chnl_bitmap |= (0x01 << chnl_id);
			s_dma_chnl_pool.chnl_user[chnl_id] = user_id;
			return chnl_id;
		}
	}

	//alloc failed
	DMA_LOGE("%s:chnl_id=%d\r\n", __func__, chnl_id);
	return DMA_ID_MAX;
}

dma_id_t dma_fixed_chnl_alloc(u32 user_id, dma_id_t fixed_chnl_id)
{

    if((fixed_chnl_id >= CONFIG_DMA_LOGIC_CHAN_ID_MIN) && (fixed_chnl_id < CONFIG_DMA_LOGIC_CHAN_ID_MIN + CONFIG_DMA_LOGIC_CHAN_CNT))
	{
		if((s_dma_chnl_pool.chnl_bitmap & (0x01 << fixed_chnl_id)) == 0)
		{
			s_dma_chnl_pool.chnl_bitmap |= (0x01 << fixed_chnl_id);
			s_dma_chnl_pool.chnl_user[fixed_chnl_id] = user_id;
			return fixed_chnl_id;
		}
	}

	DMA_LOGE("chan=%d has been allocated\r\n", fixed_chnl_id);
	return DMA_ID_MAX;
}

/* used internally, called in context of interrupt disabled. */
bk_err_t dma_chnl_free(u32 user_id, dma_id_t chnl_id)
{
	if( chnl_id >= DMA_ID_MAX )
		return BK_ERR_DMA_ID;

	if( s_dma_chnl_pool.chnl_user[chnl_id] != user_id )
		return BK_ERR_PARAM;

	s_dma_chnl_pool.chnl_bitmap &= ~(0x01 << chnl_id);
	s_dma_chnl_pool.chnl_user[chnl_id] = -1;

	return BK_OK;
}

/* used internally. */
u32 dma_chnl_user(dma_id_t chnl_id)
{
	if( chnl_id >= DMA_ID_MAX )
		return -1;

	return s_dma_chnl_pool.chnl_user[chnl_id];
}

bk_err_t bk_dma_driver_init(void)
{
    if (s_dma_driver_is_init) {
        return BK_OK;
    }

	s_dma_chnl_pool.chnl_bitmap = 0;
	for(u8 i = CONFIG_DMA_LOGIC_CHAN_ID_MIN; i < CONFIG_DMA_LOGIC_CHAN_ID_MIN + CONFIG_DMA_LOGIC_CHAN_CNT; i++)
	{
		s_dma_chnl_pool.chnl_user[i] = -1;
	}

    // workaround: must uncomment it after mailbox problem fixed
    // bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_DMA0, PM_POWER_MODULE_STATE_ON);
    // bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_DMA1, PM_POWER_MODULE_STATE_ON);

    /* 1)intc_service_register
     * 2)init dma_finish_int handler, dma_half_finish_int handler
     * 3)disable dma_en (0~6), clear int status
     * 4)init dma_config
     */
    os_memset(&s_dma, 0, sizeof(s_dma));
    os_memset(&s_dma_finish_isr, 0, sizeof(s_dma_finish_isr));
    os_memset(&s_dma_half_finish_isr, 0, sizeof(s_dma_half_finish_isr));
    os_memset(&s_dma_bus_err_isr, 0, sizeof(s_dma_bus_err_isr));

	//TODO:if the channels in the selected DMA, it should register ISR, else no need to register it.
    #if defined(CONFIG_SPE)
    bk_int_isr_register(INT_SRC_GDMA, dma_isr, NULL);
    #elif (CONFIG_SPE == 1)
    bk_int_isr_register(INT_SRC_GDMA, dma_isr, NULL);
    #else
    bk_int_isr_register(INT_SRC_DMA0_NSEC, dma_isr, NULL);
    #endif

#if (!(defined(CONFIG_SOC_BK7236N)) && !(defined(CONFIG_SOC_BK7239XX)))
	#if defined(CONFIG_SPE)
	bk_int_isr_register(INT_SRC_DMA1_SEC, dma1_isr, NULL);
    #elif (CONFIG_SPE == 1)
	bk_int_isr_register(INT_SRC_DMA1_SEC, dma1_isr, NULL);
    #else
	bk_int_isr_register(INT_SRC_DMA1_NSEC, dma1_isr, NULL);
    #endif
#endif

    for (uint32_t uint_id = 0; uint_id < SOC_DMA_UNIT_NUM; uint_id++) {
	    s_dma[uint_id].hal.id = uint_id;
		dma_hal_init(&s_dma[uint_id].hal);
	}
#if defined(CONFIG_ARCH_RISCV)
    rtos_regist_plat_dump_hook((uint32_t)(s_dma.hal.hw), sizeof(dma_hw_t));
#endif

    s_dma_driver_is_init = true;

    return BK_OK;
}

bk_err_t bk_dma_driver_deinit(void)
{
    if (!s_dma_driver_is_init) {
        return BK_OK;
    }

    for (int id = 0; id < (SOC_DMA_CHAN_NUM_PER_UNIT*SOC_DMA_UNIT_NUM); id++) {
        dma_id_deinit_common(id);
    }

    bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_DMA0, PM_POWER_MODULE_STATE_OFF);
    bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_DMA1, PM_POWER_MODULE_STATE_OFF);

#if (defined(CONFIG_SYSTEM_CTRL))
	sys_drv_int_disable(GDMA_INTERRUPT_CTRL_BIT);
	sys_drv_int_group2_disable(DMA1_SEC_INTERRUPT_CTRL_BIT);

#else
    icu_disable_dma_interrupt();
#endif

    s_dma_driver_is_init = false;

    return BK_OK;
}

dma_id_t bk_dma_alloc(u16 user_id)
{
    if (!s_dma_driver_is_init)
    {
        return DMA_ID_MAX;
    }

    u32 int_mask = dma_enter_critical();

    u8 chnl_id = dma_chnl_alloc(user_id);

    dma_exit_critical(int_mask);

    return chnl_id;
}

dma_id_t bk_fixed_dma_alloc(u16 user_id, dma_id_t chnl_id)
{
    if (!s_dma_driver_is_init)
    {
        return DMA_ID_MAX;
    }

    u32 int_mask = dma_enter_critical();

    u8 chnl_id_ret = dma_fixed_chnl_alloc(user_id, chnl_id);

    dma_exit_critical(int_mask);

    return chnl_id_ret;
}

bk_err_t bk_dma_free(u16 user_id, dma_id_t chnl_id)
{
    if (!s_dma_driver_is_init)
    {
        return BK_ERR_DMA_NOT_INIT;
    }

    u32  int_mask = dma_enter_critical();

    bk_err_t ret_val = dma_chnl_free(user_id, chnl_id);

    dma_exit_critical(int_mask);

    return ret_val;
}

uint32_t bk_dma_user(dma_id_t chnl_id)
{
    if (!s_dma_driver_is_init)
    {
        return -1;
    }

    return dma_chnl_user(chnl_id);
}

bk_err_t bk_dma_init(dma_id_t id, const dma_config_t *config)
{
    uint8_t dma_channel,dma_num;
    DMA_RETURN_ON_NOT_INIT();
    BK_RETURN_ON_NULL(config);
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);
    if (0 == dma_num) {
        //bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_DMA0, PM_POWER_MODULE_STATE_ON);
    } else if (1 == dma_num) {
        //bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_DMA1, PM_POWER_MODULE_STATE_ON);
    }
    dma_hal_init_without_channels(&s_dma[dma_num].hal);	//TODO:special codes for DMA init after enter low voltage
    DMA_RETURN_ON_INVALID_ID(id);
    DMA_RETURN_ON_INVALID_ADDR(config->src.start_addr, config->src.end_addr);
    DMA_RETURN_ON_INVALID_ADDR(config->dst.start_addr, config->dst.end_addr);
    DMA_LOG_ON_ID_IS_STARTED(dma_num,dma_channel);

#if defined(CONFIG_CACHE_ENABLE)
    flush_dcache((void *)config->src.start_addr, config->src.end_addr - config->src.start_addr);
    flush_dcache((void *)config->dst.start_addr, config->dst.end_addr - config->dst.start_addr);
#endif

    dma_id_init_common(id);
    DMA_LOGD("%s and %d 0x%x\r\n",__func__,__LINE__,s_dma[dma_num].hal);
    DMA_LOGD("%s and %d %d\r\n",__func__,__LINE__,dma_channel);
    return dma_hal_init_dma(&s_dma[dma_num].hal, dma_channel, config);
}

bk_err_t bk_dma_deinit(dma_id_t id)
{
	uint8_t dma_channel,dma_num;
	dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);
    DMA_RETURN_ON_INVALID_ID(id);
    dma_id_deinit_common(id);
    bk_dma_register_isr(id, NULL, NULL);
    return BK_OK;
}

bk_err_t bk_dma_start(dma_id_t id)
{
	uint8_t dma_channel,dma_num;
	dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_ID_NOT_INIT(dma_num,dma_channel);
    dma_hal_start_common(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

bk_err_t bk_dma_stop(dma_id_t id)
{
	uint8_t dma_channel,dma_num;
	dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
	DMA_RETURN_ON_ID_NOT_INIT(dma_num, dma_channel);
    dma_hal_stop_common(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

uint32_t bk_dma_get_enable_status(dma_id_t id)
{
	uint8_t dma_channel,dma_num;
	dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_ID_NOT_INIT(dma_num,dma_channel);
    uint32_t ret;
    ret = dma_hal_get_enable_status(&s_dma[dma_num].hal, dma_channel);
    return ret;
}

#define DMA_MAX_BUSY_TIME (10000)  //us
uint32_t dma_wait_to_idle(dma_id_t id)
{
	uint8_t dma_channel,dma_num;
	dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

	if(dma_hal_get_work_mode(&s_dma[dma_num].hal, dma_channel) == DMA_WORK_MODE_SINGLE)
	{
		uint32_t i = 0;
		while(dma_hal_get_enable_status(&s_dma[dma_num].hal, dma_channel))
		{
			delay_us(1);

			i++;
			if(i > DMA_MAX_BUSY_TIME)
			{
				DMA_LOGE("ch%d busy,remain len=%d,dst_addr=%x\r\n", dma_channel,
							dma_hal_get_remain_len(&s_dma[dma_num].hal, dma_channel),
							dma_hal_get_dest_write_addr(&s_dma[dma_num].hal, dma_channel));
				break;
			}
		}

		return i;
	}
	else
	{
		//TODO:
	}

	return 0;
}

/* DTCM->peripheral
 */
bk_err_t bk_dma_write(dma_id_t id, const uint8_t *data, uint32_t size)
{
	uint8_t dma_channel,dma_num;
	dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_ID_NOT_INIT(dma_num,dma_channel);
	dma_wait_to_idle(id);

    dma_hal_set_src_start_addr(&s_dma[dma_num].hal, dma_channel, (uint32_t)data);
    dma_hal_set_src_loop_addr(&s_dma[dma_num].hal, dma_channel, (uint32_t)data, (uint32_t)(data + size));
    dma_hal_set_transfer_len(&s_dma[dma_num].hal, dma_channel, size);
    dma_hal_start_common(&s_dma[dma_num].hal, dma_channel);

    return BK_OK;
}

/* peripheral->DTCM
 */
bk_err_t bk_dma_read(dma_id_t id, uint8_t *data, uint32_t size)
{
	uint8_t dma_channel,dma_num;
	dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_ID_NOT_INIT(dma_num,dma_channel);
	dma_wait_to_idle(id);

    dma_hal_set_dest_start_addr(&s_dma[dma_num].hal, dma_channel, (uint32_t)data);
    dma_hal_set_dest_loop_addr(&s_dma[dma_num].hal, dma_channel, (uint32_t)data, (uint32_t)(data + size));
    dma_hal_set_transfer_len(&s_dma[dma_num].hal, dma_channel, size);
    dma_hal_start_common(&s_dma[dma_num].hal, dma_channel);

    return BK_OK;
}

bk_err_t bk_dma_enable_finish_interrupt(dma_id_t id)
{
	uint8_t dma_channel,dma_num;
	dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);
    DMA_RETURN_ON_ID_NOT_INIT(dma_num,dma_channel);
    DMA_RETURN_ON_INVALID_ID(id);
    dma_id_enable_interrupt_common(dma_num);
    dma_hal_enable_finish_interrupt(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

bk_err_t bk_dma_disable_finish_interrupt(dma_id_t id)
{
	uint8_t dma_channel,dma_num;
	dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_ID_NOT_INIT(dma_num,dma_channel);
    DMA_RETURN_ON_INVALID_ID(id);

    dma_hal_disable_finish_interrupt(&s_dma[dma_num].hal, dma_channel);
    dma_hal_clear_finish_interrupt_status(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

bk_err_t bk_dma_enable_half_finish_interrupt(dma_id_t id)
{
	uint8_t dma_channel,dma_num;
	dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_ID_NOT_INIT(dma_num,dma_channel);
    DMA_RETURN_ON_INVALID_ID(id);
    dma_id_enable_interrupt_common(dma_channel);
    dma_hal_enable_half_finish_interrupt(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

bk_err_t bk_dma_disable_half_finish_interrupt(dma_id_t id)
{
	uint8_t dma_channel,dma_num;
	dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_ID_NOT_INIT(dma_num,dma_channel);
    DMA_RETURN_ON_INVALID_ID(id);

    dma_hal_disable_half_finish_interrupt(&s_dma[dma_num].hal, dma_channel);
    dma_hal_clear_half_finish_interrupt_status(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

bk_err_t bk_dma_enable_bus_err_interrupt(dma_id_t id)
{
	uint8_t dma_channel,dma_num;
	dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);
    DMA_RETURN_ON_ID_NOT_INIT(dma_num,dma_channel);
    DMA_RETURN_ON_INVALID_ID(id);
    dma_id_enable_interrupt_common(dma_num);
    dma_hal_enable_bus_err_interrupt(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

bk_err_t bk_dma_disable_bus_err_interrupt(dma_id_t id)
{
	uint8_t dma_channel,dma_num;
	dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_ID_NOT_INIT(dma_num,dma_channel);
    DMA_RETURN_ON_INVALID_ID(id);

    dma_hal_disable_bus_err_interrupt(&s_dma[dma_num].hal, dma_channel);
    dma_hal_clear_bus_err_interrupt_status(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

bk_err_t bk_dma_register_isr(dma_id_t id, dma_isr_t half_finish_isr, dma_isr_t finish_isr)
{
    uint8_t dma_num,dma_channel;
    DMA_RETURN_ON_NOT_INIT();
	dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);
    DMA_RETURN_ON_INVALID_ID(id);
    DMA_LOGD("bk_dma_register_isr %d and %d\r\n",dma_num,dma_channel);
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    s_dma_half_finish_isr[dma_num][dma_channel] = half_finish_isr;
    s_dma_finish_isr[dma_num][dma_channel] = finish_isr;
    GLOBAL_INT_RESTORE();

    return BK_OK;
}

bk_err_t bk_dma_register_bus_err_isr(dma_id_t id, dma_isr_t bus_err_isr)
{
    uint8_t dma_num,dma_channel;
    DMA_RETURN_ON_NOT_INIT();
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);
    DMA_RETURN_ON_INVALID_ID(id);
    DMA_LOGD("bk_dma_register_isr %d and %d\r\n",dma_num,dma_channel);
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    s_dma_bus_err_isr[dma_num][dma_channel] = bus_err_isr;
    GLOBAL_INT_RESTORE();

    return BK_OK;
}

uint32_t bk_dma_get_transfer_len_max(dma_id_t id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    return dma_hal_get_transfer_len_max(&s_dma[dma_num].hal);
}

bk_err_t bk_dma_set_transfer_len(dma_id_t id, uint32_t tran_len)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    if(tran_len > dma_hal_get_transfer_len_max(&s_dma[dma_num].hal))
    {
        DMA_LOGE("%s:dma_id=%d,len=%d over 65536 bytes\r\n",__func__, id, tran_len);
        return BK_ERR_DMA_TRANS_LEN;
    }

    dma_wait_to_idle(id);
    dma_hal_set_transfer_len(&s_dma[dma_num].hal, dma_channel, tran_len);
    return BK_OK;
}

bk_err_t bk_dma_set_src_addr(dma_id_t id, uint32_t start_addr, uint32_t end_addr)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_set_src_start_addr(&s_dma[dma_num].hal, dma_channel, start_addr);
    dma_hal_set_src_loop_addr(&s_dma[dma_num].hal, dma_channel, start_addr, end_addr);
    return BK_OK;
}

bk_err_t bk_dma_set_src_start_addr(dma_id_t id, uint32_t start_addr)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_set_src_start_addr(&s_dma[dma_num].hal, dma_channel, start_addr);
    return BK_OK;
}

bk_err_t bk_dma_set_dest_addr(dma_id_t id, uint32_t start_addr, uint32_t end_addr)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_wait_to_idle(id);
    dma_hal_set_dest_start_addr(&s_dma[dma_num].hal, dma_channel, start_addr);
    dma_hal_set_dest_loop_addr(&s_dma[dma_num].hal, dma_channel, start_addr, end_addr);
    return BK_OK;
}

bk_err_t bk_dma_set_dest_start_addr(dma_id_t id, uint32_t start_addr)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_wait_to_idle(id);
    dma_hal_set_dest_start_addr(&s_dma[dma_num].hal, dma_channel, start_addr);
    return BK_OK;
}

bk_err_t bk_dma_enable_src_addr_increase(dma_id_t id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_enable_src_addr_inc(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

bk_err_t bk_dma_disable_src_addr_increase(dma_id_t id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_disable_src_addr_inc(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

bk_err_t bk_dma_enable_src_addr_loop(dma_id_t id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_enable_src_addr_loop(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

bk_err_t bk_dma_disable_src_addr_loop(dma_id_t id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_disable_src_addr_loop(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

bk_err_t bk_dma_enable_dest_addr_increase(dma_id_t id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_enable_dest_addr_inc(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

bk_err_t bk_dma_disable_dest_addr_increase(dma_id_t id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_disable_dest_addr_inc(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

bk_err_t bk_dma_enable_dest_addr_loop(dma_id_t id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_enable_dest_addr_loop(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

bk_err_t bk_dma_disable_dest_addr_loop(dma_id_t id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_disable_dest_addr_loop(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

uint32_t bk_dma_get_remain_len(dma_id_t id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_ID_NOT_INIT(dma_num,dma_channel);
    return dma_hal_get_remain_len(&s_dma[dma_num].hal, dma_channel);
}

bk_err_t dma_set_src_pause_addr(dma_id_t id, uint32_t addr)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    dma_hal_set_src_pause_addr(&s_dma[dma_num].hal, dma_channel, addr);

    return BK_OK;
}

bk_err_t dma_set_dst_pause_addr(dma_id_t id, uint32_t addr)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    dma_hal_set_dest_pause_addr(&s_dma[dma_num].hal, dma_channel, addr);

    return BK_OK;
}

uint32_t dma_get_src_read_addr(dma_id_t id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    return dma_hal_get_src_read_addr(&s_dma[dma_num].hal, dma_channel);
}

uint32_t dma_get_dest_write_addr(dma_id_t id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    return dma_hal_get_dest_write_addr(&s_dma[dma_num].hal, dma_channel);
}

bk_err_t bk_dma_set_src_data_width(dma_id_t id, dma_data_width_t data_width)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    dma_wait_to_idle(id);
    dma_hal_set_src_data_width(&s_dma[dma_num].hal, dma_channel, data_width);
    return BK_OK;
}

bk_err_t bk_dma_set_dest_data_width(dma_id_t id, dma_data_width_t data_width)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    dma_wait_to_idle(id);
    dma_hal_set_dest_data_width(&s_dma[dma_num].hal, dma_channel, data_width);
    return BK_OK;
}

bk_err_t bk_dma_flush_src_buffer(dma_id_t id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    dma_hal_flush_src_buffer(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

#ifdef CONFIG_SPE
bk_err_t bk_dma_set_pixel_trans_type(dma_id_t id, dma_pixel_trans_type_t type)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);


    dma_hal_set_pixel_trans_type(&s_dma[dma_num].hal, id, type);
    return BK_OK;
}

uint32_t bk_dma_get_pixel_trans_type(dma_id_t id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);


    return dma_hal_get_pixel_trans_type(&s_dma[dma_num].hal, id);
}

bk_err_t bk_dma_set_dest_burst_len(dma_id_t id, dma_burst_len_t len)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    dma_hal_set_dest_burst_len(&s_dma[dma_num].hal, dma_channel, len);
    return BK_OK;
}

uint32_t bk_dma_get_dest_burst_len(dma_id_t id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    return dma_hal_get_dest_burst_len(&s_dma[dma_num].hal, dma_channel);
}

bk_err_t bk_dma_set_src_burst_len(dma_id_t id, dma_burst_len_t len)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    dma_hal_set_src_burst_len(&s_dma[dma_num].hal, dma_channel, len);
    return BK_OK;
}

uint32_t bk_dma_get_src_burst_len(dma_id_t id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    return dma_hal_get_src_burst_len(&s_dma[dma_num].hal, dma_channel);
}

bk_err_t bk_dma_bus_err_int_enable(dma_id_t id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    dma_hal_bus_err_int_enable(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

bk_err_t bk_dma_bus_err_int_diable(dma_id_t id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    dma_hal_bus_err_int_disable(&s_dma[dma_num].hal, dma_channel);
    return BK_OK;
}

bk_err_t bk_dma_set_dest_sec_attr(dma_id_t id, dma_sec_attr_t attr)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    dma_hal_set_dest_sec_attr(&s_dma[dma_num].hal, dma_channel, attr);
    return BK_OK;
}

bk_err_t bk_dma_set_src_sec_attr(dma_id_t id, dma_sec_attr_t attr)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    dma_hal_set_src_sec_attr(&s_dma[dma_num].hal, dma_channel, attr);
    return BK_OK;
}
#endif

#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
bk_err_t bk_dma_set_sec_attr(dma_id_t id, dma_sec_attr_t attr)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    dma_hal_set_sec_attr(&s_dma[dma_num].hal, dma_channel, attr);
    return BK_OK;
}

bk_err_t bk_dma_set_privileged_attr(dma_id_t id, dma_sec_attr_t attr)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    dma_hal_set_privileged_attr(&s_dma[dma_num].hal, dma_channel, attr);
    return BK_OK;
}

bk_err_t bk_dma_set_int_allocate(dma_id_t id,dma_int_id_t int_id)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_set_int_allocate(&s_dma[dma_num].hal,dma_channel,int_id);
    return BK_OK;
}

#endif

uint32_t bk_dma_get_repeat_wr_pause(dma_id_t id)
{
	uint8_t dma_channel,dma_num;
	dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

	return dma_hal_repeat_wr_pause(&s_dma[dma_num].hal, dma_channel);
}

uint32_t bk_dma_get_repeat_rd_pause(dma_id_t id)
{
	uint8_t dma_channel,dma_num;
	dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

	return dma_hal_repeat_rd_pause(&s_dma[dma_num].hal, dma_channel);
}

uint32_t bk_dma_get_finish_interrupt_cnt(dma_id_t id)
{
	uint8_t dma_channel,dma_num;
	dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

	return dma_hal_finish_interrupt_cnt(&s_dma[dma_num].hal, dma_channel);
}

uint32_t bk_dma_get_half_finish_interrupt_cnt(dma_id_t id)
{
	uint8_t dma_channel,dma_num;
	dma_id_to_hw_id_ch(id,&dma_num,&dma_channel);

	return dma_hal_half_finish_interrupt_cnt(&s_dma[dma_num].hal, dma_channel);
}

bk_err_t bk_dma_stateless_judgment_configuration(void *out, const void *in, uint32_t len, dma_id_t cpy_chnl, void *finish_isr)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(cpy_chnl,&dma_num,&dma_channel);

    dma_config_t dma_config;

    os_memset(&dma_config, 0, sizeof(dma_config_t));

    dma_config.mode = DMA_WORK_MODE_REPEAT;
    dma_config.chan_prio = 0;

    dma_config.src.dev = DMA_DEV_DTCM;
    dma_config.src.width = DMA_DATA_WIDTH_32BITS;
    dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
    dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
    dma_config.src.start_addr = (uint32_t)in;
    dma_config.src.end_addr = (uint32_t)(in + len + 4);

    dma_config.dst.dev = DMA_DEV_DTCM;
    dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
    dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
    dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
    dma_config.dst.start_addr = (uint32_t)out;
    dma_config.dst.end_addr = (uint32_t)(out + len + 4);

    /* init */
    s_dma[dma_num].id_init_bits |= BIT(dma_channel);
#if defined(CONFIG_CACHE_ENABLE)
    flush_dcache((void *)dma_config.src.start_addr, dma_config.src.end_addr - dma_config.src.start_addr);
    flush_dcache((void *)dma_config.dst.start_addr, dma_config.dst.end_addr - dma_config.dst.start_addr);
#endif
    dma_hal_init_dma(&s_dma[dma_num].hal, dma_channel, &dma_config);

    /* register isr */
    s_dma_finish_isr[dma_num][dma_channel] = (dma_isr_t)finish_isr;

    /* enable or disable finish interrupt*/
    if(finish_isr) {
        dma_id_enable_interrupt_common(dma_num);
        dma_hal_enable_finish_interrupt(&s_dma[dma_num].hal, dma_channel);
    }

    return BK_OK;

}

bk_err_t dma_memcpy_by_chnl(void *out, const void *in, uint32_t len, dma_id_t cpy_chnl)
{
    uint8_t dma_channel,dma_num;
    dma_id_to_hw_id_ch(cpy_chnl,&dma_num,&dma_channel);

    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(cpy_chnl);

    dma_config_t dma_config;

    os_memset(&dma_config, 0, sizeof(dma_config_t));

    dma_config.mode = DMA_WORK_MODE_SINGLE;
    dma_config.chan_prio = 0;

    dma_config.src.dev = DMA_DEV_DTCM;
    dma_config.src.width = DMA_DATA_WIDTH_32BITS;
    dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
    dma_config.src.start_addr = (uint32_t)in;
    dma_config.src.end_addr = (uint32_t)(in + len);

    dma_config.dst.dev = DMA_DEV_DTCM;
    dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
    dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
    dma_config.dst.start_addr = (uint32_t)out;
    dma_config.dst.end_addr = (uint32_t)(out + len);

    DMA_LOGD("dma_memcpy cpy_chnl: %d\r\n", cpy_chnl);

    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();

    dma_wait_to_idle(cpy_chnl);

    bk_dma_init(cpy_chnl, &dma_config);
    dma_hal_set_transfer_len(&s_dma[dma_num].hal, dma_channel, len);
#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
    dma_hal_set_src_sec_attr(&s_dma[dma_num].hal, dma_channel, DMA_ATTR_SEC);
    dma_hal_set_dest_sec_attr(&s_dma[dma_num].hal, dma_channel, DMA_ATTR_SEC);
#endif
    dma_hal_start_common(&s_dma[dma_num].hal, dma_channel);
    GLOBAL_INT_RESTORE();

//TODO:I think no need to wait copy data finish,just confirm before copy start, the previous one is finish.
    BK_WHILE(dma_hal_get_enable_status(&s_dma[dma_num].hal, dma_channel));

    return BK_OK;

}

bk_err_t dma_memcpy(void *out, const void *in, uint32_t len)
{
    DMA_RETURN_ON_NOT_INIT();

    bk_err_t ret;
    dma_id_t cpy_chnl = bk_dma_alloc(DMA_DEV_DTCM);
	uint8_t dma_channel,dma_num;
	dma_id_to_hw_id_ch(cpy_chnl,&dma_num,&dma_channel);
    DMA_RETURN_ON_INVALID_ID(cpy_chnl);

    ret = dma_memcpy_by_chnl(out, in, len, cpy_chnl);

#if defined(CONFIG_CACHE_ENABLE)
    flush_all_dcache();
#endif

    bk_dma_free(DMA_DEV_DTCM, cpy_chnl);

    return ret;
}

static void dma_isr_common(dma_unit_t dma_unit_id)
{
    dma_hal_t *hal = &s_dma[dma_unit_id].hal;
    uint32_t channel = 0;
    for (int id = 0; id < SOC_DMA_CHAN_NUM_PER_UNIT; id++) {
        channel = id + dma_unit_id * SOC_DMA_CHAN_NUM_PER_UNIT;
        if (((channel) < CONFIG_DMA_LOGIC_CHAN_ID_MIN) || ((channel) >= CONFIG_DMA_LOGIC_CHAN_ID_MIN + CONFIG_DMA_LOGIC_CHAN_CNT)) {
            continue;
        }
        if (dma_hal_is_half_finish_interrupt_triggered(hal, id)) {
            DMA_LOGD("dma_isr HALF FINISH TRIGGERED! id: %d\r\n", id);
            //NOTES:clear intrrupt in condition because maybe multi-core(two CPU) access one DMA
            //it can't cleared peer-side channels status.
            if (s_dma_half_finish_isr[dma_unit_id][id]) {
                DMA_LOGD("dma_isr HALF_finish_isr! id: %d\r\n", id);
                dma_hal_clear_half_finish_interrupt_status(hal, id);
                s_dma_half_finish_isr[dma_unit_id][id](channel);
            }
        }
        if (dma_hal_is_finish_interrupt_triggered(hal, id)) {
#if defined(CONFIG_CACHE_ENABLE)
            flush_all_dcache();
#endif
            DMA_LOGD("dma_isr ALL FINISH TRIGGERED! id: %d\r\n", id);
            if (s_dma_finish_isr[dma_unit_id][id]) {
                DMA_LOGD("dma_isr ALL_finish_isr! id: %d\r\n", id);
                dma_hal_clear_finish_interrupt_status(hal, id);
                s_dma_finish_isr[dma_unit_id][id](channel);
            }
        }

           if (dma_hal_is_bus_err_interrupt_triggered(hal, id)) {
           DMA_LOGE("dma_isr ALL FINISH TRIGGERED! id: %d\r\n", id);
           if (s_dma_bus_err_isr[dma_unit_id][id]) {
               DMA_LOGE("dma_isr ALL_finish_isr! id: %d\r\n", id);
               dma_hal_clear_finish_interrupt_status(hal, id);
               s_dma_bus_err_isr[dma_unit_id][id](channel);
           }
        }

   }
}

static void dma_isr(void)
{
	DMA_LOGD("dma_isr hw 0\r\n");
	dma_isr_common(0);
}
static void dma1_isr(void)
{
	DMA_LOGD("dma_isr hw 1\r\n");
	dma_isr_common(1);
}

