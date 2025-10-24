/**
 * @if copyright_display
 *      Copyright (C), 2018-2021, Arm Technology (China) Co., Ltd.
 *      All rights reserved
 *
 *      The content of this file or document is CONFIDENTIAL and PROPRIETARY
 *      to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 *      License Agreement between Licensee and Arm Technology (China) Co., Ltd
 *      restricting among other things, the use, reproduction, distribution
 *      and transfer.  Each of the embodiments, including this information and
 *      any derivative work shall retain this copyright notice.
 * @endif
 */

#ifndef __DBH_DRIVER_INTERNAL_H__
#define __DBH_DRIVER_INTERNAL_H__

typedef enum _dubhe_bool_t {
    DBH_FALSE = 0,
    DBH_TRUE  = 1,
} dubhe_bool_t;

#if defined( DUBHE_FOR_RUNTIME )
/* driver interrupt state */
#define DBH_STAT_ACA0_INTR ( 0x01 )
#define DBH_STAT_HASH0_INTR ( 0x02 )
#define DBH_STAT_SCA0_INTR ( 0x04 )
#define DBH_STAT_RNP0_INTR ( 0x08 )
#define DBH_STAT_TRNG_INTR ( 0x10 )
#endif

#endif

/*************************** The End Of File*****************************/
