/******************************************************************
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

#include "simulator_resource_attribute_jni.h"
#include "simulator_resource_model_jni.h"
#include "simulator_resource_model_schema_jni.h"
#include "simulator_utils_jni.h"
#include "jni_string.h"
#include "jni_map.h"

jobject SimulatorResourceAttributeToJava(JNIEnv *env,
        const SimulatorResourceAttribute &attribute)
{
    static jmethodID simulatorResAttributeCtor = env->GetMethodID(
                gSimulatorClassRefs.simulatorResourceAttributeCls, "<init>",
                "(Ljava/lang/String;Lorg/oic/simulator/AttributeValue;Lorg/oic/simulator/AttributeProperty;)V");

    jstring jAttrName = env->NewStringUTF(attribute.getName().c_str());
    jobject jAttrValue = AttributeValueToJava(env, attribute.getValue());
    jobject jAttrProperty = AttributePropertyToJava(env, attribute.getProperty());

    return env->NewObject(gSimulatorClassRefs.simulatorResourceAttributeCls,
                          simulatorResAttributeCtor, jAttrName, jAttrValue, jAttrProperty);
}

bool SimulatorResourceAttributeToCpp(JNIEnv *env, jobject jAttribute,
                                     SimulatorResourceAttribute &attribute)
{
    if (!jAttribute)
        return false;

    static jfieldID nameFID = env->GetFieldID(gSimulatorClassRefs.simulatorResourceAttributeCls,
                              "mName", "Ljava/lang/String;");
    static jfieldID valueFID = env->GetFieldID(gSimulatorClassRefs.simulatorResourceAttributeCls,
                               "mValue", "Lorg/oic/simulator/AttributeValue;");
    static jfieldID propertyFID = env->GetFieldID(gSimulatorClassRefs.simulatorResourceAttributeCls,
                                  "mProperty", "Lorg/oic/simulator/AttributeProperty;");

    jstring jAttrName = (jstring) env->GetObjectField(jAttribute, nameFID);
    jobject jAttrValue = env->GetObjectField(jAttribute, valueFID);
    jobject jAttrProperty = env->GetObjectField(jAttribute, propertyFID);

    if (!jAttrName || !jAttrValue)
        return false;

    JniString attrName(env, jAttrName);
    AttributeValueVariant value;
    AttributeValueToCpp(env, jAttrValue, value);

    attribute.setName(attrName.get());
    attribute.setValue(value);
    if (jAttrProperty)
    {
        std::shared_ptr<AttributeProperty> property = AttributePropertyToCpp(env, jAttrProperty);
        attribute.setProperty(property);
    }

    return true;
}

jobject SimulatorResourceAttributesToJava(JNIEnv *env,
        const std::map<std::string, SimulatorResourceAttribute> &attributes)
{
    JniMap jAttributesMap(env);

    // Add attribute values for Attributes table
    for (auto &attributeEntry : attributes)
    {
        jstring jAttrName = env->NewStringUTF((attributeEntry.first).c_str());
        jobject jAttribute = SimulatorResourceAttributeToJava(env, attributeEntry.second);

        jAttributesMap.put(jAttrName, jAttribute);
    }

    return jAttributesMap.get();
}