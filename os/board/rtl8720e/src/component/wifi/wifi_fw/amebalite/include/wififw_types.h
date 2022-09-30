#ifndef _ARCH_TYPES_H
#define _ARCH_TYPES_H

//-------------------------------------------------------
//
//      Belows are Keil-C Related.
//
//-------------------------------------------------------
typedef unsigned char           BOOLEAN, *PBOOLEAN;
typedef unsigned char           u1Byte, *pu1Byte;
typedef unsigned short          u2Byte, *pu2Byte;
typedef unsigned long           u4Byte, *pu4Byte;
typedef unsigned long long		u8Byte, *pu8Byte;

typedef char                    s1Byte, *ps1Byte;
typedef short                   s2Byte, *ps2Byte;
typedef long                    s4Byte, *ps4Byte;

#ifndef PVOID
typedef void                     *PVOID;
#endif

typedef signed char              s8;
typedef unsigned char           u8;

typedef short                   s16;
typedef unsigned short          u16;

#ifndef s32
typedef long                    s32;
#endif

#ifndef u32
typedef unsigned long           u32;
#endif

#ifndef u64
typedef unsigned long long 	u64;
#endif

#define printf                  DBG_8195A

#if !defined(__IARSTDLIB__)
#ifndef memcmp
#define memcmp(dst, src, sz)			_memcmp(dst, src, sz)
#endif
#ifndef memset
#define memset(dst, val, sz)			_memset(dst, val, sz)
#endif
#ifndef memcpy
#define memcpy(dst, src, sz)			_memcpy(dst, src, sz)
#endif
#endif /* #if !defined(__IARSTDLIB__) */

#define _U16_MSB(x)             (((u16)(x))>>8)
#define _U16_LSB(x)             ((u8)(x))
#define _B2W(LSB, MSB)          ((u16)(LSB) | (((u16)(MSB))<<8))
#define _B2D(x0,x1,x2,x3)       ((u32)(x0)|((u32)(x1)<<8)|((u32)(x2)<<16)|((u32)(x3)<<24))
#define _D2B(x, idx)            (u8)(((u32)(x))>>(8*(idx)))
#define _L2B(L)                 (((u32)(L&0xFF000000)>>24)|((u32)(L&0x00FF0000)>>8)|((u32)(L&0x0000FF00)<<8)|((u32)(L&0x000000FF)<<24))
#define _nop_()

#endif  /* #ifndef _ARCH_TYPES_H */
