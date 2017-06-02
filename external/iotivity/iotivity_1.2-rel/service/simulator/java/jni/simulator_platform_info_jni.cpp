/******************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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

#include "simulator_platform_info_jni.h"
#include "simulator_utils_jni.h"
#include "jni_string.h"

extern SimulatorClassRefs gSimulatorClassRefs;

jobject JniPlatformInfo::toJava(PlatformInfo &platformInfo)
{
    if (!m_env)
        return nullptr;

    static jmethodID platformInfoCtor = m_env->GetMethodID(gSimulatorClassRefs.platformInfoCls,
                                        "<init>", "()V");
    jobject jPlatformInfo = (jobject) m_env->NewObject(gSimulatorClassRefs.platformInfoCls,
                            platformInfoCtor);
    setFieldValue(jPlatformInfo, "mPlatformId", platformInfo.getPlatformID());
    setFieldValue(jPlatformInfo, "mManufacturerName", platformInfo.getManufacturerName());
    setFieldValue(jPlatformInfo, "mManufacturerUrl", platformInfo.getManufacturerUrl());
    setFieldValue(jPlatformInfo, "mModelNumber", platformInfo.getModelNumber());
    setFieldValue(jPlatformInfo, "mDateOfManufacture", platformInfo.getDateOfManfacture());
    setFieldValue(jPlatformInfo, "mPlatformVersion", platformInfo.getPlatformVersion());
    setFieldValue(jPlatformInfo, "mOperationSystemVersion", platformInfo.getOSVersion());
    setFieldValue(jPlatformInfo, "mHardwareVersion", platformInfo.getHardwareVersion());
    setFieldValue(jPlatformInfo, "mFirmwareVersion", platformInfo.getFirmwareVersion());
    setFieldValue(jPlatformInfo, "mSupportUrl", platformInfo.getSupportUrl());
    setFieldValue(jPlatformInfo, "mSystemTime", platformInfo.getSystemTime());

    return jPlatformInfo;
}

PlatformInfo JniPlatformInfo::toCpp(jobject jPlatformInfo)
{
    PlatformInfo platformInfo;
    if (!m_env || !jPlatformInfo)
        return platformInfo;

    platformInfo.setPlatformID(getFieldValue(jPlatformInfo, "mPlatformId"));
    platformInfo.setManufacturerName(getFieldValue(jPlatformInfo, "mManufacturerName"));
    platformInfo.setManufacturerUrl(getFieldValue(jPlatformInfo, "mManufacturerUrl"));
    platformInfo.setModelNumber(getFieldValue(jPlatformInfo, "mModelNumber"));
    platformInfo.setDateOfManfacture(getFieldValue(jPlatformInfo, "mDateOfManufacture"));
    platformInfo.setPlatformVersion(getFieldValue(jPlatformInfo, "mPlatformVersion"));
    platformInfo.setOSVersion(getFieldValue(jPlatformInfo, "mOperationSystemVersion"));
    platformInfo.setHardwareVersion(getFieldValue(jPlatformInfo, "mHardwareVersion"));
    platformInfo.setFirmwareVersion(getFieldValue(jPlatformInfo, "mFirmwareVersion"));
    platformInfo.setSupportUrl(getFieldValue(jPlatformInfo, "mSupportUrl"));
    platformInfo.setSystemTime(getFieldValue(jPlatformInfo, "mSystemTime"));

    return std::move(platformInfo);
}

void JniPlatformInfo::setFieldValue(jobject jPlatformInfo, const std::string &fieldName,
                                    const std::string &value)
{
    jfieldID fieldID = m_env->GetFieldID(m_env->GetObjectClass(jPlatformInfo), fieldName.c_str(),
                                         "Ljava/lang/String;");
    jstring valueStr = m_env->NewStringUTF(value.c_str());
    m_env->SetObjectField(jPlatformInfo, fieldID, valueStr);
}

std::string JniPlatformInfo::getFieldValue(jobject jPlatformInfo, const std::string &fieldName)
{
    jfieldID fieldID = m_env->GetFieldID(m_env->GetObjectClass(jPlatformInfo), fieldName.c_str(),
                                         "Ljava/lang/String;");
    jstring jvalue = (jstring) m_env->GetObjectField(jPlatformInfo, fieldID);
    JniString value(m_env, jvalue);
    return value.get();
}
