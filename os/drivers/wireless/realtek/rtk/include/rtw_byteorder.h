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
#ifndef _RTL871X_BYTEORDER_H_
#define _RTL871X_BYTEORDER_H_

#include <drv_conf.h>

#if defined(CONFIG_LITTLE_ENDIAN) && defined(CONFIG_BIG_ENDIAN)
#error "Shall be CONFIG_LITTLE_ENDIAN or CONFIG_BIG_ENDIAN, but not both!\n"
#endif

#if defined(CONFIG_LITTLE_ENDIAN)
#ifndef CONFIG_PLATFORM_MSTAR389
#include <byteorder/little_endian.h>
#endif
#elif defined(CONFIG_BIG_ENDIAN)
#include <byteorder/big_endian.h>
#else
#error "Must be LITTLE/BIG Endian Host"
#endif

#ifdef CONFIG_BIG_ENDIAN
#define _htons(x) (x)
#define _ntohs(x) (x)
#define _htonl(x) (x)
#define _ntohl(x) (x)
#else  /* !CONFIG_BIG_ENDIAN */
u16 _htons(u16 x);
u16 _ntohs(u16 x);
u32 _htonl(u32 x);
u32 _ntohl(u32 x);
#endif /* CONFIG_BIG_ENDIAN */

#endif /* _RTL871X_BYTEORDER_H_ */
