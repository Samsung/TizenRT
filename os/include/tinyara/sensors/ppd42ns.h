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

#ifndef __PPD42NS_H__
#define __PPD42NS_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/
typedef void (*ppd42ns_handler_t)(FAR void *arg);

struct ppd42ns_config_s {
	/* gpio pin number */
	int gpionum;

	/* callback function: read gpio pin signal */
	int (*read_gpio)(struct ppd42ns_config_s *config);

	/* callback function: attach the ppd42ns interrupt handler to the GPIO interrupt */
	int (*attach)(struct ppd42ns_config_s *config, ppd42ns_handler_t handler, FAR char *arg);

	/* callback function: enable or disable gpio pin interrupt */
	int (*enable)(struct ppd42ns_config_s *config, int enable);

	/* board specific data */
	void *priv;
};

/****************************************************************************
 * Public Function
 ****************************************************************************/

/****************************************************************************
 * Name: ppd42ns_register
 *
 * Description:
 *  This function will register ppd42ns dust sensor driver as /dev/dustN where N
 *  is the minor device number
 *
 * Input Parameters:
 *   devname  - The full path to the driver to register. E.g., "/dev/dust0"
 *   config      - configuration for the ppd42ns driver.
 *
 * Returned Value:
 *   Zero is returned on success.  Otherwise, a negated errno value is
 *   returned to indicate the nature of the failure.
 *
 ****************************************************************************/
int ppd42ns_register(FAR const char *devname, FAR struct ppd42ns_config_s *config);

#endif							/* __PPD42NS_H__ */
