/**
 * @file	SWAPv2.h
 * @brief	white box AES module for SSA(Samsung Security Architecture)
 * @author	Samsung WhiteBox AES Version 2 (SWAPv1 based)
 *				- Chul Lee(chuls.lee@samsung.com)
 *				- Dept : Software R&D Center / Cloud Computing Lab.
 *
 *			Samsung WhiteBox AES Version 1
 *				 - Jisoon Park(js00n.park@samsung.com)
 *				 - Dept: DMC R&D Center/Convergence Software Lab.
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
 */

/**
 * @interface	SWAPv2
 * @brief		encryption/decryption processing module of SWAPv2(Samsung Whitebox AES Project Version 2)
 *
 * @par Features :
 * # Functionality1 : encryption/decryption processing module of SWAPv2
 * @date 2012-10-05
 * @version 2.0
 */

#ifndef _SAMSUNG_WHITE_BOX_AES_PROJECT_VERSION2_H_
#define _SAMSUNG_WHITE_BOX_AES_PROJECT_VERSION2_H_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

/*! SWAPv2 return values */
typedef enum
{
	SWAPv2_RETURN_SUCCESS				=	0x00000000,		/*!< success case */
	SWAPv2_RETURN_INVALID_PARAMETER		=	0x80000001,		/*!< null parameter */
	SWAPv2_RETURN_INVALID_TBLSIZE		=	0x80000002,		/*!< invalid table size */
} SWAPv2_RETURN_VAL;

/**
 * @fn		SWAPv2_RETURN_VAL SWAPv2_Process(unsigned char *Table, unsigned int TableLen, unsigned char* in, unsigned char* out, unsigned int OptimizLev)
 * @brief	processing encryption or decryption according to the table
 *
 * @param	[in] Table		look-up table for SWAPv2
 * @param	[in] TableLen	byte-length of look-up table
 * @param	[in] in			input bytes(16-byte length)
 * @param	[out] out		output bytes(16-byte length)
 * @param	[in] OptimizLev	Performance Optimization Level [0~2]
 * @return	SWAPv2_RETURN_VAL
 */
SWAPv2_RETURN_VAL SWAPv2_Process(unsigned char *Table, unsigned int TableLen, unsigned char* in, unsigned char* out);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_SAMSUNG_WHITE_BOX_AES_PROJECT_H_