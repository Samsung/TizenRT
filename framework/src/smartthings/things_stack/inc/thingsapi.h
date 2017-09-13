//#******************************************************************
//#
//# Copyright 2016 SAMSUNG ELECTRONICS All Rights Reserved.
//#
//#******************************************************************

/**
 * @file thingsapi.h
 * This file contains the list of apis of the OCF Server Stack for DA.
 */

#ifndef _THINGSAPI_H_
#define _THINGSAPI_H_

#include <stdint.h>
#include "oiceasysetup.h"
#include "oicresource.h"


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


    //**********************************************************//
    //                                                          //
    //              OIC API :  DATA MODEL(STRUCTURE)            //
    //                                                          //
    //**********************************************************//

    #define MAX_DEVICEID_LEN 8
    #define MAX_RESOURCE_LEN 128
    #define MAX_QUERY_LEN 100
    #define MAX_ATTRIBUTE_LEN 64
    #define MAX_VALUETYPE_LEN 32
    #define MAX_ATTRVALUE_LEN 512

    #define MAX_KEY_LENGTH    50
    #define MAX_IT_CNT        3
    #define MAX_RT_CNT        2

    /**
        * @brief Data structure for interfacting with DA SW
        */
    typedef struct OICInfo
    {
        char deviceId[MAX_DEVICEID_LEN];
        char resource[MAX_RESOURCE_LEN];
        char query[MAX_QUERY_LEN];
        char attribute[MAX_ATTRIBUTE_LEN];
        char type[MAX_VALUETYPE_LEN];
        char value[MAX_ATTRVALUE_LEN];
        char id[MAX_DEVICEID_LEN];

        void* requestHandle;
        void* resourceHandle;

    } OICInfo;

    typedef struct st_attribute {
        char key[MAX_KEY_LENGTH];
        int type;
        bool mandatory;
        int rw;
    } st_attribute;

    typedef struct st_resource {   
        char uri                   [MAX_RESOURCE_LEN];
        char* interfaceTypes       [MAX_IT_CNT];
        char* resourceTypes        [MAX_RT_CNT];
    
        int ifcnt;
        int rtcnt;
        int policy;
    } st_resource;

    #define TYPE_INT_64 "int64"
    #define TYPE_BOOL   "boolean"
    #define TYPE_DOUBLE "double"
    #define TYPE_STRING "string"
    #define TYPE_BYTE    "byte"

    #define MAX_IPADDR           (16)

    #define OIC_UUID_STRING_SIZE (37)


    /**
        * @brief Data Structure definition for the Cloud Connection API
        */
    typedef struct {
        char* domain;           // mandatory ( You can choose one in both "domain" and "ip". )
        char* ip;
        char* port;             // optional
        char* auth_provider;    // mandatory
        char* access_token;     // mandatory
        char* access_token_type;// optional
        char* refresh_token;    // mandatory
        char* user_id;          // mandatory
        char* client_id;        // mandatory
    } OICCloudInfo;

    //**********************************************************//
    //                                                          //
    //                   OIC API :  FUNCTIONS                   //
    //                                                          //
    //**********************************************************//

    /**
        * @brief : Geter of the OIC DA Stack SW version
        * @return : version string of the this stack ex) OIC_X.X.X.d.MM.DD.HH
        */
    const char * OICGetSWVersion();

    /**
        * @brief : Geter of the OIC Device ID
        * @return : UUID Ver.4 based device ID. ex) 167baa8a-855d-407c-8f08-5abf07f7be7d
        */
    const char * OICGetDeviceIDString();

    /**
        * @brief             : Seter of the OIC Device Name with Device Number(Stack should be initialized)
        * @param devNum      : Device Number for Supporting Multi-Model(Hosting Device == 0)
        * @param name        : Device Name
        * @return            : 1 (success), 0(failed)
        */
    int OICSetDeviceName(int devNum, char * name);

    int OICInitializeStack(const char* jsonPath, bool* easysetupCompleted);
    int OICDeinitializeStack();

    /**
        * @brief : Initiate OIC DA stack
        * @param resource_Path    : OCF Resources File Path
        * @param info_Path        : OCF (Device) Info File Path (Device Profile)
        * @param cloud_Path       : File Path to be saved Easy-Setup data. (need RW-permission)
        * @return : 1(Success), 0(Fail)
        */
    int OICStartStack();

    /**
        * @brief : Retreive Result of Stack-reset
        * @param result   : Result of Stack-reset (1 : success, 0 : failure)
        */
    typedef void (*OICResetResultFuncType) (int result);

    /**
        * @brief : This Function called to trigger UI for User-Opinion with regard to Reset-Starting.
        * @brief : This is non-Blocking function.
        * @param funcCarrier : If you want to receive async-result of reset, then add function-pointer to funcCarrier.
        * @param resetType   : It indicate which type option added to reset-process.
        * @return            : 1 (Success trigger UI), 0 (Occur error with UI)
        */
    typedef int (*OICResetConfirmFuncType) ( OICResetResultFuncType* funcCarrier, ESEnrolleeReset resetType);

    /**
    * @brief : CB registration function for Reset-Confirmation function type.
    * @param OICResetConfirmFuncType : Refernce of the CB function
    * @return : 1 (success), 0 (failed)
    */
    int OICRegisterConfirmResetStartFunc( OICResetConfirmFuncType func);

    /**
    * @brief : Call Function for sending User-Opinion from DA-App to THINGS_STACK.
    * @param bResetStart : User Opinion about Reset-Starting. (1 : allow Reset, 0 : not allow Reset)
    * @return : 1 (start reset), 0 (discard reset)
    */
    int OICReturnUserOpinion4Reset(int bResetStart);

    /**
        * @brief : Reset all the (Security Related) data being used in the stack
        * @brief : Application(Resource Server) can be Setup again after this call
        * @brief : Will be deprecated
        * @param remoteOwner : If User trigger reset using remote-client like mobile, then need value(OicResource*) of "remoteOwner". \n
        *                      If User trigger reset using built-in menu with device, then insert NULL value in remoteOwner.
        *        resetType : It indicate which type option added to reset-process.
        * @return : 1(Reset-trigger Success), 0(Reset-trigger Fail)
        */
    int OICReset(void* remoteOwner, ESEnrolleeReset resetType);

    /**
        * @brief : Remove all the data being used internally & release all the
                memory allocated for this stack
                OICStartStack should be called to initiate stack.
        * @return : 1(Success), 0(Fail)
        */
    int OICStop(void);

    /**
    * @brief : Retreive Request Information
    * @param pResource   : Target resource
    */
    typedef int (*OICHandleRequestFuncType) (struct OicResource* pResource); // jun

    /**
    * @brief : CB registration function for Retreive Device Status Information function type
    * @param OICHandleRequestFuncType : Refernce of the CB function
    * @param list : List of Target Resource URIs for receiving CB when GET/POST delivered
    * @param listCnt : Cnt of Resource URI List
    * @return : 1 (success), 0 (failed)
    */
    int OICRegisterHandleRequestFunc(OICHandleRequestFuncType getFunc, OICHandleRequestFuncType setFunc); // jun

    /**
        * @brief : DevConfiguration Provisioning Data Update Function
        * @param pDevProvData    : pointer of DevConf Provisioning Data
        */
    typedef int (*OICUpdateDevProvDataFuncType)(ESDevConfProvData* pDevProvData);

    /**
    * @brief : CB registration function for DevConfiguration Provisioning Data Update Function type
    * @param OICUpdateDevProvDataFuncType : Refernce of the CB function
    * @return : 1 (success), 0 (failed)
    */
    int OICRegisterUpdateDevProvDataFunc(OICUpdateDevProvDataFuncType func);


    /**
        * @brief : CB for Soft AP stop notification
        * @param value : true(stop soft AP), false(ignore)
        */
    typedef int (*OICStopSoftAPFuncType)(int value);

    /**
    * @brief : CB registration function for Stopping Soft AP
    * @param OICStopSoftAPFuncType : Refernce of the CB function
    * @return : 1 (success), 0 (failed)
    */
    int OICRegisterStopSoftAPFunc(OICStopSoftAPFuncType func);


    #if 0
    /**
    * @brief Send out notification to those observers
                Observe response + Notification Service + Cloud Push Notification
    * @param uri   : Resource uri of which value(s) status has changed
    * @param query : Extra information of the target resource, ex) uri : /temperatures/0, query : id=0 (freezer)
    * @param OICNotiMessageInfo : NotificationMsgInfo
    * @param OCRepPayload : Reply Payload Data
    * @return : 1 (success), 0 (failed)
    */
    int OICNotificationForPush(const char* uri,
                                const char* query,
                                OICNotiMessageInfo* msgInfo,
                                OCRepPayload* convertedData);
    #endif

    int OICNotifyObservers(const char* uri);

    int OICSendPushMessage(const char* push_uri, void* payload);

    /**
    * @brief : WiFi AP State Change Call-Back
    * @param int : State of AP Connection. 1(connected), 0(disconnected)
    * @param char* : AP SSID
    * @param char* : IP Address (IPv4)
    * @return : 1 (success), 0 (failed)
    */
    int OICWifiChangedCallFunc(int state, char* ap_name, char* ip_addr);

    /**
    * @brief : Callback for getting randomly generated PIN
    * @param pinData * : PIN data in string format
    * @param pinSizse * : Length of the PIN String
    */
    typedef void (*OICPINGeneratedFuncType)(char* pinData, size_t pinSize);

    /**
    * @brief : CB registration function for getting randomly generated PIN
    *          for the PIN-Based Ownership Transfer Request
    * @return : 1 (success), 0 (failed)
    */
    int OICRegisterPINGeneratedFunc(OICPINGeneratedFuncType func);

    /**
    * @brief : Callback for closing PIN display
    */
    typedef void (*OICPINDisplayCloseFuncType)(void);

    /**
    * @brief : CB registration function for closing PIN display
    * @return : 1 (success), 0 (failed)
    */
    int OICRegisterPINDisplayCloseFunc(OICPINDisplayCloseFuncType func);


    //jun
    // int OICRegisterHandleRequestCallback(HandleGetRequestCallback onGetFunc, HandleSetRequestCallback onSetFunc);

    /**
    * @brief : Callback for getting users confirmation
    * @return : 1 (Confirmed), otherwise (Denied)
    */
    typedef int (*OICGetUserConfirmFuncType)(void);

    /**
    * @brief : CB registration function for getting user confirmation
    *          for MUTUAL VERIFICATION BASED JUST WORK Ownership transfer
    * @return : 1 (success), 0 (failed)
    */
    int OICRegisterUserConfirmFunc(OICGetUserConfirmFuncType func);

    /**
    * @brief : Set Device name & WiFi mode/frequency.
    * @param : name : pointer of Device Name.
    * @param : mode : pointer of WIFI_MODE array.
    * @param : ea_mode : count of element in mode array.
    * @param : freq : WIFI_FREQ value.
    * @return : 1 (success), 0 (failed)
    */
    int OICSetDeviceProperty(const char* name, const WIFI_MODE* mode, const int ea_mode, const WIFI_FREQ freq);

    /**
    * @brief : Get Device easy-setup state.
    * @return : 1 (easy-setup done), 0 (need easy-setup)
    */
    int OICGetEasySetupState(void);

    /**
    * @brief : Callback for getting Easy Setup State
    */
    typedef void (*OICGetEasySetupStateFuncType)(ESEnrolleeState state);

    /**
    * @brief : CB registration function for getting Easy Setup State
    * @return : 1 (success), 0 (failed)
    */
    int OICRegisterEasySetupStateFunc(OICGetEasySetupStateFuncType func);


    /**
    * @brief : Callback for notifing result of Cloud-Connection
    * @param result   : Result of Cloud-Connection (1 : success, 0 : failure)
    * @param domain   : If result is success, then describe domain which connected CI.
    * @param access_token : If result is success, then describe allowed accessToken with connected CI.
    */
    typedef void (*OICCloudConResultFuncType) (int result, char* domain, char* access_token);

    /**
    * @brief : Call function for Cloud-Connection with regard to CIinfo variables.
    * @param CIinfo : Cloud information for Cloud Connection.
    * @param func   : Call-Back Function pointer is called when finish Cloud connection.
    * @return : 1 (success), 0 (failed), -1 (Try again, Now is Reset Processing)
    */
    int OICCloudSignUP(OICCloudInfo* CIinfo, OICCloudConResultFuncType func);

    /**
    * @brief : Call function for trigerring Sign-IN with Cloud-Data in ocf_cloud.json.
    * @param func   : Call-Back Function pointer is called when finish Cloud connection.
    * @return : 1 (success), 0 (failed), -1 (Try again, Now is Reset Processing)
    */
    int OICCloudSignIN( OICCloudConResultFuncType func );

    /**
    * @brief : Call function for triggerring Sign-OUT when Cloud-Connected.
    */
    void OICCloudSignOUT( void );

    /**
    * @brief : Callback for aborting Easy Setup process
    * @param level : 1 (before Reset Confirm), 2 (before Security Confirm), 3(after Security Confirm)
    */
    typedef void (*OICAbortEasySetupFuncType)(ESEnrolleeAbort level);

    /**
    * @brief : CB registration function for aborting Easy Setup process.
    * @return : 1 (success), 0 (failed)
    */
    void OICRegisterEasySetupAbortFunc( OICAbortEasySetupFuncType func );

    /**
    * @brief : Callback function to recevie the OTM event on server side.
    *
    * @param addr : PT's address (address can be NULL in case of init state)
    * @param port : PT's port (port can be zero in case of init state & BLE)
    * @param uuid : PT's UUID (UUID can be NULL in case of init state & coap reqest)
    * @param event : OTM state (@ref OicSecOtmEvent_t)
    *                           0 (OTM Ready),
    *                           1 (OTM Started),
    *                           2 (OTM Done)
    *                           3 (OTM Error)
    */
    typedef void (*OICSecOTMStateFuncType)(const char* addr, uint16_t port,
                                            const char* uuid, int event);

    /**
    * @brief : API to register event handler to receive the Security Ownership Transfer state change
    * @param otmEventCallback : Implementation of OTM state event handler (@ref OICSecOTMStateFuncType)
    */
    int OICRegisterOTMEventHandler(OICSecOTMStateFuncType otmEventCallback);

    /**
    * Function to save the ACL
    *
    * @param uuid                 UUID of controller or wildcard("*")
    *                             If uuid is a wildcard("*"), all devices can access to this resource.
    * @param resourceUri          resource URI to be added into access control list.
    * @param resourceType         Type name of resource
    * @param interfaceName        interface name of resource (oic.if.baseline will be added internally.)
    * @param permission           Access permission value
    *               Access policy in least significant bits (from Spec):
    *                   b00000001  (1):  CREATE
    *                   b00000010  (2):  READ
    *                   b00000100  (4):  WRITE
    *                   b00001000  (8):  DELETE
    *                   b00010000 (16):  NOTIFY
    *                   b00011111 (31):  Full permission
    * @ret 0 is success, otherwise fail.
    */
    int OICSaveACL(const char* uuid, const char* resourceUri, const char* resourceType, const char* interfaceName, uint16_t permission);

    /**
    * API to add preconfigured PIN for MOT
    *
    * @param[in] preconfPin string type of preconfigured PIN
    *
    * @ret 0 is success, otherwise fail.
    */
    int OICSavePreconfiguredPin(const char* pin);

    /**
    * Function to set a MOT status
    *
    * @param enable whether the MOT is enabled. (true=enable, false=disable)
    *
    * @ret 0 is success, otherwise fail.
    */
    int OICSetMOTStatus(bool enable);

    /**
    * @brief : get deviceType
    * @return : deviceType
    */
    char* OICGetDeviceType(void);
     
    /**
    * @brief : get deviceName
    * @return : deviceName
    */
    char* OICGetDeviceName(void);

    /**
    * @brief : get resourceType
    * @param : resource_uri
    * @param : count
    * @param : resource_types
    * @return : resourceType
    */
    int OICGetResourceType(const char* resource_uri, int* count, char*** resource_types);

    /**
    * @brief : get deviceId
    * @return : deviceId
    */
    char* OICGetServerUUID(void);

    /**
    * @brief : get attributes by resource type
    * @param : res_type
    * @param : count
    * @param : attributes
    * @return : 0 on success and 1 on failure
    */
    int OICGetAttributesByResourceType(const char *res_type, int *count, st_attribute ***attributes);

    /**
    * @brief : check whether the given resource is collection resource or not
    * @param : res_uri
    * @return : true if the given resource is collection ( else false )
    */
    bool OICIsCollectionResource(const char *res_uri);

    /**
    * @brief : get child resources
    * @param : col_res_uri
    * @param : count
    * @param : child_resources
    * @return : 0 on success and 1 on failure
    */
    int OICGetChildResources(const char *col_res_uri, int *count, st_resource ***child_resources);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _THINGSAPI_H_
