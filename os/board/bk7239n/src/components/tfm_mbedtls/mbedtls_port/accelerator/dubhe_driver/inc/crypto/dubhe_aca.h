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

#ifndef __DUBHE_ACA_H__
#define __DUBHE_ACA_H__

#include <stdint.h>
#include <stddef.h>
#include "dubhe_driver_internal.h"

#define DBH_ACA_BAD_PARAM_ERR -0x15000
#define DBH_ACA_GENERIC_ERR -0x15001
#define DBH_ACA_REG_SRAM_ALLOC_FAIL_ERR -0x15002
#define DBH_ACA_REG_INDEX_ALLOC_FAIL_ERR -0x15003
#define DBH_ACA_BIGNUM_NOT_ACCEPTABLE -0x15004
#define DBH_ACA_SYS_SRAM_ALLOC_FAIL_ERR -0x15005

#define MAX_OP_BITS_HALF 4000

void dubhe_aca_driver_init( void );
void dubhe_aca_driver_cleanup( void );
void dubhe_aca_set_n_np_t0_t1_reg( unsigned char N,
                                   unsigned char NP,
                                   unsigned char T0,
                                   unsigned char T1 );

int dubhe_aca_shift_l( unsigned char len_id, unsigned char T1, size_t count );

int dubhe_aca_shift_r( unsigned char len_id, unsigned char T1, size_t count );

int dubhe_aca_cmp( unsigned char len_id,
                   unsigned char T1,
                   unsigned char T2,
                   unsigned char regTmp );

int dubhe_aca_add( unsigned char len_id,
                   unsigned char R,
                   unsigned char T1,
                   unsigned char T2 );

/*R=T1&T2*/
int dubhe_aca_and( unsigned char len_id,
                   unsigned char R,
                   unsigned char T1,
                   unsigned char T2 );

unsigned int dubhe_aca_sub( unsigned char len_id,
                            unsigned char R,
                            unsigned char T1,
                            unsigned char T2 );

int dubhe_aca_add_im( unsigned char len_id,
                      unsigned char R,
                      unsigned char T1,
                      unsigned char a );
unsigned int dubhe_aca_sub_im( unsigned char len_id,
                               unsigned char R,
                               unsigned char T1,
                               unsigned char a );
int dubhe_aca_mul( unsigned char len_id,
                   unsigned char R1,
                   unsigned char R2,
                   unsigned char T1,
                   unsigned char T2 );

int dubhe_aca_div( unsigned char len_id,
                   unsigned char TQ,
                   unsigned char TR,
                   unsigned char T1,
                   unsigned char T2,
                   unsigned char regTmp1,
                   unsigned char regTmp2 );

unsigned int dubhe_aca_mod_reduce( unsigned char len_id,
                                   unsigned char TR,
                                   unsigned char T1,
                                   unsigned char N );

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
                        unsigned int sizeNbits );

int dubhe_aca_mod_exp( unsigned char len_id,
                       unsigned char TR,
                       unsigned char T1,
                       unsigned char TE );

int dubhe_aca_mod_inv( unsigned char len_id,
                       unsigned char TR,
                       unsigned char T1,
                       unsigned char regTmp );

int dubhe_aca_mod_mul( unsigned char len_id,
                       unsigned char TR,
                       unsigned char T1,
                       unsigned char T2,
                       unsigned char regTmp1,
                       unsigned char regTmp2 );
int dubhe_aca_mod_add( unsigned char len_id,
                       unsigned char R,
                       unsigned char T1,
                       unsigned char T2 );
int dubhe_aca_mod_sub( unsigned char len_id,
                       unsigned char R,
                       unsigned char T1,
                       unsigned char T2 );
int dubhe_aca_mod_inv_even( unsigned char len_id,
                            unsigned char TR,
                            unsigned char TA,
                            unsigned char TN,
                            unsigned int sizeNbits,
                            unsigned char tmpT[] );

int dubhe_aca_copy( unsigned char len_id, unsigned char T1, unsigned char T2 );

void dubhe_aca_wait_on_aca_done( void );

void aca_copy_data_from_reg( uint32_t *dst_ptr,  /*!< [out] Pointer to
                                                  * destination buffer. */
                             uint32_t sizeWords, /*!< [in]  Source size in
                                                  * words. */
                             uint32_t srcReg );  /*!< [in]  Virtual address
                                                  * (number) of source PKA
                                                  * register. */

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
                             uint32_t sizeWords );    /*!< [in] Data size in
                                                       * words. */

void dubhe_aca_clear_reg_sram( uint32_t dstReg,
                               uint32_t offset,
                               uint32_t sizeWords );
void dubhe_aca_double_point( unsigned char len_id,
                             unsigned char T[],
                             const unsigned char x,
                             const unsigned char y,
                             const unsigned char z,
                             const unsigned char x1,
                             const unsigned char y1,
                             const unsigned char z1,
                             const unsigned char t1 );
void dubhe_aca_double_point_jj( unsigned char len_id,
                                unsigned char T[],
                                const unsigned char x,
                                const unsigned char y,
                                const unsigned char z,
                                const unsigned char w,
                                const unsigned char o_mj_en );

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
                          const unsigned char z2 );
void dubhe_aca_point_add_ajj( unsigned char len_id,
                              unsigned char T[],
                              const unsigned char x,
                              const unsigned char y,
                              const unsigned char z,
                              const unsigned char x1,
                              const unsigned char y1,
                              const unsigned char a,
                              const unsigned char w,
                              const unsigned char o_mj_en );

void dubhe_aca_jac2affine( unsigned char len_id,
                           unsigned char T[],
                           const unsigned char x,
                           const unsigned char y,
                           const unsigned char z );
int dubhe_aca_check_small_factors( unsigned char len_id,
                                          unsigned char X,
                                          unsigned char T[] );

int dubhe_aca_miller_rabin( unsigned char len_id,
                        unsigned char X,
                        unsigned char T[],
                        int ( *f_rng )( void *, unsigned char *, size_t ),
                        void *p_rng );

int dubhe_aca_is_prime( unsigned char len_id,
                        unsigned char X,
                        size_t xbits,
                        unsigned char T[],
                        int ( *f_rng )( void *, unsigned char *, size_t ),
                        void *p_rng );
int dubhe_aca_gen_dh_prime( unsigned char len_id,
                            unsigned char X,
                            size_t xbits,
                            unsigned char T[],
                            int ( *f_rng )( void *, unsigned char *, size_t ),
                            void *p_rng );

#endif

/*************************** The End Of File*****************************/
