/******************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef __CSI_LOGS_DUMPER__
#define __CSI_LOGS_DUMPER__

#include "csifw.h"

#ifdef CONFIG_CSI_DATA_DUMP_OVER_NETWORK
#define CSI_DUMP_DATA_QUEUE_NAME "/dev/csidump_data"

typedef enum EVENT {
	STOP_DUMP,
	START_DUMP
} EVENT;

CSIFW_RES csi_logs_dumper_init();
CSIFW_RES csi_logs_dumper_deinit();

typedef void (*csiDataDumpListener)(EVENT event);
void set_event_listener(csiDataDumpListener listener);
#endif

#endif  /* __CSI_LOGS_DUMPER__ */

