/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#ifndef _LINUX_BYTEORDER_SWAB_H
#define _LINUX_BYTEORDER_SWAB_H

#ifndef __arch__swab16
__inline static __u16 __arch__swab16(__u16 x)
{
	return ___swab16(x);
}
#endif

#ifndef __arch__swab32
__inline static __u32 __arch__swab32(__u32 x)
{
	__u32 __tmp = (x);
	return ___swab32(__tmp);
}
#endif

#ifndef __arch__swab64

__inline static __u64 __arch__swab64(__u64 x)
{
	__u64 __tmp = (x);
	return ___swab64(__tmp);
}
#endif

#ifndef __swab16
#define __swab16(x) __fswab16(x)
#define __swab32(x) __fswab32(x)
#define __swab64(x) __fswab64(x)
#endif // __swab16

#if defined(PLATFORM_LINUX) || defined(PLATFORM_WINDOWS)
#define swab16 __swab16
#define swab32 __swab32
#define swab64 __swab64
#define swab16p __swab16p
#define swab32p __swab32p
#define swab64p __swab64p
#define swab16s __swab16s
#define swab32s __swab32s
#define swab64s __swab64s
#endif

#endif /* _LINUX_BYTEORDER_SWAB_H */
