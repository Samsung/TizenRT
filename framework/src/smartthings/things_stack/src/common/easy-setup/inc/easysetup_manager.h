#define _POSIX_C_SOURCE 200809L

#ifndef _EASYSETUP_MANAGER_H_
#define _EASYSETUP_MANAGER_H_

#include "thingsapi.h"
#include "escommon.h"
#include "oicserverbuilder.h"

typedef enum
{
    ESM_OK = 0,
    ESM_ERROR,
    ESM_PARAM_INVAIND,
    ESM_MODULE_STARTED,
    ESM_MODULE_TERMINATED,
    ESM_REQ_DENIED
} ESMResult;

typedef void (*PINGeneratedFuncType)(char* pinData, size_t pinSize);

typedef int  (*PINCloseFuncType)(void);

typedef int  (*UserConfirmResultFuncType)(void);

int ESMSetDevicePropertyByApp(char* name, const WIFI_MODE* mode, int ea_mode, const WIFI_FREQ freq);

ESMResult ESMInitEasySetup(int restartFlag, OICServerBuilder* serverBuilder);

ESMResult ESMTerminateEasySetup(void);

ESMResult ESMSetWifiConnErr(void);

bool      ESMGetNetworkStatus(void);

void*     ESMRegisterCloudCb(void *);

void      ESMRegisterUpdateDevProvDataFunc(OICUpdateDevProvDataFuncType func);

int       ESMRegisterPINGeneratedCB(PINGeneratedFuncType func);

int       ESMRegisterPINCloseCB(PINCloseFuncType func);

int       ESMRegisterUserConfirmCB(UserConfirmResultFuncType func);

int       ESMWifiProvCheckCB(int enabled, char* ssid, char* addr);


#endif  // _EASYSETUP_MANAGER_H_
