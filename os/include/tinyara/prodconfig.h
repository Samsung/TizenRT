/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

#ifndef __INCLUDE_TINYARA_PRODCONFIG_H
#define __INCLUDE_TINYARA_PRODCONFIG_H

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define PRODCONFIG_DRVPATH     "/dev/prodconfig"

#define DOWNLOAD_ENABLED      (1 << 2)
#define SWD_ENABLED           (1 << 1)
#define SWD_DOWNLOAD_DISABLED 0

int up_check_prodswd(void);
int up_check_proddownload(void);
void prodconfig_register(void);

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __INCLUDE_TINYARA_PRODCONFIG_H */
