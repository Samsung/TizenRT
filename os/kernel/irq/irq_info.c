/****************************************************************************
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


/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include "irq/irq.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: irq_info
 *
 * Description:
 *   Display the registered IRQ information
 *
 ****************************************************************************/

void irq_info(void)
{
	uint16_t i;
	uint16_t j;

	printf(" %5s | %7s | %9s | %3s \n", "INDEX", "IRQ_NUM", "INT_COUNT", "ISR_NAME");
	printf("-------|---------|-----------|----------\n");
	for (i = 0, j = 0; i < NR_IRQS; i++) {
		if (g_irqvector[i].handler != irq_unexpected_isr) {
			j++;
			printf(" %5d | %7d | %9d | %s \n", j, i, g_irqvector[i].count, g_irqvector[i].irq_name);
		}
	}
}
