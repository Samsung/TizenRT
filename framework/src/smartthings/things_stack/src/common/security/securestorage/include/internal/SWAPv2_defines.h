/**
 * @file	SWAPv2_defines.h
 * @brief	defines the constants, types and macros used in SWAPv2(Samsung Whitebox AES Project version2)
 * @author	Samsung WhiteBox AES Version 2 (SWAPv1 based)
 *				- Chul Lee(chuls.lee@samsung.com)
 *				- Dept : Software R&D Center / Cloud Computing Lab.
 *
 *			Samsung WhiteBox AES Version 1
 *				 - Jisoon Park(js00n.park@samsung.com)
 *				 - Dept: DMC R&D Center/Convergence Software Lab.
 *
 *			
 *			
 *
 * @see		SWAPv2
 * @date	2012-10-05
 * @version	2.0
 * @par		Copyright:
 * Copyright 2012 by Samsung Electronics, Inc.,
 * This software is the confiedential and properietary information 
 * of Samsung Elctronics, Inc. ("Confidential Information"). You 
 * shall not disclose such Confidential Information and shall use 
 * it only in accordance with the term of the lice agreement 
 * you entered into with Samsung.
 * @par		Warning:
 * This code sould not be exported out of Samsung Electronics.
 * This code cannot be inserted in any customer device.
 * The logic of whitebox table generation is confidential.
 */

#ifndef _SWAPv2_DEFINES_H_
#define _SWAPv2_DEFINES_H_


////////////////////////////////////////////////////////////////////////////
// Count of round-value for Simple AES 
////////////////////////////////////////////////////////////////////////////
//#define SIMPLE_AES_NR	3


////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////
#define SWAPv2_WORD_SIZE		4			/*!< Each word has 4-byte length, defined in FIPS-197  */
#define SWAPv2_COLUMNS			4			/*!< Number of columns comprising the State. Defined as Nb in FIPS-197  */

#define SWAPv2_128_ROUNDS		10			/*!< Number of rounds, for AES-128  */
#define SWAPv2_192_ROUNDS		12			/*!< Number of rounds, for AES-192  */
#define SWAPv2_256_ROUNDS		14			/*!< Number of rounds, for AES-256  */

////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////
typedef unsigned char		SWAPv2_WORD[SWAPv2_WORD_SIZE];					//!< SWAPv2_WORD represents each word
typedef unsigned char		SWAPv2_RK_BOX[SWAPv2_COLUMNS][SWAPv2_WORD_SIZE];//!< SWAPv2_RK_BOX represents round keys for one round
typedef SWAPv2_WORD			SWAPv2_MIX_COLUMN_BOX[16][16];					//!< SWAPv2_MIX_COLUMN_BOX represents each MC box
typedef unsigned char		SWAPv2_SBOX[16][16];							//!< SWAPv2_SBOX represents each S-box
typedef SWAPv2_WORD			SWAPv2_TBOX[16][16];							//!< SWAPv2_TBOX represents transform table for each byte of round key
typedef unsigned char		SWAPv2_PBOX[4][8];								//!< SWAPv2_PBOX represents permutation table for each round
typedef unsigned char		SWAPv2_PVECTOR[16];								//!< SWAPv2_PVECTOR represents permutation vector for each round
typedef SWAPv2_WORD			SWAPv2_WORD_PBOX[16];							//!< SWAPv2_WORD_PBOX represents permutation word of transform table for each round
typedef unsigned char		SWAPv2_BIT_PBOX[8];


////////////////////////////////////////////////////////////////////////////
// Macros
////////////////////////////////////////////////////////////////////////////
#define SBOX_SIZ					(16 * 16)														/*! SBOX_SIZ represents the size of S-box(16x16 matrix), for 4bit-4bit lookup */
#define SWAPv2_T_TABLE_ROUND_SIZE		(sizeof(SWAPv2_TBOX) * SWAPv2_COLUMNS * SWAPv2_WORD_SIZE)	/*! SWAPv2_T_TABLE_ROUND_SIZE represents the size of transform table for one round */
#define SWAPv2_P_TABLE_ROUND_SIZE		(sizeof(SWAPv2_PBOX))										/*! SWAPv2_P_TABLE_ROUND_SIZE represents the size of permutation table for one round */
#define SWAPv2_WP_TABLE_ROUND_SIZE	SWAPv2_WORD_SIZE * 4											/*! SWAPv2_WP_TABLE_ROUND_SIZE represents the size of permutation word of transform table for one round */
#define SWAPv2_BP_TABLE_ROUND_SIZE	8																/*! SWAPv2_BP_TABLE_ROUND_SIZE represents the size of permutation bit of transform table for one round */
#define TABLE_LENGTH				164432
#define SETBIT_U8(val, idx, bit)	bit == 1 ?	(unsigned char) ( val | (1 << idx) ) :	(unsigned char) ( val & (~(1 << idx)) )		/*! set a bit */ 
#define GETBIT_U8(val, idx)			(val & (1 << idx)) >> idx																		/*! get a bit */

/**
 * @var static const unsigned char byte_pMatrix[24][4]
 * @brief look-up matrix for CalcPWTBox, Calculate Word-Permutation T-Box
 */
static const unsigned char byte_pMatrix[24][4] = 
{
	{0, 1, 2, 3}, {0, 1, 3, 2}, {0, 2, 1, 3}, {0, 2, 3, 1}, {0, 3, 1, 2}, {0, 3, 2, 1},
	{1, 0, 2, 3}, {1, 0, 3, 2}, {1, 2, 0, 3}, {1, 2, 3, 0}, {1, 3, 0, 2}, {1, 3, 2, 0},
	{2, 0, 1, 3}, {2, 0, 3, 1}, {2, 1, 0, 3}, {2, 1, 3, 0}, {2, 3, 0, 1}, {2, 3, 1, 0},
	{3, 0, 1, 2}, {3, 0, 2, 1}, {3, 1, 0, 2}, {3, 1, 2, 0}, {3, 2, 0, 1}, {3, 2, 1, 0}
};

#endif //_SWAPv2_DEFINES_H_
