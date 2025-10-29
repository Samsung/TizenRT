// Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
// All rights reserved
//
// The content of this file or document is CONFIDENTIAL and PROPRIETARY
// to Arm Technology (China) Co., Ltd. It is subject to the terms of a
// License Agreement between Licensee and Arm Technology (China) Co., Ltd
// restricting among other things, the use, reproduction, distribution
// and transfer.  Each of the embodiments, including this information and,,
// any derivative work shall retain this copyright notice.

#ifndef __HAL_SECURE_DEBUG_H__
#define __HAL_SECURE_DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_common.h"

typedef struct _debug_cmd_t {
    uint32_t cmd0 : 1;
    uint32_t cmd1 : 1;
    uint32_t cmd2 : 1;
    uint32_t cmd3 : 1;
    uint32_t cmd4 : 1;
    uint32_t cmd5 : 1;
    uint32_t cmd6 : 1;
    uint32_t cmd7 : 1;
    uint32_t cmd8 : 1;
    uint32_t cmd9 : 1;
    uint32_t cmd10 : 1;
    uint32_t cmd11 : 1;
    uint32_t cmd12 : 1;
    uint32_t cmd13 : 1;
    uint32_t cmd14 : 1;
    uint32_t cmd15 : 1;
    uint32_t cmd16 : 1;
    uint32_t cmd17 : 1;
    uint32_t cmd18 : 1;
    uint32_t cmd19 : 1;
    uint32_t cmd20 : 1;
    uint32_t cmd21 : 1;
    uint32_t cmd22 : 1;
    uint32_t cmd23 : 1;
    uint32_t cmd24 : 1;
    uint32_t cmd25 : 1;
    uint32_t cmd26 : 1;
    uint32_t cmd27 : 1;
    uint32_t cmd28 : 1;
    uint32_t cmd29 : 1;
    uint32_t cmd30 : 1;
    uint32_t cmd31 : 1;
} debug_cmd_t;

HAL_API bool hal_secure_debug_is_enabled(void);
HAL_API hal_ret_t hal_secure_debug_excute_debug_cmd(debug_cmd_t cmds);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_SECURE_DEBUG_H__ */