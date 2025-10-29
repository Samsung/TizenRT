/**
 * @if copyright_display
 *      Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
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

#ifndef _DUBHE_SRAM_ALLOC_H_
#define _DUBHE_SRAM_ALLOC_H_

#include "pal.h"

typedef struct _virt_addr_mgr_attr_t {
    uint32_t algn;      /* 0 if no algn req */
    bool is_multi_thrd; /* true if supporting multi-threading */
} virt_addr_mgr_attr_t;

typedef void *virt_addr_mgr_t;

virt_addr_mgr_t virt_addr_mgr_create_handle( void *base,
                                             uint32_t size,
                                             virt_addr_mgr_attr_t *attr );
void virt_addr_mgr_destroy_handle( virt_addr_mgr_t handle );

void *virt_addr_mgr_alloc_vaddr( virt_addr_mgr_t handle, uint32_t size );
void virt_addr_mgr_free_vaddr( virt_addr_mgr_t handle, void *vaddr );

#if CONFIG_DBG_VIRT_ADDR_MGR
void virt_addr_mgr_dump( virt_addr_mgr_t handle );
#endif /* CONFIG_DBG_VIRT_ADDR_MGR */

#endif /* _DUBHE_SRAM_ALLOC_H_ */
/*************************** The End Of File*****************************/
