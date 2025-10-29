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

#include "dubhe_regs.h"
#include "dubhe_trng.h"
#include "dubhe_driver.h"
#include "pal_string.h"
#include "pal_log.h"
#include "pal_time.h"
#include "dubhe_event.h"

#define POOL_SIZE ( 8 )
#define WAIT_POOL_FILLED_US ( 3U )
#define BIT_MASK( V, M ) ( ( V ) & ( M ) )

#define _ALIGN_UP( addr, size )                                                \
    ( ( ( addr ) + (size) -1 ) & ( ~( (size) -1 ) ) )
#define _ALARM_LINE( _size ) ( ( _size * 8 ) / 256 )

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define PUT_UINT32_LE( n, b, i )                                               \
    {                                                                          \
        ( b )[( i )]     = (unsigned char) ( ( ( n ) >> 24 ) & 0xFF );         \
        ( b )[( i ) + 1] = (unsigned char) ( ( ( n ) >> 16 ) & 0xFF );         \
        ( b )[( i ) + 2] = (unsigned char) ( ( ( n ) >> 8 ) & 0xFF );          \
        ( b )[( i ) + 3] = (unsigned char) ( ( ( n ) ) & 0xFF );               \
    }
#else
#define PUT_UINT32_LE( n, b, i )                                               \
    {                                                                          \
        ( b )[( i )]     = (unsigned char) ( ( ( n ) ) & 0xFF );               \
        ( b )[( i ) + 1] = (unsigned char) ( ( ( n ) >> 8 ) & 0xFF );          \
        ( b )[( i ) + 2] = (unsigned char) ( ( ( n ) >> 16 ) & 0xFF );         \
        ( b )[( i ) + 3] = (unsigned char) ( ( ( n ) >> 24 ) & 0xFF );         \
    }
#endif

/*  default config for dubhe trng */
static const dbh_trng_config_t trng_config = {
        .src = {
            .grp0_en = 1,
            .grp1_en = 1,
            .grp2_en = 1,
            .grp3_en = 1,
            .src_sel = SRC_INTERNAL,
            .rsvd = 0,
        },
        .sample = {
            .div = 100,
            .dly = 3,
            .rsvd = 0,
        },
        .ro = {
            .grp0_taps_en = 4,
            .grp1_taps_en = 5,
            .grp2_taps_en = 6,
            .grp3_taps_en = 7,
            .rsvd3 = 0,
            .rsvd7 = 0,
            .rsvd11 = 0,
            .rsvd15 = 0,
        },
        .postproc = {
            .prng_bypass = 1,
            .vn_bypass = 0,
            .crng_bypass = 0,
            .rsvd3 = 0,
            .lfsr_drop_num = 0,
            .lfsr_sel = 0,
            .fast_mode = 1,
            .rsvd15 = 0,
        },
        .eval = {
            .adap_tst_th = 589,
            .rep_tst_th = 11,
            .adap_tst_en = 1,
            .rep_tst_en = 1,
            .ac_tst_en = 1,
            .ac_tst_th = 10,
            .rsvd = 0,
        },
        .thr = {
            .vn_err_th = 1,
            .crng_err_th = 1,
            .rep_err_th = 1,
            .adap_err_th = 1,
        }
};

/* handler for handle trng error */
__attribute__( ( __weak__ ) ) void on_trng_error( int errno )
{
    (void) errno;
}

/* handler for produced random data data */
__attribute__( ( __weak__ ) ) void on_trng_produce_data( void *data,
                                                         size_t size )
{
    (void) data;
    (void) size;
}

static void arm_ce_trng_setup( const dbh_trng_config_t *trng_cfg )
{
    uint32_t value = 0;
    dbh_trng_int_t int_mask = {
        .vn_err = 1,
        .crng_err = 1,
        .rep_tst_err = 1,
        .adap_tst_err = 1,
    };
    union {
        dbh_trng_entropy_src_t src;
        dbh_trng_entropy_sample_t sample;
        dbh_trng_ro_t ro;
        dbh_trng_postproc_t postproc;
        dbh_trng_eval_t eval;
        dbh_trng_err_thr_t thr;
        dbh_trng_int_t intr;
        uint32_t val;
    } u = {0};

    /* set entropy source */
    u.src = trng_cfg->src;
    DBH_WRITE_REGISTER( TRNG_CTRL, TRNG_ENTROPY_SRC, u.val );
    /* set entropy sample */
    u.sample = trng_cfg->sample;
    DBH_WRITE_REGISTER( TRNG_CTRL, TRNG_ENTROPY_SAMPLE, u.val );
    /* set ro setting */
    u.ro = trng_cfg->ro;
    DBH_WRITE_REGISTER( TRNG_CTRL, TRNG_RO_SETTING, u.val );
    /* set post process */
    u.postproc = trng_cfg->postproc;
    DBH_WRITE_REGISTER( TRNG_CTRL, TRNG_POST_PROCESS, u.val );
    /* set evaluation setting */
    u.eval = trng_cfg->eval;
    DBH_WRITE_REGISTER( TRNG_CTRL, TRNG_EVAL, u.val );
    /* set err cnt threshold */
    u.thr = trng_cfg->thr;
    DBH_WRITE_REGISTER( TRNG_CTRL, TRNG_ERR_CNT_TH, u.val );

    /* mask ctrl interrupts */
    u.intr = int_mask;
    DBH_WRITE_REGISTER( TRNG_CTRL, TRNG_INTR_MASK, u.val );

    /* mask pool interrupt */
    value = DBH_READ_REGISTER( TRNG_POOL, TRNG_POOL_INTR_MASK );
    DBH_REG_FLD_SET( TRNG_POOL_INTR_MASK, FILL_DONE, value, 0x1 );
    DBH_WRITE_REGISTER( TRNG_POOL, RESET_CTRL, value );
}

static int _arm_ce_trng_error_count( void )
{
    int ret = 0;
    int err = 0;

    ret = DBH_READ_REGISTER( TRNG_CTRL, TRNG_INTR );
    if ( BIT_MASK( ret, 0x0F ) ) {
        DBH_WRITE_REGISTER( TRNG_CTRL, TRNG_INTR, ret );
    }

    if ( BIT_MASK( ret, 1 << DBH_TRNG_INTR_ADP_TEST_ERR_BIT_SHIFT ) ) {
        err |= 1 << DBH_TRNG_ADP_TEST_ERR_INTR;
    }

    if ( BIT_MASK( ret, 1 << DBH_TRNG_INTR_REP_TEST_ERR_BIT_SHIFT ) ) {
        err |= 1 << DBH_TRNG_REP_TEST_ERR_INTR;
    }

    if ( BIT_MASK( ret, 1 << DBH_TRNG_INTR_CRNG_ERR_BIT_SHIFT ) ) {
        err |= 1 << DBH_TRNG_CRNG_ERR_INTR;
    }

    if ( BIT_MASK( ret, 1 << DBH_TRNG_INTR_VN_ERR_BIT_SHIFT ) ) {
        err |= 1 << DBH_TRNG_VN_ERR_INTR;
    }

    ret = DBH_READ_REGISTER( TRNG_POOL, TRNG_POOL_STAT );
    if ( BIT_MASK( ret, 0x1 ) ) {
        err |= 1 << DBH_TRNG_AUTO_CORR_ERR_INTR;
    }

    return err;
}

static inline void _arm_ce_trng_reset( void )
{
    uint32_t value = 0;

    /* Top reset TRNG module */
    value = DBH_READ_REGISTER( TOP_CTRL, RESET_CTRL );
    DBH_REG_FLD_SET( RESET_CTRL, TRNG, value, 0x1 );
    DBH_WRITE_REGISTER( TOP_CTRL, RESET_CTRL, value );

    DBH_REG_FLD_SET( RESET_CTRL, TRNG, value, 0x0 );
    DBH_WRITE_REGISTER( TOP_CTRL, RESET_CTRL, value );
}

static inline void _arm_ce_fill_pool_request( void )
{
    uint32_t value = 0;

    value = DBH_READ_REGISTER( TRNG_POOL, TRNG_POOL_CTRL );
    DBH_REG_FLD_SET( TRNG_POOL_CTRL, FILL_REQ, value, 0x1 );
    DBH_WRITE_REGISTER( TRNG_POOL, TRNG_POOL_CTRL, value );
}

static inline bool _arm_ce_is_pool_full( void )
{
    int ret = 0;

    ret = DBH_READ_REGISTER( TRNG_POOL, TRNG_POOL_INTR );

    if ( BIT_MASK( ret, 0x01 ) ) {
        DBH_WRITE_REGISTER( TRNG_POOL, TRNG_POOL_INTR, ret );
        return true;
    } else {
        return false;
    }
}

static void _arm_ce_trng_flush_pool( uint32_t desbuf[POOL_SIZE] )
{
    uint32_t tmp = 0;
    for ( size_t i = 0; i < POOL_SIZE; i++ ) {
        tmp = DBH_READ_REGISTER2( DBH_BASE_TRNG_POOL +
                                  DBH_TRNG_POOL_DATA_WORD_REG_OFFSET +
                                  i * DBH_TRNG_POOL_DATA_WORD_REG_BYTE_SIZE );
        PUT_UINT32_LE( tmp, (unsigned char *) &desbuf[i], 0 );
    }
}

#define CONFIG_DUBHE_FPGA 0

#if CONFIG_DUBHE_FPGA
#error "[FPGA]check whether target supports trng!!!"

#include <stdlib.h>

static void arm_ce_random_data_read( unsigned char *buf, size_t buf_len, bool need_error )
{
	unsigned char *pos;
	int rand_value, size;
	int copy_cnt;

	size = buf_len;
	pos = buf;
	while(size){
		rand_value = rand();

		copy_cnt = (size) > (sizeof(int)) ? (size) : (sizeof(int));
		/*copy_cnt = min(size, sizeof(int));*/
		memcpy(pos, &rand_value, copy_cnt);

		pos = &pos[copy_cnt];
		size -= copy_cnt;
	}

    return;
}

void arm_ce_trng_driver_init( void )
{
}

int arm_ce_seed_read( unsigned char *buf, size_t buf_len )
{
    if ( buf_len == 0 ) {
        return 0;
    }
    if ( buf == NULL ) {
        PAL_LOG_ERR("arm_ce_seed_read input invalid\n");
        return DBH_TRNG_PARAM_INVALID;
    }

#if defined( DUBHE_FOR_RUNTIME )
    dubhe_mutex_lock( DBH_TRNG_MUTEX );
#endif

    arm_ce_random_data_read( buf, buf_len, false );

#if defined( DUBHE_FOR_RUNTIME )
    dubhe_mutex_unlock( DBH_TRNG_MUTEX );
#endif
    return 0;
}

int arm_ce_trng_calibration_dump( uint32_t regBase,
                                  dbh_trng_config_t *cfg,
                                  void *bulk_buf,
                                  size_t bulk_size,
                                  size_t size,
                                  bool b_conf )
{
    return 0;
}
#else
static void arm_ce_random_data_read( unsigned char *buf, size_t buf_len, bool need_error )
{
    uint32_t i, block_size = 0, extra_size = 0, error_count = 0;
    uint32_t random_data[POOL_SIZE];
    size_t offset = 0;

    block_size = _ALIGN_UP( buf_len, ( POOL_SIZE * DBH_TRNG_WORD_SIZE ) )
                 / ( POOL_SIZE * DBH_TRNG_WORD_SIZE );
    extra_size = buf_len % ( POOL_SIZE * DBH_TRNG_WORD_SIZE );

    /* Read data from TRNG pool */
    for ( i = 0; i < block_size; ) {
        /* fill request */
        _arm_ce_fill_pool_request();
        /* wait pool full */
        do {
            if ( _arm_ce_is_pool_full() ) {
                /* flush pool*/
                _arm_ce_trng_flush_pool( random_data );
                offset = i * POOL_SIZE * DBH_TRNG_WORD_SIZE;
                i++;
                pal_memcpy( buf + offset,
                            random_data,
                            ( i == block_size ) ? ( extra_size ? extra_size
                                           : ( POOL_SIZE * DBH_TRNG_WORD_SIZE ) )
                            : ( POOL_SIZE * DBH_TRNG_WORD_SIZE ) );
                if ( need_error ) {
                #if defined( DUBHE_SECURE )
                    error_count = _arm_ce_trng_error_count( );
                #endif
                    on_trng_error( error_count );
                    error_count = 0;
                }
                break;
            }
            pal_udelay( WAIT_POOL_FILLED_US );
        } while ( true );
    }

    return;
}

void arm_ce_trng_driver_init( void )
{
    _arm_ce_trng_reset( );
#if defined( DUBHE_SECURE )
    arm_ce_trng_setup( &trng_config );
#endif
}

int arm_ce_seed_read( unsigned char *buf, size_t buf_len )
{
    if ( buf_len == 0 ) {
        return 0;
    }
    if ( buf == NULL ) {
        PAL_LOG_ERR("arm_ce_seed_read input invalid\n");
        return DBH_TRNG_PARAM_INVALID;
    }

#if defined( DUBHE_FOR_RUNTIME )
    dubhe_mutex_lock( DBH_TRNG_MUTEX );
#endif
    dubhe_clk_enable( DBH_MODULE_TRNG );

    arm_ce_random_data_read( buf, buf_len, false );

    dubhe_clk_disable( DBH_MODULE_TRNG );
#if defined( DUBHE_FOR_RUNTIME )
    dubhe_mutex_unlock( DBH_TRNG_MUTEX );
#endif
    return 0;
}

/* dump unpredictable data for trng calibration */
int arm_ce_trng_calibration_dump( uint32_t regBase,
                                  dbh_trng_config_t *cfg,
                                  void *bulk_buf,
                                  size_t bulk_size,
                                  size_t size,
                                  bool b_conf )
{
    unsigned char *buf = NULL;
    int ret            = 0;
    int block_size     = 0;

#if defined( DUBHE_FOR_RUNTIME )
    dubhe_mutex_lock( DBH_TRNG_MUTEX );
#endif
    dubhe_clk_enable( DBH_MODULE_TRNG );

    if ( b_conf ) {
        _arm_ce_trng_reset( );
        if ( cfg == NULL ) {
            PAL_LOG_ERR("arm_ce_trng_calibration_dump input invalid\n");
            ret = DBH_TRNG_PARAM_INVALID;
            goto __out__;
        }
#if defined( DUBHE_SECURE )
        arm_ce_trng_setup( cfg );
#endif
    }

    if ( size == 0 || bulk_size == 0 ) {
        goto __out__;
    }

    if ( bulk_buf == NULL ) {
        PAL_LOG_ERR("arm_ce_trng_calibration_dump input invalid\n");
        ret = DBH_TRNG_PARAM_INVALID;
        goto __out__;
    }

    (void) regBase;
    buf = (unsigned char *) bulk_buf;

    block_size = _ALIGN_UP( size, bulk_size ) / bulk_size;

    for ( size_t i = 0; i < block_size; i++ ) {
        arm_ce_random_data_read( buf, bulk_size, true );
        on_trng_produce_data( buf, bulk_size );
    }

__out__:
    dubhe_clk_disable( DBH_MODULE_TRNG );
#if defined( DUBHE_FOR_RUNTIME )
    dubhe_mutex_unlock( DBH_TRNG_MUTEX );
#endif
    return ret;
}

#endif
/*************************** The End Of File*****************************/
