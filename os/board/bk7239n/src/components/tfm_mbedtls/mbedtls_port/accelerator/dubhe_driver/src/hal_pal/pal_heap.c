/**
 * Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#include "pal_common.h"
#include "psa_sp_api.h"
#include "pal_string.h"
#include "pal_heap.h"
#include <stdlib.h>
#include <string.h>
#if CONFIG_ENABLE_MCUBOOT_BL2
void *pal_malloc( size_t size )
{
    void *p = NULL;

    p = malloc( size );
    if ( p ) {
        memset( p, 0, size );
    }

    return p;
}

void pal_free( void *ptr )
{
    free( ptr );
}
#else
void *pal_malloc( size_t size )
{
    void *p = NULL;

    p = psa_malloc( size );
    if ( p ) {
        pal_memset( p, 0, size );
    }

    return p;
}

void pal_free( void *ptr )
{
    psa_free( ptr );
}
#endif
/*************************** The End Of File*****************************/

