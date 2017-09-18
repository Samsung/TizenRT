/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef _SWAPV2_DEFINES_H_
#define _SWAPV2_DEFINES_H_

////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////
#define SWAPV2_WORD_SIZE		4			/*!< Each word has 4-byte length, defined in FIPS-197  */
#define SWAPV2_COLUMNS			4			/*!< Number of columns comprising the State. Defined as Nb in FIPS-197  */

#define SWAPV2_128_ROUNDS		10			/*!< Number of rounds, for AES-128  */
#define SWAPV2_192_ROUNDS		12			/*!< Number of rounds, for AES-192  */
#define SWAPV2_256_ROUNDS		14			/*!< Number of rounds, for AES-256  */

////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////
typedef unsigned char SWAPV2_WORD[SWAPV2_WORD_SIZE];	//!< SWAPV2_WORD represents each word
typedef unsigned char SWAPV2_RK_BOX[SWAPV2_COLUMNS][SWAPV2_WORD_SIZE];	//!< SWAPV2_RK_BOX represents round keys for one round
typedef SWAPV2_WORD SWAPV2_MIX_COLUMN_BOX[16][16];	//!< SWAPV2_MIX_COLUMN_BOX represents each MC box
typedef unsigned char SWAPV2_SBOX[16][16];	//!< SWAPV2_SBOX represents each S-box
typedef SWAPV2_WORD SWAPV2_TBOX[16][16];	//!< SWAPV2_TBOX represents transform table for each byte of round key
typedef unsigned char SWAPV2_PBOX[4][8];	//!< SWAPV2_PBOX represents permutation table for each round
typedef unsigned char SWAPV2_PVECTOR[16];	//!< SWAPV2_PVECTOR represents permutation vector for each round
typedef SWAPV2_WORD SWAPV2_WORD_PBOX[16];	//!< SWAPV2_WORD_PBOX represents permutation word of transform table for each round
typedef unsigned char SWAPV2_BIT_PBOX[8];


////////////////////////////////////////////////////////////////////////////
// Macros
////////////////////////////////////////////////////////////////////////////
#define SBOX_SIZ					(16 * 16)														/*! SBOX_SIZ represents the size of S-box(16x16 matrix), for 4bit-4bit lookup */
#define SWAPV2_T_TABLE_ROUND_SIZE		(sizeof(SWAPV2_TBOX) * SWAPV2_COLUMNS * SWAPV2_WORD_SIZE)	/*! SWAPV2_T_TABLE_ROUND_SIZE represents the size of transform table for one round */
#define SWAPV2_P_TABLE_ROUND_SIZE		(sizeof(SWAPV2_PBOX))										/*! SWAPV2_P_TABLE_ROUND_SIZE represents the size of permutation table for one round */
#define SWAPV2_WP_TABLE_ROUND_SIZE	SWAPV2_WORD_SIZE * 4											/*! SWAPV2_WP_TABLE_ROUND_SIZE represents the size of permutation word of transform table for one round */
#define SWAPV2_BP_TABLE_ROUND_SIZE	8																/*! SWAPV2_BP_TABLE_ROUND_SIZE represents the size of permutation bit of transform table for one round */
#define TABLE_LENGTH				164432
#define SETBIT_U8(val, idx, bit)	bit == 1 ?	(unsigned char) ( val | (1 << idx) ) :	(unsigned char) ( val & (~(1 << idx)) )		/*! set a bit */
#define GETBIT_U8(val, idx)			(val & (1 << idx)) >> idx																		/*! get a bit */

/**
 * @var static const unsigned char byte_pMatrix[24][4]
 * @brief look-up matrix for CalcPWTBox, Calculate Word-Permutation T-Box
 */
static const unsigned char byte_pMatrix[24][4] = {
	{0, 1, 2, 3}, {0, 1, 3, 2}, {0, 2, 1, 3}, {0, 2, 3, 1}, {0, 3, 1, 2}, {0, 3, 2, 1},
	{1, 0, 2, 3}, {1, 0, 3, 2}, {1, 2, 0, 3}, {1, 2, 3, 0}, {1, 3, 0, 2}, {1, 3, 2, 0},
	{2, 0, 1, 3}, {2, 0, 3, 1}, {2, 1, 0, 3}, {2, 1, 3, 0}, {2, 3, 0, 1}, {2, 3, 1, 0},
	{3, 0, 1, 2}, {3, 0, 2, 1}, {3, 1, 0, 2}, {3, 1, 2, 0}, {3, 2, 0, 1}, {3, 2, 1, 0}
};



#endif							//_SWAPV2_DEFINES_H_
