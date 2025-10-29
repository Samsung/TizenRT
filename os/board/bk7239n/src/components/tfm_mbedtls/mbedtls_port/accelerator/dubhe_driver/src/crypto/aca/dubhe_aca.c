/**
 * @if copyright_display
 *      Copyright (C), 2018-2021, Arm Technology (China) Co., Ltd.
 *      All rights reserved
 *
 *          The content of this file or document is CONFIDENTIAL and PROPRIETARY
 *      to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 *      License Agreement between Licensee and Arm Technology (China) Co., Ltd
 *      restricting among other things, the use, reproduction, distribution
 *      and transfer.  Each of the embodiments, including this information and
 *      any derivative work shall retain this copyright notice.
 * @endif
 */

#include "dubhe_aca_hw.h"
#include "dubhe_aca.h"
#include "dubhe_aca_internal.h"
#include "dubhe_aca_hw_defs.h"
#include "dubhe_regs.h"
#include "pal_log.h"
#include "pal_string.h"
#include "pal_heap.h"

//#define ACA_REG_DEBUG
#define EXTRA_BITS_FOR_MAX_OP 192
#if defined( ACA_REG_DEBUG )
void aca_dump_word_data( char *prefix, uint32_t *data, unsigned int size )
{
    int i, j = 0;

    __PAL_LOG_PLAT( "%s(%d)", prefix, size );
    __PAL_LOG_PLAT( ":\n" );

    for ( i = (int) size - 1; i >= 0; i-- ) {
        __PAL_LOG_PLAT( "0x%08X,", (unsigned int) data[i] );
        if ( ( j + 1 ) % 4 == 0 ) {
            __PAL_LOG_PLAT( "\n" );
        }
        j++;
    }
    __PAL_LOG_PLAT( "\n" );
}

void aca_dump_byte_data( char *prefix, unsigned char *data, unsigned int size )
{
    int i, j = 0;

    __PAL_LOG_PLAT( "%s(%d)", prefix, size );
    __PAL_LOG_PLAT( ":\n" );

    for ( i = (int) size - 1; i >= 0; i-- ) {
        __PAL_LOG_PLAT( "0x%02X,", data[i] );
        if ( ( j + 1 ) % 16 == 0 ) {
            __PAL_LOG_PLAT( "\n" );
        }
        j++;
    }
    __PAL_LOG_PLAT( "\n" );
}
int test_flag;
extern void aca_print_all_allocated_reg_sram_info( int index );
#endif
extern unsigned long _g_Dubhe_RegBase;
void dubhe_aca_driver_init( void )
{
    uint32_t value = 0;
    PAL_LOG_DEBUG( "dubhe_aca_driver_init start:0x%08x\n", _g_Dubhe_RegBase );
    /* Clock enable */
    value = DBH_READ_REGISTER( TOP_CTRL, CLK_CTRL );

    PAL_LOG_DEBUG( "TOP_CTRL CLK_CTRL: (addr:0x%08x) value: 0x%08x\n",
                   ( _g_Dubhe_RegBase + DBH_REG_OFFSET( TOP_CTRL, CLK_CTRL ) ),
                   value );
    DBH_REG_FLD_SET( CLK_CTRL, ACA_EN, value, 0x1 );
    DBH_WRITE_REGISTER( TOP_CTRL, CLK_CTRL, value );
    /* Reset ACA */
    value = DBH_READ_REGISTER( TOP_CTRL, RESET_CTRL );
    PAL_LOG_DEBUG( "TOP_CTRL CLK_CTRL: (offset:0x%08x) value: 0x%08x\n",
                   DBH_REG_OFFSET( TOP_CTRL, RESET_CTRL ),
                   value );
    DBH_REG_FLD_SET( RESET_CTRL, ACA, value, 0x1 );
    DBH_WRITE_REGISTER( TOP_CTRL, RESET_CTRL, value );

    DBH_REG_FLD_SET( RESET_CTRL, ACA, value, 0x0 );
    DBH_WRITE_REGISTER( TOP_CTRL, RESET_CTRL, value );
#if defined( DUBHE_FOR_RUNTIME )
    ACA_UNMASK_DEFAULT_INTR( );
#endif
    ACA_SET_DEFAULT_OP_CFG( );
    aca_hw_resources_init( );
    PAL_LOG_DEBUG( "dubhe_aca_driver_init end\n" );
}

void dubhe_aca_driver_cleanup( void )
{
    uint32_t value = 0;
    /*Disable the clock*/
    value = DBH_READ_REGISTER( TOP_CTRL, CLK_CTRL );
    DBH_REG_FLD_SET( CLK_CTRL, ACA_EN, value, 0x0 );
    DBH_WRITE_REGISTER( TOP_CTRL, CLK_CTRL, value );
    aca_hw_resources_cleanup( );
}

void dubhe_aca_set_n_np_t0_t1_reg( unsigned char N,
                                   unsigned char NP,
                                   unsigned char T0,
                                   unsigned char T1 )
{
    ACA_SET_N_NP_T0_T1( N, NP, T0, T1 );
}

int dubhe_aca_shift_l( unsigned char len_id, unsigned char T1, size_t count )
{
    size_t i, v0, t1;

    if ( count == 0 ) {
        return 0;
    }

    v0 = count / MAX_SHIFT_DATA_BIT_SIZE;
    t1 = count & ( MAX_SHIFT_DATA_BIT_SIZE - 1 );

/*
 * shift by count / limb_size
 */
#if defined( ACA_REG_DEBUG )
    size_t full_len, sizebits;
    uint32_t r[260];
    full_len = aca_get_full_len( len_id );
    ACA_GET_REG_SIZE( sizebits, len_id );
    PAL_LOG_ERR( "SHL0 fullen:%d sizebits:%d v0:%d t1:%d\n",
                 full_len,
                 sizebits,
                 v0,
                 t1 );
    if ( test_flag == 1 ) {
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T1 );
        aca_dump_word_data( "before ACA_SHL0", r, full_len );
    }
#endif
    if ( v0 > 0 ) {
        for ( i = 0; i < v0; i++ ) {
            ACA_SHL_FILL0( len_id, T1, T1, 1, MAX_SHIFT_DATA_BIT_SIZE / 2 - 1 );
        }
    }

    if ( t1 > 0 ) {
        if ( t1 > MAX_SHIFT_DATA_BIT_SIZE / 2 ) {
            ACA_SHL_FILL0( len_id /*LenID*/,
                           T1,
                           T1,
                           1,
                           t1 - MAX_SHIFT_DATA_BIT_SIZE / 2 - 1 );
        } else {
            ACA_SHL_FILL0( len_id /*LenID*/, T1, T1, 0, t1 - 1 );
        }
    }
#if defined( ACA_REG_DEBUG )
    if ( test_flag == 1 ) {
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T1 );
        aca_dump_word_data( "after ACA_SHL0", r, full_len );
    }
#endif
    return 0;
}

int dubhe_aca_shift_r( unsigned char len_id, unsigned char T1, size_t count )
{
    size_t i, v0, t1;

    if ( count == 0 ) {
        return 0;
    }

    v0 = count / MAX_SHIFT_DATA_BIT_SIZE;
    t1 = count & ( MAX_SHIFT_DATA_BIT_SIZE - 1 );

    /*
     * shift by count / limb_size
     */
    if ( v0 > 0 ) {
        for ( i = 0; i < v0; i++ ) {
            ACA_SHR_FILL0( len_id, T1, T1, 1, MAX_SHIFT_DATA_BIT_SIZE / 2 - 1 );
        }
    }

    if ( t1 > 0 ) {
        if ( t1 > MAX_SHIFT_DATA_BIT_SIZE / 2 ) {
            ACA_SHR_FILL0( len_id /*LenID*/,
                           T1,
                           T1,
                           1,
                           t1 - MAX_SHIFT_DATA_BIT_SIZE / 2 - 1 );
        } else {
            ACA_SHR_FILL0( len_id /*LenID*/, T1, T1, 0, t1 - 1 );
        }
    }
#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    full_len = aca_get_full_len( len_id );
    pal_memset( r, 0, sizeof( r ) );
    aca_copy_data_from_reg( r, full_len, T1 );
    aca_dump_word_data( "ACA_SHR0", r, full_len );
#endif
    return 0;
}
int dubhe_aca_cmp( unsigned char len_id,
                   unsigned char T1,
                   unsigned char T2,
                   unsigned char regTmp )
{
    int ret             = -1;
    unsigned int status = 0;

    ACA_COMPARE_STATUS( len_id, T1, T2, status );

#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    if ( test_flag == 1 ) {
        full_len = aca_get_full_len( len_id );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T1 );
        aca_dump_word_data( "ACA_COMPARE_A", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T2 );
        aca_dump_word_data( "ACA_COMPARE_B", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        PAL_LOG_ERR( "ACA_COMPARE_STATUS:%d\n", status );
    }
#endif
    if ( status == 1 ) {
        ret = 0;
        goto cleanup;
    }

    ACA_SUB( len_id, regTmp, T1, T2 );
    ACA_GET_STATUS_CARRY( status );

    ret = ( ( !status ) ? 1 : -1 );

cleanup:
    return ret;
}

int dubhe_aca_cmp_im( unsigned char len_id,
                      unsigned char T1,
                      unsigned char a,
                      unsigned char regTmp )
{
    int ret             = -1;
    unsigned int status = 0;

    ACA_COMPARE_IM_STATUS( len_id, T1, a, status );

#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    if ( test_flag == 1 ) {
        full_len = aca_get_full_len( len_id );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T1 );
        aca_dump_word_data( "ACA_COMPARE_IM_A", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        PAL_LOG_ERR( "ACA_COMPARE_IM_b:%d\n", a );
        PAL_LOG_ERR( "ACA_COMPARE_STATUS:%d\n", status );
    }
#endif
    if ( status == 1 ) {
        ret = 0;
        goto cleanup;
    }

    ACA_SUB_IM( len_id, regTmp, T1, a );
    ACA_GET_STATUS_CARRY( status );

    ret = ( ( !status ) ? 1 : -1 );

cleanup:
    return ret;
}

int dubhe_aca_add( unsigned char len_id,
                   unsigned char R,
                   unsigned char T1,
                   unsigned char T2 )
{
#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    full_len = aca_get_full_len( len_id );
    if ( test_flag == 1 ) {
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T1 );
        aca_dump_word_data( "ACA_ADD_A", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T2 );
        aca_dump_word_data( "ACA_ADD_B", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
    }
#endif
    ACA_ADD( len_id, R, T1, T2 );
#if defined( ACA_REG_DEBUG )
    if ( test_flag == 1 ) {

        aca_copy_data_from_reg( r, full_len, R );
        aca_dump_word_data( "ACA_ADD_R", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
    }
#endif
    return 0;
}

/*R=T1&T2*/
int dubhe_aca_and( unsigned char len_id,
                   unsigned char R,
                   unsigned char T1,
                   unsigned char T2 )
{
#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    full_len = aca_get_full_len( len_id );
    PAL_LOG_ERR( "dubhe_aca_and reg %d %d %d len:%d\n", R, T1, T2, full_len );
    if ( test_flag == 1 ) {
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T1 );
        aca_dump_word_data( "ACA_AND_A", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T2 );
        aca_dump_word_data( "ACA_AND_B", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
    }
#endif
    ACA_AND( len_id, R, T1, T2 );
#if defined( ACA_REG_DEBUG )
    if ( test_flag == 1 ) {
        aca_copy_data_from_reg( r, full_len, R );
        aca_dump_word_data( "ACA_AND_R", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
    }
#endif
    return 0;
}

unsigned int dubhe_aca_sub( unsigned char len_id,
                            unsigned char R,
                            unsigned char T1,
                            unsigned char T2 )
{
    unsigned int status = 0;
#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    full_len = aca_get_full_len( len_id );
    if ( test_flag == 1 ) {
        PAL_LOG_ERR( "len_ld:%d full_len:%d\n", len_id, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T1 );
        aca_dump_word_data( "ACA_SUB_A", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T2 );
        aca_dump_word_data( "ACA_SUB_B\n", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
    }
#endif
    ACA_SUB( len_id, R, T1, T2 );
#if defined( ACA_REG_DEBUG )
    if ( test_flag == 1 ) {

        aca_copy_data_from_reg( r, full_len, R );
        aca_dump_word_data( "ACA_SUB_R", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
    }
#endif

    ACA_GET_STATUS_CARRY( status );
#if defined( ACA_REG_DEBUG )
    if ( test_flag == 1 ) {

        PAL_LOG_ERR(
            " dubhe_aca_sub status:%d R:%d T1:%d T2:%d\n", status, R, T1, T2 );
    }
#endif
    if ( status == 1 ) {
        ACA_INVERT_BITS( len_id, R, R );
        ACA_ADD_IM( len_id, R, R, 1 );
#if defined( ACA_REG_DEBUG )
        if ( test_flag == 1 ) {
            aca_copy_data_from_reg( r, full_len, R );
            aca_dump_word_data(
                "ACA_SUB_R(after reverse and + 1)", r, full_len );
            pal_memset( r, 0, sizeof( r ) );
        }
#endif
    }

    return status;
}

int dubhe_aca_add_im( unsigned char len_id,
                      unsigned char R,
                      unsigned char T1,
                      unsigned char a )
{
    ACA_ADD_IM( len_id, R, T1, a );
    return 0;
}
unsigned int dubhe_aca_sub_im( unsigned char len_id,
                               unsigned char R,
                               unsigned char T1,
                               unsigned char a )
{
    unsigned int status = 0;
#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    full_len = aca_get_full_len( len_id );
    if ( test_flag == 1 ) {
        PAL_LOG_ERR(
            "dubhe_aca_sub_im led_id:%d reg(%d %d %d)\n", len_id, R, T1, a );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T1 );
        aca_dump_word_data( "ACA_SUB_IM_A", r, full_len );
        PAL_LOG_ERR( "ACA_SUB_IM_b:%d\n", a );
    }
#endif
    ACA_SUB_IM( len_id, R, T1, a );
#if defined( ACA_REG_DEBUG )
    if ( test_flag == 1 ) {
        aca_copy_data_from_reg( r, full_len, R );
        aca_dump_word_data( "ACA_SUB_IM_R", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
    }
#endif

    ACA_GET_STATUS_CARRY( status );
    if ( status == 1 ) {
        ACA_INVERT_BITS( len_id, R, R );
        ACA_ADD_IM( len_id, R, R, 1 );
    }

    return status;
}
int dubhe_aca_mul( unsigned char len_id,
                   unsigned char R1,
                   unsigned char R2,
                   unsigned char T1,
                   unsigned char T2 )
{
#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    full_len = aca_get_full_len( len_id );

    if ( test_flag == 1 ) {
        PAL_LOG_ERR( "dubhe_aca_mul led_id:%d reg(%d %d %d %d)\n",
                     len_id,
                     R1,
                     R2,
                     T1,
                     T2 );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T1 );
        aca_dump_word_data( "ACA_MUL_A", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T2 );
        aca_dump_word_data( "ACA_MUL_B", r, full_len );
    }
#endif
    ACA_MUL( len_id, R1, T1, T2 );
#if defined( ACA_REG_DEBUG )
    if ( test_flag == 1 ) {

        aca_copy_data_from_reg( r, full_len, R1 );
        aca_dump_word_data( "ACA_MUL_R1", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
    }
#endif
    ACA_MUL_HIGH( len_id, R2, T1, T2 );
#if defined( ACA_REG_DEBUG )
    if ( test_flag == 1 ) {

        aca_copy_data_from_reg( r, full_len, R2 );
        aca_dump_word_data( "ACA_MUL_R2", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
    }
#endif
    return 0;
}

int dubhe_aca_div( unsigned char len_id,
                   unsigned char TQ,
                   unsigned char TR,
                   unsigned char T1,
                   unsigned char T2,
                   unsigned char regTmp1,
                   unsigned char regTmp2 )
{
    /* for DIV operation, opA will be modified when call ACA_DIV,
     * so we need to alloc temp reg TA. and opB == Res is also
     * not allowed by ACA_DIV, so if opB == Res, we alloc tmp reg
     * TB.
     */

    ACA_CLEAR( len_id, regTmp1 );
    ACA_CLEAR( len_id, regTmp2 );
    ACA_COPY( len_id, regTmp1, T1 );
#if defined( ACA_REG_DEBUG )
    size_t full_len, sizebits;
    uint32_t r[130];
    full_len = aca_get_full_len( len_id );
    if ( test_flag == 1 ) {
        ACA_GET_REG_SIZE( sizebits, len_id );
        PAL_LOG_ERR( "sizebits:%d\n", sizebits );
        PAL_LOG_ERR( "dubhe_aca_div reg %d %d %d %d %d %d len:%d\n",
                     TQ,
                     TR,
                     T1,
                     T2,
                     regTmp1,
                     regTmp2,
                     full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, regTmp1 );
        aca_dump_word_data( "ACA_MOD_DIV_A", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T2 );
        aca_dump_word_data( "ACA_MOD_DIV_B", r, full_len );
    }
#endif
    if ( TQ == 0xFF ) {
        ACA_DIV( len_id, regTmp2, regTmp1, T2, 0 );
    } else {
        if ( TQ == T2 ) {
            ACA_COPY( len_id, regTmp2, T2 );
            ACA_DIV( len_id, TQ, regTmp1, regTmp2, 0 );
        } else {
            ACA_DIV( len_id, TQ, regTmp1, T2, 0 );
        }
    }
    if ( TR != 0xFF ) {
        ACA_COPY( len_id, TR, regTmp1 );
    }
#if defined( ACA_REG_DEBUG )
    if ( test_flag == 1 ) {
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, regTmp1 );
        aca_dump_word_data( "regTmp1", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T2 );
        aca_dump_word_data( "T2", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, TR );
        aca_dump_word_data( "ACA_MOD_DIV_R", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, regTmp2 );
        aca_dump_word_data( "ACA_MOD_DIV_Q(tmp2)", r, full_len );
    }
#endif

    return 0;
}

unsigned int dubhe_aca_mod_reduce( unsigned char len_id,
                                   unsigned char TR,
                                   unsigned char T1,
                                   unsigned char N )
{
    unsigned int status = 0;

#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    full_len = aca_get_full_len( len_id );
    PAL_LOG_ERR( "dubhe_aca_mod_reduce reg %d %d len:%d\n", TR, T1, full_len );
    pal_memset( r, 0, sizeof( r ) );
    aca_copy_data_from_reg( r, full_len, T1 );
    aca_dump_word_data( "ACA_MOD_RED_A", r, full_len );
    pal_memset( r, 0, sizeof( r ) );
    aca_copy_data_from_reg( r, full_len, N );
    aca_dump_word_data( "ACA_MOD_RED_N", r, full_len );

#endif

    if ( ( TR == T1 ) || ( TR == N ) ) {
        ACA_REDUCE( len_id, TR, T1, 1, 0 );
    } else {
        ACA_REDUCE( len_id, TR, T1, 0, 0 );
    }

#if defined( ACA_REG_DEBUG )
    pal_memset( r, 0, sizeof( r ) );
    aca_copy_data_from_reg( r, full_len, TR );
    aca_dump_word_data( "ACA_MOD_RED_R", r, full_len );
#endif

    ACA_COMPARE_IM_STATUS( len_id, TR, 0, status );

    return status;
}

/***********     dubhe_aca_calc_np  function      **********************/
/*
 * The function uses physical data pointers to calculate and output
 * the Barrett tag Np.
 *
 *  For N bitsize > 2*A+2*X it uses truncated sizes:
 *      Np = truncated(2^(3*A+3*X-1) / ceiling(n/(2^(N-2*A-2*X)));
 *  For  N bitsize <= 2*A+2*X:
 *      Np = truncated(2^(N+A+X-1) / n);
 *
 */

void dubhe_aca_calc_np( unsigned char len_id,
                        unsigned char tmp_len_id,
                        unsigned char NP,
                        unsigned char N,
                        unsigned char regTmp1,
                        unsigned char regTmp2,
                        unsigned int sizeNbits )
{
    unsigned int A = DBH_ACA_WORD_SIZE_IN_BITS;
    unsigned int X = ACA_EXTRA_BITS;
    int32_t wT, bNom, wNom; /*Sizes in words and bits  */
    uint32_t val;
    int32_t sh, st, i;

    ACA_CLEAR( tmp_len_id, regTmp1 );
    ACA_CLEAR( tmp_len_id, regTmp2 );

    ACA_COPY( len_id, regTmp2, N );

    if ( sizeNbits <= ( 2 * A + 2 * X ) ) {
        wNom = CALC_FULL_32BIT_WORDS( sizeNbits + A + X - 1 );
        /* Sizes of nominator (N+A+X-1) in 32-bit words */
        bNom = ( sizeNbits + A + X - 1 ) % DBH_BITS_IN_32BIT_WORD; /*remain
                                                                    * bits*/
        if ( bNom ) {
            val = 1UL << bNom;
        } else {
            wNom++;
            val = 1UL;
        }

        /* Set T1 = 2^(N+A+X-1) */
        ACA_WRITE_WORD_TO_REG( val, wNom - 1, regTmp1 );
        ACA_DIV( tmp_len_id /*LenID*/, NP, regTmp1, regTmp2, 0 );
    } else {
        wNom = CALC_FULL_32BIT_WORDS( 3 * A + 3 * X - 1 ); /*words count in
                                                            * nominator */
        /* Calc. sizes of Nominator */
        bNom = ( 3 * A + 3 * X - 1 ) % DBH_BITS_IN_32BIT_WORD; /*remain bits
                                                                * count*/
        if ( bNom ) {
            val = 1UL << bNom;
        } else {
            wNom++;
            val = 1UL;
        }

        /* Set T1 = 2^D, where D=(3*A+3*X-1) */
        ACA_WRITE_WORD_TO_REG( val, wNom - 1, regTmp1 );

        /* Set rN = high part of the modulus as divisor */
        /* count low bits to truncate the modulus */
        st = sizeNbits - 2 * A - 2 * X;
        /* count of words to truncate */
        wT = st / MAX_SHIFT_DATA_BIT_SIZE;
        /* shift for truncation */
        sh = st % MAX_SHIFT_DATA_BIT_SIZE;

        /* prevent further ceiling increment, if it not needed */
        ACA_SUB_IM( tmp_len_id /*LenID*/, regTmp2, regTmp2, 1 /*OpBIm*/ );

        /* truncate modulus by words and then by bits */
        for ( i = 0; i < wT; i++ ) {
            ACA_SHR_FILL0( tmp_len_id /*LenID*/,
                           regTmp2,
                           regTmp2,
                           1,
                           MAX_SHIFT_DATA_BIT_SIZE / 2 - 1 );
        }

        if ( sh > 0 ) {
            if ( sh > MAX_SHIFT_DATA_BIT_SIZE / 2 ) {
                ACA_SHR_FILL0( tmp_len_id /*LenID*/,
                               regTmp2,
                               regTmp2,
                               1,
                               sh - MAX_SHIFT_DATA_BIT_SIZE / 2 - 1 );
            } else {
                ACA_SHR_FILL0(
                    tmp_len_id /*LenID*/, regTmp2, regTmp2, 0, sh - 1 );
            }
        }

        /* Ceiling */
        ACA_ADD_IM( tmp_len_id /*LenID*/, regTmp2, regTmp2, 1 /*OpBIm*/ );
        ACA_DIV( tmp_len_id /*LenID*/, NP, regTmp1, regTmp2, 0 );
    }

#if defined( ACA_REG_DEBUG )
        size_t full_len;
        uint32_t r[130];
        full_len = aca_get_full_len( tmp_len_id );
        if ( test_flag == 1 ) {
        PAL_LOG_ERR( "dubhe_aca_calc_np led_id:%d led_id1:%d reg(%d %d %d %d)\n",
                     len_id,
                     tmp_len_id,
                     N,
                     NP,
                     regTmp1,
                     regTmp2 );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, N );
        aca_dump_word_data( "N", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, NP );
        aca_dump_word_data( "NP", r, full_len );
            }
#endif

    ACA_CLEAR( tmp_len_id, regTmp1 );
    ACA_CLEAR( tmp_len_id, regTmp2 );
}

int dubhe_aca_mod_exp( unsigned char len_id,
                       unsigned char TR,
                       unsigned char T1,
                       unsigned char TE )
{
#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    unsigned char N, NP, hwT0, hwT1;
    if ( test_flag == 1 ) {
    ACA_GET_N_NP_T0_T1( N, NP, hwT0, hwT1 );
    PAL_LOG_ERR( "N_NP_T0_T1(%d %d %d %d)\n", N, NP, hwT0, hwT1 );
    full_len = aca_get_full_len( len_id );
    PAL_LOG_ERR( "len_ld:%d full_len:%d\n", len_id, full_len );
    pal_memset( r, 0, sizeof( r ) );
    aca_copy_data_from_reg( r, full_len, T1 );
    aca_dump_word_data( "ACA_MOD_EXP_A", r, full_len );
    pal_memset( r, 0, sizeof( r ) );
    aca_copy_data_from_reg( r, full_len, TE );
    aca_dump_word_data( "ACA_MOD_EXP_E", r, full_len );
    pal_memset( r, 0, sizeof( r ) );
    aca_copy_data_from_reg( r, full_len, N );
    aca_dump_word_data( "ACA_MOD_EXP_N", r, full_len );
    pal_memset( r, 0, sizeof( r ) );
    aca_copy_data_from_reg( r, 4, NP );
    aca_dump_word_data( "ACA_MOD_EXP_NP", r, 4 );
    pal_memset( r, 0, sizeof( r ) );
    ACA_MOD_EXP( len_id, TR, T1, TE, 0 );
    aca_copy_data_from_reg( r, full_len, TR );
    aca_dump_word_data( "ACA_MOD_EXP_R", r, full_len );
    pal_memset( r, 0, sizeof( r ) );
        }
#else
    ACA_MOD_EXP( len_id, TR, T1, TE, 0 );
#endif

    // ACA_MOD_EXP(len_id, TR, T1, TE, 0);
    return 0;
}

int dubhe_aca_mod_inv( unsigned char len_id,
                       unsigned char TR,
                       unsigned char T1,
                       unsigned char regTmp )
{
    int ret;
    unsigned int status = 0;
    ACA_COPY( len_id, regTmp, T1 );
/* dubhe hw for mod inv, the opB will change
 * to gcd value, so we need to use temp regTmp
 */
#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    unsigned char N, NP, hwT0, hwT1;
    ACA_GET_N_NP_T0_T1( N, NP, hwT0, hwT1 );
    PAL_LOG_ERR( "N_NP_T0_T1(%d %d %d %d)\n", N, NP, hwT0, hwT1 );
    full_len = aca_get_full_len( len_id );
    if ( test_flag == 1 ) {
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, regTmp );
        aca_dump_word_data( "ACA_MOD_INV_A", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, N );
        aca_dump_word_data( "ACA_MOD_INV_N", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, 4, NP );
        aca_dump_word_data( "ACA_MOD_INV_NP", r, 4 );
        pal_memset( r, 0, sizeof( r ) );
    }
    ACA_MOD_INV( len_id, TR, regTmp, 0 );
    if ( test_flag == 1 ) {
        aca_copy_data_from_reg( r, full_len, TR );
        aca_dump_word_data( "ACA_MOD_INV_R", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, regTmp );
        aca_dump_word_data( "ACA_MOD_INV_GCD", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
    }
#else
    ACA_MOD_INV( len_id, TR, regTmp, 0 );
#endif
    /*regTmp is the gcd of T1 and N, we need to check
     *the gcd value, if gcd == 1, then the TR is valid
     *modINV result, if gcd != 1, the modINV result is
     *invalid.
     */
    ACA_COMPARE_IM_STATUS( len_id, regTmp, 1, status );
    ret = ( ( status == 1 ) ? 0 : -1 );
    return ret;
}

int dubhe_aca_mod_mul( unsigned char len_id,
                       unsigned char TR,
                       unsigned char T1,
                       unsigned char T2,
                       unsigned char regTmp1,
                       unsigned char regTmp2 )
{
    if ( TR == T1 ) {
        ACA_COPY( len_id, regTmp1, T1 );
        T1 = regTmp1;
    }
    if ( TR == T2 ) {
        ACA_COPY( len_id, regTmp2, T2 );
        T2 = regTmp2;
    }

#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    unsigned char N, NP, hwT0, hwT1;
    ACA_GET_N_NP_T0_T1( N, NP, hwT0, hwT1 );
    full_len = aca_get_full_len( len_id );
    if ( test_flag == 1 ) {
        PAL_LOG_ERR( "get N_NP_T0_T1(%d %d %d %d)\n", N, NP, hwT0, hwT1 );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T1 );
        aca_dump_word_data( "ACA_MOD_MUL_A", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T2 );
        aca_dump_word_data( "ACA_MOD_MUL_B", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, N );
        aca_dump_word_data( "ACA_MOD_MUL_N", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, 4, NP );
        aca_dump_word_data( "ACA_MOD_MUL_NP", r, 4 );
        pal_memset( r, 0, sizeof( r ) );
    }
    ACA_MOD_MUL( len_id, TR, T1, T2 );

    if ( test_flag == 1 ) {
        aca_copy_data_from_reg( r, full_len, TR );
        aca_dump_word_data( "ACA_MOD_MUL_R", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
    }
#else

    /*hardware limitation, R can't equal to A or B*/
    ACA_MOD_MUL( len_id, TR, T1, T2 );
#endif
    return 0;
}

int dubhe_aca_mod_add( unsigned char len_id,
                       unsigned char R,
                       unsigned char T1,
                       unsigned char T2 )
{
#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    full_len = aca_get_full_len( len_id );
    if ( test_flag == 1 ) {
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T1 );
        aca_dump_word_data( "ACA_MOD_ADD_A", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T2 );
        aca_dump_word_data( "ACA_MOD_ADD_B", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
    }
#endif
    ACA_MOD_ADD( len_id, R, T1, T2 );
#if defined( ACA_REG_DEBUG )
    if ( test_flag == 1 ) {

        aca_copy_data_from_reg( r, full_len, R );
        aca_dump_word_data( "ACA_MOD_ADD_R", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
    }
#endif
    return 0;
}

int dubhe_aca_mod_sub( unsigned char len_id,
                       unsigned char R,
                       unsigned char T1,
                       unsigned char T2 )
{
#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    full_len = aca_get_full_len( len_id );
    if ( test_flag == 1 ) {
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T1 );
        aca_dump_word_data( "ACA_MOD_SUB_A", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T2 );
        aca_dump_word_data( "ACA_MOD_SUB_B", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
    }
#endif
    ACA_MOD_SUB( len_id, R, T1, T2 );
#if defined( ACA_REG_DEBUG )
    if ( test_flag == 1 ) {

        aca_copy_data_from_reg( r, full_len, R );
        aca_dump_word_data( "ACA_MOD_SUB_R", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
    }
#endif
    return 0;
}

int dubhe_aca_copy( unsigned char len_id, unsigned char T1, unsigned char T2 )
{
    ACA_COPY( len_id, T1, T2 );
#if defined( ACA_REG_DEBUG )
    if ( test_flag == 1 ) {
        size_t full_len;
        uint32_t r[130];
        full_len = aca_get_full_len( len_id );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T1 );
        aca_dump_word_data( "ACA_COPY_A", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T2 );
        aca_dump_word_data( "ACA_COPY_B", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
    }
#endif
    return 0;
}

void dubhe_aca_wait_on_aca_done( void )
{
    ACA_WAIT_ON_ACA_DONE( );
}

void aca_copy_data_from_reg( uint32_t *dst_ptr,  /*!< [out] Pointer to
                                                  * destination buffer. */
                             uint32_t sizeWords, /*!< [in]  Source size in
                                                  * words. */
                             uint32_t srcReg )   /*!< [in]  Virtual address
                                                  * (number) of source PKA
                                                  * register. */
{
    uint32_t currAddr;

    ACA_GET_REG_ADDRESS( srcReg, currAddr /*PhysAddr*/ );
    ACA_HW_READ_BLOCK_FROM_ACA_MEM( currAddr, dst_ptr, sizeWords );
}

void aca_copy_data_into_reg( uint32_t dstReg,         /*!< [out] Virtual
                                                       * address (number) of
                                                       * destination register.
                                                       * */
                             uint32_t lenId,          /*!< [in] ID of entry of
                                                       * regsSizesTable
                                                       * defines registers
                                                       * length with word
                                                       * extension. */
                             const uint32_t *src_ptr, /*!< [in] Pointer to
                                                       * source buffer. */
                             uint32_t sizeWords )     /*!< [in] Data size in
                                                       * words. */
{
    uint32_t currAddr;
    uint32_t regSize;

    /*  copy data from src buffer into PKA register with 0-padding
     *  in the last PKA-word
     */
    ACA_GET_REG_ADDRESS( dstReg, currAddr );
    ACA_HW_LOAD_BLOCK_TO_ACA_MEM( currAddr, src_ptr, sizeWords );
    /* data size aligned to full PKA-word */

    currAddr = currAddr + sizeWords;
    regSize  = aca_get_full_len( lenId );

    /* zeroe not significant high words of the register */
    if ( regSize > sizeWords ) {
        ACA_HW_CLEAR_ACA_MEM( currAddr, regSize - sizeWords );
    }
}

void dubhe_aca_clear_reg_sram( uint32_t dstReg,
                               uint32_t offset,
                               uint32_t sizeWords )
{
    uint32_t currAddr;

    ACA_GET_REG_ADDRESS( dstReg, currAddr );
    currAddr = currAddr + offset;
    ACA_HW_CLEAR_ACA_MEM( currAddr, sizeWords );
}

void dubhe_aca_double_point_jj( unsigned char len_id,
                                unsigned char T[],
                                const unsigned char x,
                                const unsigned char y,
                                const unsigned char z,
                                const unsigned char w,
                                const unsigned char o_mj_en )
{
    ACA_MOD_MUL( len_id, T[0], x, x );        // X^2
    ACA_MOD_ADD( len_id, T[1], T[0], T[0] );  // 2*X^2
    ACA_MOD_MUL( len_id, T[2], y, y );        // Y^2
    ACA_MOD_MUL( len_id, T[3], x, T[2] );     // X*Y^2
    ACA_MOD_ADD( len_id, T[4], T[3], T[3] );  // 2*X*Y^2
    ACA_MOD_MUL( len_id, T[5], T[2], T[2] );  // Y^4
    ACA_MOD_ADD( len_id, T[6], T[5], T[5] );  // 2*Y^4
    ACA_MOD_ADD( len_id, T[7], T[6], T[6] );  // 4*Y^4

    ACA_MOD_ADD( len_id, T[8], T[0], T[1] );   // 3*X^2
    ACA_MOD_ADD( len_id, T[9], T[4], T[4] );   // 4*X*Y^2
    ACA_MOD_ADD( len_id, T[10], T[7], T[7] );  // 8*Y^4
    ACA_MOD_MUL( len_id, T[11], y, z );        // Y*Z
    ACA_WAIT_ON_ACA_DONE( );
    ACA_MOD_ADD( len_id, T[0], w, T[8] );
    ACA_MOD_MUL( len_id, T[1], T[0], T[0] );
    ACA_MOD_ADD( len_id, T[2], T[9], T[9] );
    ACA_MOD_SUB( len_id, x, T[1], T[2] );
    ACA_MOD_SUB( len_id, T[3], T[9], x );
    ACA_MOD_MUL( len_id, T[4], T[0], T[3] );
    ACA_MOD_SUB( len_id, y, T[4], T[10] );
    ACA_MOD_ADD( len_id, z, T[11], T[11] );
    if ( o_mj_en == 1 ) {
        ACA_MOD_ADD( len_id, T[5], T[10], T[10] );
        ACA_MOD_MUL( len_id, T[6], w, T[5] );
        ACA_MOD_ADD_IM( len_id, w, T[6], 0 );
    }

#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    full_len = aca_get_full_len( len_id );
    PAL_LOG_ERR( "double Point jj----\n" );
    if ( test_flag == 1 ) {
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, x );
        aca_dump_word_data( "x", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, y );
        aca_dump_word_data( "y", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, z );
        aca_dump_word_data( "z", r, full_len );
    }
#endif
    return;
}

void dubhe_aca_double_point( unsigned char len_id,
                             unsigned char T[],
                             const unsigned char x,
                             const unsigned char y,
                             const unsigned char z,
                             const unsigned char x1,
                             const unsigned char y1,
                             const unsigned char z1,
                             const unsigned char t1 )
{
    ACA_MOD_MUL( len_id, T[0], x1, x1 );  // X^2
    ACA_MOD_ADD( len_id, T[1], T[0], T[0] );
    ACA_MOD_ADD( len_id, T[1], T[1], T[0] );          // 3*X^2
    ACA_MOD_MUL( len_id, T[2], z1, z1 );              // Z^2
    ACA_MOD_MUL( len_id, T[3], T[2], T[2] );          // Z^4
    ACA_MOD_MUL( len_id, T[4], y1, y1 );              // Y^2
    ACA_MOD_MUL( len_id, T[5], x1, T[4] );            // X*Y^2
    ACA_MOD_MUL( len_id, T[6], T[4], T[4] );          // Y^4
    ACA_MOD_MUL( len_id, T[7], y1, z1 );              // Y*Z
    ACA_MOD_MUL_ACC( len_id, T[8], t1, T[3], T[1] );  //λ1=a*Z^4 + 3*X^2
    ACA_MOD_ADD( len_id, T[9], T[5], T[5] );          // 2*X*Y^2
    ACA_MOD_ADD( len_id, T[9], T[9], T[9] );          //λ2=4*X*Y^2
    ACA_MOD_MUL_IM( len_id, T[10], T[6], 8 );         //λ3=8*Y^4
    ACA_WAIT_ON_ACA_DONE( );
    ACA_MOD_MUL( len_id, T[0], T[8], T[8] );  //λ1^2
    ACA_MOD_ADD( len_id, T[1], T[9], T[9] );  // 2*λ2
    ACA_MOD_SUB( len_id, x, T[0], T[1] );     // X3=λ1^2 - 2*λ2
    ACA_MOD_SUB( len_id, T[2], T[9], x );     //λ2 - X3
    ACA_MOD_MUL( len_id, T[3], T[8], T[2] );  ////λ1(λ2 - X3)
    ACA_MOD_SUB( len_id, y, T[3], T[10] );    // Y3=λ1(λ2 - X3) - λ3
    ACA_MOD_ADD( len_id, z, T[7], T[7] );     // Z3=2*Y*Z

#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    full_len = aca_get_full_len( len_id );
    PAL_LOG_ERR( "double Point----\n" );
    if ( test_flag == 1 ) {
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, x );
        aca_dump_word_data( "x", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, y );
        aca_dump_word_data( "y", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, z );
        aca_dump_word_data( "z", r, full_len );
    }
#endif
    return;
}
void dubhe_aca_point_add_ajj( unsigned char len_id,
                              unsigned char T[],
                              const unsigned char x,
                              const unsigned char y,
                              const unsigned char z,
                              const unsigned char x1,
                              const unsigned char y1,
                              const unsigned char a,
                              const unsigned char w,
                              const unsigned char o_mj_en )
{
    ACA_MOD_MUL( len_id, T[0], z, z );     // z1^2
    ACA_MOD_MUL( len_id, T[1], T[0], z );  // z1^3
    ACA_MOD_MUL( len_id, T[2], x1, T[0] );
    ACA_MOD_MUL( len_id, T[3], y1, T[1] );
    ACA_MOD_SUB( len_id, T[4], T[2], x );
    ACA_MOD_MUL( len_id, T[5], T[4], T[4] );

    ACA_MOD_MUL( len_id, T[7], z, T[4] );
    ACA_MOD_SUB( len_id, T[8], T[3], y );
    ACA_MOD_MUL( len_id, T[9], T[8], T[8] );
    ACA_MOD_MUL( len_id, T[10], T[4], T[5] );
    ACA_MOD_MUL( len_id, T[11], x, T[5] );
    ACA_WAIT_ON_ACA_DONE( );

    ACA_MOD_ADD( len_id, T[0], T[11], T[11] );
    ACA_MOD_ADD( len_id, T[1], T[10], T[0] );
    ACA_MOD_SUB( len_id, x, T[9], T[1] );
    ACA_MOD_SUB( len_id, T[2], T[11], x );
    ACA_MOD_MUL( len_id, T[3], T[8], T[2] );
    ACA_MOD_MUL( len_id, T[4], y, T[10] );
    ACA_MOD_SUB( len_id, y, T[3], T[4] );
    ACA_MOD_ADD_IM( len_id, z, T[7], 0 );
    if ( o_mj_en == 1 ) {
        ACA_MOD_MUL( len_id, T[5], z, z );
        ACA_MOD_MUL( len_id, T[6], T[5], T[5] );
        ACA_MOD_MUL( len_id, w, a, T[6] );
    }

#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    full_len = aca_get_full_len( len_id );
    PAL_LOG_ERR( "Point add ajj----\n" );
    if ( test_flag == 1 ) {
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, x );
        aca_dump_word_data( "x", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, y );
        aca_dump_word_data( "y", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, z );
        aca_dump_word_data( "z", r, full_len );
    }
#endif
    return;
}

void dubhe_aca_point_add( unsigned char len_id,
                          unsigned char T[],
                          const unsigned char t,
                          const unsigned char x,
                          const unsigned char y,
                          const unsigned char z,
                          const unsigned char x1,
                          const unsigned char y1,
                          const unsigned char z1,
                          const unsigned char x2,
                          const unsigned char y2,
                          const unsigned char z2 )
{
    ACA_MOD_MUL( len_id, T[0], z2, z2 );       // z2^2
    ACA_MOD_MUL( len_id, T[1], z1, z1 );       // z1^2
    ACA_MOD_MUL( len_id, T[2], T[0], z2 );     // Z2^3
    ACA_MOD_MUL( len_id, T[3], T[1], z1 );     // z1^3
    ACA_MOD_MUL( len_id, T[4], T[0], x1 );     //λ1=x1*Z2^2
    ACA_MOD_MUL( len_id, T[5], T[1], x2 );     //λ2=x2*Z1^2
    ACA_MOD_MUL( len_id, T[6], T[2], y1 );     //λ4=y1*Z2^3
    ACA_MOD_MUL( len_id, T[7], T[3], y2 );     //λ5=y2*Z1^3
    ACA_MOD_MUL( len_id, T[8], z1, z2 );       // z1*z2
    ACA_MOD_SUB( len_id, T[9], T[4], T[5] );   //λ3=λ1 - λ2
    ACA_MOD_SUB( len_id, T[10], T[6], T[7] );  //λ6=λ4 - λ5
    ACA_MOD_ADD( len_id, T[11], T[4], T[5] );  //λ7=λ1 + λ2
    ACA_MOD_ADD( len_id, T[12], T[6], T[7] );  //λ8=λ4 + λ5
    ACA_MOD_MUL( len_id, z, T[8], T[9] );      // Z3=Z1*Z2*λ3
    ACA_WAIT_ON_ACA_DONE( );
    ACA_MOD_MUL( len_id, T[0], T[10], T[10] );  //λ6^2
    ACA_MOD_MUL( len_id, T[1], T[9], T[9] );    //λ3^2
    ACA_MOD_MUL( len_id, T[2], T[11], T[1] );   //λ7*λ3^2
    ACA_MOD_SUB( len_id, x, T[0], T[2] );       // X3=λ6^2 - λ7*λ3^2
    ACA_MOD_ADD( len_id, T[3], x, x );          // 2*X3
    ACA_MOD_SUB( len_id, T[4], T[2], T[3] );    //λ9=λ7*λ3^2-2*X3
    ACA_MOD_MUL( len_id, T[5], T[4], T[10] );   //λ9*λ6
    ACA_MOD_MUL( len_id, T[6], T[1], T[9] );    //λ3^3
    ACA_MOD_MUL( len_id, T[7], T[12], T[6] );   //λ8*λ3^3
    ACA_MOD_SUB( len_id, T[8], T[5], T[7] );    //λ9*λ6 -λ8*λ3^3
    ACA_SHR_FILL0( len_id, T[9], t, 0, 0 );
    ACA_MOD_MUL_ACC( len_id, y, T[8], T[9], T[8] );  // Y3=(λ9*λ6 -λ8*λ3^3)/2

#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    full_len = aca_get_full_len( len_id );
    PAL_LOG_ERR( "Point add----\n" );
    if ( test_flag == 1 ) {
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, x );
        aca_dump_word_data( "x", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, y );
        aca_dump_word_data( "y", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, z );
        aca_dump_word_data( "z", r, full_len );
    }
#endif
    return;
}

// R:quotient T1:remainder
static int dubhe_aca_div_long_num( unsigned char len_id,
                                   unsigned char R,
                                   unsigned char T1,
                                   unsigned char T2,
                                   unsigned char TmpT1 )
{
    size_t count, sizebits;

#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[260];
    full_len = aca_get_full_len( len_id );
    PAL_LOG_ERR( "div_long_num(%d %d %d %d)----\n", R, T1, T2, TmpT1 );
    if ( test_flag == 1 ) {
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T1 );
        aca_dump_word_data( "T1", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, T2 );
        aca_dump_word_data( "T2", r, full_len );
    }
#endif

    ACA_GET_REG_SIZE( sizebits, len_id );
    count = GET_FULL_OP_SIZE_BITS( sizebits );
    if ( count <= MAX_OP_BITS_HALF ) {
        ACA_SET_REG_SIZE( 2 * sizebits, len_id );
        ACA_CLEAR( len_id, R );
        ACA_SET_REG_SIZE( sizebits, len_id );
        ACA_SET_BIT0( len_id, R, R );
        ACA_SET_REG_SIZE( sizebits + DBH_ACA_WORD_SIZE_IN_BITS, len_id );
        dubhe_aca_shift_l( len_id, R, count );

        ACA_SET_REG_SIZE( 2 * sizebits, len_id );
        ACA_MUL( len_id, TmpT1, R, T1 );
        ACA_DIV( len_id, R, TmpT1, T2, 0 );
        ACA_SET_REG_SIZE( sizebits, len_id );
        ACA_COPY( len_id, T1, TmpT1 );
    } else {
        sizebits = MAX_OP_BITS_HALF;
        ACA_SET_REG_SIZE( 2 * sizebits, len_id );
        ACA_CLEAR( len_id, R );
        ACA_SET_REG_SIZE( sizebits, len_id );
        ACA_SET_BIT0( len_id, R, R );
        dubhe_aca_shift_l( len_id, R, 2 * MAX_OP_BITS_HALF - count );
        ACA_SET_REG_SIZE( 2 * sizebits, len_id );
        ACA_MUL( len_id, TmpT1, R, T1 );
        ACA_DIV( len_id, R, TmpT1, T2, 0 );
        ACA_SET_REG_SIZE( count + EXTRA_BITS_FOR_MAX_OP, len_id );
        dubhe_aca_shift_l(
            len_id, TmpT1, count - ( 2 * MAX_OP_BITS_HALF - count ) );
        ACA_SET_REG_SIZE( 2 * sizebits, len_id );
        ACA_COPY( len_id, T1, TmpT1 );
        ACA_SET_REG_SIZE( count + EXTRA_BITS_FOR_MAX_OP, len_id );
        ACA_DIV( len_id, TmpT1, T1, T2, 0 );
        ACA_SET_REG_SIZE( count, len_id );
        dubhe_aca_shift_l(
            len_id, R, count - ( 2 * MAX_OP_BITS_HALF - count ) );
        ACA_ADD( len_id, R, R, TmpT1 );
    }
    return 0;
}

int dubhe_aca_mod_inv_even( unsigned char len_id,
                            unsigned char TR,
                            unsigned char TA,
                            unsigned char TN,
                            unsigned int sizeNbits,
                            unsigned char tmpT[] )
{

    uint32_t regSize;
    unsigned int status = 0;
    int ret = 0;
#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[260];

    full_len = aca_get_full_len( len_id );
    PAL_LOG_ERR( "mod_inv_even(%d %d %d) len_id:%d----\n", TA, TN, TR, len_id );
    if ( test_flag == 1 ) {
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, TA );
        aca_dump_word_data( "TA", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, TN );
        aca_dump_word_data( "TN", r, full_len );
    }
#endif
    if ( sizeNbits < DBH_ACA_WORD_SIZE_IN_BITS ) {
        sizeNbits = DBH_ACA_WORD_SIZE_IN_BITS;
    }
    ACA_SET_REG_SIZE( sizeNbits, len_id );
    ACA_COPY( len_id, tmpT[0], TN );
    /* if TA == 1, don't calculate  tmpT[0] mod TA, set to 1 */
    ACA_COMPARE_IM_STATUS( len_id, TA, 1, status );
    if ( 1 == status ) {
        ACA_COPY( len_id, tmpT[1], TA );
    } else {
        ret = dubhe_aca_mod_inv( len_id, tmpT[1], tmpT[0], tmpT[2]);
        if ( 0 != ret )
            return ret;
    }
    ACA_MUL( len_id, TR, TN, tmpT[1] );
    ACA_MUL_HIGH( len_id, tmpT[0], TN, tmpT[1] );
    ACA_SUB_IM( len_id, TR, TR, 1 );
    dubhe_aca_div_long_num( len_id, tmpT[1], tmpT[0], TA, tmpT[2] );
    ACA_COPY( len_id, tmpT[2], TR );
    ACA_SET_REG_SIZE( sizeNbits + EXTRA_BITS_FOR_MAX_OP, len_id );
    ACA_ADD( len_id, tmpT[2], tmpT[2], tmpT[0] );
    ACA_DIV( len_id, tmpT[0], tmpT[2], TA, 0 );
    ACA_SET_REG_SIZE( sizeNbits, len_id );
    ACA_ADD( len_id, TR, tmpT[1], tmpT[0] );
    ACA_SUB( len_id, TR, TN, TR );
    regSize = aca_get_full_len( len_id );
    dubhe_aca_clear_reg_sram( TR,
                              GET_FULL_OP_SIZE_32BIT_WORDS( sizeNbits ),
                              regSize
                                  - GET_FULL_OP_SIZE_32BIT_WORDS( sizeNbits ) );
#if defined( ACA_REG_DEBUG )

    if ( test_flag == 1 ) {
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, TA );
        aca_dump_word_data( "TA", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, TN );
        aca_dump_word_data( "TN", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, TR );
        aca_dump_word_data( "TR", r, full_len );
    }
#endif
    return 0;
}
/**
 * The function converts the jacobian EC point to affine representation:
 *   p(x,y,z) -> p(x,y)
 *
 * Part of PKA registers are implicitly defined in pka_ecc_glob_regs_def.h file
 *
 * @param scaProtect - flag defining is SCA protection needed (1) or not (0).
 * @param x - [in/out] coordinate x,
 * @param y - [in/out] coordinate y,
 * @param z - [in] coordinate z.
 */
void dubhe_aca_jac2affine( unsigned char len_id,
                           unsigned char T[],
                           const unsigned char x,
                           const unsigned char y,
                           const unsigned char z )
{
    ACA_MOD_INV( len_id, T[0], z, 0 ); /*1/Z*/
    /* ecc-to-affine */
    ACA_MOD_MUL( len_id, T[1], y, T[0] );    /*Y/Z*/
    ACA_MOD_MUL( len_id, T[2], T[0], T[0] ); /*1/Z^2*/
    ACA_MOD_MUL( len_id, T[3], x, T[2] );    /*X/Z^2   --- affine result*/
    ACA_MOD_MUL( len_id, y, T[1], T[2] );    /*Y/Z^3  --- affine result*/
    ACA_COPY( len_id, x, T[3] );
    ACA_SET_VAL( len_id, z, 1 );

#if defined( ACA_REG_DEBUG )
    size_t full_len;
    uint32_t r[130];
    full_len = aca_get_full_len( len_id );
    PAL_LOG_ERR( "jac2affine----\n" );
    if ( test_flag == 1 ) {
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, x );
        aca_dump_word_data( "x", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, y );
        aca_dump_word_data( "y", r, full_len );
        pal_memset( r, 0, sizeof( r ) );
        aca_copy_data_from_reg( r, full_len, z );
        aca_dump_word_data( "z", r, full_len );
    }
#endif
    return;
}

static const int small_prime[] = {
    3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,  43,  47,
    53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101, 103, 107, 109,
    113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191,
    193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269,
    271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353,
    359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439,
    443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523,
    541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617,
    619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709,
    719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811,
    821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907,
    911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997, -103};

int dubhe_aca_check_small_factors( unsigned char len_id,
                                          unsigned char X,
                                          unsigned char T[] )
{
    int ret = 0;
    size_t i, status;
    uint32_t prime;
    ACA_TEST_BIT0( len_id, X );
    ACA_GET_STATUS_AND_RESULT_ZERO( status );
    if ( status == 1 ) {
        ret = DBH_ACA_BIGNUM_NOT_ACCEPTABLE;
        goto exit;
    }

    for ( i = 0; small_prime[i] > 0; i++ ) {
        prime = small_prime[i];
        aca_copy_data_into_reg( T[0], len_id, &prime, 1 );
        if ( dubhe_aca_cmp( len_id, X, T[0], T[2] ) <= 0 ) {
            return 1;
        }

        dubhe_aca_div( len_id, 0xFF, T[1], X, T[0], T[2], T[3] );
#if defined( ACA_REG_DEBUG )
        {
            size_t full_len;
            uint32_t r[260];
            full_len = aca_get_full_len( len_id );
            pal_memset( r, 0, sizeof( r ) );
            aca_copy_data_from_reg( r, full_len, X );
            aca_dump_word_data( "X", r, full_len - 2 );
            pal_memset( r, 0, sizeof( r ) );
            aca_copy_data_from_reg( r, full_len, T[1] );
            aca_dump_word_data( "_R", r, full_len - 2 );
        }
#endif
        if ( dubhe_aca_cmp_im( len_id, T[1], 0, T[2] ) == 0 ) {
            ret = DBH_ACA_BIGNUM_NOT_ACCEPTABLE;
            goto exit;
        }
    }
exit:
    return ret;
}

#define ciL ( sizeof( uint32_t ) ) /* chars in uint32_t  */
#define biL ( ciL << 3 )           /* bits  in uint32_t  */

/*
 * Count leading zero bits in a given integer
 */
static size_t aca_clz( const uint32_t x )
{
    size_t j;
    uint32_t mask = (uint32_t) 1 << ( biL - 1 );

    for ( j = 0; j < biL; j++ ) {
        if ( x & mask ) {
            break;
        }

        mask >>= 1;
    }

    return j;
}

/*
 * Return the number of bits
 */
static size_t _aca_calc_bitlen( uint32_t *tmpbuf, size_t len )
{
    size_t i, j;
    for ( i = len - 1; i > 0; i-- ) {
        if ( tmpbuf[i] != 0 ) {
            break;
        }
    }

    j = biL - aca_clz( tmpbuf[i] );

    return ( i * biL ) + j;
}

int dubhe_aca_miller_rabin( unsigned char len_id,
                        unsigned char X,
                        unsigned char T[],
                        int ( *f_rng )( void *, unsigned char *, size_t ),
                        void *p_rng )
{
    int ret = 0, count;
    size_t i, j, k, n, s, l, bitval, opbits;
    size_t full_len, byte_size;
    uint32_t *tmpbuf    = NULL;
    unsigned char *bbuf = NULL;

    ACA_GET_REG_SIZE( opbits, len_id );
    full_len  = GET_FULL_OP_SIZE_32BIT_WORDS( opbits );
    byte_size = ( ( opbits + 7 ) >> 3 );
    tmpbuf    = (uint32_t *) pal_malloc( full_len * ciL );
    if ( tmpbuf == NULL ) {
        return DBH_ACA_SYS_SRAM_ALLOC_FAIL_ERR;
    }
    bbuf = (unsigned char *) pal_malloc( full_len * ciL );
    if ( tmpbuf == NULL ) {
        ret = DBH_ACA_SYS_SRAM_ALLOC_FAIL_ERR;
        goto exit;
    }
    dubhe_aca_sub_im( len_id, T[0], X, 1 );
    dubhe_aca_copy( len_id, T[1], T[0] );
    s = 1;
    while ( 1 ) {
        ACA_SHR_FILL0( len_id, T[1], T[1], 0, 0 );
        ACA_READ_BIT0( len_id, T[1], bitval );
        if ( bitval == 0 ) {
            s++;
        } else {
            break;
        }
    }

    pal_memset( tmpbuf, 0, full_len * ciL );
    aca_copy_data_from_reg( tmpbuf, full_len, X );
    i = _aca_calc_bitlen( tmpbuf, full_len );
    k = i;

    /*
     * HAC, table 4.4
     */
    n = ( ( i >= 1300 )
              ? 2
              : ( i >= 850 )
                    ? 3
                    : ( i >= 650 )
                          ? 4
                          : ( i >= 350 )
                                ? 8
                                : ( i >= 250 ) ? 12 : ( i >= 150 ) ? 18 : 27 );
    for ( i = 0; i < n; i++ ) {
        /*
         * pick a random A, 1 < A < |X| - 1
         */
        count = 0;
        do {
            pal_memset( tmpbuf, 0, full_len * ciL );
            pal_memset( bbuf, 0, full_len * ciL );
            ret = f_rng( p_rng, bbuf, byte_size );
            for ( l = ( ( byte_size + 3 ) >> 2 ) * ciL, j = 0; l > 0;
                  l--, j++ ) {
                tmpbuf[j / ciL] |= ( (uint32_t) bbuf[l - 1] )
                                   << ( ( j % ciL ) << 3 );
            }
            j = _aca_calc_bitlen( tmpbuf, full_len );
            aca_copy_data_into_reg( T[2], len_id, tmpbuf, full_len );

            if ( j > k ) {
                dubhe_aca_shift_r( len_id, T[2], j - k );
            }
            if ( count++ > 0xFF ) {
                PAL_LOG_ERR( "%s %d\n", __func__, __LINE__ );
                ret = DBH_ACA_BIGNUM_NOT_ACCEPTABLE;
                goto exit;
            }
        } while ( dubhe_aca_cmp( len_id, T[2], T[0], T[3] ) >= 0
                  || dubhe_aca_cmp_im( len_id, T[2], 1, T[3] ) <= 0 );

        ACA_MOD_EXP( len_id, T[4], T[2], T[1], 0 );
        ACA_COPY( len_id, T[2], T[4] );

        if ( dubhe_aca_cmp( len_id, T[2], T[0], T[3] ) == 0
             || dubhe_aca_cmp_im( len_id, T[2], 1, T[3] ) == 0 )
            continue;

        j = 1;
        while ( j < s && dubhe_aca_cmp( len_id, T[2], T[0], T[3] ) != 0 ) {
            ACA_MOD_MUL( len_id, T[4], T[2], T[2] );
            ACA_COPY( len_id, T[2], T[4] );
            if ( dubhe_aca_cmp_im( len_id, T[2], 1, T[3] ) == 0 )
                break;

            j++;
        }
        if ( dubhe_aca_cmp( len_id, T[2], T[0], T[3] ) != 0
             || dubhe_aca_cmp_im( len_id, T[2], 1, T[3] ) == 0 ) {
            ret = DBH_ACA_BIGNUM_NOT_ACCEPTABLE;
            goto exit;
        }
    }
exit:
    if ( tmpbuf != NULL ) {
        pal_free( tmpbuf );
    }
    if ( bbuf != NULL ) {
        pal_free( bbuf );
    }
    return ret;
}
int dubhe_aca_is_prime( unsigned char len_id,
                        unsigned char X,
                        size_t xbits,
                        unsigned char T[],
                        int ( *f_rng )( void *, unsigned char *, size_t ),
                        void *p_rng )
{
    int ret;

    ACA_SET_REG_SIZE( xbits, len_id );
    if ( dubhe_aca_cmp_im( len_id, X, 0, T[3] ) == 0
         || dubhe_aca_cmp_im( len_id, X, 1, T[3] ) == 0 ) {
        return DBH_ACA_BIGNUM_NOT_ACCEPTABLE;
    }
    if ( dubhe_aca_cmp_im( len_id, X, 2, T[3] ) == 0 ) {
        return 0;
    }
    if ( ( ret = dubhe_aca_check_small_factors( len_id, X, T ) ) != 0 ) {
        if ( ret == 1 ) {
            ret = 0;
        }
        goto exit;
    }
    ret = dubhe_aca_miller_rabin( len_id, X, T, f_rng, p_rng );
exit:
    return ret;
}

/*************************** The End Of File*****************************/
