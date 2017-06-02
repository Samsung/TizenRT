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

#ifndef JNI_BUNDLERESOURCE_H_
#define JNI_BUNDLERESOURCE_H_

#include <map>
#include <vector>
#include <string>
#include <jni.h>
#include "BundleResource.h"
#include "SoftSensorResource.h"
#include "ResourceContainerImpl.h"

#include <jni.h>

class JNIEnvWrapper;

void initRCSJniBundleResource(JNIEnvWrapper *);

using namespace std;

namespace OIC
{
    namespace Service
    {
        class JniBundleResource: public SoftSensorResource
        {
        public:
            JniBundleResource();
            JniBundleResource(JNIEnv *env, jobject obj, jobject bundleResource, string bundleId,
                    jobjectArray attributes);
            virtual ~JniBundleResource();

            void handleSetAttributeRequest(const std::string& key,
                    RCSResourceAttributes::Value&&);

            RCSResourceAttributes::Value handleGetAttributeRequest(const std::string& key);

            virtual void handleSetAttributesRequest(const RCSResourceAttributes &attrs,
                                                    const std::map< std::string, std::string > &queryParams);

            virtual RCSResourceAttributes handleGetAttributesRequest(const
                         std::map< std::string, std::string > &queryParams);

            /**
            * SoftSensor logic. Has to be provided by the soft sensor developer.
            * This function will be executed if an input attribute is updated.
            *
            * @return void
            */
            virtual void executeLogic();

            /**
            * Callback from the client module in the container.
            * This function will be called if input data from remote resources are updated.
            * SoftSensor resource can get a vector of input data from multiple input resources
            *    which have attributeName that softsensor needs to execute its logic.
            *
            * @param attributeName Attribute key of input data
            *
            * @param values Vector of input data value
            *
            * @return void
            */
            virtual void onUpdatedInputResource(const std::string attributeName,
                                                std::vector<RCSResourceAttributes::Value> values);

            virtual void initAttributes();
        private:
            // needs to be a GlobalRef
            jobject m_bundleResource;
            jobjectArray m_attributes;
            jclass m_bundleResourceClass;
            jmethodID m_attributeSetRequestHandler;
            jmethodID m_attributeGetRequestHandler;
            jmethodID m_onUpdatedInputResource;
            jclass m_vectorClazz;
            jmethodID m_vectorAddMethod;
            string m_bundleId;
            jclass m_superclass;
            jclass m_classClass;
            JNIEnv *m_env;
            JavaVM *m_jvm;
            jfieldID g_field_mNativeHandle;
        };
    }
}

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_iotivity_service_resourcecontainer_JniBundleResource
 * Method:    updateNativeInstance
 * Signature: (Lorg/iotivity/service/resourcecontainer/RcsResourceAttributes;)V
 */
JNIEXPORT void JNICALL Java_org_iotivity_service_resourcecontainer_BundleResource_updateNativeInstance
  (JNIEnv *, jobject, jobject);

#ifdef __cplusplus
}
#endif
#endif

