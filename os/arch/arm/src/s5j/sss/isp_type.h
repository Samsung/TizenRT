/*!
 *  @file       isp_type.h
 *  @brief      Headerfile : types for ISP
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

//! @defgroup SECURITY_ISP api for isp driver
//! @defgroup SECURITY_ISP_MAILBOX api for mailbox interface to ISP

#ifndef ISP_TYPE_H_
#define ISP_TYPE_H_

#ifndef CONFIG_S5J_SILICON
#define CONFIG_S5J_SILICON
#endif

/********** types **********/

typedef unsigned int  u32;
typedef signed int    s32;
typedef unsigned char u8;
typedef signed char   s8;

/********** Macro Function for SFR access **********/
#define BIT(nbit)                   (0x1u << (nbit))

#define SFR_BIT_CLR(val, bit)       ((val) &= (~(bit))) ///<bit clear by bit value
#define SFR_BIT_SET(val, bit)       ((val) |= (bit))    ///<bit set by bit value

#define SFR_SET(sfr, val)           ((sfr) = (val))
#define SFR_GET(sfr, val)           ((val) = (sfr))

#define WAIT_SFR_BIT_CLR(sfr, bit)  while ((sfr) & (bit))    ///<wait until bit is cleared
#define WAIT_SFR_BIT_SET(sfr, bit)  while (!((sfr) & (bit))) ///<wait until bit is set

#define SET_DWORD_TO_BBUF(buf, dword)			\
	((u8 *)(buf))[3] = ((u8)((dword) >> 0));	\
	((u8 *)(buf))[2] = ((u8)((dword) >> 8));	\
	((u8 *)(buf))[1] = ((u8)((dword) >> 16));	\
	((u8 *)(buf))[0] = ((u8)((dword) >> 24));
#define GET_DWORD_FROM_BBUF(buf)			\
	(u32)(									\
		((((u8 *)(buf))[3]) << 0)	|		\
		((((u8 *)(buf))[2]) << 8)	|		\
		((((u8 *)(buf))[1]) << 16)	|		\
		((((u8 *)(buf))[0]) << 24))

#define SWAP32(val)							\
	(u32)(									\
		(((val) & 0xff) << 24) |			\
		(((val) & 0xff00) << 8) |			\
		(((val) & 0xff0000) >> 8) |			\
		(((val) & 0xff000000) >> 24)		\
	)

#define CEIL_BY_WORD(val)   (val & 0x3) ? (1 + (val >> 2)) : (val >> 2)
#define CEIL_BY_16BYTE(val) (val & 0xF) ? ((val & 0xFFFFFFF0) + 0x10) : (val)

/********** Defines  **********/
#define SUCCESS (0x00)
#define FAIL    (0x01)

#ifndef NULL
#define NULL    ((void *) 0)
#endif

#endif							/* ISP_TYPE_H_ */
