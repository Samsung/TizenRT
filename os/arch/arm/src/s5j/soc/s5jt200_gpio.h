/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
#ifndef __ARCH_ARM_SRC_S5J_SOC_S5JT200_GPIO_H
#define __ARCH_ARM_SRC_S5J_SOC_S5JT200_GPIO_H

enum {
	GPP0,
	GPP1,
	GPP2,
	GPP3,
	GPG0,
	GPG1,
	GPG2,
	GPG3,
	GPA0,
	GPA1,
	GPA2,
	GPA3,
	GPP4,
	ETC0,

	GPEND,
};

enum {
	GPIO_GROUP_ALIVE_EACH,
	GPIO_GROUP_ALIVE,
	GPIO_GROUP_COMMON,
};

#define CONFIG_MAX_GPIO_PORT 8

#endif							// __ARCH_ARM_SRC_S5J_SOC_S5JT200_GPIO_H
