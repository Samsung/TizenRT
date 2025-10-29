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

#if defined( DUBHE_FOR_RUNTIME )

#include <stdint.h>
#include "dubhe_regs.h"
#include "dubhe_aca.h"
#include "dubhe_sca.h"
#include "dubhe_hash.h"
#include "dubhe_trng.h"
#include "dubhe_intr_handler.h"
#include "dubhe_driver_internal.h"
#include "dubhe_aca_internal.h"
#include "pal_log.h"
#include "dubhe_event.h"

#define BIT_MASK( V, M ) ( ( V ) & ( M ) )

static void dubhe_aca_intr_handler( void )
{
    volatile uint32_t value = 0;
    uint32_t tmp = 0;
    volatile uint32_t output_reg_val;

    value = DBH_READ_REGISTER( ACA, ACA_INTR_STAT );

    if ( BIT_MASK( value, DBH_ACA_FIFO_UNDER_WM_INTR ) ) {
        output_reg_val = DBH_READ_REGISTER( ACA, ACA_INTR_MASK );
        tmp = DBH_REG_FLD_GET(ACA_INTR_MASK,FIFO_UNDER_WM,output_reg_val);
        if(tmp == 0){
            DBH_REG_FLD_SET( ACA_INTR_MASK, FIFO_UNDER_WM, output_reg_val, 0x1 );
            DBH_WRITE_REGISTER( ACA, ACA_INTR_MASK, output_reg_val );
            dubhe_release_event( DBH_EVENT_ACA_FIFO_READY );
        }
    }

    if ( BIT_MASK( value, DBH_ACA_ENGINE_DONE_INTR ) ) {
        output_reg_val = DBH_READ_REGISTER( ACA, ACA_INTR_MASK );
        tmp = DBH_REG_FLD_GET(ACA_INTR_MASK,ACA_DONE,output_reg_val);
        if(tmp == 0){
            DBH_REG_FLD_SET( ACA_INTR_MASK, ACA_DONE, output_reg_val, 0x1 );
            DBH_WRITE_REGISTER( ACA, ACA_INTR_MASK, output_reg_val );
            dubhe_release_event( DBH_EVENT_ACA_DONE );
        }
    }

    if ( BIT_MASK( value, DBH_ACA_CMD_FINISH_INTR ) ) {
        PAL_LOG_ERR( "value(%X) unexpect interupt DBH_ACA_CMD_FINISH_INTR!! \n",
                     value );
    }

    if ( BIT_MASK( value, DBH_ACA_FIFO_OVERFLOW_INTR ) ) {
        PAL_LOG_ERR( "value(%X) ACA process error unexpect interupt "
                     "DBH_ACA_FIFO_OVERFLOW_INTR \n",
                     value );
    }

    if ( BIT_MASK( value, DBH_ACA_DIV_ZERO_INTR ) ) {
        PAL_LOG_ERR( "value(%X) ACA process error unexpect interupt "
                     "DBH_ACA_DIV_ZERO_INTR \n",
                     value );
    }

    if ( BIT_MASK( value, DBH_ACA_MODINV_ZERO_INTR ) ) {
        PAL_LOG_ERR( "value(%X) ACA process error unexpect interupt "
                     "DBH_ACA_MODINV_ZERO_INTR \n",
                     value );
    }

    if ( BIT_MASK( value, DBH_ACA_MULT_RED_ERR_INTR ) ) {
        PAL_LOG_ERR( "value(%X) ACA process error unexpect interupt "
                     "DBH_ACA_MULT_RED_ERR_INTR \n",
                     value );
    }

    /*write clear the interrupt*/
    DBH_WRITE_REGISTER( ACA, ACA_INTR_STAT, value );
}

static uint32_t dubhe_sca_intr_handler( void )
{
    volatile uint32_t value = 0;

    value = DBH_READ_REGISTER( SCA, SCA_INTR_STAT );

    if ( BIT_MASK( value, DBH_SCA_CMD_INTR ) ) {
        dubhe_event_set_sca_status(0x00);
        dubhe_release_event( DBH_EVENT_SCA_CMD_EXCUTED );
    }

    if ( BIT_MASK( value, DBH_SCA_INVAL_CMD_INTR ) ) {
        dubhe_event_set_sca_status(DBH_SCA_INVALID_CMD_ERR);
        dubhe_release_event( DBH_EVENT_SCA_CMD_EXCUTED );
        PAL_LOG_ERR( "value(%X) SCA process error unexpect interupt "
                     "DBH_SCA_INVAL_CMD_INTR \n",
                     value );
    }

    if ( BIT_MASK( value, DBH_SCA_INVAL_KEY_ERR_INTR ) ) {
        dubhe_event_set_sca_status(DBH_SCA_KEY_TYPE_INVAL_ERR);
        dubhe_release_event( DBH_EVENT_SCA_CMD_EXCUTED );
        PAL_LOG_ERR( "value(%X) SCA process error unexpect interupt "
                     "DBH_SCA_INVAL_KEY_ERR_INTR \n",
                     value );
    }

    if ( BIT_MASK( value, DBH_SCA_BUS_ERR_INTR ) ) {
        dubhe_event_set_sca_status(DBH_SCA_BUS_ERR);
        dubhe_release_event( DBH_EVENT_SCA_CMD_EXCUTED );
        PAL_LOG_ERR( "value(%X) SCA process error unexpect interupt "
                     "DBH_SCA_BUS_ERR_INTR \n",
                     value );
    }

    if ( BIT_MASK( value, DBH_SCA_WM_INTR ) ) {
        dubhe_event_set_sca_status(DBH_SCA_INFO_WM);
        dubhe_release_event( DBH_EVENT_SCA_CMD_EXCUTED );
        PAL_LOG_ERR(
            "value(%X) SCA process error unexpect interupt DBH_SCA_WM_INTR \n",
            value );
    }

    /*write clear the interrupt*/
    DBH_WRITE_REGISTER( SCA, SCA_INTR_STAT, value );

    return ( value );
}

static uint32_t dubhe_hash_intr_handler( void )
{
    volatile uint32_t value = 0;

    value = DBH_READ_REGISTER( HASH, HASH_INTR_STAT );
    /* Watermark interrupt is ignored, so &0xF */

    if ( BIT_MASK( value, DBH_HASH_CMD_INTR ) ) {
        dubhe_event_set_hash_status(0x00);
        dubhe_release_event( DBH_EVENT_HASH_CMD_EXCUTED );
    }

    if ( BIT_MASK( value, DBH_HASH_INVALID_CMD_INTR ) ) {
        dubhe_event_set_hash_status(DBH_HASH_INVALID_CMD_ERR);
        dubhe_release_event( DBH_EVENT_HASH_CMD_EXCUTED );
        PAL_LOG_ERR( "value(%X) HASH process error unexpect interupt "
                     "DBH_HASH_INVALID_CMD_INTR \n",
                     value );
    }

    if ( BIT_MASK( value, DBH_HASH_PADDING_ERR_INTR ) ) {
        dubhe_event_set_hash_status(DBH_HASH_PADDING_ERR);
        dubhe_release_event( DBH_EVENT_HASH_CMD_EXCUTED );
        PAL_LOG_ERR( "value(%X) HASH process error unexpect interupt "
                     "DBH_HASH_PADDING_ERR_INTR \n",
                     value );
    }

    if ( BIT_MASK( value, DBH_HASH_BUS_ERR_INTR ) ) {
        dubhe_event_set_hash_status(DBH_HASH_BUS_ERR);
        dubhe_release_event( DBH_EVENT_HASH_CMD_EXCUTED );
        PAL_LOG_ERR( "value(%X) HASH process error unexpect interupt "
                     "DBH_HASH_BUS_ERR_INTR \n",
                     value );
    }

    if ( BIT_MASK( value, DBH_HASH_WM_INTR ) ) {
        dubhe_event_set_hash_status(DBH_HASH_INFO_WM);
        dubhe_release_event( DBH_EVENT_HASH_CMD_EXCUTED );
        PAL_LOG_ERR(
            "value(%X) HASH process error unexpect interupt DBH_HASH_WM_INTR \n",
            value );
    }
    /*write clear the interrupt*/
    DBH_WRITE_REGISTER( HASH, HASH_INTR_STAT, value );

    return ( value );
}

int dubhe_intr_handler( void )
{
    volatile uint32_t value = 0;

    value = DBH_READ_REGISTER( TOP_STAT, TOP_INTR_STAT_HOST0 );
    value &= 0x7F;

    if(BIT_MASK(value,DBH_STAT_ACA0_INTR)){
        dubhe_aca_intr_handler( );
    }
    if(BIT_MASK(value,DBH_STAT_HASH0_INTR)){
        dubhe_hash_intr_handler( );
    }
    if(BIT_MASK(value,DBH_STAT_SCA0_INTR)){
        dubhe_sca_intr_handler( );
    }
    if(BIT_MASK(value,DBH_STAT_TRNG_INTR)){
        PAL_LOG_ERR( "!!!currently TRNG interrupt is not used !\n" );
    }

    return ( 0 );
}

int dubhe_intr_sync_handler( dubhe_event_type_t dubhe_event )
{
    int ret;
    uint32_t status = STATUS_INTR_UNHAPPEN;
    volatile uint32_t value = 0;

    value = DBH_READ_REGISTER( TOP_STAT, TOP_INTR_STAT_HOST0 );
    value &= 0x7F;
    switch ( dubhe_event ) {
    case DBH_EVENT_SCA_CMD_EXCUTED:
        ret = dubhe_sca_intr_handler( );
        if ( BIT_MASK( ret, DBH_SCA_INVAL_CMD_INTR ) ) {
            ret = DBH_SCA_INVALID_CMD_ERR;
            goto exit;
        } else if ( BIT_MASK( ret, DBH_SCA_INVAL_KEY_ERR_INTR ) ) {
            ret = DBH_SCA_KEYBITS_INVAL_ERR;
            goto exit;
        } else if ( BIT_MASK( ret, DBH_SCA_BUS_ERR_INTR ) ) {
            ret = DBH_SCA_BUS_ERR;
            goto exit;
        } else if ( BIT_MASK( ret, DBH_SCA_CMD_INTR ) ) {
            status = STATUS_INTR_EXPECT_OCCUR;
        } else {
            status = STATUS_INTR_UNHAPPEN;
        }
        break;
    case DBH_EVENT_HASH_CMD_EXCUTED:
        ret = dubhe_hash_intr_handler( );
        if ( BIT_MASK( ret, DBH_HASH_BUS_ERR_INTR ) ) {
            ret = DBH_HASH_BUS_ERR;
            goto exit;
        } else if ( BIT_MASK( ret, DBH_HASH_INVALID_CMD_INTR ) ) {
            ret = DBH_HASH_INVALID_CMD_ERR;
            goto exit;
        } else if ( BIT_MASK( ret, DBH_HASH_PADDING_ERR_INTR ) ) {
            ret = DBH_HASH_PADDING_ERR;
            goto exit;
        } else if ( BIT_MASK( ret, DBH_HASH_CMD_INTR ) ) {
            status = STATUS_INTR_EXPECT_OCCUR;
        } else {
            status = STATUS_INTR_UNHAPPEN;
        }
        break;
    case DBH_EVENT_ACA_DONE:
        if ( DBH_STAT_ACA0_INTR == ( value & 0x01 ) ) {
            dubhe_aca_intr_handler( );
            ACA_GET_ACA_DONE_STATUS( status );

	    status = STATUS_INTR_EXPECT_OCCUR;
        }
        break;
    case DBH_EVENT_ACA_FIFO_READY:
        if ( DBH_STAT_ACA0_INTR == ( value & 0x01 ) ) {
            dubhe_aca_intr_handler( );
            ACA_GET_ACA_FIFO_READY_STATUS( status );

	    status = STATUS_INTR_EXPECT_OCCUR;
        }
        break;
    default:
        return ( -1 );
    }

    if (STATUS_INTR_EXPECT_OCCUR == status ) {
        ret = 0;
    } else {
        ret = -1;
    }

exit:
    return ( ret );
}
#endif /*DUBHE_FOR_RUNTIME*/
/*************************** The End Of File*****************************/
