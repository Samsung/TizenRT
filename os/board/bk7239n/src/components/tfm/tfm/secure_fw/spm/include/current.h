/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __CURRENT_H__
#define __CURRENT_H__

#include "thread.h"
#include "spm_ipc.h"

/* Get current component */
#define GET_CURRENT_COMPONENT()  GET_CTX_OWNER(CURRENT_THREAD->p_context_ctrl)
/* Set current component */
#define SET_CURRENT_COMPONENT(p) THRD_UPDATE_CUR_CTXCTRL(&(p)->ctx_ctrl)

#endif /* __CURRENT_H__ */
