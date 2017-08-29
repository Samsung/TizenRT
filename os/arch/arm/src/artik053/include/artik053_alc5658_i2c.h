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

typedef int (*alc_write)(uint16_t addr, uint16_t data);
typedef int (*alc_read)(uint16_t addr, uint16_t *data);
typedef int (*alc_modify)(uint16_t addr, uint16_t set, uint16_t clear);

typedef struct {
	alc_write write;
	alc_read read;
	alc_modify modify;
} i2c_alc;

i2c_alc *alc5658_i2c_initialize(void);
