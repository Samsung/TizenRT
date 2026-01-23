/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/serial/serial.h>
#include <debug.h>

/****************************************************************************
 * Name: up_flush_console
 *
 * Description:
 *    This function is used to flush all characters in the console UART TX buffer
 *    during abort situations.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void up_flush_console(void)
{
	uart_dev_t *dev = (uart_dev_t *)up_get_console_dev();

	if (dev == NULL) {
		return;
	}

	lldbg_noarg("Flush Console:\n");
	while (dev->xmit.head != dev->xmit.tail) {
		up_lowputc(dev->xmit.buffer[dev->xmit.tail]);

		if (++(dev->xmit.tail) >= dev->xmit.size) {
			dev->xmit.tail = 0;
		}
	}
	lldbg_noarg("\n\n");
}
