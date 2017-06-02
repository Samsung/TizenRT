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
 *
 * This file contains common utility function for CA transport adaptors.
 */

#ifndef CA_ADAPTER_UTILS_H_
#define CA_ADAPTER_UTILS_H_

#include "iotivity_config.h"

#include <stdbool.h>
#ifdef __ANDROID__
#include <jni.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#if defined(HAVE_WINSOCK2_H) && defined(HAVE_WS2TCPIP_H)
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#include "cacommon.h"
#include "logger.h"
#include <coap/pdu.h>
#include "uarraylist.h"
#include "cacommonutil.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Length of network interface name.
 */
#define CA_INTERFACE_NAME_SIZE 16

/**
 * Macro to allocate memory for ipv4 address in the form of uint8_t.
 */
#define IPV4_ADDR_ONE_OCTECT_LEN 4

#ifdef SINGLE_THREAD
/**
 * Network Interface Information. Only needed for Arduino.
 */
typedef struct
{
    char ipAddress[CA_IPADDR_SIZE];             /**< Address of the interface. **/
    char subnetMask[CA_IPADDR_SIZE];            /**< Maintains interface subnetmask. **/
    char interfaceName[CA_INTERFACE_NAME_SIZE]; /**< Interface name. **/
} CANetInfo_t;
#endif

/**
 * unicast and multicast server information.
 */
typedef struct
{
    int socketFd;                               /**< Socket descriptor. **/
    CAEndpoint_t endpoint;                      /**< endpoint description. **/
    bool isServerStarted;                       /**< Indicates server started. **/
    bool isMulticastServer;                     /**< Indicates multicast server. **/
    char ifAddr[CA_IPADDR_SIZE];                /**< Address of the multicast interface. **/
    char interfaceName[CA_INTERFACE_NAME_SIZE]; /**< Interface Name. **/
    char subNetMask[CA_IPADDR_SIZE];            /**< Subnet Mask. **/
} CAServerInfo_t;

/**
 * To parse the IP address and port from "ipaddress:port".
 * @param[in]   ipAddrStr       IP address to be parsed.
 * @param[out]  ipAddr          Parsed IP address.
 * @param[in]   ipAddr          Buffer length for parsed IP address.
 * @param[out]  port            Parsed Port number.
 * @return ::CA_STATUS_OK or Appropriate error code.
 */
CAResult_t CAParseIPv4AddressInternal(const char *ipAddrStr, uint8_t *ipAddr,
                                      size_t ipAddrLen, uint16_t *port);

/**
 * Check if two ip address belong to same subnet.
 * @param[in]   ipAddress1      IP address to be checked.
 * @param[in]   ipAddress2      IP address to be checked.
 * @param[in]   netMask         Subnet mask.
 * @return  true if same subnet and false if not same subnet.
 */
bool CAAdapterIsSameSubnet(const char *ipAddress1, const char *ipAddress2,
                           const char *netMask);
/**
 * Used to check the multicast server is running or not.
 *
 * @param[in]   serverInfoList    Server information list.
 * @param[in]   ipAddress         Interface address of the server.
 * @param[in]   multicastAddress  Multicast address of the server.
 * @param[in]   port              Port number of the server.
 *
 * @return  true or false.
 */
bool CAIsMulticastServerStarted(const u_arraylist_t *serverInfoList, const char *ipAddress,
                                const char *multicastAddress, uint16_t port);

/**
 * Used to check the unicast server is running or not.
 *
 * @param[in]   serverInfoList   Server information list.
 * @param[in]   ipAddress        Ip address of the server.
 * @param[in]   port             Port number of the server.
 *
 * @return  true or false.
 */
bool CAIsUnicastServerStarted(const u_arraylist_t *serverInfoList, const char *ipAddress,
                              uint16_t port);

/**
 * Used to get the port number based on given information.
 *
 * @param[in]   serverInfoList   Server information list.
 * @param[in]   ipAddress        Ip address of the server.
 * @param[in]   isSecured        specifies whether to get secured or normal unicast server port.
 *
 * @return  positive value on success and 0 on error.
 */
uint16_t CAGetServerPort(const u_arraylist_t *serverInfoList, const char *ipAddress,
                         bool isSecured);

/**
 * Used to get the socket fd for given server information.
 *
 * @param[in]   serverInfoList   Server information list.
 * @param[in]   isMulticast      To check whether it is multicast server or not.
 * @param[in]   endpoint         network address

 * @return  positive value on success and -1 on error.
 */
int CAGetSocketFdForUnicastServer(const u_arraylist_t *serverInfoList,
                         bool isMulticast, const CAEndpoint_t *endpoint);

/**
 * Used to add the server information into serverinfo list.
 *
 * @param[in/out]   serverInfoList    server information list.
 * @param[in]       info              server informations like ip, port.
 *
 * @return ::CA_STATUS_OK or Appropriate error code.
 * @retval ::CA_STATUS_OK  Successful.
 * @retval ::CA_STATUS_INVALID_PARAM Invalid input data.
 * @retval ::CA_STATUS_FAILED Initialization failed.
 */
CAResult_t CAAddServerInfo(u_arraylist_t *serverInfoList, CAServerInfo_t *info);

/**
 * Used to remove the server information based on socket fd from server info list.
 *
 * @param[in/out]   serverInfoList    server information list.
 * @param[in]       sockFd            Socket descriptor.
 */
void CARemoveServerInfo(u_arraylist_t *serverInfoList, int sockFd);

/**
 * Used to clear the memory of network interface list.
 * Memory pointed by infoList will become invalid after this function call.
 *
 * @param[in]   infoList    Network interface list.
 */
void CAClearNetInterfaceInfoList(u_arraylist_t *infoList);

/**
 * Used to clear the memory of server info list.
 * Memory pointed by serverInfoList will become invalid after this function call.
 *
 * @param[in]   infoList    Server information list.
 */
void CAClearServerInfoList(u_arraylist_t *serverInfoList);

#ifndef WITH_ARDUINO
/**
 * Convert address from binary to string.
 * @param[in]    sockAddr     IP address info.
 * @param[in]    sockAddrLen  size of sockAddr.
 * @param[out]   host         address string (must be CA_IPADDR_SIZE).
 * @param[out]   port         host order port number.
 */
void CAConvertAddrToName(const struct sockaddr_storage *sockAddr, socklen_t sockAddrLen,
                         char *host, uint16_t *port);

/**
 * Convert address from string to binary.
 * @param[in]   host      address string.
 * @param[in]   port      host order port number.
 * @param[out]  ipaddr    IP address info.
 */
void CAConvertNameToAddr(const char *host, uint16_t port, struct sockaddr_storage *sockaddr);
#endif /* WITH_ARDUINO */

#ifdef __ANDROID__
/**
 * To set context of JNI Application.
 * This must be called by the Android API before CA Initialization.
 * @param[in]   env         JNI interface pointer.
 * @param[in]   context     context object.
 */
void CANativeJNISetContext(JNIEnv *env, jobject context);

/**
 * To set jvm object.
 * This must be called by the Android API before CA Initialization.
 * @param[in]   jvm         jvm object.
 */
void CANativeJNISetJavaVM(JavaVM *jvm);

/**
 * To get context.
 * Called by adapters to get Application context.
 * @return  context object.
 */
jobject CANativeJNIGetContext();

/**
 * To get JVM object.
 * Called from adapters to get JavaVM object.
 * @return  JVM object.
 */
JavaVM *CANativeJNIGetJavaVM();

/**
 * To set Activity to JNI.
 * This must be called by the Android API before CA Initialization.
 * @param[in]   env         JNI Environment pointer.
 * @param[in]   activity    Activity object.
 */
void CANativeSetActivity(JNIEnv *env, jobject activity);

/**
 * To get Activity.
 * Called from adapters to get Activity.
 * @return  Activity object.
 */
jobject *CANativeGetActivity();

/**
 * get method ID for method Name and class
 * @param[in]   env              JNI interface pointer.
 * @param[in]   className        android class.
 * @param[in]   methodName       android method name.
 * @param[in]   methodFormat     method type of methodName.
 * @return      jmethodID        iD of the method.
 */
jmethodID CAGetJNIMethodID(JNIEnv *env, const char* className,
                           const char* methodName,
                           const char* methodFormat);

/**
 * check JNI exception occurrence
 * @param[in]   env              JNI interface pointer.
 * @return  true(occurrence) or false(no occurrence).
 */
bool CACheckJNIException(JNIEnv *env);

/**
 * To Delete other Global References
 * Called during CATerminate to remove global references
 */
void CADeleteGlobalReferences();

#endif

#ifndef WITH_ARDUINO
/**
 * print send state in the adapter.
 * @param[in]   adapter          transport adapter type.
 * @param[in]   addr             remote address.
 * @param[in]   port             port.
 * @param[in]   sentLen          sent data length.
 * @param[in]   isSuccess        sent state.
 * @param[in]   message          detailed message.
 */
void CALogSendStateInfo(CATransportAdapter_t adapter,
                        const char *addr, uint16_t port, ssize_t sentLen,
                        bool isSuccess, const char* message);

/**
 * print adapter state in the adapter.
 * @param[in]   adapter          transport adapter type.
 * @param[in]   state            adapter state.
 */
void CALogAdapterStateInfo(CATransportAdapter_t adapter, CANetworkStatus_t state);

/**
 * print adapter type name in the adapter.
 * @param[in]   adapter          transport adapter type.
 */
void CALogAdapterTypeInfo(CATransportAdapter_t adapter);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif  /* CA_ADAPTER_UTILS_H_ */

