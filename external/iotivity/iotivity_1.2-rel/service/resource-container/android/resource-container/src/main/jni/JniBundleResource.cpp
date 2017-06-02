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


#include "JniBundleResource.h"
#include "JniRcsResourceAttributes.h"
#include "JavaClasses.h"
#include "JavaLocalRef.h"
#include "JNIEnvWrapper.h"
#include "JniRcsValue.h"

#include <jni.h>
#include <string.h>
#include <iostream>
#include "Log.h"

#define LOG_TAG "JNI-JniBundleResource"

using namespace OIC::Service;
using namespace std;

namespace
{
    jclass g_cls_RCSBundleInfo;
    jfieldID g_field_mNativeHandle;
}

void initRCSJniBundleResource(JNIEnvWrapper *env)
{
    auto clsJniBundleResource = env->FindClass(PACKAGE_NAME "/BundleResource");

    g_field_mNativeHandle = env->GetFieldID(clsJniBundleResource, "mNativeHandle", "J");
}

JniBundleResource::JniBundleResource()
{

}

void JniBundleResource::initAttributes()
{

}

JniBundleResource::JniBundleResource(JNIEnv *env, jobject obj, jobject bundleResource,
                                       string bundleId, jobjectArray attributes)
{
    LOGD("Creating android resource, bundleId: %s", bundleId.c_str());
    (void) obj;
    m_env = env;
    int stringCount = m_env->GetArrayLength(attributes);
    LOGD("string count is %d", stringCount);

    LOGD("Get java vm.");
    int jvmAccess = m_env->GetJavaVM(&m_jvm);


    for (int i = 0; i < stringCount; i++)
    {
        jstring str = (jstring) m_env->GetObjectArrayElement(attributes, i);
        const char *rawString = m_env->GetStringUTFChars(str, 0);
        string s(rawString, strlen(rawString));
        JniBundleResource::setAttribute(s, "");
        m_env->ReleaseStringUTFChars(str, rawString);
        m_env->DeleteLocalRef(str);
        LOGD("Deleting and releasing resources - JNI bundle resource");
    }

    m_bundleId = bundleId;

    this->m_bundleResource = m_env->NewGlobalRef(bundleResource);

    m_bundleResourceClass = (jclass) m_env->NewGlobalRef(m_env->GetObjectClass(bundleResource));
    LOGD("Looking for setter.");
    m_attributeSetRequestHandler = m_env->GetMethodID(m_bundleResourceClass,
            "handleSetAttributesRequest",
            "(Lorg/iotivity/service/resourcecontainer/RcsResourceAttributes;)V");
    LOGD("Looking for getter.");
    m_attributeGetRequestHandler = m_env->GetMethodID(m_bundleResourceClass,
            "handleGetAttributesRequest",
            "()Lorg/iotivity/service/resourcecontainer/RcsResourceAttributes;");

    LOGD("Looking for onUpdatedInputResource.");
    m_superclass = (jclass) m_env->NewGlobalRef(m_env->GetSuperclass(m_bundleResourceClass));

    m_classClass = (jclass) m_env->NewGlobalRef(m_env->FindClass("java/lang/Class"));


    m_vectorClazz = (jclass) m_env->NewGlobalRef(m_env->FindClass("java/util/Vector"));
    if (m_env->ExceptionCheck()) {
        m_env->ExceptionDescribe();
    }

    if (m_env->ExceptionCheck()) {
       m_env->ExceptionDescribe();
    }

    if(m_classClass != NULL){
        // Find the getName() method on the class object
        jmethodID mid = env->GetMethodID(m_classClass, "getName", "()Ljava/lang/String;");

        // Call the getName() to get a jstring object back
        if(m_superclass != NULL){
            jstring strObj = (jstring)env->CallObjectMethod(m_superclass, mid);

            // Now get the c string from the java jstring object
            const char* str = env->GetStringUTFChars(strObj, NULL);

            LOGD("Name of super class is %s", str);

            //check for softsensor resource
            if(strcmp("org.iotivity.service.resourcecontainer.BundleSoftSensorResource", str) == 0){
               m_onUpdatedInputResource = m_env->GetMethodID(m_bundleResourceClass,
                       "onUpdatedInputResource", "(Ljava/lang/String;Ljava/util/Vector;)V");
               if (m_env->ExceptionCheck()) {
                   m_env->ExceptionDescribe();
               }

               LOGD("Looking up vector add method.");
               if(m_vectorClazz != NULL){
                   m_vectorAddMethod =  m_env->GetMethodID(m_vectorClazz, "add", "(Ljava/lang/Object;)Z");
                   if (m_env->ExceptionCheck()) {
                       m_env->ExceptionDescribe();
                   }
               }

            }
            LOGD("Deleting and releasing resources - JNIBundleResource 2");
            m_env->ReleaseStringUTFChars(strObj, str);
        }
    }
}

JniBundleResource::~JniBundleResource()
{

}

RCSResourceAttributes::Value JniBundleResource::handleGetAttributeRequest(
        const std::string &attributeName)
{
    LOGD("handleGetAttributeRequest called2");
    LOGD("Attaching thread now");
    int attached = m_jvm->AttachCurrentThread(&m_env, NULL);
    if(attached>0)
    {
        LOGE("Failed to attach thread to JavaVM");
    }
    else{
        if(m_attributeGetRequestHandler != NULL){
            jstring attrName = m_env->NewStringUTF(attributeName.c_str());
            auto responseObj =  m_env->CallObjectMethod(m_bundleResource,
                    m_attributeGetRequestHandler, attrName);

            if (responseObj)
            {
                LOGD("parsing attributes");
                RCSResourceAttributes attrs = toNativeAttributes(m_env, responseObj);
                LOGD("Received attributes %d", attrs.size());
            }
        }
    }
    return JniBundleResource::getAttribute(attributeName);
}

void JniBundleResource::handleSetAttributeRequest(const std::string &attributeName,
                                      RCSResourceAttributes::Value &&value)
{
    if(m_attributeSetRequestHandler != NULL){
        jstring attrName = m_env->NewStringUTF(attributeName.c_str());
        jstring val = m_env->NewStringUTF(value.toString().c_str());

        //LOGD("handleSetAttributeRequest calling object method %d", &m_attributeSetRequestHandler);
        m_env->CallObjectMethod(m_bundleResource, m_attributeSetRequestHandler, attrName, val);
    }
    JniBundleResource::setAttribute(attributeName, std::move(value));
}


void JniBundleResource::handleSetAttributesRequest(const RCSResourceAttributes &attrs,
        const std::map< std::string, std::string > &queryParams)
{
    LOGD("handleSetAttributesRequest called %d", attrs.size());

    //m_env->CallObjectMethod(m_bundleResource, m_attributeSetRequestHandler, attrName, val);
    //JniBundleResource::setAttribute(attributeName, std::move(value));
    if(m_attributeSetRequestHandler != NULL && m_bundleResource != NULL){
        int attached = m_jvm->AttachCurrentThread(&m_env, NULL);

        if(attached>0)
        {
            LOGE("Failed to attach thread to JavaVM");
        }
        else{
            LOGD("Creating resource attributes for JNI.");

            m_env->MonitorEnter(m_bundleResource);
            auto jniRcsAttributes = newAttributesObject(m_env, attrs);
            LOGD("jobject created. calling");
            m_env->CallVoidMethod(m_bundleResource,
                    m_attributeSetRequestHandler, jniRcsAttributes);
            JniBundleResource::setAttributes(attrs);
            m_env->MonitorExit(m_bundleResource);

            m_jvm->DetachCurrentThread();
        }
    }
}

RCSResourceAttributes JniBundleResource::handleGetAttributesRequest(const
        std::map< std::string, std::string > &queryParams)
{
    LOGD("handleGetAttributesRequest");

    if(m_attributeGetRequestHandler != NULL && m_bundleResource != NULL){
        LOGD("attaching thread");
        int attached = m_jvm->AttachCurrentThread(&m_env, NULL);
        if(attached>0)
        {
            LOGE("Failed to attach thread to JavaVM");
        }
        else{
            LOGD("attached, calling get request handler");
            auto responseObj =  m_env->CallObjectMethod(m_bundleResource,
                    m_attributeGetRequestHandler);

            if (responseObj)
            {
                LOGD("parsing attributes");
                m_env->MonitorEnter(m_bundleResource);
                LOGD("to native attributes");
                const RCSResourceAttributes attrs = toNativeAttributes(m_env, responseObj);
                LOGD("Received attributes %d", attrs.size());
                JniBundleResource::setAttributes(attrs, false);
                m_env->MonitorExit(m_bundleResource);
            }

            m_jvm->DetachCurrentThread();
        }
        LOGD("JniBundleResource::getAttributes().size() %d",
                JniBundleResource::getAttributes().size());
    }
    return JniBundleResource::getAttributes();
}

void JniBundleResource::executeLogic(){
    // IS CALLED AT JAVA LAYER
}

void JniBundleResource::onUpdatedInputResource(const std::string attributeName,
        std::vector<RCSResourceAttributes::Value> values){
    LOGD("onUpdatedInputResource");
    if(m_vectorClazz == NULL || m_classClass == NULL){
        return;
    }
    int attached = m_jvm->AttachCurrentThread(&m_env, NULL);
    jobject valueObj;
    if(attached>0)
    {
        LOGE("Failed to attach thread to JavaVM");
    }
    else{


        jobject obj = m_env->NewObject(m_vectorClazz,
                m_env->GetMethodID(m_vectorClazz, "<init>", "()V"));

        LOGD("Looking up vector add method.");

        jmethodID m_vectorAddMethod =  m_env->GetMethodID(m_vectorClazz, "add",
                "(Ljava/lang/Object;)Z");
        if (m_env->ExceptionCheck()) {
            m_env->ExceptionDescribe();
        }

        if(m_vectorAddMethod == NULL){
            m_jvm->DetachCurrentThread();
            return;
        }

        for (int n=0;n<values.size();n++)
        {
           valueObj  = newRCSValueObject(m_env, values[n]);
           m_env->CallBooleanMethod(obj, m_vectorAddMethod, valueObj);
        }

        // Find the getName() method on the class object
        jmethodID mid = m_env->GetMethodID(m_classClass, "getName",
                "()Ljava/lang/String;");

        // Call the getName() to get a jstring object back
        if(m_superclass == NULL || mid == NULL){
            m_jvm->DetachCurrentThread();
            return;
        }

        jstring strObj = (jstring)m_env->CallObjectMethod(m_superclass, mid);

        // Now get the c string from the java jstring object
        if(strObj == NULL){
            m_jvm->DetachCurrentThread();
            return;
        }
        const char* str = m_env->GetStringUTFChars(strObj, NULL);

        LOGD("Name of super class is %s", str);

        jstring attrName = m_env->NewStringUTF(attributeName.c_str());

        //check for softsensor resource
        if(strcmp("org.iotivity.service.resourcecontainer.BundleSoftSensorResource", str) == 0){
            jmethodID m_onUpdatedInputResource = m_env->GetMethodID(m_bundleResourceClass,
                    "onUpdatedInputResource", "(Ljava/lang/String;Ljava/util/Vector;)V");
            if(m_onUpdatedInputResource != NULL){
                m_env->MonitorEnter(m_bundleResource);
                m_env->CallVoidMethod(m_bundleResource,
                m_onUpdatedInputResource, attrName, obj);
                m_env->MonitorExit(m_bundleResource);
                if (m_env->ExceptionCheck()) {
                    m_env->ExceptionDescribe();
                }
            }
        }
        LOGD("Deleting and releasing resources - onUpdatedInputResource");
        m_env->DeleteLocalRef(attrName);
        m_env->ReleaseStringUTFChars(strObj, str);

        if (m_env->ExceptionCheck()) {
               m_env->ExceptionDescribe();
        }
        m_jvm->DetachCurrentThread();
    }

    LOGD("JniBundleResource::getAttributes().size() %d", JniBundleResource::getAttributes().size());
}

JNIEXPORT void
JNICALL Java_org_iotivity_service_resourcecontainer_BundleResource_updateNativeInstance
(JNIEnv* env, jobject obj, jobject updates)
{
    LOGD("updateNativeInstance");
    BundleResource* JniBundleResource =
            reinterpret_cast<BundleResource*>(env->GetLongField(obj, g_field_mNativeHandle));
    RCSResourceAttributes attrs = toNativeAttributes(env, updates);
    LOGD("Received attributes %d", attrs.size());
    JniBundleResource->setAttributes(attrs, true);
}
