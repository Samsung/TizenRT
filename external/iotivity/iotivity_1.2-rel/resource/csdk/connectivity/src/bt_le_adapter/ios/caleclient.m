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

#include "caleclient.h"
#include "caleserver.h"
#include "caleutils.h"
#include "caleinterface.h"
#include "caadapterutils.h"

#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "cathreadpool.h" /* for thread pool */
#include "octhread.h"
#include "uarraylist.h"

#define TAG PCF("OIC_CA_LE_CLIENT")

#define MICROSECS_PER_SEC 1000000
#define WAIT_TIME_WRITE_CHARACTERISTIC 10 * MICROSECS_PER_SEC

static ca_thread_pool_t g_threadPoolHandle = NULL;

static u_arraylist_t *g_deviceList = NULL; // device list to have same UUID
static u_arraylist_t *g_gattObjectList = NULL;
static u_arraylist_t *g_deviceStateList = NULL;

static CAPacketReceiveCallback g_packetReceiveCallback = NULL;
static CABLEErrorHandleCallback g_clientErrorCallback;

static CAManagerAdapterStateChangeCallback g_caManagerAdapterStateChangeCallback = NULL;
static CAManagerConnectionCallback g_caManagerConnectionCallback = NULL;
static CAManagerServiceDiscoveredCallback g_caManagerServiceDiscoveredCallback = NULL;
static dispatch_queue_t g_caManagerQueue = NULL;

static NSString *mac_ref = @"AB:CD:EF:15:";
int mac_add = 4096;

static bool g_isStartedLEClient = false;
static bool g_isStartedScan = false;

static NSData* g_sendBuffer = nil;
static uint32_t g_targetCnt = 0;
static uint32_t g_currentSentCnt = 0;
static bool g_isFinishedSendData = false;
static oc_mutex g_SendFinishMutex = NULL;
static oc_mutex g_threadMutex = NULL;
static oc_cond g_threadCond = NULL;
static oc_cond g_deviceDescCond = NULL;

static oc_mutex g_threadSendMutex = NULL;
static oc_mutex g_threadWriteCharacteristicMutex = NULL;
static oc_cond g_threadWriteCharacteristicCond = NULL;
static bool g_isSignalSetFlag = false;

static oc_mutex g_bleReqRespClientCbMutex = NULL;
static oc_mutex g_bleServerBDAddressMutex = NULL;

static oc_mutex g_deviceListMutex = NULL;
static oc_mutex g_gattObjectMutex = NULL;
static oc_mutex g_deviceStateListMutex = NULL;

static oc_mutex g_deviceScanRetryDelayMutex = NULL;
static oc_cond g_deviceScanRetryDelayCond = NULL;

static oc_mutex g_scanMutex = NULL;
static oc_mutex waitMutex = NULL;

static oc_mutex g_initializeMutex = NULL;
static oc_cond g_initializeCond = NULL;

static CABLEDataReceivedCallback g_CABLEClientDataReceivedCallback = NULL;

@interface CBCentralIfClass:NSObject <CBCentralManagerDelegate>
@end
static id cbCentralIf;
NSMutableArray *centralMgrs;
NSMutableDictionary *cbPeripherals;
NSMutableDictionary *cbServices;
NSMutableDictionary *requestCharacs;
NSMutableDictionary *responseCharacs;
NSArray<CBUUID *> *g_uuidList;

@implementation CBCentralIfClass
static CBCentralManager *centralMgr = nil;

- (void) CALEClientCreateCentralManager{
    centralMgr = [[CBCentralManager alloc] initWithDelegate:self queue:nil];

    centralMgrs = [NSMutableArray new];

    cbPeripherals = [[NSMutableDictionary alloc] init];
    cbServices = [[NSMutableDictionary alloc] init];
    requestCharacs = [[NSMutableDictionary alloc] init];
    responseCharacs = [[NSMutableDictionary alloc] init];
}

- (CAResult_t) CALEClientStartScanImpl{
    OIC_LOG(DEBUG, TAG, "[CBCtrl] In - CALEClientStartScanImpl");
    if ([centralMgrs count] == 0){
        OIC_LOG(ERROR, TAG, "[CBCtrl] central manager is not available");
        return CA_ADAPTER_NOT_ENABLED;
    }else{
        CALEClientSetScanFlag(true);

        NSArray <CBUUID *> *uuidList = [NSArray arrayWithObjects:
                [CBUUID UUIDWithString:[NSString stringWithUTF8String:OIC_GATT_SERVICE_UUID]],
                [CBUUID UUIDWithString:[NSString stringWithUTF8String:CUSTOM_UUID]],
                [CBUUID UUIDWithString:[NSString stringWithUTF8String:CUSTOM_UUID2]], nil];

        [centralMgr scanForPeripheralsWithServices:uuidList
            options: @{CBCentralManagerScanOptionAllowDuplicatesKey: @NO}];
    }
    return CA_STATUS_OK;
}

- (CAResult_t) CALEClientStartScanWithUUIDImpl: (NSArray<CBUUID*> *)uuids{
    OIC_LOG(DEBUG, TAG, "[CBCtrl] In - CALEClientStartScanWithUUIDImpl");
    if ([centralMgrs count] == 0){
        OIC_LOG(ERROR, TAG, "[CBCtrl] central manager is not available");
        return CA_ADAPTER_NOT_ENABLED;
    }else{
        CALEClientSetScanFlag(true);
        [centralMgr scanForPeripheralsWithServices:uuids
            options:@{CBCentralManagerScanOptionAllowDuplicatesKey: @YES}];
    }
    return CA_STATUS_OK;
}

- (CAResult_t) CALEClientStopScanImpl{
    OIC_LOG(DEBUG, TAG, "[CBCtrl] In - CALEClientStopScanImpl");
    if ([centralMgrs count] == 0){
        OIC_LOG(ERROR, TAG, "[CBCtrl] central manager is not available");
        return CA_ADAPTER_NOT_ENABLED;
    }else{
        CALEClientSetScanFlag(false);
        [centralMgr stopScan];
    }
    return CA_STATUS_OK;
}

- (CAResult_t) CALEClientConnectImpl: (CBPeripheral *)peripheral{
    if (peripheral == (id) [NSNull null]){
        OIC_LOG(ERROR, TAG, "[CBCtrl] In - CALEClientConnectImpl: peripheral is null");
    }else{
        OIC_LOG(DEBUG, TAG, "[CBCtrl] In - CALEClientConnectImpl");
#ifdef TB_LOG
        NSLog(@"peripheral: %@", peripheral);
#endif
        [centralMgr connectPeripheral: peripheral options:nil];
        return CA_STATUS_OK;
    }
    return CA_STATUS_FAILED;
}

- (CAResult_t) CALEClientDiscoverServices: (CBPeripheral *)peripheral{
    if (peripheral == (id) [NSNull null]){
        OIC_LOG(ERROR, TAG, "[CBCtrl] In - CALEClientDiscoverServices: peripheral is null");
    }else{
        OIC_LOG_V(DEBUG, TAG, "[CBCtrl] In - CALEClientDiscoverServices: %s",
                  [[cbPeripherals objectForKey:peripheral] UTF8String] );
#ifdef TB_LOG
        NSLog(@"peripheral: %@", peripheral);
#endif
        [peripheral discoverServices:
         @[[CBUUID UUIDWithString:[[NSString alloc] initWithCString:OIC_GATT_SERVICE_UUID]]]];
        return CA_STATUS_OK;
    }
    return CA_STATUS_FAILED;
}

- (CAResult_t) CALEClientDiscoverServiceWithUUID: (CBPeripheral *)peripheral
                                                 : (NSArray<CBUUID *> *)uuids{
    //  OIC_LOG(DEBUG, TAG, "DISCOVER SERVICE WITHUUID");
#ifdef TB_LOG
    NSLog(@"discover service withuuid to %@ (%@)", peripheral, uuids);
#endif
    [peripheral discoverServices:uuids];
}


-(CAResult_t) CALEClientSetNotifyCharacteristic: (CBPeripheral *)peripheral
                                               : (CBCharacteristic *)characteristic{
    if ((peripheral == (id) [NSNull null]) || (characteristic == (id) [NSNull null])){
        OIC_LOG(ERROR, TAG, "[CBCtrl] CALEClientSetNotifyCharacteristic: parameter is null");
    }else{
        OIC_LOG(DEBUG, TAG, "[CBCtrl] CALEClientSetNotifyCharacteristic");
        [peripheral setNotifyValue:YES forCharacteristic:characteristic];
        return CA_STATUS_OK;
    }
    return CA_STATUS_FAILED;
}

- (void) CALEClientCancelPeripheralConnection: (CBPeripheral *)peripheral{
    [centralMgr cancelPeripheralConnection:peripheral];
}

- (CAResult_t) CALEClientWriteValue: (CBPeripheral *)peripheral: (CBCharacteristic *)characteristic{
    OIC_LOG(DEBUG, TAG, "[CBCtrl] CALEClientWriteRequest");
    if ([characteristic.UUID isEqual: [CBUUID UUIDWithString: [[NSString alloc]
                initWithCString: OIC_GATT_CHARACTERISTIC_REQUEST_UUID]]]){
        if (g_sendBuffer == nil || g_sendBuffer == NULL || [g_sendBuffer length] <= 0){
            OIC_LOG(ERROR, TAG, "[CBCtrl] g_sendbuffer size is 0!!!");
            return CA_STATUS_FAILED;
        }else{
            NSUInteger len = [g_sendBuffer length];
            OIC_LOG_V(DEBUG, TAG, "[CBCtrl] write data: size(%lu)", len);
            uint8_t *data = (uint8_t*)malloc(len);
            memcpy(data, [g_sendBuffer bytes], len);
            OIC_LOG_BUFFER(DEBUG, TAG, data, len);
            free(data);
            [peripheral writeValue:g_sendBuffer
                 forCharacteristic:characteristic type:CBCharacteristicWriteWithResponse];
            return CA_STATUS_OK;
        }
    }else{
        OIC_LOG(ERROR, TAG, "[CBCtrl] requestUUID is different");
    }
    return CA_STATUS_FAILED;
}


- (NSString *) CALEClientGetMAC{

    NSString *Hex = [NSString stringWithFormat:@"0x%X", mac_add];
    NSString *Hex_add = [Hex substringFromIndex:2];
    NSString *first_add = [Hex_add substringToIndex:2];
    NSString *second_add = [Hex_add substringFromIndex:2];
    NSString *comma = @":";


    NSString *MAC;
    MAC = [NSString stringWithString:mac_ref];
    MAC = [MAC stringByAppendingString:first_add];
    MAC = [MAC stringByAppendingString:comma];
    MAC = [MAC stringByAppendingString:second_add];

    NSLog(@"MAC: %@", MAC);

    if(mac_add == 65535){
    mac_add=256;
    }else{
    mac_add++;
    }

    return MAC;
}

- (NSString *) CALEGetAddressFromBTDevice: (CBPeripheral *)peripheral{
    if (peripheral == (id) [NSNull null]){
        OIC_LOG(ERROR, TAG, "[CBCtrl] In - CALEGetAddressFromBTDevice: peripheral is null");
    }else{
        return [cbPeripherals objectForKey: peripheral];
    }
    return nil;
}

#pragma mark - CBCentralManagerDelegate
- (void)centralManagerDidUpdateState:(CBCentralManager *)central{
    switch(central.state){
    case CBCentralManagerStatePoweredOn:
        OIC_LOG(DEBUG, TAG, "CBCentralManager update state --> Powered ON");

        //if state callback is called before bt le client initialize done, wait signal.
        if (!g_isStartedLEClient) {
            OIC_LOG(INFO, TAG, "wait for finishing initialize..");

            if (NULL == g_initializeMutex)
            {
                g_initializeMutex = oc_mutex_new();
                if (NULL == g_initializeMutex)
                {
                    OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
                }
            }

            oc_mutex_lock(g_initializeMutex);
            oc_cond_wait(g_initializeCond, g_initializeMutex);
            oc_mutex_unlock(g_initializeMutex);
        }

        if (![centralMgrs containsObject: centralMgr]) {
            centralMgr = central;
            [centralMgrs addObject: centralMgr];
        }

        CALEClientSetFlagBTAdapter(true);

        CALEClientNWStateChangeCallback(CALE_STATE_ON);

        CALEClientStartScan();
        break;
    case CBCentralManagerStateResetting:
        OIC_LOG(DEBUG, TAG, "CBCentralManager update state --> StateResetting");
        break;
    case CBCentralManagerStateUnsupported:
    case CBCentralManagerStateUnauthorized:
    case CBCentralManagerStatePoweredOff:
    default:
        OIC_LOG(DEBUG, TAG, "CBCentralManager update state --> Not Available");
        CALEClientSetFlagBTAdapter(false);
        CALEClientResetDeviceStateForAll();

        CALEClientRemoveAllGattObjs();
        break;
    }

    if(g_caManagerQueue)
    {
        dispatch_async(g_caManagerQueue, ^{

            if(g_caManagerAdapterStateChangeCallback)
            {
                g_caManagerAdapterStateChangeCallback(central);
            }
        });
    }
}

- (void)centralManager:(CBCentralManager *)central
        didDiscoverPeripheral:(CBPeripheral *)peripheral
        advertisementData:(NSDictionary<NSString*, id> *) advertisementData
        RSSI:(NSNumber *) RSSI{
    OIC_LOG(DEBUG, TAG, "[CBCtrl] did discover peripheral");
    if (g_isStartedLEClient){
#ifdef TB_LOG
        NSLog(@"%@", peripheral);
#endif
        if ([cbPeripherals count] > 0){
            if ([cbPeripherals objectForKey:peripheral] == nil){//(NSString *)[NSNull null]){
                OIC_LOG(DEBUG, TAG, "new peripheral");
                [cbPeripherals setObject:[cbCentralIf CALEClientGetMAC] forKey:peripheral];
                CALEClientAddScanDeviceToList(peripheral);
            }
        }else{
            if (nil != cbCentralIf){
                OIC_LOG(DEBUG, TAG, "1st discovered peripheral");
                [cbPeripherals setObject:[cbCentralIf CALEClientGetMAC] forKey: peripheral];
                CALEClientAddScanDeviceToList(peripheral);
            }
        }
    }else{
        OIC_LOG(DEBUG, TAG, "[CBCtrl] g_isStertedLEClient == false");
    }
}

- (void)centralManager:(CBCentralManager*)central
        didConnectPeripheral:(CBPeripheral *)peripheral {
    peripheral.delegate = self;
    OIC_LOG(DEBUG, TAG, "[CBCtrl] connected peripheral");

    char *address = [[cbCentralIf CALEGetAddressFromBTDevice: peripheral] UTF8String];
    CALEState_t* curState = CALEClientGetStateInfo(address);
    if (curState == NULL || curState->connectedState != STATE_CONNECTED) {
        CALEClientUpdateDeviceState(address, STATE_CONNECTED,
                                    STATE_CHARACTER_UNSET,STATE_SEND_NONE);

        [cbCentralIf CALEClientDiscoverServices: peripheral];
    }

    if(g_caManagerQueue)
    {
        dispatch_async(g_caManagerQueue, ^{

            if(g_caManagerConnectionCallback)
            {
                g_caManagerConnectionCallback(peripheral, address, true, nil);
            }
        });
    }

}

- (void)centralManager: (CBCentralManager *)central
        didFailToConnectPeripheral: (CBPeripheral *)peripheral
        error:(NSError *)error{

    OIC_LOG(ERROR, TAG, "[CBCtrl] didFailToConnectPeripheral & start scan");
    if(error){
        OIC_LOG(ERROR, TAG, "didFailToConnectPeripheral");
#ifdef TB_LOG
        NSLog(@"%@", error);
#endif
    }

    CAResult_t res = CALEClientStartScan();
    if (CA_STATUS_OK != res){
        OIC_LOG(ERROR, TAG, "start scan error");
        CALEClientSendFinish(peripheral);
        return;
    }

     char *address = (char*)[[cbCentralIf CALEGetAddressFromBTDevice: peripheral] UTF8String];
     if (!address){
         CALEClientSendFinish(peripheral);
         return;
     }else{
         res = CALEClientRemoveDeviceState(address);
         if (CA_STATUS_OK != res){
             OIC_LOG(ERROR, TAG, "removedevicestate error");
             CALEClientSendFinish(peripheral);
             return;
         }
     }
}

- (void)centralManager:(CBCentralManager *)central
        didDisconnectPeripheral:(CBPeripheral *)peripheral
        error:(NSError *)error{
    if(error){
        OIC_LOG(ERROR, TAG, "didDisconnectPeripheral");
#ifdef TB_LOG
        NSLog(@"%@", error);
#endif
    }

    NSString *nsAddr = [cbPeripherals objectForKey:peripheral];
    const char *address = [nsAddr UTF8String];

    if(address){
        CALEClientUpdateDeviceState(address, STATE_DISCONNECTED,
                                    STATE_CHARACTER_UNSET,STATE_SEND_NONE);
        OIC_LOG_V(DEBUG, TAG, "[CBCtrl] did DISCONNECT Peripheral: %s", address);

        if(g_caManagerQueue)
        {
            dispatch_async(g_caManagerQueue, ^{

                if(g_caManagerConnectionCallback)
                {
                    g_caManagerConnectionCallback(peripheral, address, false, error);
                }
            });
        }

        CALEClientUpdateSendCnt();

        if ( false == CALEClientGetAutoConnectFlag(address) )
        {
            CALEClientRemoveDeviceInScanDeviceList(nsAddr);
            //CALEClientStartScan();
        }
    }
}

- (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error {
    if (error){
        OIC_LOG(ERROR, TAG, "[CBCtrl] error - didDiscoverServices");
#ifdef TB_LOG
        NSLog(@"%@", error);
#endif
        CALEClientSendFinish(peripheral);
        return;
    }
    const char *address = [CALEClientGetAddressFromGattObj(peripheral) UTF8String];
#ifdef TB_LOG
    NSLog(@"find service %s %@", address, peripheral);
#endif
    if (!address){
        CALEClientSendFinish(peripheral);
        return;
    }

    if (!CALEClientIsSetCharacteristic(address)){
        char *serviceUUID = NULL;

        for (CBService *service in peripheral.services){

            [cbServices setObject:service forKey:peripheral];

#ifdef TB_LOG
            NSLog(@"discover characteristics: %@", service);
#endif
            [peripheral discoverCharacteristics:@[
                [CBUUID UUIDWithString: [[NSString alloc] initWithCString:
                    OIC_GATT_CHARACTERISTIC_REQUEST_UUID]],
                [CBUUID UUIDWithString: [[NSString alloc] initWithCString:
                    OIC_GATT_CHARACTERISTIC_RESPONSE_UUID]]] forService:service];
        }
    }else{
        OIC_LOG(DEBUG, TAG, "already set characteristics");

        if ( g_sendBuffer != nil ) {
            CAResult_t res = CALEClientWriteCharacteristic(peripheral);
            if (CA_STATUS_OK != res){
                OIC_LOG(ERROR, TAG, "CALEClientWriteCharacteristic failed");
                CALEClientSendFinish(peripheral);
                return;
            }
        }

        if(g_caManagerQueue){
            OIC_LOG(DEBUG, TAG, "Call caManagerServiceDiscoveredCallback");
            dispatch_async(g_caManagerQueue, ^{

                if(g_caManagerServiceDiscoveredCallback)
                {
                    g_caManagerServiceDiscoveredCallback(peripheral, address, error);
                }
            });
        }
    }
}

- (void)peripheral: (CBPeripheral *)peripheral
        didDiscoverCharacteristicsForService: (CBService *)service
        error:(NSError *)error{

    if(error){
        OIC_LOG(ERROR, TAG, "[CBCtrl] didDiscoverCharacteristicsForService: error");
#ifdef TB_LOG
        NSLog(@"%@", error);
#endif
        return;
    }
    OIC_LOG(DEBUG, TAG, "[CBCtrl] didDiscoverCharacteristicsForService");
    const char *address = [[cbCentralIf CALEGetAddressFromBTDevice: peripheral] UTF8String];

    for (CBCharacteristic *characteristic in service.characteristics){
        if (!CALEClientIsSetCharacteristic(address)){
            if ([characteristic.UUID isEqual:
                 [CBUUID UUIDWithString:
                  [[NSString alloc] initWithCString:OIC_GATT_CHARACTERISTIC_RESPONSE_UUID]]]){
                OIC_LOG_V(DEBUG, TAG, "[CBCtrl] find RESPONSE characteristic @%s", address);

                CAResult_t res = CALEClientSetCharacteristicNotification(peripheral,
                                                                         characteristic);
                if (CA_STATUS_OK != res){
                    OIC_LOG(ERROR, TAG, "CALEClientSetCharacteristicNotification has failed");
                    CALEClientSendFinish(peripheral);
                    return;
                }
            }

            if ([characteristic.UUID isEqual:
                 [CBUUID UUIDWithString:
                  [[NSString alloc] initWithCString:OIC_GATT_CHARACTERISTIC_REQUEST_UUID]]]){
                OIC_LOG_V(DEBUG, TAG, "[CBCtrl] find REQUEST characteristic @%s", address);
                [requestCharacs setObject:characteristic forKey:peripheral];

                CAResult_t res = CALEClientUpdateDeviceState(address, STATE_CONNECTED,
                        STATE_CHARACTER_SET, STATE_SEND_NONE);

                if (CA_STATUS_OK != res){
                    OIC_LOG(ERROR, TAG, "CALEClientUpdataeDeviceState has failed");
                    CALEClientSendFinish(peripheral);
                    return;
                }

                res = CALEClientAddGattobjToList(peripheral);
                if (CA_STATUS_OK != res){
                    OIC_LOG(ERROR, TAG, "CALEClientAddGattobjToList has failed");
                    CALEClientSendFinish(peripheral);
                    return;
                }

                if ( g_sendBuffer != nil ) {
                    res = CALEClientWriteCharacteristic(peripheral);
                    if (CA_STATUS_OK != res){
                        OIC_LOG(ERROR, TAG, "CALEClientWriteCharacteristic has failed");
                        CALEClientSendFinish(peripheral);
                        return;
                    }
                }
            }
        }else{
            CAResult_t res = CALEClientWriteCharacteristic(peripheral);
            if (CA_STATUS_OK != res){
                OIC_LOG(ERROR, TAG, "CALEClientWriteCharacteristic has failed");
                CALEClientSendFinish(peripheral);
                return;
            }
        }
    }


    if(g_caManagerQueue)
    {
        OIC_LOG(DEBUG, TAG, "Call caManagerServiceDiscoveredCallback");
        dispatch_async(g_caManagerQueue, ^{

            if(g_caManagerServiceDiscoveredCallback)
            {
                g_caManagerServiceDiscoveredCallback(peripheral, address, error);
            }
        });
    }

}

- (void)peripheral: (CBPeripheral *)peripheral didUpdateNotificationStateForCharacteristic
                    :(CBCharacteristic *)characteristic error:(NSError *)error{
    if(error){
        OIC_LOG(ERROR, TAG, "didUpdateNotificationStateForCharacteristic error");
#ifdef TB_LOG
        NSLog(@"Error changing notification state: %@", [error localizedDescription]);
#endif
    }

    [responseCharacs setObject:characteristic forKey:peripheral];
}

- (void)peripheral: (CBPeripheral *)peripheral
    didUpdateValueForCharacteristic: (CBCharacteristic *)characteristic
    error:(NSError *)error{
        if(error){
            OIC_LOG(ERROR, TAG, "[CBCtrl] didUpdateValueForCharacteristic: ERROR");
#ifdef TB_LOG
            NSLog(@"%@", error);
#endif
            return;
        }

        const char *address = [[cbPeripherals objectForKey: peripheral] UTF8String];
        CBService *service = [responseCharacs objectForKey: characteristic];
#ifdef TB_LOG
        NSLog(@ "[CBctrl] Updata Value \n address: %s \n service: %@ \n==>Data ",
                address, service);
#endif

        NSMutableData *nsData = [[NSMutableData alloc] init];
        [nsData appendData:characteristic.value];
        uint32_t len = [nsData length];
        uint8_t *data = (uint8_t*) OICMalloc(len);
        uint32_t sentLength = 0;
        memcpy(data, (const uint8_t*)[nsData bytes], len);
        [nsData release];
        OIC_LOG_BUFFER(DEBUG, TAG, data, len);
        oc_mutex_lock(g_bleServerBDAddressMutex);
        g_CABLEClientDataReceivedCallback(address, data, len, &sentLength);
        oc_mutex_unlock(g_bleServerBDAddressMutex);
    }

@end

int ishexdigit(char var)
{
    if(var <= '9')
    {
        if(var >= '0') return 1;
        else           return 0;
    }
    else if(var <= 'F')
    {
        if(var >= 'A') return 1;
        else           return 0;
    }
    else if(var <= 'f')
    {
        if(var >= 'a') return 1;
        else           return 0;
    }

    return 0;
}

int isMAC(char *address)
{
    /*
    * Check if the provided MAC address is valid.
    * 1. The length of MAC address should always be 17.
    * 2. Hyphens are expected at positions {3, 6, 9, 12, 15}.
    * 3. The rest characters should be simple xdigits.
    */
    int hyphens[5] = {3, 6, 9, 12, 15};
    if (strlen(address) != 17)
    {
        return 0;//Oops. The lenth doesn't match.
    }

    for (int i = 0, counter = 0; i < 17; i ++)
    {
        char var = address[i];
        if (i == hyphens[counter] - 1)// Check if a hyphen is expected here.
        {
            // Yep. We need a hyphen here.
            if (var != ':')
            {
                return 0;// Oops. The character is not a hyphen.
            }
            else
            {
                counter++;// Move on to the next expected hyphen position.
            }
        }
        else
        {
            // Nope. The character here should be a simple xdigit
            if (ishexdigit(var) == 0)
            {
                return 0;// Oops. The current character is not a hyphen.
            }
        }
    }
    return 1;// Seen'em all!
}

CAResult_t CALEClientInitialize()
{
    OIC_LOG(DEBUG, TAG, "IN - CALEClientInitialize");

    CAResult_t ret = CALEClientInitGattMutexVaraibles();
    if (CA_STATUS_OK != ret){
        OIC_LOG(ERROR, TAG, "CALEClientInitGattMutexVaraibles has failed!");
        CALEClientTerminateGattMutexVariables();

        return ret;
    }

    // init cond for initialize logic
    if (g_initializeCond == NULL) {
        g_initializeCond = oc_cond_new();
    }

    cbCentralIf = [[CBCentralIfClass alloc] init];
    [cbCentralIf CALEClientCreateCentralManager];

    g_deviceDescCond = oc_cond_new();

    // init mutex for send logic
    g_threadCond = oc_cond_new();
    g_threadWriteCharacteristicCond = oc_cond_new();
    g_deviceScanRetryDelayCond = oc_cond_new();

    CALEClientCreateDeviceList();

    g_isStartedLEClient = true;

    oc_cond_signal(g_initializeCond);

    return CA_STATUS_OK;
}

void CALEClientTerminate()
{
    OIC_LOG(DEBUG, TAG, "IN - CALEClientTerminate");

    if (g_sendBuffer != nil){
        g_sendBuffer = nil;
    }

    CAResult_t ret = CALEClientRemoveAllDeviceState();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEClientRemoveAllDeviceState has failed");
    }

    ret = CALEClientRemoveAllScanDevices();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEClientRemoveAllScanDevices has failed");
    }

    ret = CALEClientRemoveAllGattObjs();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEClientRemoveAllGattObjs has failed");
    }

    CALEClientSetScanFlag(false);
    CALEClientSetSendFinishFlag(true);

    CALEClientTerminateGattMutexVariables();

    oc_cond_free(g_deviceDescCond);
    oc_cond_free(g_threadCond);
    oc_cond_free(g_threadWriteCharacteristicCond);
    oc_cond_free(g_deviceScanRetryDelayCond);
    oc_cond_free(g_initializeCond);

    g_deviceDescCond = NULL;
    g_threadCond = NULL;
    g_threadWriteCharacteristicCond = NULL;
    g_deviceScanRetryDelayCond = NULL;
    g_initializeCond = NULL;

    g_isSignalSetFlag = false;

    centralMgr.delegate = nil;
    if ([centralMgrs count] != 0) {
        for (CBCentralManager *mgr in centralMgrs){
            [centralMgrs removeObject:mgr];
        }
    }

    [cbCentralIf release];
    cbCentralIf = nil;

    OIC_LOG(DEBUG, TAG, "OUT - CALEClientTerminate");
}

void CALEClientSendFinish(CBPeripheral *peripheral)
{
    OIC_LOG(DEBUG, TAG, "CALEClientSendFinish");

    if (peripheral)
    {
        CAResult_t res = CALEClientDisconnect(peripheral);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALEClientDisconnect has failed");
        }
    }
    CALEClientUpdateSendCnt();
}

CAResult_t CALEClientSendUnicastMessage(const char* address,
                                        const uint8_t* data,
                                        const uint32_t dataLen)
{
    OIC_LOG_V(DEBUG, TAG, "CALEClientSendUnicastMessage(%s, %p)", address, data);
    VERIFY_NON_NULL(address, TAG, "address is null");
    VERIFY_NON_NULL(data, TAG, "data is null");

    return CALEClientSendUnicastMessageImpl(address, data, dataLen);
}

CAResult_t CALEClientSendMulticastMessage(const uint8_t* data,
                                          const uint32_t dataLen)
{
    OIC_LOG_V(DEBUG, TAG, "CALEClientSendMulticastMessage(%p)", data);
    VERIFY_NON_NULL(data, TAG, "data is null");

    CAResult_t ret = CALEClientSendMulticastMessageImpl(data, dataLen);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEClientSendMulticastMessageImpl has failed");
    }

    return ret;
}

CAResult_t CALEClientStartUnicastServer(const char* address)
{
    OIC_LOG_V(DEBUG, TAG, "it is not needed in this platform (%s)", address);

    return CA_NOT_SUPPORTED;
}

CAResult_t CALEClientStartMulticastServer()
{
    OIC_LOG(DEBUG, TAG, "it is not needed in this platform");

    return CA_NOT_SUPPORTED;
}

void CALEClientStopUnicastServer()
{
    OIC_LOG(DEBUG, TAG, "CALEClientStopUnicastServer");
}

void CALEClientStopMulticastServer()
{
    OIC_LOG(DEBUG, TAG, "CALEClientStopMulticastServer");
}

void CALEClientSetCallback(CAPacketReceiveCallback callback)
{
    g_packetReceiveCallback = callback;
}

void CALEClientSetCAManagerCallback(CAManagerAdapterStateChangeCallback adapterStateChangeCB,
                                     CAManagerConnectionCallback connectionCB,
                                     CAManagerServiceDiscoveredCallback serviceDiscoverdCB)
{
    VERIFY_NON_NULL_VOID(adapterStateChangeCB, TAG, "adapterStateChangeCB is null");
    VERIFY_NON_NULL_VOID(connectionCB, TAG, "connectionCB is null");
    VERIFY_NON_NULL_VOID(serviceDiscoverdCB, TAG, "serviceDiscoverdCB is null");

    if (!g_caManagerQueue) {
        g_caManagerQueue = dispatch_queue_create("com.caManager", DISPATCH_QUEUE_SERIAL);
    }

    g_caManagerAdapterStateChangeCallback = adapterStateChangeCB;
    g_caManagerConnectionCallback = connectionCB;
    g_caManagerServiceDiscoveredCallback = serviceDiscoverdCB;
}


void CASetBLEClientErrorHandleCallback(CABLEErrorHandleCallback callback)
{
    g_clientErrorCallback = callback;
}

CAResult_t CALEClientIsThereScannedDevices(const char* address)
{
    if (!g_deviceList)
    {
        OIC_LOG(ERROR, TAG, "CALEClientIsThereScannedDevices g_deviceList is null");
        return CA_STATUS_FAILED;
    }

    if (0 == u_arraylist_length(g_deviceList) // multicast
            || (address && !CALEClientIsDeviceInScanDeviceList(address))) // unicast
    {
        // Wait for LE peripherals to be discovered.

        // Number of times to wait for discovery to complete.
        static size_t const RETRIES = 7;

        static uint64_t const TIMEOUT =
            3 * MICROSECS_PER_SEC;  // Microseconds

        bool devicesDiscovered = false;
        for (size_t i = 0; i < RETRIES; ++i)
        {
            OIC_LOG(DEBUG, TAG, "waiting for target device");
            if (oc_cond_wait_for(g_deviceDescCond,
                                 g_threadSendMutex,
                                 TIMEOUT) == OC_WAIT_SUCCESS)
            {
                oc_mutex_lock(g_deviceListMutex);
                size_t scannedDeviceLen = u_arraylist_length(g_deviceList);
                oc_mutex_unlock(g_deviceListMutex);

                if (0 < scannedDeviceLen)
                {
                    if (!address  // multicast
                        || (address && CALEClientIsDeviceInScanDeviceList(address))) // unicast
                    {
                      devicesDiscovered = true;
                      break;
                    }
                    else
                    {
                        if (address)
                        {
                            OIC_LOG(INFO, TAG, "waiting..");

                            oc_mutex_lock(g_deviceScanRetryDelayMutex);
                            if (oc_cond_wait_for(g_deviceScanRetryDelayCond,
                                                 g_deviceScanRetryDelayMutex,
                                                 MICROSECS_PER_SEC) == OC_WAIT_SUCCESS)
                            {
                                OIC_LOG(INFO, TAG, "finish to waiting for target device");
                                oc_mutex_unlock(g_deviceScanRetryDelayMutex);
                                break;
                            }
                            oc_mutex_unlock(g_deviceScanRetryDelayMutex);
                            // time out

                            // checking whether a target device is found while waiting for time-out.
                            if (CALEClientIsDeviceInScanDeviceList(address))
                            {
                                devicesDiscovered = true;
                                break;
                            }
                        }
                    }
                }
            }
        }

        // time out for scanning devices
        if (!devicesDiscovered)
        {
            return CA_STATUS_FAILED;
        }
    }

    return CA_STATUS_OK;
}


CAResult_t CALEClientSendUnicastMessageImpl(const char* address,
                                            const uint8_t* data,
                                            const uint32_t dataLen)
{
    OIC_LOG_V(DEBUG, TAG, "CALEClientSendUnicastMessageImpl: \n address: %s, data: %p",
                        address, data);
    VERIFY_NON_NULL(address, TAG, "address is null");
    VERIFY_NON_NULL(data, TAG, "data is null");

    oc_mutex_lock(g_threadSendMutex);

    CALEClientSetSendFinishFlag(false);

    CAResult_t ret = CALEClientIsThereScannedDevices(address);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(INFO, TAG, "there is no scanned device");
        goto error_exit;
    }

    char *founded_address;
   uint32_t length = u_arraylist_length(g_deviceList);
   OIC_LOG_V(DEBUG, TAG, "g_deviceList : %d", length);
   int is_mac = isMAC(address);
   for (uint32_t index = 0; index < length; index++)
   {
           CBPeripheral *peripheral = (CBPeripheral *)u_arraylist_get(g_deviceList, index);
        if (!peripheral)
        {
            OIC_LOG(ERROR, TAG, "devicelist is empty");
            goto error_exit;
        }

        char* setAddress;
        if(is_mac == 0)
        {
            setAddress = [[[peripheral identifier] UUIDString] UTF8String];
        }
        else  //address
        {
            setAddress = [[cbCentralIf CALEGetAddressFromBTDevice: peripheral] UTF8String];
        }

        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "setAddress is null");
            if(is_mac == 0) continue;
            else goto error_exit;
        }

        OIC_LOG_V(DEBUG, TAG, "remote device address is %s", setAddress);
#ifdef TB_LOG
        NSLog(@"scan device info: %@", peripheral);
#endif

        if (!strcmp(setAddress, address))
        {
            founded_address = [[cbCentralIf CALEGetAddressFromBTDevice: peripheral] UTF8String];

            // connect to gatt server
            OIC_LOG(DEBUG, TAG, "CALEClientSendUnicastMessageImpl --> stop scan");
            ret = CALEClientStopScan();
            if (CA_STATUS_OK != ret)
            {
                OIC_LOG(ERROR, TAG, "CALEClientStopScan has failed");
                goto error_exit;
            }

            if (g_sendBuffer != nil ){
                g_sendBuffer = nil;
            }
            g_sendBuffer = [NSData dataWithBytes:(const uint8_t*)data length:dataLen];

#ifdef TB_LOG
            NSLog(@"unicast send data: %@, size: %lu", g_sendBuffer, [g_sendBuffer length]);
#endif
            ret = CALEClientSendData(peripheral);
            if (CA_STATUS_OK != ret)
            {
                OIC_LOG(ERROR, TAG, "CALEClientSendData in unicast is failed");
                goto error_exit;
            }

            OIC_LOG(INFO, TAG, "wake up");
            break;
        }
    }

    OIC_LOG(DEBUG, TAG, "connection routine is finished for unicast");

    // wait for finish to send data through "CALeGattServicesDiscoveredCallback"
    // if there is no connection state.
    oc_mutex_lock(g_threadMutex);
    if (!g_isFinishedSendData)
    {
        OIC_LOG(DEBUG, TAG, "waiting send finish signal");
        oc_cond_wait(g_threadCond, g_threadMutex);
        OIC_LOG(DEBUG, TAG, "the data was sent");
    }
    oc_mutex_unlock(g_threadMutex);

    // start LE Scan again
    ret = CALEClientStartScan();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEClientStartScan has failed");
        oc_mutex_unlock(g_threadSendMutex);
        return ret;
    }

    oc_mutex_unlock(g_threadSendMutex);
    OIC_LOG(INFO, TAG, "unicast - send logic has finished");
    return CALECheckSendState(founded_address);

    // error label.
error_exit:

    // start LE Scan again
    if (!CALEClientStartScan())
    {
        OIC_LOG(ERROR, TAG, "CALEClientStartScan has failed");
        oc_mutex_unlock(g_threadSendMutex);
        return CA_STATUS_FAILED;
    }

    if (g_clientErrorCallback)
    {
        g_clientErrorCallback(address, data, dataLen, CA_SEND_FAILED);
    }
    oc_mutex_unlock(g_threadSendMutex);
    return CA_SEND_FAILED;
}

CAResult_t CALEClientSendMulticastMessageImpl(const uint8_t* data,
                                              const uint32_t dataLen)
{
    OIC_LOG_V(DEBUG, TAG, "CASendMulticastMessageImpl, send to, data: %p, %u", data, dataLen);
    VERIFY_NON_NULL(data, TAG, "data is null");

    if (!g_deviceList)
    {
        OIC_LOG(ERROR, TAG, "g_deviceList is null");
        return CA_STATUS_FAILED;
    }

    uint32_t length = 0;
    oc_cond waitCond = NULL;

    oc_mutex_lock(g_threadSendMutex);

    CALEClientSetSendFinishFlag(false);

    OIC_LOG(DEBUG, TAG, "set byteArray for data");

    CAResult_t res = CALEClientIsThereScannedDevices(NULL);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(INFO, TAG, "there is no scanned device");
        goto error_exit;
    }

    // connect to gatt server
    OIC_LOG(DEBUG, TAG, "CALEClientSendMulticastMessageImpl --> stop scan");
    res = CALEClientStopScan();
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CALEClientStopScan has failed");
        oc_mutex_unlock(g_threadSendMutex);
        return res;
    }
    length = u_arraylist_length(g_deviceList);
    g_targetCnt = length;

    if ( g_sendBuffer != nil ){
        g_sendBuffer = nil;
    }

    g_sendBuffer = [NSData dataWithBytes:(const uint8_t*)data length:dataLen];
#ifdef TB_LOG
    NSLog(@"multicast send data: %@, size: %lu", g_sendBuffer, [g_sendBuffer length]);
#endif

    for (uint32_t index = 0; index < length; index++)
    {
        OIC_LOG_V(DEBUG, TAG, "target cnt: %d/%d", index+1,length);
        CBPeripheral *peripheral = (CBPeripheral *)u_arraylist_get(g_deviceList, index);
        if (!peripheral)
        {
            OIC_LOG(ERROR, TAG, "peripheral is not available");
            continue;
        }
        else{
            CALEClientSendData(peripheral);
        }
    }
    OIC_LOG(DEBUG, TAG, "connection routine is finished for multicast");

    // wait for finish to send data through "CALeGattServicesDiscoveredCallback"
    oc_mutex_lock(g_threadMutex);
    if (!g_isFinishedSendData)
    {
        OIC_LOG(DEBUG, TAG, "waiting send finish signal");
        oc_cond_wait(g_threadCond, g_threadMutex);
        OIC_LOG(DEBUG, TAG, "the data was sent");
    }
    oc_mutex_unlock(g_threadMutex);

    // start LE Scan again
    res = CALEClientStartScan();
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CALEClientStartScan has failed");
        oc_mutex_unlock(g_threadSendMutex);
        return res;
    }

    oc_mutex_unlock(g_threadSendMutex);
    OIC_LOG(DEBUG, TAG, "OUT - CALEClientSendMulticastMessageImpl");
    return CA_STATUS_OK;

error_exit:
    res = CALEClientStartScan();
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CALEClientStartScan has failed");
        oc_mutex_unlock(g_threadSendMutex);
        return res;
    }

    oc_mutex_unlock(g_threadSendMutex);
    OIC_LOG(DEBUG, TAG, "OUT - CALEClientSendMulticastMessageImpl");
    return CA_SEND_FAILED;
}

CAResult_t CALECheckSendState(const char* address)
{
    VERIFY_NON_NULL(address, TAG, "address is null");

    oc_mutex_lock(g_deviceStateListMutex);
    CALEState_t* state = CALEClientGetStateInfo(address);
    if (NULL == state)
    {
        OIC_LOG(ERROR, TAG, "state is null");
        oc_mutex_unlock(g_deviceStateListMutex);
        return CA_SEND_FAILED;
    }

    if (STATE_SEND_SUCCESS != state->sendState)
    {
        OIC_LOG(ERROR, TAG, "sendstate is not STATE_SEND_SUCCESS");
        oc_mutex_unlock(g_deviceStateListMutex);
        return CA_SEND_FAILED;
    }

    OIC_LOG(INFO, TAG, "sendstate is STATE_SEND_SUCCESS");
    oc_mutex_unlock(g_deviceStateListMutex);
    return CA_STATUS_OK;
}

CAResult_t CALEClientSendData(CBPeripheral *peripheral)//, CBService *service)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEClientSendData");

    // get BLE address from bluetooth device object.
    CALEState_t* state = NULL;
    char *address = (char*)[[cbCentralIf CALEGetAddressFromBTDevice: peripheral] UTF8String];
    if (!address)
    {
        OIC_LOG(ERROR, TAG, "address is not available");
        return CA_STATUS_FAILED;
    }
    oc_mutex_lock(g_deviceStateListMutex);
    state = CALEClientGetStateInfo(address);
    oc_mutex_unlock(g_deviceStateListMutex);

    if (!state)
    {
        OIC_LOG(DEBUG, TAG, "state is empty..start to connect LE");

        // cancel previous connection request before connection
        // if there is gatt object in g_gattObjectList.

        // connection request
        CAResult_t ret = CALEClientConnect(peripheral, false);
        if (CA_STATUS_OK != ret){
            OIC_LOG(ERROR, TAG, "CALEClientConnect failed");
            return ret;
        }
    }
    else
    {
        if (STATE_CONNECTED == state->connectedState)
        {
            OIC_LOG(INFO, TAG, "GATT has already connected");

            CBPeripheral *tPeripheral = CALEClientGetGattObjInList(address);
            if (!tPeripheral)
            {
                OIC_LOG(ERROR, TAG, "CALEClientGetGattObjInList has failed");
                //return CA_STATUS_FAILED;
                goto exit;
            }

            CAResult_t ret = CALEClientWriteCharacteristic(peripheral);
            //CAResult_t ret = CALESetValueAndWriteCharacteristic(peripheral, service);
            if (CA_STATUS_OK != ret)
            {
                OIC_LOG(ERROR, TAG, "WriteCharacteristic has failed");
                return ret;
            }
        }
        else
        {
            OIC_LOG(INFO, TAG, "STATE_DISCONNECTED - start to connect LE");

            // cancel previous connection request before connection
            // if there is gatt object in g_gattObjectList.

            OIC_LOG(DEBUG, TAG, "start to connect LE");
            CAResult_t ret = CALEClientConnect(peripheral, CALEClientGetAutoConnectFlag(address));
            if (CA_STATUS_OK != ret){
                OIC_LOG(ERROR, TAG, "CALEClientConnect failed");
                return ret;
            }
        }
    }

    return CA_STATUS_OK;
exit:
    return CA_STATUS_OK;
}

NSString *CALEClientGetAddressFromGattObj(CBPeripheral *peripheral)
{
    VERIFY_NON_NULL_RET(peripheral, TAG, "peripheral is null", NULL);

    return [cbPeripherals objectForKey: peripheral];
}

/**
 * BLE layer
 */
CAResult_t CALEClientGattClose(CBPeripheral *peripheral)
{
    // GATT CLOSE
    OIC_LOG(DEBUG, TAG, "Gatt Close");
    [cbCentralIf CALEClientCancelPeripheralConnection:peripheral];
    return CA_STATUS_OK;
}

CAResult_t CALEClientStartScan()
{
    if (!CALEClientIsEnableBTAdapter())
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    if (!g_isStartedLEClient)
    {
        OIC_LOG(ERROR, TAG, "LE client is not started");
        return CA_STATUS_FAILED;
    }

    if (g_isStartedScan)
    {
        OIC_LOG(INFO, TAG, "scanning is already started");
        return CA_STATUS_OK;
    }

    OIC_LOG(DEBUG, TAG, "CALEClientStartScan");

    CAResult_t ret = CA_STATUS_OK;

#ifdef UUID_SCAN
        ret = CA_ADAPTER_NOT_ENABLE;
#else
        ret = [cbCentralIf CALEClientStartScanImpl];
#endif
        if (CA_STATUS_OK != ret)
        {
            if (CA_ADAPTER_NOT_ENABLED == ret)
            {
                OIC_LOG(DEBUG, TAG, "Adapter is disabled");
            }
            else
            {
                OIC_LOG(ERROR, TAG, "start scan has failed");
            }
        }

    return ret;
}

CAResult_t CALEClientStopScan()
{
    if (!g_isStartedScan)
    {
        OIC_LOG(INFO, TAG, "scanning is already stopped");
        return CA_STATUS_OK;
    }

    CAResult_t ret = [cbCentralIf CALEClientStopScanImpl];
    if (CA_STATUS_OK != ret)
    {
        if (CA_ADAPTER_NOT_ENABLED == ret)
        {
            OIC_LOG(DEBUG, TAG, "Adapter is disabled");
        }
        else
        {
            OIC_LOG(ERROR, TAG, "CALEClientStopScanImpl has failed");
        }
    }
    else
    {
        CALEClientSetScanFlag(false);
    }

    return ret;
}

void CALEClientSetScanFlag(bool flag)
{
    oc_mutex_lock(g_scanMutex);
    g_isStartedScan = flag;
    oc_mutex_unlock(g_scanMutex);
}

CAResult_t CALEClientSetAutoConnectFlag(const char *address, bool flag)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEClientSetAutoConnectFlag");

    oc_mutex_lock(g_deviceStateListMutex);

    if (!address)
    {
        OIC_LOG(ERROR, TAG, "address is not available");
        return CA_STATUS_FAILED;
    }

    if (CALEClientIsDeviceInList(address))
    {
        CALEState_t* curState = CALEClientGetStateInfo(address);
        if(!curState)
        {
            OIC_LOG(ERROR, TAG, "curState is null");
            oc_mutex_unlock(g_deviceStateListMutex);
            return CA_STATUS_FAILED;
        }
        OIC_LOG_V(INFO, TAG, "auto connect flag is set %d", flag);

        curState->autoConnectFlag = flag;
    }

    oc_mutex_unlock(g_deviceStateListMutex);
    OIC_LOG(DEBUG, TAG, "OUT - CALEClientSetAutoConnectFlag");
    return CA_STATUS_OK;
}

bool CALEClientGetAutoConnectFlag(const char *address)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEClientGetAutoConnectFlag");

    oc_mutex_lock(g_deviceStateListMutex);

    if (!address)
    {
        OIC_LOG(ERROR, TAG, "address is not available");
        return false;
    }

    CALEState_t* curState = CALEClientGetStateInfo(address);
    if(!curState)
    {
        OIC_LOG(INFO, TAG, "there is no information. auto connect flag is false");
        oc_mutex_unlock(g_deviceStateListMutex);
        return false;
    }
    OIC_LOG_V(INFO, TAG, "auto connect flag is %d", curState->autoConnectFlag);

    oc_mutex_unlock(g_deviceStateListMutex);

    OIC_LOG(DEBUG, TAG, "OUT - CALEClientGetAutoConnectFlag");
    return curState->autoConnectFlag;
}

CAResult_t CALEClientConnect(CBPeripheral *peripheral, bool autoconnect)
{
    OIC_LOG(DEBUG, TAG, "CALEClientConnect");

    return [cbCentralIf CALEClientConnectImpl:peripheral];
}

CAResult_t CALEClientDisconnect(CBPeripheral *peripheral)
{
    OIC_LOG(DEBUG, TAG, "GATT DISCONNECT");

    [cbCentralIf CALEClientCancelPeripheralConnection: peripheral];
    return CA_STATUS_OK;
}

CAResult_t CALEClientConnectCancel(CBPeripheral *peripheral)
{
    OIC_LOG(DEBUG, TAG, "GATT CONNECT CENCEL");

    [cbCentralIf CALEClientCancelPeripheralConnection: peripheral];
    return CA_STATUS_OK;
}

CAResult_t CALEClientDisconnectAll()
{
    OIC_LOG(DEBUG, TAG, "CALEClientDisconnectAll");

    if (!g_gattObjectList)
    {
        OIC_LOG(DEBUG, TAG, "already removed for g_gattObjectList");
        return CA_STATUS_OK;
    }

    uint32_t length = u_arraylist_length(g_gattObjectList);
    OIC_LOG_V(DEBUG, TAG, "list length : %d", length);
    for (uint32_t index = 0; index < length; index++)
    {
        OIC_LOG(DEBUG, TAG, "start CALEClientDisconnectAll");
        CBPeripheral *tPeripheral = (CBPeripheral *) u_arraylist_get(g_gattObjectList, index);

        CAResult_t res = CALEClientDisconnect(tPeripheral);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALEClientDisconnect has failed");
            continue;
        }
    }

    return CA_STATUS_OK;
}

CAResult_t CALEClientDisconnectforAddress(const char *remote_address)
{
    OIC_LOG(DEBUG, TAG, "IN-CALEClientDisconnectforAddress");

    if (!g_gattObjectList)
    {
        OIC_LOG(DEBUG, TAG, "already removed for g_gattObjectList");
        return CA_STATUS_OK;
    }

    uint32_t length = u_arraylist_length(g_gattObjectList);
    for (uint32_t index = 0; index < length; index++)
    {
        CBPeripheral *tPeripheral = (CBPeripheral *) u_arraylist_get(g_gattObjectList, index);

        const char* setAddress = [[cbCentralIf CALEGetAddressFromBTDevice: tPeripheral] UTF8String];
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "setAddress is null");
            return CA_STATUS_FAILED;
        }

        if (!strcmp(remote_address, setAddress))
        {
            CAResult_t res = CALEClientDisconnect(tPeripheral);
        }
    }
    OIC_LOG(DEBUG, TAG, "OUT-CALEClientDisconnectforAddress");
    return CA_STATUS_OK;
}

CAResult_t CALEClientWriteCharacteristic(CBPeripheral *peripheral)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEClientWriteCharacteristic");

    CBService *service = [cbServices objectForKey:peripheral];
    if (service == NULL){
        OIC_LOG(ERROR, TAG, "can't find service obj");
        return CA_STATUS_FAILED;
    }else{
        CBCharacteristic *requestCharacteristic = [requestCharacs objectForKey:peripheral];
        if (requestCharacteristic == NULL){
            OIC_LOG(ERROR, TAG, "can't find request characteristic obj");
            return CA_STATUS_FAILED;
        }else{
            CAResult_t ret = CALEClientWriteCharacteristicImpl(peripheral, requestCharacteristic);
            if (CA_STATUS_OK != ret){
                CALEClientSendFinish(peripheral);
                return CA_STATUS_FAILED;
            }
        }
    }

    OIC_LOG(DEBUG, TAG, "OUT - CALEClientWriteCharacteristic");
    return CA_STATUS_OK;
}

CAResult_t CALEClientWriteCharacteristicImpl(CBPeripheral *peripheral,
                                             CBCharacteristic *requestCharc)
{
    OIC_LOG(DEBUG, TAG, "WRITE GATT CHARACTERISTIC");

    if (!CALEClientIsEnableBTAdapter())
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return CA_STATUS_FAILED;
    }

    CAResult_t ret = [cbCentralIf CALEClientWriteValue: peripheral: requestCharc];

    const char *address = [CALEClientGetAddressFromGattObj(peripheral) UTF8String];
    if (CA_STATUS_OK != ret){
        OIC_LOG(ERROR, TAG, "write characteristic failed");
        CALEClientUpdateDeviceState(address, STATE_CONNECTED, STATE_CHARACTER_SET,
                                    STATE_SEND_FAILED);

        CALEClientSendFinish(peripheral);
        return CA_STATUS_FAILED;
    }else{
        OIC_LOG(DEBUG, TAG, "writeCharacteristic is called successfully");
        CALEClientUpdateDeviceState(address, STATE_CONNECTED, STATE_CHARACTER_SET,
                                    STATE_SEND_SUCCESS);
        CALEClientUpdateSendCnt();
    }

    return CA_STATUS_OK;
}

CAResult_t CALEClientSetCharacteristicNotification(CBPeripheral *peripheral,
                                                   CBCharacteristic *responsCharc)
{
    if (!CALEClientIsEnableBTAdapter())
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    [cbCentralIf CALEClientSetNotifyCharacteristic: peripheral: responsCharc];

    return CA_STATUS_OK;
}

void CALEClientCreateScanDeviceList()
{
    OIC_LOG(DEBUG, TAG, "CALEClientCreateScanDeviceList");

    oc_mutex_lock(g_deviceListMutex);
    // create new object array
    if (g_deviceList == NULL)
    {
        OIC_LOG(DEBUG, TAG, "Create device list");

        g_deviceList = u_arraylist_create();
    }
    oc_mutex_unlock(g_deviceListMutex);
}

CAResult_t CALEClientAddScanDeviceToList(CBPeripheral *peripheral)
{
    oc_mutex_lock(g_deviceListMutex);

    if (!g_deviceList)
    {
        OIC_LOG(ERROR, TAG, "gdevice_list is null");

        CALEClientSetScanFlag(false);
        OIC_LOG(DEBUG, TAG, "CALEClientAddScanDeviceToList --> stop scan");
        if(CA_STATUS_OK != CALEClientStopScan())
        {
            OIC_LOG(ERROR, TAG, "CALEClientStopScan has failed");
        }

        oc_mutex_unlock(g_deviceListMutex);
        return CA_STATUS_FAILED;
    }

    const char *remoteAddress = [[cbCentralIf CALEGetAddressFromBTDevice: peripheral] UTF8String];
    if (!remoteAddress)
    {
        OIC_LOG(ERROR, TAG, "remoteAddress is null");
        oc_mutex_unlock(g_deviceListMutex);
        return CA_STATUS_FAILED;
    }

    if (!CALEClientIsDeviceInScanDeviceList(remoteAddress))
    {
        u_arraylist_add(g_deviceList, peripheral);
        oc_cond_signal(g_deviceDescCond);
        OIC_LOG_V(DEBUG, TAG, "Added this BT Device[%s] in the List", remoteAddress);
    }

    oc_mutex_unlock(g_deviceListMutex);

    return CA_STATUS_OK;
}

bool CALEClientIsDeviceInScanDeviceList(const char* remoteAddress)
{
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress is null", true);

    if (!g_deviceList)
    {
        OIC_LOG(DEBUG, TAG, "g_deviceList is null");
        return true;
    }

    uint32_t length = u_arraylist_length(g_deviceList);
	int is_mac = isMAC(remoteAddress);
    for (uint32_t index = 0; index < length; index++)
    {
        CBPeripheral *tPeripheral = (CBPeripheral *) u_arraylist_get(g_deviceList, index);

        if(is_mac == 1)
        {
            const char* setAddress = [[cbCentralIf CALEGetAddressFromBTDevice:
                                       tPeripheral] UTF8String];

            if (!strcmp(remoteAddress, setAddress))
            {
                return true;
            }
        }
        else
        {
            OIC_LOG(DEBUG, TAG, "this is uuid unicast!");
            const char* localUuid = [[[tPeripheral identifier] UUIDString] UTF8String];

            if(localUuid == NULL)
            {
                continue;
            }

            if(!strcmp(remoteAddress, localUuid))
            {
                OIC_LOG(DEBUG, TAG, "find same one!!!!");
                return true;
            }
        }
    }

    OIC_LOG(DEBUG, TAG, "there are no the device in list. we can add");

    return false;
}

CAResult_t CALEClientRemoveAllScanDevices()
{
    OIC_LOG(DEBUG, TAG, "CALEClientRemoveAllScanDevices");

    oc_mutex_lock(g_deviceListMutex);

    if (!g_deviceList)
    {
        OIC_LOG(ERROR, TAG, "g_deviceList is null");
        oc_mutex_unlock(g_deviceListMutex);
        return CA_STATUS_FAILED;
    }

    uint32_t length = u_arraylist_length(g_deviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        CBPeripheral *tPeripheral = (CBPeripheral *) u_arraylist_get(g_deviceList, 0);
        tPeripheral.delegate = nil;
        [cbPeripherals removeObjectForKey: tPeripheral];
        u_arraylist_remove(g_deviceList, 0);
    }

    OICFree(g_deviceList);
    g_deviceList = NULL;

    oc_mutex_unlock(g_deviceListMutex);
    return CA_STATUS_OK;
}

CAResult_t CALEClientRemoveDeviceInScanDeviceList(NSString *address)
{
    OIC_LOG(DEBUG, TAG, "CALEClientRemoveDeviceInScanDeviceList");
    VERIFY_NON_NULL(address, TAG, "address is null");
    if (nil == address){
        OIC_LOG(INFO, TAG, "CALEClientRemoveDeviceInScanDeviceList address == nil");
        return CA_STATUS_FAILED;
    }

    oc_mutex_lock(g_deviceListMutex);

    if (!g_deviceList)
    {
        OIC_LOG(ERROR, TAG, "g_deviceList is null");
        oc_mutex_unlock(g_deviceListMutex);
        return CA_STATUS_FAILED;
    }

    uint32_t length = u_arraylist_length(g_deviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        CBPeripheral *tPeripheral = (CBPeripheral *) u_arraylist_get(g_deviceList, index);

        NSString *setAddress = [cbCentralIf CALEGetAddressFromBTDevice: tPeripheral];

#ifdef TB_LOG
        NSLog(@"removedeviceinscandevicelist \n-address: %@\n-setaddr: %@", address, setAddress);
#endif
        if ([address isEqualToString:setAddress])
        {
            [cbPeripherals removeObjectForKey: tPeripheral];
            if (NULL == u_arraylist_remove(g_deviceList, index))
            {
                OIC_LOG(ERROR, TAG, "List removal failed.");
                oc_mutex_unlock(g_deviceListMutex);
                return CA_STATUS_FAILED;
            }
            oc_mutex_unlock(g_deviceListMutex);
            return CA_STATUS_OK;
        }
    }

    oc_mutex_unlock(g_deviceListMutex);
    OIC_LOG(DEBUG, TAG, "There are no object in the device list");

    return CA_STATUS_OK;
}

/**
 * Gatt Object List
 */

CAResult_t CALEClientAddGattobjToList(CBPeripheral *peripheral)
{
    OIC_LOG(INFO, TAG, "CALEClientAddGattobjToList");

    oc_mutex_lock(g_gattObjectMutex);

    if (!g_gattObjectList)
    {
        OIC_LOG(ERROR, TAG, "g_gattObjectList is not available");
        oc_mutex_unlock(g_gattObjectMutex);
        return CA_STATUS_FAILED;
    }

    NSString *remoteAddress = CALEClientGetAddressFromGattObj(peripheral);
    if (remoteAddress == nil || [remoteAddress isEqualToString:@"<null>"]
            || [remoteAddress isEqualToString:@"null"] || remoteAddress == (id)[NSNull null]
            || [[remoteAddress stringByTrimmingCharactersInSet:
                 [NSCharacterSet whitespaceAndNewlineCharacterSet]] isEqualToString:@""]){
        OIC_LOG(ERROR, TAG, "remoteAddress is null");
        oc_mutex_unlock(g_gattObjectMutex);
        return CA_STATUS_FAILED;
    }

    const char *remoteAddr = [remoteAddress UTF8String];
    OIC_LOG_V(INFO, TAG, "remote address : %s", remoteAddr);
    if (!CALEClientIsGattObjInList(remoteAddr))
    {
        u_arraylist_add(g_gattObjectList, peripheral);
        OIC_LOG(INFO, TAG, "Set GATT Object to Array as Element");
    }

    oc_mutex_unlock(g_gattObjectMutex);
    return CA_STATUS_OK;
}

bool CALEClientIsGattObjInList(const char* remoteAddress)
{
    OIC_LOG(DEBUG, TAG, "CALEClientIsGattObjInList");

    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress is null", true);

    uint32_t length = u_arraylist_length(g_gattObjectList);
    for (uint32_t index = 0; index < length; index++)
    {

        CBPeripheral *tPeripheral = (CBPeripheral *) u_arraylist_get(g_gattObjectList, index);
        if (!tPeripheral)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            return true;
        }

        NSString *setAddress = CALEClientGetAddressFromGattObj(tPeripheral);
        if (setAddress == nil || [setAddress isEqualToString:@"<null>"]
            || [setAddress isEqualToString:@"null"] || setAddress == (id)[NSNull null]
            || [[setAddress stringByTrimmingCharactersInSet:
                 [NSCharacterSet whitespaceAndNewlineCharacterSet]] isEqualToString:@""]){
            OIC_LOG(ERROR, TAG, "jni_setAddress is null");
            return true;
        }

        const char* setAddr = [setAddress UTF8String];
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "setAddress is null");
            return true;
        }

        if (!strcmp(remoteAddress, setAddr))
        {
            OIC_LOG(DEBUG, TAG, "the device is already set");
            return true;
        }
        else
        {
            continue;
        }
    }

    OIC_LOG(DEBUG, TAG, "There are no GATT object in list. it can be added");
    return false;
}

CBPeripheral *CALEClientGetGattObjInList(const char* remoteAddress)
{
    OIC_LOG(DEBUG, TAG, "CALEClientGetGattObjInList");
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress is null", NULL);

    oc_mutex_lock(g_gattObjectMutex);
    uint32_t length = u_arraylist_length(g_gattObjectList);
    for (uint32_t index = 0; index < length; index++)
    {
        CBPeripheral *tPeripheral = (CBPeripheral *) u_arraylist_get(g_gattObjectList, index);
        if (!tPeripheral)
        {
            OIC_LOG(ERROR, TAG, "peripheral is null");
            oc_mutex_unlock(g_gattObjectMutex);
            return NULL;
        }

        NSString *setAddress = CALEClientGetAddressFromGattObj(tPeripheral);
        if (setAddress == nil || [setAddress isEqualToString:@"<null>"]
            || [setAddress isEqualToString:@"null"] || setAddress == (id)[NSNull null]
            || [[setAddress stringByTrimmingCharactersInSet:
                 [NSCharacterSet whitespaceAndNewlineCharacterSet]] isEqualToString:@""]){
            OIC_LOG(ERROR, TAG, "setAddress is null");
            oc_mutex_unlock(g_gattObjectMutex);
            return NULL;
        }

        const char* setAddr = [setAddress UTF8String];
        if (!setAddr)
        {
            OIC_LOG(ERROR, TAG, "setAddress is null");
            oc_mutex_unlock(g_gattObjectMutex);
            return NULL;
        }

        if (!strcmp(remoteAddress, setAddr))
        {
            OIC_LOG(DEBUG, TAG, "the device is already set");
            oc_mutex_unlock(g_gattObjectMutex);
            return tPeripheral;
        }
    }

    oc_mutex_unlock(g_gattObjectMutex);
    OIC_LOG(DEBUG, TAG, "There are no the gatt object in list");
    return NULL;
}

CAResult_t CALEClientRemoveAllGattObjs()
{
    OIC_LOG(DEBUG, TAG, "CALEClientRemoveAllGattObjs");

    oc_mutex_lock(g_gattObjectMutex);
    if (!g_gattObjectList)
    {
        OIC_LOG(DEBUG, TAG, "already removed for g_gattObjectList");
        oc_mutex_unlock(g_gattObjectMutex);
        return CA_STATUS_OK;
    }

    uint32_t length = u_arraylist_length(g_gattObjectList);
    for (uint32_t index = 0; index < length; index++)
    {
        CBPeripheral *tPeripheral = (CBPeripheral *) u_arraylist_get(g_gattObjectList, index);
        if (!tPeripheral)
        {
            OIC_LOG(ERROR, TAG, "tPeripheral is null");
            continue;
        }else{
            if (NULL == u_arraylist_remove(g_gattObjectList, index))
            {
                OIC_LOG(ERROR, TAG, "List removal failed.");
                oc_mutex_unlock(g_gattObjectMutex);
                return CA_STATUS_FAILED;
            }
        }
    }

    OICFree(g_gattObjectList);
    g_gattObjectList = NULL;
    OIC_LOG(INFO, TAG, "g_gattObjectList is removed");
    oc_mutex_unlock(g_gattObjectMutex);
    return CA_STATUS_OK;
}

CAResult_t CALEClientRemoveGattObj(CBPeripheral *peripheral)
{
    OIC_LOG(DEBUG, TAG, "CALEClientRemoveGattObj");

    oc_mutex_lock(g_gattObjectMutex);
    if (!g_gattObjectList)
    {
        OIC_LOG(DEBUG, TAG, "already removed for g_gattObjectList");
        oc_mutex_unlock(g_gattObjectMutex);
        return CA_STATUS_OK;
    }

    uint32_t length = u_arraylist_length(g_gattObjectList);
    for (uint32_t index = 0; index < length; index++)
    {
        CBPeripheral *tPeripheral = (CBPeripheral *) u_arraylist_get(g_gattObjectList, index);
        if ([peripheral isEqual:tPeripheral]){
            if (NULL == u_arraylist_remove(g_gattObjectList, index))
            {
                OIC_LOG(ERROR, TAG, "List removal failed.");
                oc_mutex_unlock(g_gattObjectMutex);
                return CA_STATUS_FAILED;
            }
        }
    }

    oc_mutex_unlock(g_gattObjectMutex);
    OIC_LOG(DEBUG, TAG, "there are no target object");
    return CA_STATUS_OK;
}

CAResult_t CALEClientRemoveGattObjForAddr(const char* remoteAddr)
{
    OIC_LOG(DEBUG, TAG, "CALEClientRemoveGattObjForAddr");
    VERIFY_NON_NULL(remoteAddr, TAG, "addr is null");

    oc_mutex_lock(g_gattObjectMutex);
    if (!g_gattObjectList)
    {
        OIC_LOG(DEBUG, TAG, "already removed for g_gattObjectList");
        oc_mutex_unlock(g_gattObjectMutex);
        return CA_STATUS_OK;
    }

    uint32_t length = u_arraylist_length(g_gattObjectList);
    for (uint32_t index = 0; index < length; index++)
    {
        CBPeripheral *tPeripheral = (CBPeripheral *) u_arraylist_get(g_gattObjectList, index);
        if (!tPeripheral)
        {
            OIC_LOG(ERROR, TAG, "tPeripheral is null");
            oc_mutex_unlock(g_gattObjectMutex);
            return CA_STATUS_FAILED;
        }

        NSString *setAddress = CALEClientGetAddressFromGattObj(tPeripheral);
        if (setAddress == nil || [setAddress isEqualToString:@"<null>"]
            || [setAddress isEqualToString:@"null"] || setAddress == (id)[NSNull null]
            || [[setAddress stringByTrimmingCharactersInSet:
                 [NSCharacterSet whitespaceAndNewlineCharacterSet]] isEqualToString:@""]){
            OIC_LOG(ERROR, TAG, "jni_setAddress is null");
            oc_mutex_unlock(g_gattObjectMutex);
            return CA_STATUS_FAILED;
        }

        const char* setAddr = [setAddress UTF8String];
        if (!setAddr)
        {
            OIC_LOG(ERROR, TAG, "setAddress is null");
            oc_mutex_unlock(g_gattObjectMutex);
            return CA_STATUS_FAILED;
        }

        if (!strcmp(setAddr, remoteAddr))
        {
            OIC_LOG_V(DEBUG, TAG, "remove object : %s", remoteAddr);
            if (NULL == u_arraylist_remove(g_gattObjectList, index))
            {
                OIC_LOG(ERROR, TAG, "List removal failed.");
                oc_mutex_unlock(g_gattObjectMutex);
                return CA_STATUS_FAILED;
            }
            oc_mutex_unlock(g_gattObjectMutex);
            return CA_STATUS_OK;
        }
    }

    oc_mutex_unlock(g_gattObjectMutex);
    OIC_LOG(DEBUG, TAG, "there are no target object");
    return CA_STATUS_FAILED;
}

/**
 * BT State List
 */

CAResult_t CALEClientUpdateDeviceState(const char* address,
                                       uint32_t connectedState,
                                       uint16_t notificationState,
                                       uint16_t sendState)
{
    VERIFY_NON_NULL(address, TAG, "address is null");

    CALEState_t *newstate = (CALEState_t*) OICCalloc(1, sizeof(*newstate));
    if (!newstate)
    {
        OIC_LOG(ERROR, TAG, "out of memory");
        return CA_MEMORY_ALLOC_FAILED;
    }

    if (strlen(address) > CA_MACADDR_SIZE)
    {
        OIC_LOG(ERROR, TAG, "address is not proper");
        OICFree(newstate);
        return CA_STATUS_FAILED;
    }

    OICStrcpy(newstate->address, sizeof(newstate->address), address);
    newstate->connectedState = connectedState;
    newstate->notificationState = notificationState;
    newstate->sendState = sendState;
    return CALEClientAddDeviceStateToList(newstate);
}

CAResult_t CALEClientAddDeviceStateToList(CALEState_t* state)
{
    VERIFY_NON_NULL(state, TAG, "state is null");

    oc_mutex_lock(g_deviceStateListMutex);

    if (!g_deviceStateList)
    {
        OIC_LOG(ERROR, TAG, "gdevice_list is null");
        oc_mutex_unlock(g_deviceStateListMutex);
        return CA_STATUS_FAILED;
    }

    if (CALEClientIsDeviceInList(state->address))
    {
        CALEState_t* curState = CALEClientGetStateInfo(state->address);
        if(!curState)
        {
            OIC_LOG(ERROR, TAG, "curState is null");
            oc_mutex_unlock(g_deviceStateListMutex);
            return CA_STATUS_FAILED;
        }

        if (STATE_CHARACTER_NO_CHANGE == state->notificationState)
        {
            state->notificationState = curState->notificationState;
        }
        state->autoConnectFlag = curState->autoConnectFlag;

        // delete previous state for update new state
        CAResult_t res = CALEClientRemoveDeviceState(state->address);
        if (CA_STATUS_OK != res)
        {
            OIC_LOG(ERROR, TAG, "CALEClientRemoveDeviceState has failed");
            oc_mutex_unlock(g_deviceStateListMutex);
            return res;
        }
    }
    u_arraylist_add(g_deviceStateList, state); // update new state
    OIC_LOG_V(INFO, TAG, "Set State Info to List : %d, %d, %s, %d",
              state->connectedState, state->notificationState,
              state->address, state->autoConnectFlag);

    oc_mutex_unlock(g_deviceStateListMutex);
    return CA_STATUS_OK;
}

bool CALEClientIsDeviceInList(const char* remoteAddress)
{
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress is null", false);

    if (!g_deviceStateList)
    {
        OIC_LOG(ERROR, TAG, "g_deviceStateList is null");
        return false;
    }

    uint32_t length = u_arraylist_length(g_deviceStateList);
    for (uint32_t index = 0; index < length; index++)
    {
        CALEState_t* state = (CALEState_t*) u_arraylist_get(g_deviceStateList, index);
        if (!state)
        {
            OIC_LOG(ERROR, TAG, "CALEState_t object is null");
            return false;
        }

        if (!strcmp(remoteAddress, state->address))
        {
            OIC_LOG(DEBUG, TAG, "the device is already set");
            return true;
        }
    }

    OIC_LOG(DEBUG, TAG, "there are no the device in list.");
    return false;
}

CAResult_t CALEClientRemoveAllDeviceState()
{
    OIC_LOG(DEBUG, TAG, "CALEClientRemoveAllDeviceState");

    oc_mutex_lock(g_deviceStateListMutex);
    if (!g_deviceStateList)
    {
        OIC_LOG(ERROR, TAG, "g_deviceStateList is null");
        oc_mutex_unlock(g_deviceStateListMutex);
        return CA_STATUS_FAILED;
    }

    uint32_t length = u_arraylist_length(g_deviceStateList);
    for (uint32_t index = 0; index < length; index++)
    {
        CALEState_t* state = (CALEState_t*) u_arraylist_get(g_deviceStateList, index);
        if (!state)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            continue;
        }else{
            u_arraylist_remove(g_deviceStateList, index);
        }
        OICFree(state);
    }

    OICFree(g_deviceStateList);
    g_deviceStateList = NULL;
    oc_mutex_unlock(g_deviceStateListMutex);

    return CA_STATUS_OK;
}

CAResult_t CALEClientResetDeviceStateForAll()
{
    OIC_LOG(DEBUG, TAG, "CALEClientResetDeviceStateForAll");

    oc_mutex_lock(g_deviceStateListMutex);
    if (!g_deviceStateList)
    {
        OIC_LOG(ERROR, TAG, "g_deviceStateList is null");
        oc_mutex_unlock(g_deviceStateListMutex);
        return CA_STATUS_FAILED;
    }

    size_t length = u_arraylist_length(g_deviceStateList);
    for (size_t index = 0; index < length; index++)
    {
        CALEState_t* state = (CALEState_t*) u_arraylist_get(g_deviceStateList, index);
        if (!state)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            continue;
        }

        // autoConnectFlag value will be not changed,
        // since it has reset only termination case.
        state->connectedState = STATE_DISCONNECTED;
        state->notificationState = STATE_CHARACTER_UNSET;
        state->sendState = STATE_SEND_NONE;
    }
    oc_mutex_unlock(g_deviceStateListMutex);

    return CA_STATUS_OK;
}

CAResult_t CALEClientRemoveDeviceState(const char* remoteAddress)
{
    OIC_LOG(DEBUG, TAG, "CALEClientRemoveDeviceState");
    VERIFY_NON_NULL(remoteAddress, TAG, "remoteAddress is null");

    if (!g_deviceStateList)
    {
        OIC_LOG(ERROR, TAG, "g_deviceStateList is null");
        return CA_STATUS_FAILED;
    }

    uint32_t length = u_arraylist_length(g_deviceStateList);
    for (uint32_t index = 0; index < length; index++)
    {
        CALEState_t* state = (CALEState_t*) u_arraylist_get(g_deviceStateList, index);
        if (!state)
        {
            OIC_LOG(ERROR, TAG, "CALEState_t object is null");
            continue;
        }

        if (!strcmp(state->address, remoteAddress))
        {
            OIC_LOG_V(DEBUG, TAG, "remove state : %s", state->address);

            CALEState_t* targetState  = (CALEState_t*)u_arraylist_remove(g_deviceStateList,
                                                                         index);
            if (NULL == targetState)
            {
                OIC_LOG(ERROR, TAG, "List removal failed.");
                return CA_STATUS_FAILED;
            }

            OICFree(targetState);
            return CA_STATUS_OK;
        }
    }

    return CA_STATUS_OK;
}

CALEState_t* CALEClientGetStateInfo(const char* remoteAddress)
{
    OIC_LOG(DEBUG, TAG, "CALEClientGetStateInfo");
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress is null", NULL);

    if (!g_deviceStateList)
    {
        OIC_LOG(ERROR, TAG, "g_deviceStateList is null");
        return NULL;
    }

    uint32_t length = u_arraylist_length(g_deviceStateList);
    OIC_LOG_V(DEBUG, TAG, "CALEClientGetStateInfo : %d", length);

    for (uint32_t index = 0; index < length; index++)
    {
        CALEState_t* state = (CALEState_t*) u_arraylist_get(g_deviceStateList, index);
        if (!state)
        {
            OIC_LOG(ERROR, TAG, "CALEState_t object is null");
            continue;
        }

        OIC_LOG_V(DEBUG, TAG, "target address : %s", remoteAddress);
        OIC_LOG_V(DEBUG, TAG, "state address : %s", state->address);

        if (!strcmp(state->address, remoteAddress))
        {
            OIC_LOG_V(DEBUG, TAG, "get state : %s", remoteAddress);
            return state;
        }
    }
    return NULL;
}

bool CALEClientIsConnectedDevice(const char* remoteAddress)
{
    OIC_LOG(DEBUG, TAG, "CALEClientIsConnectedDevice");
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress is null", false);

    oc_mutex_lock(g_deviceStateListMutex);
    if (!g_deviceStateList)
    {
        OIC_LOG(ERROR, TAG, "g_deviceStateList is null");
        oc_mutex_unlock(g_deviceStateListMutex);
        return false;
    }

    uint32_t length = u_arraylist_length(g_deviceStateList);
    for (uint32_t index = 0; index < length; index++)
    {
        CALEState_t* state = (CALEState_t*) u_arraylist_get(g_deviceStateList, index);
        if (!state)
        {
            OIC_LOG(ERROR, TAG, "CALEState_t object is null");
            continue;
        }

        if (!strcmp(state->address, remoteAddress))
        {
            OIC_LOG(DEBUG, TAG, "check whether it is connected or not");

            if (STATE_CONNECTED == state->connectedState)
            {
                oc_mutex_unlock(g_deviceStateListMutex);
                return true;
            }
            else
            {
                oc_mutex_unlock(g_deviceStateListMutex);
                return false;
            }
        }
    }
    oc_mutex_unlock(g_deviceStateListMutex);
    return false;
}

bool CALEClientIsSetCharacteristic(const char* remoteAddress)//, const char* serviceId)
{
    OIC_LOG(DEBUG, TAG, "CALEClientIsSetCharacteristic");
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress is null", false);

    oc_mutex_lock(g_deviceStateListMutex);
    if (!g_deviceStateList)
    {
        OIC_LOG(ERROR, TAG, "g_deviceStateList is null");
        oc_mutex_unlock(g_deviceStateListMutex);
        return false;
    }

    uint32_t length = u_arraylist_length(g_deviceStateList);
    for (uint32_t index = 0; index < length; index++)
    {
        CALEState_t* state = (CALEState_t*) u_arraylist_get(g_deviceStateList, index);
        if (!state)
        {
            OIC_LOG(ERROR, TAG, "CALEState_t object is null");
            continue;
        }

        if (!strcmp(state->address, remoteAddress))
        {
            OIC_LOG_V(DEBUG, TAG, "check whether it was set or not:%d", state->notificationState);

            if (STATE_CHARACTER_SET == state->notificationState)
            {
                oc_mutex_unlock(g_deviceStateListMutex);
                return true;
            }
            else
            {
                oc_mutex_unlock(g_deviceStateListMutex);
                return false;
            }
        }
    }

    oc_mutex_unlock(g_deviceStateListMutex);
    return false;
}

void CALEClientCreateDeviceList()
{
    OIC_LOG(DEBUG, TAG, "CALEClientCreateDeviceList");

    // create new object array
    if (!g_gattObjectList)
    {
        OIC_LOG(DEBUG, TAG, "Create g_gattObjectList");

        g_gattObjectList = u_arraylist_create();
    }

    if (!g_deviceStateList)
    {
        OIC_LOG(DEBUG, TAG, "Create g_deviceStateList");

        g_deviceStateList = u_arraylist_create();
    }

    if (!g_deviceList)
    {
        OIC_LOG(DEBUG, TAG, "Create g_deviceList");

        g_deviceList = u_arraylist_create();
    }
}

/**
 * Check Sent Count for remove g_sendBuffer
 */
void CALEClientUpdateSendCnt()
{
    OIC_LOG(DEBUG, TAG, "CALEClientUpdateSendCnt");

    // mutex lock
    oc_mutex_lock(g_threadMutex);

    g_currentSentCnt++;

    if (g_targetCnt <= g_currentSentCnt)
    {
        g_targetCnt = 0;
        g_currentSentCnt = 0;

        //init g_sendbuffer
        if (g_sendBuffer != nil ){//|| g_sendBuffer != NULL || [g_sendBuffer length] > 0){
            //[g_sendBuffer release];
            g_sendBuffer = nil;
        }

        // notity the thread
        oc_cond_signal(g_threadCond);

        CALEClientSetSendFinishFlag(true);

        OIC_LOG(DEBUG, TAG, "set signal for send data");
    }
    // mutex unlock
    oc_mutex_unlock(g_threadMutex);
}

CAResult_t CALEClientInitGattMutexVaraibles()
{
    if (NULL == g_bleReqRespClientCbMutex)
    {
        g_bleReqRespClientCbMutex = oc_mutex_new();
        if (NULL == g_bleReqRespClientCbMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_bleServerBDAddressMutex)
    {
        g_bleServerBDAddressMutex = oc_mutex_new();
        if (NULL == g_bleServerBDAddressMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_threadMutex)
    {
        g_threadMutex = oc_mutex_new();
        if (NULL == g_threadMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_threadSendMutex)
    {
        g_threadSendMutex = oc_mutex_new();
        if (NULL == g_threadSendMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_deviceListMutex)
    {
        g_deviceListMutex = oc_mutex_new();
        if (NULL == g_deviceListMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_gattObjectMutex)
    {
        g_gattObjectMutex = oc_mutex_new();
        if (NULL == g_gattObjectMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_deviceStateListMutex)
    {
        g_deviceStateListMutex = oc_mutex_new();
        if (NULL == g_deviceStateListMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_SendFinishMutex)
    {
        g_SendFinishMutex = oc_mutex_new();
        if (NULL == g_SendFinishMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_scanMutex)
    {
        g_scanMutex = oc_mutex_new();
        if (NULL == g_scanMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == waitMutex){
        waitMutex = oc_mutex_new();
    }

    if (NULL == g_threadWriteCharacteristicMutex)
    {
        g_threadWriteCharacteristicMutex = oc_mutex_new();
        if (NULL == g_threadWriteCharacteristicMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_deviceScanRetryDelayMutex)
    {
        g_deviceScanRetryDelayMutex = oc_mutex_new();
        if (NULL == g_deviceScanRetryDelayMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    if (NULL == g_initializeMutex)
    {
        g_initializeMutex = oc_mutex_new();
        if (NULL == g_initializeMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }

    return CA_STATUS_OK;
}

void CALEClientTerminateGattMutexVariables()
{
    oc_mutex_free(g_bleReqRespClientCbMutex);
    g_bleReqRespClientCbMutex = NULL;

    oc_mutex_free(g_bleServerBDAddressMutex);
    g_bleServerBDAddressMutex = NULL;

    oc_mutex_free(g_threadMutex);
    g_threadMutex = NULL;

    oc_mutex_free(g_threadSendMutex);
    g_threadSendMutex = NULL;

    oc_mutex_free(g_deviceListMutex);
    g_deviceListMutex = NULL;

    oc_mutex_free(g_SendFinishMutex);
    g_SendFinishMutex = NULL;

    oc_mutex_free(g_scanMutex);
    g_scanMutex = NULL;

    oc_mutex_free(waitMutex);
    waitMutex = NULL;

    oc_mutex_free(g_threadWriteCharacteristicMutex);
    g_threadWriteCharacteristicMutex = NULL;

    oc_mutex_free(g_deviceScanRetryDelayMutex);
    g_deviceScanRetryDelayMutex = NULL;

    oc_mutex_free(g_initializeMutex);
    g_initializeMutex = NULL;
}

void CALEClientSetSendFinishFlag(bool flag)
{
    OIC_LOG_V(DEBUG, TAG, "g_isFinishedSendData is %d", flag);

    oc_mutex_lock(g_SendFinishMutex);
    g_isFinishedSendData = flag;
    oc_mutex_unlock(g_SendFinishMutex);
}

/**
 * adapter common
 */

CAResult_t CAStartLEGattClient()
{
    // init mutex for send logic
    if (!g_deviceDescCond)
    {
        g_deviceDescCond = oc_cond_new();
    }

    if (!g_threadCond)
    {
        g_threadCond = oc_cond_new();
    }

    if (!g_threadWriteCharacteristicCond)
    {
        g_threadWriteCharacteristicCond = oc_cond_new();
    }

    g_isStartedLEClient = true;
    return CA_STATUS_OK;
}

void CAStopLEGattClient()
{
    OIC_LOG(DEBUG, TAG, "CAStopBLEGattClient");

    g_isStartedLEClient = false;

    CAResult_t ret = CALEClientDisconnectAll();
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEClientDisconnectAll has failed");
    }

    ret = CALEClientStopScan();
    if(CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CALEClientStopScan has failed");
    }

    oc_mutex_lock(g_threadMutex);
    OIC_LOG(DEBUG, TAG, "signal - connection cond");
    oc_cond_signal(g_threadCond);
    CALEClientSetSendFinishFlag(true);
    oc_mutex_unlock(g_threadMutex);

    oc_mutex_lock(g_threadWriteCharacteristicMutex);
    OIC_LOG(DEBUG, TAG, "signal - WriteCharacteristic cond");
    oc_cond_signal(g_threadWriteCharacteristicCond);
    oc_mutex_unlock(g_threadWriteCharacteristicMutex);

    oc_mutex_lock(g_deviceScanRetryDelayMutex);
    OIC_LOG(DEBUG, TAG, "signal - delay cond");
    oc_cond_signal(g_deviceScanRetryDelayCond);
    oc_mutex_unlock(g_deviceScanRetryDelayMutex);

    oc_cond_signal(g_deviceDescCond);   //change

    oc_cond_free(g_deviceDescCond);
    oc_cond_free(g_threadCond);
    oc_cond_free(g_threadWriteCharacteristicCond);
    oc_cond_free(g_deviceScanRetryDelayCond);

    g_deviceDescCond = NULL;
    g_threadCond = NULL;
    g_threadWriteCharacteristicCond = NULL;
    g_deviceScanRetryDelayCond = NULL;
}

CAResult_t CAInitializeLEGattClient()
{
    OIC_LOG(DEBUG, TAG, "Initialize GATT Client");
    CALEClientInitialize();
    return CA_STATUS_OK;
}

void CATerminateLEGattClient()
{
    OIC_LOG(DEBUG, TAG, "Terminate GATT Client");
    CAStopLEGattClient();
    CALEClientTerminate();
}

CAResult_t CAUpdateCharacteristicsToGattServer(const char *remoteAddress,
                                                const uint8_t  *data,
                                                uint32_t dataLen,
                                                CALETransferType_t type,
                                                int32_t position)
{
    OIC_LOG(DEBUG, TAG, "call CALEClientSendUnicastMessage");
    VERIFY_NON_NULL(data, TAG, "data is null");
    VERIFY_NON_NULL(remoteAddress, TAG, "remoteAddress is null");

    if (LE_UNICAST != type || position < 0)
    {
        OIC_LOG(ERROR, TAG, "this request is not unicast");
        return CA_STATUS_INVALID_PARAM;
    }

    return CALEClientSendUnicastMessage(remoteAddress, data, dataLen);
}

CAResult_t CAUpdateCharacteristicsToAllGattServers(const uint8_t *data, uint32_t dataLen)
{
    OIC_LOG(DEBUG, TAG, "call CALEClientSendMulticastMessage");
    VERIFY_NON_NULL(data, TAG, "data is null");

    return CALEClientSendMulticastMessage(data, dataLen);
}

void CASetLEReqRespClientCallback(CABLEDataReceivedCallback callback)
{
    oc_mutex_lock(g_bleReqRespClientCbMutex);
    g_CABLEClientDataReceivedCallback = callback;
    oc_mutex_unlock(g_bleReqRespClientCbMutex);
}

void CASetLEClientThreadPoolHandle(ca_thread_pool_t handle)
{
    g_threadPoolHandle = handle;
}

CAResult_t CAGetLEAddress(char **local_address){
    return CA_NOT_SUPPORTED;
}

void CALEClientSetFlagBTAdapter(bool state){
    isEnableBtAdapter = state;
}

bool CALEClientIsEnableBTAdapter(){
    return isEnableBtAdapter;
}
