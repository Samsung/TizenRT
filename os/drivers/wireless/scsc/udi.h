/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __SLSI_UDI_H__
#define __SLSI_UDI_H__

#include "dev.h"

#define SLSI_UDI_MINOR_NODES 2	/* Maximum number of nodes supported. */

int slsi_udi_node_init(struct slsi_dev *sdev);
int slsi_udi_node_deinit(struct slsi_dev *sdev);

int slsi_udi_init(void);
int slsi_udi_deinit(void);
int slsi_kernel_to_user_space_event(struct slsi_log_client *log_client, u16 event, u32 data_length, const u8 *data);
int slsi_check_cdev_refs(void);

#endif
