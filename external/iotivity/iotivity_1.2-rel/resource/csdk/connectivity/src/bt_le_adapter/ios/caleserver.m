/******************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "caleserver.h"
#include "caleutils.h"
#include "caleinterface.h"
#include "caadapterutils.h"

#include "logger.h"
#include "oic_malloc.h"
#include "cathreadpool.h"
#include "octhread.h"
#include "uarraylist.h"

#define TAG PCF("OIC_CA_LE_SERVER")

#define WAIT_TIME_WRITE_CHARACTERISTIC 10000000

static CAPacketReceiveCallback g_packetReceiveCallback = NULL;
static CABLEErrorHandleCallback g_serverErrorCallback;

static u_arraylist_t *g_connectedDeviceList = NULL;

static bool g_isStartServer = false;
static bool g_isInitializedServer = false;

static CABLEDataReceivedCallback g_CABLEServerDataReceivedCallback = NULL;
static oc_mutex g_bleReqRespCbMutex = NULL;
static oc_mutex g_bleClientBDAddressMutex = NULL;
static oc_mutex g_connectedDeviceListMutex = NULL;

static bool g_isSignalSetFlag = false;

static oc_cond g_transmitQueueCond = NULL;
static oc_mutex g_transmitQueueMutex = NULL;

static bool g_isStartAdvertising = false;
static bool isEnableGattServer = true;


static id cbPeripheralIf;
@interface CBPeripheralIfClass:NSObject <CBPeripheralManagerDelegate>
@end

NSMutableArray *peripheralMgrs;
static CBMutableCharacteristic *readCharacteristic = nil;
static CBMutableCharacteristic *writeCharacteristic = nil;
NSMutableDictionary *cbCentrals;
NSMutableDictionary *cbCentralsConnInfo;

@implementation CBPeripheralIfClass
static CBPeripheralManager *peripheralMgr = nil;

- (void) createPeripheralManager{
    OIC_LOG(DEBUG, TAG, "[CBPrl] create peripheral manager");
    peripheralMgr = [[CBPeripheralManager alloc] initWithDelegate:self
                    queue:nil options:nil];
    peripheralMgrs = [NSMutableArray new];
    cbCentrals = [[NSMutableDictionary alloc] init];
    cbCentralsConnInfo = [[NSMutableDictionary alloc] init];
}

- (void) setCharacteristic{
    OIC_LOG(DEBUG, TAG, "[CBPrl] set Characteristic");
    readCharacteristic = [[CBMutableCharacteristic alloc]
            initWithType:[CBUUID UUIDWithString:
                [[NSString alloc] initWithCString:OIC_GATT_CHARACTERISTIC_RESPONSE_UUID]]
            properties:CBCharacteristicPropertyNotify | CBCharacteristicPropertyRead
            value: nil
            permissions:CBAttributePermissionsReadable];
    writeCharacteristic = [[CBMutableCharacteristic alloc]
            initWithType:[CBUUID UUIDWithString:
                [[NSString alloc] initWithCString:OIC_GATT_CHARACTERISTIC_REQUEST_UUID]]
            properties: CBCharacteristicPropertyWrite | CBCharacteristicPropertyWriteWithoutResponse
            value:nil
            permissions:CBAttributePermissionsWriteable];
}

- (CBMutableService *) setService{
    OIC_LOG(DEBUG, TAG, "PeripheralInterface: set Service");
    CBMutableService *leService = [[CBMutableService alloc]
            initWithType: [CBUUID UUIDWithString:
                           [[NSString alloc] initWithCString:OIC_GATT_SERVICE_UUID]]
            primary:YES];
    return leService;
}

- (void) addService: (CBMutableService *)leService: (CBPeripheralManager *)peripheral{
    OIC_LOG(DEBUG, TAG, "PeripheralInterface: add Service");
    leService.characteristics = @[readCharacteristic, writeCharacteristic];
    [peripheral addService:leService];
}

- (void) startAdvertising: (CBPeripheralManager *)peripheral{
    [self setCharacteristic];
    [self addService:[self setService]:peripheral];

    NSString *beaconName = [NSString stringWithString:@"IoTivity Service"];

    if (!g_isStartAdvertising){
        OIC_LOG(DEBUG, TAG, "PeripheralInterface: startAdvertising 1");
        [peripheral startAdvertising:@{
            CBAdvertisementDataLocalNameKey: beaconName,
            CBAdvertisementDataServiceUUIDsKey:
            @[[CBUUID UUIDWithString: [[NSString alloc] initWithCString:OIC_GATT_SERVICE_UUID]]]}
        ];
        g_isStartAdvertising = true;
    }
}

- (void) stopAdvertising{
    OIC_LOG(DEBUG, TAG, "PeripheralInterface: stopAdvertising");
    [peripheralMgr stopAdvertising];
}

- (CAResult_t) sendData: (CBCentral *) connCentral: (NSData *)data{
    if (peripheralMgr != nil){
        if (connCentral != nil){
            NSLog(@"central: %@", connCentral);
            OIC_LOG(DEBUG, TAG,"senddata updateValue");
            NSUInteger tLen = [data length];
            uint8_t *tData = (uint8_t *)malloc(tLen);
            memcpy(tData, [data bytes], tLen);
            OIC_LOG_BUFFER(DEBUG, TAG, tData, tLen);
            free(tData);
            bool result = [peripheralMgr updateValue: data
                    forCharacteristic:readCharacteristic
                    onSubscribedCentrals:@[connCentral]];
            if (result){
                OIC_LOG(DEBUG, TAG, "CBCI: sending success");
                return CA_STATUS_OK;
            }else{
                OIC_LOG(ERROR, TAG, "CBCI: transmit queue is full! --> wait");

                oc_mutex_lock(g_transmitQueueMutex);
                oc_cond_wait(g_transmitQueueCond, g_transmitQueueMutex);
                oc_mutex_unlock(g_transmitQueueMutex);

                [peripheralMgr updateValue: data
                        forCharacteristic:readCharacteristic
                        onSubscribedCentrals:@[connCentral]];
                return CA_STATUS_OK;
            }
        }else{
            OIC_LOG(ERROR, TAG, "CBCI: connectedCentral ERROR");
            return CA_STATUS_FAILED;
        }
    }
    return CA_STATUS_FAILED;
}

- (NSString *) createCentralUUID{
    CFUUIDRef theUUID = CFUUIDCreate(NULL);
    CFStringRef uuidString = CFUUIDCreateString(NULL, theUUID);
    NSString *uuid = (NSString *)CFStringCreateCopy(NULL, uuidString);
    CFRelease(theUUID);
    CFRelease(uuidString);
    return [uuid autorelease];
}

#pragma mark - CBPeripheralManagerDelegate

- (void) peripheralManagerDidUpdateState:(CBPeripheralManager *)peripheral{
    if (peripheral.state == CBPeripheralManagerStatePoweredOn){
        OIC_LOG(DEBUG, TAG, "PeripheralInterface: update state --> POWERED ON");
        peripheralMgr = peripheral;
        [peripheralMgrs addObject:peripheralMgr];

        CALEServerSetFlagBTAdapter(true);

        if (!g_isStartAdvertising){
            CALEServerStartAdvertise();
        }
    }else {
        OIC_LOG(DEBUG, TAG, "PeripheralInterface: update state --> ! powered on");
        CBPeripheralManager *tPM;
        NSUInteger index = 0;
        [peripheralMgrs removeObject: peripheral];

        CALEServerSetFlagBTAdapter(false);

        return;
    }
}

- (void)peripheralManager:(CBPeripheralManager *)peripheral
        didAddService:(CBService *)service error:(NSError *)error{
    NSLog(@"peripheralManager:%@ didAddService:%@", peripheral, service);
    if (error){
        NSLog(@"peripheralManager:%@ didAddService:%@ error:%@",
                peripheral, service, [error localizedDescription]);
    }
}

- (void)peripheralManagerDidStartAdvertising:(CBPeripheralManager *)peripheral
        error:(NSError *)error{
    if ([[error localizedDescription] isEqualToString:@"null"]
            || [[error localizedDescription] isEqualToString:@"<null>"]
            || [[error localizedDescription] isEqualToString:@"(null)"]
            || [[error localizedDescription] length] != 0){

        NSLog(@"peripheralManagerDidStartAdvertising peripheral:%@ error_1:%@",
                peripheral, [error localizedDescription]);
    }
}

- (void)peripheralManager: (CBPeripheralManager *)peripheral
        didReceiveReadRequest:(CBATTRequest *)request{
    NSLog(@"didReceiveReadRequest : %@", request);
}

- (void)peripheralManager:(CBPeripheralManager *)peripheral
        central:(CBCentral *)central
        didSubscribeToCharacteristic:(CBCharacteristic *)characteristic{
    OIC_LOG(DEBUG, TAG, "PeripheralInterface: connected");
    NSLog(@"connected characteristic %@", characteristic);

    bool hasCentral = false;
    for (CBCentral *key in cbCentrals){
        if ([key isEqual:central]){
            hasCentral = true;
            char *tuuid = (char*)[[cbCentrals objectForKey:key] UTF8String];
            NSLog(@"%s : %@ : %@", tuuid, [cbCentrals objectForKey:key], key);
        }
    }

    if (!hasCentral){
        NSLog(@"set Central Object %@", cbCentrals);
        [cbCentrals setObject:[cbPeripheralIf createCentralUUID] forKey:central];
    }

    const char *address = [CALEServerGetAddressFromGattObj(central) UTF8String];
    if (false == CALEServerIsDeviceInList(address)){
        NSLog(@"add connected Central obj to cache: \n%@ : \n%s", central, address);
        CALEServerAddDeviceToList(central);
    }

    [cbCentralsConnInfo setObject:[NSNumber numberWithInteger:(int)STATE_CONNECTED] forKey:central];
}

- (void)peripheralManager:(CBPeripheralManager *)peripheral
        central:(CBCentral *)central
        didUnsubscribeFromCharacteristic:(CBCharacteristic *)characteristic{
    OIC_LOG(DEBUG, TAG, "PeripheralInterface: disconnected");

    const char *address = [CALEServerGetAddressFromGattObj(central) UTF8String];
    if (true == CALEServerIsDeviceInList(address)){
        NSLog(@"remove connected Central obj to cache: \n%@ : \n%s", central, address);
        CALEServerRemoveDevice(address);
    }

    [cbCentralsConnInfo setObject:[NSNumber numberWithInteger:
                                   (int)STATE_DISCONNECTED] forKey:central];
}

- (void)peripheralManager:(CBPeripheralManager *)peripheral
        didReceiveWriteRequests: (NSArray *)requests{
    NSMutableData *nsData = [[NSMutableData alloc] init];
    for (CBATTRequest *request in requests){
        if ([request.characteristic.UUID isEqual:
                [CBUUID UUIDWithString: [[NSString alloc] initWithCString:
                    OIC_GATT_CHARACTERISTIC_REQUEST_UUID]]]){
            NSLog(@"Received data from central %@", request.central);

            const char* address = [CALEServerGetAddressFromGattObj(request.central) UTF8String];
            if (!address){
                OIC_LOG(ERROR, TAG, "address is not available");
                return;
            }

            NSLog(@"------------- received data (size:%lu)----------------\n%@",
                    request.value.length, request.value);
            [nsData appendData:request.value];
            uint32_t len = [nsData length];
            uint8_t *data = (uint8_t*)OICMalloc(len);
            uint32_t sentLength = 0;

            memcpy(data, (const uint8_t*)[nsData bytes], len);

            OIC_LOG_BUFFER(DEBUG, TAG, data, len);

            oc_mutex_lock(g_bleClientBDAddressMutex);
            g_CABLEServerDataReceivedCallback(address, data, len, &sentLength);
            oc_mutex_unlock(g_bleClientBDAddressMutex);
        }
    }
}

- (void)peripheralManagerIsReadyToUpdateSubscribers:(CBPeripheralManager *) peripheral{
    oc_mutex_lock(g_transmitQueueMutex);
    OIC_LOG(DEBUG, TAG, "transmit queue is ready to update subscribers");
    oc_cond_signal(g_transmitQueueCond);
    oc_mutex_unlock(g_transmitQueueMutex);
}

@end


/**
 * get the current connection state of the gatt profile to the remote device.
 * @param[in]   device           bluetooth device object
 * @return  state of the profile connection.
 */
static int CALEServerGetConnectionState(CBCentral *central)
{
    OIC_LOG(DEBUG, TAG, "CALEServerGetConnectionState");

    int connState = [[cbCentralsConnInfo objectKey:central] intValue];
    OIC_LOG_V(INFO, TAG, "connection state is %d", connState);
    return connState;
}

CAResult_t CALEStartAdvertise()
{
    CAResult_t ret = CALEServerStartAdvertise();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerStartAdvertise has failed");
    }

    return ret;
}

CAResult_t CALEServerStartAdvertise()
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerStartAdvertise");

    if (!CALEServerIsEnableBTAdapter())
    {
        OIC_LOG(ERROR, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    if ([peripheralMgrs count] != 0 && caglobals.serverFlags){
        [cbPeripheralIf startAdvertising: peripheralMgrs[0]];
    }else{
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, TAG, "Advertising started!!");

    OIC_LOG(DEBUG, TAG, "OUT - CALEServerStartAdvertise");
    return CA_STATUS_OK;

}

CAResult_t CALEServerStopAdvertise()
{
    OIC_LOG(DEBUG, TAG, "LEServerStopAdvertise");

    if (!CALEServerIsEnableBTAdapter())
    {
        OIC_LOG(ERROR, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    [cbPeripheralIf stopAdvertising];

    OIC_LOG(DEBUG, TAG, "Advertising stopped!!");
    return CA_STATUS_OK;
}

CAResult_t CALEServerStartGattServer()
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerStartGattServer");

    if (!CALEServerIsEnableBTAdapter())
    {
        OIC_LOG(ERROR, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    if (g_isStartServer)
    {
        OIC_LOG(DEBUG, TAG, "Gatt server already started");
    }

    OIC_LOG(DEBUG,TAG, "OUT - CALEServerStartGattServer");
    return CA_STATUS_OK;
}

CAResult_t CALEServerSend(CBCentral *central, const uint8_t *data, const uint32_t dataLen)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerSend");

    if (!CALEServerIsEnableBTAdapter())
    {
        OIC_LOG(ERROR, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    NSData *nsData = [NSData dataWithBytes:(const uint8_t*)data length:dataLen];
    NSLog(@"send data: %@, size:%lu", nsData, [nsData length]);

    CAResult_t result = [cbPeripheralIf sendData: central: nsData];

    if (CA_STATUS_OK != result)
    {
        OIC_LOG(ERROR, TAG, "Fail to send response data");
        return result;
    }

    OIC_LOG(DEBUG, TAG, "OUT - CALEServerSend");
    return result;
}

CAResult_t CALEServerInitialize()
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerInitialize");

    cbPeripheralIf = [[CBPeripheralIfClass alloc] init];
    [cbPeripheralIf createPeripheralManager];

    CAResult_t ret = CA_STATUS_FAILED;
    g_transmitQueueCond = oc_cond_new();

    ret = CALEServerInitMutexVaraibles();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerInitMutexVaraibles has failed");

        return CA_STATUS_FAILED;
    }
    CALEServerCreateCachedDeviceList();

    g_isInitializedServer = true;
    OIC_LOG(DEBUG, TAG, "OUT - CALEServerInitialize");
    return CA_STATUS_OK;
}

void CALEServerTerminate()
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerTerminate");

    CALEServerTerminateMutexVaraibles();
    CALEServerTerminateConditionVaraibles();

    g_isInitializedServer = false;

    oc_cond_free(g_transmitQueueCond);
    g_transmitQueueCond = NULL;

    OIC_LOG(DEBUG, TAG, "OUT - CALEServerTerminate");
}

CAResult_t CALEServerSendUnicastMessage(const char* address, const uint8_t* data, uint32_t dataLen)
{
    OIC_LOG_V(DEBUG, TAG, "CALEServerSendUnicastMessage(%s, %p)", address, data);
    VERIFY_NON_NULL(address, TAG, "address is null");
    VERIFY_NON_NULL(data, TAG, "data is null");

    CAResult_t ret = CALEServerSendUnicastMessageImpl(address, data, dataLen);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerSendUnicastMessageImpl has failed");
    }

    return ret;
}

CAResult_t CALEServerSendMulticastMessage(const uint8_t* data, uint32_t dataLen)
{
    OIC_LOG_V(DEBUG, TAG, "CALEServerSendMulticastMessage(%p)", data);
    VERIFY_NON_NULL(data, TAG, "data is null");

    CAResult_t ret = CALEServerSendMulticastMessageImpl(data, dataLen);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerSendMulticastMessageImpl has failed");
    }

    return ret;
}

CAResult_t CALEServerStartMulticastServer()
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerStartMulticastServer");

    if (!g_isInitializedServer)
    {
        OIC_LOG(INFO, TAG, "server is not initialized");
        return CA_STATUS_FAILED;
    }

    if (g_isStartServer)
    {
        OIC_LOG(INFO, TAG, "server is already started..it will be skipped");
        return CA_STATUS_FAILED;
    }

    g_isStartServer = true;

    CAResult_t ret = CALEServerStartGattServer();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "Fail to start gatt server");
        return ret;
    }

    if(!CALEServerIsEnableBTAdapter())
    {
        OIC_LOG(DEBUG, TAG, "Cannot start advertise. Not powered on yet.");
        return ret;
    }
    if(!g_isStartAdvertising)
        ret = CALEServerStartAdvertise();
    else
    {
        OIC_LOG(DEBUG, TAG, "skipped cause already advertising.");
    }
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerStartAdvertise has failed");
    }

    OIC_LOG(DEBUG, TAG, "OUT - CALEServerStartMulticastServer");
    return ret;
}

CAResult_t CALEServerStopMulticastServer()
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerStopMulticastServer");

    if (false == g_isStartServer)
    {
        OIC_LOG(INFO, TAG, "server is already stopped..it will be skipped");
        return CA_STATUS_FAILED;
    }

    CAResult_t ret = CALEServerStopAdvertise();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerStopAdvertise has failed");
    }

    g_isStartServer = false;

    OIC_LOG(DEBUG, TAG, "OUT - CALEServerStopMulticastServer");
    return ret;
}

void CALEServerSetCallback(CAPacketReceiveCallback callback)
{
    OIC_LOG(DEBUG, TAG, "CALEServerSetCallback");
    g_packetReceiveCallback = callback;
}

CAResult_t CALEServerSendUnicastMessageImpl(const char* address, const uint8_t* data,
                                            uint32_t dataLen)
{
    OIC_LOG_V(DEBUG, TAG, "CALEServerSendUnicastMessageImpl, address: %s, data: %p, size: %u",
            address, data, dataLen);
    OIC_LOG_BUFFER(DEBUG, TAG, data, dataLen);
    VERIFY_NON_NULL(address, TAG, "address is null");
    VERIFY_NON_NULL(data, TAG, "data is null");

    if (!g_connectedDeviceList)
    {
        OIC_LOG(ERROR, TAG, "g_connectedDeviceList is null");
        return CA_STATUS_FAILED;
    }


    uint32_t length = u_arraylist_length(g_connectedDeviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        OIC_LOG(DEBUG, TAG, "check device address");
        CBCentral *tCentral = (CBCentral *) u_arraylist_get(g_connectedDeviceList, index);
        if (!tCentral)
        {
            OIC_LOG(ERROR, TAG, "tCentral is null");
            return CA_SEND_FAILED;
        }

        const char *setAddress = [CALEServerGetAddressFromGattObj(tCentral) UTF8String];
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "setAddress is null");
            return CA_SEND_FAILED;
        }

        OIC_LOG_V(DEBUG, TAG, "setAddress : %s", setAddress);
        OIC_LOG_V(DEBUG, TAG, "address : %s", address);

        if (!strcmp(setAddress, address))
        {
            OIC_LOG_V(DEBUG, TAG, "found the device: %s", setAddress);

            CAResult_t res = CALEServerSend(tCentral, data, dataLen);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "send has failed");
                return CA_SEND_FAILED;
            }
            break;
        }
    }

    OIC_LOG(INFO, TAG, "unicast - send request is successful");
    return CA_STATUS_OK;
}

CAResult_t CALEServerSendMulticastMessageImpl(const uint8_t *data, uint32_t dataLen)
{
    OIC_LOG_V(DEBUG, TAG, "CALEServerSendMulticastMessageImpl, send to, data: %s", data);
    VERIFY_NON_NULL(data, TAG, "data is null");

    if (!g_connectedDeviceList)
    {
        OIC_LOG(ERROR, TAG, "g_connectedDeviceList is null");
        return CA_STATUS_FAILED;
    }

    uint32_t length = u_arraylist_length(g_connectedDeviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        CBCentral *tCentral = (CBCentral *) u_arraylist_get(g_connectedDeviceList, index);
        if (!tCentral)
        {
            OIC_LOG(ERROR, TAG, "central is null");
            continue;
        }

        const char *addr = [CALEServerGetAddressFromGattObj(tCentral) UTF8String];
        CAResult_t res = CALEServerSend(tCentral, data, dataLen);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG_V(ERROR, TAG, "Send failed: %s", addr);
        }

        OIC_LOG_V(INFO, TAG, "unicast - send request is successful for a device[%s]", addr);
    }

    return CA_STATUS_OK;
}

void CALEServerCreateCachedDeviceList()
{
    oc_mutex_lock(g_connectedDeviceListMutex);
    if (!g_connectedDeviceList)
    {
        OIC_LOG(DEBUG, TAG, "Create device list");
        g_connectedDeviceList = u_arraylist_create();
    }
    oc_mutex_unlock(g_connectedDeviceListMutex);
}

bool CALEServerIsDeviceInList(const char* remoteAddress)
{
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress is null", false);

    if (!g_connectedDeviceList)
    {
        OIC_LOG(ERROR, TAG, "list is null");
        return false;
    }

    uint32_t length = u_arraylist_length(g_connectedDeviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        CBCentral *central = (CBCentral *) u_arraylist_get(g_connectedDeviceList, index);

        if (!central)
        {
            OIC_LOG(ERROR, TAG, "connectedDevice is not available");
            return false;
        }

        const char* setAddress = [CALEServerGetAddressFromGattObj(central) UTF8String];
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "setAddress is null");
            return false;
        }

        if (!strcmp(remoteAddress, setAddress))
        {
            OIC_LOG(ERROR, TAG, "the device is already set");
            return true;
        }
    }

    OIC_LOG(DEBUG, TAG, "there are no device in the list");
    return false;
}

CAResult_t CALEServerAddDeviceToList(CBCentral *central)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerAddDeviceToList");
    VERIFY_NON_NULL(central, TAG, "central is null");

    oc_mutex_lock(g_connectedDeviceListMutex);

    if (!g_connectedDeviceList)
    {
        OIC_LOG(ERROR, TAG, "list is null");
        oc_mutex_unlock(g_connectedDeviceListMutex);
        return CA_STATUS_FAILED;
    }

    const char *remoteAddress = [CALEServerGetAddressFromGattObj(central) UTF8String];
    if (!remoteAddress)
    {
        OIC_LOG(ERROR, TAG, "remoteAddress is null");
        oc_mutex_unlock(g_connectedDeviceListMutex);
        return CA_STATUS_FAILED;
    }

    if (false == CALEServerIsDeviceInList(remoteAddress))
    {
        u_arraylist_add(g_connectedDeviceList, central);
        OIC_LOG_V(DEBUG, TAG, "Set the object to ArrayList as Element : %s", remoteAddress);
    }

    oc_mutex_unlock(g_connectedDeviceListMutex);
    OIC_LOG(DEBUG, TAG, "OUT - CALEServerAddDeviceToList");
    return CA_STATUS_OK;
}

CAResult_t CALEServerRemoveAllDevices()
{
    OIC_LOG(DEBUG, TAG, "IN - CALEServerRemoveAllDevices");

    oc_mutex_lock(g_connectedDeviceListMutex);
    if (!g_connectedDeviceList)
    {
        OIC_LOG(ERROR, TAG, "g_connectedDeviceList is null");
        oc_mutex_unlock(g_connectedDeviceListMutex);
        return CA_STATUS_FAILED;
    }

    uint32_t length = u_arraylist_length(g_connectedDeviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        CBCentral *central = (CBCentral *)u_arraylist_get(g_connectedDeviceList, index);
        [cbCentrals removeObjectForKey: central];
        if (NULL == u_arraylist_remove(g_connectedDeviceList, index)){
            oc_mutex_unlock(g_connectedDeviceListMutex);
            return CA_STATUS_FAILED;
        }
    }

    OICFree(g_connectedDeviceList);
    g_connectedDeviceList = NULL;
    oc_mutex_unlock(g_connectedDeviceListMutex);

    OIC_LOG(DEBUG, TAG, "OUT - CALEServerRemoveAllDevices");
    return CA_STATUS_OK;
}

CAResult_t CALEServerRemoveDevice(const char *remoteAddress)
{
    OIC_LOG(DEBUG, TAG, "IN CALEServerRemoveDevice");

    oc_mutex_lock(g_connectedDeviceListMutex);
    if (!g_connectedDeviceList)
    {
        OIC_LOG(ERROR, TAG, "no deviceList");
        oc_mutex_unlock(g_connectedDeviceListMutex);
        return CA_STATUS_FAILED;
    }

    uint32_t length = u_arraylist_length(g_connectedDeviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        CBCentral *central = (CBCentral *)u_arraylist_get(g_connectedDeviceList, index);

        if (central)
        {
            const char* setAddress = [CALEServerGetAddressFromGattObj(central) UTF8String];
            if (!setAddress)
            {
                OIC_LOG(ERROR, TAG, "setAddress is null");
                continue;
            }

            if (!strcmp(setAddress, remoteAddress))
            {
                OIC_LOG_V(DEBUG, TAG, "device address : %s", remoteAddress);

                if (NULL == u_arraylist_remove(g_connectedDeviceList, index))
                {
                    OIC_LOG(ERROR, TAG, "List removal failed.");
                    oc_mutex_unlock(g_connectedDeviceListMutex);
                    return CA_STATUS_FAILED;
                }
                [cbCentrals removeObjectForKey: central];
                oc_mutex_unlock(g_connectedDeviceListMutex);
                return CA_STATUS_OK;
            }
        }
    }

    oc_mutex_unlock(g_connectedDeviceListMutex);

    OIC_LOG(DEBUG, TAG, "there are no device in the device list");

    OIC_LOG(DEBUG, TAG, "IN CALEServerRemoveDevice");
    return CA_STATUS_FAILED;
}


/**
 * adapter common
 */

CAResult_t CAStartLEGattServer()
{
    CALEServerStartMulticastServer();

    return CA_STATUS_OK;
}

CAResult_t CAStopLEGattServer()
{
    OIC_LOG(DEBUG, TAG, "CAStopLEGattServer");

    CAResult_t ret = CALEServerRemoveAllDevices();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEServerRemoveAllDevices has failed");
        return CA_STATUS_FAILED;
    }

    g_isStartServer = false;

    return CA_STATUS_OK;
}

CAResult_t CAInitializeLEGattServer()
{
    OIC_LOG(DEBUG, TAG, "Initialize Gatt Server");
    return CALEServerInitialize();
}

void CATerminateLEGattServer()
{
    OIC_LOG(DEBUG, TAG, "Terminate Gatt Server");
    CALEServerTerminate();
}

void CASetLEReqRespServerCallback(CABLEDataReceivedCallback callback)
{
    oc_mutex_lock(g_bleReqRespCbMutex);
    g_CABLEServerDataReceivedCallback = callback;
    oc_mutex_unlock(g_bleReqRespCbMutex);
}

void CASetBLEServerErrorHandleCallback(CABLEErrorHandleCallback callback)
{
    g_serverErrorCallback = callback;
}

CAResult_t CAUpdateCharacteristicsToGattClient(const char *address,
                                               const uint8_t *charValue,
                                               uint32_t charValueLen)
{
    CAResult_t result = CA_SEND_FAILED;
    VERIFY_NON_NULL(charValue, TAG, "charValue is null");

    if (address)
    {
        result = CALEServerSendUnicastMessage(address, charValue, charValueLen);
    }

    return result;
}

CAResult_t CAUpdateCharacteristicsToAllGattClients(const uint8_t *charValue,
                                                   uint32_t charValueLen)
{
    VERIFY_NON_NULL(charValue, TAG, "device is null");

    CAResult_t result = CALEServerSendMulticastMessage(charValue, charValueLen);

    return result;
}

void CASetLEServerThreadPoolHandle(ca_thread_pool_t handle)
{
    OIC_LOG(INFO, TAG, "CASetLEServerThreadPoolHandle is not support");
    (void)handle;
}

CAResult_t CALEServerInitMutexVaraibles()
{
    if (NULL == g_bleReqRespCbMutex)
    {
        g_bleReqRespCbMutex = oc_mutex_new();
        if (NULL == g_bleReqRespCbMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_bleClientBDAddressMutex)
    {
        g_bleClientBDAddressMutex = oc_mutex_new();
        if (NULL == g_bleClientBDAddressMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_connectedDeviceListMutex)
    {
        g_connectedDeviceListMutex = oc_mutex_new();
        if (NULL == g_connectedDeviceListMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_transmitQueueMutex){
        g_transmitQueueMutex = oc_mutex_new();
        if (NULL == g_transmitQueueMutex){
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    return CA_STATUS_OK;
}

void CALEServerTerminateMutexVaraibles()
{
    oc_mutex_free(g_bleReqRespCbMutex);
    g_bleReqRespCbMutex = NULL;

    oc_mutex_free(g_bleClientBDAddressMutex);
    g_bleClientBDAddressMutex = NULL;

    oc_mutex_free(g_connectedDeviceListMutex);
    g_connectedDeviceListMutex = NULL;

    oc_mutex_free(g_transmitQueueMutex);
    g_transmitQueueMutex = NULL;
}

void CALEServerTerminateConditionVaraibles()
{
    OIC_LOG(DEBUG, TAG, "this method is not supported");
}

NSString *CALEServerGetAddressFromGattObj(CBCentral *central){
    VERIFY_NON_NULL_RET(central, TAG, "central is null", NULL);
    NSString *address = [cbCentrals objectForKey: central];
    if (!address){
        OIC_LOG(ERROR, TAG, "address isn't available");
        return NULL;
    }
    return address;
}

void CALEServerSetFlagBTAdapter(bool state){
    isEnableGattServer = state;
}

bool CALEServerIsEnableBTAdapter(){
    return isEnableGattServer;
}
