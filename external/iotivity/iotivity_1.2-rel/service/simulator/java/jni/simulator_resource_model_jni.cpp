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

#include "simulator_resource_model_jni.h"
#include "simulator_utils_jni.h"
#include "jni_string.h"
#include "jni_map.h"

class JniTypeInfo
{
    public:
        JniTypeInfo(JNIEnv *env) : m_env(env) {}

        SimulatorResourceModel::TypeInfo toCpp(jobject jAttributeValue)
        {
            static jmethodID typeInfoMID = m_env->GetMethodID(gSimulatorClassRefs.attributeValueCls,
                                           "typeInfo", "()Lorg/oic/simulator/AttributeValue$TypeInfo;");
            static jfieldID typeFID = m_env->GetFieldID(gSimulatorClassRefs.attributeTypeInfoCls,
                                      "mType", "Lorg/oic/simulator/AttributeValue$ValueType;");
            static jfieldID baseTypeFID = m_env->GetFieldID(gSimulatorClassRefs.attributeTypeInfoCls,
                                          "mBaseType", "Lorg/oic/simulator/AttributeValue$ValueType;");
            static jfieldID depthFID = m_env->GetFieldID(gSimulatorClassRefs.attributeTypeInfoCls,
                                       "mDepth", "I");

            jobject jTypeInfo = m_env->CallObjectMethod(jAttributeValue, typeInfoMID);
            jobject jType = m_env->GetObjectField(jTypeInfo, typeFID);
            jobject jBaseType = m_env->GetObjectField(jTypeInfo, baseTypeFID);
            jint jDepth = m_env->GetIntField(jTypeInfo, depthFID);

            return SimulatorResourceModel::TypeInfo(getValueType(jType),
                                                    getValueType(jBaseType), jDepth);
        }

    private:
        AttributeValueType getValueType(jobject jValueType)
        {
            static jmethodID ordinalMID = m_env->GetMethodID(
                                              gSimulatorClassRefs.attributeValueTypeCls, "ordinal", "()I");

            int ordinal = m_env->CallIntMethod(jValueType, ordinalMID);
            return AttributeValueType(ordinal);
        }

        JNIEnv *m_env;
};

class ValueConverterJava : public boost::static_visitor<jobject>
{
    public:
        ValueConverterJava(JNIEnv *env) : m_env(env) {}

        jobject operator ()(const int &value)
        {
            static jmethodID integerCtor =
                m_env->GetMethodID(gSimulatorClassRefs.integerCls, "<init>", "(I)V");
            return m_env->NewObject(gSimulatorClassRefs.integerCls,
                                    integerCtor, value);
        }

        jobject operator ()(const double &value)
        {
            static jmethodID doubleCtor =
                m_env->GetMethodID(gSimulatorClassRefs.doubleCls, "<init>", "(D)V");
            return m_env->NewObject(gSimulatorClassRefs.doubleCls,
                                    doubleCtor, value);
        }

        jobject operator ()(const bool &value)
        {
            static jmethodID booleanCtor =
                m_env->GetMethodID(gSimulatorClassRefs.booleanCls, "<init>", "(Z)V");
            return m_env->NewObject(gSimulatorClassRefs.booleanCls,
                                    booleanCtor, value);
        }

        jobject operator ()(const std::string &value)
        {
            jstring stringValue = m_env->NewStringUTF(value.c_str());
            return static_cast<jobject>(stringValue);
        }

        jobject operator ()(const SimulatorResourceModel &value)
        {
            return SimulatorResourceModelToJava(m_env, const_cast<SimulatorResourceModel &>(value));
        }

        template <typename T>
        jobject operator ()(const std::vector<T> &values)
        {
            jobjectArray jArray = m_env->NewObjectArray(values.size(), getClass(values), nullptr);
            if (!jArray)
                return nullptr;

            for (size_t index = 0; index < values.size(); index++)
            {
                jobject element = operator()(values[index]);
                m_env->SetObjectArrayElement(jArray, index, element);
            }

            return jArray;
        }

        template <typename T>
        jobject operator ()(const std::vector<std::vector<T>> &values)
        {
            jobjectArray jArray = m_env->NewObjectArray(values.size(), getClass(values), nullptr);
            if (!jArray)
                return nullptr;

            for (size_t index = 0; index < values.size(); index++)
            {
                jobject element = operator()(values[index]);
                m_env->SetObjectArrayElement(jArray, index, element);
            }

            return jArray;
        }

        template <typename T>
        jobject operator ()(const std::vector<std::vector<std::vector<T>>> &values)
        {
            jobjectArray jArray = m_env->NewObjectArray(values.size(), getClass(values), nullptr);
            if (!jArray)
                return nullptr;

            for (size_t index = 0; index < values.size(); index++)
            {
                jobject element = operator()(values[index]);
                m_env->SetObjectArrayElement(jArray, index, element);
            }

            return jArray;
        }

    private:
        jclass getClass(const std::vector<int> &)
        {
            return gSimulatorClassRefs.integerCls;
        }

        jclass getClass(const std::vector<std::vector<int>> &)
        {
            return gSimulatorClassRefs.integer1DArrayCls;
        }

        jclass getClass(const std::vector<std::vector<std::vector<int>>> &)
        {
            return gSimulatorClassRefs.integer2DArrayCls;
        }

        jclass getClass(const std::vector<double> &)
        {
            return gSimulatorClassRefs.doubleCls;
        }

        jclass getClass(const std::vector<std::vector<double>> &)
        {
            return gSimulatorClassRefs.double1DArrayCls;
        }

        jclass getClass(const std::vector<std::vector<std::vector<double>>> &)
        {
            return gSimulatorClassRefs.double2DArrayCls;
        }

        jclass getClass(const std::vector<bool> &)
        {
            return gSimulatorClassRefs.booleanCls;
        }

        jclass getClass(const std::vector<std::vector<bool>> &)
        {
            return gSimulatorClassRefs.boolean1DArrayCls;
        }

        jclass getClass(const std::vector<std::vector<std::vector<bool>>> &)
        {
            return gSimulatorClassRefs.boolean2DArrayCls;
        }

        jclass getClass(const std::vector<std::string> &)
        {
            return gSimulatorClassRefs.stringCls;
        }

        jclass getClass(const std::vector<std::vector<std::string>> &)
        {
            return gSimulatorClassRefs.string1DArrayCls;
        }

        jclass getClass(const std::vector<std::vector<std::vector<std::string>>> &)
        {
            return gSimulatorClassRefs.string2DArrayCls;
        }

        jclass getClass(const std::vector<SimulatorResourceModel> &)
        {
            return gSimulatorClassRefs.simulatorResourceModelCls;
        }

        jclass getClass(const std::vector<std::vector<SimulatorResourceModel>> &)
        {
            return gSimulatorClassRefs.simulatorResModel1DArrayCls;
        }

        jclass getClass(const std::vector<std::vector<std::vector<SimulatorResourceModel>>> &)
        {
            return gSimulatorClassRefs.simulatorResModel2DArrayCls;
        }

        JNIEnv *m_env;
};

class ValueConverterCpp
{
    public:
        ValueConverterCpp(JNIEnv *env, SimulatorResourceModel::TypeInfo &typeInfo,
                          jobject &value) : m_env(env), m_typeInfo(typeInfo), m_value(value) {}

        void convert()
        {
            switch (m_typeInfo.baseType())
            {
                case AttributeValueType::INTEGER:
                    return handleByDepth<int>();
                case AttributeValueType::DOUBLE:
                    return handleByDepth<double>();
                case AttributeValueType::BOOLEAN:
                    return handleByDepth<bool>();
                case AttributeValueType::STRING:
                    return handleByDepth<std::string>();
                case AttributeValueType::RESOURCE_MODEL:
                    return handleByDepth<SimulatorResourceModel>();
                case AttributeValueType::VECTOR:
                case AttributeValueType::UNKNOWN:
                    break;
            }
        }

        AttributeValueVariant get()
        {
            return std::move(m_result);
        }

    private:
        template <typename T>
        void handleByDepth()
        {
            if (0 == m_typeInfo.depth())
            {
                T value;
                getValue(m_value, value);
                m_result = value;
            }
            else if (1 == m_typeInfo.depth())
            {
                std::vector<T> value;
                getValue(m_value, value);
                m_result = value;
            }
            else if (2 == m_typeInfo.depth())
            {
                std::vector<std::vector<T>> value;
                getValue(m_value, value);
                m_result = value;
            }
            else if (3 == m_typeInfo.depth())
            {
                std::vector<std::vector<std::vector<T>>> value;
                getValue(m_value, value);
                m_result = value;
            }
        }

        void getValue(jobject &jValue, int &value)
        {
            static jmethodID intValueMID = m_env->GetMethodID(
                                               gSimulatorClassRefs.integerCls, "intValue", "()I");

            jint temp = m_env->CallIntMethod(jValue, intValueMID);
            value = temp;
        }

        void getValue(jobject &jValue, std::vector<int> &value)
        {
            jobjectArray array = (jobjectArray) jValue;
            size_t length = m_env->GetArrayLength(array);
            for (size_t i = 0; i < length; i++)
            {
                jobject jElement = m_env->GetObjectArrayElement(array, i);

                int element;
                getValue(jElement, element);
                value.push_back(element);
            }
        }

        void getValue(jobject &jValue, std::vector<std::vector<int>> &value)
        {
            jobjectArray array = (jobjectArray) jValue;
            size_t length = m_env->GetArrayLength(array);
            for (size_t i = 0; i < length; i++)
            {
                jobject jElement = m_env->GetObjectArrayElement(array, i);

                std::vector<int> element;
                getValue(jElement, element);
                value.push_back(element);
            }
        }

        void getValue(jobject &jValue, std::vector<std::vector<std::vector<int>>> &value)
        {
            jobjectArray array = (jobjectArray) jValue;
            size_t length = m_env->GetArrayLength(array);
            for (size_t i = 0; i < length; i++)
            {
                jobject jElement = m_env->GetObjectArrayElement(array, i);

                std::vector<std::vector<int>> element;
                getValue(jElement, element);
                value.push_back(element);
            }
        }

        void getValue(jobject &jValue, double &value)
        {
            static jmethodID doubleValueMID = m_env->GetMethodID(
                                                  gSimulatorClassRefs.doubleCls, "doubleValue", "()D");

            value = m_env->CallDoubleMethod(jValue, doubleValueMID);
        }

        void getValue(jobject &jValue, std::vector<double> &value)
        {
            jobjectArray array = (jobjectArray) jValue;
            size_t length = m_env->GetArrayLength(array);
            for (size_t i = 0; i < length; i++)
            {
                jobject jElement = m_env->GetObjectArrayElement(array, i);

                double element;
                getValue(jElement, element);
                value.push_back(element);
            }
        }

        void getValue(jobject &jValue, std::vector<std::vector<double>> &value)
        {
            jobjectArray array = (jobjectArray) jValue;
            size_t length = m_env->GetArrayLength(array);
            for (size_t i = 0; i < length; i++)
            {
                jobject jElement = m_env->GetObjectArrayElement(array, i);

                std::vector<double> element;
                getValue(jElement, element);
                value.push_back(element);
            }
        }

        void getValue(jobject &jValue, std::vector<std::vector<std::vector<double>>> &value)
        {
            jobjectArray array = (jobjectArray) jValue;
            size_t length = m_env->GetArrayLength(array);
            for (size_t i = 0; i < length; i++)
            {
                jobject jElement = m_env->GetObjectArrayElement(array, i);

                std::vector<std::vector<double>> element;
                getValue(jElement, element);
                value.push_back(element);
            }
        }

        void getValue(jobject &jValue, bool &value)
        {
            static jmethodID boolValueMID = m_env->GetMethodID(
                                                gSimulatorClassRefs.booleanCls, "booleanValue", "()Z");

            value = m_env->CallBooleanMethod(jValue, boolValueMID);
        }

        void getValue(jobject &jValue, std::vector<bool> &value)
        {
            jobjectArray array = (jobjectArray) jValue;
            size_t length = m_env->GetArrayLength(array);
            for (size_t i = 0; i < length; i++)
            {
                jobject jElement = m_env->GetObjectArrayElement(array, i);

                bool element;
                getValue(jElement, element);
                value.push_back(element);
            }
        }

        void getValue(jobject &jValue, std::vector<std::vector<bool>> &value)
        {
            jobjectArray array = (jobjectArray) jValue;
            size_t length = m_env->GetArrayLength(array);
            for (size_t i = 0; i < length; i++)
            {
                jobject jElement = m_env->GetObjectArrayElement(array, i);

                std::vector<bool> element;
                getValue(jElement, element);
                value.push_back(element);
            }
        }

        void getValue(jobject &jValue, std::vector<std::vector<std::vector<bool>>> &value)
        {
            jobjectArray array = (jobjectArray) jValue;
            size_t length = m_env->GetArrayLength(array);
            for (size_t i = 0; i < length; i++)
            {
                jobject jElement = m_env->GetObjectArrayElement(array, i);

                std::vector<std::vector<bool>> element;
                getValue(jElement, element);
                value.push_back(element);
            }
        }

        void getValue(jobject &jValue, std::string &value)
        {
            jstring stringValue = (jstring) jValue;
            JniString jniValue(m_env, stringValue);
            value = jniValue.get();
        }

        void getValue(jobject jValue, std::vector<std::string> &value)
        {
            jobjectArray array = (jobjectArray) jValue;
            size_t length = m_env->GetArrayLength(array);
            for (size_t i = 0; i < length; i++)
            {
                jobject jElement = m_env->GetObjectArrayElement(array, i);

                std::string element;
                getValue(jElement, element);
                value.push_back(element);
            }
        }

        void getValue(jobject &jValue, std::vector<std::vector<std::string>> &value)
        {
            jobjectArray array = (jobjectArray) jValue;
            size_t length = m_env->GetArrayLength(array);
            for (size_t i = 0; i < length; i++)
            {
                jobject jElement = m_env->GetObjectArrayElement(array, i);

                std::vector<std::string> element;
                getValue(jElement, element);
                value.push_back(element);
            }
        }

        void getValue(jobject &jValue, std::vector<std::vector<std::vector<std::string>>> &value)
        {
            jobjectArray array = (jobjectArray) jValue;
            size_t length = m_env->GetArrayLength(array);
            for (size_t i = 0; i < length; i++)
            {
                jobject jElement = m_env->GetObjectArrayElement(array, i);

                std::vector<std::vector<std::string>> element;
                getValue(jElement, element);
                value.push_back(element);
            }
        }

        void getValue(jobject &jValue, SimulatorResourceModel &value)
        {
            SimulatorResourceModelToCpp(m_env, jValue, value);
        }

        void getValue(jobject &jValue, std::vector<SimulatorResourceModel> &value)
        {
            jobjectArray array = (jobjectArray) jValue;
            size_t length = m_env->GetArrayLength(array);
            std::vector<SimulatorResourceModel> result(length);
            for (size_t i = 0; i < length; i++)
            {
                jobject jElement = m_env->GetObjectArrayElement(array, i);

                SimulatorResourceModel element;
                getValue(jElement, element);
                result[i] = element;
            }

            value = result;
        }

        void getValue(jobject &jValue, std::vector<std::vector<SimulatorResourceModel>> &value)
        {
            jobjectArray array = (jobjectArray) jValue;
            size_t length = m_env->GetArrayLength(array);
            std::vector<std::vector<SimulatorResourceModel>> result(length);
            for (size_t i = 0; i < length; i++)
            {
                jobject jElement = m_env->GetObjectArrayElement(array, i);

                std::vector<SimulatorResourceModel> childArray;
                getValue(jElement, childArray);
                value[i] = childArray;
            }

            value = result;
        }

        void getValue(jobject &jValue, std::vector<std::vector<std::vector<SimulatorResourceModel>>> &value)
        {
            jobjectArray array = (jobjectArray) jValue;
            size_t length = m_env->GetArrayLength(array);
            std::vector<std::vector<std::vector<SimulatorResourceModel>>> result(length);
            for (size_t i = 0; i < length; i++)
            {
                jobject jElement = m_env->GetObjectArrayElement(array, i);

                std::vector<std::vector<SimulatorResourceModel>> childArray;
                getValue(jElement, childArray);
                value[i] = childArray;
            }

            value = result;
        }

        JNIEnv *m_env;
        SimulatorResourceModel::TypeInfo &m_typeInfo;
        jobject &m_value;
        AttributeValueVariant m_result;
};

class JniAttributeValue
{
    public:
        static jobject toJava(JNIEnv *env, const SimulatorResourceAttribute &attribute)
        {
            return toJava(env, attribute.getValue());
        }

        static jobject toJava(JNIEnv *env, const AttributeValueVariant &value)
        {
            ValueConverterJava converter(env);
            jobject jValue =  boost::apply_visitor(converter, value);

            static jmethodID attrValueCtor = env->GetMethodID(
                                                 gSimulatorClassRefs.attributeValueCls, "<init>", "(Ljava/lang/Object;)V");

            return env->NewObject(gSimulatorClassRefs.attributeValueCls, attrValueCtor, jValue);
        }

        static AttributeValueVariant toCpp(JNIEnv *env, jobject &jAttributeValue)
        {
            static jmethodID getMID = env->GetMethodID(gSimulatorClassRefs.attributeValueCls,
                                      "get", "()Ljava/lang/Object;");

            SimulatorResourceModel::TypeInfo typeInfo = JniTypeInfo(env).toCpp(jAttributeValue);
            jobject jValue = env->CallObjectMethod(jAttributeValue, getMID);

            ValueConverterCpp converter(env, typeInfo, jValue);
            converter.convert();
            return converter.get();
        }
};

jobject SimulatorResourceModelToJava(JNIEnv *env, const SimulatorResourceModel &resModel)
{
    JniMap jAttributesMap(env);

    // Add attribute values for HashMap
    for (auto &attributeEntry : resModel.getAttributeValues())
    {
        jstring jAttrName = env->NewStringUTF((attributeEntry.first).c_str());
        jobject jAttributeValue = JniAttributeValue::toJava(env, attributeEntry.second);

        jAttributesMap.put(jAttrName, jAttributeValue);
    }

    // Create Java SimulatorResourceModel object
    static jmethodID simulatorResourceModelCtor = env->GetMethodID(
                gSimulatorClassRefs.simulatorResourceModelCls, "<init>", "(Ljava/util/Map;)V");

    return env->NewObject(gSimulatorClassRefs.simulatorResourceModelCls,
                          simulatorResourceModelCtor, jAttributesMap.get());
}

bool SimulatorResourceModelToCpp(JNIEnv *env, jobject jResModel, SimulatorResourceModel &resModel)
{
    if (!jResModel)
        return false;

    static jfieldID valuesFID = env->GetFieldID(gSimulatorClassRefs.simulatorResourceModelCls,
                                "mValues", "Ljava/util/Map;");
    static jmethodID entrySetMID = env->GetMethodID(gSimulatorClassRefs.mapCls, "entrySet",
                                   "()Ljava/util/Set;");
    static jmethodID iteratorMID = env->GetMethodID(gSimulatorClassRefs.setCls, "iterator",
                                   "()Ljava/util/Iterator;");
    static jmethodID hasNextMID = env->GetMethodID(gSimulatorClassRefs.iteratorCls, "hasNext",
                                  "()Z");
    static jmethodID nextMID = env->GetMethodID(gSimulatorClassRefs.iteratorCls, "next",
                               "()Ljava/lang/Object;");
    static jmethodID getKeyMID = env->GetMethodID(gSimulatorClassRefs.mapEntryCls, "getKey",
                                 "()Ljava/lang/Object;");
    static jmethodID getValueMID = env->GetMethodID(gSimulatorClassRefs.mapEntryCls, "getValue",
                                   "()Ljava/lang/Object;");

    jobject jValues = env->GetObjectField(jResModel, valuesFID);
    if (jValues)
    {
        jobject entrySet = env->CallObjectMethod(jValues, entrySetMID);
        jobject iterator = env->CallObjectMethod(entrySet, iteratorMID);
        if (entrySet && iterator)
        {
            while (env->CallBooleanMethod(iterator, hasNextMID))
            {
                jobject entry = env->CallObjectMethod(iterator, nextMID);
                jstring key = (jstring) env->CallObjectMethod(entry, getKeyMID);
                jobject value = env->CallObjectMethod(entry, getValueMID);
                resModel.add(JniString(env, key).get(), JniAttributeValue::toCpp(env, value));

                env->DeleteLocalRef(entry);
                env->DeleteLocalRef(key);
                env->DeleteLocalRef(value);
            }
        }
    }

    return true;
}

jobject AttributeValueToJava(JNIEnv *env,
                             const AttributeValueVariant &value)
{
    return JniAttributeValue::toJava(env, value);
}

bool AttributeValueToCpp(JNIEnv *env, jobject jAttributeValue,
                         AttributeValueVariant &jValue)
{
    if (!jAttributeValue)
        return false;

    jValue = JniAttributeValue::toCpp(env, jAttributeValue);
    return true;
}

