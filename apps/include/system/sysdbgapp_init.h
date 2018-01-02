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
#ifndef __APPS_INCLUDE_SYSTEM_SYSDBGAPP_INIT_H
#define __APPS_INCLUDE_SYSTEM_SYSDBGAPP_INIT_H

/***************************************************************************
 * Included Files
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
#ifdef CONFIG_DEBUG_SYSTEM_APP
extern void sysdbgapp_init(void);
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __APPS_INCLUDE_SYSTEM_SYSDBGAPP_INIT_H */
