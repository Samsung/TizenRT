/*!
 *  @file       isp_util.h
 *  @brief      Headerfile : util functions to support memset, memcpy, memcmp
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */
#ifndef ISP_UTIL_H_
#define ISP_UTIL_H_

#include "isp_type.h"

// ======================================
// Function
// ======================================
int _isp_memcpy_u32_4PKA(u32 *pu32Dst, u32 *pu32Src, u32 u32Size);
int _isp_memset_u32(u32 *pu32Dst, u32 u32Src, u32 u32Size);
int _isp_memcpy_u32(u32 *pu32Dst, u32 *pu32Src, u32 u32Size);
int _isp_memcmp_u32(const u32 *pu32Src1, const u32 *pu32Src2, u32 u32Size);

int _isp_memxor_u32(u32 *pu32Dst, u32 *pu32Src1, u32 *pu32Src2, u32 u32Size);
int _isp_memcpy_u32_4PKA_Swap(u32 *pu32Dst, u32 *pu32Src, u32 u32Size, int u32Swap);
int _isp_memcpy_swap_u32(u32 *pu32Dst, u32 *pu32Src, u32 u32Size);

int _isp_memset_u8(u8 *pu8Dst, u8 u8Src, u32 u32Size);
int _isp_memcpy_u8(u8 *pu8Dst, u8 *pu8Src, u32 u32Size);
int _isp_memcmp_u8(const u8 *pu8Src1, const u8 *pu8Src2, u32 u32Size);

int _isp_memcpy_mailbox(u32 *pu32Dst, u32 *pu32Src, u32 u32Size_byte_len);
int _isp_memcpy_mailbox_swap(u32 *pu32Dst, u32 *pu32Src, u32 u32Size_byte_len);

int _isp_check_oid(u32 inputoid, u32 algorithm);
int _isp_is_zero(const u32 *pu32Src, u32 u32Size);

#define ISP_HMAC	(0x001)
#define ISP_HASH	(0x002)
#define ISP_DH		(0x003)
#define ISP_ECDH	(0x004)
#define ISP_RSA		(0x005)
#define ISP_ECDSA	(0x006)

#endif							/* ISP_UTIL_H_ */
