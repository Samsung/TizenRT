// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_typedef.h>
#include "legacy/drv_model_pub.h"

#define DRV_FAILURE                  ((UINT32)-5)
#define DRV_SUCCESS                  (0)

#define DD_HANDLE_UNVALID            ((UINT32)-1)
#define DD_ID_UNVALID                ((UINT32)-1)

#define sddev_register_dev(handle, ops)  ddev_register_dev(handle, ops)
#define sddev_unregister_dev(handle)     ddev_unregister_dev(handle)

typedef struct _dd_operations_
{
    UINT32 (*open) (UINT32 op_flag);
    UINT32 (*close) (void);
    UINT32 (*read) (char *user_buf, UINT32 count, UINT32 op_flag);
    UINT32 (*write) (char *user_buf, UINT32 count, UINT32 op_flag);
    UINT32 (*control) (UINT32 cmd, void *parm);
} DD_OPERATIONS;

typedef enum _DD_OPEN_METHOD_
{
    DD_OPEN_METHOD_ONE_TIME = 0,	// open one time only
    DD_OPEN_METHOD_MUTI_TIME	    // open multi times
} DD_OPEN_METHOD;

 
/*******************************************************************************
* Function Declarations
*******************************************************************************/
UINT32 drv_model_init(void);
UINT32 drv_model_uninit(void);
DD_HANDLE ddev_open(dd_device_type dev, UINT32 *status, UINT32 op_flag);
UINT32 ddev_close(DD_HANDLE handle);
UINT32 ddev_read(DD_HANDLE handle, char *user_buf , UINT32 count, UINT32 op_flag);
UINT32 ddev_write(DD_HANDLE handle, char *user_buf , UINT32 count, UINT32 op_flag);
UINT32 ddev_register_dev(DD_HANDLE handle, DD_OPERATIONS *optr);
UINT32 ddev_unregister_dev(DD_HANDLE handle);

#ifdef __cplusplus
}
#endif
