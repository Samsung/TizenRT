//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
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

#include "RCSResourceObject.h"

#include <functional>

#include "RequestHandler.h"
#include "AssertUtils.h"
#include "AtomicHelper.h"
#include "ResourceAttributesConverter.h"
#include "ResourceAttributesUtils.h"
#include "RCSRequest.h"
#include "RCSRepresentation.h"
#include "InterfaceHandler.h"

#include "logger.h"
#include "OCPlatform.h"

#define LOG_TAG "RCSResourceObject"

namespace
{
    using namespace OIC::Service;

    inline bool hasProperty(uint8_t base, uint8_t target)
    {
        return (base & target) == target;
    }

    inline uint8_t makePropertyFlags(uint8_t base, uint8_t target, bool add)
    {
        if (add)
        {
            return base | target;
        }

        return base & ~target;
    }

    OCEntityHandlerResult sendResponse(const std::shared_ptr< OC::OCResourceRequest >& ocRequest,
            const std::shared_ptr< OC::OCResourceResponse >& ocResponse)
    {
        ocResponse->setRequestHandle(ocRequest->getRequestHandle());
        ocResponse->setResourceHandle(ocRequest->getResourceHandle());

        try
        {
            if (OC::OCPlatform::sendResponse(ocResponse) == OC_STACK_OK)
            {
                return OC_EH_OK;
            }
        }
        catch (const OC::OCException& e)
        {
            OIC_LOG_V(WARNING, LOG_TAG, "Error (%s)", e.what());
        }

        return OC_EH_ERROR;
    }

    RCSResourceAttributes getAttributesFromOCRequest(
            const std::shared_ptr< OC::OCResourceRequest >& request)
    {
        return ResourceAttributesConverter::fromOCRepresentation(
                request->getResourceRepresentation());
    }

    template< typename HANDLER, typename RESPONSE =
            typename std::decay<HANDLER>::type::result_type >
    RESPONSE invokeHandler(RCSResourceAttributes& attrs, const RCSRequest& request,
            std::shared_ptr< HANDLER > handler)
    {
        if (handler && *handler)
        {
            return (*handler)(request, attrs);
        }

        return RESPONSE::defaultAction();
    }

    typedef void (RCSResourceObject::* AutoNotifyFunc)
            (bool, RCSResourceObject::AutoNotifyPolicy) const;

    std::function<void()> createAutoNotifyInvoker(AutoNotifyFunc autoNotifyFunc,
            const RCSResourceObject& resourceObject,
            const RCSResourceAttributes& resourceAttributes,
            RCSResourceObject::AutoNotifyPolicy autoNotifyPolicy)
    {
        if(autoNotifyPolicy == RCSResourceObject::AutoNotifyPolicy::UPDATED)
        {
            auto&& compareAttributesFunc =
                    std::bind(std::not_equal_to<RCSResourceAttributes>(),
                                resourceAttributes,
                                std::cref(resourceAttributes));
            return std::bind(autoNotifyFunc,
                    &resourceObject, std::move(compareAttributesFunc), autoNotifyPolicy);
        }
        else if(autoNotifyPolicy == RCSResourceObject::AutoNotifyPolicy::ALWAYS)
        {
            return std::bind(autoNotifyFunc,
                    &resourceObject, true, autoNotifyPolicy);
        }
        return {};
    }

    void insertValue(std::vector<std::string>& container, std::string value)
    {
        if (value.empty()) return;

        if (std::find(container.begin(), container.end(), value) == container.end())
        {
            container.push_back(std::move(value));
        }
    }

} // unnamed namespace

namespace OIC
{
    namespace Service
    {

        RCSResourceObject::Builder::Builder(std::string uri, std::string type,
                std::string interface) :
                m_uri{ std::move(uri) },
                m_types{ std::move(type) },
                m_interfaces{ },
                m_defaultInterface{ interface },
                m_properties{ OC_DISCOVERABLE | OC_OBSERVABLE },
                m_resourceAttributes{ }
        {
            addInterface(interface);
            addInterface(BASELINE_INTERFACE);

            if (m_defaultInterface.empty())
            {
                m_defaultInterface = BASELINE_INTERFACE;
            }
        }

        RCSResourceObject::Builder& RCSResourceObject::Builder::addInterface(std::string interface)
        {
            insertValue(m_interfaces, std::move(interface));
            return *this;
        }

        RCSResourceObject::Builder& RCSResourceObject::Builder::addType(std::string type)
        {
            insertValue(m_types, std::move(type));
            return *this;
        }

        RCSResourceObject::Builder& RCSResourceObject::Builder::setDefaultInterface(
                std::string interface)
        {
            if (std::find(m_interfaces.begin(), m_interfaces.end(), interface) ==
                    m_interfaces.end())
            {
                throw RCSBadRequestException{"The interface should be added, first."};
            }

            m_defaultInterface = std::move(interface);

            return *this;
        }

        RCSResourceObject::Builder& RCSResourceObject::Builder::setDiscoverable(
                bool discoverable)
        {
            m_properties = ::makePropertyFlags(m_properties, OC_DISCOVERABLE, discoverable);
            return *this;
        }

        RCSResourceObject::Builder& RCSResourceObject::Builder::setObservable(
                bool observable)
        {
            m_properties = ::makePropertyFlags(m_properties, OC_OBSERVABLE, observable);
            return *this;
        }

        RCSResourceObject::Builder& RCSResourceObject::Builder::setSecureFlag(
            bool secureFlag)
        {
            m_properties = ::makePropertyFlags(m_properties, OC_SECURE, secureFlag);
            return *this;
        }
        RCSResourceObject::Builder& RCSResourceObject::Builder::setAttributes(
                const RCSResourceAttributes& attrs)
        {
            m_resourceAttributes = attrs;
            return *this;
        }

        RCSResourceObject::Builder& RCSResourceObject::Builder::setAttributes(
                RCSResourceAttributes&& attrs)
        {
            m_resourceAttributes = std::move(attrs);
            return *this;
        }

        RCSResourceObject::Ptr RCSResourceObject::Builder::build()
        {
            OCResourceHandle handle{ nullptr };

            RCSResourceObject::Ptr server {
                new RCSResourceObject{ m_uri, m_properties, std::move(m_resourceAttributes) } };

            OC::EntityHandler entityHandler{ std::bind(&RCSResourceObject::entityHandler,
                    std::weak_ptr< RCSResourceObject >{ server }, std::placeholders::_1) };

            typedef OCStackResult (*RegisterResource)(OCResourceHandle&, std::string&,
                    const std::string&, const std::string&, OC::EntityHandler, uint8_t);

            invokeOCFunc(static_cast<RegisterResource>(OC::OCPlatform::registerResource),
                    handle, m_uri, m_types[0], m_interfaces[0], entityHandler, m_properties);

            std::for_each(m_interfaces.begin() + 1, m_interfaces.end(),
                    [&handle](const std::string& interfaceName){
                invokeOCFunc(OC::OCPlatform::bindInterfaceToResource, handle, interfaceName);
            });

            std::for_each(m_types.begin() + 1, m_types.end(),
                    [&handle](const std::string& typeName){
                invokeOCFunc(OC::OCPlatform::bindTypeToResource, handle, typeName);
            });

            server->init(handle, m_interfaces, m_types, m_defaultInterface);

            return server;
        }


        RCSResourceObject::RCSResourceObject(const std::string& uri,
                uint8_t properties, RCSResourceAttributes&& attrs) :
                m_properties{ properties },
                m_uri{ uri },
                m_interfaces{ },
                m_types{ },
                m_defaultInterface{ },
                m_resourceHandle{ },
                m_resourceAttributes{ std::move(attrs) },
                m_getRequestHandler{ },
                m_setRequestHandler{ },
                m_autoNotifyPolicy{ AutoNotifyPolicy::UPDATED },
                m_setRequestHandlerPolicy{ SetRequestHandlerPolicy::NEVER },
                m_attributeUpdatedListeners{ },
                m_lockOwner{ },
                m_mutex{ },
                m_mutexAttributeUpdatedListeners{ }
        {
            m_lockOwner.reset(new AtomicThreadId);
        }

        void RCSResourceObject::init(OCResourceHandle handle,
                const std::vector< std::string >& interfaces,
                const std::vector< std::string >& types,
                const std::string& defaultInterface)
        {
            m_resourceHandle = handle;
            m_interfaces = interfaces;
            m_types = types;
            m_defaultInterface = defaultInterface;

            for (const auto& itf : interfaces)
            {
                m_interfaceHandlers.insert({ itf, getDefaultInterfaceHandler(itf,
                        m_defaultInterface) });
            }
        }

        RCSResourceObject::~RCSResourceObject()
        {
            if (m_resourceHandle)
            {
                try
                {
                    invokeOCFunc(OC::OCPlatform::unregisterResource, m_resourceHandle);
                }
                catch (...)
                {
                    OIC_LOG(WARNING, LOG_TAG, "Failed to unregister resource.");
                }
            }
        }

        template< typename K, typename V >
        void RCSResourceObject::setAttributeInternal(K&& key, V&& value)
        {
            bool needToNotify = false;
            bool valueUpdated = false;

            {
                WeakGuard lock(*this);

                if (lock.hasLocked())
                {
                    needToNotify = true;
                    valueUpdated = testValueUpdated(key, value);
                }

                m_resourceAttributes[std::forward< K >(key)] = std::forward< V >(value);
            }

            if (needToNotify) autoNotify(valueUpdated);
        }
        void RCSResourceObject::setAttribute(const std::string& key,
                const RCSResourceAttributes::Value& value)
        {
            setAttributeInternal(key, value);
        }

        void RCSResourceObject::setAttribute(const std::string& key,
                RCSResourceAttributes::Value&& value)
        {
            setAttributeInternal(key, std::move(value));
        }

        void RCSResourceObject::setAttribute(std::string&& key,
                const RCSResourceAttributes::Value& value)
        {
            setAttributeInternal(std::move(key), value);
        }

        void RCSResourceObject::setAttribute(std::string&& key,
                RCSResourceAttributes::Value&& value)
        {
            setAttributeInternal(std::move(key), std::move(value));
        }

        RCSResourceAttributes::Value RCSResourceObject::getAttributeValue(
                const std::string& key) const
        {
            WeakGuard lock(*this);
            return m_resourceAttributes.at(key);
        }

        bool RCSResourceObject::removeAttribute(const std::string& key)
        {
            bool needToNotify = false;
            bool erased = false;
            {
                WeakGuard lock(*this);

                if (m_resourceAttributes.erase(key))
                {
                    erased = true;
                    needToNotify = lock.hasLocked();
                }
            }

            if (needToNotify) autoNotify(true);

            return erased;
        }

        bool RCSResourceObject::containsAttribute(const std::string& key) const
        {
            WeakGuard lock(*this);
            return m_resourceAttributes.contains(key);
        }

        RCSResourceAttributes& RCSResourceObject::getAttributes()
        {
            expectOwnLock();
            return m_resourceAttributes;
        }

        const RCSResourceAttributes& RCSResourceObject::getAttributes() const
        {
            expectOwnLock();
            return m_resourceAttributes;
        }

        void RCSResourceObject::expectOwnLock() const
        {
            if (getLockOwner() != std::this_thread::get_id())
            {
                throw NoLockException{ "Must acquire the lock first using LockGuard." };
            }
        }

        std::thread::id RCSResourceObject::getLockOwner() const noexcept
        {
            return *m_lockOwner;
        }

        void RCSResourceObject::setLockOwner(std::thread::id&& id) const noexcept
        {
            m_lockOwner->store(std::move(id));
        }

        bool RCSResourceObject::isObservable() const
        {
            return ::hasProperty(m_properties, OC_OBSERVABLE);
        }

        bool RCSResourceObject::isDiscoverable() const
        {
            return ::hasProperty(m_properties, OC_DISCOVERABLE);
        }

        void RCSResourceObject::setGetRequestHandler(GetRequestHandler h)
        {
            m_getRequestHandler = std::make_shared< GetRequestHandler >(std::move(h));
        }

        void RCSResourceObject::setSetRequestHandler(SetRequestHandler h)
        {
            m_setRequestHandler = std::make_shared< SetRequestHandler >(std::move(h));
        }

        void RCSResourceObject::notify() const
        {
            typedef OCStackResult (*NotifyAllObservers)(OCResourceHandle);

            invokeOCFuncWithResultExpect({ OC_STACK_OK, OC_STACK_NO_OBSERVERS },
                    static_cast< NotifyAllObservers >(OC::OCPlatform::notifyAllObservers),
                    m_resourceHandle);
        }

        void RCSResourceObject::addAttributeUpdatedListener(const std::string& key,
                AttributeUpdatedListener h)
        {
            std::lock_guard< std::mutex > lock(m_mutexAttributeUpdatedListeners);

            m_attributeUpdatedListeners[key] =
                    std::make_shared< AttributeUpdatedListener >(std::move(h));
        }

        void RCSResourceObject::addAttributeUpdatedListener(std::string&& key,
                AttributeUpdatedListener h)
        {
            std::lock_guard< std::mutex > lock(m_mutexAttributeUpdatedListeners);

            m_attributeUpdatedListeners[std::move(key)] =
                    std::make_shared< AttributeUpdatedListener >(std::move(h));
        }

        bool RCSResourceObject::removeAttributeUpdatedListener(const std::string& key)
        {
            std::lock_guard< std::mutex > lock(m_mutexAttributeUpdatedListeners);

            return m_attributeUpdatedListeners.erase(key) != 0;
        }

        bool RCSResourceObject::testValueUpdated(const std::string& key,
                const RCSResourceAttributes::Value& value) const
        {
            return m_resourceAttributes.contains(key) == false
                    || m_resourceAttributes.at(key) != value;
        }

        void RCSResourceObject::setAutoNotifyPolicy(AutoNotifyPolicy policy)
        {
            m_autoNotifyPolicy = policy;
        }

        RCSResourceObject::AutoNotifyPolicy RCSResourceObject::getAutoNotifyPolicy() const
        {
            return m_autoNotifyPolicy;
        }

        void RCSResourceObject::setSetRequestHandlerPolicy(SetRequestHandlerPolicy policy)
        {
            m_setRequestHandlerPolicy = policy;
        }

        auto RCSResourceObject::getSetRequestHandlerPolicy() const -> SetRequestHandlerPolicy
        {
            return m_setRequestHandlerPolicy;
        }

        void RCSResourceObject::bindResource(const RCSResourceObject::Ptr& resource)
        {
            if (!resource || resource.get() == this)
            {
                throw RCSInvalidParameterException("The resource is invalid!");
            }

            invokeOCFunc(OC::OCPlatform::bindResource,
                    m_resourceHandle, resource->m_resourceHandle);

            std::lock_guard< std:: mutex > lock{ m_mutexForBoundResources };
            m_boundResources.push_back(resource);
        }

        void RCSResourceObject::unbindResource(const RCSResourceObject::Ptr& resource)
        {
            if (!resource || resource.get() == this)
            {
                throw RCSInvalidParameterException("The resource is invalid!");
            }

            invokeOCFunc(OC::OCPlatform::unbindResource,
                    m_resourceHandle, resource->m_resourceHandle);

            std::lock_guard< std:: mutex > lock{ m_mutexForBoundResources };
            m_boundResources.erase(std::find(m_boundResources.begin(), m_boundResources.end(),
                    resource));
        }

        std::vector< RCSResourceObject::Ptr > RCSResourceObject::getBoundResources() const
        {
            std::lock_guard< std:: mutex > lock{ m_mutexForBoundResources };
            return m_boundResources;
        }

        std::string RCSResourceObject::getUri() const
        {
            return m_uri;
        }

        std::string RCSResourceObject::getDefaultInterface() const
        {
            return m_defaultInterface;
        }

        std::vector< std::string > RCSResourceObject::getInterfaces() const
        {
            return m_interfaces;
        }

        std::vector< std::string > RCSResourceObject::getTypes() const
        {
            return m_types;
        }

        RCSRepresentation RCSResourceObject::getRepresentation(const RCSRequest& request) const
        {
            if (request.getOCRequest()->getRequestType() == "GET")
            {
                return findInterfaceHandler(
                        request.getInterface()).getGetResponseBuilder()(request, *this);
            }

            if (request.getOCRequest()->getRequestType() == "POST")
            {
                return findInterfaceHandler(
                        request.getInterface()).getSetResponseBuilder()(request, *this);
            }

            throw RCSBadRequestException{ "Unsupported request type!" };
        }

        void RCSResourceObject::autoNotify(bool isAttributesChanged) const
        {
            autoNotify(isAttributesChanged, m_autoNotifyPolicy);
        }

        void RCSResourceObject::autoNotify(
                        bool isAttributesChanged, AutoNotifyPolicy autoNotifyPolicy) const
        {
            if(autoNotifyPolicy == AutoNotifyPolicy::NEVER) return;
            if(autoNotifyPolicy == AutoNotifyPolicy::UPDATED &&
                    isAttributesChanged == false) return;

            notify();
        }

        OCEntityHandlerResult RCSResourceObject::entityHandler(
                const std::weak_ptr< RCSResourceObject >& weakRes,
                const std::shared_ptr< OC::OCResourceRequest >& request)
        {
            auto resource = weakRes.lock();

            if (!resource) return OC_EH_ERROR;

            OIC_LOG(WARNING, LOG_TAG, "entityHandler");
            if (!request)
            {
                return OC_EH_ERROR;
            }

            try
            {
                RCSRequest rcsRequest{ resource, request };

                if (request->getRequestHandlerFlag() & OC::RequestHandlerFlag::RequestFlag)
                {
                    return resource->handleRequest(rcsRequest);
                }

                if (request->getRequestHandlerFlag() & OC::RequestHandlerFlag::ObserverFlag)
                {
                    return resource->handleObserve(rcsRequest);
                }
            }
            catch (const std::exception& e)
            {
                OIC_LOG_V(WARNING, LOG_TAG, "Failed to handle request : %s", e.what());
                throw;
            }
            catch (...)
            {
                OIC_LOG(WARNING, LOG_TAG, "Failed to handle request.");
                throw;
            }

            return OC_EH_ERROR;
        }

        OCEntityHandlerResult RCSResourceObject::handleRequest(
                const RCSRequest& request)
        {
            if (request.getInterface() != "" &&
                    m_interfaceHandlers.find(request.getInterface()) == m_interfaceHandlers.end())
            {
                return OC_EH_ERROR;
            }

            if (request.getOCRequest()->getRequestType() == "GET")
            {
                return handleRequestGet(request);
            }

            if (request.getOCRequest()->getRequestType() == "POST")
            {
                return handleRequestSet(request);
            }

            return OC_EH_ERROR;
        }

        OCEntityHandlerResult RCSResourceObject::handleRequestGet(const RCSRequest& request)
        {
            if (!findInterfaceHandler(request.getInterface()).isGetSupported())
            {
                return OC_EH_ERROR;
            }

            auto attrs = getAttributesFromOCRequest(request.getOCRequest());

            auto response = invokeHandler(attrs, request, m_getRequestHandler);

            if (response.isSeparate()) return OC_EH_SLOW;

            return sendResponse(request, response,
                         findInterfaceHandler(request.getInterface()).getGetResponseBuilder());
        }

        bool RCSResourceObject::applyAcceptanceMethod(
                const RCSSetResponse& response, const RCSResourceAttributes& requestAttrs)
        {
            auto requestHandler = response.getHandler();

            assert(requestHandler != nullptr);

            RCSResourceAttributes result;

            auto replaced = requestHandler->applyAcceptanceMethod(response.getAcceptanceMethod(),
                    *this, requestAttrs);

            OIC_LOG_V(WARNING, LOG_TAG, "replaced num %zu", replaced.size());
            for (const auto& attrKeyValPair : replaced)
            {
                std::shared_ptr< AttributeUpdatedListener > foundListener;
                {
                    std::lock_guard< std::mutex > lock(m_mutexAttributeUpdatedListeners);

                    auto it = m_attributeUpdatedListeners.find(attrKeyValPair.first);
                    if (it != m_attributeUpdatedListeners.end())
                    {
                        foundListener = it->second;
                    }
                }

                if (foundListener)
                {
                    (*foundListener)(attrKeyValPair.second, requestAttrs.at(attrKeyValPair.first));
                }
            }

            return !replaced.empty();
        }

        OCEntityHandlerResult RCSResourceObject::handleRequestSet(const RCSRequest& request)
        {
            if (!findInterfaceHandler(request.getInterface()).isSetSupported())
            {
                return OC_EH_ERROR;
            }

            auto attrs = getAttributesFromOCRequest(request.getOCRequest());

            auto response = invokeHandler(attrs, request, m_setRequestHandler);

            if (response.isSeparate()) return OC_EH_SLOW;

            autoNotify(applyAcceptanceMethod(response, attrs), m_autoNotifyPolicy);

            return sendResponse(request, response,
                    findInterfaceHandler(request.getInterface()).getSetResponseBuilder());
        }

        OCEntityHandlerResult RCSResourceObject::handleObserve(const RCSRequest&)
        {
            if (!isObservable())
            {
                return OC_EH_ERROR;
            }

            return OC_EH_OK;
        }

        InterfaceHandler RCSResourceObject::findInterfaceHandler(
                const std::string& interfaceName) const
        {
            auto it = m_interfaceHandlers.find(interfaceName);

            if (it != m_interfaceHandlers.end()) return it->second;

            assert(m_interfaceHandlers.find(m_defaultInterface) != m_interfaceHandlers.end());

            return m_interfaceHandlers.find(m_defaultInterface)->second;
        }

        template <typename RESPONSE, typename RESPONSE_BUILDER>
        OCEntityHandlerResult RCSResourceObject::sendResponse(
                const RCSRequest& request, const RESPONSE& response,
                const RESPONSE_BUILDER& resBuilder)
        {
            auto reqHandler = response.getHandler();
            auto ocResponse = std::make_shared< OC::OCResourceResponse >();

            ocResponse->setResponseResult(OC_EH_OK);
            ocResponse->setErrorCode(reqHandler->getErrorCode());

            if (reqHandler->hasCustomRepresentation())
            {
                ocResponse->setResourceRepresentation(reqHandler->getRepresentation());
            }
            else
            {
                ocResponse->setResourceRepresentation(
                        RCSRepresentation::toOCRepresentation(resBuilder(request, *this)));
            }

            return ::sendResponse(request.getOCRequest(), ocResponse);
        }


        RCSResourceObject::LockGuard::LockGuard(const RCSResourceObject::Ptr ptr) :
                m_resourceObject(*ptr),
                m_autoNotifyPolicy{ ptr->getAutoNotifyPolicy() },
                m_isOwningLock{ false }
        {
            init();
        }

        RCSResourceObject::LockGuard::LockGuard(
                const RCSResourceObject& serverResource) :
                m_resourceObject(serverResource),
                m_autoNotifyPolicy{ serverResource.getAutoNotifyPolicy() },
                m_isOwningLock{ false }
        {
            init();
        }

        RCSResourceObject::LockGuard::LockGuard(
                const RCSResourceObject::Ptr ptr, AutoNotifyPolicy autoNotifyPolicy) :
                m_resourceObject(*ptr),
                m_autoNotifyPolicy { autoNotifyPolicy },
                m_isOwningLock{ false }
        {
            init();
        }

        RCSResourceObject::LockGuard::LockGuard(
                const RCSResourceObject& resourceObject, AutoNotifyPolicy autoNotifyPolicy) :
                m_resourceObject(resourceObject),
                m_autoNotifyPolicy { autoNotifyPolicy },
                m_isOwningLock{ false }
        {
            init();
        }

        RCSResourceObject::LockGuard::~LockGuard() noexcept(false)
        {
            if (!std::uncaught_exception() && m_autoNotifyFunc) m_autoNotifyFunc();

            if (m_isOwningLock)
            {
                m_resourceObject.setLockOwner(std::thread::id{ });
                m_resourceObject.m_mutex.unlock();
            }
        }

        void RCSResourceObject::LockGuard::init()
        {
            if (m_resourceObject.getLockOwner() != std::this_thread::get_id())
            {
                m_resourceObject.m_mutex.lock();
                m_resourceObject.setLockOwner(std::this_thread::get_id());
                m_isOwningLock = true;
            }
            m_autoNotifyFunc = ::createAutoNotifyInvoker(&RCSResourceObject::autoNotify,
                    m_resourceObject, m_resourceObject.m_resourceAttributes, m_autoNotifyPolicy);
        }

      RCSResourceObject::WeakGuard::WeakGuard(
                const RCSResourceObject& resourceObject) :
                m_isOwningLock{ false },
                m_resourceObject(resourceObject)
        {
            if (m_resourceObject.getLockOwner() != std::this_thread::get_id())
            {
                m_resourceObject.m_mutex.lock();
                m_resourceObject.setLockOwner(std::this_thread::get_id());
                m_isOwningLock = true;
            }
        }

        RCSResourceObject::WeakGuard::~WeakGuard()
        {
            if (m_isOwningLock)
            {
                m_resourceObject.setLockOwner(std::thread::id{ });
                m_resourceObject.m_mutex.unlock();
            }
        }

        bool RCSResourceObject::WeakGuard::hasLocked() const
        {
            return m_isOwningLock;
        }

    }
}
