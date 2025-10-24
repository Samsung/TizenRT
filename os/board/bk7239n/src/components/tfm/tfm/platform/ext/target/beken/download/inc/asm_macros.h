/*
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#ifndef __ASM_MACROS_H__
#define __ASM_MACROS_H__
/*
 * Macro to define default handlers. Default handler
 * will be weak symbol and just dead loops. They can be
 * overwritten by other handlers
 */
.macro    def_default_handler    handler_name
.align 1
.thumb_func
.weak    \handler_name
.type    \handler_name, %function
\handler_name :
b    .
.size    \handler_name, . - \handler_name
.endm

.macro    def_irq_default_handler    handler_name
.weak     \handler_name
.set      \handler_name, Default_Handler
.endm

#endif /* __ASM_MACROS_H__ */


