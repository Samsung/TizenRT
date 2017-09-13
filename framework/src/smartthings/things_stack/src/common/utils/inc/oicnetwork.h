/*
 * oicnetwork.h
 *
 *  Created on: 2016. 10. 25.
 *      Author: eunseok
 */

#ifndef OICNETWORK_H_
#define OICNETWORK_H_

#include "cacommon.h"
#include "thingsapi.h"

#define MAX_ESSID            (36)
#define MAX_SECUIRTYKEY      (64)
#define MAX_TYPE_AUTH        (16)
#define MAX_TYPE_ENC         (16)
#define MAX_CHANNEL           (8)
#define MAX_LEVEL_SIGNAL     (12)
#define MAX_BSSID            (20)
	
typedef struct AccessPointInfo1{
	char ESSID[MAX_ESSID];	// mandatory
	char SecurityKey[MAX_SECUIRTYKEY];	// mandatory
	char AuthType[MAX_TYPE_AUTH];	// mandatory (None | WEP | WPA-PSK | WPA2-PSK)
	char EncType[MAX_TYPE_ENC];	// mandatory (WEP-64 | WEP-128 | TKIP | AES | TKIP_AES)
	char Channel[MAX_CHANNEL];	// optional
	char SignalLevel[MAX_LEVEL_SIGNAL];	// optional
	char BSSID[MAX_BSSID];	// optional
} AccessPointInfo1;

bool OICIsConnectedAP(void);

int OICIsNetAPIInitialize(void);

void OICWifiStateChangedCB_init(void);

int OICWifiStateChangedCB(int state, char* ap_name, char* ip_addr);

void OICAdapterStateCB(CATransportAdapter_t adapter, bool enabled);

void OICTCPsessStateCB(const CAEndpoint_t * info, bool connected);

int OICSetAPConnection( AccessPointInfo1* APinfo );

int OICGetAPList(AccessPointInfo1*** pInfo, int* pCount);

typedef int (*OICSetAPConnectionFuncType)(AccessPointInfo1* pInfo, char* pCmdId);
void OICRegisterSetAPConFunc(OICSetAPConnectionFuncType func);

typedef int (*OICGetAPSearchListFuncType)(AccessPointInfo1*** pInfo, int* pCount);
void OICRegisterGetAPListFunc(OICGetAPSearchListFuncType func);

/*
 * If Easy-Setup is Reseted, then need to recover about Current Connected-AP info.
 * This Function serve to recover Connected-AP info with Customized-method.
 */
void OICApplyCurrentAP(void);

#endif /* OICNETWORK_H_ */
