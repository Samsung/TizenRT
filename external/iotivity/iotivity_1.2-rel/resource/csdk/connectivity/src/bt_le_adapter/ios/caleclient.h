/* ****************************************************************
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

/**
 * @file
 * This file contains the APIs for BT LE communications.
 */
#ifndef CA_LECLIENT_H_
#define CA_LECLIENT_H_

#include "cacommon.h"
#include "cathreadpool.h"
#include "uarraylist.h"

#include "calenwmonitor.h"
#include "caleutils.h"

#ifdef __cplusplus
extern "C"
{
#endif

static const uint16_t GATT_ERROR = 133;

static const uint16_t STATE_CHARACTER_SET = 2;
static const uint16_t STATE_CHARACTER_UNSET = 1;
static const uint16_t STATE_CHARACTER_NO_CHANGE = 0;

static const uint16_t STATE_SEND_NONE = 0;
static const uint16_t STATE_SEND_SUCCESS = 1;
static const uint16_t STATE_SEND_FAILED = 2;

typedef struct le_state_info
{
    char address[CA_MACADDR_SIZE];
    int connectedState;
    uint16_t notificationState;
    uint16_t sendState;
    bool autoConnectFlag;
} CALEState_t;

/**
 * Callback to be notified on reception of any data from remote devices.
 * @param[in]  address                MAC address of remote device.
 * @param[in]  data                   Data received from remote device.
 * @pre  Callback must be registered using CALESetCallback(CAPacketReceiveCallback callback)
 */
typedef void (*CAPacketReceiveCallback)(const char *address,
                                        const uint8_t *data);

typedef void (*CAManagerAdapterStateChangeCallback)(CBCentralManager *central);
typedef void (*CAManagerConnectionCallback)(CBPeripheral *peripheral, const char *remote_address,
                                            bool connected, NSError *error);
typedef void (*CAManagerServiceDiscoveredCallback)(CBPeripheral *peripheral,
                                                   const char *remote_address, NSError *error);

/**
 * initialize client for BLE.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientInitialize();

/**
 * terminate client for BLE.
 */
void CALEClientTerminate();


/**
 * for destroy sending routine.
 * @param[in]   peripheral    remotePeripheral object
 */
void CALEClientSendFinish(CBPeripheral *peripheral);

/**
 * send data for unicast (interface).
 * @param[in]   address               remote address.
 * @param[in]   data                  data for transmission.
 * @param[in]   dataLen               data length.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientSendUnicastMessage(const char *address,
                                        const uint8_t *data,
                                        const uint32_t dataLen);

/**
 * send data for multicast (interface).
 * @param[in]   data                  data for transmission.
 * @param[in]   dataLen               data length.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientSendMulticastMessage(const uint8_t *data, const uint32_t dataLen);

/**
 * start unicast server.
 * @param[in]   address               remote address.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientStartUnicastServer(const char *address);

/**
 * start multicast server (start discovery).
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientStartMulticastServer();

/**
 * stop unicast server.
 */
void CALEClientStopUnicastServer();

/**
 * stop multicast server (stop discovery).
 */
void CALEClientStopMulticastServer();

/**
 * set this callback for receiving data packets from peer devices.
 * @param[in]   callback              callback to be notified on reception of
 *                                    unicast/multicast data packets.
 */
void CALEClientSetCallback(CAPacketReceiveCallback callback);

void CALEClientSetCAManagerCallback(CAManagerAdapterStateChangeCallback adapterStateChangeCB,
                                    CAManagerConnectionCallback connectionCB,
                                    CAManagerServiceDiscoveredCallback serviceDiscoverdCB);

/**
 * waiting to get scanned device from BT Platform.
 * if there is no scanned device in the list.
 * @param[in]   address               LE address.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientIsThereScannedDevices(const char* address);

/**
 * send data for unicast (implement).
 * @param[in]   address               remote address.
 * @param[in]   data                  data for transmission.
 * @param[in]   dataLen               data length.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientSendUnicastMessageImpl(const char *address,
                                            const uint8_t *data,
                                            const uint32_t dataLen);

/**
 * send data for multicast (implement).
 * @param[in]   data                  data for transmission.
 * @param[in]   dataLen               data length.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientSendMulticastMessageImpl(const uint8_t *data,
                                              const uint32_t dataLen);

/**
 * check whether it is connected or not with remote address.
 * @param[in]   address               remote address.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALECheckSendState(const char* address);

/**
 * send data to remote device.
 * if it isn't connected yet. connect LE before try to send data.
 * @param[in]   peripheral          remote peripheral object
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientSendData(CBPeripheral *peripheral);

/**
 * get address from bluetooth gatt object.
 * @param[in]   peripheral        remote peripheral Object
 * @return  bluetooth address.
 */
NSString *CALEClientGetAddressFromGattObj(CBPeripheral *peripheral);

/**
 * get remote address from bluetooth socket object.
 * @param[in]   peripheral        remote peripheral Object
 * @return  bluetooth address.
 */
NSString *CALEClientGetRemoteAddress(CBPeripheral *peripheral);

/**
 * close gatt.
 * @param[in]   peripheral        remote peripheral Object
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientGattClose(CBPeripheral *peripheral);

/**
 * start to scan whole bluetooth devices (interface).
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientStartScan();

/**
 * stop scan (interface).
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientStopScan();

/**
 * set ble scanning flag.
 * @param[in]   flag        scan flag.
 */
void CALEClientSetScanFlag(bool flag);

/**
 * set auto connect flag for connectGatt API.
 * @param[in]   address           remote address.
 * @param[in]   flag              auto connect flag.
 */
CAResult_t CALEClientSetAutoConnectFlag(const char *remoteAddress, bool flag);

/**
 * get auto connect flag.
 * @param[in]   address           remote address.
 * @return  current auto connect flag;
 */
bool CALEClientGetAutoConnectFlag(const char *address);

/**
 * connect to gatt server hosted.
 * @param[in]   bluetoothDevice       bluetooth Device object.
 * @param[in]   autoconnect           whether to directly connect to the remote device(false) or
 *                                     to automatically connect as soon as the remote device
 *                                     becomes available.
 * @return  void
 */
CAResult_t CALEClientConnect(CBPeripheral *peripheral, bool autoconnect);

/**
 * disconnect to gatt server by a target device.
 * @param[in]   peripheral        remote peripheral Object
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientDisconnect(CBPeripheral *peripheral);

/**
 * cancel to gatt connecting by a target device.
 * @param[in]   peripheral        remote peripheral Object
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientConnectCancel(CBPeripheral *peripheral);

/**
 * set retreived peripheral to scan device list.
 * @param[in]   peripheral        remote peripheral Object
 * @return  ::remote_address.
 */

/**
 * disconnect to gatt server by whole devices.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientDisconnectAll();

/**
 * disconnect to gatt server by selected address.
 * @param[in]   remoteAddress         remote address.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientDisconnectforAddress(const char *remoteAddress);

/**
 * start discovery server.
 * @param[in]   peripheral        remote peripheral Objectyy
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientDiscoverServices(CBPeripheral *peripheral);

/**
 * call CALESetValueAndWriteCharacteristic when connection is successful.
 * @param[in]   peripheral        remote peripheral Objectyy
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientWriteCharacteristic(CBPeripheral *peripheral);


/**
 * create GattCharacteristic and call CALEClientWriteCharacteristicImpl
 * for request to write gatt characteristic.
 * @param[in]   peripheral        remote peripheral Objectyy
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */

/**
 * request to write gatt characteristic.
 * @param[in]   peripheral        remote peripheral Objectyy
 * @param[in]   characteristic    characteristic object that contain data to send.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientWriteCharacteristicImpl(CBPeripheral *peripheral,
                                             CBCharacteristic *characteristic);

/**
 * enable notification for a target device.
 * @param[in]   peripheral        remote peripheral Objectyy
 * @param[in]   characteristic    characteristic object that contain data to send.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientSetCharacteristicNotification(CBPeripheral *peripheral,
                                                   CBCharacteristic *characteristic);

/**
 * create gatt characteristic object.
 * @param[in]   peripheral        remote peripheral Objectyy
 * @return  CBCharacteristic handler
 */
CBCharacteristic *CALEClientCreateGattCharacteristic(CBPeripheral *peripheral);

/**
 * add device object to scan device list.
 * @param[in]   device                bluetooth device object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientAddScanDeviceToList(CBPeripheral *peripheral);

/**
 * check whether the device exist in list or not.
 * @param[in]   remoteAddress         remote address.
 * @return  true or false.
 */
bool CALEClientIsDeviceInScanDeviceList(const char *remoteAddress);

/**
 * remove all devices in scan device list.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientRemoveAllScanDevices();

/**
 * remove target device in scan device list.
 * @param[in]   remoteAddress         remote address.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientRemoveDeviceInScanDeviceList(NSString *remoteAddress);

/**
 * add gatt object to gatt object list.
 * @param[in]   gatt                  Gatt profile object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientAddGattobjToList(CBPeripheral *peripheral);

/**
 * check whether the gatt object exist in list or not.
 * @param[in]   remoteAddress         remote address.
 * @return  true or false.
 */
bool CALEClientIsGattObjInList(const char *remoteAddress);

/**
 * get the gatt object.
 * @param[in]   remoteAddress         remote address.
 * @return  gatt object.
 */
CBPeripheral *CALEClientGetGattObjInList(const char* remoteAddress);

/**
 * remove all gatt objects in gatt object list.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientRemoveAllGattObjs();

/**
 * remove target device in gatt object list.
 * @param[in]   gatt                  Gatt profile object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientRemoveGattObj(CBPeripheral *peripheral);

/**
 * remove gatt object of target device for address in gatt object list.
 * @param[in]   gatt                  Gatt profile object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientRemoveGattObjForAddr(const char* addr);

/**
 * get ble address from Bluetooth device.
 * @param[in]   bluetoothDevice       Bluetooth device.
 * @return  ble address.
 */
NSString *CALEClientGetLEAddressFromBTDevice(CBPeripheral *peripheral);

/**
 * update new state information.
 * @param[in]   address               remote address.
 * @param[in]   connectedState        connection state.
 * @param[in]   notificationState     whether characteristic notification already set or not.
 * @param[in]   sendState             whether sending was success or not.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientUpdateDeviceState(const char* address,
                                    uint32_t connectedState,
                                    uint16_t notificationState,
                                    uint16_t sendState);

/**
 * add new state to state list.
 * @param[in]   state                 new state.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientAddDeviceStateToList(CALEState_t* state);

/**
 * check whether the remote address is existed or not.
 * @param[in]   address               remote address.
 * @return  true or false.
 */
bool CALEClientIsDeviceInList(const char *remoteAddress);

/**
 * remove all device states.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientRemoveAllDeviceState();

/**
 * Reset values of device state for all of devices.
 * this method has to be invoked when BT adapter is disabled.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientResetDeviceStateForAll();

/**
 * remove the device state for a remote device.
 * @param[in]   remoteAddress         remote address.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientRemoveDeviceState(const char* remoteAddress);

/**
 * get state information for a remote device.
 * @param[in]   remoteAddress         remote address.
 * @return  CALEState_t.
 */
CALEState_t* CALEClientGetStateInfo(const char* remoteAddress);

/**
 * check whether the remote address is connected or not.
 * @param[in]   remoteAddress         remote address.
 * @return  true or false.
 */
bool CALEClientIsConnectedDevice(const char* remoteAddress);

/**
 * check whether the remote address set CharacteristicNotification or not.
 * @param[in]   remoteAddress         remote address.
 * @return  true or false.
 */
bool CALEClientIsSetCharacteristic(const char* remoteAddress);

/**
 * create scan device list.
 */
void CALEClientCreateDeviceList();

/**
 * update the counter which data is sent to remote device.
 */
void CALEClientUpdateSendCnt();

/**
 * initialize mutex.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientInitGattMutexVaraibles();

/**
 * terminate mutex.
 */
void CALEClientTerminateGattMutexVariables();

/**
 * set send finish flag.
 * @param[in]   flag        finish flag.
 */
void CALEClientSetSendFinishFlag(bool flag);

static bool isEnableBtAdapter = false;
bool CALEClientIsEnableBTAdapter();
void CALEClientSetFlagBTAdapter(bool state);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CA_LECLIENT_H_ */
