/* ****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

#ifndef CA_MANAGER_H_
#define CA_MANAGER_H_

#include <OCApi.h>

namespace OC
{
    /**
    * This namespace contains the main entrance/functionality to monitoring network changes.
    * It may be used with OC::CAManager::functionName. To set a custom callback function,
    * the implementer must make a call to CAManager::setNetworkMonitorHandler.
    */
    namespace CAManager
    {
        /**
         *  Data structure to provide the configuration for cautil.
         */
        struct CAUtilConfig
        {
            /** the flag for ble advertising. */
            OCTransportBTFlags_t       bleFlag;

            public:
                CAUtilConfig()
                    : bleFlag(OC_DEFAULT_BT_FLAGS)
            {}
                CAUtilConfig(OCTransportBTFlags_t bleFlag_)
                    : bleFlag(bleFlag_)
            {}
        };

        // typedef to get adapter status changes from CA.
        typedef std::function<void(const std::string&, OCConnectivityType,
                                   bool)> ConnectionChangedCallback;

        // typedef to get connection status changes from CA.
        typedef std::function<void(OCTransportAdapter, bool)> AdapterChangedCallback;

        /**
        * Set network monitoring handler.
        * @param adapterHandler adapter state change handler to handle changes for
        *                       any transport types.
        * @param connectionHandler connection state change handler to handle changes for
        *                          connection with remote devices.
        * @return Returns ::OC_STACK_OK if success.
        */
        OCStackResult setNetworkMonitorHandler(AdapterChangedCallback adapterHandler,
                                               ConnectionChangedCallback connectionHandler);

        /**
        * Unset network monitoring handler.
        * @return Returns ::OC_STACK_OK if success.
        */
        OCStackResult unsetNetworkMonitorHandler();

        /**
        * Set port number to use.
        * @param adapter transport adapter type to assign the specified port number.
        * @param flag transport flag information.
        * @param port the specified port number to use.
        * @return Returns ::OC_STACK_OK if success.
        */
        OCStackResult setPortNumberToAssign(OCTransportAdapter adapter,
                                            OCTransportFlags flag, uint16_t port);

        /**
        * Get the assigned port number.
        * @param adapter transport adapter type to get the opened port number.
        * @param flag   transport flag information.
        * @return Returns currently assigned port number.
        */
        uint16_t getAssignedPortNumber(OCTransportAdapter adapter, OCTransportFlags flag);

        /**
         * start BLE advertising.
         * @return Returns ::OC_STACK_OK if success.
         */
        OCStackResult startLEAdvertising();

        /**
         * stop BLE advertising.
         * @return Returns ::OC_STACK_OK if success.
         */
        OCStackResult stopLEAdvertising();

        /**
         * set BT configure.
         * @param[in]  config       ::CAUtilConfig data
         * @return Returns ::OC_STACK_OK if success.
         */
        OCStackResult setBTConfigure(const CAUtilConfig& config);

        /**
         * set CAUtil log preference.
         * @param[in]  level                     ::OCLogLevel value.
         * @param[in]  hidePrivateLogEntries     Private Log Entries.
         *                                       Example:
         *                                       true : hide private log.
         *                                       false : show private log.
         *                                       (privacy : uid, did, access token, etc)
         */
        void setLogLevel(OCLogLevel level, bool hidePrivateLogEntries);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
        /**
         * Select the cipher suite for TLS/DTLS handshake.
         * @param cipher  cipher suite (Note : Make sure endianness).
         *                        TLS_RSA_WITH_AES_256_CBC_SHA256          0x3D
         *                        TLS_RSA_WITH_AES_128_GCM_SHA256          0x009C
         *                        TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256  0xC02B
         *                        TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8       0xC0AE
         *                        TLS_ECDHE_ECDSA_WITH_AES_128_CCM         0xC0AC
         *                        TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256  0xC023
         *                        TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384  0xC024
         *                        TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384  0xC02C
         *                        TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256    0xC037
         *                        TLS_ECDH_anon_WITH_AES_128_CBC_SHA       0xC018
         * @param adapter transport adapter type.
         * @return Returns ::OC_STACK_OK if success.
         */
        OCStackResult setCipherSuite(const uint16_t cipher, OCTransportAdapter adapter);
#endif // defined(__WITH_DTLS__) || defined(__WITH_TLS__)

#if defined(TCP_ADAPTER) && defined(WITH_CLOUD)
        /**
         * Set User Preference
         * @param user preference
         *        0: OC_USER_PREF_CLOUD (default)
         *        1: OC_USER_PREF_LOCAL_UDP
         *        2: OC_USER_PREF_LOCAL_TCP
         * @return ::OC_STACK_OK if succes.
         */
        OCStackResult setConnectionManagerUserConfig(OCConnectUserPref_t connPriority);
#endif
    }
}

#endif // CA_MANAGER_H_



