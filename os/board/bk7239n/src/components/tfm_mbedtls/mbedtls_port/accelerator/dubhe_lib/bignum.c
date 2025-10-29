/*
 *  Multi-precision integer library
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

/*
 *  The following sources were referenced in the design of this Multi-precision
 *  Integer library:
 *
 *  [1] Handbook of Applied Cryptography - 1997
 *      Menezes, van Oorschot and Vanstone
 *
 *  [2] Multi-Precision Math
 *      Tom St Denis
 *      https://github.com/libtom/libtommath/blob/develop/tommath.pdf
 *
 *  [3] GNU Multi-Precision Arithmetic Library
 *      https://gmplib.org/manual/index.html
 *
 */
#include "common.h"
#if !defined( MBEDTLS_CONFIG_FILE )
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined( MBEDTLS_BIGNUM_C )

#include "mbedtls/bignum.h"
#include "mbedtls/bignum_wrap.h"
#include "bn_mul.h"

#include <string.h>

#if defined( MBEDTLS_PLATFORM_C )
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_printf printf
#define mbedtls_calloc calloc
#define mbedtls_free free
#endif

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_mpi_zeroize( mbedtls_mpi_uint *v, size_t n )
{
    volatile mbedtls_mpi_uint *p = v;
    while ( n-- )
    {
        *p++ = 0;
    }
}

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n )
{
    volatile unsigned char *p = v;
    while ( n-- )
    {
        *p++ = 0;
    }
}

#define ciL ( sizeof( mbedtls_mpi_uint ) ) /* chars in limb  */
#define biL ( ciL << 3 )                   /* bits  in limb  */
#define biH ( ciL << 2 )                   /* half limb size */
#define MPI_SIZE_T_MAX ( (size_t) -1 )     /* SIZE_T_MAX is not standard */

/*
 * Convert between bits/chars and number of limbs
 * Divide first in order to avoid potential overflows
 */
#define BITS_TO_LIMBS( i ) ( ( i ) / biL + ( ( i ) % biL != 0 ) )
#define CHARS_TO_LIMBS( i ) ( ( i ) / ciL + ( ( i ) % ciL != 0 ) )

#if defined( MBEDTLS_THREADING_C )
#include "mbedtls/threading.h"
#endif

#include "dubhe_aca_hw.h"
#include "dubhe_aca.h"
#include "pal_list.h"

#if defined( MBEDTLS_THREADING_C )
#define HW_OPS_MUTEX_LOCK                                                      \
    do                                                                         \
    {                                                                          \
        mbedtls_mutex_lock( &hw_ops_mutex );                                   \
    } while ( 0 )
#define HW_OPS_MUTEX_UNLOCK                                                    \
    do                                                                         \
    {                                                                          \
        mbedtls_rel_dubhe_resource();                                          \
        mbedtls_mutex_unlock( &hw_ops_mutex );                                 \
    } while ( 0 )
#else
#define HW_OPS_MUTEX_LOCK                                                      \
    do                                                                         \
    {                                                                          \
    } while ( 0 )
#define HW_OPS_MUTEX_UNLOCK                                                    \
    do                                                                         \
    {                                                                          \
    } while ( 0 )
#endif

#ifndef TE_ASSERT
#define TE_ASSERT(cond)                                                       \
    do {                                                                      \
        if (!(cond)) {                                                        \
            mbedtls_printf("ASSERT !%s +%d !%s\n", __func__, __LINE__, #cond);\
            while ( true ) ;                                                  \
        }                                                                     \
    } while ( 0 )
#endif


#define MAX3( A, B, C )                                                       \
    ( ( ( A ) > ( B ) && ( A ) > ( C ) ) ? ( A )                              \
                                         : ( ( B ) > ( C ) ? ( B ) : ( C ) ) )

typedef struct _mpi_reg_map_info_t {
    mbedtls_mpi *X;
    unsigned int reg_index;
    unsigned int len_id;
    unsigned int full_len;
    dubhe_bool_t dirty_flag;
    dubhe_bool_t op_flag;
} mpi_reg_map_info_t;

typedef struct _mpi_reg_map_node_t {
    pal_list_t node;
    mpi_reg_map_info_t map_info;
} mpi_reg_map_node_t;

static pal_list_t head_list;
static int _g_dubhe_bignum_init_flag = 0;

#if defined( MBEDTLS_THREADING_C )
//static mbedtls_threading_mutex_t mutex;
static mbedtls_threading_mutex_t hw_ops_mutex;

#endif

#define MUL_ECP_MAX_TMP_REG_NUM 16
#define INV_MOD_EVEN_MAX_TMP_REG_NUM 3
#define JAC_2_AFFINE_MAX_TMP_REG_NUM 4
#define PRIME_JUDGE_MAX_TMP_REG_NUM 6
#define INVALID_REG_INDEX 0xFF
/*for control log level*/
//#define AYSMC_DEBUG 0
//#define AYSMC_INFO 0

static int mbedtls_reg_to_mpi( mbedtls_mpi *X,
                               unsigned int full_len,
                               unsigned char reg_index );
static size_t _mbedtls_mpi_bitlen( const mbedtls_mpi *X );
static int _mbedtls_mpi_shrink( mbedtls_mpi *X, size_t nblimbs );
static int _mbedtls_mpi_add_int( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         mbedtls_mpi_sint b );
static int _mbedtls_mpi_sub_int( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         mbedtls_mpi_sint b );
static int _mbedtls_mpi_mul_int( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         mbedtls_mpi_uint b );

int mbedtls_reg_mpi_sync( mbedtls_mpi *X )
{
    int ret = 0;
    mpi_reg_map_node_t *cur;
    cur = (mpi_reg_map_node_t *) X->mhandle;
    if ( cur == NULL )
    {
        return 0;
    }

    if ( ( cur->map_info.reg_index != INVALID_REG_INDEX )
         && ( DBH_TRUE == cur->map_info.dirty_flag ) )
    {
        MBEDTLS_MPI_CHK( mbedtls_reg_to_mpi(
            X, cur->map_info.full_len, cur->map_info.reg_index ) );
        cur->map_info.dirty_flag = DBH_FALSE;
    }
cleanup:
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}

static void mbedtls_reset_op_flag( mbedtls_mpi *X )
{
    mpi_reg_map_node_t *cur;

    cur = (mpi_reg_map_node_t *) X->mhandle;
    if ( cur == NULL )
    {
        mbedtls_printf( "%s ERROR MPI not inititialized !!!\n", __func__ );
        return;
    }

    if ( ( cur->map_info.reg_index != INVALID_REG_INDEX )
         && ( DBH_TRUE == cur->map_info.op_flag ) )
    {
        cur->map_info.op_flag = DBH_FALSE;
    }
}
static void mbedtls_free_tmp_reg_and_sram( unsigned char regT )
{
    if ( INVALID_REG_INDEX != regT )
    {
        aca_reg_sram_free_from_reg( regT );
        aca_reg_index_free( regT );
    }
}

static int mbedtls_rel_dubhe_resource( )
{
    pal_list_t *entry, *next;
    mpi_reg_map_node_t *n;

    pal_list_iter_safe( &head_list, entry, next )
    {
        n = pal_list_entry( entry, mpi_reg_map_node_t, node );
        if ( n->map_info.reg_index != INVALID_REG_INDEX )
        {
            if ( n->map_info.op_flag == DBH_FALSE )
            {
                if ( n->map_info.dirty_flag == DBH_TRUE )
                {
                    mbedtls_reg_to_mpi( n->map_info.X,
                                        n->map_info.full_len,
                                        n->map_info.reg_index );
                }
                mbedtls_free_tmp_reg_and_sram( n->map_info.reg_index );
                n->map_info.reg_index  = INVALID_REG_INDEX;
                n->map_info.dirty_flag = DBH_FALSE;
            }
        }
        /*have already go over the list, then break*/
        if ( entry->next == &head_list )
        {
            break;
        }
    }
    return 0;
}

static int mbedtls_alloc_tmp_reg_and_sram( unsigned char len_id,
                                           unsigned char *regT )
{
    int ret = -1, tries = 0;
    unsigned int tmp_reg = INVALID_REG_INDEX;

    do
    {
        ret = aca_reg_index_alloc( &tmp_reg );

        if ( ret == DBH_ACA_REG_INDEX_ALLOC_FAIL_ERR )
        {
            mbedtls_rel_dubhe_resource( );
            // mbedtls_printf("%s try aca_reg_index_alloc again\n",__func__);
            tries++;
        }
    } while ( ( ret == DBH_ACA_REG_INDEX_ALLOC_FAIL_ERR ) && ( tries <= 1 ) );

    if ( ret )
    {
        goto finish;
    }
    tries = 0;
    do
    {
        ret = aca_reg_sram_alloc_from_reg( tmp_reg, len_id );

        if ( ret == DBH_ACA_REG_SRAM_ALLOC_FAIL_ERR )
        {
            mbedtls_rel_dubhe_resource( );
            // mbedtls_printf("%s try aca_reg_sram_alloc_from_reg
            // again\n",__func__);
            tries++;
        }
    } while ( ( ret == DBH_ACA_REG_SRAM_ALLOC_FAIL_ERR ) && ( tries <= 1 ) );
finish:
    *regT = tmp_reg;
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}

static int mbedtls_rel_dubhe_res_by_mpi( mbedtls_mpi *X )
{
    int ret = -1;
    mpi_reg_map_node_t *cur;

    cur = (mpi_reg_map_node_t *) X->mhandle;
    if ( cur == NULL )
    {
        mbedtls_printf( "%s ERROR MPI not inititialized !!!\n", __func__ );
        return ret;
    }
    dubhe_aca_wait_on_aca_done( );
    if ( cur->map_info.reg_index != INVALID_REG_INDEX )
    {
        mbedtls_free_tmp_reg_and_sram( cur->map_info.reg_index );
        cur->map_info.reg_index  = INVALID_REG_INDEX;
        cur->map_info.dirty_flag = 0;
        cur->map_info.op_flag    = 0;
    }
    return 0;
}

/*for dedicate  hw tmp reg T0/T1 and sw tmp reg T30 and T31*/
static int mbedtls_alloc_sram_for_dedicate_reg( unsigned char len_id,
                                                unsigned char *regT0,
                                                unsigned char *regT1,
                                                unsigned char *regNp )
{
    int ret;
    if ( NULL != regT0 )
    {
        MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, regT0 ) );
    }
    if ( NULL != regT1 )
    {
        MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, regT1 ) );
    }
    if ( NULL != regNp )
    {
        MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, regNp ) );
    }

cleanup:
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}

static int mbedtls_clear_reg_sram_by_mpi( mbedtls_mpi *X )
{
    int ret = -1;
    mpi_reg_map_node_t *cur;

    cur = (mpi_reg_map_node_t *) X->mhandle;
    if ( cur == NULL )
    {
        mbedtls_printf( "%s ERROR MPI not inititialized !!!\n", __func__ );
        return ret;
    }
    dubhe_aca_wait_on_aca_done( );
    if ( cur->map_info.reg_index != INVALID_REG_INDEX )
    {
        //    mbedtls_printf("%s index:%d
        //    full_len:%d\n",__func__,cur->map_info.reg_index,cur->map_info.full_len);
        dubhe_aca_clear_reg_sram(
            cur->map_info.reg_index, 0, cur->map_info.full_len );
    }

    if ( X->p != NULL )
    {
        mbedtls_mpi_zeroize( X->p, X->n );
        _mbedtls_mpi_shrink( X, 1 );
    }
    return 0;
}
/*for dedicate  hw tmp reg T0/T1 and sw tmp reg T30 and T31*/
static void mbedtls_free_sram_for_dedicate_reg( unsigned char *regT0,
                                                unsigned char *regT1,
                                                unsigned char *regNp )
{
    if ( NULL != regT0 )
    {
        mbedtls_free_tmp_reg_and_sram( *regT0 );
    }
    if ( NULL != regT1 )
    {
        mbedtls_free_tmp_reg_and_sram( *regT1 );
    }
    if ( NULL != regNp )
    {
        mbedtls_free_tmp_reg_and_sram( *regNp );
    }

    return;
}

/*for other tmp regs*/
static int mbedtls_mpi_to_reg( unsigned char *reg,
                               mbedtls_mpi *X,
                               unsigned char len_id,
                               dubhe_bool_t op_flag,
                               dubhe_bool_t dirty_flag )
{
    int ret;
    mpi_reg_map_node_t *cur;
    unsigned int wordsize ,tmp_size;
    unsigned char tmp_reg = 0;

    wordsize = BITS_TO_LIMBS( _mbedtls_mpi_bitlen( X ) );

    cur = (mpi_reg_map_node_t *) X->mhandle;
    if ( cur == NULL )
    {
        mbedtls_printf( "%s ERROR MPI not inititialized !!!\n", __func__ );
        return -1;
    }

#ifdef AYSMC_INFO
    if ( test_flag )
        mbedtls_printf( "cur:0x%x cur->map_info.reg_index:%d \n",
                        cur,
                        cur->map_info.reg_index );
#endif
    if ( ( cur->map_info.reg_index == INVALID_REG_INDEX )
         || ( aca_get_sram_size( cur->map_info.reg_index )
              < aca_get_full_len( len_id ) ) )
    {
        ret = mbedtls_alloc_tmp_reg_and_sram( len_id, &tmp_reg );
        if ( 0 != ret )
        {
            mbedtls_printf( "mbedtls_alloc_tmp_reg_and_sram fail ret:0x%x\n", -ret );
            goto cleanup;
        }
    }

    if ( cur->map_info.reg_index == INVALID_REG_INDEX )
    {
        cur->map_info.reg_index  = tmp_reg;
        cur->map_info.dirty_flag = dirty_flag;
        cur->map_info.op_flag    = op_flag;
        cur->map_info.len_id     = len_id;
        cur->map_info.full_len   = aca_get_full_len( len_id );
        tmp_size = ((cur->map_info.full_len >= wordsize)? wordsize:(cur->map_info.full_len));
        aca_copy_data_into_reg(
            cur->map_info.reg_index, len_id, X->p, tmp_size );
        dubhe_aca_clear_reg_sram( cur->map_info.reg_index, tmp_size,
            cur->map_info.full_len - tmp_size);
        *reg = cur->map_info.reg_index;
#ifdef AYSMC_INFO
        if ( test_flag )
            mbedtls_printf(
            "mbedtls_mpi_to_reg alloc reg_index:%d word_size:%d full_len:%d\n",
            cur->map_info.reg_index,
            wordsize,
            aca_get_full_len( len_id ) );
#endif
    }
    else
    { /*should also update dirty flag*/
#ifdef AYSMC_INFO
        if ( test_flag )
            mbedtls_printf( "mbedtls_mpi_to_reg already have reg_index:%d "
                        "tmp_reg:%d sram size:%d full_len:%d\n",
                        cur->map_info.reg_index,
                        tmp_reg,
                        aca_get_sram_size( cur->map_info.reg_index ),
                        aca_get_full_len( len_id ) );
#endif
        if ( aca_get_sram_size( cur->map_info.reg_index )
             < aca_get_full_len( len_id ) )
        {
            /*can't use copy here, because cur->map_info.len_id maybe not the
             * correct one */
            // dubhe_aca_copy(cur->map_info.len_id,tmp_reg,cur->map_info.reg_index);
            aca_copy_data_into_reg( tmp_reg, len_id, X->p, wordsize );
            mbedtls_free_tmp_reg_and_sram( cur->map_info.reg_index );
            cur->map_info.reg_index  = tmp_reg;
            cur->map_info.full_len   = aca_get_full_len( len_id );
            cur->map_info.dirty_flag = dirty_flag;
        }
        else
        {
            dubhe_aca_clear_reg_sram(
                cur->map_info.reg_index, aca_get_full_len( len_id ),
                aca_get_sram_size( cur->map_info.reg_index ) -
                aca_get_full_len( len_id ));
        }

        *reg                  = cur->map_info.reg_index;
        cur->map_info.op_flag = op_flag;
        cur->map_info.len_id  = len_id;  // len_id maybe reset in previous
                                         // operation, also need to update
        if ( dirty_flag )
        {
            cur->map_info.dirty_flag = dirty_flag;
            cur->map_info.full_len   = aca_get_full_len( len_id );
        }
    }
    ret = 0;
cleanup:
    return ret;
}

static int mbedtls_reg_to_mpi( mbedtls_mpi *X,
                               unsigned int full_len,
                               unsigned char reg_index )
{
    mbedtls_mpi_uint *p;

    dubhe_aca_wait_on_aca_done( );

#ifdef AYSMC_INFO
    mbedtls_printf(
        " full_len:%d x->n:%d reg_index:%d\n", full_len, X->n, reg_index );
#endif
    /* Ensure that target MPI has exactly the necessary number of limbs */
    if ( full_len > X->n )
    {
        if ( X->p != NULL )
        {
            mbedtls_mpi_zeroize( X->p, X->n );
            mbedtls_free( X->p );
        }

        X->n = 0;
        X->p = NULL;
        if ( ( p = (mbedtls_mpi_uint *) mbedtls_calloc( full_len, ciL ) )
             == NULL )
        {
            return MBEDTLS_ERR_MPI_ALLOC_FAILED;
        }

        X->n = full_len;
        X->p = p;
    }
    mbedtls_mpi_zeroize( X->p, X->n );
    aca_copy_data_from_reg( X->p, full_len, reg_index );
    return 0;
}

static int mbedtls_get_len_id( unsigned char *len_id,
                               mbedtls_mpi *A,
                               mbedtls_mpi *B,
                               mbedtls_mpi *N,
                               unsigned char op_code )
{
    int ret = -1, tries = 0;
    unsigned int len1 = 0, len2 = 0, len3 = 0, full_len = 0, op_len_bits = 0;

    if ( A != NULL )
    {
        len1 = _mbedtls_mpi_bitlen( A );
        if ( ( op_code == ACA_OPCODE_ID_SHL0 ) && ( len1 < A->n * biL ) )
        {
            len1 = A->n * biL;
        }
    }

    if ( B != NULL )
    {
        len2 = _mbedtls_mpi_bitlen( B );
    }

    if ( N != NULL )
    {
        len3 = _mbedtls_mpi_bitlen( N );
    }
    op_len_bits = MAX3( len1, len2, len3 );
    ret = aca_get_full_len_by_opcode( op_code, &op_len_bits, &full_len );

    if ( ret != 0 )
    {
#ifdef AYSMC_DEBUG
        mbedtls_printf( "%s failed !!!\n", __func__ );
#endif
        return ret;
    }

    do
    {
        ret = aca_get_len_id( len_id, op_len_bits, full_len );
        if ( ret != 0 )
        {
            mbedtls_rel_dubhe_resource();
//            mbedtls_printf( "%s aca_reset_len_id %s!!!\n", __func__,pcTaskGetName(NULL));
            aca_reset_len_id( );
            tries++;
            // mbedtls_printf("%s try aca_get_len_id again\n",__func__);
        }
    } while ( ( ret != 0 ) && ( tries <= 1 ) );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}

static int mbedtls_calc_np( unsigned char len_id,
                            unsigned char N,
                            unsigned int sizeNbits,
                            unsigned char *NP )
{
    int ret, tries = 0;
    unsigned int A = DBH_ACA_WORD_SIZE_IN_BITS;
    unsigned int X = ACA_EXTRA_BITS;
    unsigned int tmp_bits;
    unsigned char len_id1, hwT0, hwT1, swT0, swT1, tmpNP;

    swT0  = INVALID_REG_INDEX;
    swT1  = INVALID_REG_INDEX;
    hwT0  = INVALID_REG_INDEX;
    hwT1  = INVALID_REG_INDEX;
    tmpNP = INVALID_REG_INDEX;
    if ( sizeNbits > ( 3 * A + 3 * X ) )
    {
        len_id1 = len_id;
    }
    else
    {
        if ( sizeNbits > ( 2 * A + 2 * X ) )
        {
            tmp_bits = 3 * A + 3 * X;
        }
        else
        {
            tmp_bits = sizeNbits + A + X;
        }

        do
        {
            ret = aca_get_len_id(
                &len_id1, tmp_bits, CALC_FULL_32BIT_WORDS( tmp_bits ) );

            if ( ret != 0 )
            {
                mbedtls_rel_dubhe_resource();
                //mbedtls_printf( "%s aca_reset_len_id %s!!!\n", __func__,pcTaskGetName(NULL));
                aca_reset_len_id( );
                aca_set_len_id(
                    len_id,
                    sizeNbits,
                    CALC_FULL_32BIT_WORDS( sizeNbits + 2 ) );
                // mbedtls_printf("%s try aca_get_len_id again\n",__func__);
                tries++;
            }
        } while ( ( ret != 0 ) && ( tries <= 1 ) );
        if ( ret )
        {
            return ret;
        }
    }

    MBEDTLS_MPI_CHK(
        mbedtls_alloc_sram_for_dedicate_reg( len_id1, &hwT0, &hwT1, &tmpNP ) );
    MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id1, &swT0 ) );
    MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id1, &swT1 ) );
    dubhe_aca_set_n_np_t0_t1_reg( N, tmpNP, hwT0, hwT1 );
    dubhe_aca_calc_np( len_id, len_id1, tmpNP, N, swT0, swT1, sizeNbits );
    *NP = tmpNP;

cleanup:
    mbedtls_free_sram_for_dedicate_reg( &hwT0, &hwT1, NULL );
    mbedtls_free_tmp_reg_and_sram( swT0 );
    mbedtls_free_tmp_reg_and_sram( swT1 );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}

static void mbedtls_dubhe_init( )
{
    pal_list_init_head( &head_list );
#if defined( MBEDTLS_THREADING_C )
//    mbedtls_mutex_init( &mutex );
    mbedtls_mutex_init( &hw_ops_mutex );
#endif
}

void mbedtls_dubhe_cleanup( )
{
    if(1 != _g_dubhe_bignum_init_flag){
        return;
    }
    HW_OPS_MUTEX_LOCK;
    if ( !pal_list_is_empty( &head_list ) )
    {
        pal_list_t *entry, *tmp;
        pal_list_iter_safe( &head_list, entry, tmp )
        {
            mpi_reg_map_node_t *tmp =
                pal_list_entry( entry, mpi_reg_map_node_t, node );
            if ( entry != NULL )
                pal_list_del( entry );

            if ( tmp != NULL )
                mbedtls_free( tmp );
        }
    }
    _g_dubhe_bignum_init_flag = 0;
    HW_OPS_MUTEX_UNLOCK;
#if defined( MBEDTLS_THREADING_C )
    mbedtls_mutex_free( &hw_ops_mutex );
#endif
}

/*
 * Initialize one MPI
 */
static void _mbedtls_mpi_init( mbedtls_mpi *X )
{
    mpi_reg_map_node_t *cur;

    X->s       = 1;
    X->n       = 0;
    X->p       = NULL;
    X->mhandle = NULL;
    cur = (mpi_reg_map_node_t *) mbedtls_calloc( sizeof( mpi_reg_map_node_t ),
                                                 1 );

    if ( cur == NULL )
    {
        mbedtls_printf( "%s ERROR mbedtls_calloc failed !!!\n", __func__ );
        return;
    }

    memset( cur, 0, sizeof( mpi_reg_map_node_t ) );
    cur->map_info.len_id    = 0xFF;
    cur->map_info.reg_index = INVALID_REG_INDEX;
    cur->map_info.X         = X;

    X->mhandle = (mpi_handle *) cur;
    pal_list_add( &( cur->node ), &head_list );
}

void mbedtls_mpi_free_handle( mbedtls_mpi *X )
{
    mpi_reg_map_node_t *cur;
    pal_list_t *entry, *next;
    mpi_reg_map_node_t *n;
    if ( X == NULL )
    {
        return;
    }
    HW_OPS_MUTEX_LOCK;
    cur = (mpi_reg_map_node_t *) X->mhandle;
    if ( cur != NULL )
    {
        mbedtls_rel_dubhe_res_by_mpi( X );
        pal_list_iter_safe( &head_list, entry, next )
        {
            n = pal_list_entry( entry, mpi_reg_map_node_t, node );

            if ( ( cur == n ) && ( X == n->map_info.X ) )
            {
                pal_list_del( entry );
                memset( cur, 0, sizeof( mpi_reg_map_node_t ) );
                mbedtls_free( cur );
                cur = NULL;
                break;
            }
        }
    }
    else
    {
        mbedtls_printf( "%s ERROR MPI not inititialized !!!\n", __func__ );
    }

    X->mhandle = NULL;
    HW_OPS_MUTEX_UNLOCK;
}

static void _mbedtls_mpi_free( mbedtls_mpi *X )
{
    mpi_reg_map_node_t *cur;
    pal_list_t *entry, *next;
    mpi_reg_map_node_t *n;
    if ( X == NULL )
    {
        return;
    }

    cur = (mpi_reg_map_node_t *) X->mhandle;
    if ( cur != NULL )
    {
        mbedtls_rel_dubhe_res_by_mpi( X );
        pal_list_iter_safe( &head_list, entry, next )
        {
            n = pal_list_entry( entry, mpi_reg_map_node_t, node );

            if ( ( cur == n ) && ( X == n->map_info.X ) )
            {
                pal_list_del( entry );
                memset( cur, 0, sizeof( mpi_reg_map_node_t ) );

                mbedtls_free( cur );
                cur = NULL;
                break;
            }
        }
    }

    if ( X->p != NULL )
    {
        mbedtls_mpi_zeroize( X->p, X->n );
        mbedtls_free( X->p );
    }

    X->s       = 1;
    X->n       = 0;
    X->p       = NULL;
    X->mhandle = NULL;
}
static int _mbedtls_mpi_grow( mbedtls_mpi *X, size_t nblimbs )
{
    mbedtls_mpi_uint *p;
    if ( nblimbs > MBEDTLS_MPI_MAX_LIMBS )
    {
        return MBEDTLS_ERR_MPI_ALLOC_FAILED;
    }
    mbedtls_reg_mpi_sync( X );

    if ( X->n < nblimbs )
    {
        if ( ( p = (mbedtls_mpi_uint *) mbedtls_calloc( nblimbs, ciL ) )
             == NULL )
        {
            return MBEDTLS_ERR_MPI_ALLOC_FAILED;
        }

        if ( X->p != NULL )
        {
            memcpy( p, X->p, X->n * ciL );
            mbedtls_mpi_zeroize( X->p, X->n );
            mbedtls_free( X->p );
        }

        mbedtls_rel_dubhe_res_by_mpi( X );

        X->n = nblimbs;
        X->p = p;
    }
    return 0;
}
static int _mbedtls_mpi_shrink( mbedtls_mpi *X, size_t nblimbs )
{
    mbedtls_mpi_uint *p;
    size_t i;

    mbedtls_reg_mpi_sync( X );

    /* Actually resize up in this case */
    if ( X->n <= nblimbs )
    {
        return _mbedtls_mpi_grow( X, nblimbs );
    }

    for ( i = X->n - 1; i > 0; i-- )
    {
        if ( X->p[i] != 0 )
        {
            break;
        }
    }
    i++;

    if ( i < nblimbs )
    {
        i = nblimbs;
    }

    if ( ( p = (mbedtls_mpi_uint *) mbedtls_calloc( i, ciL ) ) == NULL )
    {
        return MBEDTLS_ERR_MPI_ALLOC_FAILED;
    }

    if ( X->p != NULL )
    {
        memcpy( p, X->p, i * ciL );
        mbedtls_mpi_zeroize( X->p, X->n );
        mbedtls_free( X->p );
    }

    X->n = i;
    X->p = p;

    return 0;
}
static int _mbedtls_mpi_copy( mbedtls_mpi *X, const mbedtls_mpi *Y )
{
    int ret = 0;
    unsigned char T1, T2, len_id;

    if ( X == Y )
    {
        return 0;
    }

    MBEDTLS_MPI_CHK( mbedtls_get_len_id(
        &len_id, NULL, (mbedtls_mpi *) Y, NULL, ACA_OPCODE_ID_OR ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &T1, (mbedtls_mpi *) X, len_id, DBH_TRUE, DBH_TRUE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &T2, (mbedtls_mpi *) Y, len_id, DBH_TRUE, DBH_FALSE ) );
    dubhe_aca_copy( len_id, T1, T2 );
    X->s = Y->s;
cleanup:
    mbedtls_reset_op_flag( (mbedtls_mpi *) X );
    mbedtls_reset_op_flag( (mbedtls_mpi *) Y );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}

/*
 * Swap the contents of X and Y
 */
void mbedtls_mpi_swap( mbedtls_mpi *X, mbedtls_mpi *Y )
{
    mbedtls_mpi T;
    mpi_reg_map_node_t *cur1;
    mpi_reg_map_node_t *cur2;

    cur1 = (mpi_reg_map_node_t *) X->mhandle;
    cur2 = (mpi_reg_map_node_t *) Y->mhandle;
    if ( cur1 == NULL || cur2 == NULL )
    {
        mbedtls_printf( "%s ERROR MPI not inititialized !!!\n", __func__ );
        return;
    }

    HW_OPS_MUTEX_LOCK;
    memcpy( &T, X, sizeof( mbedtls_mpi ) );
    memcpy( X, Y, sizeof( mbedtls_mpi ) );
    memcpy( Y, &T, sizeof( mbedtls_mpi ) );
    cur1->map_info.X = Y;
    cur2->map_info.X = X;
    HW_OPS_MUTEX_UNLOCK;
}

/*
 * Conditionally assign X = Y, without leaking information
 * about whether the assignment was made or not.
 * (Leaking information about the respective sizes of X and Y is ok however.)
 */
int mbedtls_mpi_safe_cond_assign( mbedtls_mpi *X,
                                  const mbedtls_mpi *Y,
                                  unsigned char assign )
{
    int ret = 0;
    size_t i;
    unsigned char len_id;
    unsigned int wordsize;
    mpi_reg_map_node_t *cur;

    HW_OPS_MUTEX_LOCK;
    /* make sure assign is 0 or 1 in a time-constant manner */
    assign = ( assign | (unsigned char) -assign ) >> 7;

    MBEDTLS_MPI_CHK( mbedtls_get_len_id(
        &len_id, (mbedtls_mpi *) Y, NULL, NULL, ACA_OPCODE_ID_OR ) );
    MBEDTLS_MPI_CHK( _mbedtls_mpi_grow( X, Y->n ) );
    X->s = X->s * ( 1 - assign ) + Y->s * assign;

    for ( i = 0; i < Y->n; i++ )
    {
        X->p[i] = X->p[i] * ( 1 - assign ) + Y->p[i] * assign;
    }

    for ( ; i < X->n; i++ )
    {
        X->p[i] *= ( 1 - assign );
    }

    wordsize = BITS_TO_LIMBS( _mbedtls_mpi_bitlen( X ) );
    cur      = (mpi_reg_map_node_t *) X->mhandle;
    if ( cur->map_info.reg_index != INVALID_REG_INDEX )
    {
        dubhe_aca_wait_on_aca_done( );
        MBEDTLS_MPI_CHK(
            mbedtls_mpi_to_reg( (unsigned char *) &cur->map_info.reg_index,
                                (mbedtls_mpi *) X,
                                len_id,
                                DBH_FALSE,
                                DBH_FALSE ) );
        aca_copy_data_into_reg(
            cur->map_info.reg_index, len_id, X->p, wordsize );
    }
cleanup:
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Conditionally swap X and Y, without leaking information
 * about whether the swap was made or not.
 * Here it is not ok to simply swap the pointers, which whould lead to
 * different memory access patterns when X and Y are used afterwards.
 */
int mbedtls_mpi_safe_cond_swap( mbedtls_mpi *X,
                                mbedtls_mpi *Y,
                                unsigned char swap )
{
    int ret, s;
    size_t i;
    mbedtls_mpi_uint tmp;
    unsigned char len_id;
    unsigned int wordsize, wordsize1;
    mpi_reg_map_node_t *cur, *cur1;
    if ( X == Y )
    {
        return 0;
    }

    HW_OPS_MUTEX_LOCK;
    /* make sure swap is 0 or 1 in a time-constant manner */
    swap = ( swap | (unsigned char) -swap ) >> 7;
    MBEDTLS_MPI_CHK(
        mbedtls_get_len_id( &len_id, X, Y, NULL, ACA_OPCODE_ID_OR ) );
    MBEDTLS_MPI_CHK( _mbedtls_mpi_grow( X, Y->n ) );
    MBEDTLS_MPI_CHK( _mbedtls_mpi_grow( Y, X->n ) );

    s    = X->s;
    X->s = X->s * ( 1 - swap ) + Y->s * swap;
    Y->s = Y->s * ( 1 - swap ) + s * swap;

    for ( i = 0; i < X->n; i++ )
    {
        tmp     = X->p[i];
        X->p[i] = X->p[i] * ( 1 - swap ) + Y->p[i] * swap;
        Y->p[i] = Y->p[i] * ( 1 - swap ) + tmp * swap;
    }
    wordsize  = BITS_TO_LIMBS( _mbedtls_mpi_bitlen( X ) );
    cur       = (mpi_reg_map_node_t *) X->mhandle;
    wordsize1 = BITS_TO_LIMBS( _mbedtls_mpi_bitlen( Y ) );
    cur1      = (mpi_reg_map_node_t *) Y->mhandle;

    if ( cur->map_info.reg_index != INVALID_REG_INDEX )
    {
        MBEDTLS_MPI_CHK(
            mbedtls_mpi_to_reg( (unsigned char *) &cur->map_info.reg_index,
                                (mbedtls_mpi *) X,
                                len_id,
                                DBH_FALSE,
                                DBH_FALSE ) );
        dubhe_aca_wait_on_aca_done( );
        aca_copy_data_into_reg(
            cur->map_info.reg_index, len_id, X->p, wordsize );
    }
    if ( cur1->map_info.reg_index != INVALID_REG_INDEX )
    {
        MBEDTLS_MPI_CHK(
            mbedtls_mpi_to_reg( (unsigned char *) &cur1->map_info.reg_index,
                                (mbedtls_mpi *) Y,
                                len_id,
                                DBH_FALSE,
                                DBH_FALSE ) );
        dubhe_aca_wait_on_aca_done( );
        aca_copy_data_into_reg(
            cur1->map_info.reg_index, len_id, Y->p, wordsize1 );
    }

cleanup:
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

static int _mbedtls_mpi_lset( mbedtls_mpi *X, mbedtls_mpi_sint z )
{
    int ret = -1;
    mpi_reg_map_node_t *cur;
    unsigned int wordsize;
    unsigned char len_id;

    MBEDTLS_MPI_CHK( _mbedtls_mpi_grow( X, 1 ) );
    memset( X->p, 0, X->n * ciL );

    X->p[0]  = ( z < 0 ) ? -z : z;
    X->s     = ( z < 0 ) ? -1 : 1;
    wordsize = BITS_TO_LIMBS( _mbedtls_mpi_bitlen( X ) );

    MBEDTLS_MPI_CHK(
        mbedtls_get_len_id( &len_id, X, NULL, NULL, ACA_OPCODE_ID_OR ) );

    cur = (mpi_reg_map_node_t *) X->mhandle;
    if ( cur == NULL )
    {
        mbedtls_printf( "%s ERROR MPI not inititialized !!!\n", __func__ );
        ret = MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
        goto cleanup;
    }

    if ( cur->map_info.reg_index != INVALID_REG_INDEX )
    {
        dubhe_aca_wait_on_aca_done( );
        aca_copy_data_into_reg(
            cur->map_info.reg_index, len_id, X->p, wordsize );
    }

cleanup:
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}

/*
 * Get a specific bit
 */
int mbedtls_mpi_get_bit( const mbedtls_mpi *X, size_t pos )
{
    HW_OPS_MUTEX_LOCK;
    mbedtls_reg_mpi_sync( (mbedtls_mpi *) X );
    HW_OPS_MUTEX_UNLOCK;
    if ( X->n * biL <= pos )
    {
        return 0;
    }
    return ( X->p[pos / biL] >> ( pos % biL ) ) & 0x01;
}

/*
 * Set a bit to a specific value of 0 or 1
 */
int mbedtls_mpi_set_bit( mbedtls_mpi *X, size_t pos, unsigned char val )
{
    int ret    = 0;
    size_t off = pos / biL;
    size_t idx = pos % biL;
    unsigned int wordsize;
    mpi_reg_map_node_t *cur;
    unsigned char len_id;

    if ( val != 0 && val != 1 )
    {
        return MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
    }

    HW_OPS_MUTEX_LOCK;
    mbedtls_reg_mpi_sync( X );

    if ( X->n * biL <= pos )
    {
        if ( val == 0 )
        {
            HW_OPS_MUTEX_UNLOCK;
            return 0;
        }

        MBEDTLS_MPI_CHK( _mbedtls_mpi_grow( X, off + 1 ) );
    }

    X->p[off] &= ~( (mbedtls_mpi_uint) 0x01 << idx );
    X->p[off] |= (mbedtls_mpi_uint) val << idx;

    wordsize = BITS_TO_LIMBS( _mbedtls_mpi_bitlen( X ) );
    MBEDTLS_MPI_CHK(
        mbedtls_get_len_id( &len_id, X, NULL, NULL, ACA_OPCODE_ID_OR ) );
    cur = (mpi_reg_map_node_t *) X->mhandle;
    if ( cur->map_info.reg_index != INVALID_REG_INDEX )
    {
        dubhe_aca_wait_on_aca_done( );
        aca_copy_data_into_reg(
            cur->map_info.reg_index, len_id, X->p, wordsize );
    }
cleanup:
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Return the number of less significant zero-bits
 */
size_t mbedtls_mpi_lsb( const mbedtls_mpi *X )
{
    size_t i, j, count = 0;

    HW_OPS_MUTEX_LOCK;
    mbedtls_reg_mpi_sync( (mbedtls_mpi *) X );
    HW_OPS_MUTEX_UNLOCK;

    for ( i = 0; i < X->n; i++ )
    {
        for ( j = 0; j < biL; j++, count++ )
        {
            if ( ( ( X->p[i] >> j ) & 1 ) != 0 )
            {
                return count;
            }
        }
    }

    return 0;
}

/*
 * Count leading zero bits in a given integer
 */
static size_t mbedtls_clz( const mbedtls_mpi_uint x )
{
    size_t j;
    mbedtls_mpi_uint mask = (mbedtls_mpi_uint) 1 << ( biL - 1 );

    for ( j = 0; j < biL; j++ )
    {
        if ( x & mask )
        {
            break;
        }

        mask >>= 1;
    }

    return j;
}

static size_t _mbedtls_mpi_bitlen( const mbedtls_mpi *X )
{
    size_t i, j;

    mbedtls_reg_mpi_sync( (mbedtls_mpi *) X );

    if ( X->n == 0 )
    {
        return 0;
    }

    for ( i = X->n - 1; i > 0; i-- )
    {
        if ( X->p[i] != 0 )
        {
            break;
        }
    }

    j = biL - mbedtls_clz( X->p[i] );

    return ( i * biL ) + j;
}
static size_t _mbedtls_mpi_size( const mbedtls_mpi *X )
{
    return ( _mbedtls_mpi_bitlen( X ) + 7 ) >> 3;
}

/*
 * Convert an ASCII character to digit value
 */
static int mpi_get_digit( mbedtls_mpi_uint *d, int radix, char c )
{
    *d = 255;

    if ( c >= 0x30 && c <= 0x39 )
    {
        *d = c - 0x30;
    }
    if ( c >= 0x41 && c <= 0x46 )
    {
        *d = c - 0x37;
    }
    if ( c >= 0x61 && c <= 0x66 )
    {
        *d = c - 0x57;
    }

    if ( *d >= (mbedtls_mpi_uint) radix )
    {
        return MBEDTLS_ERR_MPI_INVALID_CHARACTER;
    }

    return 0;
}

/*
 * Import from an ASCII string
 */
int mbedtls_mpi_read_string( mbedtls_mpi *X, int radix, const char *s )
{
    int ret;
    size_t i, j, slen, n;
    unsigned int wordsize;
    mbedtls_mpi_uint d;
    mbedtls_mpi T;
    mpi_reg_map_node_t *cur;
    unsigned char len_id;

    /* Because dubhe driver not support bignum over 4k bit,
     * when string length > 4096 / 8 * 2, call sw wrap API.
     */
    if ( strlen( s ) > ( DBG_GR_LEN_MAX_SIZE_IN_BITS / 8 * 2 ) )
    {
        ret = mbedtls_mpi_read_string_wrap( X, radix, s );
        return ret;
    }

    if ( radix < 2 || radix > 16 )
    {
        return MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
    }
    HW_OPS_MUTEX_LOCK;
    _mbedtls_mpi_init( &T );

    slen = strlen( s );

    MBEDTLS_MPI_CHK( mbedtls_rel_dubhe_res_by_mpi( X ) );

    if ( radix == 16 )
    {
        if ( slen > MPI_SIZE_T_MAX >> 2 )
        {
            ret = MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
            goto cleanup;
        }

        n = BITS_TO_LIMBS( slen << 2 );
        MBEDTLS_MPI_CHK( _mbedtls_mpi_grow( X, n ) );
        MBEDTLS_MPI_CHK( _mbedtls_mpi_lset( X, 0 ) );
        for ( i = slen, j = 0; i > 0; i--, j++ )
        {
            if ( i == 1 && s[i - 1] == '-' )
            {
                X->s = -1;
                break;
            }

            MBEDTLS_MPI_CHK( mpi_get_digit( &d, radix, s[i - 1] ) );
            X->p[j / ( 2 * ciL )] |= d << ( ( j % ( 2 * ciL ) ) << 2 );
        }
    }
    else
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_lset( X, 0 ) );

        for ( i = 0; i < slen; i++ )
        {
            if ( i == 0 && s[i] == '-' )
            {
                X->s = -1;
                continue;
            }

            MBEDTLS_MPI_CHK( mpi_get_digit( &d, radix, s[i] ) );
            MBEDTLS_MPI_CHK( _mbedtls_mpi_mul_int( &T, X, radix ) );

            if ( X->s == 1 )
            {
                MBEDTLS_MPI_CHK( _mbedtls_mpi_add_int( X, &T, d ) );
            }
            else
            {
                MBEDTLS_MPI_CHK( _mbedtls_mpi_sub_int( X, &T, d ) );
            }
        }
    }

    wordsize = BITS_TO_LIMBS( _mbedtls_mpi_bitlen( X ) );
    MBEDTLS_MPI_CHK(
        mbedtls_get_len_id( &len_id, X, NULL, NULL, ACA_OPCODE_ID_OR ) );
    cur = (mpi_reg_map_node_t *) X->mhandle;
    if ( cur->map_info.reg_index != INVALID_REG_INDEX )
    {
        dubhe_aca_wait_on_aca_done( );
        MBEDTLS_MPI_CHK(
            mbedtls_mpi_to_reg( (unsigned char *) &cur->map_info.reg_index,
                                (mbedtls_mpi *) X,
                                len_id,
                                DBH_FALSE,
                                DBH_FALSE ) );
        aca_copy_data_into_reg(
            cur->map_info.reg_index, len_id, X->p, wordsize );
    }
cleanup:
    _mbedtls_mpi_free( &T );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Helper to write the digits high-order first
 */
static int mpi_write_hlp( mbedtls_mpi *X, int radix, char **p )
{
    int ret;
    mbedtls_mpi_uint r;

    if ( radix < 2 || radix > 16 )
    {
        return MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
    }

    MBEDTLS_MPI_CHK( mbedtls_mpi_mod_int( &r, X, radix ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_div_int( X, NULL, X, radix ) );

    if ( mbedtls_mpi_cmp_int( X, 0 ) != 0 )
    {
        MBEDTLS_MPI_CHK( mpi_write_hlp( X, radix, p ) );
    }

    if ( r < 10 )
    {
        *( *p )++ = (char) ( r + 0x30 );
    }
    else
    {
        *( *p )++ = (char) ( r + 0x37 );
    }

cleanup:
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}

/*
 * Export into an ASCII string
 */
int mbedtls_mpi_write_string(
    const mbedtls_mpi *X, int radix, char *buf, size_t buflen, size_t *olen )
{
    int ret = 0;
    size_t n;
    char *p;
    mbedtls_mpi T;

    if ( radix < 2 || radix > 16 )
    {
        return MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
    }

    n = mbedtls_mpi_bitlen( X );
    if ( radix >= 4 )
    {
        n >>= 1;
    }
    if ( radix >= 16 )
    {
        n >>= 1;
    }
    /*
     * Round up the buffer length to an even value to ensure that there is
     * enough room for hexadecimal values that can be represented in an odd
     * number of digits.
     */

    n += 3 + ( ( n + 1 ) & 1 );

    if ( buflen < n )
    {
        *olen = n;
        return MBEDTLS_ERR_MPI_BUFFER_TOO_SMALL;
    }

    p = buf;
    mbedtls_mpi_init( &T );

    if ( X->s == -1 )
    {
        *p++ = '-';
    }

    if ( radix == 16 )
    {
        int c;
        size_t i, j, k;

        for ( i = X->n, k = 0; i > 0; i-- )
        {
            for ( j = ciL; j > 0; j-- )
            {
                c = ( X->p[i - 1] >> ( ( j - 1 ) << 3 ) ) & 0xFF;

                if ( c == 0 && k == 0 && ( i + j ) != 2 )
                {
                    continue;
                }
                *( p++ ) = "0123456789ABCDEF"[c / 16];
                *( p++ ) = "0123456789ABCDEF"[c % 16];
                k        = 1;
            }
        }
    }
    else
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &T, X ) );

        if ( T.s == -1 )
        {
            T.s = 1;
        }

        MBEDTLS_MPI_CHK( mpi_write_hlp( &T, radix, &p ) );
    }

    *p++  = '\0';
    *olen = p - buf;

cleanup:

    mbedtls_mpi_free( &T );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}

#if defined( MBEDTLS_FS_IO )
/*
 * Read X from an opened file
 */
int mbedtls_mpi_read_file( mbedtls_mpi *X, int radix, FILE *fin )
{
    mbedtls_mpi_uint d;
    size_t slen;
    char *p;
    /*
     * Buffer should have space for (short) label and decimal formatted MPI,
     * newline characters and '\0'
     */
    char s[MBEDTLS_MPI_RW_BUFFER_SIZE];

    memset( s, 0, sizeof( s ) );
    if ( fgets( s, sizeof( s ) - 1, fin ) == NULL )
    {
        return MBEDTLS_ERR_MPI_FILE_IO_ERROR;
    }

    slen = strlen( s );
    if ( slen == sizeof( s ) - 2 )
    {
        return MBEDTLS_ERR_MPI_BUFFER_TOO_SMALL;
    }

    if ( slen > 0 && s[slen - 1] == '\n' )
    {
        slen--;
        s[slen] = '\0';
    }
    if ( slen > 0 && s[slen - 1] == '\r' )
    {
        slen--;
        s[slen] = '\0';
    }

    p = s + slen;
    while ( p-- > s )
    {
        if ( mpi_get_digit( &d, radix, *p ) != 0 )
        {
            break;
        }
    }

    return mbedtls_mpi_read_string( X, radix, p + 1 );
}

/*
 * Write X into an opened file (or stdout if fout == NULL)
 */
int mbedtls_mpi_write_file( const char *p,
                            const mbedtls_mpi *X,
                            int radix,
                            FILE *fout )
{
    int ret;
    size_t n, slen, plen;
    /*
     * Buffer should have space for (short) label and decimal formatted MPI,
     * newline characters and '\0'
     */
    char s[MBEDTLS_MPI_RW_BUFFER_SIZE];

    memset( s, 0, sizeof( s ) );

    MBEDTLS_MPI_CHK(
        mbedtls_mpi_write_string( X, radix, s, sizeof( s ) - 2, &n ) );

    if ( p == NULL )
    {
        p = "";
    }

    plen      = strlen( p );
    slen      = strlen( s );
    s[slen++] = '\r';
    s[slen++] = '\n';

    if ( fout != NULL )
    {
        if ( fwrite( p, 1, plen, fout ) != plen
             || fwrite( s, 1, slen, fout ) != slen )
        {
            return MBEDTLS_ERR_MPI_FILE_IO_ERROR;
        }
    }
    else
    {
        mbedtls_printf( "%s%s", p, s );
    }

cleanup:
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}
#endif /* MBEDTLS_FS_IO */

/*
 * Import X from unsigned binary data, big endian
 */
int mbedtls_mpi_read_binary( mbedtls_mpi *X,
                             const unsigned char *buf,
                             size_t buflen )
{
    int ret;
    size_t i, j;
    size_t const limbs = CHARS_TO_LIMBS( buflen );
    size_t wordsize;
    mpi_reg_map_node_t *cur;
    unsigned char len_id;

    HW_OPS_MUTEX_LOCK;
    /* Ensure that target MPI has exactly the necessary number of limbs */
    if ( X->n != limbs )
    {
        _mbedtls_mpi_free( X );
        _mbedtls_mpi_init( X );
        MBEDTLS_MPI_CHK( _mbedtls_mpi_grow( X, limbs ) );
    }

    MBEDTLS_MPI_CHK( mbedtls_rel_dubhe_res_by_mpi( X ) );

    MBEDTLS_MPI_CHK( _mbedtls_mpi_lset( X, 0 ) );
    for ( i = buflen, j = 0; i > 0; i--, j++ )
    {
        X->p[j / ciL] |= ( (mbedtls_mpi_uint) buf[i - 1] )
                         << ( ( j % ciL ) << 3 );
    }
    wordsize = BITS_TO_LIMBS( _mbedtls_mpi_bitlen( X ) );

    cur = (mpi_reg_map_node_t *) X->mhandle;
    if ( cur->map_info.reg_index != INVALID_REG_INDEX )
    {
        MBEDTLS_MPI_CHK(
            mbedtls_get_len_id( &len_id, X, NULL, NULL, ACA_OPCODE_ID_OR ) );
        dubhe_aca_wait_on_aca_done( );
        MBEDTLS_MPI_CHK(
            mbedtls_mpi_to_reg( (unsigned char *) &cur->map_info.reg_index,
                                (mbedtls_mpi *) X,
                                len_id,
                                DBH_FALSE,
                                DBH_FALSE ) );
        aca_copy_data_into_reg(
            cur->map_info.reg_index, len_id, X->p, wordsize );
    }
cleanup:
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Export X into unsigned binary data, big endian
 */
int mbedtls_mpi_write_binary( const mbedtls_mpi *X,
                              unsigned char *buf,
                              size_t buflen )
{
    size_t i, j, n;

    HW_OPS_MUTEX_LOCK;
    mbedtls_reg_mpi_sync( (mbedtls_mpi *) X );

    n = _mbedtls_mpi_size( X );
    HW_OPS_MUTEX_UNLOCK;

    if ( buflen < n )
    {
        return MBEDTLS_ERR_MPI_BUFFER_TOO_SMALL;
    }

    memset( buf, 0, buflen );

    for ( i = buflen - 1, j = 0; n > 0; i--, j++, n-- )
    {
        buf[i] = (unsigned char) ( X->p[j / ciL] >> ( ( j % ciL ) << 3 ) );
    }

    return 0;
}

/*
 * Left-shift: X <<= count
 */
int mbedtls_mpi_shift_l( mbedtls_mpi *X, size_t count )
{
    int ret;
    unsigned char T1, len_id;

    HW_OPS_MUTEX_LOCK;
    if ( X->n * biL < ( _mbedtls_mpi_bitlen( X ) + count ) )
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_grow(
            X, ( _mbedtls_mpi_bitlen( X ) + count + biL - 1 ) / biL ) );
    }

    MBEDTLS_MPI_CHK(
        mbedtls_get_len_id( &len_id, X, NULL, NULL, ACA_OPCODE_ID_SHL0 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg( &T1, X, len_id, DBH_TRUE, DBH_TRUE ) );
    MBEDTLS_MPI_CHK( dubhe_aca_shift_l( len_id, T1, count ) );

cleanup:
    mbedtls_reset_op_flag( X );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Right-shift: X >>= count
 */
int mbedtls_mpi_shift_r( mbedtls_mpi *X, size_t count )
{
    int ret;
    unsigned char T1, len_id;

    HW_OPS_MUTEX_LOCK;
    MBEDTLS_MPI_CHK(
        mbedtls_get_len_id( &len_id, X, NULL, NULL, ACA_OPCODE_ID_SHR0 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg( &T1, X, len_id, DBH_TRUE, DBH_TRUE ) );
    MBEDTLS_MPI_CHK( dubhe_aca_shift_r( len_id, T1, count ) );

cleanup:
    mbedtls_reset_op_flag( X );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Compare unsigned values
 */
int mbedtls_mpi_cmp_abs( const mbedtls_mpi *X, const mbedtls_mpi *Y )
{
    int ret = -1;
    unsigned char T1, T2, len_id, swT0;

    HW_OPS_MUTEX_LOCK;
    if ( ( ret = mbedtls_get_len_id( &len_id,
                                     (mbedtls_mpi *) X,
                                     (mbedtls_mpi *) Y,
                                     NULL,
                                     ACA_OPCODE_ID_SUB ) )
         != 0 )
    {
        HW_OPS_MUTEX_UNLOCK;
        return ret;
    }

    if ( ( ret = mbedtls_mpi_to_reg(
               &T1, (mbedtls_mpi *) X, len_id, DBH_TRUE, DBH_FALSE ) )
         != 0 )
    {
        HW_OPS_MUTEX_UNLOCK;
        mbedtls_reset_op_flag( (mbedtls_mpi *) X );
        return ret;
    }
    if ( ( ret = mbedtls_mpi_to_reg(
               &T2, (mbedtls_mpi *) Y, len_id, DBH_TRUE, DBH_FALSE ) )
         != 0 )
    {
        HW_OPS_MUTEX_UNLOCK;
        mbedtls_reset_op_flag( (mbedtls_mpi *) Y );
        return ret;
    }

    MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, &swT0 ) );
    ret = dubhe_aca_cmp( len_id, T1, T2, swT0 );
cleanup:
    mbedtls_free_tmp_reg_and_sram( swT0 );
    mbedtls_reset_op_flag( (mbedtls_mpi *) X );
    mbedtls_reset_op_flag( (mbedtls_mpi *) Y );
    HW_OPS_MUTEX_UNLOCK;

    return ret;
}

static int _mbedtls_mpi_cmp_mpi( const mbedtls_mpi *X, const mbedtls_mpi *Y )
{
    int ret    = -1;
    int status = 0;
    unsigned char T1, T2, len_id, swT0;

    if ( X->s > 0 && Y->s < 0 )
    {
        return 1;
    }
    if ( Y->s > 0 && X->s < 0 )
    {
        return -1;
    }

    if ( ( _mbedtls_mpi_bitlen( X ) == 0 ) && ( _mbedtls_mpi_bitlen( Y ) == 0 ) )
    {
        return 0;
    }

    if ( ( ret = mbedtls_get_len_id( &len_id,
                                     (mbedtls_mpi *) X,
                                     (mbedtls_mpi *) Y,
                                     NULL,
                                     ACA_OPCODE_ID_SUB ) )
         != 0 )
    {
        ret = DBH_ACA_GENERIC_ERR;
        goto error;
    }

    if ( ( ret = mbedtls_mpi_to_reg(
               &T1, (mbedtls_mpi *) X, len_id, DBH_TRUE, DBH_FALSE ) )
         != 0 )
    {
        mbedtls_reset_op_flag( (mbedtls_mpi *) X );
        ret = DBH_ACA_GENERIC_ERR;
        goto error;
    }
    if ( ( ret = mbedtls_mpi_to_reg(
               &T2, (mbedtls_mpi *) Y, len_id, DBH_TRUE, DBH_FALSE ) )
         != 0 )
    {
        mbedtls_reset_op_flag( (mbedtls_mpi *) Y );
        ret = DBH_ACA_GENERIC_ERR;
        goto error;
    }

    MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, &swT0 ) );
    status = dubhe_aca_cmp( len_id, T1, T2, swT0 );
    if ( status == 0 )
    {
        ret = 0;
        goto cleanup;
    }

    if ( X->s > 0 && Y->s > 0 )
    {
        ret = status;
    }

    if ( X->s < 0 && Y->s < 0 )
    {
        ret = -status;
    }

cleanup:
    mbedtls_free_tmp_reg_and_sram( swT0 );
    mbedtls_reset_op_flag( (mbedtls_mpi *) X );
    mbedtls_reset_op_flag( (mbedtls_mpi *) Y );
error:
    TE_ASSERT( ret >= -1 );
    return ret;
}
static int _mbedtls_mpi_cmp_int( const mbedtls_mpi *X, mbedtls_mpi_sint z )
{
    int ret;
    mbedtls_mpi Y;

    _mbedtls_mpi_init( &Y );

    Y.s = ( z < 0 ) ? -1 : 1;
    MBEDTLS_MPI_CHK( _mbedtls_mpi_grow( &Y, 1 ) );
    Y.p[0] = ( z < 0 ) ? -z : z;
    MBEDTLS_MPI_CHK( _mbedtls_mpi_cmp_mpi( X, &Y ) );
cleanup:
    _mbedtls_mpi_free( &Y );
    TE_ASSERT( ret >= -1 );
    return ret;
}

/*
 * Unsigned addition: X = |A| + |B|  (HAC 14.7)
 */
int mbedtls_mpi_add_abs( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         const mbedtls_mpi *B )
{
    int ret;
    unsigned char T1, T2, R, len_id;

    HW_OPS_MUTEX_LOCK;
    MBEDTLS_MPI_CHK( mbedtls_get_len_id( &len_id,
                                         (mbedtls_mpi *) A,
                                         (mbedtls_mpi *) B,
                                         NULL,
                                         ACA_OPCODE_ID_ADD ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &T1, (mbedtls_mpi *) A, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &T2, (mbedtls_mpi *) B, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg( &R, X, len_id, DBH_TRUE, DBH_TRUE ) );
    MBEDTLS_MPI_CHK( dubhe_aca_add( len_id, R, T1, T2 ) );
    /*
     * X should always be positive as a result of unsigned additions.
     */
    X->s = 1;
cleanup:
    mbedtls_reset_op_flag( X );
    mbedtls_reset_op_flag( (mbedtls_mpi *) A );
    mbedtls_reset_op_flag( (mbedtls_mpi *) B );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Unsigned subtraction: X = |A| - |B|  (HAC 14.9)
 */
int mbedtls_mpi_sub_abs( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         const mbedtls_mpi *B )
{
    int ret;
    unsigned int status = 0;
    unsigned char T1, T2, R, len_id;

    HW_OPS_MUTEX_LOCK;
    MBEDTLS_MPI_CHK( mbedtls_get_len_id( &len_id,
                                         (mbedtls_mpi *) A,
                                         (mbedtls_mpi *) B,
                                         NULL,
                                         ACA_OPCODE_ID_SUB ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &T1, (mbedtls_mpi *) A, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &T2, (mbedtls_mpi *) B, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg( &R, X, len_id, DBH_TRUE, DBH_TRUE ) );
    status = dubhe_aca_sub( len_id, R, T1, T2 );
    if ( status == 1 )
    {
        ret  = MBEDTLS_ERR_MPI_NEGATIVE_VALUE;
        X->s = -1;
        goto cleanup;
    }

    X->s = 1;
cleanup:
    mbedtls_reset_op_flag( X );
    mbedtls_reset_op_flag( (mbedtls_mpi *) A );
    mbedtls_reset_op_flag( (mbedtls_mpi *) B );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Signed addition: X = A & B
 */
int mbedtls_mpi_and_mpi( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         const mbedtls_mpi *B )
{
    int ret, s = A->s;
    unsigned char T1, T2, R, len_id;

    if ( A->s < 0 || B->s < 0 )
    {
        return MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
    }
    HW_OPS_MUTEX_LOCK;
    if ( X != A && X != B )
        MBEDTLS_MPI_CHK( mbedtls_clear_reg_sram_by_mpi( X ) );

    MBEDTLS_MPI_CHK( mbedtls_get_len_id( &len_id,
                                         (mbedtls_mpi *) A,
                                         (mbedtls_mpi *) B,
                                         NULL,
                                         ACA_OPCODE_ID_AND ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &T1, (mbedtls_mpi *) A, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &T2, (mbedtls_mpi *) B, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg( &R, X, len_id, DBH_TRUE, DBH_TRUE ) );
    MBEDTLS_MPI_CHK( dubhe_aca_and( len_id, R, T1, T2 ) );

    X->s = s;

cleanup:
    mbedtls_reset_op_flag( X );
    mbedtls_reset_op_flag( (mbedtls_mpi *) A );
    mbedtls_reset_op_flag( (mbedtls_mpi *) B );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

static int _mbedtls_mpi_add_mpi( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         const mbedtls_mpi *B )
{
    int ret, s = A->s;
    unsigned int status = 0;
    unsigned char T1, T2, R, len_id;

    MBEDTLS_MPI_CHK( mbedtls_get_len_id( &len_id,
                                         (mbedtls_mpi *) A,
                                         (mbedtls_mpi *) B,
                                         NULL,
                                         ACA_OPCODE_ID_ADD ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &T1, (mbedtls_mpi *) A, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &T2, (mbedtls_mpi *) B, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg( &R, X, len_id, DBH_TRUE, DBH_TRUE ) );
    if ( A->s * B->s < 0 )
    {
        status = dubhe_aca_sub( len_id, R, T1, T2 );
        if ( status == 1 )
        {
            X->s = -s;
        }
        else
        {
            X->s = s;
        }
    }
    else
    {
        MBEDTLS_MPI_CHK( dubhe_aca_add( len_id, R, T1, T2 ) );
        X->s = s;
    }

cleanup:
    mbedtls_reset_op_flag( X );
    mbedtls_reset_op_flag( (mbedtls_mpi *) A );
    mbedtls_reset_op_flag( (mbedtls_mpi *) B );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}

/*
 * Unsigned subtraction: X = |A| - |B|  (HAC 14.9)
 */
static int _mbedtls_mpi_sub_mpi( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         const mbedtls_mpi *B )
{
    int ret, s = A->s;
    unsigned int status = 0;
    unsigned char T1, T2, R, len_id;

    MBEDTLS_MPI_CHK( mbedtls_get_len_id( &len_id,
                                         (mbedtls_mpi *) A,
                                         (mbedtls_mpi *) B,
                                         NULL,
                                         ACA_OPCODE_ID_SUB ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &T1, (mbedtls_mpi *) A, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &T2, (mbedtls_mpi *) B, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg( &R, X, len_id, DBH_TRUE, DBH_TRUE ) );

    if ( A->s * B->s > 0 )
    {
        status = dubhe_aca_sub( len_id, R, T1, T2 );
        if ( status == 1 )
        {
            X->s = -s;
        }
        else
        {
            X->s = s;
        }
    }
    else
    {
        MBEDTLS_MPI_CHK( dubhe_aca_add( len_id, R, T1, T2 ) );
        X->s = s;
    }

cleanup:
    mbedtls_reset_op_flag( X );
    mbedtls_reset_op_flag( (mbedtls_mpi *) A );
    mbedtls_reset_op_flag( (mbedtls_mpi *) B );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}
static int _mbedtls_mpi_add_int( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         mbedtls_mpi_sint b )
{
    int ret, s = A->s;
    mbedtls_mpi _B;
    mbedtls_mpi_uint a;
    unsigned int status = 0;
    unsigned char T1, R, len_id;

    _mbedtls_mpi_init( &_B );

    a    = ( b < 0 ) ? -b : b;
    _B.s = ( b < 0 ) ? -1 : 1;

    if ( a <= MAX_IM_DATA_BIT_SIZE )
    {
        MBEDTLS_MPI_CHK( mbedtls_get_len_id(
            &len_id, (mbedtls_mpi *) A, &_B, NULL, ACA_OPCODE_ID_ADD ) );

        MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
            &T1, (mbedtls_mpi *) A, len_id, DBH_TRUE, DBH_FALSE ) );
        MBEDTLS_MPI_CHK(
            mbedtls_mpi_to_reg( &R, X, len_id, DBH_TRUE, DBH_TRUE ) );
        if ( A->s * _B.s < 0 )
        {
            status = dubhe_aca_sub_im( len_id, R, T1, a );
            if ( status == 1 )
            {
                X->s = -s;
            }
            else
            {
                X->s = s;
            }
        }
        else
        {
            MBEDTLS_MPI_CHK( dubhe_aca_add_im( len_id, R, T1, a ) );
            X->s = s;
        }
    }
    else
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_grow( &_B, 1 ) );
        _B.p[0] = a;
        MBEDTLS_MPI_CHK( _mbedtls_mpi_add_mpi( X, A, &_B ) );
        MBEDTLS_MPI_CHK( mbedtls_reg_mpi_sync( X ) );
    }

cleanup:
    _mbedtls_mpi_free( &_B );
    mbedtls_reset_op_flag( X );
    mbedtls_reset_op_flag( (mbedtls_mpi *) A );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif

    return ret;
}
static int _mbedtls_mpi_sub_int( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         mbedtls_mpi_sint b )
{
    int ret, s = A->s;
    mbedtls_mpi _B;
    mbedtls_mpi_uint a;
    unsigned int status = 0;
    unsigned char T1, R, len_id;

    _mbedtls_mpi_init( &_B );
    a = ( b < 0 ) ? -b : b;

    if ( X != A )
        MBEDTLS_MPI_CHK( mbedtls_clear_reg_sram_by_mpi( X ) );

    if ( a <= MAX_IM_DATA_BIT_SIZE )
    {
        MBEDTLS_MPI_CHK( mbedtls_get_len_id(
            &len_id, (mbedtls_mpi *) A, NULL, NULL, ACA_OPCODE_ID_SUB ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
            &T1, (mbedtls_mpi *) A, len_id, DBH_TRUE, DBH_FALSE ) );
        MBEDTLS_MPI_CHK(
            mbedtls_mpi_to_reg( &R, X, len_id, DBH_TRUE, DBH_TRUE ) );

        if ( A->s > 0 )
        {
            status = dubhe_aca_sub_im( len_id, R, T1, a );
            if ( status == 1 )
            {
                X->s = -s;
            }
            else
            {
                X->s = s;
            }
        }
        else
        {
            MBEDTLS_MPI_CHK( dubhe_aca_add_im( len_id, R, T1, a ) );
            X->s = s;
        }
    }
    else
    {
        _B.s = ( b < 0 ) ? -1 : 1;
        MBEDTLS_MPI_CHK( _mbedtls_mpi_grow( &_B, 1 ) );
        _B.p[0] = a;
        MBEDTLS_MPI_CHK( _mbedtls_mpi_sub_mpi( X, A, &_B ) );
        MBEDTLS_MPI_CHK( mbedtls_reg_mpi_sync( X ) );
    }
cleanup:
    _mbedtls_mpi_free( &_B );
    mbedtls_reset_op_flag( X );
    mbedtls_reset_op_flag( (mbedtls_mpi *) A );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif

    return ret;
}
static int _mbedtls_mpi_mul_mpi( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         const mbedtls_mpi *B )
{
    int ret;
    unsigned char T1, T2, len_id, swT0, swT1;
    size_t len = 0;
    mbedtls_mpi R;

    if ( ( ret = mbedtls_get_len_id( &len_id,
                                     (mbedtls_mpi *) A,
                                     (mbedtls_mpi *) B,
                                     NULL,
                                     ACA_OPCODE_ID_MUL ) )
         != 0 )
    {
        return ret;
    }

    len = aca_get_full_len( len_id );

    if ( ( ret = mbedtls_mpi_to_reg(
               &T1, (mbedtls_mpi *) A, len_id, DBH_TRUE, DBH_FALSE ) )
         != 0 )
    {
        mbedtls_reset_op_flag( (mbedtls_mpi *) A );
        return ret;
    }
    if ( ( ret = mbedtls_mpi_to_reg(
               &T2, (mbedtls_mpi *) B, len_id, DBH_TRUE, DBH_FALSE ) )
         != 0 )
    {
        mbedtls_reset_op_flag( (mbedtls_mpi *) B );
        return ret;
    }

    swT0 = INVALID_REG_INDEX;
    swT1 = INVALID_REG_INDEX;
    MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, &swT0 ) );
    MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, &swT1 ) );
    MBEDTLS_MPI_CHK( dubhe_aca_mul( len_id, swT0, swT1, T1, T2 ) );

    _mbedtls_mpi_init( &R );
    MBEDTLS_MPI_CHK( _mbedtls_mpi_grow( &R, 2 * len ) );
    mbedtls_mpi_zeroize( R.p, 2 * len );

    aca_copy_data_from_reg( &( R.p[0] ), len, swT0 );
    aca_copy_data_from_reg( &( R.p[len] ), len, swT1 );
    MBEDTLS_MPI_CHK( _mbedtls_mpi_grow( X, 2 * len ) );
    mbedtls_mpi_zeroize( X->p, X->n );
    mbedtls_rel_dubhe_res_by_mpi( X );
    memcpy(X->p, R.p, X->n*ciL);

    X->s = A->s * B->s;
cleanup:
    _mbedtls_mpi_free( &R );
    mbedtls_free_tmp_reg_and_sram( swT0 );
    mbedtls_free_tmp_reg_and_sram( swT1 );
    mbedtls_reset_op_flag( (mbedtls_mpi *) A );
    mbedtls_reset_op_flag( (mbedtls_mpi *) B );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}
static int _mbedtls_mpi_mul_int( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         mbedtls_mpi_uint b )
{
    int ret;
    mbedtls_mpi _B;

    _mbedtls_mpi_init( &_B );
    _B.s = 1;
    MBEDTLS_MPI_CHK( _mbedtls_mpi_grow( &_B, 1 ) );
    _B.p[0] = b;
    MBEDTLS_MPI_CHK( _mbedtls_mpi_mul_mpi( X, A, &_B ) );
cleanup:
    _mbedtls_mpi_free( &_B );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}

static int _mbedtls_mpi_div_mpi( mbedtls_mpi *Q,
                         mbedtls_mpi *R,
                         const mbedtls_mpi *A,
                         const mbedtls_mpi *B )
{
    int ret;
    unsigned char T1, T2, TQ, TR, len_id, hwT0, hwT1, swT0, swT1;

    if ( _mbedtls_mpi_cmp_int( B, 0 ) == 0 )
        return ( MBEDTLS_ERR_MPI_DIVISION_BY_ZERO );


    if ( ( ret = mbedtls_get_len_id( &len_id,
                                     (mbedtls_mpi *) A,
                                     (mbedtls_mpi *) B,
                                     NULL,
                                     ACA_OPCODE_ID_DIV ) )
         != 0 )
    {
        return ret;
    }

    if ( ( ret = mbedtls_mpi_to_reg(
               &T1, (mbedtls_mpi *) A, len_id, DBH_TRUE, DBH_FALSE ) )
         != 0 )
    {
        mbedtls_reset_op_flag( (mbedtls_mpi *) A );
        return ret;
    }
    if ( ( ret = mbedtls_mpi_to_reg(
               &T2, (mbedtls_mpi *) B, len_id, DBH_TRUE, DBH_FALSE ) )
         != 0 )
    {
        mbedtls_reset_op_flag( (mbedtls_mpi *) B );
        return ret;
    }
    swT0 = INVALID_REG_INDEX;
    swT1 = INVALID_REG_INDEX;
    hwT0 = INVALID_REG_INDEX;
    hwT1 = INVALID_REG_INDEX;
    MBEDTLS_MPI_CHK(
        mbedtls_alloc_sram_for_dedicate_reg( len_id, &hwT0, &hwT1, NULL ) );
    MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, &swT0 ) );
    MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, &swT1 ) );

    /*for DIV operation, opA will be modified when call ACA_DIV,
     * so we need to alloc temp reg TA. and opB == Res is also
     * not allowed by ACA_DIV, so if opB == Res, we alloc tmp reg
     * TB.
     */

    if ( Q == NULL )
    {
        TQ = INVALID_REG_INDEX;  // invalid reg
    }
    else
    {
        MBEDTLS_MPI_CHK(
            mbedtls_mpi_to_reg( &TQ, Q, len_id, DBH_TRUE, DBH_TRUE ) );
        Q->s = A->s * B->s;
    }

    if ( R == NULL )
    {
        TR = INVALID_REG_INDEX;  // invalid reg
    }
    else
    {
        MBEDTLS_MPI_CHK(
            mbedtls_mpi_to_reg( &TR, R, len_id, DBH_TRUE, DBH_TRUE ) );
        R->s = A->s;
    }
    dubhe_aca_set_n_np_t0_t1_reg( 0, 0, hwT0, hwT1 );
    ret = dubhe_aca_div( len_id, TQ, TR, T1, T2, swT0, swT1 );

cleanup:
    mbedtls_free_sram_for_dedicate_reg( &hwT0, &hwT1, NULL );
    mbedtls_free_tmp_reg_and_sram( swT0 );
    mbedtls_free_tmp_reg_and_sram( swT1 );
    if ( Q != NULL )
    {
        mbedtls_reset_op_flag( Q );
    }
    if ( R != NULL )
    {
        mbedtls_reset_op_flag( R );
    }
    mbedtls_reset_op_flag( (mbedtls_mpi *) A );
    mbedtls_reset_op_flag( (mbedtls_mpi *) B );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    //  mbedtls_printf("%s end\n",__func__);

    return ret;
}
static int _mbedtls_mpi_div_int( mbedtls_mpi *Q,
                         mbedtls_mpi *R,
                         const mbedtls_mpi *A,
                         mbedtls_mpi_sint b )
{
    int ret;
    mbedtls_mpi _B;

    _mbedtls_mpi_init( &_B );

    _B.s = ( b < 0 ) ? -1 : 1;
    MBEDTLS_MPI_CHK( _mbedtls_mpi_grow( &_B, 1 ) );
    _B.p[0] = ( b < 0 ) ? -b : b;
    MBEDTLS_MPI_CHK( _mbedtls_mpi_div_mpi( Q, R, A, &_B ) );
    if ( Q != NULL )
    {
        MBEDTLS_MPI_CHK( mbedtls_reg_mpi_sync( Q ) );
    }

    if ( R != NULL )
    {
        MBEDTLS_MPI_CHK( mbedtls_reg_mpi_sync( R ) );
    }
cleanup:
    _mbedtls_mpi_free( &_B );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}
static int _mbedtls_mpi_mod_mpi( mbedtls_mpi *R,
                         const mbedtls_mpi *A,
                         const mbedtls_mpi *B )
{
    int ret;

    ret = _mbedtls_mpi_cmp_int( B, 0 );

    if ( ret < 0 )
    {
        ret = MBEDTLS_ERR_MPI_NEGATIVE_VALUE;
        goto cleanup;
    }

    if ( ret == 0 )
    {
        ret = MBEDTLS_ERR_MPI_DIVISION_BY_ZERO;
        goto cleanup;
    }

    MBEDTLS_MPI_CHK( _mbedtls_mpi_div_mpi( NULL, R, A, B ) );

    while ( _mbedtls_mpi_cmp_int( R, 0 ) < 0 )
        MBEDTLS_MPI_CHK( _mbedtls_mpi_add_mpi( R, R, B ) );

    while ( _mbedtls_mpi_cmp_mpi( R, B ) >= 0 )
        MBEDTLS_MPI_CHK( _mbedtls_mpi_sub_mpi( R, R, B ) );

    R->s = 1;
cleanup:
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}
static int _mbedtls_mpi_mod_int( mbedtls_mpi_uint *r,
                         const mbedtls_mpi *A,
                         mbedtls_mpi_sint b )
{
    int ret;
    mbedtls_mpi _B, _R;

    if ( b == 0 )
    {
        return MBEDTLS_ERR_MPI_DIVISION_BY_ZERO;
    }

    if ( b < 0 )
    {
        return MBEDTLS_ERR_MPI_NEGATIVE_VALUE;
    }

    /*
     * handle trivial cases
     */
    if ( b == 1 )
    {
        *r = 0;
        return 0;
    }

    if ( b == 2 )
    {
        *r = A->p[0] & 1;
        return 0;
    }
    _mbedtls_mpi_init( &_B );
    _mbedtls_mpi_init( &_R );
    _B.s = ( b < 0 ) ? -1 : 1;
    _R.s = 1;
    MBEDTLS_MPI_CHK( _mbedtls_mpi_grow( &_B, 1 ) );
    _B.p[0] = ( b < 0 ) ? -b : b;

    MBEDTLS_MPI_CHK( _mbedtls_mpi_mod_mpi( &_R, A, &_B ) );
    MBEDTLS_MPI_CHK( mbedtls_reg_mpi_sync( &_R ) );
    *r = _R.p[0];

cleanup:
    _mbedtls_mpi_free( &_R );
    _mbedtls_mpi_free( &_B );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}

void mbedtls_mpi_init( mbedtls_mpi *X )
{
    if ( X == NULL )
    {
        return;
    }
    if ( !_g_dubhe_bignum_init_flag )
    {
        mbedtls_dubhe_init( );
        _g_dubhe_bignum_init_flag = 1;
    }
    HW_OPS_MUTEX_LOCK;
    _mbedtls_mpi_init(X);
    HW_OPS_MUTEX_UNLOCK;
}

/*
 * Unallocate one MPI
 */
void mbedtls_mpi_free( mbedtls_mpi *X )
{
    HW_OPS_MUTEX_LOCK;
    _mbedtls_mpi_free(X);
    HW_OPS_MUTEX_UNLOCK;
}

/*
 * Enlarge to the specified number of limbs
 */

int mbedtls_mpi_grow( mbedtls_mpi *X, size_t nblimbs )
{
    int ret;
    HW_OPS_MUTEX_LOCK;
    ret = _mbedtls_mpi_grow(X,nblimbs);
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}


/*
 * Resize down as much as possible,
 * while keeping at least the specified number of limbs
 */

int mbedtls_mpi_shrink( mbedtls_mpi *X, size_t nblimbs )
{
    int ret;
    HW_OPS_MUTEX_LOCK;
    ret = _mbedtls_mpi_shrink(X,nblimbs);
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}


/*
 * Copy the contents of Y into X
 */

int mbedtls_mpi_copy( mbedtls_mpi *X, const mbedtls_mpi *Y )
{
    int ret;
    HW_OPS_MUTEX_LOCK;
    ret = _mbedtls_mpi_copy(X,Y);
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Set value from integer
 */

int mbedtls_mpi_lset( mbedtls_mpi *X, mbedtls_mpi_sint z )
{
    int ret;
    HW_OPS_MUTEX_LOCK;
    ret = _mbedtls_mpi_lset(X,z);
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Return the number of bits
 */

size_t mbedtls_mpi_bitlen( const mbedtls_mpi *X )
{
    size_t i;

    HW_OPS_MUTEX_LOCK;
    i = _mbedtls_mpi_bitlen(X);
    HW_OPS_MUTEX_UNLOCK;

    return i;
}

/*
 * Return the total size in bytes
 */
size_t mbedtls_mpi_size( const mbedtls_mpi *X )
{
    size_t ret;
    HW_OPS_MUTEX_LOCK;
    ret = _mbedtls_mpi_size(X);
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Compare signed values
 */

int mbedtls_mpi_cmp_mpi( const mbedtls_mpi *X, const mbedtls_mpi *Y )
{
    int ret;

    HW_OPS_MUTEX_LOCK;
    ret = _mbedtls_mpi_cmp_mpi(X,Y);
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Compare signed values
 */
int mbedtls_mpi_cmp_int( const mbedtls_mpi *X, mbedtls_mpi_sint z )
{
    int ret;

    HW_OPS_MUTEX_LOCK;
    ret = _mbedtls_mpi_cmp_int(X,z);
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Signed addition: X = A + B
 */
int mbedtls_mpi_add_mpi( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         const mbedtls_mpi *B )
{
    int ret;
    HW_OPS_MUTEX_LOCK;
    ret = _mbedtls_mpi_add_mpi(X,A,B);
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Signed subtraction: X = A - B
 */

int mbedtls_mpi_sub_mpi( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         const mbedtls_mpi *B )
{
    int ret;
    HW_OPS_MUTEX_LOCK;
    ret = _mbedtls_mpi_sub_mpi(X,A,B);
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}
/*
 * Signed addition: X = A + b
 */

int mbedtls_mpi_add_int( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         mbedtls_mpi_sint b )
{
    int ret;
    HW_OPS_MUTEX_LOCK;
    ret = _mbedtls_mpi_add_int(X,A,b);
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Signed subtraction: X = A - b
 */

int mbedtls_mpi_sub_int( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         mbedtls_mpi_sint b )
{
    int ret;
    HW_OPS_MUTEX_LOCK;
    ret = _mbedtls_mpi_sub_int(X,A,b);
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Baseline multiplication: X = A * B  (HAC 14.12)
 */

int mbedtls_mpi_mul_mpi( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         const mbedtls_mpi *B )
{
    int ret;
    HW_OPS_MUTEX_LOCK;
    ret = _mbedtls_mpi_mul_mpi(X,A,B);
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Baseline multiplication: X = A * b
 */

int mbedtls_mpi_mul_int( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         mbedtls_mpi_uint b )
{
    int ret;
    HW_OPS_MUTEX_LOCK;
    ret = _mbedtls_mpi_mul_int(X,A,b);
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}


/*
 * Division by mbedtls_mpi: A = Q * B + R  (HAC 14.20)
 */

int mbedtls_mpi_div_mpi( mbedtls_mpi *Q,
                         mbedtls_mpi *R,
                         const mbedtls_mpi *A,
                         const mbedtls_mpi *B )
{
    int ret;
    HW_OPS_MUTEX_LOCK;
    ret = _mbedtls_mpi_div_mpi(Q,R,A,B);
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}


/*
 * Division by int: A = Q * b + R
 */

int mbedtls_mpi_div_int( mbedtls_mpi *Q,
                         mbedtls_mpi *R,
                         const mbedtls_mpi *A,
                         mbedtls_mpi_sint b )
{
    int ret;
    HW_OPS_MUTEX_LOCK;
    ret = _mbedtls_mpi_div_int(Q,R,A,b);
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Modulo: R = A mod B
 */

int mbedtls_mpi_mod_mpi( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         const mbedtls_mpi *B )
{
    int ret;
    HW_OPS_MUTEX_LOCK;
    ret = _mbedtls_mpi_mod_mpi(X,A,B);
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Modulo: r = A mod b
 */

int mbedtls_mpi_mod_int( mbedtls_mpi_uint *r,
                         const mbedtls_mpi *A,
                         mbedtls_mpi_sint b )
{
    int ret;
    HW_OPS_MUTEX_LOCK;
    ret = _mbedtls_mpi_mod_int(r,A,b);
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

static int _mbedtls_mpi_exp_mod( mbedtls_mpi *X,
                                 const mbedtls_mpi *A,
                                 const mbedtls_mpi *E,
                                 const mbedtls_mpi *N )
{
    int ret;
    unsigned int sizeNbits;
    unsigned char regTA, regTE, regTR, hwT0, hwT1, regNp, regN, len_id;

    HW_OPS_MUTEX_LOCK;
    hwT0      = INVALID_REG_INDEX;
    hwT1      = INVALID_REG_INDEX;
    regNp     = INVALID_REG_INDEX;
    sizeNbits = (unsigned int) _mbedtls_mpi_bitlen( N );
    if ( ( ret = mbedtls_get_len_id( &len_id,
                                     (mbedtls_mpi *) A,
                                     (mbedtls_mpi *) E,
                                     (mbedtls_mpi *) N,
                                     ACA_OPCODE_ID_MODEXP ) )
         != 0 )
    {
        HW_OPS_MUTEX_UNLOCK;
        return ret;
    }

    if ( ( ret = mbedtls_mpi_to_reg(
               &regN, (mbedtls_mpi *) N, len_id, DBH_TRUE, DBH_FALSE ) )
         != 0 )
    {
        HW_OPS_MUTEX_UNLOCK;
        mbedtls_reset_op_flag( (mbedtls_mpi *) N );
        return ret;
    }

    MBEDTLS_MPI_CHK( mbedtls_calc_np( len_id, regN, sizeNbits, &regNp ) );
    MBEDTLS_MPI_CHK(
        mbedtls_alloc_sram_for_dedicate_reg( len_id, &hwT0, &hwT1, NULL ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regTA, (mbedtls_mpi *) A, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regTE, (mbedtls_mpi *) E, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK(
        mbedtls_mpi_to_reg( &regTR, X, len_id, DBH_TRUE, DBH_TRUE ) );
    dubhe_aca_set_n_np_t0_t1_reg( regN, regNp, hwT0, hwT1 );
    MBEDTLS_MPI_CHK( dubhe_aca_mod_exp( len_id, regTR, regTA, regTE ) );

cleanup:
    mbedtls_free_sram_for_dedicate_reg( &hwT0, &hwT1, &regNp );
    mbedtls_reset_op_flag( X );
    mbedtls_reset_op_flag( (mbedtls_mpi *) A );
    mbedtls_reset_op_flag( (mbedtls_mpi *) N );
    mbedtls_reset_op_flag( (mbedtls_mpi *) E );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

static int _mbedtls_mpi_exp_mod_largeE( mbedtls_mpi *X,
                                        const mbedtls_mpi *A,
                                        const mbedtls_mpi *E,
                                        const mbedtls_mpi *N )
{
    int ret;
    mbedtls_mpi tmpEQ, tmpER, tmpR, tmpA, tmpE, tmpT1,tmpRR, tmpEM;
    mbedtls_mpi_init( &tmpEQ );
    mbedtls_mpi_init( &tmpER );
    mbedtls_mpi_init( &tmpR );
    mbedtls_mpi_init( &tmpT1 );
    mbedtls_mpi_init( &tmpA );
    mbedtls_mpi_init( &tmpE );
    mbedtls_mpi_init( &tmpRR );
    mbedtls_mpi_init( &tmpEM );

    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &tmpRR, 1 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &tmpEM, 1 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_shift_l( &tmpEM, mbedtls_mpi_bitlen( N ) ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_sub_int( &tmpEM, &tmpEM, 1) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &tmpA, A ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &tmpE, E ) );
    while ( true ) {
        MBEDTLS_MPI_CHK( mbedtls_mpi_div_mpi( &tmpEQ, &tmpER, &tmpE, &tmpEM ) );
        MBEDTLS_MPI_CHK( _mbedtls_mpi_exp_mod( &tmpR, &tmpA, &tmpER, N ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &tmpRR, &tmpRR, &tmpR, N ) );
        if ( mbedtls_mpi_cmp_int( &tmpEQ, 0 ) == 0 ) {
            break;
        }
        MBEDTLS_MPI_CHK( _mbedtls_mpi_exp_mod( &tmpT1, &tmpA, &tmpEM, N ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &tmpA, &tmpT1 ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &tmpE, &tmpEQ ) );
    }

    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( X, &tmpRR ) );

cleanup:
    mbedtls_mpi_free( &tmpEQ );
    mbedtls_mpi_free( &tmpER );
    mbedtls_mpi_free( &tmpR );
    mbedtls_mpi_free( &tmpT1 );
    mbedtls_mpi_free( &tmpA );
    mbedtls_mpi_free( &tmpE );
    mbedtls_mpi_free( &tmpRR );
    mbedtls_mpi_free( &tmpEM );
    return ret;
}
/*
 * Sliding-window exponentiation: X = A^E mod N  (HAC 14.85)
 */
int mbedtls_mpi_exp_mod( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         const mbedtls_mpi *E,
                         const mbedtls_mpi *N,
                         mbedtls_mpi *_RR )
{
    int ret;
    unsigned int sizeNbits;
    mbedtls_mpi tmpA;

    mbedtls_mpi_init( &tmpA );

    /*mbedtls can't support N is even, but Dubhe support*/
    if ( mbedtls_mpi_cmp_int( N, 0 ) <= 0 || ( mbedtls_mpi_get_bit(N,0) & 1 ) == 0 )
    {
        ret = MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
        goto cleanup;
    }

    if ( mbedtls_mpi_cmp_int( E, 0 ) < 0 )
    {
        ret = MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
        goto cleanup;
    }
    sizeNbits = (unsigned int) mbedtls_mpi_bitlen( N );

    if ( mbedtls_mpi_bitlen( A ) > sizeNbits )
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &tmpA, A, N ) );
        MBEDTLS_MPI_CHK( mbedtls_reg_mpi_sync( &tmpA ) );
    }
    else
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &tmpA, A ) );
    }

    if ( mbedtls_mpi_bitlen( E ) > sizeNbits )
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_exp_mod_largeE( X, &tmpA, E, N ) );
    }
    else
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_exp_mod( X, &tmpA, E, N ) );
    }

    if ( NULL != _RR )
    {
        _RR->p = NULL; /*RR is not used here*/
    }

    if ( tmpA.s == -1 && E->n != 0 && ( E->p[0] & 1 ) != 0 )
    {
        X->s = -1;
        MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( X, X, N ) );
    }
cleanup:
    mbedtls_mpi_free( &tmpA );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}

/*
 * Greatest common divisor: G = gcd(A, B)  (HAC 14.54)
 */
int mbedtls_mpi_gcd( mbedtls_mpi *G,
                     const mbedtls_mpi *A,
                     const mbedtls_mpi *B )
{
    int ret;
    size_t lz, lzt;
    mbedtls_mpi TG, TA, TB;

    mbedtls_mpi_init( &TG );
    mbedtls_mpi_init( &TA );
    mbedtls_mpi_init( &TB );

    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &TA, A ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &TB, B ) );
    lz  = mbedtls_mpi_lsb( &TA );
    lzt = mbedtls_mpi_lsb( &TB );
    if ( lzt < lz )
    {
        lz = lzt;
    }

    MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( &TA, lz ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( &TB, lz ) );
    TA.s = TB.s = 1;
    while ( mbedtls_mpi_cmp_int( &TA, 0 ) != 0 )
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( &TA, mbedtls_mpi_lsb( &TA ) ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( &TB, mbedtls_mpi_lsb( &TB ) ) );

        if ( mbedtls_mpi_cmp_mpi( &TA, &TB ) >= 0 )
        {
            if ( mbedtls_mpi_cmp_int( &TA, 0 ) != 0 )
            {
                MBEDTLS_MPI_CHK( mbedtls_mpi_sub_abs( &TA, &TA, &TB ) );
                MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( &TA, 1 ) );
                MBEDTLS_MPI_CHK( mbedtls_reg_mpi_sync( &TA ) );
            }
        }
        else
        {
            if ( mbedtls_mpi_cmp_int( &TB, 0 ) != 0 )
            {
                MBEDTLS_MPI_CHK( mbedtls_mpi_sub_abs( &TB, &TB, &TA ) );
                MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( &TB, 1 ) );
                MBEDTLS_MPI_CHK( mbedtls_reg_mpi_sync( &TB ) );
            }
        }
    }

    MBEDTLS_MPI_CHK( mbedtls_mpi_shift_l( &TB, lz ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( G, &TB ) );
cleanup:

    mbedtls_mpi_free( &TG );
    mbedtls_mpi_free( &TA );
    mbedtls_mpi_free( &TB );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}

/*
 * Fill X with size bytes of random.
 *
 * Use a temporary bytes representation to make sure the result is the same
 * regardless of the platform endianness (useful when f_rng is actually
 * deterministic, eg for tests).
 */
int mbedtls_mpi_fill_random( mbedtls_mpi *X,
                             size_t size,
                             int ( *f_rng )( void *, unsigned char *, size_t ),
                             void *p_rng )
{
    int ret;
    unsigned char buf[MBEDTLS_MPI_MAX_SIZE];

    if ( size > MBEDTLS_MPI_MAX_SIZE )
    {
        return MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
    }

    MBEDTLS_MPI_CHK( f_rng( p_rng, buf, size ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( X, buf, size ) );

cleanup:
    mbedtls_zeroize( buf, sizeof( buf ) );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}

static int _mbedtls_mpi_inv_mod_even( mbedtls_mpi *X,
                                      mbedtls_mpi *A,
                                      const mbedtls_mpi *N )
{
    int ret, i;
    unsigned int sizeNbits;
    unsigned char regTA, regTR, regTN, hwT0, hwT1,
        T[INV_MOD_EVEN_MAX_TMP_REG_NUM], len_id;

    HW_OPS_MUTEX_LOCK;

    hwT0 = INVALID_REG_INDEX;
    hwT1 = INVALID_REG_INDEX;
    memset( T, INVALID_REG_INDEX, sizeof( T ) );
    sizeNbits = _mbedtls_mpi_bitlen( N );
    if ( sizeNbits >= MAX_OP_BITS_HALF / 2 )
    {
        mbedtls_rel_dubhe_resource( );
    }
    MBEDTLS_MPI_CHK( mbedtls_get_len_id(
        &len_id, A, NULL, (mbedtls_mpi *) N, ACA_VIRTUAL_OPCODE_LONG_DIV ) );
    MBEDTLS_MPI_CHK(
        mbedtls_alloc_sram_for_dedicate_reg( len_id, &hwT0, &hwT1, NULL ) );
    MBEDTLS_MPI_CHK(
        mbedtls_mpi_to_reg( &regTA, A, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regTN, (mbedtls_mpi *) N, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK(
        mbedtls_mpi_to_reg( &regTR, X, len_id, DBH_TRUE, DBH_TRUE ) );
    for ( i = 0; i < INV_MOD_EVEN_MAX_TMP_REG_NUM; i++ )
    {
        MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, &T[i] ) );
    }

    dubhe_aca_set_n_np_t0_t1_reg( regTA, 0, hwT0, hwT1 );
    MBEDTLS_MPI_CHK(
        dubhe_aca_mod_inv_even( len_id, regTR, regTA, regTN, sizeNbits, T ) );
    MBEDTLS_MPI_CHK(
        _mbedtls_mpi_shrink( X, CALC_FULL_32BIT_WORDS( sizeNbits ) ) );

cleanup:
    mbedtls_rel_dubhe_res_by_mpi( X );
    mbedtls_rel_dubhe_res_by_mpi( A );
    mbedtls_rel_dubhe_res_by_mpi( (mbedtls_mpi *) N );
    mbedtls_free_sram_for_dedicate_reg( &hwT0, &hwT1, NULL );

    for ( i = 0; i < INV_MOD_EVEN_MAX_TMP_REG_NUM; i++ )
    {
        if ( T[i] != INVALID_REG_INDEX )
            mbedtls_free_tmp_reg_and_sram( T[i] );
    }
    mbedtls_reset_op_flag( X );
    mbedtls_reset_op_flag( (mbedtls_mpi *) A );
    mbedtls_reset_op_flag( (mbedtls_mpi *) N );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

static int
_mbedtls_mpi_inv_mod_odd( mbedtls_mpi *X, mbedtls_mpi *A, const mbedtls_mpi *N )
{
    int ret;
    unsigned char regTA, regTR, regTN, hwT0, hwT1, swT0, len_id;

    HW_OPS_MUTEX_LOCK;
    hwT0 = INVALID_REG_INDEX;
    hwT1 = INVALID_REG_INDEX;
    swT0 = INVALID_REG_INDEX;
    MBEDTLS_MPI_CHK( mbedtls_get_len_id( &len_id,
                                         (mbedtls_mpi *) A,
                                         NULL,
                                         (mbedtls_mpi *) N,
                                         ACA_OPCODE_ID_MODINV ) );
    MBEDTLS_MPI_CHK(
        mbedtls_alloc_sram_for_dedicate_reg( len_id, &hwT0, &hwT1, NULL ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regTA, (mbedtls_mpi *) A, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regTN, (mbedtls_mpi *) N, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK(
        mbedtls_mpi_to_reg( &regTR, X, len_id, DBH_TRUE, DBH_TRUE ) );
    MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, &swT0 ) );
    dubhe_aca_set_n_np_t0_t1_reg( regTN, 0, hwT0, hwT1 );
    ret = dubhe_aca_mod_inv( len_id, regTR, regTA, swT0 );
    if ( ret )
    {
        mbedtls_printf( "no valid INV for input data\n" );
        ret = MBEDTLS_ERR_MPI_NOT_ACCEPTABLE;
    }
cleanup:
    mbedtls_free_sram_for_dedicate_reg( &hwT0, &hwT1, NULL );
    mbedtls_free_tmp_reg_and_sram( swT0 );
    mbedtls_reset_op_flag( X );
    mbedtls_reset_op_flag( (mbedtls_mpi *) A );
    mbedtls_reset_op_flag( (mbedtls_mpi *) N );
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Modular inverse: X = A^-1 mod N  (HAC 14.61 / 14.64)
 */
int mbedtls_mpi_inv_mod( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         const mbedtls_mpi *N )
{
    int ret;
    mbedtls_mpi tmpA;

    mbedtls_mpi_init( &tmpA );

    if ( mbedtls_mpi_cmp_int( N, 1 ) <= 0 )
    {
        ret = MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
        goto cleanup;
    }
    if ( mbedtls_mpi_cmp_int( A, 0 ) == 0 )
    {
        mbedtls_printf( "%s MBEDTLS_ERR_MPI_BAD_INPUT_DATA\n", __func__ );
        ret = MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
        goto cleanup;
    }
    if ( ( mbedtls_mpi_get_bit( N, 0 ) == 0 )
         && ( mbedtls_mpi_get_bit( A, 0 ) == 0 ) )
    {
        ret = MBEDTLS_ERR_MPI_NOT_ACCEPTABLE;
        goto cleanup;
    }

    MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &tmpA, A, N ) );
    MBEDTLS_MPI_CHK( mbedtls_reg_mpi_sync( &tmpA ) );
    /*N is ODD*/
    if ( mbedtls_mpi_get_bit( N, 0 ) == 1 )
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_inv_mod_odd( X, &tmpA, N ) );
    }
    else
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_inv_mod_even( X, &tmpA, N ) );
    }
cleanup:
    mbedtls_mpi_free( &tmpA );

#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    return ret;
}

/*
 * Modular multiplication: X = A*B mod N
 */
int mbedtls_mpi_mul_mod( mbedtls_mpi *X,
                         const mbedtls_mpi *A,
                         const mbedtls_mpi *B,
                         const mbedtls_mpi *N )
{
    int ret;
    unsigned int sizeNbits;
    unsigned char regTA, regTB, regTR, hwT0, regN, regNp, swT0, swT1, len_id;
    mbedtls_mpi tmpA, tmpB;

    HW_OPS_MUTEX_LOCK;
    swT0  = INVALID_REG_INDEX;
    swT1  = INVALID_REG_INDEX;
    hwT0  = INVALID_REG_INDEX;
    regNp = INVALID_REG_INDEX;
    _mbedtls_mpi_init( &tmpA );
    _mbedtls_mpi_init( &tmpB );
    if ( _mbedtls_mpi_cmp_int( N, 0 ) <= 0 )
    {
        ret = MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
        goto cleanup;
    }
    sizeNbits = _mbedtls_mpi_bitlen( N );

    if ( _mbedtls_mpi_bitlen( A ) > sizeNbits )
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_mod_mpi( &tmpA, A, N ) );
        MBEDTLS_MPI_CHK( mbedtls_reg_mpi_sync( &tmpA ) );
    }
    else
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_copy( &tmpA, A ) );
    }
    if ( _mbedtls_mpi_bitlen( B ) > sizeNbits )
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_mod_mpi( &tmpB, B, N ) );
        MBEDTLS_MPI_CHK( mbedtls_reg_mpi_sync( &tmpB ) );
    }
    else
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_copy( &tmpB, B ) );
    }
    MBEDTLS_MPI_CHK( mbedtls_get_len_id( &len_id,
                                         (mbedtls_mpi *) &tmpA,
                                         (mbedtls_mpi *) &tmpB,
                                         (mbedtls_mpi *) N,
                                         ACA_OPCODE_ID_MODMUL ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regN, (mbedtls_mpi *) N, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_calc_np( len_id, regN, sizeNbits, &regNp ) );
    MBEDTLS_MPI_CHK(
        mbedtls_alloc_sram_for_dedicate_reg( len_id, &hwT0, NULL, NULL ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regTA, (mbedtls_mpi *) &tmpA, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regTB, (mbedtls_mpi *) &tmpB, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK(
        mbedtls_mpi_to_reg( &regTR, X, len_id, DBH_TRUE, DBH_TRUE ) );
    MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, &swT0 ) );
    MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, &swT1 ) );
    dubhe_aca_set_n_np_t0_t1_reg( regN, regNp, hwT0, 0 );
    ret = dubhe_aca_mod_mul( len_id, regTR, regTA, regTB, swT0, swT1 );

    if ( tmpA.s * tmpB.s < 0 )
    {
        MBEDTLS_MPI_CHK( dubhe_aca_mod_sub( len_id, regTR, regN, regTR ) );
        X->s = 1;
    }

cleanup:
    mbedtls_free_sram_for_dedicate_reg( &hwT0, NULL, &regNp );
    _mbedtls_mpi_free( &tmpA );
    _mbedtls_mpi_free( &tmpB );
    mbedtls_free_tmp_reg_and_sram( swT0 );
    mbedtls_free_tmp_reg_and_sram( swT1 );
    mbedtls_reset_op_flag( X );
    mbedtls_reset_op_flag( (mbedtls_mpi *) A );
    mbedtls_reset_op_flag( (mbedtls_mpi *) N );
    mbedtls_reset_op_flag( (mbedtls_mpi *) B );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

int mbedtls_mpi_mul_ecp_no_naf( mbedtls_mpi *Qx,
                                mbedtls_mpi *Qy,
                                mbedtls_mpi *Qz,
                                const mbedtls_mpi *d,
                                const mbedtls_mpi *Gx,
                                const mbedtls_mpi *Gy,
                                const mbedtls_mpi *Gz,
                                const mbedtls_mpi *A,
                                const mbedtls_mpi *P )
{
    int i;
    int j;
    int ret;
    unsigned int sizeNbits;
    unsigned char found_msb = 0;
    unsigned char regTA, regX1, regY1, regZ1, regX, regY, regZ;
    unsigned char hwT0, regN, regNp, T[MUL_ECP_MAX_TMP_REG_NUM], len_id;
    unsigned char dummyT1, dummyT2, dummyT3;
    mbedtls_mpi tmpA, tmpGx, tmpGy, tmpGz;
    mbedtls_mpi_uint mask;

    HW_OPS_MUTEX_LOCK;
    _mbedtls_mpi_init( &tmpA );
    _mbedtls_mpi_init( &tmpGx );
    _mbedtls_mpi_init( &tmpGy );
    _mbedtls_mpi_init( &tmpGz );
    hwT0  = INVALID_REG_INDEX;
    regNp = INVALID_REG_INDEX;
    memset( T, INVALID_REG_INDEX, sizeof( T ) );
    if ( A->p == NULL )
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_sub_int( &tmpA, P, 3 ) );
    }
    else
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_copy( &tmpA, A ) );
    }
    if ( Qx == Gx )
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_copy( &tmpGx, Gx ) );
    }
    if ( Qy == Gy )
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_copy( &tmpGy, Gy ) );
    }
    if ( Qz == Gz )
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_copy( &tmpGz, Gz ) );
    }

    MBEDTLS_MPI_CHK( mbedtls_get_len_id( &len_id,
                                         (mbedtls_mpi *) &tmpA,
                                         NULL,
                                         (mbedtls_mpi *) P,
                                         ACA_OPCODE_ID_MODMUL ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regN, (mbedtls_mpi *) P, len_id, DBH_TRUE, DBH_FALSE ) );
    sizeNbits = _mbedtls_mpi_bitlen( P );
    MBEDTLS_MPI_CHK( mbedtls_calc_np( len_id, regN, sizeNbits, &regNp ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regTA, (mbedtls_mpi *) &tmpA, len_id, DBH_TRUE, DBH_FALSE ) );
    if ( Qx == Gx )
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
            &regX1, (mbedtls_mpi *) &tmpGx, len_id, DBH_TRUE, DBH_FALSE ) );
    }
    else
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
            &regX1, (mbedtls_mpi *) Gx, len_id, DBH_TRUE, DBH_FALSE ) );
    }
    if ( Qy == Gy )
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
            &regY1, (mbedtls_mpi *) &tmpGy, len_id, DBH_TRUE, DBH_FALSE ) );
    }
    else
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
            &regY1, (mbedtls_mpi *) Gy, len_id, DBH_TRUE, DBH_FALSE ) );
    }
    if ( Qz == Gz )
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
            &regZ1, (mbedtls_mpi *) &tmpGz, len_id, DBH_TRUE, DBH_FALSE ) );
    }
    else
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
            &regZ1, (mbedtls_mpi *) Gz, len_id, DBH_TRUE, DBH_FALSE ) );
    }
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regX, (mbedtls_mpi *) Qx, len_id, DBH_TRUE, DBH_TRUE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regY, (mbedtls_mpi *) Qy, len_id, DBH_TRUE, DBH_TRUE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regZ, (mbedtls_mpi *) Qz, len_id, DBH_TRUE, DBH_TRUE ) );
    for ( i = 0; i < MUL_ECP_MAX_TMP_REG_NUM; i++ )
    {
        MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, &T[i] ) );
    }
    MBEDTLS_MPI_CHK(
        mbedtls_alloc_sram_for_dedicate_reg( len_id, &hwT0, NULL, NULL ) );
    dubhe_aca_set_n_np_t0_t1_reg( regN, regNp, hwT0, 0x1F );
    mbedtls_reg_mpi_sync( (mbedtls_mpi *) d );
    for ( i = d->n - 1; i > 0; i-- )
    {
        if ( d->p[i] != 0 )
        {
            break;
        }
    }
    /*dummyT1/dummyT2/dummyT3 used for timing attack resistant*/
    dummyT1 = T[MUL_ECP_MAX_TMP_REG_NUM - 1];
    dummyT2 = T[MUL_ECP_MAX_TMP_REG_NUM - 2];
    dummyT3 = T[MUL_ECP_MAX_TMP_REG_NUM - 3];
    for ( ; i >= 0; i-- )
    {
        mask = (mbedtls_mpi_uint) 1 << ( biL - 1 );
        for ( j = 0; j < biL; j++ )
        {
            if ( !found_msb )
            {
                if ( d->p[i] & mask )
                {
                    found_msb = 1;
                    dubhe_aca_add_im( len_id, regX, regX1, 0 );
                    dubhe_aca_add_im( len_id, regY, regY1, 0 );
                    dubhe_aca_add_im( len_id, regZ, regZ1, 0 );
                }
            }
            else
            {
                dubhe_aca_double_point(
                    len_id, T, regX, regY, regZ, regX, regY, regZ, regTA );
                dubhe_aca_add_im( len_id, dummyT1, regX, 0 );
                dubhe_aca_add_im( len_id, dummyT2, regY, 0 );
                dubhe_aca_add_im( len_id, dummyT3, regZ, 0 );
                if ( d->p[i] & mask )
                {
                    dubhe_aca_point_add( len_id,
                                         T,
                                         regN,
                                         regX,
                                         regY,
                                         regZ,
                                         regX1,
                                         regY1,
                                         regZ1,
                                         regX,
                                         regY,
                                         regZ );
                }
                else
                {
                    /*dummy point add operation*/
                    dubhe_aca_point_add( len_id,
                                         T,
                                         regN,
                                         dummyT1,
                                         dummyT2,
                                         dummyT3,
                                         regX1,
                                         regY1,
                                         regZ1,
                                         dummyT1,
                                         dummyT2,
                                         dummyT3 );
                }
            }
            mask >>= 1;
        }
    }

cleanup:
    mbedtls_free_sram_for_dedicate_reg( &hwT0, NULL, &regNp );
    _mbedtls_mpi_free( &tmpA );
    _mbedtls_mpi_free( &tmpGx );
    _mbedtls_mpi_free( &tmpGy );
    _mbedtls_mpi_free( &tmpGz );
    for ( i = 0; i < MUL_ECP_MAX_TMP_REG_NUM; i++ )
    {
        if ( T[i] != INVALID_REG_INDEX )
            mbedtls_free_tmp_reg_and_sram( T[i] );
    }
    mbedtls_reset_op_flag( Qx );
    mbedtls_reset_op_flag( Qy );
    mbedtls_reset_op_flag( Qz );
    mbedtls_reset_op_flag( (mbedtls_mpi *) Gx );
    mbedtls_reset_op_flag( (mbedtls_mpi *) Gy );
    mbedtls_reset_op_flag( (mbedtls_mpi *) Gz );
    mbedtls_reset_op_flag( (mbedtls_mpi *) A );
    mbedtls_reset_op_flag( (mbedtls_mpi *) d );
    mbedtls_reset_op_flag( (mbedtls_mpi *) P );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

static size_t
_mbedtls_add_counter( size_t *pCntBuff, size_t val, size_t cntSize )
{
    size_t currCntVal;
    size_t i;

    for ( i = 0; i < cntSize; i++ )
    {
        currCntVal = pCntBuff[i];
        pCntBuff[i] += val;
        val = ( currCntVal > pCntBuff[i] ) ? 1 : 0;
    }

    return val;
}

/*Divides a vector by 2 - in a secured way*/
static void
_mbedtls_divide_by2( size_t *pVecBuff,    /*!< [in/out] Vector buffer. */
                     size_t SizeInWords ) /*!< [in] Size of vecrot in Words. */
{
    size_t i;
    size_t Temp;

    /* for loop for dividing the vectors arrays by 2 */
    for ( i = 0; i < (SizeInWords) -1; i++ )
    {
        pVecBuff[i] = pVecBuff[i] >> 1;
        Temp        = pVecBuff[i + 1] & 1UL;
        pVecBuff[i] = pVecBuff[i] | Temp << ( 32 - 1 );
    }

    /* dividing the MS word */
    pVecBuff[SizeInWords - 1] = pVecBuff[SizeInWords - 1] >> 1;

    return;
}

static int _mbedtls_build_naf( char **naf,
                               size_t *naf_size,
                               mbedtls_mpi_uint *pbuf,
                               size_t psize_bits )
{
    int ret = 0;
    size_t wK, i = 0;
    char *p; /* a pointer to the current NAF digit */

    if ( psize_bits == 0 || ( psize_bits + 2 ) > *naf_size )
    {
        ret = -1;
        goto finish;
    }
    /* MSBit must be 1 */
    if ( ( pbuf[( psize_bits - 1 ) / biL] >> ( ( psize_bits - 1 ) & 0x1F ) )
         != 1 )
    {
        ret = -1;
        goto finish;
    }

    /* set initial values */
    *naf_size = 0;                     /*NAF size in bytes*/
    p         = *naf + psize_bits + 1; /* start from the last byte */
    *p        = 0;
    wK = CALC_FULL_32BIT_WORDS( psize_bits ) /*+1*/; /*key size + extra word*/
    /* zeroing extra word of key buffer */
    pbuf[wK] = 0;

    /* scan key bits and convert to NAF */
    while ( psize_bits )
    {
        uint32_t carry, msBit;

        i++;
        ( *naf_size )++;
        --p;
        /* check overflow */
        if ( p < *naf )
        {
            ret = -1;
            goto finish;
        }
        /* set NAF digit */
        *p = ( pbuf[0] & 1 ) ? ( ( pbuf[0] & 2 ) ? '-' : '+' ) : '0';

        msBit = pbuf[wK - 1]
                >> ( ( psize_bits - 1 ) & 0x1F );  //((psize_bits%biL)-1);
        if ( *p == '-' )
        {
            carry = _mbedtls_add_counter( (size_t *) pbuf, 1, wK );  // k += 1
            if ( carry )
            {
                pbuf[wK] = 1;
                psize_bits++;
            }
            else if ( ( pbuf[wK - 1] >> ( ( psize_bits - 1 ) & 0x1F ) )
                      > msBit )
            {
                psize_bits++;
            }
        }

        _mbedtls_divide_by2( (size_t *) pbuf, wK + 1 );  // k >>= 1
        psize_bits--;

        /* if MSbit is zeroed set new size value */
        wK = ( CALC_FULL_32BIT_WORDS( psize_bits ) );
    }

    /* actual NAF vector begin */
    *naf = p;
finish:
    return ret;
}

void dump_mpi_to_string( const mbedtls_mpi *X, char *name )
{
    int ret;
    char p[1000];
    size_t plen, olen;

    mpi_reg_map_node_t *cur;


    cur = (mpi_reg_map_node_t *) X->mhandle;
    if ( cur == NULL )
    {
        mbedtls_printf( "%s ERROR MPI not inititialized !!!\n", __func__ );
        return;
    }

    plen = sizeof( p );
    memset( p, 0, plen );

    if ( ( ret = mbedtls_mpi_write_string( X, 16, p, plen, &olen ) != 0 ) )
    {
        mbedtls_printf( " failed\n  ! mbedtls_mpi_write_file returned %x\n\n",
                        -ret );
        goto exit;
    }
    mbedtls_printf( "%s:%s len:%d reg:%d\n", name, p, olen,cur->map_info.reg_index);
exit:
    return;
}

int mbedtls_mpi_mul_ecp( mbedtls_mpi *Qx,
                         mbedtls_mpi *Qy,
                         mbedtls_mpi *Qz,
                         const mbedtls_mpi *d,
                         const mbedtls_mpi *Gx,
                         const mbedtls_mpi *Gy,
                         const mbedtls_mpi *Gz,
                         const mbedtls_mpi *A,
                         const mbedtls_mpi *P )
{
    int i;
    int ret;
    unsigned int sizeNbits;
    unsigned int sizeDbits, naf_size;
    unsigned char regTA, regX1, regY1, regZ1, regX, regY, regZ;
    unsigned char hwT0, hwT1, regN, regNp, T[MUL_ECP_MAX_TMP_REG_NUM], len_id;
    unsigned char regNegY1, regW;
    mbedtls_mpi tmpA, tmpGx, tmpGy, tmpGz, tmpD;
    char *naf = NULL, *ptr = NULL;

    HW_OPS_MUTEX_LOCK;
    _mbedtls_mpi_init( &tmpD );
    _mbedtls_mpi_init( &tmpA );
    _mbedtls_mpi_init( &tmpGx );
    _mbedtls_mpi_init( &tmpGy );
    _mbedtls_mpi_init( &tmpGz );
    hwT0  = INVALID_REG_INDEX;
    hwT1  = INVALID_REG_INDEX;
    regNp = INVALID_REG_INDEX;
    memset( T, INVALID_REG_INDEX, sizeof( T ) );

    if ( A->p == NULL )
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_sub_int( &tmpA, P, 3 ) );
    }
    else
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_copy( &tmpA, A ) );
    }
    if ( Qx == Gx )
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_copy( &tmpGx, Gx ) );
    }
    if ( Qy == Gy )
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_copy( &tmpGy, Gy ) );
    }
    if ( Qz == Gz )
    {
        MBEDTLS_MPI_CHK( _mbedtls_mpi_copy( &tmpGz, Gz ) );
    }

    sizeDbits = _mbedtls_mpi_bitlen( d );
    MBEDTLS_MPI_CHK( _mbedtls_mpi_copy( &tmpD, d ) );
    MBEDTLS_MPI_CHK(
        _mbedtls_mpi_grow( &tmpD, ( sizeDbits + biL - 1 ) / biL + 1 ) );

    MBEDTLS_MPI_CHK( mbedtls_get_len_id( &len_id,
                                         (mbedtls_mpi *) &tmpA,
                                         NULL,
                                         (mbedtls_mpi *) P,
                                         ACA_OPCODE_ID_MODMUL ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regN, (mbedtls_mpi *) P, len_id, DBH_TRUE, DBH_FALSE ) );
    sizeNbits = _mbedtls_mpi_bitlen( P );
    MBEDTLS_MPI_CHK( mbedtls_calc_np( len_id, regN, sizeNbits, &regNp ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regTA, (mbedtls_mpi *) &tmpA, len_id, DBH_TRUE, DBH_FALSE ) );
    if ( Qx == Gx )
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
            &regX1, (mbedtls_mpi *) &tmpGx, len_id, DBH_TRUE, DBH_FALSE ) );
    }
    else
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
            &regX1, (mbedtls_mpi *) Gx, len_id, DBH_TRUE, DBH_FALSE ) );
    }
    if ( Qy == Gy )
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
            &regY1, (mbedtls_mpi *) &tmpGy, len_id, DBH_TRUE, DBH_FALSE ) );
    }
    else
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
            &regY1, (mbedtls_mpi *) Gy, len_id, DBH_TRUE, DBH_FALSE ) );
    }
    if ( Qz == Gz )
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
            &regZ1, (mbedtls_mpi *) &tmpGz, len_id, DBH_TRUE, DBH_FALSE ) );
    }
    else
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
            &regZ1, (mbedtls_mpi *) Gz, len_id, DBH_TRUE, DBH_FALSE ) );
    }

    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regX, (mbedtls_mpi *) Qx, len_id, DBH_TRUE, DBH_TRUE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regY, (mbedtls_mpi *) Qy, len_id, DBH_TRUE, DBH_TRUE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regZ, (mbedtls_mpi *) Qz, len_id, DBH_TRUE, DBH_TRUE ) );
    for ( i = 0; i < MUL_ECP_MAX_TMP_REG_NUM; i++ )
    {
        MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, &T[i] ) );
    }
    MBEDTLS_MPI_CHK(
        mbedtls_alloc_sram_for_dedicate_reg( len_id, &hwT0, &hwT1, NULL ) );

    naf_size = ( ( sizeDbits + biL - 1 ) / biL + 1 ) * biL;
    if ( ( ptr = (char *) mbedtls_calloc( naf_size, sizeof( char ) ) ) == NULL )
    {
        ret = MBEDTLS_ERR_MPI_ALLOC_FAILED;
        goto cleanup;
    }

    naf = ptr;
    MBEDTLS_MPI_CHK( _mbedtls_build_naf( &naf, &naf_size, tmpD.p, sizeDbits ) );

    dubhe_aca_set_n_np_t0_t1_reg( regN, regNp, hwT0, hwT1 );
    regNegY1 = T[MUL_ECP_MAX_TMP_REG_NUM
                 - 1];  // the last tempT used to record Y1 o -P
    regW     = T[MUL_ECP_MAX_TMP_REG_NUM
             - 2];  // the second last tempT used to A*Z^4,marked as w
    dubhe_aca_copy( len_id, regW, regTA );  // w=a*z^4=a*1*1*1*1(z=1)=a
    dubhe_aca_sub( len_id, regNegY1, regN, regY1 );
    dubhe_aca_add_im( len_id, regX, regX1, 0 );
    dubhe_aca_add_im( len_id, regY, regY1, 0 );
    dubhe_aca_add_im( len_id, regZ, regZ1, 0 );
    if ( *naf == '+' )
        naf++;

    while ( *naf )
    {
        if ( *naf == '0' )
        {
            dubhe_aca_double_point_jj( len_id, T, regX, regY, regZ, regW, 1 );
        }
        else
        {
            dubhe_aca_double_point_jj( len_id, T, regX, regY, regZ, regW, 0 );
            if ( *naf == '+' )
            {
                dubhe_aca_point_add_ajj(
                    len_id, T, regX, regY, regZ, regX1, regY1, regTA, regW, 1 );
            }
            else
            { /**naf == '-'*/
                dubhe_aca_point_add_ajj( len_id,
                                         T,
                                         regX,
                                         regY,
                                         regZ,
                                         regX1,
                                         regNegY1,
                                         regTA,
                                         regW,
                                         1 );
            }
        }
        naf++;
    }
    if ( _mbedtls_mpi_cmp_int( Qz, 0 ) == 0 )
    {
        ret = MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
    }
cleanup:
    mbedtls_free_sram_for_dedicate_reg( &hwT0, &hwT1, &regNp );
    _mbedtls_mpi_free( &tmpD );
    _mbedtls_mpi_free( &tmpA );
    _mbedtls_mpi_free( &tmpGx );
    _mbedtls_mpi_free( &tmpGy );
    _mbedtls_mpi_free( &tmpGz );
    if ( ptr != NULL )
    {
        mbedtls_free( ptr );
        naf = NULL;
        ptr = NULL;
    }
    for ( i = 0; i < MUL_ECP_MAX_TMP_REG_NUM; i++ )
    {
        if ( T[i] != INVALID_REG_INDEX )
            mbedtls_free_tmp_reg_and_sram( T[i] );
    }
    mbedtls_reset_op_flag( Qx );
    mbedtls_reset_op_flag( Qy );
    mbedtls_reset_op_flag( Qz );
    mbedtls_reset_op_flag( (mbedtls_mpi *) Gx );
    mbedtls_reset_op_flag( (mbedtls_mpi *) Gy );
    mbedtls_reset_op_flag( (mbedtls_mpi *) Gz );
    mbedtls_reset_op_flag( (mbedtls_mpi *) A );
    mbedtls_reset_op_flag( (mbedtls_mpi *) d );
    mbedtls_reset_op_flag( (mbedtls_mpi *) P );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

int mbedtls_mpi_add_ecp( mbedtls_mpi *Qx,
                         mbedtls_mpi *Qy,
                         mbedtls_mpi *Qz,
                         const mbedtls_mpi *Gx1,
                         const mbedtls_mpi *Gy1,
                         const mbedtls_mpi *Gz1,
                         const mbedtls_mpi *Gx2,
                         const mbedtls_mpi *Gy2,
                         const mbedtls_mpi *Gz2,
                         const mbedtls_mpi *P )
{
    int i;
    int ret;
    unsigned int sizeNbits;
    unsigned char regX1, regY1, regZ1, regX2, regY2, regZ2, regX, regY, regZ,
        hwT0, regN, regNp, T[MUL_ECP_MAX_TMP_REG_NUM], len_id;

    HW_OPS_MUTEX_LOCK;
    hwT0  = INVALID_REG_INDEX;
    regNp = INVALID_REG_INDEX;
    memset( T, INVALID_REG_INDEX, sizeof( T ) );
    MBEDTLS_MPI_CHK( mbedtls_get_len_id(
        &len_id, NULL, NULL, (mbedtls_mpi *) P, ACA_OPCODE_ID_MODMUL ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regN, (mbedtls_mpi *) P, len_id, DBH_TRUE, DBH_FALSE ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regX1, (mbedtls_mpi *) Gx1, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regY1, (mbedtls_mpi *) Gy1, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regZ1, (mbedtls_mpi *) Gz1, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regX2, (mbedtls_mpi *) Gx2, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regY2, (mbedtls_mpi *) Gy2, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regZ2, (mbedtls_mpi *) Gz2, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regX, (mbedtls_mpi *) Qx, len_id, DBH_TRUE, DBH_TRUE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regY, (mbedtls_mpi *) Qy, len_id, DBH_TRUE, DBH_TRUE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regZ, (mbedtls_mpi *) Qz, len_id, DBH_TRUE, DBH_TRUE ) );
    for ( i = 0; i < MUL_ECP_MAX_TMP_REG_NUM; i++ )
    {
        MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, &T[i] ) );
    }
    sizeNbits = _mbedtls_mpi_bitlen( P );
    MBEDTLS_MPI_CHK( mbedtls_calc_np( len_id, regN, sizeNbits, &regNp ) );
    MBEDTLS_MPI_CHK(
        mbedtls_alloc_sram_for_dedicate_reg( len_id, &hwT0, NULL, NULL ) );
    dubhe_aca_set_n_np_t0_t1_reg( regN, regNp, hwT0, 0 );

    dubhe_aca_point_add( len_id,
                         T,
                         regN,
                         regX,
                         regY,
                         regZ,
                         regX1,
                         regY1,
                         regZ1,
                         regX2,
                         regY2,
                         regZ2 );
    if ( _mbedtls_mpi_cmp_int( Qz, 0 ) == 0 )
    {
        ret = MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
    }

cleanup:
    mbedtls_free_sram_for_dedicate_reg( &hwT0, NULL, &regNp );
    for ( i = 0; i < MUL_ECP_MAX_TMP_REG_NUM; i++ )
    {
        if ( T[i] != INVALID_REG_INDEX )
            mbedtls_free_tmp_reg_and_sram( T[i] );
    }
    mbedtls_reset_op_flag( Qx );
    mbedtls_reset_op_flag( Qy );
    mbedtls_reset_op_flag( Qz );
    mbedtls_reset_op_flag( (mbedtls_mpi *) Gx1 );
    mbedtls_reset_op_flag( (mbedtls_mpi *) Gy1 );
    mbedtls_reset_op_flag( (mbedtls_mpi *) Gz1 );
    mbedtls_reset_op_flag( (mbedtls_mpi *) Gx2 );
    mbedtls_reset_op_flag( (mbedtls_mpi *) Gy2 );
    mbedtls_reset_op_flag( (mbedtls_mpi *) Gz2 );
    mbedtls_reset_op_flag( (mbedtls_mpi *) P );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

int mbedtls_mpi_jac2affine( mbedtls_mpi *Tx,
                            mbedtls_mpi *Ty,
                            mbedtls_mpi *Tz,
                            const mbedtls_mpi *P )
{
    int i;
    int ret;
    unsigned int sizeNbits;
    unsigned char regX, regY, regZ, hwT0, hwT1, regN, regNp,
        T[JAC_2_AFFINE_MAX_TMP_REG_NUM], len_id;

    HW_OPS_MUTEX_LOCK;
    if ( _mbedtls_mpi_cmp_int( Tz, 0 ) == 0 )
    {
        HW_OPS_MUTEX_UNLOCK;
        return MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
    }

    memset( T, INVALID_REG_INDEX, sizeof( T ) );
    hwT0  = INVALID_REG_INDEX;
    hwT1  = INVALID_REG_INDEX;
    regNp = INVALID_REG_INDEX;
    MBEDTLS_MPI_CHK( mbedtls_get_len_id(
        &len_id, NULL, NULL, (mbedtls_mpi *) P, ACA_OPCODE_ID_MODINV ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regN, (mbedtls_mpi *) P, len_id, DBH_TRUE, DBH_FALSE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regX, (mbedtls_mpi *) Tx, len_id, DBH_TRUE, DBH_TRUE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regY, (mbedtls_mpi *) Ty, len_id, DBH_TRUE, DBH_TRUE ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_to_reg(
        &regZ, (mbedtls_mpi *) Tz, len_id, DBH_TRUE, DBH_TRUE ) );
    for ( i = 0; i < JAC_2_AFFINE_MAX_TMP_REG_NUM; i++ )
    {
        MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, &T[i] ) );
    }
    sizeNbits = _mbedtls_mpi_bitlen( P );
    MBEDTLS_MPI_CHK( mbedtls_calc_np( len_id, regN, sizeNbits, &regNp ) );
    MBEDTLS_MPI_CHK(
        mbedtls_alloc_sram_for_dedicate_reg( len_id, &hwT0, &hwT1, NULL ) );
    dubhe_aca_set_n_np_t0_t1_reg( regN, regNp, hwT0, hwT1 );
    dubhe_aca_jac2affine( len_id, T, regX, regY, regZ );
cleanup:
    mbedtls_free_sram_for_dedicate_reg( &hwT0, &hwT1, &regNp );
    for ( i = 0; i < JAC_2_AFFINE_MAX_TMP_REG_NUM; i++ )
    {
        if ( T[i] != INVALID_REG_INDEX )
            mbedtls_free_tmp_reg_and_sram( T[i] );
    }
    mbedtls_reset_op_flag( Tx );
    mbedtls_reset_op_flag( Ty );
    mbedtls_reset_op_flag( Tz );
    mbedtls_reset_op_flag( (mbedtls_mpi *) P );
#ifdef AYSMC_DEBUG
    if ( ret != 0 )
    {
        mbedtls_printf( "%s failed !!!\n", __func__ );
    }
#endif
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

#if defined( MBEDTLS_GENPRIME )

/*static const int small_prime[] = {
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
    911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997, -103};*/
#define GEN_PRIME_DEFAULT_VALUE 0x80000000
/*
 * Small divisors test (X must be positive)
 *
 * Return values:
 * 0: no small factor (possible prime, more tests needed)
 * 1: certain prime
 * MBEDTLS_ERR_MPI_NOT_ACCEPTABLE: certain non-prime
 * other negative: error
 */
static int mpi_check_small_factors( const mbedtls_mpi *X )
{
    int ret, i;
    unsigned char hwT0, hwT1, T[PRIME_JUDGE_MAX_TMP_REG_NUM], len_id;

    if ( mbedtls_mpi_cmp_int( X, 0 ) <= 0 )
        return MBEDTLS_ERR_MPI_NOT_ACCEPTABLE;

    HW_OPS_MUTEX_LOCK;
    hwT0  = INVALID_REG_INDEX;
    hwT1  = INVALID_REG_INDEX;
    memset( T, INVALID_REG_INDEX, sizeof( T ) );
    if ( ( ret = mbedtls_get_len_id(
               &len_id, NULL, NULL, (mbedtls_mpi *) X, ACA_OPCODE_ID_DIV ) )
         != 0 )
    {
        HW_OPS_MUTEX_UNLOCK;
        return ret;
    }

    if ( ( ret = mbedtls_mpi_to_reg(
               &T[PRIME_JUDGE_MAX_TMP_REG_NUM - 1], (mbedtls_mpi *) X, len_id, DBH_TRUE, DBH_FALSE ) )
         != 0 )
    {
        HW_OPS_MUTEX_UNLOCK;
        return ret;
    }
    MBEDTLS_MPI_CHK(
        mbedtls_alloc_sram_for_dedicate_reg( len_id, &hwT0, &hwT1, NULL ) );
    dubhe_aca_set_n_np_t0_t1_reg( 0, 0, hwT0, hwT1 );
    for ( i = 0; i < PRIME_JUDGE_MAX_TMP_REG_NUM - 1; i++ )
    {
        MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, &T[i] ) );
    }
    MBEDTLS_MPI_CHK(
        dubhe_aca_check_small_factors( len_id, T[PRIME_JUDGE_MAX_TMP_REG_NUM - 1], T ) );
cleanup:
    if ( ret == DBH_ACA_BIGNUM_NOT_ACCEPTABLE )
    {
        ret = MBEDTLS_ERR_MPI_NOT_ACCEPTABLE;
    }
    mbedtls_free_sram_for_dedicate_reg( &hwT0, &hwT1, NULL );
    for ( i = 0; i < PRIME_JUDGE_MAX_TMP_REG_NUM - 1; i++ )
    {
        if ( T[i] != INVALID_REG_INDEX )
            mbedtls_free_tmp_reg_and_sram( T[i] );
    }
    mbedtls_reset_op_flag( (mbedtls_mpi *) X );
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Miller-Rabin pseudo-primality test  (HAC 4.24)
 */
static int mpi_miller_rabin( const mbedtls_mpi *X,
                             int ( *f_rng )( void *, unsigned char *, size_t ),
                             void *p_rng )
{
    int ret, i;
    unsigned int sizeNbits;
    unsigned char hwT0, hwT1, regNp, regN, T[PRIME_JUDGE_MAX_TMP_REG_NUM],
    len_id;

    if ( mbedtls_mpi_cmp_int( X, 0 ) <= 0 )
        return MBEDTLS_ERR_MPI_NOT_ACCEPTABLE;

    HW_OPS_MUTEX_LOCK;
    hwT0  = INVALID_REG_INDEX;
    hwT1  = INVALID_REG_INDEX;
    regNp = INVALID_REG_INDEX;
    memset( T, INVALID_REG_INDEX, sizeof( T ) );
    sizeNbits = (unsigned int) _mbedtls_mpi_bitlen( X );
    if ( ( ret = mbedtls_get_len_id(
               &len_id, NULL, NULL, (mbedtls_mpi *) X, ACA_OPCODE_ID_MODEXP ) )
         != 0 )
    {
        HW_OPS_MUTEX_UNLOCK;
        return ret;
    }

    if ( ( ret = mbedtls_mpi_to_reg(
               &regN, (mbedtls_mpi *) X, len_id, DBH_TRUE, DBH_FALSE ) )
         != 0 )
    {
        HW_OPS_MUTEX_UNLOCK;
        return ret;
    }
    MBEDTLS_MPI_CHK( mbedtls_calc_np( len_id, regN, sizeNbits, &regNp ) );
    MBEDTLS_MPI_CHK(
        mbedtls_alloc_sram_for_dedicate_reg( len_id, &hwT0, &hwT1, NULL ) );
    dubhe_aca_set_n_np_t0_t1_reg( regN, regNp, hwT0, hwT1 );
    for ( i = 0; i < PRIME_JUDGE_MAX_TMP_REG_NUM; i++ )
    {
        MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, &T[i] ) );
    }
    MBEDTLS_MPI_CHK(
        dubhe_aca_miller_rabin( len_id, regN, T, f_rng, p_rng ) );
cleanup:
    if ( ret == DBH_ACA_BIGNUM_NOT_ACCEPTABLE )
    {
        ret = MBEDTLS_ERR_MPI_NOT_ACCEPTABLE;
    }
    mbedtls_free_sram_for_dedicate_reg( &hwT0, &hwT1, &regNp );
    for ( i = 0; i < PRIME_JUDGE_MAX_TMP_REG_NUM; i++ )
    {
        if ( T[i] != INVALID_REG_INDEX )
            mbedtls_free_tmp_reg_and_sram( T[i] );
    }
    mbedtls_reset_op_flag( (mbedtls_mpi *) X );
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Pseudo-primality test: small factors, then Miller-Rabin
 */
int mbedtls_mpi_is_prime( const mbedtls_mpi *X,
                          int ( *f_rng )( void *, unsigned char *, size_t ),
                          void *p_rng )
{
    int ret, i;
    unsigned int sizeNbits;
    unsigned char hwT0, hwT1, regNp, regN, T[PRIME_JUDGE_MAX_TMP_REG_NUM],
        len_id;

    if ( mbedtls_mpi_cmp_int( X, 0 ) <= 0 )
        return MBEDTLS_ERR_MPI_NOT_ACCEPTABLE;

    HW_OPS_MUTEX_LOCK;
    hwT0  = INVALID_REG_INDEX;
    hwT1  = INVALID_REG_INDEX;
    regNp = INVALID_REG_INDEX;
    memset( T, INVALID_REG_INDEX, sizeof( T ) );
    sizeNbits = (unsigned int) _mbedtls_mpi_bitlen( X );
    if ( ( ret = mbedtls_get_len_id(
               &len_id, NULL, NULL, (mbedtls_mpi *) X, ACA_OPCODE_ID_MODEXP ) )
         != 0 )
    {
        HW_OPS_MUTEX_UNLOCK;
        return ret;
    }

    if ( ( ret = mbedtls_mpi_to_reg(
               &regN, (mbedtls_mpi *) X, len_id, DBH_TRUE, DBH_FALSE ) )
         != 0 )
    {
        HW_OPS_MUTEX_UNLOCK;
        return ret;
    }
    MBEDTLS_MPI_CHK( mbedtls_calc_np( len_id, regN, sizeNbits, &regNp ) );
    MBEDTLS_MPI_CHK(
        mbedtls_alloc_sram_for_dedicate_reg( len_id, &hwT0, &hwT1, NULL ) );
    dubhe_aca_set_n_np_t0_t1_reg( regN, regNp, hwT0, hwT1 );
    for ( i = 0; i < PRIME_JUDGE_MAX_TMP_REG_NUM; i++ )
    {
        MBEDTLS_MPI_CHK( mbedtls_alloc_tmp_reg_and_sram( len_id, &T[i] ) );
    }
    MBEDTLS_MPI_CHK(
        dubhe_aca_is_prime( len_id, regN, sizeNbits, T, f_rng, p_rng ) );
cleanup:
    if ( ret == DBH_ACA_BIGNUM_NOT_ACCEPTABLE )
    {
        ret = MBEDTLS_ERR_MPI_NOT_ACCEPTABLE;
    }
    mbedtls_free_sram_for_dedicate_reg( &hwT0, &hwT1, &regNp );
    for ( i = 0; i < PRIME_JUDGE_MAX_TMP_REG_NUM; i++ )
    {
        if ( T[i] != INVALID_REG_INDEX )
            mbedtls_free_tmp_reg_and_sram( T[i] );
    }
    mbedtls_reset_op_flag( (mbedtls_mpi *) X );
    HW_OPS_MUTEX_UNLOCK;
    return ret;
}

/*
 * Prime number generation
 *
 * If dh_flag is 0 and nbits is at least 1024, then the procedure
 * follows the RSA probably-prime generation method of FIPS 186-4.
 * NB. FIPS 186-4 only allows the specific bit lengths of 1024 and 1536.
 */
int mbedtls_mpi_gen_prime( mbedtls_mpi *X,
                           size_t nbits,
                           int dh_flag,
                           int ( *f_rng )( void *, unsigned char *, size_t ),
                           void *p_rng )
{
#ifdef MBEDTLS_HAVE_INT64
// ceil(2^63.5)
#define CEIL_MAXUINT_DIV_SQRT2 0xb504f333f9de6485ULL
#else
// ceil(2^31.5)
#define CEIL_MAXUINT_DIV_SQRT2 0xb504f334U
#endif
    int ret = MBEDTLS_ERR_MPI_NOT_ACCEPTABLE;
    size_t k, n;
    mbedtls_mpi_uint r;
    mbedtls_mpi Y;

    if( nbits < 3 || nbits > MBEDTLS_MPI_MAX_BITS )
        return( MBEDTLS_ERR_MPI_BAD_INPUT_DATA );

    mbedtls_mpi_init( &Y );

    n = BITS_TO_LIMBS( nbits );

    while( 1 )
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_fill_random( X, n * ciL, f_rng, p_rng ) );
        /* make sure generated number is at least (nbits-1)+0.5 bits (FIPS 186-4
         * <A1><EC>B.3.3 steps 4.4, 5.5) */

        if( X->p[n-1] < CEIL_MAXUINT_DIV_SQRT2 ) continue;

        k = n * biL;
        if( k > nbits ) MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( X, k - nbits ) );
        //X->p[0] |= 1;
        MBEDTLS_MPI_CHK( mbedtls_mpi_set_bit( X, 0, 1 ) );

        if( dh_flag == 0 )
        {
            ret = mbedtls_mpi_is_prime( X, f_rng, p_rng );

            if( ret != MBEDTLS_ERR_MPI_NOT_ACCEPTABLE )
                goto cleanup;
        }
        else
        {
            /*
             * An necessary condition for Y and X = 2Y + 1 to be prime
             * is X = 2 mod 3 (which is equivalent to Y = 2 mod 3).
             * Make sure it is satisfied, while keeping X = 3 mod 4
             */

            //X->p[0] |= 2;
            MBEDTLS_MPI_CHK( mbedtls_mpi_set_bit( X, 1, 1 ) );

            MBEDTLS_MPI_CHK( mbedtls_mpi_mod_int( &r, X, 3 ) );
            if( r == 0 )
                MBEDTLS_MPI_CHK( mbedtls_mpi_add_int( X, X, 8 ) );
            else if( r == 1 )
                MBEDTLS_MPI_CHK( mbedtls_mpi_add_int( X, X, 4 ) );

            /* Set Y = (X-1) / 2, which is X / 2 because X is odd */
            MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &Y, X ) );
            MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( &Y, 1 ) );

            while( 1 )
            {
                /*
                 * First, check small factors for X and Y
                 * before doing Miller-Rabin on any of them
                 */
                if( ( ret = mpi_check_small_factors(  X         ) ) == 0 &&
                    ( ret = mpi_check_small_factors( &Y         ) ) == 0 &&
                    ( ret = mpi_miller_rabin(  X, f_rng, p_rng  ) ) == 0 &&
                    ( ret = mpi_miller_rabin( &Y, f_rng, p_rng  ) ) == 0 )
                    goto cleanup;

                if( ret != MBEDTLS_ERR_MPI_NOT_ACCEPTABLE )
                    goto cleanup;

                /*
                 * Next candidates. We want to preserve Y = (X-1) / 2 and
                 * Y = 1 mod 2 and Y = 2 mod 3 (eq X = 3 mod 4 and X = 2 mod 3)
                 * so up Y by 6 and X by 12.
                 */
                MBEDTLS_MPI_CHK( mbedtls_mpi_add_int(  X,  X, 12 ) );
                MBEDTLS_MPI_CHK( mbedtls_mpi_add_int( &Y, &Y, 6  ) );
            }
        }
    }

cleanup:

    mbedtls_mpi_free( &Y );

    return( ret );
}

#endif /* MBEDTLS_GENPRIME */

#if defined( MBEDTLS_SELF_TEST )

#define GCD_PAIR_COUNT 3

static const int gcd_pairs[GCD_PAIR_COUNT][3] = {
    {693, 609, 21}, {1764, 868, 28}, {768454923, 542167814, 1}};

/*
 * Checkup routine
 */
int mbedtls_mpi_self_test( int verbose )
{
    int ret, i;
    mbedtls_mpi A, E, N, X, Y, U, V;

    mbedtls_mpi_init( &A );
    mbedtls_mpi_init( &E );
    mbedtls_mpi_init( &N );
    mbedtls_mpi_init( &X );
    mbedtls_mpi_init( &Y );
    mbedtls_mpi_init( &U );
    mbedtls_mpi_init( &V );

    MBEDTLS_MPI_CHK(
        mbedtls_mpi_read_string( &A,
                                 16,
                                 "EFE021C2645FD1DC586E69184AF4A31E"
                                 "D5F53E93B5F123FA41680867BA110131"
                                 "944FE7952E2517337780CB0DB80E61AA"
                                 "E7C8DDC6C5C6AADEB34EB38A2F40D5E6" ) );

    MBEDTLS_MPI_CHK(
        mbedtls_mpi_read_string( &E,
                                 16,
                                 "B2E7EFD37075B9F03FF989C7C5051C20"
                                 "34D2A323810251127E7BF8625A4F49A5"
                                 "F3E27F4DA8BD59C47D6DAABA4C8127BD"
                                 "5B5C25763222FEFCCFC38B832366C29E" ) );

    MBEDTLS_MPI_CHK(
        mbedtls_mpi_read_string( &N,
                                 16,
                                 "0066A198186C18C10B2F5ED9B522752A"
                                 "9830B69916E535C8F047518A889A43A5"
                                 "94B6BED27A168D31D4A52F88925AA8F5" ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &X, &A, &N ) );

    MBEDTLS_MPI_CHK(
        mbedtls_mpi_read_string( &U,
                                 16,
                                 "602AB7ECA597A3D6B56FF9829A5E8B85"
                                 "9E857EA95A03512E2BAE7391688D264A"
                                 "A5663B0341DB9CCFD2C4C5F421FEC814"
                                 "8001B72E848A38CAE1C65F78E56ABDEF"
                                 "E12D3C039B8A02D6BE593F0BBBDA56F1"
                                 "ECF677152EF804370C1A305CAF3B5BF1"
                                 "30879B56C61DE584A0F53A2447A51E" ) );

    if ( verbose != 0 )
    {
        mbedtls_printf( "  MPI test #1 (mul_mpi): " );
    }

    if ( mbedtls_mpi_cmp_mpi( &X, &U ) != 0 )
    {
        if ( verbose != 0 )
        {
            mbedtls_printf( "failed\n" );
        }

        ret = 1;
        goto cleanup;
    }

    if ( verbose != 0 )
    {
        mbedtls_printf( "passed\n" );
    }

    MBEDTLS_MPI_CHK( mbedtls_mpi_div_mpi( &X, &Y, &A, &N ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_read_string(
        &U, 16, "256567336059E52CAE22925474705F39A94" ) );

    MBEDTLS_MPI_CHK(
        mbedtls_mpi_read_string( &V,
                                 16,
                                 "6613F26162223DF488E9CD48CC132C7A"
                                 "0AC93C701B001B092E4E5B9F73BCD27B"
                                 "9EE50D0657C77F374E903CDFA4C642" ) );

    if ( verbose != 0 )
    {
        mbedtls_printf( "  MPI test #2 (div_mpi): " );
    }

    if ( mbedtls_mpi_cmp_mpi( &X, &U ) != 0
         || mbedtls_mpi_cmp_mpi( &Y, &V ) != 0 )
    {
        if ( verbose != 0 )
        {
            mbedtls_printf( "failed\n" );
        }

        ret = 1;
        goto cleanup;
    }

    if ( verbose != 0 )
    {
        mbedtls_printf( "passed\n" );
    }

    MBEDTLS_MPI_CHK( mbedtls_mpi_exp_mod( &X, &A, &E, &N, NULL ) );

    MBEDTLS_MPI_CHK(
        mbedtls_mpi_read_string( &U,
                                 16,
                                 "36E139AEA55215609D2816998ED020BB"
                                 "BD96C37890F65171D948E9BC7CBAA4D9"
                                 "325D24D6A3C12710F10A09FA08AB87" ) );

    if ( verbose != 0 )
    {
        mbedtls_printf( "  MPI test #3 (exp_mod): " );
    }

    if ( mbedtls_mpi_cmp_mpi( &X, &U ) != 0 )
    {
        if ( verbose != 0 )
        {
            mbedtls_printf( "failed\n" );
        }

        ret = 1;
        goto cleanup;
    }

    if ( verbose != 0 )
    {
        mbedtls_printf( "passed\n" );
    }

    MBEDTLS_MPI_CHK( mbedtls_mpi_inv_mod( &X, &A, &N ) );

    MBEDTLS_MPI_CHK(
        mbedtls_mpi_read_string( &U,
                                 16,
                                 "003A0AAEDD7E784FC07D8F9EC6E3BFD5"
                                 "C3DBA76456363A10869622EAC2DD84EC"
                                 "C5B8A74DAC4D09E03B5E0BE779F2DF61" ) );

    if ( verbose != 0 )
    {
        mbedtls_printf( "  MPI test #4 (inv_mod): " );
    }

    if ( mbedtls_mpi_cmp_mpi( &X, &U ) != 0 )
    {
        if ( verbose != 0 )
        {
            mbedtls_printf( "failed\n" );
        }

        ret = 1;
        goto cleanup;
    }

    if ( verbose != 0 )
    {
        mbedtls_printf( "passed\n" );
    }

    if ( verbose != 0 )
    {
        mbedtls_printf( "  MPI test #5 (simple gcd): " );
    }

    for ( i = 0; i < GCD_PAIR_COUNT; i++ )
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &X, gcd_pairs[i][0] ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &Y, gcd_pairs[i][1] ) );

        MBEDTLS_MPI_CHK( mbedtls_mpi_gcd( &A, &X, &Y ) );

        if ( mbedtls_mpi_cmp_int( &A, gcd_pairs[i][2] ) != 0 )
        {
            if ( verbose != 0 )
            {
                mbedtls_printf( "failed at %d\n", i );
            }

            ret = 1;
            goto cleanup;
        }
    }

    if ( verbose != 0 )
    {
        mbedtls_printf( "passed\n" );
    }

cleanup:

    if ( ret != 0 && verbose != 0 )
    {
        mbedtls_printf( "Unexpected error, return code = %08X\n", ret );
    }

    mbedtls_mpi_free( &A );
    mbedtls_mpi_free( &E );
    mbedtls_mpi_free( &N );
    mbedtls_mpi_free( &X );
    mbedtls_mpi_free( &Y );
    mbedtls_mpi_free( &U );
    mbedtls_mpi_free( &V );

    if ( verbose != 0 )
    {
        mbedtls_printf( "\n" );
    }

    return ret;
}

#endif /* MBEDTLS_SELF_TEST */

#endif /* MBEDTLS_BIGNUM_C */
/*************************** The End Of File*****************************/
