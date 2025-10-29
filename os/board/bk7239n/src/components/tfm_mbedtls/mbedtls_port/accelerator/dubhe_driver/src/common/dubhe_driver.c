/**
 * @if copyright_display
 *      Copyright (C), 2018-2021, Arm Technology (China) Co., Ltd.
 *      All rights reserved
 *
 *      The content of this file or document is CONFIDENTIAL and PROPRIETARY
 *      to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 *      License Agreement between Licensee and Arm Technology (China) Co., Ltd
 *      restricting among other things, the use, reproduction, distribution
 *      and transfer.  Each of the embodiments, including this information and
 *      any derivative work shall retain this copyright notice.
 * @endif
 */

#include <stdint.h>
#include "pal_log.h"
#include "dubhe_event.h"
#include "dubhe_regs.h"
#include "dubhe_aca.h"
#include "dubhe_sca.h"
#include "dubhe_hash.h"
#include "dubhe_trng.h"
#include "dubhe_otp.h"
#include "dubhe_driver.h"
#include "system_hw.h"

unsigned long _g_Dubhe_RegBase;

static void dubhe_dma_disable(void)
{
    uint32_t value = 0;

    value = DBH_READ_REGISTER( TOP_CTRL, CLK_CTRL );
    DBH_REG_FLD_SET( CLK_CTRL, DMA_AHB_EN, value, 0 );
    DBH_WRITE_REGISTER( TOP_CTRL, CLK_CTRL, value );
}

int dubhe_clk_enable( dubhe_module_type_t type )
{
    int ret        = 0;
    uint32_t value = 0;

    value = DBH_READ_REGISTER( TOP_CTRL, CLK_CTRL );
    switch ( type ) {
    case DBH_MODULE_HASH:
        DBH_REG_FLD_SET( CLK_CTRL, HASH_EN, value, 1 );
        break;
    case DBH_MODULE_SCA:
        DBH_REG_FLD_SET( CLK_CTRL, SCA_EN, value, 1 );
        break;
    case DBH_MODULE_ACA:
        DBH_REG_FLD_SET( CLK_CTRL, ACA_EN, value, 1 );
        break;
    case DBH_MODULE_OTP:
        DBH_REG_FLD_SET( CLK_CTRL, OTP_EN, value, 1 );
        break;
    case DBH_MODULE_TRNG:
        DBH_REG_FLD_SET( CLK_CTRL, TRNG_EN, value, 1 );
        break;
    default:
        PAL_LOG_ERR( "invalid dubhe module %d\n", type );
        ret = -1;
        return ret;
    }

    DBH_WRITE_REGISTER( TOP_CTRL, CLK_CTRL, value );

    return ret;
}

int dubhe_clk_disable( dubhe_module_type_t type )
{
    int ret        = 0;
    uint32_t value = 0;

    value = DBH_READ_REGISTER( TOP_CTRL, CLK_CTRL );
    switch ( type ) {
    case DBH_MODULE_HASH:
        DBH_REG_FLD_SET( CLK_CTRL, HASH_EN, value, 0 );
        break;
    case DBH_MODULE_SCA:
        DBH_REG_FLD_SET( CLK_CTRL, SCA_EN, value, 0 );
        break;
    case DBH_MODULE_ACA:
        DBH_REG_FLD_SET( CLK_CTRL, ACA_EN, value, 0 );
        break;
    case DBH_MODULE_OTP:
        DBH_REG_FLD_SET( CLK_CTRL, OTP_EN, value, 0 );
        break;
    case DBH_MODULE_TRNG:
        DBH_REG_FLD_SET( CLK_CTRL, TRNG_EN, value, 0 );
        break;
    default:
        PAL_LOG_ERR( "invalid dubhe module %d\n", type );
        ret = -1;
        return ret;
    }

    DBH_WRITE_REGISTER( TOP_CTRL, CLK_CTRL, value );

    return ret;
}

int dubhe_driver_init( unsigned long dbh_base_addr )
{
    /* power on encp module */
    sys_ll_set_cpu_power_sleep_wakeup_pwd_encp(0);

    _g_Dubhe_RegBase = dbh_base_addr;

    dubhe_dma_disable();

#if defined( ARM_CE_DUBHE_ACA )
    dubhe_aca_driver_init( );
#endif
#if defined( ARM_CE_DUBHE_HASH )
    dubhe_clk_enable( DBH_MODULE_HASH );
    arm_ce_hash_driver_init( );
#endif
#if defined( ARM_CE_DUBHE_SCA )
    arm_ce_sca_driver_init( );
#endif
#if defined( ARM_CE_DUBHE_TRNG )
    arm_ce_trng_driver_init( );
#endif
#if defined( ARM_CE_DUBHE_OTP )
    arm_ce_otp_driver_init( );
#endif

#if defined( DUBHE_FOR_RUNTIME )
    dubhe_event_init( );
#endif

    return 0;
}

void dubhe_driver_cleanup( void )
{
#if defined( ARM_CE_DUBHE_ACA )
    dubhe_aca_driver_cleanup( );
    extern void mbedtls_dubhe_cleanup( );
    mbedtls_dubhe_cleanup( );
#endif

    dubhe_event_cleanup( );
}

/*************************** The End Of File*****************************/
