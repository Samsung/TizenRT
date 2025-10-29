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

#ifndef __DUBHE_ACA_INTERNAL_H__
#define __DUBHE_ACA_INTERNAL_H__

#include <stdint.h>
#include "dubhe_aca_hw_defs.h"
#include "pal_log.h"

#if defined( DUBHE_FOR_RUNTIME )
/* ACA interrupt state */
#define DBH_ACA_CMD_FINISH_INTR ( 0x01 )
#define DBH_ACA_FIFO_UNDER_WM_INTR ( 0x02 )
#define DBH_ACA_FIFO_OVERFLOW_INTR ( 0x04 )
#define DBH_ACA_DIV_ZERO_INTR ( 0x08 )
#define DBH_ACA_MODINV_ZERO_INTR ( 0x10 )
#define DBH_ACA_ENGINE_DONE_INTR ( 0x20 )
#define DBH_ACA_MULT_RED_ERR_INTR ( 0x40 )

#endif

/* define result discard value */
#define RES_DISCARD 0x1

#ifdef ACA_DEBUG

#define ACA_DUMP_FULL_OPCODE( fullopcode )                                     \
    do {                                                                       \
        PAL_LOG_DEBUG( "fullopcode:0x%x\n", (unsigned int) fullOpCode );       \
        PAL_LOG_DEBUG( "NEED_INTR:0x%x\n",                                     \
                       (unsigned int) DBH_REG_FLD_GET(                         \
                           OPCODE_ENTRY, NEED_INTR, fullopcode ) );            \
        PAL_LOG_DEBUG( "REG_C:0x%x\n",                                         \
                       (unsigned int) DBH_REG_FLD_GET(                         \
                           OPCODE_ENTRY, REG_C, fullopcode ) );                \
        PAL_LOG_DEBUG( "R_DISCARD:0x%x\n",                                     \
                       (unsigned int) DBH_REG_FLD_GET(                         \
                           OPCODE_ENTRY, R_DISCARD, fullopcode ) );            \
        PAL_LOG_DEBUG( "REG_R:0x%x\n",                                         \
                       (unsigned int) DBH_REG_FLD_GET(                         \
                           OPCODE_ENTRY, REG_R, fullopcode ) );                \
        PAL_LOG_DEBUG( "REG_B:0x%x\n",                                         \
                       (unsigned int) DBH_REG_FLD_GET(                         \
                           OPCODE_ENTRY, REG_B, fullopcode ) );                \
        PAL_LOG_DEBUG( "REG_B_IM:0x%x\n",                                      \
                       (unsigned int) DBH_REG_FLD_GET(                         \
                           OPCODE_ENTRY, REG_B_IM, fullopcode ) );             \
        PAL_LOG_DEBUG( "REG_A:0x%x\n",                                         \
                       (unsigned int) DBH_REG_FLD_GET(                         \
                           OPCODE_ENTRY, REG_A, fullopcode ) );                \
        PAL_LOG_DEBUG( "RESERVE:0x%x\n",                                       \
                       (unsigned int) DBH_REG_FLD_GET(                         \
                           OPCODE_ENTRY, RESERVE, fullopcode ) );              \
        PAL_LOG_DEBUG(                                                         \
            "LEN:0x%x\n",                                                      \
            (unsigned int) DBH_REG_FLD_GET( OPCODE_ENTRY, LEN, fullopcode ) ); \
        PAL_LOG_DEBUG( "OPCODE:0x%x\n",                                        \
                       (unsigned int) DBH_REG_FLD_GET(                         \
                           OPCODE_ENTRY, OPCODE, fullopcode ) );               \
    } while ( 0 )

#define ACA_EXEC_OPERATION(                                                    \
    Opcode, lenId, red_eq, OpA, isBImmed, OpB, Res, ResDiscard, Tag, NeedIrr ) \
    do {                                                                       \
        uint32_t fullOpCode = 0;                                               \
        ACA_ASSEMBLY_OPCODE( ( fullOpCode ),                                   \
                             ( Opcode ),                                       \
                             ( lenId ),                                        \
                             ( red_eq ),                                       \
                             ( OpA ),                                          \
                             ( isBImmed ),                                     \
                             ( OpB ),                                          \
                             ( Res ),                                          \
                             ( ResDiscard ),                                   \
                             ( Tag ),                                          \
                             ( NeedIrr ) );                                    \
        ACA_WAIT_ON_FIFO_READY( );                                             \
        ACA_DUMP_FULL_OPCODE( fullOpCode );                                    \
        DBH_WRITE_REGISTER( ACA, OPCODE_ENTRY, fullOpCode );                   \
    } while ( 0 )
#else
#define ACA_EXEC_OPERATION(                                                    \
    Opcode, lenId, red_eq, OpA, isBImmed, OpB, Res, ResDiscard, Tag, NeedIrr ) \
    do {                                                                       \
        uint32_t fullOpCode = 0;                                               \
        ACA_ASSEMBLY_OPCODE( ( fullOpCode ),                                   \
                             ( Opcode ),                                       \
                             ( lenId ),                                        \
                             ( red_eq ),                                       \
                             ( OpA ),                                          \
                             ( isBImmed ),                                     \
                             ( OpB ),                                          \
                             ( Res ),                                          \
                             ( ResDiscard ),                                   \
                             ( Tag ),                                          \
                             ( NeedIrr ) );                                    \
        ACA_WAIT_ON_FIFO_READY( );                                             \
        DBH_WRITE_REGISTER( ACA, OPCODE_ENTRY, fullOpCode );                   \
    } while ( 0 )

#endif

/*************************************************************************/
/*                    Macros for calling ACA operations                  */
/*************************************************************************/

/*----------------------------------*/
/*   1.  ADD - SUBTRACT operations  */
/*----------------------------------*/
/*  Add:   Res =  OpA + OpB  */
#define ACA_ADD( lenId, Res, OpA, OpB )                                        \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_ADD,                                     \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        0,                                                     \
                        ( OpB ),                                               \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  AddIm:  Res =  OpA + OpBIm  */
#define ACA_ADD_IM( lenId, Res, OpA, OpBIm )                                   \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_ADD,                                     \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        1,                                                     \
                        ( OpBIm ),                                             \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  Sub:  Res =  OpA - OpB  */
#define ACA_SUB( lenId, Res, OpA, OpB )                                        \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_SUB,                                     \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        0,                                                     \
                        ( OpB ),                                               \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  SubIm:  Res =  OpA - OpBIm  */
#define ACA_SUB_IM( lenId, Res, OpA, OpBIm )                                   \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_SUB,                                     \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        1,                                                     \
                        ( OpBIm ),                                             \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  ModAdd:  Res =  (OpA + OpB) mod N  */
#define ACA_MOD_ADD( lenId, Res, OpA, OpB )                                    \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_MODADD,                                  \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        0,                                                     \
                        ( OpB ),                                               \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  ModAddIm:  Res =  (OpA + OpBIm) mod N  */
#define ACA_MOD_ADD_IM( lenId, Res, OpA, OpBIm )                               \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_MODADD,                                  \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        1,                                                     \
                        ( OpBIm ),                                             \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  ModSub:  Res =  (OpA - OpB) mod N  */
#define ACA_MOD_SUB( lenId, Res, OpA, OpB )                                    \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_MODSUB,                                  \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        0,                                                     \
                        ( OpB ),                                               \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  ModSubIm:  Res =  (OpA - OpBIm) mod N  */
#define ACA_MOD_SUB_IM( lenId, Res, OpA, OpBIm )                               \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_MODSUB,                                  \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        1,                                                     \
                        ( OpBIm ),                                             \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )

/*----------------------------------*/
/*   2.  Logical   operations       */
/*----------------------------------*/

/*  AND:  Res =  OpA & OpB  */
#define ACA_AND( lenId, Res, OpA, OpB )                                        \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_AND,                                     \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        0,                                                     \
                        ( OpB ),                                               \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  AndIm:  Res =  OpA & OpB  */
#define ACA_AND_IM( lenId, Res, OpA, OpB )                                     \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_AND,                                     \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        1,                                                     \
                        ( OpB ),                                               \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  Tst0:  OpA & 0x1 -  tests the bit 0 of operand A. If bit0 = 0, then
 * ZeroOfStatus = 1, else 0  */
#define ACA_TEST_BIT0( lenId, OpA )                                            \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_AND,                                     \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        1,                                                     \
                        0x01,                                                  \
                        0,                                                     \
                        RES_DISCARD,                                           \
                        0,                                                     \
                        0 )
/*  TstBit:  OpA & (1<<i) -  tests the bit i of operand A. If biti = 0, then
 * ZeroOfStatus = 1, else 0  */
#define ACA_TEST_BIT( lenId, OpA, i )                                          \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_AND,                                     \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        1,                                                     \
                        0x01 << ( i ),                                         \
                        0,                                                     \
                        RES_DISCARD,                                           \
                        0,                                                     \
                        0 )
/*  Clr0:  Res =  OpA & (-2)  - clears the bit 0 of operand A.  Note:  -2 = 0x1E
 *  for 5-bit size */
#define ACA_CLEAR_BIT0( lenId, Res, OpA )                                      \
    ACA_EXEC_OPERATION(                                                        \
        ACA_OPCODE_ID_AND, ( lenId ), 0, ( OpA ), 1, 0x1E, ( Res ), 0, 0, 0 )
/*  Clr:  Res =  OpA & 0  - clears the operand A.  */
#define ACA_CLEAR( lenId, OpA )                                                \
    ACA_EXEC_OPERATION(                                                        \
        ACA_OPCODE_ID_AND, ( lenId ), 0, ( OpA ), 1, 0x00, ( OpA ), 0, 0, 0 )
/*  OR:  Res =  OpA || OpB  */
#define ACA_OR( lenId, Res, OpA, OpB )                                         \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_OR,                                      \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        0,                                                     \
                        ( OpB ),                                               \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  OrIm:  Res =  OpA || OpBIm  */
#define ACA_OR_IM( lenId, Res, OpA, OpBIm )                                    \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_OR,                                      \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        1,                                                     \
                        ( OpBIm ),                                             \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  Copy:  OpDest =  OpSrc || 0  */
#define ACA_COPY( lenId, OpDest, OpSrc )                                       \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_OR,                                      \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpSrc ),                                             \
                        1,                                                     \
                        0x00,                                                  \
                        ( OpDest ),                                            \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  Set0:  Res =  OpA || 1  : set bit0 = 1, other bits are not changed */
#define ACA_SET_BIT0( lenId, Res, OpA )                                        \
    ACA_EXEC_OPERATION(                                                        \
        ACA_OPCODE_ID_OR, ( lenId ), 0, ( OpA ), 1, 0x01, ( Res ), 0, 0, 0 )
/*  Xor:  Res =  OpA ^ OpB  */
#define ACA_XOR( lenId, Res, OpA, OpB )                                        \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_XOR,                                     \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        0,                                                     \
                        ( OpB ),                                               \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  XorIm:  Res =  OpA ^ OpBIm  */
#define ACA_XOR_IM( lenId, Res, OpA, OpBIm )                                   \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_XOR,                                     \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        1,                                                     \
                        ( OpBIm ),                                             \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  Flip0:  OpA =  OpA ^ 1  - inverts the bit 0 of operand A  */
#define ACA_FLIP_BIT0( lenId, Res, OpA )                                       \
    ACA_EXEC_OPERATION(                                                        \
        ACA_OPCODE_ID_XOR, ( lenId ), 0, ( OpA ), 1, 0x01, ( Res ), 0, 0, 0 )
/*  Invert:  Res =  OpA ^ 0xFFF.FF  :  inverts all bits of OpA .
                    Note: 0xFFFFF =  0x1F for 5 bits size of second operand */
#define ACA_INVERT_BITS( lenId, Res, OpA )                                     \
    ACA_EXEC_OPERATION(                                                        \
        ACA_OPCODE_ID_XOR, ( lenId ), 0, ( OpA ), 1, 0x1F, ( Res ), 0, 0, 0 )
/*  Compare:  OpA ^ OpB . Rsult of compare in ZeroBitOfStatus:  If OpA == OpB
 * then Z = 1 */
#define ACA_COMPARE( lenId, OpA, OpB )                                         \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_XOR,                                     \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        0,                                                     \
                        ( OpB ),                                               \
                        0,                                                     \
                        RES_DISCARD,                                           \
                        0,                                                     \
                        0 )
/*  CompareImmediate:  OpA ^ OpBim . Rsult of compare in ZeroBitOfStatus:  If
   OpA == OpBim then status Z = 1 */
#define ACA_COMPARE_IM( lenId, OpA, OpBim )                                    \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_XOR,                                     \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        1,                                                     \
                        ( OpBim ),                                             \
                        0,                                                     \
                        RES_DISCARD,                                           \
                        0,                                                     \
                        0 )

/*-------------------------------------------*/
/*   3.  SHIFT    operations  - S <= 31      */
/*-------------------------------------------*/

/*Dubhe  can support max 64bit shift, if shift size bigger than 32, than set
 * isBImmed to 1*/
/*  SHR0:  Res =  OpA >> (S+1) :   shifts right operand A by S+1 bits, insert 0
 * to left most bits */
#define ACA_SHR_FILL0( lenId, Res, OpA, isBImmed, S )                          \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_SHR0,                                    \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        ( isBImmed ),                                          \
                        ( S ),                                                 \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  SHR1:  Res =  OpA >> (S+1) :   shifts right operand A by S+1 bits, insert 1
 * to left most bits */
#define ACA_SHR_FILL1( lenId, Res, OpA, isBImmed, S )                          \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_SHR1,                                    \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        ( isBImmed ),                                          \
                        ( S ),                                                 \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  SHL0:  Res =  OpA << (S+1) :   shifts left operand A by S+1 bits, insert 0
 * to right most bits */
#define ACA_SHL_FILL0( lenId, Res, OpA, isBImmed, S )                          \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_SHL0,                                    \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        ( isBImmed ),                                          \
                        ( S ),                                                 \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  SHL1:  Res =  OpA << (S+1) :   shifts left operand A by S+1 bits, insert 1
 * to right most bits */
#define ACA_SHL_FILL1( lenId, Res, OpA, isBImmed, S )                          \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_SHL1,                                    \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        ( isBImmed ),                                          \
                        ( S ),                                                 \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )

/*-----------------------------------------------------*/
/*   4.  Multiplication releated operations            */
/*       Note:  See notes to ACA_ExecOperation         */
/*-----------------------------------------------------*/

/*  Mul:Res =  LowHalfOf(OpA * OpB)  */
#define ACA_MUL( lenId, Res, OpA, OpB )                                        \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_MUL,                                     \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        0,                                                     \
                        ( OpB ),                                               \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  MulIm:  Res =  OpA * OpBIm  */
#define ACA_MUL_IM( lenId, Res, OpA, OpBIm )                                   \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_MUL,                                     \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        1,                                                     \
                        ( OpBIm ),                                             \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  HMul:  Res =  HighHalfOf(OpA * OpB), where size of
           operands is equaled to operation size, defined by lenId. Note: Size
   of operation result is by one word large, than operation size */
#define ACA_MUL_HIGH( lenId, Res, OpA, OpB )                                   \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_MULHIGH,                                 \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        0,                                                     \
                        ( OpB ),                                               \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  MulIm:  Res =  OpA * OpBIm  mod N*/
#define ACA_MOD_MUL_IM( lenId, Res, OpA, OpBIm )                               \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_MODMUL,                                  \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        1,                                                     \
                        ( OpBIm ),                                             \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  ModMul:  Res =  OpA * OpB  mod N - modular multiplication */
#define ACA_MOD_MUL( lenId, Res, OpA, OpB )                                    \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_MODMUL,                                  \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        0,                                                     \
                        ( OpB ),                                               \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  ModMulN:  Res =  OpA * OpB  mod N - modular multiplication (all reduction is
 * omitted)* up to ACA_EXTRA_BITS extra bits */
#define ACA_MOD_MUL_NFR( lenId, Res, OpA, OpB )                                \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_MODMULNR,                                \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        0,                                                     \
                        ( OpB ),                                               \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        0 )
/*  ModMulAcc:  Res =  OpA * OpB + OpC mod N - modular multiplication and     *
 *   adding, result reduced */
#define ACA_MOD_MUL_ACC( lenId, Res, OpA, OpB, OpC )                           \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_MODMLACC,                                \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        0,                                                     \
                        ( OpB ),                                               \
                        ( Res ),                                               \
                        0,                                                     \
                        ( OpC ),                                               \
                        0 )
/*  ModMulAccN:  Res =  OpA * OpB + OpC mod N - modular multiplication and    *
 *   acdding (final reduction is omitted) -  up to ACA_EXTRA_BITS extra bits */
#define ACA_MOD_MUL_ACC_NFR( lenId, Res, OpA, OpB, OpC )                       \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_MODMLACCNR,                              \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        0,                                                     \
                        ( OpB ),                                               \
                        ( Res ),                                               \
                        0,                                                     \
                        ( OpC ),                                               \
                        0 )

/*-----------------------------------------------------*/
/*   4.  other operations                              */
/*       Note:  See notes to ACA_ExecOperation         */
/*-----------------------------------------------------*/

/*  ModExp:  Res =  OpA ** OpB  mod N - modular exponentiation */
#define ACA_MOD_EXP( lenId, Res, OpA, OpB, NeedIrr )                           \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_MODEXP,                                  \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        0,                                                     \
                        ( OpB ),                                               \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        NeedIrr )
/*  Divide:  Res =  OpA / OpB , OpA = OpA mod OpB - division,  */
#define ACA_DIV( lenId, Res, OpA, OpB, NeedIrr )                               \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_DIV,                                     \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        0,                                                     \
                        ( OpB ),                                               \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        NeedIrr )
/*  ModInv:  Modular inversion: calculates   Res = 1/OpA mod N  */
#define ACA_MOD_INV( lenId, Res, OpA, NeedIrr )                                \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_MODINV,                                  \
                        ( lenId ),                                             \
                        0,                                                     \
                        ( OpA ),                                               \
                        0,                                                     \
                        0 /*opB not need*/,                                    \
                        ( Res ),                                               \
                        0,                                                     \
                        0,                                                     \
                        NeedIrr )
/* Modular reduction: Res = OpA mod N  by subtract the modulus N     *
 *   times, while Res > N. the counter can be read by status register   */
#define ACA_REDUCE( lenId, Res, OpA, R_eq, NeedIrr )                           \
    ACA_EXEC_OPERATION( ACA_OPCODE_ID_RED,                                     \
                        ( lenId ),                                             \
                        R_eq,                                                  \
                        ( OpA ),                                               \
                        0,                                                     \
                        0 /*opB not need*/,                                    \
                        ( Res ),                                               \
                        0,                                                     \
                        0 /*Tag*/,                                             \
                        NeedIrr )

/*************************************************
 **************  second Level macros ************
 **************************************************/
#if 0
/* mod inversion using exponentiation, used when 'a' can be even number, but *
*  runs at constant time                                                     */
#define ACA_MOD_INV_W_EXP( res, a, nm2 )                                       \
    {                                                                          \
        ACA_SUB_IM( LEN_ID_N_ACA_REG_BITS, ( nm2 ), 0 /*n*/, 2 );              \
        ACA_MOD_EXP( LEN_ID_N_BITS, ( res ), ( a ), ( nm2 ) );                 \
    }

#define ACA_SET_VAL( a, v )                                                    \
    {                                                                          \
        ACA_AND_IM( LEN_ID_N_ACA_REG_BITS, a, a, 0 );                          \
        ACA_OR_IM( LEN_ID_N_ACA_REG_BITS, a, a, v );                           \
    }
#endif
#define ACA_SET_VAL( lenId, a, v )                                             \
    {                                                                          \
        ACA_AND_IM( lenId, a, a, 0 );                                          \
        ACA_OR_IM( lenId, a, a, v );                                           \
    }

#define ACA_COMPARE_STATUS( lenId, a, b, stat )                                \
    {                                                                          \
        ACA_COMPARE( lenId, a, b );                                            \
        ACA_GET_STATUS_XOR_RESULT_ZERO( stat );                                \
    }

#define ACA_COMPARE_IM_STATUS( lenId, a, b, stat )                             \
    {                                                                          \
        ACA_COMPARE_IM( lenId, a, b );                                         \
        ACA_GET_STATUS_XOR_RESULT_ZERO( stat );                                \
    }

#define ACA_READ_BIT0( lenId, reg, bitVal )                                    \
    {                                                                          \
        ACA_TEST_BIT0( lenId, reg );                                           \
        ACA_GET_STATUS_AND_RESULT_ZERO( bitVal );                              \
        ( bitVal ) = !( bitVal );                                              \
    }

/*uint32 b - bit i value, i-num. of LS bit, i <= 31*/
#define ACA_READ_BIT( bitVal, reg, i )                                         \
    {                                                                          \
        ACA_TEST_BIT( 1 /*lenId*/, reg, ( i ), 0 );                            \
        ACA_GET_STATUS_AND_RESULT_ZERO( ( bitVal ) );                          \
        ( bitVal ) = !( bitVal );                                              \
    }

#define ACA_READ_WORD_FROM_REG( val, i, virtReg )                              \
    {                                                                          \
        uint32_t addr;                                                         \
        ACA_GET_REG_ADDRESS( virtReg, addr );                                  \
        ACA_HW_READ_VALUE_FROM_ACA_MEM( addr + ( i ), val );                   \
    }

#define ACA_WRITE_WORD_TO_REG( Val, i, VirtReg )                               \
    {                                                                          \
        uint32_t addr;                                                         \
        ACA_GET_REG_ADDRESS( ( VirtReg ), addr );                              \
        ACA_HW_LOAD_VALUE_TO_ACA_MEM( addr + ( i ), Val );                     \
    }

void aca_copy_data_from_reg( uint32_t *dst_ptr,       /*!< [out] Pointer to
                                                       * destination buffer. */
                             uint32_t sizeWords,      /*!< [in]  Source size in
                                                       * words. */
                             uint32_t srcReg );       /*!< [in]  Virtual address
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
void dubhe_aca_driver_init( );

#endif

/*************************** The End Of File*****************************/
