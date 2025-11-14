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

#ifndef __ACA_HW_DEFS_H__
#define __ACA_HW_DEFS_H__
#include <stdint.h>

#include "dubhe_regs.h"
#include "dubhe_event.h"
#include "dubhe_aca_hw.h"
#include "hal_irq.h"

/*T0/T1 is used by HW, and N/NP/T30/T31 used for dedicate tempary use*/
#define ACA_TMP_REG_START 0
#define ACA_AVAIL_TMP_REG_NUM 32
#define ACA_TOTAL_TMP_REG_NUM 32

typedef enum _aca_engine_stat_t {
    ACA_ENGINE_IDLE           = 0,
    ACA_ENGINE_BUSY_FOR_OTHER = 1,
    ACA_ENGINE_BUSY           = 2,
} aca_engine_stat_t;

typedef enum _aca_sram_size_mode_t {
    ACA_SRAM_4K  = 0,
    ACA_SRAM_8K  = 1,
    ACA_SRAM_16K = 2,
} aca_sram_size_mode_t;

#define DBH_ACA_FIFO_EMPTY 1
#define DBH_ACA_FIFO_FULL 1

#define ACA_WORD_SIZE_IN_32BIT_WORDS                                           \
    ( DBH_ACA_WORD_SIZE_IN_BITS / DBH_BITS_IN_32BIT_WORD )

#define DBH_RSA_MAX_VALID_KEY_SIZE_VALUE_IN_BITS 4096

#define DBH_RSA_MAX_KEY_GENERATION_HW_SIZE_BITS 4096
#define DBH_ACA_MIN_SRAM_SIZE_IN_BYTES 4096
#define DBH_ACA_MIN_SRAM_SIZE_IN_32BIT_WORDS 1024

/* Full register (operation) size including extra ACA-word (64-bit). Op size
 * must include extra bits if needed in the algorithm:
 * there are two reasons for extra bits:
 * (1) some algorithm need the extra bits: at present, modINV needs extra 8bits
 * and modMUL needs extra 72bits; (2)  after (1) and then ceil the OP bits as
 * the multi-64 as the hardware use 64-radix; Adding 1 for operation with
 * carry/additional bits
 */
#define GET_FULL_OP_SIZE_ACA_WORDS( opSizeInBits )                             \
    ( ( ( opSizeInBits ) / DBH_ACA_WORD_SIZE_IN_BITS                           \
        + ( ( ( opSizeInBits ) & ( DBH_ACA_WORD_SIZE_IN_BITS - 1 ) ) > 0 ) ) )
#define GET_FULL_OP_SIZE_BITS( opSizeInBits )                                  \
    ( DBH_ACA_WORD_SIZE_IN_BITS                                                \
      * GET_FULL_OP_SIZE_ACA_WORDS( ( opSizeInBits ) ) )
#define GET_FULL_OP_SIZE_32BIT_WORDS( opSizeInBits )                           \
    ( GET_FULL_OP_SIZE_BITS( ( opSizeInBits ) ) / DBH_BITS_IN_32BIT_WORD )

typedef enum _aca_len_types_t {
    LEN_TYPE_0 = 0,  // 0
    LEN_TYPE_1,
    LEN_TYPE_2,
    LEN_TYPE_3,
    LEN_TYPE_4,
    LEN_TYPE_5,
    LEN_TYPE_6,
    LEN_TYPE_7,

    LEN_TYPE_MAX,  // 8  - not allowed, indicates out of
                   // range  (RSA, EC)
} aca_len_types_t;

#ifdef REG_DEBUG
extern unsigned long _g_Dubhe_RegBase;

#define ACA_GET_ACA_DONE_STATUS( status )                                      \
    do {                                                                       \
        volatile uint32_t output_reg_val;                                      \
        uint32_t engine_stat, queue_empty_flag;                                \
        output_reg_val = DBH_READ_REGISTER( ACA, ACA_STAT );                   \
        engine_stat = DBH_REG_FLD_GET( ACA_STAT, ENGIN_STAT, output_reg_val ); \
        queue_empty_flag =                                                     \
            DBH_REG_FLD_GET( ACA_STAT, FIFO_EMPTY, output_reg_val );           \
        PAL_LOG_DEBUG( "ACA_GET_ACA_DONE_STATUS  addr: 0x%08x reg:0x%08x "     \
                       "stat:0x%08x empty_flag:0x%08x \n",                     \
                       ( _g_Dubhe_RegBase + DBH_REG_OFFSET( ACA, ACA_STAT ) ), \
                       output_reg_val,                                         \
                       engine_stat,                                            \
                       queue_empty_flag );                                     \
        status = ( ( ( engine_stat & 0x02 ) != ACA_ENGINE_BUSY )               \
                   && ( ( queue_empty_flag & 0x01 ) == DBH_ACA_FIFO_EMPTY ) ); \
    } while ( 0 )

#define ACA_GET_ACA_FIFO_READY_STATUS( status )                                \
    do {                                                                       \
        volatile uint32_t output_reg_val;                                      \
        uint32_t queue_full_flag;                                              \
        output_reg_val = DBH_READ_REGISTER( ACA, ACA_STAT );                   \
        queue_full_flag =                                                      \
            DBH_REG_FLD_GET( ACA_STAT, FIFO_FULL, output_reg_val );            \
        PAL_LOG_DEBUG( "\n ACA_GET_ACA_FIFO_READY_STATUS reg_val:0x%08x "      \
                       "queue_full:0x%08x \n",                                 \
                       output_reg_val,                                         \
                       queue_full_flag );                                      \
        status = ( ( queue_full_flag & 0x01 ) != DBH_ACA_FIFO_FULL );          \
    } while ( 0 )
#else
#define ACA_GET_ACA_DONE_STATUS( status )                                      \
    do {                                                                       \
        volatile uint32_t output_reg_val;                                      \
        uint32_t engine_stat, queue_empty_flag;                                \
        output_reg_val = DBH_READ_REGISTER( ACA, ACA_STAT );                   \
        engine_stat = DBH_REG_FLD_GET( ACA_STAT, ENGIN_STAT, output_reg_val ); \
        queue_empty_flag =                                                     \
            DBH_REG_FLD_GET( ACA_STAT, FIFO_EMPTY, output_reg_val );           \
        status = ( ( ( engine_stat & 0x02 ) != ACA_ENGINE_BUSY )               \
                   && ( ( queue_empty_flag & 0x01 ) == DBH_ACA_FIFO_EMPTY ) ); \
    } while ( 0 )

#define ACA_GET_ACA_FIFO_READY_STATUS( status )                                \
    do {                                                                       \
        volatile uint32_t output_reg_val;                                      \
        uint32_t queue_full_flag;                                              \
        output_reg_val = DBH_READ_REGISTER( ACA, ACA_STAT );                   \
        queue_full_flag =                                                      \
            DBH_REG_FLD_GET( ACA_STAT, FIFO_FULL, output_reg_val );            \
        status = ( ( queue_full_flag & 0x01 ) != DBH_ACA_FIFO_FULL );          \
    } while ( 0 )
#endif

/* defining a macro for waiting to the ACA_OP_DONE */
#if defined( DUBHE_FOR_RUNTIME )
#define ACA_WAIT_ON_ACA_DONE( )                                                \
    do {                                                                       \
        uint32_t status;                                                       \
        ACA_GET_ACA_DONE_STATUS( status );                                     \
        if ( !status ) {                                                       \
            volatile uint32_t output_reg_val;                                  \
            unsigned int irq_stat;                                             \
            irq_stat = hal_irq_disable();                                      \
            output_reg_val = DBH_READ_REGISTER( ACA, ACA_INTR_MASK );          \
            DBH_REG_FLD_SET( ACA_INTR_MASK, ACA_DONE, output_reg_val, 0x0 );   \
            DBH_WRITE_REGISTER( ACA, ACA_INTR_MASK, output_reg_val );          \
            hal_irq_enable(irq_stat);                                          \
            (void) dubhe_wait_event( DBH_EVENT_ACA_DONE );                     \
        }                                                                      \
    } while ( 0 )

#define ACA_WAIT_ON_FIFO_READY( )                                              \
    do {                                                                       \
        uint32_t status;                                                       \
        ACA_GET_ACA_FIFO_READY_STATUS( status );                               \
        if ( !status ) {                                                       \
            volatile uint32_t output_reg_val;                                  \
            unsigned int irq_stat;                                             \
            irq_stat = hal_irq_disable();                                      \
            output_reg_val = DBH_READ_REGISTER( ACA, ACA_INTR_MASK );          \
            DBH_REG_FLD_SET(                                                   \
                ACA_INTR_MASK, FIFO_UNDER_WM, output_reg_val, 0x0 );           \
            DBH_WRITE_REGISTER( ACA, ACA_INTR_MASK, output_reg_val );          \
            hal_irq_enable(irq_stat);                                          \
            (void) dubhe_wait_event( DBH_EVENT_ACA_FIFO_READY );               \
        }                                                                      \
    } while ( 0 )
#else
#define ACA_WAIT_ON_ACA_DONE( )                                                \
    do {                                                                       \
        uint32_t status;                                                       \
        do {                                                                   \
            ACA_GET_ACA_DONE_STATUS( status );                                 \
        } while ( !status );                                                   \
    } while ( 0 )

#define ACA_WAIT_ON_FIFO_READY( )                                              \
    do {                                                                       \
        uint32_t status;                                                       \
        do {                                                                   \
            ACA_GET_ACA_FIFO_READY_STATUS( status );                           \
        } while ( !status );                                                   \
    } while ( 0 )
#endif

/* Returns the ALU ADD Zero-bit from ACA_STAT register */
#define ACA_GET_STATUS_ADD_RESULT_ZERO( status )                               \
    do {                                                                       \
        ACA_WAIT_ON_ACA_DONE( );                                               \
        status = DBH_READ_REGISTER( ACA, ACA_STAT );                           \
        status = DBH_REG_FLD_GET( ACA_STAT, ADD_RESULT_ZERO, status );         \
    } while ( 0 )

/* Returns the ALU AND Zero-bit from ACA_STAT register */
#define ACA_GET_STATUS_AND_RESULT_ZERO( status )                               \
    do {                                                                       \
        ACA_WAIT_ON_ACA_DONE( );                                               \
        status = DBH_READ_REGISTER( ACA, ACA_STAT );                           \
        status = DBH_REG_FLD_GET( ACA_STAT, AND_RESULT_ZERO, status );         \
    } while ( 0 )

/* Returns the ALU XOR Zero-bit from ACA_STAT register */
#define ACA_GET_STATUS_XOR_RESULT_ZERO( status )                               \
    do {                                                                       \
        ACA_WAIT_ON_ACA_DONE( );                                               \
        status = DBH_READ_REGISTER( ACA, ACA_STAT );                           \
        status = DBH_REG_FLD_GET( ACA_STAT, XOR_RESULT_ZERO, status );         \
    } while ( 0 )

/* Returns the ALU Carry-bit from ACA_STAT register */
#define ACA_GET_STATUS_CARRY( status )                                         \
    do {                                                                       \
        ACA_WAIT_ON_ACA_DONE( );                                               \
        status = DBH_READ_REGISTER( ACA, ACA_STAT );                           \
        status = DBH_REG_FLD_GET( ACA_STAT, ALU_CARRY, status );               \
    } while ( 0 )

/* Returns the SRAM SIZE MODE from ACA_STAT register */
#define ACA_GET_STATUS_SRAM_SIZE_MODE( status )                                \
    do {                                                                       \
        status = DBH_READ_REGISTER( ACA, ACA_STAT );                           \
        status = DBH_REG_FLD_GET( ACA_STAT, SRAM_SIZE_MODE, status );          \
    } while ( 0 )

/*  Sets the size into regsSizesTable entry */
#define ACA_SET_REG_SIZE( SizeBits, EntryNum )                                 \
    do {                                                                       \
        ACA_WAIT_ON_ACA_DONE( );                                               \
        DBH_WRITE_REGISTER2(                                                   \
            ( DBH_REG_OFFSET( ACA, LEN_TYPE0 ) + 4 * ( EntryNum ) ),           \
            ( SizeBits ) );                                                    \
    } while ( 0 )
/*  Gets the size from regsSizesTable entry */
#define ACA_GET_REG_SIZE( SizeBits, EntryNum )                                 \
    do {                                                                       \
        SizeBits = DBH_READ_REGISTER2( DBH_REG_OFFSET( ACA, LEN_TYPE0 )        \
                                       + 4 * ( EntryNum ) );                   \
    } while ( 0 )

/*The following macros are used for setting and reading the data registers
 * addresses in mapping table.*/

/*  Sets the physical address PhysAddr of register VirtReg in mapping table
 * */
#define ACA_SET_REG_ADDRESS( Reg, SramAddr )                                   \
    do {                                                                       \
        DBH_WRITE_REGISTER2(                                                   \
            ( DBH_REG_OFFSET( ACA, SRAM_MAP0 ) + 4 * ( Reg ) ),                \
            ( SramAddr ) );                                                    \
    } while ( 0 )

/*  Returns the physical address of register VirtReg from mapping table  */
#define ACA_GET_REG_ADDRESS( Reg, SramAddr )                                   \
    do {                                                                       \
        ( SramAddr ) = DBH_READ_REGISTER2( DBH_REG_OFFSET( ACA, SRAM_MAP0 )    \
                                           + 4 * ( Reg ) );                    \
    } while ( 0 )

#define ACA_GET_N_NP_T0_T1( N, NP, T0, T1 )                                    \
    do {                                                                       \
        volatile uint32_t reg_val = 0;                                         \
        reg_val                   = DBH_READ_REGISTER( ACA, N_NP_T0_T1_ADDR ); \
        N  = DBH_REG_FLD_GET( N_NP_T0_T1_ADDR, N_VIRTUAL_ADDR, reg_val );      \
        NP = DBH_REG_FLD_GET( N_NP_T0_T1_ADDR, NP_VIRTUAL_ADDR, reg_val );     \
        T0 = DBH_REG_FLD_GET( N_NP_T0_T1_ADDR, T0_VIRTUAL_ADDR, reg_val );     \
        T1 = DBH_REG_FLD_GET( N_NP_T0_T1_ADDR, T1_VIRTUAL_ADDR, reg_val );     \
    } while ( 0 )

#define ACA_SET_N_NP_T0_T1( N, NP, T0, T1 )                                    \
    do {                                                                       \
        uint32_t reg_val = 0;                                                  \
        DBH_REG_FLD_SET( N_NP_T0_T1_ADDR, N_VIRTUAL_ADDR, reg_val, N );        \
        DBH_REG_FLD_SET( N_NP_T0_T1_ADDR, NP_VIRTUAL_ADDR, reg_val, NP );      \
        DBH_REG_FLD_SET( N_NP_T0_T1_ADDR, T0_VIRTUAL_ADDR, reg_val, T0 );      \
        DBH_REG_FLD_SET( N_NP_T0_T1_ADDR, T1_VIRTUAL_ADDR, reg_val, T1 );      \
        DBH_WRITE_REGISTER( ACA, N_NP_T0_T1_ADDR, reg_val );                   \
    } while ( 0 )

#define ACA_CFG_OP_RUN( )                                                      \
    do {                                                                       \
        volatile uint32_t output_reg_val;                                      \
        output_reg_val = DBH_READ_REGISTER( ACA, OP_CFG );                     \
        DBH_REG_FLD_SET( OP_CFG, RUN, output_reg_val, 1 );                     \
        DBH_WRITE_REGISTER( ACA, OP_CFG, output_reg_val );                     \
    } while ( 0 )

#define ACA_SET_DEFAULT_OP_CFG( )                                              \
    do {                                                                       \
        volatile uint32_t output_reg_val;                                      \
        output_reg_val = DBH_READ_REGISTER( ACA, OP_CFG );                     \
        DBH_REG_FLD_SET( OP_CFG, RUN, output_reg_val, 1 );                     \
        DBH_REG_FLD_SET( OP_CFG, FIFO_WM, output_reg_val, 8 );                 \
        DBH_WRITE_REGISTER( ACA, OP_CFG, output_reg_val );                     \
    } while ( 0 )

#define ACA_UNMASK_DEFAULT_INTR( )                                             \
    do {                                                                       \
        volatile uint32_t output_reg_val;                                      \
        output_reg_val = DBH_READ_REGISTER( ACA, ACA_INTR_MASK );              \
        DBH_REG_FLD_SET( ACA_INTR_MASK, FIFO_OVERFLOW, output_reg_val, 0x0 );  \
        DBH_REG_FLD_SET( ACA_INTR_MASK, DIV_ZERO, output_reg_val, 0x0 );       \
        DBH_REG_FLD_SET( ACA_INTR_MASK, MODINV_ZERO, output_reg_val, 0x0 );    \
        DBH_WRITE_REGISTER( ACA, ACA_INTR_MASK, output_reg_val );              \
    } while ( 0 )

#define ACA_ASSEMBLY_OPCODE( opcode_val,                                       \
                             Opcode,                                           \
                             lenId,                                            \
                             red_eq,                                           \
                             OpA,                                              \
                             isBImmed,                                         \
                             OpB,                                              \
                             Res,                                              \
                             ResDiscard,                                       \
                             Tag,                                              \
                             NeedIrr )                                         \
    do {                                                                       \
        DBH_REG_FLD_SET( OPCODE_ENTRY, NEED_INTR, opcode_val, NeedIrr );       \
        DBH_REG_FLD_SET( OPCODE_ENTRY, REG_C, opcode_val, Tag );               \
        DBH_REG_FLD_SET( OPCODE_ENTRY, R_DISCARD, opcode_val, ResDiscard );    \
        DBH_REG_FLD_SET( OPCODE_ENTRY, REG_R, opcode_val, Res );               \
        DBH_REG_FLD_SET( OPCODE_ENTRY, REG_B, opcode_val, OpB );               \
        DBH_REG_FLD_SET( OPCODE_ENTRY, REG_B_IM, opcode_val, isBImmed );       \
        DBH_REG_FLD_SET( OPCODE_ENTRY, REG_A, opcode_val, OpA );               \
        DBH_REG_FLD_SET( OPCODE_ENTRY, RESERVE, opcode_val, red_eq );          \
        DBH_REG_FLD_SET( OPCODE_ENTRY, LEN, opcode_val, lenId );               \
        DBH_REG_FLD_SET( OPCODE_ENTRY, OPCODE, opcode_val, Opcode );           \
    } while ( 0 )

/* ******************************************************************* */
/* MACRO DEFINITIONS FOR WORKING WITH INDIRECT ACCESS TO ACA SRAM DATA */
/* ******************************************************************* */
/* defining a macro to clear the not-completed part of ACA big word*
 *  ii value was set in macro calling  ACA_SRAM_WRITE_CLR()         */
#define ACA_SRAM_WRITE_CLR( SizeWords )                                        \
    do {                                                                       \
        for ( ; ii < ( ( ( SizeWords + ( ACA_WORD_SIZE_IN_32BIT_WORDS - 1 ) )  \
                         / ACA_WORD_SIZE_IN_32BIT_WORDS )                      \
                       * ACA_WORD_SIZE_IN_32BIT_WORDS );                       \
              ii++ ) {                                                         \
            DBH_WRITE_REGISTER( ACA, SRAM_WDATA, 0 );                          \
        }                                                                      \
    } while ( 0 )

/* macro to load block to SRAM memory */
#define ACA_HW_LOAD_BLOCK_TO_ACA_MEM( Addr, ptr, SizeWords )                   \
    do {                                                                       \
        uint32_t ii;                                                           \
        ACA_WAIT_ON_ACA_DONE( );                                               \
        DBH_WRITE_REGISTER( ACA, SRAM_WADDR, ( Addr ) );                       \
        for ( ii = 0; ii < ( SizeWords ); ii++ ) {                             \
            DBH_WRITE_REGISTER(                                                \
                ACA, SRAM_WDATA, ( (uint32_t *) ( ptr ) )[ii] );               \
        }                                                                      \
        ACA_SRAM_WRITE_CLR( SizeWords );                                       \
    } while ( 0 )

/* macro to load a value to SRAM with 32-bit access */
#define ACA_HW_LOAD_VALUE_TO_ACA_MEM( Addr, Val )                              \
    do {                                                                       \
        ACA_WAIT_ON_ACA_DONE( );                                               \
        DBH_WRITE_REGISTER( ACA, SRAM_WADDR, ( Addr ) );                       \
        DBH_WRITE_REGISTER( ACA, SRAM_WDATA, ( Val ) );                        \
    } while ( 0 )

/* macro to clear ACA memory: Addr must be alighned to ACA_WORD_SIZE */
#define ACA_HW_CLEAR_ACA_MEM( Addr, SizeWords )                                \
    do {                                                                       \
        uint32_t ii;                                                           \
        ACA_WAIT_ON_ACA_DONE( );                                               \
        DBH_WRITE_REGISTER( ACA, SRAM_WADDR, ( Addr ) );                       \
        for ( ii = 0; ii < ( uint32_t )( SizeWords ); ii++ ) {                 \
            DBH_WRITE_REGISTER( ACA, SRAM_WDATA, 0x0UL );                      \
        }                                                                      \
    } while ( 0 )

/* macro to read a value from the ACA data memory */
#define ACA_HW_READ_VALUE_FROM_ACA_MEM( Addr, Val )                            \
    do {                                                                       \
        volatile uint32_t dummy;                                               \
        ACA_WAIT_ON_ACA_DONE( );                                               \
        DBH_WRITE_REGISTER( ACA, SRAM_RADDR, ( Addr ) );                       \
        dummy   = DBH_READ_REGISTER( ACA, SRAM_RDATA );                        \
        ( Val ) = dummy;                                                       \
    } while ( 0 )

/* macro to read a block from the ACA data memory */
#define ACA_HW_READ_BLOCK_FROM_ACA_MEM( Addr, ptr, SizeWords )                   \
    do {                                                                         \
        uint32_t ii;                                                             \
        volatile uint32_t dummy;                                                 \
        ACA_WAIT_ON_ACA_DONE( );                                                 \
        DBH_WRITE_REGISTER( ACA, SRAM_RADDR, ( Addr ) );                         \
        for ( ii = 0; ii < ( SizeWords ); ii++ ) {                               \
            dummy                        = DBH_READ_REGISTER( ACA, SRAM_RDATA ); \
            ( (uint32_t *) ( ptr ) )[ii] = ( dummy );                            \
        }                                                                        \
    } while ( 0 )

#endif

/*************************** The End Of File*****************************/
