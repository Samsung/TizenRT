/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <debug.h>
#include <errno.h>
#include <sys/types.h>
#include <messaging/messaging.h>
#include "messaging_internal.h"

/****************************************************************************
 * private functions
 ****************************************************************************/
/****************************************************************************
 * messaging_multicast
 ****************************************************************************/
int messaging_multicast(const char *port_name, msg_send_data_t *send_data)
{
	int ret;
	if (port_name == NULL) {
		msgdbg("[Messaging] multicast send fail : no port name.\n");
		return ERROR;
	}

	if (send_data == NULL || send_data->msg == NULL || send_data->msglen <= 0 || send_data->priority < 0) {
		msgdbg("[Messaging] multicast send fail : invalid param of send data.\n");
		return ERROR;
	}

	ret = messaging_send_internal(port_name, MSG_SEND_MULTI, send_data, NULL, NULL);
	if (ret == ERROR) {
		return ERROR;
	}

	return ret;
}
