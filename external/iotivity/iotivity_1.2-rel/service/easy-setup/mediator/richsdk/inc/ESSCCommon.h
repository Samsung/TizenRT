//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef ESSC_COMMON_RICH_H_
#define ESSC_COMMON_RICH_H_

#include "ESRichCommon.h"

using namespace OC;
using namespace std;

#define SC_RSRVD_ES_VENDOR_NETCONNECTION_STATE  "x.com.samsung.ncs"
#define SC_RSRVD_ES_VENDOR_DISCOVERY_CHANNEL    "x.com.samsung.chn"
#define SC_RSRVD_ES_VENDOR_DEVICE_TYPE          "x.com.samsung.dt"
#define SC_RSRVD_ES_VENDOR_DEVICE_SUBTYPE       "x.com.samsung.sdt"
#define SC_RSRVD_ES_VENDOR_LOCATION             "x.com.samsung.location"
#define SC_RSRVD_ES_VENDOR_CLIENTID             "x.com.samsung.clientid"
#define SC_RSRVD_ES_VENDOR_ACCOUNT              "x.com.samsung.account"
#define SC_RSRVD_ES_VENDOR_ADDITIONAL_AUTH_CODE "x.com.samsung.aac"
#define SC_RSRVD_ES_VENDOR_ACCESS_TOKEN         "x.com.samsung.accesstoken"
#define SC_RSRVD_ES_VENDOR_REFRESH_TOKEN        "x.com.samsung.refreshtoken"
#define SC_RSRVD_ES_VENDOR_UTC_DATE_TIME        "x.com.samsung.datetime"
#define SC_RSRVD_ES_VENDOR_BSSID                "x.com.samsung.bssid"
#define SC_RSRVD_ES_VENDOR_USERID               "x.com.samsung.uid"
#define SC_RSRVD_ES_VENDOR_REGISTER_MOBILE_DEV  "x.com.samsung.rmd"
#define SC_RSRVD_ES_VENDOR_REGISTER_SET_DEV     "x.com.samsung.rsd"
#define SC_RSRVD_ES_VENDOR_TC_STATUS            "x.com.samsung.tcstatus"
#define SC_RSRVD_ES_VENDOR_TC_HEADER            "x.com.samsung.tcheader"
#define SC_RSRVD_ES_VENDOR_TC_VERSION           "x.com.samsung.tcversion"
#define SC_RSRVD_ES_VENDOR_TC_RESULT            "x.com.samsung.tcresult"
#define SC_RSRVD_ES_VENDOR_PNP_PIN              "x.com.samsung.pnppin"
#define SC_RSRVD_ES_VENDOR_MODEL_NUMBER         "x.com.samsung.modelnumber"
#define SC_RSRVD_ES_VENDOR_LANGUAGE             "x.com.samsung.language"
#define SC_RSRVD_ES_VENDOR_COUNTRY              "x.com.samsung.country"
#define SC_RSRVD_ES_VENDOR_GPSLOCATION          "x.com.samsung.gpslocation"


namespace OIC
{
    namespace Service
    {

        #define INVALID_DISCOVERY_CHANNEL -1

        typedef enum
        {
            NET_STATE_INIT = -1,                /**< Init state **/
            NET_STATE_WIRED_CONNECTED = 0,      /**< Wired connected **/
            NET_STATE_WIRELESS_CONNECTED,       /**< Wireless connected **/
            NET_STATE_NOT_CONNECTED             /**< Not connected, at all **/
        } NETCONNECTION_STATE;

        typedef enum
        {
            TC_STATUS_NOT_SUPPORTED = 0,
            TC_STATUS_ALREADY_AGREED,
            TC_STATUS_NEED_TO_AGREE,
            TC_STATUS_DOWNLOADING_TC,
            TC_STATUS_SUCCESS_TO_DOWNLOAD_TC,
            TC_STATUS_FAIL_TO_DOWNLOAD_TC,
            TC_STATUS_DISAGREED_WITH_TC,
            TC_STATUS_GEO_BLOCKED,
            TC_STATUS_TIMEOUT_TO_AGREE
        } TC_STATUS;

        /**
         * @brief Properties of provisioning resource. It includes a provisioning status and last
         *        error code.
         */
        class SCEnrolleeStatus : public EnrolleeStatus
        {
        public:
            SCEnrolleeStatus(const EnrolleeStatus&& parent) : EnrolleeStatus(parent)
            {
            }

            /**
             * Get a network connection state property of Enrollee.
             *
             * @return a network connection state property of Enrollee.
             */
            NETCONNECTION_STATE getNetConnectionState() const
            {
                if(m_rep.hasAttribute(SC_RSRVD_ES_VENDOR_NETCONNECTION_STATE))
                {
                    return static_cast<NETCONNECTION_STATE>
                                    (m_rep.getValue<int>(SC_RSRVD_ES_VENDOR_NETCONNECTION_STATE));
                }
                return NETCONNECTION_STATE::NET_STATE_INIT;
            }

            /**
             * Get T&C status
             *
             * @return T&C Status
             *
             * @see TC_STATUS
             */
            TC_STATUS getTCStatus()
            {
                if(m_rep.hasAttribute(SC_RSRVD_ES_VENDOR_TC_STATUS))
                {
                    return static_cast<TC_STATUS>
                            (m_rep.getValue<int>(SC_RSRVD_ES_VENDOR_TC_STATUS));
                }

                return TC_STATUS::TC_STATUS_NOT_SUPPORTED;
            }
        };

        /**
         * @brief Data class stored for Device property provisioning which includes a WiFi
         *        and device configuration provisioning
         */
        class SCDeviceProp : public DeviceProp
        {
        public:

            /**
             * Set DevConf resource properties to be delivered to Enrollee
             *
             * @param language IETF language tag using ISO 639X
             * @param country ISO Country Code (ISO 3166-1 Alpha-2)
             * @param location location information
             */
            void setDevConfProp(string language, string country, string location)
            {
                m_rep.setValue(SC_RSRVD_ES_VENDOR_LANGUAGE, language);
                m_rep.setValue(SC_RSRVD_ES_VENDOR_COUNTRY, country);
                m_rep.setValue(SC_RSRVD_ES_VENDOR_GPSLOCATION, location);
            }

            /**
             * Get a language to be set. A language is expressed in IETF language tag
             * using ISO 639X.
             *
             * @return a language to be set
             */
            std::string getLanguage() const
            {
                if(m_rep.hasAttribute(SC_RSRVD_ES_VENDOR_LANGUAGE))
                {
                    return m_rep.getValue<std::string>(SC_RSRVD_ES_VENDOR_LANGUAGE);
                }
                return std::string("");
            }

            /**
             * Get a country to be set. A country is expressed in ISO Country Code
             * (ISO 3166-1 Alpha-2)
             *
             * @return a country to be set
             */
            std::string getCountry() const
            {
                if(m_rep.hasAttribute(SC_RSRVD_ES_VENDOR_COUNTRY))
                {
                    return m_rep.getValue<std::string>(SC_RSRVD_ES_VENDOR_COUNTRY);
                }
                return std::string("");
            }

            /**
             * Get a location to be set. A location is GPS information
             *
             * @return a country to be set
             */
            std::string getLocation() const
            {
                if(m_rep.hasAttribute(SC_RSRVD_ES_VENDOR_GPSLOCATION))
                {
                    return m_rep.getValue<std::string>(SC_RSRVD_ES_VENDOR_GPSLOCATION);
                }
                return std::string("");
            }

            /**
             * Set discoveryChannel properties to be delivered to Enrollee
             *
             * @param discoveryChannel Channel of the Enroller for fast discover
             */
            void setDiscoveryChannel(int discoveryChannel)
            {
                m_rep.setValue(SC_RSRVD_ES_VENDOR_DISCOVERY_CHANNEL, discoveryChannel);
            }

            /**
             * Get an channel of Enroller
             *
             * @return an channel of enroller
             */
            int getDiscoveryChannel()
            {
                if(m_rep.hasAttribute(SC_RSRVD_ES_VENDOR_DISCOVERY_CHANNEL))
                {
                    return m_rep.getValue<int>(SC_RSRVD_ES_VENDOR_DISCOVERY_CHANNEL);
                }
                return INVALID_DISCOVERY_CHANNEL;
            }

            /**
             * Set samsung-specific location property to be delivered to Enrollee
             *
             * @param locations a set of location information
             */
            void setSCLocation(const std::vector<std::string> &locations)
            {
                m_rep.setValue(SC_RSRVD_ES_VENDOR_LOCATION, locations);
            }

            /**
             * Set samsung account property to be delivered to Enrollee (for TV)
             *
             * @param account a samsung account ID information (for TV)
             */
            void setSCAccount(const std::string &account)
            {
                m_rep.setValue(SC_RSRVD_ES_VENDOR_ACCOUNT, account);
            }

            /**
             * Set UTC date time property to be delivered to Enrollee.
             * The format is "[yyyy]-[mm]-[dd]T[hh]:[mm]:[ss]Z"
             *
             * @param datetime UTC date time
             */
            void setUTCDatetime(const std::string &datetime)
            {
                m_rep.setValue(SC_RSRVD_ES_VENDOR_UTC_DATE_TIME, datetime);
            }

            /**
             * Get UTC date time property
             *
             * @return UTC date time
             */
            std::string getUTCDatetime()
            {
                if(m_rep.hasAttribute(SC_RSRVD_ES_VENDOR_UTC_DATE_TIME))
                {
                    return m_rep.getValue<std::string>(SC_RSRVD_ES_VENDOR_UTC_DATE_TIME);
                }
                return {};
            }

            /**
             * Set Mobile device information used for 'register TV'
             * The format is in json. The key indicates the meaning of value and the value is a
             * corresponding information. Supported keys include: wm(Wifi MAC), pm(P2P MAC),
             * bm(BT MAC), dn(device name), dt(device type), it(icon type).
             *
             * @param regMobileDev register mobile device
             */
            void setRegisterMobileDevice(const std::string &regMobileDev)
            {
                m_rep.setValue(SC_RSRVD_ES_VENDOR_REGISTER_MOBILE_DEV, regMobileDev);
            }

            /**
             * Get Mobile device information used for 'register TV'
             *
             * @return register mobile device
             */
            std::string getRegisterMobileDevice()
            {
                if(m_rep.hasAttribute(SC_RSRVD_ES_VENDOR_REGISTER_MOBILE_DEV))
                {
                    return m_rep.getValue<std::string>(SC_RSRVD_ES_VENDOR_REGISTER_MOBILE_DEV);
                }
                return {};
            }

            /**
             * Set a BSSID of Enroller
             *
             * @param a BSSID of enroller
             */
            void setBSSID(const std::string &bssid)
            {
                m_rep.setValue(SC_RSRVD_ES_VENDOR_BSSID, bssid);
            }

            /**
             * Get a BSSID of Enroller
             *
             * @return a BSSID of enroller
             */
            std::string getBSSID()
            {
                if(m_rep.hasAttribute(SC_RSRVD_ES_VENDOR_BSSID))
                {
                    return m_rep.getValue<std::string>(SC_RSRVD_ES_VENDOR_BSSID);
                }
                return {};
            }
        };

        class SCCloudProp : public CloudProp
        {
            public:
            /**
             * Set samsung-specific client service ID property to be delivered to Enrollee
             *
             * @param clientID - Client service ID
             */
            void setClientID(const std::string &clientID)
            {
                m_rep.setValue(SC_RSRVD_ES_VENDOR_CLIENTID, clientID);
            }

            /**
             * Get samsung-specific client service ID property.
             *
             * @return Client service ID.
             */
            std::string getClientID()
            {
                if(m_rep.hasAttribute(SC_RSRVD_ES_VENDOR_CLIENTID))
                {
                    return m_rep.getValue<std::string>(SC_RSRVD_ES_VENDOR_CLIENTID);
                }
                return {};
            }

            /**
             * Set samsung-specific additional AuthCode property to be delivered to Enrollee (for TV/AV sign up VD SSO)
             *
             * @param additionalAuthcode - additional AuthCode (for TV/AV sign up VD SSO)
             */
            void setAdditionalAuthCode(const std::string &additionalAuthcode)
            {
                m_rep.setValue(SC_RSRVD_ES_VENDOR_ADDITIONAL_AUTH_CODE, additionalAuthcode);
            }

            /**
             * Set samsung-specific refresh token property to be delivered to Enrollee
             *
             * @param refreshtoken - Refresh token.
             */
            void setRefreshToken(const std::string &refreshtoken)
            {
                m_rep.setValue(SC_RSRVD_ES_VENDOR_REFRESH_TOKEN, refreshtoken);
            }

            /**
             * Get samsung-specific refresh token property.
             *
             * @return refreshtoken - Refresh token.
             */
            std::string getRefreshToken()
            {
                if(m_rep.hasAttribute(SC_RSRVD_ES_VENDOR_REFRESH_TOKEN))
                {
                    return m_rep.getValue<std::string>(SC_RSRVD_ES_VENDOR_REFRESH_TOKEN);
                }
                return {};
            }

            /**
             * Set samsung-specific user ID property to be delivered to Enrollee
             *
             * @param uid - user ID.
             */
            void setUserID(const std::string &uid)
            {
                m_rep.setValue(SC_RSRVD_ES_VENDOR_USERID, uid);
            }

            /**
             * Get samsung-specific user ID property
             *
             * @return uid - user ID.
             */
            std::string getUserID()
            {
                if(m_rep.hasAttribute(SC_RSRVD_ES_VENDOR_USERID))
                {
                    return m_rep.getValue<std::string>(SC_RSRVD_ES_VENDOR_USERID);
                }
                return {};
            }

            /**
             * Set T&C Result
             * Indicates T&C result by mobile user in json format.
             *
             * @param uid - user ID.
             */
            void setTCResult(const std::string &result)
            {
                m_rep.setValue(SC_RSRVD_ES_VENDOR_TC_RESULT, result);
            }

            /**
             * Get T&C Result
             *
             * @return uid - user ID.
             */
            std::string getTCResult()
            {
                if(m_rep.hasAttribute(SC_RSRVD_ES_VENDOR_TC_RESULT))
                {
                    return m_rep.getValue<std::string>(SC_RSRVD_ES_VENDOR_TC_RESULT);
                }
                return {};
            }
        };

        /**
         * @breif This provide a set of getter APIs from received response for getConfiguration().
         *        Received information includes a device name, WiFi supported mode, and frequency.
         *        Additionally, you can know if Enrollee can be access to cloud server with this
         *        object.
         */
        class SCEnrolleeConf : public EnrolleeConf
        {
        public:
            SCEnrolleeConf(const EnrolleeConf&& parent) : EnrolleeConf(parent)
            {
            }

            /**
             * Get a model number of Enrollee.
             *
             * @return a model number of Enrollee
             */
            std::string getModelNumber() const
            {
                std::vector<OCRepresentation> children = m_EasySetupRep.getChildren();
                for(auto child = children.begin(); child != children.end(); ++child)
                {
                    if(child->getUri().find(OC_RSRVD_ES_URI_DEVCONF) != std::string::npos)
                    {
                        OCRepresentation rep;
                        if(child->hasAttribute(OC_RSRVD_REPRESENTATION))
                        {
                            rep = child->getValue<OCRepresentation>(OC_RSRVD_REPRESENTATION);
                        }
                        else
                        {
                            return std::string("");
                        }

                        if(rep.hasAttribute(SC_RSRVD_ES_VENDOR_MODEL_NUMBER))
                        {
                            return rep.getValue<std::string>(SC_RSRVD_ES_VENDOR_MODEL_NUMBER);
                        }
                    }
                }
                return std::string("");
            }

            /**
             * Get a device type of Enrollee.
             * It is Device's human-friendly name like device model name.
             *
             * @return a device type of Enrollee
             */
            std::string getDeviceType() const
            {
                std::vector<OCRepresentation> children = m_EasySetupRep.getChildren();
                for(auto child = children.begin(); child != children.end(); ++child)
                {
                    if(child->getUri().find(OC_RSRVD_ES_URI_DEVCONF) != std::string::npos)
                    {
                        OCRepresentation rep;
                        if(child->hasAttribute(OC_RSRVD_REPRESENTATION))
                        {
                            rep = child->getValue<OCRepresentation>(OC_RSRVD_REPRESENTATION);
                        }
                        else
                        {
                            return std::string("");
                        }

                        if(rep.hasAttribute(SC_RSRVD_ES_VENDOR_DEVICE_TYPE))
                        {
                            return rep.getValue<std::string>(SC_RSRVD_ES_VENDOR_DEVICE_TYPE);
                        }
                    }
                }
                return std::string("");
            }

            /**
             * Get a device sub-type of Enrollee.
             * It is Device's human-friendly name like device model name.
             *
             * @return a device sub-type of Enrollee
             */
            std::string getDeviceSubType() const
            {
                std::vector<OCRepresentation> children = m_EasySetupRep.getChildren();
                for(auto child = children.begin(); child != children.end(); ++child)
                {
                    if(child->getUri().find(OC_RSRVD_ES_URI_DEVCONF) != std::string::npos)
                    {
                        OCRepresentation rep;
                        if(child->hasAttribute(OC_RSRVD_REPRESENTATION))
                        {
                            rep = child->getValue<OCRepresentation>(OC_RSRVD_REPRESENTATION);
                        }
                        else
                        {
                            return std::string("");
                        }

                        if(rep.hasAttribute(SC_RSRVD_ES_VENDOR_DEVICE_SUBTYPE))
                        {
                            return rep.getValue<std::string>(SC_RSRVD_ES_VENDOR_DEVICE_SUBTYPE);
                        }
                    }
                }
                return std::string("");
            }

            /**
             * Get Set device information used for 'register TV'
             * The format is in json. The key indicates the meaning of value and the value is a
             * corresponding information. Supported keys include: wm(Wifi MAC), pm(P2P MAC),
             * bm(BT MAC), rt(Remote Type)
             *
             * @return Register Set Device
             */
            std::string getRegisterSetDevice() const
            {
                std::vector<OCRepresentation> children = m_EasySetupRep.getChildren();
                for(auto child = children.begin(); child != children.end(); ++child)
                {
                    if(child->getUri().find(OC_RSRVD_ES_URI_DEVCONF) != std::string::npos)
                    {
                        OCRepresentation rep;
                        if(child->hasAttribute(OC_RSRVD_REPRESENTATION))
                        {
                            rep = child->getValue<OCRepresentation>(OC_RSRVD_REPRESENTATION);
                        }
                        else
                        {
                            return std::string("");
                        }

                        if(rep.hasAttribute(SC_RSRVD_ES_VENDOR_REGISTER_SET_DEV))
                        {
                            return rep.getValue<std::string>(SC_RSRVD_ES_VENDOR_REGISTER_SET_DEV);
                        }
                    }
                }
                return std::string("");
            }

            /**
             * Get T&C status
             * Indicates T&C is already agreed or not.
             *
             * @return T&C Status
             *
             * @see TC_STATUS
             */
            TC_STATUS getTCStatus()
            {
                OCRepresentation rep;
                if(m_EasySetupRep.hasAttribute(OC_RSRVD_REPRESENTATION))
                {
                    rep = m_EasySetupRep.getValue<OCRepresentation>(OC_RSRVD_REPRESENTATION);
                }
                else
                {
                    return TC_STATUS::TC_STATUS_NOT_SUPPORTED;
                }

                if(rep.hasAttribute(SC_RSRVD_ES_VENDOR_TC_STATUS))
                {
                    return static_cast<TC_STATUS>
                            (rep.getValue<int>(SC_RSRVD_ES_VENDOR_TC_STATUS));
                }

                return TC_STATUS::TC_STATUS_NOT_SUPPORTED;
            }

            /**
             * Get T&C Header
             * T&C Header Information in json format
             *
             * @return T&C Header
             */
            std::string getTCHeader()
            {
                std::vector<OCRepresentation> children = m_EasySetupRep.getChildren();
                for(auto child = children.begin(); child != children.end(); ++child)
                {
                    if(child->getUri().find(OC_RSRVD_ES_URI_DEVCONF) != std::string::npos)
                    {
                        OCRepresentation rep;
                        if(child->hasAttribute(OC_RSRVD_REPRESENTATION))
                        {
                            rep = child->getValue<OCRepresentation>(OC_RSRVD_REPRESENTATION);
                        }
                        else
                        {
                            return std::string("");
                        }

                        if(rep.hasAttribute(SC_RSRVD_ES_VENDOR_TC_HEADER))
                        {
                            return rep.getValue<std::string>(SC_RSRVD_ES_VENDOR_TC_HEADER);
                        }
                    }
                }
                return std::string("");
            }

            /**
             * Get T&C Version
             *
             * @return T&C Version
             */
            std::string getTCVersion()
            {
                std::vector<OCRepresentation> children = m_EasySetupRep.getChildren();
                for(auto child = children.begin(); child != children.end(); ++child)
                {
                    if(child->getUri().find(OC_RSRVD_ES_URI_DEVCONF) != std::string::npos)
                    {
                        OCRepresentation rep;
                        if(child->hasAttribute(OC_RSRVD_REPRESENTATION))
                        {
                            rep = child->getValue<OCRepresentation>(OC_RSRVD_REPRESENTATION);
                        }
                        else
                        {
                            return std::string("");
                        }

                        if(rep.hasAttribute(SC_RSRVD_ES_VENDOR_TC_VERSION))
                        {
                            return rep.getValue<std::string>(SC_RSRVD_ES_VENDOR_TC_VERSION);
                        }
                    }
                }
                return std::string("");
            }

            /**
             * Get PnP Pin
             *
             * @return PnP Pin
             */
            std::string getPnpPin() const
            {
                std::vector<OCRepresentation> children = m_EasySetupRep.getChildren();
                for(auto child = children.begin(); child != children.end(); ++child)
                {
                    if(child->getUri().find(OC_RSRVD_ES_URI_DEVCONF) != std::string::npos)
                    {
                        OCRepresentation rep;
                        if(child->hasAttribute(OC_RSRVD_REPRESENTATION))
                        {
                            rep = child->getValue<OCRepresentation>(OC_RSRVD_REPRESENTATION);
                        }
                        else
                        {
                            return std::string("");
                        }

                        if(rep.hasAttribute(SC_RSRVD_ES_VENDOR_PNP_PIN))
                        {
                            return rep.getValue<std::string>(SC_RSRVD_ES_VENDOR_PNP_PIN);
                        }
                    }
                }
                return std::string("");
            }

            /**
             * Get a network connection state property of Enrollee.
             *
             * @return a network connection state property of Enrollee.
             */
            NETCONNECTION_STATE getNetConnectionState() const
            {
                OCRepresentation rep;
                if(m_EasySetupRep.hasAttribute(OC_RSRVD_REPRESENTATION))
                {
                    rep = m_EasySetupRep.getValue<OCRepresentation>(OC_RSRVD_REPRESENTATION);
                }
                else
                {
                    return NETCONNECTION_STATE::NET_STATE_INIT;
                }

                if(rep.hasAttribute(SC_RSRVD_ES_VENDOR_NETCONNECTION_STATE))
                {
                    return static_cast<NETCONNECTION_STATE>
                                    (rep.getValue<int>(SC_RSRVD_ES_VENDOR_NETCONNECTION_STATE));
                }

                return NETCONNECTION_STATE::NET_STATE_INIT;
            }

            /**
             * Get a BSSID of Enroller
             *
             * @return a BSSID of enroller
             */
            std::string getBSSID() const
            {
                std::vector<OCRepresentation> children = m_EasySetupRep.getChildren();
                for(auto child = children.begin(); child != children.end(); ++child)
                {
                    if(child->getUri().find(OC_RSRVD_ES_URI_WIFICONF) != std::string::npos)
                    {
                        OCRepresentation rep;
                        if(child->hasAttribute(OC_RSRVD_REPRESENTATION))
                        {
                            rep = child->getValue<OCRepresentation>(OC_RSRVD_REPRESENTATION);
                        }
                        else
                        {
                            return std::string("");
                        }

                        if(rep.hasAttribute(SC_RSRVD_ES_VENDOR_BSSID))
                        {
                            return rep.getValue<std::string>(SC_RSRVD_ES_VENDOR_BSSID);
                        }
                    }
                }
                return {};
            }
        };
    }
}

#endif //ESSC_COMMON_RICH_H_
