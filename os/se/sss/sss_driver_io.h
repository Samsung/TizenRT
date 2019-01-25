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

#ifndef DRIVER_IO_H_
#define DRIVER_IO_H_

#ifndef SSS_OVER_MTDFTL
#define SSS_OVER_MTDFTL
#endif

char *sss_get_flash_device_name(void);

int sss_ro_read(unsigned int start_offset, unsigned char *buf, unsigned int byte_size);
int sss_ro_write(unsigned int start_offset, unsigned char *buf, unsigned int byte_size);

#endif							/* DRIVER_IO_H_ */
