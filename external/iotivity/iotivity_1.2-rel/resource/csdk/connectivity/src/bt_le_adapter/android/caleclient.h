/* ****************************************************************
 *
 * Copyright 2014 Samsung Electronics All Rights Reserved.
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

#include "calestate.h"
#include "cacommon.h"
#include "cathreadpool.h"
#include "uarraylist.h"
#include "jni.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * BLE Scanning State.
 */
typedef enum
{
    BLE_SCAN_ENABLE = 0, /**< BLE scan is working */
    BLE_SCAN_DISABLE,    /**< BLE scan is not working */
    BLE_SCAN_NONE        /**< Initialize State */
} CALEScanState_t;

/**
 * Callback to be notified on reception of any data from remote devices.
 * @param[in]  address                MAC address of remote device.
 * @param[in]  data                   Data received from remote device.
 * @pre  Callback must be registered using CALESetCallback(CAPacketReceiveCallback callback)
 */
typedef void (*CAPacketReceiveCallback)(const char *address, const uint8_t *data);

/**
 * initialize JNI object.
 */
void CALEClientJniInit();

/**
 * set context of application.
 */
void CALEClientJNISetContext();

/**
 * create interface object and initialize the object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientCreateJniInterfaceObject();

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
 * destroy interface object and terminate the interface.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientDestroyJniInterface();

/**
 * for destroy sending routine.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   gatt                  Gatt profile object.
 */
void CALEClientSendFinish(JNIEnv *env, jobject gatt);

/**
 * send negotiation message after gatt connection is done.
 * @param[in]   address               remote address.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientSendNegotiationMessage(const char* address);

/**
 * send data for unicast (interface).
 * @param[in]   address               remote address.
 * @param[in]   data                  data for transmission.
 * @param[in]   dataLen               data length.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientSendUnicastMessage(const char *address, const uint8_t *data,
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

/**
 * waiting to get scanned device from BT Platform.
 * if there is no scanned device in the list.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   address               LE address.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientIsThereScannedDevices(JNIEnv *env, const char* address);

/**
 * send data for unicast (implement).
 * @param[in]   address               remote address.
 * @param[in]   data                  data for transmission.
 * @param[in]   dataLen               data length.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientSendUnicastMessageImpl(const char *address, const uint8_t *data,
                                            const uint32_t dataLen);

/**
 * send data for multicast (implement).
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   data                  data for transmission.
 * @param[in]   dataLen               data length.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientSendMulticastMessageImpl(JNIEnv *env, const uint8_t *data,
                                              const uint32_t dataLen);

/**
 * send data to remote device.
 * if it isn't connected yet. connect LE before try to send data.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   device                bluetooth device object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientSendData(JNIEnv *env, jobject device);

/**
 * get address from bluetooth gatt object.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   gatt                  Gatt profile object.
 * @return  bluetooth address.
 */
jstring CALEClientGetAddressFromGattObj(JNIEnv *env, jobject gatt);

/**
 * get remote address from bluetooth socket object.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   bluetoothSocketObj    bluetooth socket.
 * @return  bluetooth address.
 */
jstring CALEClientGetRemoteAddress(JNIEnv *env, jobject bluetoothSocketObj);

/**
 * close gatt.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   bluetoothGatt         gatt profile object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientGattClose(JNIEnv *env, jobject bluetoothGatt);

/**
 * start to scan whole bluetooth devices (interface).
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientStartScan();

/**
 * start to scan whole bluetooth devices (implement).
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   callback              callback to receive device object by scanning.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientStartScanImpl(JNIEnv *env, jobject callback);

/**
 * start to scan whole bluetooth devices for android API level 21 (implement).
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   callback              callback to receive device object by scanning.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientStartScanImplForV21(JNIEnv *env, jobject callback);

/**
 * start to scan target bluetooth devices for service uuid (implement).
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   uuids                 target UUID.
 * @param[in]   callback              callback to receive device object by scanning.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientStartScanWithUUIDImpl(JNIEnv *env, jobjectArray uuids,
                                           jobject callback);

/**
 * start to scan target bluetooth devices for service uuid for android API level 21 (implement).
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   uuids                 target UUID.
 * @param[in]   callback              callback to receive device object by scanning.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientStartScanWithUUIDImplForV21(JNIEnv *env, jobjectArray uuids,
                                                 jobject callback);

/**
 * get uuid object.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   uuid                  uuid.
 * @return  uuid object.
 */
jobject CALEClientGetUUIDObject(JNIEnv *env, const char *uuid);

/**
 * stop scan (interface).
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientStopScan();

/**
 * stop scan (implement).
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   callback              callback to receive device object by scanning.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientStopScanImpl(JNIEnv *env, jobject callback);

/**
 * stop scan for android API level 21(implement).
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   callback              callback to receive device object by scanning.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientStopScanImplForV21(JNIEnv *env, jobject callback);

/**
 * connect to gatt server hosted.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   bluetoothDevice       bluetooth Device object.
 * @param[in]   autoconnect           whether to directly connect to the remote device(false) or
 *                                     to automatically connect as soon as the remote device
 *                                     becomes available.
 * @return  gatt object
 */
jobject CALEClientConnect(JNIEnv *env, jobject bluetoothDevice, jboolean autoconnect);

/**
 * disconnect to gatt server by a target device.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   bluetoothGatt         Gatt profile object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientDisconnect(JNIEnv *env, jobject bluetoothGatt);

/**
 * disconnect to gatt server by whole devices.
 * @param[in]   env                   JNI interface pointer.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientDisconnectAll(JNIEnv *env);

/**
 * disconnect to gatt server by selected address.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   remoteAddress         remote address.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientDisconnectforAddress(JNIEnv *env, jstring remoteAddress);

/**
 * request MTU size negotiation to server.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   bluetoothGatt         gatt object.
 * @param[in]   size                  MTU size.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientRequestMTU(JNIEnv *env, jobject bluetoothGatt, jint size);
/**
 * start discovery server.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   bluetoothGatt         Gatt profile object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientDiscoverServices(JNIEnv *env, jobject bluetoothGatt);

/**
 * call CALESetValueAndWriteCharacteristic when connection is successful.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   gatt                  Gatt profile object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientWriteCharacteristic(JNIEnv *env, jobject gatt);

/**
 * create GattCharacteristic and call CALEClientWriteCharacteristicImpl
 * for request to write gatt characteristic.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   gatt                  Gatt profile object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALESetValueAndWriteCharacteristic(JNIEnv *env, jobject gatt);

/**
 * request to write gatt characteristic.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   bluetoothGatt         Gatt profile object.
 * @param[in]   gattCharacteristic    characteristic object that contain data to send.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientWriteCharacteristicImpl(JNIEnv *env, jobject bluetoothGatt,
                                             jobject gattCharacteristic);

/**
 * request to read gatt characteristic.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   bluetoothGatt         Gatt profile object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientReadCharacteristic(JNIEnv *env, jobject bluetoothGatt);

/**
 * enable notification for a target device.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   bluetoothGatt         Gatt profile object.
 * @param[in]   characteristic        Characteristic object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientSetCharacteristicNotification(JNIEnv *env, jobject bluetoothGatt,
                                                  jobject characteristic);

/**
 * create gatt characteristic object.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   bluetoothGatt         Gatt profile object.
 * @param[in]   data                  for make Characteristic with data.
 * @return  Gatt Characteristic object.
 */
jobject CALEClientCreateGattCharacteristic(JNIEnv *env, jobject bluetoothGatt, jbyteArray data);

/**
 * get gatt service.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   bluetoothGatt         Gatt profile object.
 * @param[in]   characterUUID         for make BluetoothGattCharacteristic object.
 * @return  Gatt Service.
 */
jobject CALEClientGetGattService(JNIEnv *env, jobject bluetoothGatt, jstring characterUUID);

/**
 * get value from characteristic.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   characteristic        Characteristic object.
 * @return  value in characteristic.
 */
jbyteArray CALEClientGetValueFromCharacteristic(JNIEnv *env, jobject characteristic);

/**
 * create UUID List.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientCreateUUIDList();

/**
 * set UUID to descriptor.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   bluetoothGatt         Gatt profile object.
 * @param[in]   characteristic        Characteristic object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientSetUUIDToDescriptor(JNIEnv *env, jobject bluetoothGatt,
                                         jobject characteristic);

/**
 * add device object to scan device list.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   device                bluetooth device object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientAddScanDeviceToList(JNIEnv *env, jobject device);

/**
 * check whether the device exist in list or not.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   remoteAddress         remote address.
 * @return  true or false.
 */
bool CALEClientIsDeviceInScanDeviceList(JNIEnv *env, const char *remoteAddress);

/**
 * remove all devices in scan device list.
 * @param[in]   env                   JNI interface pointer.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientRemoveAllScanDevices(JNIEnv *env);

/**
 * remove target device in scan device list.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   remoteAddress         remote address.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientRemoveDeviceInScanDeviceList(JNIEnv *env, jstring remoteAddress);

/**
 * add gatt object to gatt object list.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   gatt                  Gatt profile object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientAddGattobjToList(JNIEnv *env, jobject gatt);

/**
 * check whether the gatt object exist in list or not.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   remoteAddress         remote address.
 * @return  true or false.
 */
bool CALEClientIsGattObjInList(JNIEnv *env, const char *remoteAddress);

/**
 * get the gatt object.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   remoteAddress         remote address.
 * @return  gatt object.
 */
jobject CALEClientGetGattObjInList(JNIEnv *env, const char* remoteAddress);

/**
 * remove all gatt objects in gatt object list.
 * @param[in]   env                   JNI interface pointer.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientRemoveAllGattObjs(JNIEnv *env);

/**
 * remove target device in gatt object list.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   gatt                  Gatt profile object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientRemoveGattObj(JNIEnv *env, jobject gatt);

/**
 * remove gatt object of target device for address in gatt object list.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   gatt                  Gatt profile object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientRemoveGattObjForAddr(JNIEnv *env, jstring addr);

/**
 * get ble address from Bluetooth device.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   bluetoothDevice       Bluetooth device.
 * @return  ble address.
 */
jstring CALEClientGetLEAddressFromBTDevice(JNIEnv *env, jobject bluetoothDevice);

/**
 * update new state information by Bluetooth device object.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   device                Bluetooth device.
 * @param[in]   state_type            state type.
 * @param[in]   target_state          state index to update.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientUpdateDeviceStateWithBtDevice(JNIEnv *env,
                                                   jobject device,
                                                   uint16_t state_type,
                                                   uint16_t target_state);

/**
 * set flag into State List.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   jni_address           remote address.
 * @param[in]   state_idx             state index.
 * @param[in]   flag                  auto connect flag.
 */
CAResult_t CALEClientSetFlagToState(JNIEnv *env, jstring jni_address,
                                    jint state_idx, jboolean flag);

/**
 * get flag from State List.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   jni_address           remote address.
 * @param[in]   state_idx             state index.
 * @return  current flag;
 */
jboolean CALEClientGetFlagFromState(JNIEnv *env, jstring jni_address, jint state_idx);

/**
 * get MTU size.
 * @param[in] address      the address of the remote device.
 * @return  mtu size negotiated from remote device.
 */
uint16_t CALEClientGetMtuSize(const char* address);

/**
 * Reset values of device state for all of devices.
 * this method has to be invoked when BT adapter is disabled.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientResetDeviceStateForAll();

/**
 * create scan device list.
 */
void CALEClientCreateDeviceList();

/**
 * update the counter which data is sent to remote device.
 * @param[in]   env                   JNI interface pointer.
 */
void CALEClientUpdateSendCnt(JNIEnv *env);

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

/**
 * close the connection of the profile proxy to the Service.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   gatt                  gatt profile object.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientCloseProfileProxy(JNIEnv *env, jobject gatt);

/**
 * connect to GATT Server hosted by this device.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   bluetoothDevice       bluetooth device object.
 * @param[in]   autoconnect           connect as soon as the device becomes avaiable(true).
 * @return  gatt profile object
 */
jobject CALEClientGattConnect(JNIEnv *env, jobject bluetoothDevice, jboolean autoconnect);

/**
 * connect to GATT Server hosted by this device directly.
 * @param[in]   env                   JNI interface pointer.
 * @param[in]   bluetoothDevice       bluetooth device object.
 * @param[in]   autoconnect           connect as soon as the device becomes avaiable(true).
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientDirectConnect(JNIEnv *env, jobject bluetoothDevice, jboolean autoconnect);

/**
 * check connection status.
 * @param[in] address      the address of the remote device.
 * @return  true or false.
 */
bool CALEClientIsConnected(const char* address);

/**
 * set new interval time and working count.
 * @param[in]  intervalTime             interval time(Seconds).
 * @param[in]  workingCount             working count for selected interval time.
 * @param[in]  nextScanningStep         set next scanning state.
 */
void CALEClientSetScanInterval(int32_t intervalTime, int32_t workingCount,
                               CALEScanState_t nextScanningStep);

/**
 * restart scanning with new interval time and working count.
 * @param[in]  intervalTime             interval time(Seconds).
 * @param[in]  workingCount             working count for selected interval time.
 * @param[in]  nextScanningStep         set next scanning state.
 */
void CALERestartScanWithInterval(int32_t intervalTime, int32_t workingCount,
                                 CALEScanState_t nextScanningStep);

/**
 * start LE scanning logic with interval time and working count.
 * @return  ::CA_STATUS_OK or ERROR CODES (::CAResult_t error codes in cacommon.h).
 */
CAResult_t CALEClientStartScanWithInterval();

/**
 * stop LE scanning logic with interval time and cycle.
 */
void CALEClientStopScanWithInterval();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CA_LECLIENT_H_ */
