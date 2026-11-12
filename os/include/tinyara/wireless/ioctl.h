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

#ifndef __INCLUDE_TINYARA_WIRELESS_IOCTL_H
#define __INCLUDE_TINYARA_WIRELESS_IOCTL_H

/****************************************************************************/
/* Included Files
/****************************************************************************/

#include <tinyara/config.h>
#include <sys/ioctl.h>

/****************************************************************************/
/* Public Types
/****************************************************************************/

/* Wireless character driver IOCTL commands.  The wireless driver glue is
 * not part of the 12.13.0 base bring-up (TD-BUILD-02); this header exists
 * so that sys/ioctl.h can be included unconditionally by generic code when
 * CONFIG_DRIVERS_WIRELESS is selected.
 */

#endif							/* __INCLUDE_TINYARA_WIRELESS_IOCTL_H */
