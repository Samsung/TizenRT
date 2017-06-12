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

#include "JniRcsResourceAttributes.h"

#include "JNIEnvWrapper.h"
#include "JavaClasses.h"
#include "JavaLocalRef.h"
#include "JniRcsValue.h"
#include "Log.h"
#include "Verify.h"

using namespace OIC::Service;

#define LOG_TAG "JNI-RCSResourceAttributes"

namespace
{
    jclass g_cls_RCSResourceAttributes;

    jmethodID g_ctor_RCSResourceAttributes;

    jfieldID g_field_mCache;
}

void initRCSResourceAttributes(JNIEnvWrapper* env)
{
    g_cls_RCSResourceAttributes = env->FindClassAsGlobalRef(CLS_NAME_RESOURCEATTRIBUTES);
    g_ctor_RCSResourceAttributes = env->GetConstructorID(g_cls_RCSResourceAttributes, "()V");

    g_field_mCache = env->GetFieldID(g_cls_RCSResourceAttributes, "mCache", AS_SIG(CLS_NAME_MAP));
}

void clearRCSResourceAttributes(JNIEnvWrapper* env)
{
    env->DeleteGlobalRef(g_cls_RCSResourceAttributes);
}

jobject newAttributesObject(JNIEnv* env, const RCSResourceAttributes& attrs)
{
    jobject obj = env->NewObject(g_cls_RCSResourceAttributes, g_ctor_RCSResourceAttributes);
    VERIFY_NO_EXC_RET_DEF(env);

    jobject mapObj = env->GetObjectField(obj, g_field_mCache);

    //EXPECT(mapObj, "Map is null.");
    for (const auto& p : attrs) {
        JavaLocalObject keyObj{ env, newStringObject(env, p.key()) };
        //VERIFY_NO_EXC(env);

        JavaLocalObject valueObj{ env, newRCSValueObject(env, p.value()) };
        //VERIFY_NO_EXC(env);

        invoke_Map_put(env, mapObj, keyObj, valueObj);
        //VERIFY_NO_EXC(env);
    }

 //   setSafeNativeHandle< RCSResourceAttributes >(env, obj, attrs);

    return obj;
}

jobject newAttributesObject(JNIEnvWrapper* env, const RCSResourceAttributes& attrs)
{
    jobject obj = env->NewObject(g_cls_RCSResourceAttributes, g_ctor_RCSResourceAttributes);

    jobject mapObj = env->GetObjectField(obj, g_field_mCache);

    //EXPECT(mapObj, "Map is null.");
    for (const auto& p : attrs) {
        JavaLocalObject keyObj{ env, newStringObject(env, p.key()) };
        //VERIFY_NO_EXC(env);

        JavaLocalObject valueObj{ env, newRCSValueObject(env, p.value()) };
        //VERIFY_NO_EXC(env);

        invoke_Map_put(env, mapObj, keyObj, valueObj);
        //VERIFY_NO_EXC(env);
    }

//    setSafeNativeHandle< RCSResourceAttributes >(env, obj, attrs);

    return obj;
}

RCSResourceAttributes toNativeAttributes(JNIEnv* env, jobject attrsObj)
{
    EXPECT_RET(attrsObj, "attrsObj is null!", { });

    JNIEnvWrapper envWrapper{ env };

    try
    {
        return toNativeAttributes(&envWrapper, attrsObj);
    }
    catch (const JavaException&)
    {
        return {};
    }
}

RCSResourceAttributes toNativeAttributes(JNIEnvWrapper* env, jobject attrsObj)
{
    EXPECT_RET(attrsObj, "attrsObj is null!", { });

    RCSResourceAttributes attrs;

    /*if (hasNativeHandle(env, attrsObj))
    {
        attrs = getNativeHandleAs< RCSResourceAttributes >(env, attrsObj);
    }*/
    LOGD("writeNativeAttributesFromMap");

    writeNativeAttributesFromMap(env,
            JavaLocalObject{ env, env->GetObjectField(attrsObj, g_field_mCache) }, attrs);

    return attrs;
}

void writeNativeAttributesFromMap(JNIEnv* env, jobject mapObj, RCSResourceAttributes& targetAttrs)
{
    JNIEnvWrapper envWrapper{ env };

    try
    {
        return writeNativeAttributesFromMap(&envWrapper, mapObj, targetAttrs);
    }
    catch (const JavaException&)
    {
    }
}

void writeNativeAttributesFromMap(JNIEnvWrapper* env, jobject mapObj,
        RCSResourceAttributes& targetAttrs)
{
    LOGD("in write native attributes from map");
    LOGD("invoke map entry set");
    JavaLocalObject setObj{ env, invoke_Map_entrySet(env, mapObj) };
    LOGD("invoke set iterator");
    JavaLocalObject iterObj{ env, invoke_Set_iterator(env, setObj) };
    LOGD("invoke has next");
    while (invoke_Iterator_hasNext(env, iterObj))
    {
        LOGD("invoke entry obj next");
        JavaLocalObject entryObj{ env, invoke_Iterator_next(env, iterObj) };
        LOGD("invoke key obj next");
        JavaLocalObject keyObj{ env, invoke_MapEntry_getKey(env, entryObj) };
        LOGD("invoke value obj next");
        JavaLocalObject valueObj{ env, invoke_MapEntry_getValue(env, entryObj) };
        LOGD("invoke toStdString");
        auto key = toStdString(env, static_cast< jstring >(keyObj.get()));
        LOGD("invoke toNativeAttrsvalue");
        targetAttrs[std::move(key)] = toNativeAttrsValue(env, valueObj);
    }
    LOGD("in write native attributes from map finished");
}

