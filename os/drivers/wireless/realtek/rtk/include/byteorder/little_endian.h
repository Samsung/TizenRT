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
#ifndef _LINUX_BYTEORDER_LITTLE_ENDIAN_H
#define _LINUX_BYTEORDER_LITTLE_ENDIAN_H

#ifndef __LITTLE_ENDIAN
#define __LITTLE_ENDIAN 1234
#endif
#ifndef __LITTLE_ENDIAN_BITFIELD
#define __LITTLE_ENDIAN_BITFIELD
#endif

#include <byteorder/swab.h>

#ifndef __constant_htonl

//TODO
#if 0

#define __constant_htonl(x) ___constant_swab32((x))
#define __constant_ntohl(x) ___constant_swab32((x))
#define __constant_htons(x) ___constant_swab16((x))
#define __constant_ntohs(x) ___constant_swab16((x))
#define __constant_cpu_to_le64(x) ((__u64)(x))
#define __constant_le64_to_cpu(x) ((__u64)(x))
#define __constant_cpu_to_le32(x) ((__u32)(x))
#define __constant_le32_to_cpu(x) ((__u32)(x))
#define __constant_cpu_to_le16(x) ((__u16)(x))
#define __constant_le16_to_cpu(x) ((__u16)(x))
#define __constant_cpu_to_be64(x) ___constant_swab64((x))
#define __constant_be64_to_cpu(x) ___constant_swab64((x))
#define __constant_cpu_to_be32(x) ___constant_swab32((x))
#define __constant_be32_to_cpu(x) ___constant_swab32((x))
#define __constant_cpu_to_be16(x) ___constant_swab16((x))
#define __constant_be16_to_cpu(x) ___constant_swab16((x))
#define __cpu_to_le64(x) ((__u64)(x))
#define __le64_to_cpu(x) ((__u64)(x))

#endif //#if 0

#define __cpu_to_le64(x) ((__u64)(x))
#define __le64_to_cpu(x) ((__u64)(x))
#define __cpu_to_le32(x) ((__u32)(x))
#define __le32_to_cpu(x) ((__u32)(x))
#define __cpu_to_le16(x) ((__u16)(x))
#define __le16_to_cpu(x) ((__u16)(x))
#define __cpu_to_be64(x) __swab64((x))
#define __be64_to_cpu(x) __swab64((x))
#define __cpu_to_be32(x) __swab32((x))
#define __be32_to_cpu(x) __swab32((x))
#define __cpu_to_be16(x) __swab16((x))
#define __be16_to_cpu(x) __swab16((x))
#define __cpu_to_le64p(x) (*(__u64 *)(x))
#define __le64_to_cpup(x) (*(__u64 *)(x))
#define __cpu_to_le32p(x) (*(__u32 *)(x))
#define __le32_to_cpup(x) (*(__u32 *)(x))
#define __cpu_to_le16p(x) (*(__u16 *)(x))
#define __le16_to_cpup(x) (*(__u16 *)(x))
#define __cpu_to_be64p(x) __swab64p((x))
#define __be64_to_cpup(x) __swab64p((x))
#define __cpu_to_be32p(x) __swab32p((x))
#define __be32_to_cpup(x) __swab32p((x))
#define __cpu_to_be16p(x) __swab16p((x))
#define __be16_to_cpup(x) __swab16p((x))
#define __cpu_to_le64s(x) \
	do {                  \
	} while (0)
#define __le64_to_cpus(x) \
	do {                  \
	} while (0)
#define __cpu_to_le32s(x) \
	do {                  \
	} while (0)
#define __le32_to_cpus(x) \
	do {                  \
	} while (0)
#define __cpu_to_le16s(x) \
	do {                  \
	} while (0)
#define __le16_to_cpus(x) \
	do {                  \
	} while (0)
#define __cpu_to_be64s(x) __swab64s((x))
#define __be64_to_cpus(x) __swab64s((x))
#define __cpu_to_be32s(x) __swab32s((x))
#define __be32_to_cpus(x) __swab32s((x))
#define __cpu_to_be16s(x) __swab16s((x))
#define __be16_to_cpus(x) __swab16s((x))
#endif // __constant_htonl

#include <byteorder/generic.h>

#endif /* _LINUX_BYTEORDER_LITTLE_ENDIAN_H */
