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
/****************************************************************************
 * drivers/input/ist415.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#include <tinyara/i2c.h>

struct ist415_config_s;
struct ist415_ops_s {
	CODE void (*irq_enable)(struct ist415_config_s *dev);	/* Enables the irq */
	CODE void (*irq_disable)(struct ist415_config_s *dev);	/* Disables the irq */
	CODE void (*power_off)(struct ist415_config_s *dev);	/* low the reset */
	CODE void (*power_on)(struct ist415_config_s *dev);	/* High the reset */
};

/* IST415 Device */
struct ist415_config_s {

	void (*handler)(struct ist415_config_s *dev);

	const struct ist415_ops_s *ops;

	void *upper;				/* Used by the ist415 common upper structure */
	void *priv;					/* Used by the chipset-specific logic */

};

int ist415_initialize(const char *path, struct i2c_dev_s *i2c, struct ist415_config_s *conifg);
