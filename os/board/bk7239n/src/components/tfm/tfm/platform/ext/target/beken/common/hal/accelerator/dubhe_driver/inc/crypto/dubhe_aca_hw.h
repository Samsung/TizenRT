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

#ifndef __ACA_HW_RESOURCES_H__
#define __ACA_HW_RESOURCES_H__

//TODO peter, is 28060000 correct?
#define ACA_REG_SRAM_BASE_ADDR 0x28060000
#include <stdint.h>
#include <stddef.h>

#define DBH_ACA_WORD_SIZE_IN_BITS 64
#define DBG_GR_LEN_MAX_SIZE_IN_BITS 4096

/* ACA operations maximal count of extra bits: */
#define ACA_EXTRA_BITS 8

#define DBH_ACA_NP_SIZE_IN_BITS 128
#define DBH_BITS_IN_BYTE 8
#define DBH_BITS_IN_32BIT_WORD 32

#define CALC_FULL_32BIT_WORDS( numBits )                                       \
    ( ( numBits ) / DBH_BITS_IN_32BIT_WORD                                     \
      + ( ( ( numBits ) & ( DBH_BITS_IN_32BIT_WORD - 1 ) ) > 0 ) )

#define MAX_IM_DATA_BIT_SIZE 31
#define MAX_SHIFT_DATA_BIT_SIZE 64

#define ACA_REG_T0 0
#define ACA_REG_T1 1
#define ACA_REG_N 2
#define ACA_REG_NP 3
#define ACA_REG_T30 30
#define ACA_REG_T31 31

/* ACA HW defined OPCODE values, according to HW documentation  */
#define ACA_OPCODE_ID_ADD 0x1U          // @0x1 - Add
#define ACA_OPCODE_ID_SUB 0x2U          // @0x2 - Sub
#define ACA_OPCODE_ID_MUL 0x3U          // @0x3 - Multiple
#define ACA_OPCODE_ID_DIV 0x4U          // @0x4 - Division
#define ACA_OPCODE_ID_AND 0x5U          // @0x5 - AND
#define ACA_OPCODE_ID_OR 0x6U           // @0x6 - OR
#define ACA_OPCODE_ID_XOR 0x7U          // @0x7 - XOR
#define ACA_OPCODE_ID_SHR0 0x8U         // @0x8 - SHR0
#define ACA_OPCODE_ID_SHR1 0x9U         // @0x9 - SHR1
#define ACA_OPCODE_ID_SHL0 0xAU         // @0xa - SHL0
#define ACA_OPCODE_ID_SHL1 0xBU         // @0xb - SHL1
#define ACA_OPCODE_ID_MULHIGH 0xCU      // @0xc - MULHigh
#define ACA_OPCODE_ID_RED 0x10U         // 0x10 - Reduction
#define ACA_OPCODE_ID_MODADD 0x11U      // @0x11 - ModAdd
#define ACA_OPCODE_ID_MODSUB 0x12U      // @0x12 - ModSub
#define ACA_OPCODE_ID_MODMUL 0x13U      // @0x13 - ModMul
#define ACA_OPCODE_ID_MODINV 0x14U      // @0x14 - ModInv
#define ACA_OPCODE_ID_MODEXP 0x15U      // @0x15 - ModExp
#define ACA_OPCODE_ID_MODMULNR 0x16U    // @0x16 - ModMulNFR
#define ACA_OPCODE_ID_MODMLACC 0x17U    // @0x17 - ModMulAcc
#define ACA_OPCODE_ID_MODMLACCNR 0x18U  // @0x18 - ModMulAccNFR

#define ACA_VIRTUAL_OPCODE_LONG_DIV 0x19U

unsigned int aca_get_sram_size( unsigned char reg_index );
unsigned int aca_get_full_len( unsigned char len_id );
int aca_get_full_len_by_opcode( unsigned char op_code,
                                unsigned int *op_len_bits,
                                unsigned int *full_len );
int aca_get_len_id( unsigned char *len_id,
                    unsigned int op_len_bits,
                    unsigned int full_len );
int aca_set_len_id( unsigned char len_id,
                    unsigned int op_len_bits,
                    unsigned int full_len );

void aca_reset_len_id( void );
unsigned int aca_get_free_tmp_reg_num( void );
int aca_reg_index_alloc( unsigned int *reg_index );
void aca_reg_index_free( unsigned int reg_index );
int aca_reg_sram_alloc_from_reg( unsigned char reg, unsigned char len_id );
void aca_reg_sram_free_from_reg( unsigned char reg );
void aca_hw_resources_cleanup( void );
void aca_hw_resources_init( void );
#endif

/*************************** The End Of File*****************************/
