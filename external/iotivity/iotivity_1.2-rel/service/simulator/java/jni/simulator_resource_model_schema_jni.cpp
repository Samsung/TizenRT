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

#include "simulator_resource_model_schema_jni.h"
#include "simulator_utils_jni.h"
#include "jni_string.h"

static AttributeProperty::Type getType(JNIEnv *env, jobject &jProperty)
{
    static jmethodID getTypeMID = env->GetMethodID(
                                      gSimulatorClassRefs.attributePropertyCls, "getType",
                                      "()Lorg/oic/simulator/AttributeProperty$Type;");

    static jmethodID ordinalMID = env->GetMethodID(
                                      gSimulatorClassRefs.attributePropertyTypeCls, "ordinal", "()I");

    jobject jType = env->CallObjectMethod(jProperty, getTypeMID);
    int ordinal = env->CallIntMethod(jType, ordinalMID);
    return AttributeProperty::Type(ordinal);
}

class JniIntegerProperty
{
    public:
        static jobject toJava(JNIEnv *env, const std::shared_ptr<IntegerProperty> &property)
        {
            if (!property)
                return nullptr;

            // Create Java IntegerProperty.Builder object
            jobject jPropertyBuilder = nullptr;
            static jmethodID builderCtor = env->GetMethodID(
                                               gSimulatorClassRefs.integerPropertyBuilderCls, "<init>", "()V");
            jPropertyBuilder = env->NewObject(gSimulatorClassRefs.integerPropertyBuilderCls, builderCtor);

            // Set default value
            static jmethodID setDefaultValueMID = env->GetMethodID(
                    gSimulatorClassRefs.integerPropertyBuilderCls, "setDefaultValue", "(I)V");
            env->CallVoidMethod(jPropertyBuilder, setDefaultValueMID, property->getDefaultValue());

            // Set Range or enum value set
            if (property->hasRange())
            {
                int min = 0;
                int max = 0;
                property->getRange(min, max);

                static jmethodID setRangeMID = env->GetMethodID(
                                                   gSimulatorClassRefs.integerPropertyBuilderCls, "setRange", "(II)V");
                env->CallVoidMethod(jPropertyBuilder, setRangeMID, min, max);
            }
            else if (property->hasValues())
            {
                std::vector<int> values;
                property->getValues(values);
                jintArray jIntArray = env->NewIntArray(values.size());
                env->SetIntArrayRegion(jIntArray, 0, values.size(), reinterpret_cast<const jint *>(&values[0]));

                static jmethodID setValuesMID = env->GetMethodID(
                                                    gSimulatorClassRefs.integerPropertyBuilderCls, "setValues", "([I)V");
                env->CallVoidMethod(jPropertyBuilder, setValuesMID, jIntArray);
            }

            // Create Java IntegerProperty object
            static jmethodID buildMID = env->GetMethodID(
                                            gSimulatorClassRefs.integerPropertyBuilderCls, "build", "()Lorg/oic/simulator/IntegerProperty;");
            return env->CallObjectMethod(jPropertyBuilder, buildMID);
        }

        static std::shared_ptr<IntegerProperty> toCpp(JNIEnv *env, jobject &jProperty)
        {
            static jfieldID defaultValueFID = env->GetFieldID(gSimulatorClassRefs.integerPropertyCls,
                                              "mDefaultValue", "I");
            static jfieldID hasRangeFID = env->GetFieldID(gSimulatorClassRefs.integerPropertyCls,
                                          "mHasRange", "Z");

            jint defaultValue = env->GetIntField(jProperty, defaultValueFID);
            jboolean hasRange = env->GetBooleanField(jProperty, hasRangeFID);

            std::shared_ptr<IntegerProperty> integerProperty =
                IntegerProperty::build(defaultValue);

            if (hasRange)
            {
                static jfieldID minFID = env->GetFieldID(gSimulatorClassRefs.integerPropertyCls,
                                         "mMin", "I");
                static jfieldID maxFID = env->GetFieldID(gSimulatorClassRefs.integerPropertyCls,
                                         "mMax", "I");

                jint min = env->GetIntField(jProperty, minFID);
                jint max = env->GetIntField(jProperty, maxFID);
                integerProperty->setRange(min, max);
            }
            else
            {
                static jfieldID valuesFID = env->GetFieldID(gSimulatorClassRefs.integerPropertyCls,
                                            "mValues", "[I");
                jintArray jValues = (jintArray) env->GetObjectField(jProperty, valuesFID);
                if (jValues)
                {
                    std::vector<int> values;
                    jint *jIntArray = env->GetIntArrayElements(jValues, NULL);
                    size_t length = env->GetArrayLength(jValues);
                    for (size_t index = 0; index < length; index++)
                    {
                        values.push_back(jIntArray[index]);
                    }
                    integerProperty->setValues(values);

                    env->ReleaseIntArrayElements(jValues, jIntArray, 0);
                }
            }

            return integerProperty;
        }
};

class JniDoubleProperty
{
    public:
        static jobject toJava(JNIEnv *env, const std::shared_ptr<DoubleProperty> &property)
        {
            if (!property)
                return nullptr;

            // Create Java DoubleProperty.Builder object
            jobject jPropertyBuilder = nullptr;
            static jmethodID builderCtor = env->GetMethodID(
                                               gSimulatorClassRefs.doublePropertyBuilderCls, "<init>", "()V");
            jPropertyBuilder = env->NewObject(gSimulatorClassRefs.doublePropertyBuilderCls, builderCtor);

            // Set default value
            static jmethodID setDefaultValueMID = env->GetMethodID(
                    gSimulatorClassRefs.doublePropertyBuilderCls, "setDefaultValue", "(D)V");
            env->CallVoidMethod(jPropertyBuilder, setDefaultValueMID, property->getDefaultValue());

            // Set Range or enum value set
            if (property->hasRange())
            {
                double min = 0.0;
                double max = 0.0;
                property->getRange(min, max);

                static jmethodID setRangeMID = env->GetMethodID(
                                                   gSimulatorClassRefs.doublePropertyBuilderCls, "setRange", "(DD)V");
                env->CallVoidMethod(jPropertyBuilder, setRangeMID, min, max);
            }
            else if (property->hasValues())
            {
                std::vector<double> values;
                property->getValues(values);
                jdoubleArray jDoubleArray = env->NewDoubleArray(values.size());
                env->SetDoubleArrayRegion(jDoubleArray, 0, values.size(),
                                          reinterpret_cast<const jdouble *>(&values[0]));

                static jmethodID setValuesMID = env->GetMethodID(
                                                    gSimulatorClassRefs.doublePropertyBuilderCls, "setValues", "([D)V");
                env->CallVoidMethod(jPropertyBuilder, setValuesMID, jDoubleArray);
            }

            // Create Java DoubleProperty object
            static jmethodID buildMID = env->GetMethodID(
                                            gSimulatorClassRefs.doublePropertyBuilderCls, "build", "()Lorg/oic/simulator/DoubleProperty;");
            return env->CallObjectMethod(jPropertyBuilder, buildMID);
        }

        static std::shared_ptr<DoubleProperty> toCpp(JNIEnv *env, jobject &jProperty)
        {
            static jfieldID defaultValueFID = env->GetFieldID(gSimulatorClassRefs.doublePropertyCls,
                                              "mDefaultValue", "D");
            static jfieldID hasRangeFID = env->GetFieldID(gSimulatorClassRefs.doublePropertyCls,
                                          "mHasRange", "Z");

            jdouble defaultValue = env->GetDoubleField(jProperty, defaultValueFID);
            jboolean hasRange = env->GetBooleanField(jProperty, hasRangeFID);

            std::shared_ptr<DoubleProperty> doubleProperty =
                DoubleProperty::build(defaultValue);

            if (hasRange)
            {
                static jfieldID minFID = env->GetFieldID(gSimulatorClassRefs.doublePropertyCls,
                                         "mMin", "D");
                static jfieldID maxFID = env->GetFieldID(gSimulatorClassRefs.doublePropertyCls,
                                         "mMax", "D");

                jdouble min = env->GetDoubleField(jProperty, minFID);
                jdouble max = env->GetDoubleField(jProperty, maxFID);
                doubleProperty->setRange(min, max);
            }
            else
            {
                static jfieldID valuesFID = env->GetFieldID(gSimulatorClassRefs.doublePropertyCls,
                                            "mValues", "[D");
                jdoubleArray jValues = (jdoubleArray) env->GetObjectField(jProperty, valuesFID);
                if (jValues)
                {
                    std::vector<double> values;
                    jdouble *jDoubleArray = env->GetDoubleArrayElements(jValues, NULL);
                    size_t length = env->GetArrayLength(jValues);
                    for (size_t index = 0; index < length; index++)
                    {
                        values.push_back(jDoubleArray[index]);
                    }
                    doubleProperty->setValues(values);

                    env->ReleaseDoubleArrayElements(jValues, jDoubleArray, 0);
                }
            }

            return doubleProperty;
        }
};

class JniBooleanProperty
{
    public:
        static jobject toJava(JNIEnv *env, const std::shared_ptr<BooleanProperty> &property)
        {
            if (!property)
                return nullptr;

            // Create Java BooleanProperty.Builder object
            jobject jPropertyBuilder = nullptr;
            static jmethodID builderCtor = env->GetMethodID(
                                               gSimulatorClassRefs.booleanPropertyBuilderCls, "<init>", "()V");
            jPropertyBuilder = env->NewObject(gSimulatorClassRefs.booleanPropertyBuilderCls, builderCtor);

            // Set default value
            static jmethodID setDefaultValueMID = env->GetMethodID(
                    gSimulatorClassRefs.booleanPropertyBuilderCls, "setDefaultValue", "(Z)V");
            env->CallVoidMethod(jPropertyBuilder, setDefaultValueMID, property->getDefaultValue());

            // Create Java BooleanProperty object
            static jmethodID buildMID = env->GetMethodID(
                                            gSimulatorClassRefs.booleanPropertyBuilderCls, "build", "()Lorg/oic/simulator/BooleanProperty;");
            return env->CallObjectMethod(jPropertyBuilder, buildMID);
        }

        static std::shared_ptr<BooleanProperty> toCpp(JNIEnv *env, jobject &jProperty)
        {
            static jfieldID defaultValueFID = env->GetFieldID(gSimulatorClassRefs.booleanPropertyCls,
                                              "mDefaultValue", "Z");

            jboolean defaultValue = env->GetBooleanField(jProperty, defaultValueFID);
            return BooleanProperty::build(defaultValue);
        }
};

class JniStringProperty
{
    public:
        static jobject toJava(JNIEnv *env, const std::shared_ptr<StringProperty> &property)
        {
            if (!property)
                return nullptr;

            // Create Java StringProperty.Builder object
            jobject jPropertyBuilder = nullptr;
            static jmethodID builderCtor = env->GetMethodID(
                                               gSimulatorClassRefs.stringPropertyBuilderCls, "<init>", "()V");
            jPropertyBuilder = env->NewObject(gSimulatorClassRefs.stringPropertyBuilderCls, builderCtor);

            // Set default value
            static jmethodID setDefaultValueMID = env->GetMethodID(
                    gSimulatorClassRefs.stringPropertyBuilderCls, "setDefaultValue", "(Ljava/lang/String;)V");
            jstring jDefaultValue = env->NewStringUTF(property->getDefaultValue().c_str());
            env->CallVoidMethod(jPropertyBuilder, setDefaultValueMID, jDefaultValue);

            // Set Range or enum value set
            if (property->hasRange())
            {
                size_t min = 0;
                size_t max = 0;
                property->getRange(min, max);

                static jmethodID setRangeMID = env->GetMethodID(
                                                   gSimulatorClassRefs.stringPropertyBuilderCls, "setRange", "(II)V");
                env->CallVoidMethod(jPropertyBuilder, setRangeMID, static_cast<jint>(min),
                                    static_cast<jint>(max));
            }
            else if (property->hasValues())
            {
                std::vector<std::string> values;
                property->getValues(values);
                jobjectArray jStringArray = env->NewObjectArray(values.size(), gSimulatorClassRefs.stringCls,
                                            nullptr);

                for (size_t index = 0; index < values.size(); index++)
                {
                    jstring element = env->NewStringUTF(values[index].c_str());
                    env->SetObjectArrayElement(jStringArray, index, element);
                }

                static jmethodID setValuesMID = env->GetMethodID(
                                                    gSimulatorClassRefs.stringPropertyBuilderCls, "setValues", "([Ljava/lang/String;)V");
                env->CallVoidMethod(jPropertyBuilder, setValuesMID, jStringArray);
            }

            // Create Java StringProperty object
            static jmethodID buildMID = env->GetMethodID(
                                            gSimulatorClassRefs.stringPropertyBuilderCls, "build", "()Lorg/oic/simulator/StringProperty;");
            return env->CallObjectMethod(jPropertyBuilder, buildMID);
        }

        static std::shared_ptr<StringProperty> toCpp(JNIEnv *env, jobject &jProperty)
        {
            static jfieldID defaultValueFID = env->GetFieldID(gSimulatorClassRefs.stringPropertyCls,
                                              "mDefaultValue", "Ljava/lang/String;");
            static jfieldID hasRangeFID = env->GetFieldID(gSimulatorClassRefs.stringPropertyCls,
                                          "mHasRange", "Z");

            jstring defaultValue = (jstring) env->GetObjectField(jProperty, defaultValueFID);
            jboolean hasRange = env->GetBooleanField(jProperty, hasRangeFID);

            std::shared_ptr<StringProperty> stringProperty =
                StringProperty::build(JniString(env, defaultValue).get());

            if (hasRange)
            {
                static jfieldID minFID = env->GetFieldID(gSimulatorClassRefs.stringPropertyCls,
                                         "mMin", "I");
                static jfieldID maxFID = env->GetFieldID(gSimulatorClassRefs.stringPropertyCls,
                                         "mMax", "I");

                jint min = env->GetIntField(jProperty, minFID);
                jint max = env->GetIntField(jProperty, maxFID);
                if (min >= 0 && max >= 0)
                    stringProperty->setRange(static_cast<size_t>(min), static_cast<size_t>(max));
            }
            else
            {
                static jfieldID valuesFID = env->GetFieldID(gSimulatorClassRefs.stringPropertyCls,
                                            "mValues", "[Ljava/lang/String;");
                jobjectArray jValues = (jobjectArray) env->GetObjectField(jProperty, valuesFID);
                if (jValues)
                {
                    std::vector<std::string> values;
                    size_t length = env->GetArrayLength(jValues);
                    for (size_t index = 0; index < length; index++)
                    {
                        jstring jValue = (jstring) env->GetObjectArrayElement(jValues, index);
                        values.push_back(JniString(env, jValue).get());
                    }
                    stringProperty->setValues(values);
                }
            }

            return stringProperty;
        }
};

class JniArrayProperty
{
    public:
        static jobject toJava(JNIEnv *env, const std::shared_ptr<ArrayProperty> &property)
        {
            if (!property)
                return nullptr;

            // Create Java ArrayProperty.Builder object
            jobject jPropertyBuilder = nullptr;
            static jmethodID builderCtor = env->GetMethodID(
                                               gSimulatorClassRefs.arrayPropertyBuilderCls, "<init>", "()V");
            jPropertyBuilder = env->NewObject(gSimulatorClassRefs.arrayPropertyBuilderCls, builderCtor);

            // Set variable size propety
            if (property->isVariable())
            {
                static jmethodID setVariableMID = env->GetMethodID(
                                                      gSimulatorClassRefs.arrayPropertyBuilderCls, "setVariableSize", "(Z)V");
                env->CallVoidMethod(jPropertyBuilder, setVariableMID, property->isVariable());
            }

            // Set unique propety
            if (property->isUnique())
            {
                static jmethodID setUniqueMID = env->GetMethodID(
                                                    gSimulatorClassRefs.arrayPropertyBuilderCls, "setUnique", "(Z)V");
                env->CallVoidMethod(jPropertyBuilder, setUniqueMID, property->isUnique());
            }

            // Set range property
            if (property->hasRange())
            {
                size_t min = property->getMinItems();
                size_t max = property->getMaxItems();

                static jmethodID setRangeMID = env->GetMethodID(
                                                   gSimulatorClassRefs.arrayPropertyBuilderCls, "setRange", "(II)V");
                env->CallVoidMethod(jPropertyBuilder, setRangeMID, static_cast<jint>(min),
                                    static_cast<jint>(max));
            }

            // Set element property
            jobject jElementProperty = AttributePropertyToJava(env, property->getElementProperty());
            static jmethodID setElementPropertyMID = env->GetMethodID(
                        gSimulatorClassRefs.arrayPropertyBuilderCls, "setElementProperty",
                        "(Lorg/oic/simulator/AttributeProperty;)V");
            env->CallVoidMethod(jPropertyBuilder, setElementPropertyMID, jElementProperty);

            // Create Java ArrayProperty object
            static jmethodID buildMID = env->GetMethodID(
                                            gSimulatorClassRefs.arrayPropertyBuilderCls, "build", "()Lorg/oic/simulator/ArrayProperty;");
            return env->CallObjectMethod(jPropertyBuilder, buildMID);
        }

        static std::shared_ptr<ArrayProperty> toCpp(JNIEnv *env, jobject &jProperty)
        {
            static jfieldID hasRangeFID = env->GetFieldID(gSimulatorClassRefs.arrayPropertyCls,
                                          "mHasRange", "Z");
            static jfieldID variableFID = env->GetFieldID(gSimulatorClassRefs.arrayPropertyCls,
                                          "mIsVariableSize", "Z");
            static jfieldID uniqueFID = env->GetFieldID(gSimulatorClassRefs.arrayPropertyCls,
                                        "mIsUnique", "Z");
            static jfieldID elementPropertyFID = env->GetFieldID(gSimulatorClassRefs.arrayPropertyCls,
                                                 "mElementProperty", "Lorg/oic/simulator/AttributeProperty;");

            jboolean hasRange = env->GetBooleanField(jProperty, hasRangeFID);
            jboolean isVariable = env->GetBooleanField(jProperty, variableFID);
            jboolean isUnique = env->GetBooleanField(jProperty, uniqueFID);
            jobject jElementProperty = env->GetObjectField(jProperty, elementPropertyFID);

            std::shared_ptr<ArrayProperty> arrayProperty = ArrayProperty::build();

            arrayProperty->setVariable(isVariable);
            arrayProperty->setUnique(isUnique);

            if (hasRange)
            {
                static jfieldID minFID = env->GetFieldID(gSimulatorClassRefs.arrayPropertyCls,
                                         "mMin", "I");
                static jfieldID maxFID = env->GetFieldID(gSimulatorClassRefs.arrayPropertyCls,
                                         "mMax", "I");

                jint min = env->GetIntField(jProperty, minFID);
                jint max = env->GetIntField(jProperty, maxFID);
                if (min >= 0 && max >= 0)
                    arrayProperty->setRange(static_cast<size_t>(min), static_cast<size_t>(max));
            }

            if (jElementProperty)
            {
                arrayProperty->setElementProperty(AttributePropertyToCpp(env, jElementProperty));
            }

            return arrayProperty;
        }
};

class JniModelProperty
{
    public:
        static jobject toJava(JNIEnv *env, const std::shared_ptr<ModelProperty> &property)
        {
            if (!property)
                return nullptr;

            // Create Java ModelProperty object
            jobject jModelProperty = nullptr;
            static jmethodID modelPropertyCtor = env->GetMethodID(
                    gSimulatorClassRefs.modelPropertyCls, "<init>", "()V");
            jModelProperty = env->NewObject(gSimulatorClassRefs.modelPropertyCls, modelPropertyCtor);

            // Set child propeties
            for (auto &propertyEntry : property->getChildProperties())
            {
                jstring jAttrName = env->NewStringUTF(propertyEntry.first.c_str());
                jobject jProperty = AttributePropertyToJava(env, propertyEntry.second);
                jboolean required = property->isRequired(propertyEntry.first);

                static jmethodID addMID = env->GetMethodID(
                                              gSimulatorClassRefs.modelPropertyCls, "add",
                                              "(Ljava/lang/String;Lorg/oic/simulator/AttributeProperty;Z)Z");
                env->CallBooleanMethod(jModelProperty, addMID, jAttrName, jProperty, required);
            }

            return jModelProperty;
        }

        static std::shared_ptr<ModelProperty> toCpp(JNIEnv *env, jobject &jProperty)
        {
            static jfieldID childPropertiesFID = env->GetFieldID(gSimulatorClassRefs.modelPropertyCls,
                                                 "mChildProperties", "Ljava/util/Map;");
            static jmethodID isRequiredMID = env->GetMethodID(gSimulatorClassRefs.modelPropertyCls,
                                             "isRequired", "()Z");
            static jmethodID entrySetMID = env->GetMethodID(gSimulatorClassRefs.mapCls,
                                           "entrySet", "()Ljava/util/Set;");
            static jmethodID iteratorMID = env->GetMethodID(gSimulatorClassRefs.setCls,
                                           "iterator", "()Ljava/util/Iterator;");
            static jmethodID hasNextMID = env->GetMethodID(gSimulatorClassRefs.iteratorCls,
                                          "hasNext", "()Z");
            static jmethodID nextMID = env->GetMethodID(gSimulatorClassRefs.iteratorCls,
                                       "next", "()Ljava/lang/Object;");
            static jmethodID getKeyMID = env->GetMethodID(gSimulatorClassRefs.mapEntryCls,
                                         "getKey", "()Ljava/lang/Object;");
            static jmethodID getValueMID = env->GetMethodID(gSimulatorClassRefs.mapEntryCls,
                                           "getValue", "()Ljava/lang/Object;");

            std::shared_ptr<ModelProperty> modelProperty = ModelProperty::build();
            jobject jChildProperties = env->GetObjectField(jProperty, childPropertiesFID);
            if (jChildProperties)
            {
                jobject entrySet = env->CallObjectMethod(jChildProperties, entrySetMID);
                jobject iterator = env->CallObjectMethod(entrySet, iteratorMID);
                if (entrySet && iterator)
                {
                    while (env->CallBooleanMethod(iterator, hasNextMID))
                    {
                        jobject entry = env->CallObjectMethod(iterator, nextMID);
                        jstring key = (jstring) env->CallObjectMethod(entry, getKeyMID);
                        jobject value = env->CallObjectMethod(entry, getValueMID);
                        jboolean isRequired = env->CallBooleanMethod(jProperty, isRequiredMID, key);

                        modelProperty->add(JniString(env, key).get(),
                                           AttributePropertyToCpp(env, value), isRequired);

                        env->DeleteLocalRef(entry);
                        env->DeleteLocalRef(key);
                        env->DeleteLocalRef(value);
                    }
                }
            }

            return modelProperty;
        }
};

jobject AttributePropertyToJava(JNIEnv *env,
                                const std::shared_ptr<AttributeProperty> &property)
{
    if (!property)
        return nullptr;

    if (property->isInteger())
    {
        return JniIntegerProperty::toJava(env, property->asInteger());
    }
    else if (property->isDouble())
    {
        return JniDoubleProperty::toJava(env, property->asDouble());
    }
    else if (property->isBoolean())
    {
        return JniBooleanProperty::toJava(env, property->asBoolean());
    }
    else if (property->isString())
    {
        return JniStringProperty::toJava(env, property->asString());
    }
    else if (property->isArray())
    {
        return JniArrayProperty::toJava(env, property->asArray());
    }
    else if (property->isModel())
    {
        return JniModelProperty::toJava(env, property->asModel());
    }

    return nullptr; // Control should never reach here
}

std::shared_ptr<AttributeProperty> AttributePropertyToCpp(JNIEnv *env,
        jobject &jProperty)
{
    if (!jProperty)
        return nullptr;

    switch (getType(env, jProperty))
    {
        case AttributeProperty::Type::INTEGER:
            return JniIntegerProperty::toCpp(env, jProperty);

        case AttributeProperty::Type::DOUBLE:
            return JniDoubleProperty::toCpp(env, jProperty);

        case AttributeProperty::Type::BOOLEAN:
            return JniBooleanProperty::toCpp(env, jProperty);

        case AttributeProperty::Type::STRING:
            return JniStringProperty::toCpp(env, jProperty);

        case AttributeProperty::Type::ARRAY:
            return JniArrayProperty::toCpp(env, jProperty);

        case AttributeProperty::Type::MODEL:
            return JniModelProperty::toCpp(env, jProperty);
    }

    return nullptr; // Control should never reach here
}

