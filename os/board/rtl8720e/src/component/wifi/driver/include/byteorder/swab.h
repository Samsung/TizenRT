/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef _LINUX_BYTEORDER_SWAB_H
#define _LINUX_BYTEORDER_SWAB_H
/*
#if !defined(CONFIG_PLATFORM_MSTAR_TITANIA12) && !defined(CONFIG_PLATFORM_8195A)
*/
#if !defined(CONFIG_PLATFORM_MSTAR_TITANIA12)

__inline static __u16  ___swab16(__u16 x)
{
	__u16 __x = x;
	return
		((__u16)(
			 (((__u16)(__x) & (__u16)0x00ffU) << 8) |
			 (((__u16)(__x) & (__u16)0xff00U) >> 8)));

}

__inline static __u32  ___swab32(__u32 x)
{
	__u32 __x = (x);
	return ((__u32)(
				(((__u32)(__x) & (__u32)0x000000ffUL) << 24) |
				(((__u32)(__x) & (__u32)0x0000ff00UL) <<  8) |
				(((__u32)(__x) & (__u32)0x00ff0000UL) >>  8) |
				(((__u32)(__x) & (__u32)0xff000000UL) >> 24)));
}

__inline static __u64  ___swab64(__u64 x)
{
	__u64 __x = (x);

	return
		((__u64)(\
				 (__u64)(((__u64)(__x) & (__u64)0x00000000000000ffULL) << 56) | \
				 (__u64)(((__u64)(__x) & (__u64)0x000000000000ff00ULL) << 40) | \
				 (__u64)(((__u64)(__x) & (__u64)0x0000000000ff0000ULL) << 24) | \
				 (__u64)(((__u64)(__x) & (__u64)0x00000000ff000000ULL) <<  8) | \
				 (__u64)(((__u64)(__x) & (__u64)0x000000ff00000000ULL) >>  8) | \
				 (__u64)(((__u64)(__x) & (__u64)0x0000ff0000000000ULL) >> 24) | \
				 (__u64)(((__u64)(__x) & (__u64)0x00ff000000000000ULL) >> 40) | \
				 (__u64)(((__u64)(__x) & (__u64)0xff00000000000000ULL) >> 56)));
	\
}
#endif // CONFIG_PLATFORM_MSTAR_TITANIA12

#ifndef __arch__swab16
__inline static __u16 __arch__swab16(__u16 x)
{
	return ___swab16(x);
}

#endif

#ifndef __arch__swab32
__inline static __u32 __arch__swab32(__u32 x)
{
	__u32 __tmp = (x) ;
	return ___swab32(__tmp);
}
#endif

#ifndef __arch__swab64

__inline static __u64 __arch__swab64(__u64 x)
{
	__u64 __tmp = (x) ;
	return ___swab64(__tmp);
}


#endif

#ifndef __swab16
#define __swab16(x) __fswab16(x)
#define __swab32(x) __fswab32(x)
#define __swab64(x) __fswab64(x)
#endif	// __swab16

__inline static __u16 __fswab16(__u16 x)
{
	return __arch__swab16(x);
}

__inline static __u32 __fswab32(__u32 x)
{
	return __arch__swab32(x);
}

#endif /* _LINUX_BYTEORDER_SWAB_H */

