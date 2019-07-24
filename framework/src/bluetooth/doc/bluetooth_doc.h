/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __TIZEN_NETWORK_BLUETOOTH_DOC_H__
#define __TIZEN_NETWORK_BLUETOOTH_DOC_H__

/**
 * @defgroup CAPI_NETWORK_BLUETOOTH_MODULE Bluetooth
 * @brief Bluetooth Service provides API to manage Bluetooth device.
 * @ingroup CAPI_NETWORK_FRAMEWORK
 *
 * @section CAPI_NETWORK_BLUETOOTH_MODULE_HEADER Required Header
 *   \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_MODULE_OVERVIEW Overview
 * Bluetooth Service consists of @ref CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE, @ref CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE and @ref CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE.
 *
 *<BR>
 * In order to use the above APIs, you must call #bt_initialize() in advance.
 * In addition, #bt_deinitialize() should be called when Bluetooth Service is no longer needed.
 * Please refer Bluetooth Tutorial if you want to get more detailed usages and information of this api.
 *
 * Note on callbacks: All callbacks in this module are called in the main loop context.
 *
 * @section CAPI_NETWORK_BLUETOOTH_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE,thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

/**
 * @defgroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE Bluetooth Adapter
 * @brief Bluetooth Adapter API provides functions for setting up Bluetooth and discovering other devices.
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE_OVERVIEW Overview
 * This set of function is used to control a bluetooth adapter.
 * You can also control visibility of the device, its appearance for others (adapter name).
 * In addition, this api is used to discover neighboring bluetooth devices.
 * This process is asynchronous, so it is up to you to build and hold list of devices in the neighborhood
 * - the api does not provide this list.
 * Before starting a device discovery, you can find a device from the connected devices.
 * This functionality is implemented by foreach loop.
 *
 * @section CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE,thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 * @section CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE_ASYNCHRONOUS_OPERATIONS Asynchronous Operations
 * <div><table class="doxtable" >
 *     <tr>
 *        <th><b>FUNCTION</b></th>
 *        <th><b>CALLBACK</b></th>
 *        <th><b>DESCRIPTION</b></th>
 *     </tr>
 *     <tr>
 *        <td>bt_adapter_set_name()</td>
 *        <td>bt_adapter_name_changed_cb()</td>
 *        <td>Used to set adapter name. </td>
 *     </tr>
 *     <tr>
 *        <td>bt_adapter_visibility_mode_changed_cb()</td>
 *        <td>Used to set visibility mode (#bt_adapter_visibility_mode_e). </td>
 *     </tr>
 *     <tr>
 *        <td>bt_adapter_start_device_discovery()<br>
 *            bt_adapter_stop_device_discovery()</td>
 *        <td>bt_adapter_device_discovery_state_changed_cb()</td>
 *        <td>Used to start or cancel device discovery. </td>
 *     </tr>
 *</table></div>
 *
 * @section CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE_CALLBACK_OPERATIONS Callback(Event) Operations
 * <div><table class="doxtable" >
 *     <tr>
 *        <th><b>REGISTER</b></th>
 *        <th><b>UNREGISTER</b></th>
 *        <th><b>CALLBACK</b></th>
 *        <th><b>DESCRIPTION</b></th>
 *     </tr>
 *     <tr>
 *        <td>bt_adapter_set_state_changed_cb()</td>
 *        <td>bt_adapter_unset_state_changed_cb()</td>
 *        <td>bt_adapter_state_changed_cb()</td>
 *        <td>Used to be notified of state change (enable or disable). </td>
 *     </tr>
 *     <tr>
 *        <td>bt_adapter_set_name_changed_cb()</td>
 *        <td>bt_adapter_unset_name_changed_cb()</td>
 *        <td>bt_adapter_name_changed_cb()</td>
 *        <td>Used to be notified of name change. </td>
 *     </tr>
 *     <tr>
 *        <td>bt_adapter_set_visibility_mode_changed_cb()</td>
 *        <td>bt_adapter_unset_visibility_mode_changed_cb()</td>
 *        <td>bt_adapter_visibility_mode_changed_cb()</td>
 *        <td>Used to be notified of change of visibility mode (#bt_adapter_visibility_mode_e). </td>
 *     </tr>
 *     <tr>
 *        <td>bt_adapter_set_device_discovery_state_changed_cb()</td>
 *        <td>bt_adapter_unset_device_discovery_state_changed_cb()</td>
 *        <td>bt_adapter_device_discovery_state_changed_cb()</td>
 *        <td>Used to be notified of change of device discovery state (#bt_adapter_device_discovery_state_e). </td>
 *     </tr>
 *</table></div>
 *
 * @section CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE_FOREACH_OPERATIONS Foreach Operations
 * <div><table class="doxtable" >
 *     <tr>
 *        <th><b>FOREACH</b></th>
 *        <th><b>CALLBACK</b></th>
 *        <th><b>DESCRIPTION</b></th>
 *     </tr>
 *     <tr>
 *        <td>bt_adapter_foreach_bonded_device()</td>
 *        <td>bt_adapter_bonded_device_cb()</td>
 *        <td>Used to get bonded devices.</td>
 *     </tr>
 *</table></div>
 *
 *<BR>
 * Please refer Bluetooth Tutorial if you want to get more detailed usages and information of this api.
 */

/**
 * @defgroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE Bluetooth LE Adapter
 * @brief Bluetooth LE API provides functions for managing bonds with other devices for LE and searching for supported LE services.
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE_OVERVIEW Overview
 * Bluetooth stack architecture has been changed. Thus, GATT APIs defined in Tizen 2.3 are deprecated and new GATT client APIs are defined.
 *
 * @section CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth\n
 *  - http://tizen.org/feature/network.bluetooth.le\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE,thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

/**
 * @defgroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_50_MODULE Bluetooth LE Adapter for Bluetooth 5.0
 * @brief Bluetooth LE API provides functions for bluetooth 5.0 functionality such as 2M Phy and Coded Phy
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_50_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_50_MODULE_OVERVIEW Overview
 * Bluetooth LE API provides functions for bluetooth 5.0 functionality such as 2M Phy and Coded Phy
 *
 * @section CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_50_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth\n
 *  - http://tizen.org/feature/network.bluetooth.le\n
 *  - http://tizen.org/feature/network.bluetooth.le.5_0\n
 * It is recommended to design applications with regard to features, for reliability.\n
 *
 * You can check if the device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, and control your application's actions accordingly.\n
 *
 * To ensure your application is only running on devices with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on using features in your application can be found in the <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>feature element description</b></a>.
 *
 */


/**
 * @defgroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE Bluetooth Device
 * @brief Bluetooth Device API provides functions for managing bonds with other devices and searching for supported services.
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE_OVERVIEW Overview
 * This set of functions is used to handle the connection with other devices.
 * Connected devices exchange keys needed for encrypted communication,
 * but each connection has to be approved by the latest application user.
 * You can also set authorization of other devices.
 * Authorized devices are connected, and it is connected automatically without the latest user
 * being asked for authorization.
 * In addition, this is used to search for services available on remote devices.
 *
 * @section CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE,thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 * @section CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE_ASYNCHRONOUS_OPERATIONS Asynchronous Operations
 * <div><table class="doxtable" >
 *     <tr>
 *        <th><b>FUNCTION</b></th>
 *        <th><b>CALLBACK</b></th>
 *        <th><b>DESCRIPTION</b></th>
 *     </tr>
 *     <tr>
 *        <td>bt_device_create_bond()<br>
 *            bt_device_cancel_bonding()</td>
 *        <td>bt_device_bond_created_cb()</td>
 *        <td>Used to create bond or cancel creating bond. </td>
 *     </tr>
 *     <tr>
 *        <td>bt_device_destroy_bond()</td>
 *        <td>bt_device_bond_destroyed_cb()</td>
 *        <td>Used to destroy bond. </td>
 *     </tr>
 *     <tr>
 *        <td>bt_device_set_authorization()</td>
 *        <td>bt_device_authorization_changed_cb()</td>
 *        <td>Used to set as authorized or unauthorized device. </td>
 *     </tr>
 *     <tr>
 *        <td>bt_device_start_service_search()</td>
 *        <td>bt_device_service_searched_cb()</td>
 *        <td>Used to start or cancel service search. </td>
 *     </tr>
 *</table></div>
 *
 * @section CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE_CALLBACK_OPERATIONS Callback(Event) Operations
 * <div><table class="doxtable" >
 *     <tr>
 *        <th><b>REGISTER</b></th>
 *        <th><b>UNREGISTER</b></th>
 *        <th><b>CALLBACK</b></th>
 *        <th><b>DESCRIPTION</b></th>
 *     </tr>
 *     <tr>
 *        <td>bt_device_set_bond_created_cb()</td>
 *        <td>bt_device_unset_bond_created_cb()</td>
 *        <td>bt_device_bond_created_cb()</td>
 *        <td>Used to be notified of the result of creating bond. </td>
 *     </tr>
 *     <tr>
 *        <td>bt_device_set_bond_destroyed_cb()</td>
 *        <td>bt_device_unset_bond_destroyed_cb()</td>
 *        <td>bt_device_bond_destroyed_cb()</td>
 *        <td>Used to be notified of the result of destroying bond. </td>
 *     </tr>
 *     <tr>
 *        <td>bt_device_set_authorization_changed_cb()</td>
 *        <td>bt_device_unset_authorization_changed_cb()</td>
 *        <td>bt_device_authorization_changed_cb()</td>
 *        <td>Used to be notified of change of authorization. </td>
 *     </tr>
 *     <tr>
 *        <td>bt_device_set_service_searched_cb()</td>
 *        <td>bt_device_unset_service_searched_cb()</td>
 *        <td>bt_device_service_searched_cb()</td>
 *        <td>Used to be notified of change of service search state. </td>
 *     </tr>
 *</table></div>
 *
 *<BR>
 * Please refer Bluetooth Tutorial if you want to get more detailed usages and information of this api.
 */


/**
 * @defgroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE Bluetooth Socket
 * @brief Bluetooth Socket API provides functions for managing connections to other devices and exchanging data.
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE_OVERVIEW Overview
 * This set of functions is used for exchanging data between two bluetooth
 * devices, our device can have both roles as server (service provider) and client
 * (service user).
 * Depending on the role, there is difference in creating a connection.
 * After that, processes of exchanging data and disconnection are same.
 *
 * To start communication, you should first register for #bt_socket_set_connection_state_changed_cb()
 * and #bt_socket_set_data_received_cb().
 * Next step depends on the role of your application.\n
 * If you want to be \b server \b role, application should create socket
 * (#bt_socket_create_rfcomm()), and start listening and accepting incoming connections
 * (#bt_socket_listen_and_accept_rfcomm()).
 * If you want to connect to specific device and use it's services (\a client \a role),
 * you have to start connection with #bt_socket_connect_rfcomm() and wait for connection.\n
 * After connection has been established (information is passed to your program with
 * #bt_socket_connection_state_changed_cb() function call), you can exchange data by
 * calling #bt_socket_send_data(). \n
 * If you receive data from remote device, #bt_socket_data_received_cb() functions will be called.
 * When you finish exchanging data, you should disconnect connection
 * with #bt_socket_disconnect_rfcomm() and unregister callback functions (with using
 * #bt_socket_unset_data_received_cb(), #bt_socket_unset_connection_state_changed_cb()).
 *
 * @section CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE,thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 * @section CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE_ASYNCHRONOUS_OPERATIONS Asynchronous Operations
 * <div><table class="doxtable" >
 *     <tr>
 *        <th><b>FUNCTION</b></th>
 *        <th><b>CALLBACK</b></th>
 *        <th><b>DESCRIPTION</b></th>
 *     </tr>
 *     <tr>
 *        <td>bt_socket_listen_and_accept_rfcomm()<br>
 *            bt_socket_connect_rfcomm()</td>
 *        <td>bt_socket_connection_state_changed_cb()</td>
 *        <td>Used to connect a device. </td>
 *     </tr>
 *</table></div>
 *
 * @section CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE_CALLBACK_OPERATIONS Callback(Event) Operations
 * <div><table class="doxtable" >
 *     <tr>
 *        <th><b>REGISTER</b></th>
 *        <th><b>UNREGISTER</b></th>
 *        <th><b>CALLBACK</b></th>
 *        <th><b>DESCRIPTION</b></th>
 *     </tr>
 *     <tr>
 *        <td>bt_socket_set_data_received_cb()</td>
 *        <td>bt_socket_unset_data_received_cb()</td>
 *        <td>bt_socket_data_received_cb()</td>
 *        <td>Used to be notified of received data. </td>
 *     </tr>
 *     <tr>
 *        <td>bt_socket_set_connection_state_changed_cb()</td>
 *        <td>bt_socket_unset_connection_state_changed_cb()</td>
 *        <td>bt_socket_connection_state_changed_cb()</td>
 *        <td>Used to be notified when the state of connection changes. </td>
 *     </tr>
 *</table></div>
 *
 *<BR>
 * Please refer Bluetooth Tutorial if you want to get more detailed usages and information of this api.
 */


/**
 * @defgroup CAPI_NETWORK_BLUETOOTH_OPP_MODULE Bluetooth OPP
 * @brief Bluetooth OPP(Object Push Profile) API provides functions for exchanging objects such as pictures.
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_OPP_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_OPP_MODULE_OVERVIEW Overview
 * OPP profile let users exchange objects between two devices.
 *
 */

/**
 * @defgroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE Bluetooth OPP Server
 * @brief Bluetooth OPP(Object Push Profile) Server API provides functions for accepting objects such as pictures.
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE_OVERVIEW Overview
 * This is OPP server APIs.
 *
 * @section CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth.opp\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE,thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

/**
 * @defgroup CAPI_NETWORK_BLUETOOTH_OPP_CLIENT_MODULE Bluetooth OPP Client
 * @brief Bluetooth OPP(Object Push Profile) Client API provides functions for pushing objects such as pictures.
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_OPP_CLIENT_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_OPP_CLIENT_MODULE_OVERVIEW Overview
 * This is OPP Client APIs.
 *
 * @section CAPI_NETWORK_BLUETOOTH_OPP_CLIENT_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth.opp\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE,thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

/**
 * @internal
 * @defgroup CAPI_NETWORK_BLUETOOTH_PAN_MODULE Bluetooth PAN
 * @brief Bluetooth PAN(Personal Area Networking) API provides functions for connecting to a higher level network and the Internet.
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_PAN_MODULE_OVERVIEW Overview
 * The PAN Profile describe how two or more Bluetooth enabled devices can form an ad-hoc network and how the same mechanism can be
 * used to access a remote network through a network access point. But this API only supports network access point service.
 *
 * @section CAPI_NETWORK_BLUETOOTH_PAN_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.tethering.bluetooth\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE,thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

/**
 * @internal
 * @defgroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE Bluetooth NAP
 * @brief Bluetooth NAP(Network Access Point) API provides functions for an Ethernet bridge to support network services.
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE_OVERVIEW Overview
 * A Bluetooth device that supports the NAP service is a Bluetooth device that provides some of the features of an Ethernet bridge to support network services.
 *
 * @section CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.tethering.bluetooth\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

/**
 * @internal
 * @defgroup CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE Bluetooth PANU
 * @brief Bluetooth PANU(Personal Area Networking User) API provides functions for client of both the NAP(Network Access Point).
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE_OVERVIEW Overview
 * In PAN Profile, PANU can uses either the NAP or GN service, but this API only supports NAP service.
 *
 * @section CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.tethering.bluetooth\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

/**
 * @if MOBILE
 * @defgroup CAPI_NETWORK_BLUETOOTH_HID_MODULE Bluetooth HID
 * @brief Bluetooth HID(Human Interface Device) API provides functions for connecting to Bluetooth HID such as keyboards and mouse.
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_HID_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_HID_MODULE_OVERVIEW Overview
 * In HID Profile, there are two roles - @a Host and @a Device.
 * The @a Host is a device that uses or requests the services of a HID. The @a Device is a device that provides the service of human data input and output to and from the @a Host.
 * This API only supports @a Host role.
 *
 * @section CAPI_NETWORK_BLUETOOTH_HID_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth.hid\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 * @endif
 */

/**
 * @if WEARABLE
 * @defgroup CAPI_NETWORK_BLUETOOTH_HID_MODULE Bluetooth HID
 * @brief Bluetooth HID(Human Interface Device) API provides functions for connecting to Bluetooth HID such as keyboards and mouse.
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_HID_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_HID_MODULE_OVERVIEW Overview
 * In HID Profile, there are two roles - @a Host and @a Device.
 * The @a Host is a device that uses or requests the services of a HID. The @a Device is a device that provides the service of human data input and output to and from th
 *
 * @section CAPI_NETWORK_BLUETOOTH_HID_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth.hid\n
 *  - http://tizen.org/feature/network.bluetooth.hid_device\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 * @endif
 */


/**
 * @if WEARABLE
 * @defgroup CAPI_NETWORK_BLUETOOTH_HID_HOST_MODULE Bluetooth HID Host
 * @brief Bluetooth HID(Human Interface Device) API provides functions for connecting to Bluetooth HID such as keyboards and mouse.
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_HID_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_HID_MODULE_OVERVIEW Overview
 * In HID Profile, there are two roles - @a Host and @a Device.
 * The @a Host is a device that uses or requests the services of a HID. The @a Device is a device that provides the service of human data input and output to and from th
 * This API only supports @a Host role.
 *
 * @section CAPI_NETWORK_BLUETOOTH_HID_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth.hid\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 * @endif
 */

/**
 * @if WEARABLE
 * @defgroup CAPI_NETWORK_BLUETOOTH_HID_DEVICE_MODULE Bluetooth HID Device
 * @brief The Bluetooth HID (Human Interface Device) API provides functions for connecting Bluetooth HID such as keyboards and mice.
 * @ingroup CAPI_NETWORK_BLUETOOTH_HID_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_HID_DEVICE_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_HID_DEVICE_MODULE_OVERVIEW Overview
 * In HID Profile, there are two roles - @a Host and @a Device.
 * The @a Host is a device that uses or requests the services of a HID. The @a Device is a device that provides the service of human data input and output to and from the @a Host.
 * This API supports the @a Device role.
 *
 * @section CAPI_NETWORK_BLUETOOTH_HID_DEVICE_MODULE_FEATURE Related Features
 * This API is related to the following features:\n
 *  - http://tizen.org/feature/network.bluetooth.hid_device\n
 *
 * It is recommended to use features in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling your application's actions.\n
 *
 * To ensure your application runs only on devices with the required features, please declare the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on using features in your application can be found in the <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 * @endif
 *
 */

/**
 * @defgroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE Bluetooth Audio
 * @brief Bluetooth Audio API provides functions for connecting to Bluetooth audio devices such as headset, hand-free and headphone.
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE_OVERVIEW Overview
 * This API supports the HFP(Hands-Free Profile), HSP(Headset Profile) and A2DP(Advanced Audio Distribution Profile).
 *
 * @section CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth.audio.call\n
 *  - http://tizen.org/feature/network.bluetooth.audio.media\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

/**
 * @internal
 * @defgroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE Bluetooth AG
 * @brief Bluetooth AG(Audio Gateway) API provides functions for controlling the remote audio device.
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE_OVERVIEW Overview
 * This API supports the @a Audio @a Gateway role in HSP and HFP.
 * Audio Gateway is the device that is the gateway of the audio, both for input and output, typically a mobile phone or PC.
 *
 * @section CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth.audio.call\n
 *  - http://tizen.org/feature/network.bluetooth.audio.media\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

/**
 * @internal
 * @defgroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_CALL_MODULE Call List
 * @brief Bluetooth AG(Audio Gateway) API provides functions for managing the handler of call list.
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_AUDIO_AG_CALL_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_AUDIO_AG_CALL_OVERVIEW Overview
 * This set of functions is used for managing the handler of call list.
 * This handler is used for notifying the call list to the remote bluetooth device which supports HFP(Hands-Free Profile).
 *
 * @section CAPI_NETWORK_BLUETOOTH_AUDIO_AG_CALL_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth.audio.call\n
 *  - http://tizen.org/feature/network.bluetooth.audio.media\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

/**
 * @defgroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE Bluetooth AVRCP
 * @brief Bluetooth AVRCP(Audio/Video Remote Control Profile) API provides functions for notifying the change of target device to the control device(@a Target role) and functions for controlling the remote device(@a Controller role).
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE_OVERVIEW Overview
 * AVRCP profile let users send AV control signals and information between two devices.
 *
 */

/**
 * @defgroup CAPI_NETWORK_BLUETOOTH_AVRCP_TARGET_MODULE Bluetooth AVRCP Target
 * @brief Bluetooth AVRCP(Audio/Video Remote Control Profile) Target API provides functions for notifying the change of target device to the control device.
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_AVRCP_TARGET_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_AVRCP_TARGET_MODULE_OVERVIEW Overview
 * This API supports the @a Target role in AVRCP spec.
 * The @a Target is the device whose characteristics are being altered.
 * In a "walkman" type media player scenario, the @a Control device may be a headset that allows tracks to be skipped and the @a Target device would be the actual medial player.
 *
 * @section CAPI_NETWORK_BLUETOOTH_AVRCP_TARGET_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth.audio.media\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

 /**
 * @defgroup CAPI_NETWORK_BLUETOOTH_AVRCP_CONTROL_MODULE Bluetooth AVRCP Controller
 * @brief Bluetooth AVRCP(Audio/Video Remote Control Profile) Control API provides functions for controlling the remote device.
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_AVRCP_CONTROL_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_AVRCP_CONTROL_MODULE_OVERVIEW Overview
 * This API supports the @a Controller role in the AVRCP spec.
 * In a "walkman" type media player scenario, the @a Control device may be a headset that allows tracks to be skipped and the @a Target device would be the actual medial player.
 *
 * @section CAPI_NETWORK_BLUETOOTH_AVRCP_CONTROL_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth.audio.controller\n
 *
 * It is recommended to create application with regard to features, to increase reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on a device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

/**
 * @defgroup CAPI_NETWORK_BLUETOOTH_HDP_MODULE Bluetooth HDP
 * @brief Bluetooth HDP(Health Device Profile) API provides functions for managing connections to health devices and exchanging data.
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_HDP_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_HDP_MODULE_OVERVIEW Overview
 * The @a Source is a transmitter of application data that is to be transferred to a @a Sink. The @a Sink is a receiver of application data delivered from a @a Source.
 * This API supports the @a Sink role in HDP spec.
 *
 * @section CAPI_NETWORK_BLUETOOTH_HDP_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth.health\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

/**
 * @defgroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE Bluetooth GATT
 * @brief Bluetooth GATT (Generic Attribute Profile) API provides common functions for GATT client and GATT server.
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_GATT_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_GATT_MODULE_OVERVIEW Overview
 * Two roles are defined for devices that implement GATT.
 * The @a Server is the device that accepts incoming commands and requests from the client and sends responses, indications and notifications to a client.
 * The @a Client is the device that initiates commands and requests towards the server and can receive responses, indications and notifications sent by the server.
 * This API supports both @a Server role and @a Client role in GATT. \n
 * Bluetooth stack architecture has been changed. Thus, GATT APIs defined in Tizen 2.3 are deprecated and new GATT client APIs are defined.
 *
 * @section CAPI_NETWORK_BLUETOOTH_GATT_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth.le.gatt.server\n
 *  - http://tizen.org/feature/network.bluetooth.le.gatt.client\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

 /**
 * @defgroup CAPI_NETWORK_BLUETOOTH_GATT_SERVER_MODULE Bluetooth GATT Server
 * @brief Bluetooth GATT (Generic Attribute Profile) Server API provides functions for modifying attributes and registering services, characteristics and descriptors.
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_GATT_SERVER_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_GATT_SERVER_MODULE_OVERVIEW Overview
 * Two roles are defined for devices that implement GATT.
 * The @a Server is the device that accepts incoming commands and requests from a client and sends responses, indications and notifications to the client.
 * The @a Client is the device that initiates commands and requests towards a server and can receive responses, indications and notifications sent by the server.
 *
 * @section CAPI_NETWORK_BLUETOOTH_GATT_SERVER_MODULE_FEATURE Related Features
 * This API supports both @a Server role in GATT. \n
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth.le.gatt.server\n
 *
 * It is recommended to create an application with regard to features, to increase reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

/**
 * @defgroup CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE Bluetooth GATT Client
 * @brief Bluetooth GATT (Generic Attribute Profile) Client API provides functions for discovering attributes and registering services, characteristics and descriptors of the remote device.
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE_OVERVIEW Overview
 * Two roles are defined for devices that implement GATT.
 * The @a Server is the device that accepts incoming commands and requests from a client and sends responses, indications and notifications to the client.
 * The @a Client is the device that initiates commands and requests towards a server and can receive responses, indications and notifications sent by the server.
 *
 * @section CAPI_NETWORK_BLUETOOTH_GATT_CLIENT_MODULE_FEATURE Related Features
 * This API supports both @a Client role in GATT. \n
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth.le.gatt.client\n
 *
 * It is recommended to create your application with regard to features, to increase reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, and control your application's actions accordingly.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on using features in your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

/**
 * @defgroup CAPI_NETWORK_BLUETOOTH_IPSP_MODULE Bluetooth IPSP
 * @brief Bluetooth IPSP(Internet Protocol Support Profile) API provides functions for supporting of exchanging IPv6 packets between devices over the BLE transport.
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_IPSP_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_IPSP_MODULE_OVERVIEW Overview
 * Two roles are defined for devices that implement IPSP.
 * The @a Router is used for devices that can route IPv6 packets.
 * The @a Node is used for devices that can originate or consume IPv6 application packets.
 *
 * @section CAPI_NETWORK_BLUETOOTH_IPSP_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth.le.ipsp\n
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

/**
 * @WEARABLE_ONLY
 * @defgroup CAPI_NETWORK_BLUETOOTH_PBAP_MODULE Bluetooth PBAP
 * @brief Bluetooth PBAP (Phone Book Access Profile) API provides functions for managing connections to a phone book server and retrieving phone book data.
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_PBAP_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_PBAP_MODULE_OVERVIEW Overview
 * The PBAP involves two roles, @a Server and @a Client.
 * The @a Server is a device that has phone book data. The @a Client is a device that requests the phone book data from the @a Server.
 * This API only supports @a Client role.
 *
 * @section CAPI_NETWORK_BLUETOOTH_PBAP_MODULE_FEATURE Related Features
 * This API is related to the following features:\n
 *  - http://tizen.org/feature/network.bluetooth.phonebook.client\n
 *
 * It is recommended to use features in your application for reliability.\n
 *
 * You can check if a device supports features related to this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling your application's actions.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on using features in your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 *
 */

/**
 * @defgroup CAPI_NETWORK_BLUETOOTH_OOB_MODULE Bluetooth OOB
 * @brief Bluetooth OOB (Out of Band) API provides functions for exchanging some information used in the pairing process.
 * @ingroup CAPI_NETWORK_BLUETOOTH_MODULE
 *
 * @section CAPI_NETWORK_BLUETOOTH_OOB_MODULE_HEADER Required Header
 *  \#include <bluetooth.h>
 *
 * @section CAPI_NETWORK_BLUETOOTH_OOB_MODULE_OVERVIEW Overview
 * This set of functions is used for exchanging the hash key and randomizer.
 * This API uses an external means of communication, such as near-field communication (NFC) to exchange some information used in the pairing process.
 *
 * @section CAPI_NETWORK_BLUETOOTH_HDP_MODULE_FEATURE Related Features
 * This API is related with the following features:\n
 *  - http://tizen.org/feature/network.bluetooth.oob\n
 *
 * It is recommended to use features in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, and control your application's actions accordingly.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="https://developer.tizen.org/development/tizen-studio/native-tools/configuring-your-app/manifest-text-editor#feature"><b>Feature Element</b>.</a>
 *
 */

#endif /* __TIZEN_NETWORK_BLUETOOTH_DOC_H__ */
