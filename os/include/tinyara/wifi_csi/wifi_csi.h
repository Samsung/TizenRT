/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
#ifndef __INCLUDE_TINYARA_WIFI_CSI_H
#define __INCLUDE_TINYARA_WIFI_CSI_H

#include <tinyara/config.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>

#define CSIIOC_PARAM_SETZERO         _CSIIOC(1)
#define CSIIOC_SET_CONFIG            _CSIIOC(2)
#define CSIIOC_REGISTER_CALLBACK     _CSIIOC(3)
#define CSIIOC_UNREGISTER_CALLBACK   _CSIIOC(4)
#define CSIIOC_GET_MAC_ADDR          _CSIIOC(5)


int wifi_csi_register(FAR const char *name);

#endif  /* __INCLUDE_TINYARA_WIFI_CSI_H */
