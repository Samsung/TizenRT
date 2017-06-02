//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
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

#ifndef OC_OCAPI_H_
#define OC_OCAPI_H_

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <memory>
#include <iterator>
#if defined(_MSC_VER)
#include <functional>
#endif

#include "octypes.h"
#include "OCHeaderOption.h"
#include <OCException.h>
#include "StringConstants.h"
#include "oc_logger.hpp"

#include <OCRepresentation.h>

namespace OC
{
    class OCResource;
    class OCResourceRequest;
    class OCResourceResponse;
    class OCDirectPairing;
} // namespace OC

namespace OC
{
#if defined(_MSC_VER)
    extern std::ostream& oclog();
#else
    typedef boost::iostreams::stream<OC::oc_log_stream>     log_target_t;

    namespace detail
    {
        /* We'll want to provide some sort of explicit hook for custom logging at some
        point; until then, this should do nicely (note that since these are lambdas,
        later a special target could be captured, allowing much flexibility): */
        auto oclog_target = []() -> log_target_t&
        {
            static OC::oc_log_stream    ols(oc_make_ostream_logger);
            static log_target_t         os(ols);

            return os;
        };
    } // namespace OC::detail

    auto oclog = []() -> boost::iostreams::stream<OC::oc_log_stream>&
    {
        return detail::oclog_target();
    };
#endif
} // namespace OC

namespace OC
{

    enum class OCPlatformStatus
    {
        PlatformUp,
        PlatformDown
    };

    enum class OCAdvertisementStatus
    {
        None
    };

    typedef std::string URI;

    enum class ServiceType
    {
        InProc,
        OutOfProc
    };

    /**
     * Host Mode of Operation.
     */
    enum class ModeType
    {
        Server,
        Client,
        Both,
        Gateway  /**< Client server mode along with routing capabilities.*/
    };

    /**
     * Quality of Service attempts to abstract the guarantees provided by the underlying transport
     * protocol. The precise definitions of each quality of service level depend on the
     * implementation. In descriptions below are for the current implementation and may changed
     * over time.
     */
    enum class QualityOfService : uint8_t
    {
        /** Packet delivery is best effort. */
        LowQos      = OC_LOW_QOS,

        /** Packet delivery is best effort. */
        MidQos      = OC_MEDIUM_QOS,

        /** Acknowledgments are used to confirm delivery. */
        HighQos     = OC_HIGH_QOS,

        /** No Quality is defined, let the stack decide. */
        NaQos       = OC_NA_QOS
    };

    /**
     *  Data structure to provide the configuration.
     */
    struct PlatformConfig
    {
        /** indicate InProc or OutOfProc. */
        ServiceType                serviceType;

        /** indicate whether we want to do server, client or both. */
        ModeType                   mode;

        /** default flags for server. */
        OCConnectivityType         serverConnectivity;

        /** default flags for client. */
        OCConnectivityType         clientConnectivity;

        /** transport type to initialize. */
        OCTransportAdapter         transportType;

        /** not used. */
        std::string                ipAddress;

        /** not used. */
        uint16_t                   port;

        /** indicate Quality of Service : LowQos, MidQos,HighQos and NaQos(No quality Defined). */
        QualityOfService           QoS;

        /** persistant storage Handler structure (open/read/write/close/unlink). */
        OCPersistentStorage        *ps;

        /** persistant storage Handler structure (open/read/write/close/unlink). */
        OCPersistentStorage        *psEnc;

        /** persistant storage Handler structure (open/read/write/close/unlink). */
        OCPersistentStorage        *psRescue;

        /** pointer to save key. */
        unsigned char*              key;

        public:
            PlatformConfig()
                : serviceType(ServiceType::InProc),
                mode(ModeType::Both),
                serverConnectivity(CT_DEFAULT),
                clientConnectivity(CT_DEFAULT),
                transportType(OC_DEFAULT_ADAPTER),
                ipAddress("0.0.0.0"),
                port(0),
                QoS(QualityOfService::NaQos),
                key(nullptr),
                ps(nullptr),
                psEnc(nullptr),
                psRescue(nullptr)
        {}
            PlatformConfig(const ServiceType serviceType_,
            const ModeType mode_,
            OCConnectivityType serverConnectivity_,
            OCConnectivityType clientConnectivity_,
            const QualityOfService QoS_,
            OCPersistentStorage *ps_ = nullptr,
            OCPersistentStorage *psEnc_ = nullptr,
            OCPersistentStorage *psRescue_ = nullptr,
            unsigned char *key_ = nullptr)
                : serviceType(serviceType_),
                mode(mode_),
                serverConnectivity(serverConnectivity_),
                clientConnectivity(clientConnectivity_),
                transportType(OC_DEFAULT_ADAPTER),
                ipAddress(""),
                port(0),
                QoS(QoS_),
                key(key_),
                ps(ps_),
                psEnc(psEnc_),
                psRescue(psRescue_)

        {}
            // for backward compatibility
            PlatformConfig(const ServiceType serviceType_,
            const ModeType mode_,
            const std::string& ipAddress_,
            const uint16_t port_,
            const QualityOfService QoS_,
            OCPersistentStorage *ps_ = nullptr,
            OCPersistentStorage *psEnc_ = nullptr,
            OCPersistentStorage *psRescue_ = nullptr,
            unsigned char *key_ = nullptr)
                : serviceType(serviceType_),
                mode(mode_),
                serverConnectivity(CT_DEFAULT),
                clientConnectivity(CT_DEFAULT),
                transportType(OC_DEFAULT_ADAPTER),
                ipAddress(ipAddress_),
                port(port_),
                QoS(QoS_),
                key(key_),
                ps(ps_),
                psEnc(psEnc_),
                psRescue(psRescue_)
 
        {}

            PlatformConfig(const ServiceType serviceType_,
            const ModeType mode_,
            const std::string& ipAddress_,
            const uint16_t port_,
            const OCTransportAdapter transportType_,
            const QualityOfService QoS_,
            unsigned char *key_,
            OCPersistentStorage *ps_ = nullptr,
            OCPersistentStorage *psEnc_ = nullptr,
            OCPersistentStorage *psRescue_ = nullptr)
                : serviceType(serviceType_),
                mode(mode_),
                serverConnectivity(CT_DEFAULT),
                clientConnectivity(CT_DEFAULT),
                transportType(transportType_),
                ipAddress(ipAddress_),
                port(port_),
                QoS(QoS_),
                transportType(transportType_),
                ps(ps_),
                psEnc(psEnc_),
                psRescue(psRescue_),
                key(key_)
        {}

           PlatformConfig(const ServiceType serviceType_,
            const ModeType mode_,
            OCTransportAdapter transportType_,
            const QualityOfService QoS_,
            OCPersistentStorage *ps_ = nullptr,
            OCPersistentStorage *psEnc_ = nullptr,
            OCPersistentStorage *psRescue_ = nullptr,
            unsigned char *key_ = nullptr)
                : serviceType(serviceType_),
                mode(mode_),
                serverConnectivity(CT_DEFAULT),
                clientConnectivity(CT_DEFAULT),
                transportType(transportType_),
                ipAddress(""),
                port(0),
                QoS(QoS_),
                ps(ps_),
                psEnc(psEnc_),
                psRescue(psRescue_),
                key(key_)
        {}
            PlatformConfig(const ServiceType serviceType_,
            const ModeType mode_,
            OCConnectivityType serverConnectivity_,
            OCConnectivityType clientConnectivity_,
            OCTransportAdapter transportType_,
            const QualityOfService QoS_,
            OCPersistentStorage *ps_ = nullptr,
            OCPersistentStorage *psEnc_ = nullptr,
            OCPersistentStorage *psRescue_ = nullptr,
            unsigned char *key_ = nullptr)
                : serviceType(serviceType_),
                mode(mode_),
                serverConnectivity(serverConnectivity_),
                clientConnectivity(clientConnectivity_),
                transportType(transportType_),
                ipAddress(""),
                port(0),
                QoS(QoS_),
                ps(ps_),
                psEnc(psEnc_),
                psRescue(psRescue_),
                key(key_)
        {}

    };

    enum RequestHandlerFlag
    {
        RequestFlag = 1 << 1,
        ObserverFlag = 1 << 2
    };

    enum class ObserveType
    {
        Observe,
        ObserveAll
    };

    // Typedef for list of resource handles.
    typedef std::vector<OCResourceHandle> ResourceHandles;

    // Typedef for header option vector.
    // OCHeaderOption class is in HeaderOption namespace.
    typedef std::vector<HeaderOption::OCHeaderOption> HeaderOptions;

    // Typedef for query parameter map.
    typedef std::map<std::string, std::string> QueryParamsMap;

    // Typedef for query parameter map with Vector
    typedef std::map< std::string, std::vector<std::string> > QueryParamsList;

    // Typedef for list of observation IDs.
    typedef std::vector<OCObservationId> ObservationIds;

    enum class ObserveAction
    {
        ObserveRegister,
        ObserveUnregister
    };

    typedef struct
    {
        // Action associated with observation request
        ObserveAction action;
        // Identifier for observation being registered/unregistered
        OCObservationId obsId;

        OCConnectivityType connectivityType;
        std::string address;
        uint16_t port;
    } ObservationInfo;

    // const strings for different interfaces

    // Default interface
    const std::string DEFAULT_INTERFACE = "oic.if.baseline";

    // Used in discovering (GET) links to other resources of a collection.
    const std::string LINK_INTERFACE = "oic.if.ll";

    // Used in GET, PUT, POST, DELETE methods on links to other resources of a collection.
    const std::string BATCH_INTERFACE = "oic.if.b";

    // Used in GET, PUT, POST methods on links to other remote resources of a group.
    const std::string GROUP_INTERFACE = "oic.mi.grp";

    //Typedef for list direct paired devices
    typedef std::vector<std::shared_ptr<OCDirectPairing>> PairedDevices;

    typedef std::function<void(std::shared_ptr<OCResource>)> FindCallback;

    typedef std::function<void(const std::string&, const int)> FindErrorCallback;

    typedef std::function<void(std::vector<std::shared_ptr<OCResource>>)> FindResListCallback;

    typedef std::function<void(const OCRepresentation&)> FindDeviceCallback;

    typedef std::function<void(const OCRepresentation&)> FindPlatformCallback;

    typedef std::function<OCEntityHandlerResult(
                            const std::shared_ptr<OCResourceRequest>)> EntityHandler;

    typedef std::function<void(OCStackResult, const unsigned int,
                                const std::string&)> SubscribeCallback;

    typedef std::function<void(const HeaderOptions&,
                                const OCRepresentation&, const int)> GetCallback;

    typedef std::function<void(const HeaderOptions&,
                                const OCRepresentation&, const int)> PostCallback;

    typedef std::function<void(const HeaderOptions&,
                                const OCRepresentation&, const int)> PutCallback;

    typedef std::function<void(const HeaderOptions&, const int)> DeleteCallback;

    typedef std::function<void(const HeaderOptions&,
                                const OCRepresentation&, const int, const int)> ObserveCallback;

    typedef std::function<void(std::shared_ptr<OCDirectPairing>, OCStackResult)> DirectPairingCallback;

    typedef std::function<void(const PairedDevices&)> GetDirectPairedCallback;

    typedef std::function<void(const int, const std::string&,
                               std::shared_ptr<OCResource>)> MQTopicCallback;
#ifdef TCP_ADAPTER
    typedef std::function<void(const int, const OCRepresentation&)> KeepAliveCallback;
#endif
} // namespace OC

#endif
