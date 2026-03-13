/**
  ******************************************************************************
  * @file    autoconf_eap.h
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef AUTOCONF_EAP_H
#define AUTOCONF_EAP_H

#define CONFIG_EAP

/************************ For EAP auth configurations *************************/
/* DO NOT change the below config of EAP */
#ifdef CONFIG_EAP
#define CONFIG_TLS  1
#define CONFIG_PEAP 1
#define CONFIG_TTLS 1
#define EAP_TTLS_MSCHAPv2 1
#define EAP_MSCHAPv2 1
#else  /* default config */
#define CONFIG_TLS  0
#define CONFIG_PEAP 0
#define CONFIG_TTLS 0
#endif /* CONFIG_EAP */

/*******************************************************************************/

/**
 * For EAP Method Configurations
 */

// on/off specified eap method
#define CONFIG_ENABLE_PEAP	1
#define CONFIG_ENABLE_TLS	1
#define CONFIG_ENABLE_TTLS	1

// optional feature: whether to verify the cert of radius server
#define ENABLE_EAP_SSL_VERIFY_SERVER	0

#if CONFIG_ENABLE_PEAP || CONFIG_ENABLE_TLS || CONFIG_ENABLE_TTLS
#define CONFIG_ENABLE_EAP
#endif

#if CONFIG_ENABLE_TLS
#define ENABLE_EAP_SSL_VERIFY_CLIENT	1
#else
#define ENABLE_EAP_SSL_VERIFY_CLIENT	0
#endif
/*******************************************************************************/

/************************ End of EAP configurations ***************************/

#endif /* #ifndef AUTOCONF_EAP_H */

