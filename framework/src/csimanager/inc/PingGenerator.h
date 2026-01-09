/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#pragma once

#include "CSIManager.h"

/**
 * @brief Sets the Ping sending interval for the CSI service.
 *
 * @param[in] interval Data collection interval in ms.
 *
 */
void ping_generator_change_interval(unsigned int pingInterval);
CSIFW_RES csi_ping_generator_initialize(void);
CSIFW_RES csi_ping_generator_cleanup(void);
CSIFW_RES ping_generator_start(void);
CSIFW_RES ping_generator_stop(void);
