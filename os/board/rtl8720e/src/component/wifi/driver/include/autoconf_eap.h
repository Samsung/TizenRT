/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#ifndef AUTOCONF_EAP_H
#define AUTOCONF_EAP_H

/************************ For EAP auth configurations *************************/
/* DO NOT change the below config of EAP */
#ifdef PRE_CONFIG_EAP
#define CONFIG_TLS  1
#define CONFIG_PEAP 1
#define CONFIG_TTLS 1
#else  /* default config */
#define CONFIG_TLS  0
#define CONFIG_PEAP 0
#define CONFIG_TTLS 0
#endif /* PRE_CONFIG_EAP*/

#if CONFIG_TLS || CONFIG_PEAP || CONFIG_TTLS
#define EAP_REMOVE_UNUSED_CODE 1
#endif

#if CONFIG_TTLS
#define EAP_MSCHAPv2
#define EAP_TTLS_MSCHAPv2
//#define EAP_TTLS_EAP
//#define EAP_TTLS_MSCHAP
//#define EAP_TTLS_PAP
//#define EAP_TTLS_CHAP
#endif
/************************ End of EAP configurations ***************************/

#endif /* #ifndef AUTOCONF_EAP_H */

