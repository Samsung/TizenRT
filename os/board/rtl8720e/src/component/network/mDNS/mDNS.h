 /**
  ******************************************************************************
  * @file    mDNS.h
  * @author
  * @version
  * @brief   This file provides user interface for mdns.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ****************************************************************************** 
  */

/*
 * The majority of the source code in the mDNS library (lib_mdns.a) is from 
 * mDNSResponder project, which is licensed under the terms of the Apache License, 
 * Version 2.0, available from:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _MDNS_H
#define _MDNS_H

/** @addtogroup mdns       MDNS
 *  @ingroup    network
 *  @brief      mdns functions
 *  @{
 */

#include <stdint.h>

/* Text Record */
typedef struct _TXTRecordRef_t {
	char PrivateData[16];
} TXTRecordRef;

extern void TXTRecordCreate(TXTRecordRef *txtRecord, uint16_t bufferLen, void *buffer);
extern int TXTRecordSetValue(TXTRecordRef *txtRecord, const char *key, uint8_t valueSize, const void *value);
extern void TXTRecordDeallocate(TXTRecordRef *txtRecord);

/* mDNS */
typedef void *DNSServiceRef;

extern int mDNSResponderInit(void);
extern void mDNSResponderDeinit(void);

/**
 * @brief     This function is used to register mdns service.
 * @param     name          : The service name of mdns
 * @param     service_type  : The service type of mdns
 * @param     domain        : The domain of mdns
 * @param     port          : The port of mdns
 * @param     txtRecord     : The txt record of mdns
 * @return    NULL          : Register failed, return NULL
              DNSServiceRef : Register success, return the pointer of the mdns service structure.
 */
extern DNSServiceRef mDNSRegisterService(char *name, char *service_type, char *domain, unsigned short port, TXTRecordRef *txtRecord);

/**
 * @brief     This function is used to unregister mdns service.
 * @param     serviceRef   : the pointer of the mdns service structure.
 * @return    none
 */
extern void mDNSDeregisterService(DNSServiceRef serviceRef);

/**
 * @brief     This function is used to update mdns service.
 * @param     serviceRef   : the pointer of the mdns service structure.
 * @param     txtRecord    : The txt record of mdns
 * @param     ttl          : The ttl of mdns, set to 0 for not changing ttl.
 * @return    none
 */
extern void mDNSUpdateService(DNSServiceRef serviceRef, TXTRecordRef *txtRecord, unsigned int ttl);

/*\@}*/

#endif  /* _MDNS_H */
