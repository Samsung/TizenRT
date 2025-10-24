/**
 * @if copyright_display
 *      Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
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

#include "dubhe_event.h"
#include "pal_semaphore.h"
#include "pal_mutex.h"
#include "pal_signal.h"
#include "pal_log.h"
#include "pal.h"
#include "dubhe_sca.h"
#if defined( DUBHE_FOR_RUNTIME )
#if !defined( TEE_M )
static semaphore_t _g_aca_done_sem       = NULL;
static semaphore_t _g_aca_fifo_ready_sem = NULL;
static semaphore_t _g_sca_cmd_sem        = NULL;
static semaphore_t _g_hash_cmd_sem       = NULL;

static pal_mutex_t _g_sca_mutex  = NULL;
static int32_t _g_sca_status;
static pal_mutex_t _g_hash_mutex = NULL;
static int32_t _g_hash_status;
static pal_mutex_t _g_aca_mutex  = NULL;
static pal_mutex_t _g_trng_mutex = NULL;
static pal_mutex_t _g_otp_mutex = NULL;

void dubhe_event_set_sca_status(int32_t status){
    _g_sca_status = status;
}

void dubhe_event_set_hash_status(int32_t status){
    _g_hash_status = status;
}

#else
#include "dubhe_intr_handler.h"

static uint32_t _g_signal_mask = 0;
void dubhe_event_set_sca_status(int32_t status){
    (void)status;
}

void dubhe_event_set_hash_status(int32_t status){
    (void)status;
}
#endif
#endif

int32_t dubhe_event_init( void )
{
    int32_t ret = 0;
#if defined( DUBHE_FOR_RUNTIME )
#if !defined( TEE_M )
    pal_sema_init( &_g_aca_done_sem, 0 );
    if ( NULL == _g_aca_done_sem ) {
        return ( -1 );
    }

    pal_sema_init( &_g_aca_fifo_ready_sem, 0 );
    if ( NULL == _g_aca_fifo_ready_sem ) {
        ret = ( -1 );
        goto cleanup1;
    }

    pal_sema_init( &_g_sca_cmd_sem, 0 );
    if ( NULL == _g_sca_cmd_sem ) {
        ret = ( -1 );
        goto cleanup2;
    }

    pal_sema_init( &_g_hash_cmd_sem, 0 );
    if ( NULL == _g_hash_cmd_sem ) {
        ret = ( -1 );
        goto cleanup3;
    }

    _g_sca_mutex = pal_mutex_init( );
    if ( NULL == _g_sca_mutex ) {
        ret = ( -1 );
        goto cleanup4;
    }
    _g_hash_mutex = pal_mutex_init( );
    if ( NULL == _g_hash_mutex ) {
        ret = ( -1 );
        goto cleanup5;
    }
    _g_aca_mutex = pal_mutex_init( );
    if ( NULL == _g_aca_mutex ) {
        ret = ( -1 );
        goto cleanup6;
    }
    _g_trng_mutex = pal_mutex_init( );
    if ( NULL == _g_trng_mutex ) {
        ret = ( -1 );
        goto cleanup7;
    }
    _g_otp_mutex = pal_mutex_init( );
    if ( NULL == _g_otp_mutex ) {
        ret = ( -1 );
        goto cleanup8;
    }

    ret = 0;
    goto _exit;
cleanup8:
    pal_mutex_destroy(_g_trng_mutex);
cleanup7:
    pal_mutex_destroy(_g_aca_mutex);
cleanup6:
    pal_mutex_destroy(_g_hash_mutex);
cleanup5:
    pal_mutex_destroy(_g_sca_mutex);
cleanup4:
    pal_sema_destroy(_g_hash_cmd_sem);
cleanup3:
    pal_sema_destroy(_g_sca_cmd_sem);
cleanup2:
    pal_sema_destroy(_g_aca_fifo_ready_sem);
cleanup1:
    pal_sema_destroy(_g_aca_done_sem);
_exit:
#else
    _g_signal_mask = pal_signal_init( );
#endif
#endif
    return ( ret );
}


void dubhe_event_cleanup( void )
{
#if defined( DUBHE_FOR_RUNTIME )
#if !defined( TEE_M )
    if ( NULL != _g_aca_done_sem ) {
       pal_sema_destroy(_g_aca_done_sem);
       _g_aca_done_sem = NULL;
    }

    if ( NULL != _g_aca_fifo_ready_sem ) {
       pal_sema_destroy(_g_aca_fifo_ready_sem);
       _g_aca_fifo_ready_sem = NULL;
    }

    if ( NULL != _g_sca_cmd_sem ) {
        pal_sema_destroy(_g_sca_cmd_sem);
        _g_sca_cmd_sem = NULL;
    }

    if ( NULL != _g_hash_cmd_sem ) {
        pal_sema_destroy(_g_hash_cmd_sem);
        _g_hash_cmd_sem = NULL;
    }

    if ( NULL != _g_sca_mutex ) {
        pal_mutex_destroy(_g_sca_mutex);
        _g_sca_mutex = NULL;
    }

    if ( NULL != _g_hash_mutex ) {
        pal_mutex_destroy(_g_hash_mutex);
        _g_hash_mutex = NULL;
    }

    if ( NULL != _g_aca_mutex ) {
        pal_mutex_destroy(_g_aca_mutex);
        _g_aca_mutex = NULL;
    }

    if ( NULL != _g_trng_mutex ) {
        pal_mutex_destroy(_g_trng_mutex);
        _g_trng_mutex = NULL;
    }

    if ( NULL != _g_otp_mutex ) {
        pal_mutex_destroy(_g_otp_mutex);
        _g_otp_mutex = NULL;
    }
#else
    _g_signal_mask = 0;
#endif
#endif
}


#if defined( DUBHE_FOR_RUNTIME )
int32_t dubhe_wait_event( dubhe_event_type_t dubhe_event )
{
#if !defined( TEE_M )
    int32_t ret = 0;
    switch ( dubhe_event ) {
    case DBH_EVENT_SCA_CMD_EXCUTED:
        pal_sema_down( _g_sca_cmd_sem );
        ret = _g_sca_status;
        break;
    case DBH_EVENT_HASH_CMD_EXCUTED:
        pal_sema_down( _g_hash_cmd_sem );
        ret = _g_hash_status;
        break;
    case DBH_EVENT_ACA_DONE:
        pal_sema_down( _g_aca_done_sem );
        break;
    case DBH_EVENT_ACA_FIFO_READY:
        pal_sema_down( _g_aca_fifo_ready_sem );
        break;
    default:
        ret = -1;
    }

    return ret;
#else
    int ret = -1;
    uint32_t signal;

    do {
        signal = pal_wait_signal( _g_signal_mask, 0 );
        if ( _g_signal_mask == signal ) {
            ret = dubhe_intr_sync_handler( dubhe_event );
            pal_clear_signal( _g_signal_mask );

            switch ( dubhe_event ) {
            case DBH_EVENT_SCA_CMD_EXCUTED:
                if (( ret != -1 ) && ( ret < 0 )) {
                    return ret;
                }
                break;
            case DBH_EVENT_HASH_CMD_EXCUTED:
                if (( ret != -1 ) && ( ret < 0 )) {
                    return ret;
                }
                break;
            case DBH_EVENT_ACA_DONE:
            case DBH_EVENT_ACA_FIFO_READY:
            default:
                break;
            }
        }
    } while ( ret == -1 );

    return ( 0 );
#endif
}
#endif

#if defined( DUBHE_FOR_RUNTIME )
int32_t dubhe_release_event( dubhe_event_type_t dubhe_event )
{
#if !defined( TEE_M )
    switch ( dubhe_event ) {
    case DBH_EVENT_SCA_CMD_EXCUTED:
        pal_sema_up( _g_sca_cmd_sem );
        break;
    case DBH_EVENT_HASH_CMD_EXCUTED:
        pal_sema_up( _g_hash_cmd_sem );
        break;
    case DBH_EVENT_ACA_DONE:
        pal_sema_up( _g_aca_done_sem );
        break;
    case DBH_EVENT_ACA_FIFO_READY:
        pal_sema_up( _g_aca_fifo_ready_sem );
        break;
    default:
        return ( -1 );
    }
#else
    (void) dubhe_event;
#endif
    return ( 0 );
}
#endif

int32_t dubhe_mutex_lock( dubhe_mutex_type_t dubhe_mutex )
{
#if defined( DUBHE_FOR_RUNTIME )
#if !defined( TEE_M )
    int ret = MUTEX_UNLOCK_FAIL;
    switch ( dubhe_mutex ) {
    case DBH_SCA_MUTEX:
        ret = pal_mutex_lock( _g_sca_mutex );
        break;
    case DBH_HASH_MUTEX:
        ret = pal_mutex_lock( _g_hash_mutex );
        break;
    case DBH_ACA_MUTEX:
//        ret = pal_mutex_lock( _g_aca_mutex );
        break;
    case DBH_TRNG_MUTEX:
        ret = pal_mutex_lock( _g_trng_mutex );
        break;
    case DBH_OTP_MUTEX:
        ret = pal_mutex_lock( _g_otp_mutex );
        break;
    default:
        return MUTEX_UNLOCK_FAIL;
    }
    return ret;
#else
    (void) dubhe_mutex;
    return ( 0 );
#endif
#else
    (void) dubhe_mutex;
    return ( 0 );
#endif
}

int32_t dubhe_mutex_unlock( dubhe_mutex_type_t dubhe_mutex )
{
#if defined( DUBHE_FOR_RUNTIME )
#if !defined( TEE_M )
    int ret = MUTEX_UNLOCK_FAIL;
    switch ( dubhe_mutex ) {
    case DBH_SCA_MUTEX:
        ret = pal_mutex_unlock( _g_sca_mutex );
        break;
    case DBH_HASH_MUTEX:
        ret = pal_mutex_unlock( _g_hash_mutex );
        break;
    case DBH_ACA_MUTEX:
  //      ret = pal_mutex_unlock( _g_aca_mutex );
        break;
    case DBH_TRNG_MUTEX:
        ret = pal_mutex_unlock( _g_trng_mutex );
        break;
    case DBH_OTP_MUTEX:
        ret = pal_mutex_unlock( _g_otp_mutex );
        break;
    default:
        return MUTEX_UNLOCK_FAIL;
    }
    return ret;
#else
    (void) dubhe_mutex;
    return ( 0 );
#endif
#else
    (void) dubhe_mutex;
    return ( 0 );
#endif
}



/*************************** The End Of File*****************************/
