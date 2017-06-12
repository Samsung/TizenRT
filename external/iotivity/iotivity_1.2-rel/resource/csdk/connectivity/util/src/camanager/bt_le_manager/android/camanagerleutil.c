/* ****************************************************************
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

#include <jni.h>
#include "cacommon.h"
#include "logger.h"
#include "cacommonutil.h"
#include "camanagerleutil.h"
#include "caleclient.h"
#include "caleutils.h"

#define TAG "OIC_CA_MANAGER_UTIL"

static const char CLASSPATH_CONTENT_PREF[] = "android/content/SharedPreferences";
static const char CLASSPATH_CONTENT_PREF_EDITOR[] = "android/content/SharedPreferences$Editor";
static const char CLASSPATH_CONTEXT[] = "android/content/Context";

static const char PREF_NAME_CONNECTED_DEVICE[] = "PREF_NAME_CONNECTED_DEVICE";
static const char PREF_KEY_CONNECTED_DEVICE[] = "PREF_KEY_CONNECTED_DEVICE";

static const char METHODID_BOOLEAN_RET_STRING_PARAM[] = "(Ljava/lang/String;)Z";

static jobject CAManagerGetAdapterContext(JNIEnv *env)
{
    OIC_LOG(DEBUG, TAG, "CAManagerGetAdapterContext");
    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);

    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADAPTER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTAdapter: jni_cid_BTAdapter is null");
        return NULL;
    }

    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    METHODID_OBJECTNONPARAM);
    if (!jni_mid_getDefaultAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDefaultAdapter is null");
        return NULL;
    }

    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTAdapter is null");
        return NULL;
    }
    return jni_obj_BTAdapter;
}

jobject CAManagerGetRemoteDevice(JNIEnv *env, jstring address)
{
    OIC_LOG(DEBUG, TAG, "IN - CAManagerGetRemoteDevice");

    VERIFY_NON_NULL_RET(env, TAG, "env is null", NULL);
    VERIFY_NON_NULL_RET(address, TAG, "address is null", NULL);

    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADAPTER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTAdapter is null");
        return NULL;
    }

    // get remote bt adapter method
    jmethodID jni_mid_getDefaultAdapter = (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                                                    "getDefaultAdapter",
                                                                    METHODID_OBJECTNONPARAM);
    if (!jni_mid_getDefaultAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getDefaultAdapter is null");
        return NULL;
    }

    // gat bt adapter object
    jobject jni_obj_BTAdapter = (*env)->CallStaticObjectMethod(env, jni_cid_BTAdapter,
                                                               jni_mid_getDefaultAdapter);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTAdapter is null");
        return NULL;
    }

    jmethodID jni_mid_getRemoteDevice = (*env)->GetMethodID(env, jni_cid_BTAdapter,
                                                            "getRemoteDevice",
                                                            METHODID_BT_REMOTE_DEVICE);
    if (!jni_mid_getRemoteDevice)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getRemoteDevice is null");
        return NULL;
    }

    jobject jni_obj_device = (*env)->CallObjectMethod(env, jni_obj_BTAdapter,
                                                      jni_mid_getRemoteDevice,
                                                      address);
    if (!jni_obj_device)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_device is null");
        return NULL;
    }

    OIC_LOG(DEBUG, TAG, "OUT - CAManagerGetRemoteDevice");
    return jni_obj_device;
}

bool CAManagerIsDeviceBonded(JNIEnv *env, jobject btDevice)
{
    OIC_LOG(DEBUG, TAG, "IN - CAManagerIsDeviceBonded");

    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(btDevice, TAG, "btDevice is null");

    jclass jni_cid_BTDevice = (*env)->FindClass(env, CLASSPATH_BT_DEVICE);
    if (!jni_cid_BTDevice)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTDevice is null");
        return false;
    }

    jmethodID jni_mid_getBondState = (*env)->GetMethodID(env, jni_cid_BTDevice,
                                                         "getBondState", "()I");
    if (!jni_mid_getBondState)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getBondState is null");
        return false;
    }

    jint jni_bond_state = (*env)->CallIntMethod(env, btDevice, jni_mid_getBondState);
    OIC_LOG_V(DEBUG, TAG, "bond state is %d", jni_bond_state);

    // BOND_BONDED - Constant value : 12 (0x0000000c)
    jfieldID id_bonded = (*env)->GetStaticFieldID(env, jni_cid_BTDevice,
                                                  "BOND_BONDED", "I");
    if (!id_bonded)
    {
        OIC_LOG(ERROR, TAG, "id_bonded is null");
        return false;;
    }

    jint jni_bonded_const = (*env)->GetStaticIntField(env, jni_cid_BTDevice, id_bonded);

    if (jni_bond_state == jni_bonded_const)
    {
        OIC_LOG(INFO, TAG, "it is not bonded to each other");
        return false;
    }
    OIC_LOG(DEBUG, TAG, "OUT - CAManagerIsDeviceBonded");
    return true;
}

bool CAManagerControlAdapter(JNIEnv *env, bool control_flag)
{
    OIC_LOG(DEBUG, TAG, "IN - CAManagerControlAdapter");

    VERIFY_NON_NULL_RET(env, TAG, "env", false);

    jobject jni_obj_BTAdapter = CAManagerGetAdapterContext(env);
    if (!jni_obj_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_BTAdapter is null");
        return false;
    }

    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADAPTER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTAdapter: jni_cid_BTAdapter is null");
        return NULL;
    }

    OIC_LOG_V(DEBUG, TAG, "change adapter : %d", control_flag);
    jmethodID jni_mid_control = NULL;
    if (control_flag)
    {
        // enable()
        jni_mid_control = (*env)->GetMethodID(env, jni_cid_BTAdapter, "enable", "()Z");
    }
    else
    {
        // disable()
        jni_mid_control = (*env)->GetMethodID(env, jni_cid_BTAdapter, "disable", "()Z");
    }

    if (!jni_mid_control)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_control is null");
        return false;
    }

    OIC_LOG(DEBUG, TAG, "CALL API - Adapter Will be Changed");
    jboolean jni_res = (*env)->CallBooleanMethod(env, jni_obj_BTAdapter, jni_mid_control);

    return jni_res;
}

CAResult_t CAManagerReadRemoteRssi(JNIEnv *env, jobject bluetoothGatt)
{
    VERIFY_NON_NULL(env, TAG, "env is null");
    VERIFY_NON_NULL(bluetoothGatt, TAG, "bluetoothGatt is null");

    if (!CALEIsEnableBTAdapter(env))
    {
        OIC_LOG(INFO, TAG, "BT adapter is not enabled");
        return CA_ADAPTER_NOT_ENABLED;
    }

    // get BluetoothGatt class
    jclass jni_cid_BluetoothGatt = (*env)->FindClass(env, CLASSPATH_BT_GATT);
    if (!jni_cid_BluetoothGatt)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BluetoothGatt is null");
        return CA_STATUS_FAILED;
    }

    jmethodID jni_mid_readRemoteRssi = (*env)->GetMethodID(env, jni_cid_BluetoothGatt,
                                                             "readRemoteRssi", "()Z");
    if (!jni_mid_readRemoteRssi)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_readRemoteRssi is null");
        return CA_STATUS_FAILED;
    }
    // call disconnect gatt method
    jboolean ret = (*env)->CallBooleanMethod(env, bluetoothGatt, jni_mid_readRemoteRssi);
    if (!ret)
    {
        OIC_LOG(ERROR, TAG, "readremoteRssi has not been called");
        return CA_STATUS_FAILED;
    }

    return CA_STATUS_OK;
}

jobject CAManagerGetSharedPreference(JNIEnv *env, jobject context, jstring prefName)
{
    VERIFY_NON_NULL_RET(env, TAG, "env", NULL);
    VERIFY_NON_NULL_RET(context, TAG, "context", NULL);
    VERIFY_NON_NULL_RET(prefName, TAG, "prefName", NULL);

    jclass jni_cls_context = (*env)->FindClass(env, CLASSPATH_CONTEXT);
    if (!jni_cls_context)
    {
        OIC_LOG(ERROR, TAG, "jni_cls_context is null");
        return NULL;
    }

    // getSharedPreferences
    jmethodID jni_mid_getPref = (*env)->GetMethodID(env, jni_cls_context, "getSharedPreferences",
                                                    "(Ljava/lang/String;I)"
                                                    "Landroid/content/SharedPreferences;");
    if (!jni_mid_getPref)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getPref is null");
        return NULL;
    }

    jobject jni_obj_sharedPref = (*env)->CallObjectMethod(env, context,
                                                          jni_mid_getPref,
                                                          prefName,
                                                          0);
    if (!jni_obj_sharedPref)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_sharedPref is null");
        return NULL;
    }

    return jni_obj_sharedPref;
}

jobject CAManagerGetPrefEditObject(JNIEnv *env, jobject context)
{
    OIC_LOG(DEBUG, TAG, "IN-CAManagerGetPrefEditObject");
    VERIFY_NON_NULL_RET(env, TAG, "env", NULL);
    VERIFY_NON_NULL_RET(context, TAG, "context", NULL);

    jstring jni_str_prefName = (*env)->NewStringUTF(env, PREF_NAME_CONNECTED_DEVICE);
    if (!jni_str_prefName)
    {
        OIC_LOG(ERROR, TAG, "jni_str_prefName is null");
        return NULL;
    }

    jstring jni_str_prefKey = (*env)->NewStringUTF(env, PREF_KEY_CONNECTED_DEVICE);
    if (!jni_str_prefKey)
    {
        OIC_LOG(ERROR, TAG, "jni_str_prefKey is null");
        return NULL;
    }

    // get SharedPreference
    jobject jni_obj_pref = CAManagerGetSharedPreference(env, context, jni_str_prefName);
    if (!jni_obj_pref)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_pref is null");
        return NULL;
    }

    // edit()
    jobject jni_cls_pref = (*env)->FindClass(env, CLASSPATH_CONTENT_PREF);
    if (!jni_cls_pref)
    {
        OIC_LOG(ERROR, TAG, "jni_cls_pref is null");
        return NULL;
    }

    jmethodID jni_mid_prefEdit = (*env)->GetMethodID(env, jni_cls_pref, "edit",
                                                     "()Landroid/content/SharedPreferences$Editor;");
    if (!jni_mid_prefEdit)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_prefEdit is null");
        return NULL;
    }

    jobject jni_obj_prefEdit = (*env)->CallObjectMethod(env, jni_obj_pref, jni_mid_prefEdit);
    if (!jni_obj_prefEdit)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_prefEdit is null");
        return NULL;
    }

    OIC_LOG(DEBUG, TAG, "OUT-CAManagerGetPrefEditObject");
    return jni_obj_prefEdit;
}

bool CAManagerUpdatePrefStringSet(JNIEnv *env, jobject context, jobject set)
{
    OIC_LOG(DEBUG, TAG, "IN-CAManagerUpdatePrefStringSet");

    VERIFY_NON_NULL_RET(env, TAG, "env", false);
    VERIFY_NON_NULL_RET(context, TAG, "context", false);
    VERIFY_NON_NULL_RET(set, TAG, "set", false);

    jstring jni_str_prefKey = (*env)->NewStringUTF(env, PREF_KEY_CONNECTED_DEVICE);
    if (!jni_str_prefKey)
    {
        OIC_LOG(ERROR, TAG, "jni_str_prefKey is null");
        return false;
    }

    jobject jni_obj_prefEdit = CAManagerGetPrefEditObject(env, context);
    if (!jni_obj_prefEdit)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_prefEdit is null");
        return false;
    }

    // putString()
    jobject jni_cls_edit = (*env)->FindClass(env, CLASSPATH_CONTENT_PREF_EDITOR);
    if (!jni_cls_edit)
    {
        OIC_LOG(ERROR, TAG, "jni_cls_edit is null");
        return false;
    }

    // get putString method interface
    jmethodID jni_mid_PrefPutStringSet = (*env)->GetMethodID(env, jni_cls_edit, "putStringSet",
                                                             "(Ljava/lang/String;Ljava/util/Set;)"
                                                             "Landroid/content/SharedPreferences"
                                                             "$Editor;");
    if (!jni_mid_PrefPutStringSet)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_PrefPutStringSet is null");
        return false;
    }

    jobject gSetString = (jobject)(*env)->NewGlobalRef(env, set);
    if (!gSetString)
    {
        OIC_LOG(ERROR, TAG, "gAddress is null");
        return false;
    }

    OIC_LOG(DEBUG, TAG, "CALL API - request putString for SharedPreferences.Editor");
    (*env)->CallObjectMethod(env, jni_obj_prefEdit, jni_mid_PrefPutStringSet,
                             jni_str_prefKey, gSetString);

    // get commit method interface
    jmethodID jni_mid_PrefCommit = (*env)->GetMethodID(env, jni_cls_edit, "commit",
                                                       "()Z");
    if (!jni_mid_PrefCommit)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_PrefCommit is null");
        return false;
    }

    jboolean res = (*env)->CallBooleanMethod(env, jni_obj_prefEdit, jni_mid_PrefCommit);

    OIC_LOG(DEBUG, TAG, "OUT-CAManagerUpdatePrefStringSet");
    return res;
}

jobject CAManagerGetPrefStringSet(JNIEnv *env, jobject context)
{
    OIC_LOG(DEBUG, TAG, "CAManagerGetPrefStringSet");

    VERIFY_NON_NULL_RET(env, TAG, "env", NULL);
    VERIFY_NON_NULL_RET(context, TAG, "context", NULL);

    jstring jni_str_prefName = (*env)->NewStringUTF(env, PREF_NAME_CONNECTED_DEVICE);
    if (!jni_str_prefName)
    {
        OIC_LOG(ERROR, TAG, "jni_str_prefName is null");
        return NULL;
    }

    jstring jni_str_prefKey = (*env)->NewStringUTF(env, PREF_KEY_CONNECTED_DEVICE);
    if (!jni_str_prefKey)
    {
        OIC_LOG(ERROR, TAG, "jni_str_prefKey is null");
        return NULL;
    }

    // get SharedPreference
    jobject jni_obj_pref = CAManagerGetSharedPreference(env, context, jni_str_prefName);
    if (!jni_obj_pref)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_pref is null");
        return NULL;
    }

    // contains(String key)
    jobject jni_cls_pref = (*env)->FindClass(env, CLASSPATH_CONTENT_PREF);
    if (!jni_cls_pref)
    {
        OIC_LOG(ERROR, TAG, "jni_cls_pref is null");
        return NULL;
    }

    jmethodID jni_mid_getStringSet = (*env)->GetMethodID(env, jni_cls_pref, "getStringSet",
                                                         "(Ljava/lang/String;Ljava/util/Set;)"
                                                         "Ljava/util/Set;");
    if (!jni_mid_getStringSet)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_getStringSet is null");
        return NULL;
    }

    jobject jni_defSet = CAManagerCreateSetString(env);
    if (!jni_defSet)
    {
        OIC_LOG(ERROR, TAG, "jni_defSet is null");
        return NULL;
    }

    jobject value = (*env)->CallObjectMethod(env, jni_obj_pref,
                                             jni_mid_getStringSet,
                                             jni_str_prefKey,
                                             jni_defSet);

    return value;
}

bool CAManagerContainsPrefStringSet(JNIEnv *env, jobject context)
{
    OIC_LOG(DEBUG, TAG, "CAManagerContainsPrefStringSet");

    VERIFY_NON_NULL_RET(env, TAG, "env", false);
    VERIFY_NON_NULL_RET(context, TAG, "context", false);

    jstring jni_str_prefName = (*env)->NewStringUTF(env, PREF_NAME_CONNECTED_DEVICE);
    if (!jni_str_prefName)
    {
        OIC_LOG(ERROR, TAG, "jni_str_prefName is null");
        return false;
    }

    jstring jni_str_prefKey = (*env)->NewStringUTF(env, PREF_KEY_CONNECTED_DEVICE);
    if (!jni_str_prefKey)
    {
        OIC_LOG(ERROR, TAG, "jni_str_prefKey is null");
        return false;
    }

    // get SharedPreference
    jobject jni_obj_pref = CAManagerGetSharedPreference(env, context, jni_str_prefName);
    if (!jni_obj_pref)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_pref is null");
        return false;
    }

    // contains(String key)
    jobject jni_cls_pref = (*env)->FindClass(env, CLASSPATH_CONTENT_PREF);
    if (!jni_cls_pref)
    {
        OIC_LOG(ERROR, TAG, "jni_cls_pref is null");
        return false;
    }

    jmethodID jni_mid_prefContains = (*env)->GetMethodID(env, jni_cls_pref, "contains",
                                                         METHODID_BOOLEAN_RET_STRING_PARAM);
    if (!jni_mid_prefContains)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_prefContains is null");
        return false;
    }

    jboolean res = (*env)->CallBooleanMethod(env, jni_obj_pref,
                                             jni_mid_prefContains,
                                             jni_str_prefKey);

    OIC_LOG_V(DEBUG, TAG, "Preference - contains (%d)", res);
    return res;
}

bool CAManagerRemovePrefStringSet(JNIEnv *env, jobject context)
{
    OIC_LOG(DEBUG, TAG, "CAManagerRemovePrefStringSet");

    VERIFY_NON_NULL_RET(env, TAG, "env", false);
    VERIFY_NON_NULL_RET(context, TAG, "context", false);

    jstring jni_str_prefKey = (*env)->NewStringUTF(env, PREF_KEY_CONNECTED_DEVICE);
    if (!jni_str_prefKey)
    {
        OIC_LOG(ERROR, TAG, "jni_str_prefKey is null");
        return false;
    }

    jobject jni_obj_prefEdit = CAManagerGetPrefEditObject(env, context);
    if (!jni_obj_prefEdit)
    {
        OIC_LOG(ERROR, TAG, "jni_obj_prefEdit is null");
        return false;
    }

    // putString()
    jobject jni_cls_edit = (*env)->FindClass(env, CLASSPATH_CONTENT_PREF_EDITOR);
    if (!jni_cls_edit)
    {
        OIC_LOG(ERROR, TAG, "jni_cls_edit is null");
        return false;
    }

    // get remove method interface
    jmethodID jni_mid_PrefRemove = (*env)->GetMethodID(env, jni_cls_edit, "remove",
                                                       "(Ljava/lang/String;)"
                                                       "Landroid/content/SharedPreferences"
                                                       "$Editor;");
    if (!jni_mid_PrefRemove)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_PrefRemove is null");
        return false;
    }

    OIC_LOG(DEBUG, TAG, "CALL API - request remove for SharedPreferences.Editor");
    (*env)->CallObjectMethod(env, jni_obj_prefEdit, jni_mid_PrefRemove, jni_str_prefKey);

    // get commit method interface
    jmethodID jni_mid_PrefCommit = (*env)->GetMethodID(env, jni_cls_edit, "commit",
                                                       "()Z");
    if (!jni_mid_PrefCommit)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_PrefCommit is null");
        return false;
    }

    jboolean res = (*env)->CallBooleanMethod(env, jni_obj_prefEdit, jni_mid_PrefCommit);

    OIC_LOG(DEBUG, TAG, "OUT-CAManagerAddConnectedDeviceAddress");
    return res;
}

bool CAManagerAddConnectedDeviceAddress(JNIEnv *env, jobject context,
                                        jstring address, jobject set)
{
    OIC_LOG(DEBUG, TAG, "IN-CAManagerAddConnectedDeviceAddress");

    VERIFY_NON_NULL_RET(env, TAG, "env", false);
    VERIFY_NON_NULL_RET(context, TAG, "context", false);
    VERIFY_NON_NULL_RET(address, TAG, "address", false);
    VERIFY_NON_NULL_RET(set, TAG, "set", false);

    if (CAManagerCallFuncSetString(env, address, set, CM_CONTAINS))
    {
        OIC_LOG(DEBUG, TAG, "it's already done");
        return true;
    }

    if (!CAManagerCallFuncSetString(env, address, set, CM_ADD))
    {
        OIC_LOG(ERROR, TAG, "CAManagerAddSetString has failed");
        return false;
    }

    return CAManagerUpdatePrefStringSet(env, context, set);
}

bool CAManagerIsConnectedDeviceAddress(JNIEnv *env, jobject context,
                                       jstring address, jobject set)
{
    VERIFY_NON_NULL_RET(env, TAG, "env", false);
    VERIFY_NON_NULL_RET(context, TAG, "context", false);

    return CAManagerCallFuncSetString(env, address, set, CM_CONTAINS);
}

jobject CAManagerGetConnectedDeviceAddress(JNIEnv *env, jobject context)
{
    OIC_LOG(DEBUG, TAG, "CAManagerGetConnectedDeviceAddress");

    if (!CAManagerContainsPrefStringSet(env, context))
    {
        OIC_LOG(DEBUG, TAG, "there is no set data");
        return NULL;
    }

    return CAManagerGetPrefStringSet(env, context);
}

bool CAManagerRemoveConnectedDeviceAddress(JNIEnv *env, jobject context,
                                           jstring address, jobject set)
{
    OIC_LOG(DEBUG, TAG, "CAManagerRemoveConnectedDeviceAddress");

    VERIFY_NON_NULL_RET(env, TAG, "env", false);
    VERIFY_NON_NULL_RET(context, TAG, "context", false);
    VERIFY_NON_NULL_RET(address, TAG, "address", false);
    VERIFY_NON_NULL_RET(set, TAG, "set", false);

    if (!CAManagerCallFuncSetString(env, address, set, CM_CONTAINS))
    {
        OIC_LOG(DEBUG, TAG, "it's already done");
        return true;
    }

    if (!CAManagerCallFuncSetString(env, address, set, CM_REMOVE))
    {
        OIC_LOG(ERROR, TAG, "CAManagerAddSetString has failed");
        return false;
    }

    return CAManagerUpdatePrefStringSet(env, context, set);
}

jboolean CAManagerCheckBTAddress(JNIEnv *env, jstring address)
{
    OIC_LOG(DEBUG, TAG, "CAManagerCheckBTAddress");

    VERIFY_NON_NULL_RET(env, TAG, "env is null", JNI_FALSE);
    VERIFY_NON_NULL_RET(address, TAG, "address is null", JNI_FALSE);

    jclass jni_cid_BTAdapter = (*env)->FindClass(env, CLASSPATH_BT_ADAPTER);
    if (!jni_cid_BTAdapter)
    {
        OIC_LOG(ERROR, TAG, "jni_cid_BTAdapter is null");
        return JNI_FALSE;
    }

    // get remote bt adapter method
    jmethodID jni_mid_checkBTAddress =
            (*env)->GetStaticMethodID(env, jni_cid_BTAdapter,
                                      "checkBluetoothAddress",
                                      METHODID_BOOLEAN_RET_STRING_PARAM);
    if (!jni_mid_checkBTAddress)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_checkBTAddress is null");
        return JNI_FALSE;
    }

    jboolean jni_obj_isAddress = (*env)->CallStaticBooleanMethod(env, jni_cid_BTAdapter,
                                                                 jni_mid_checkBTAddress,
                                                                 address);
    return jni_obj_isAddress;
}

jobject CAManagerCreateSetString(JNIEnv *env)
{
    OIC_LOG(DEBUG, TAG, "CAManagerCallFuncSetString");

    VERIFY_NON_NULL_RET(env, TAG, "env", NULL);

    jclass jclazzMap = (*env)->FindClass(env, "java/util/HashSet" );
    if (!jclazzMap)
    {
        OIC_LOG(ERROR, TAG, "jclazzMap is null");
        return NULL;
    }

    jmethodID jinitMap = (*env)->GetMethodID(env, jclazzMap, "<init>", "()V");
    if (!jinitMap)
    {
        OIC_LOG(ERROR, TAG, "jinitMap is null");
        return NULL;
    }

    jobject jpropertyMap = (*env)->NewObject(env, jclazzMap, jinitMap);
    if (!jpropertyMap)
    {
        OIC_LOG(ERROR, TAG, "jpropertyMap is null");
        return NULL;
    }

    return jpropertyMap;
}

bool CAManagerCallFuncSetString(JNIEnv *env, jstring address, jobject set,
                                CASetMethod_t method_type)
{
    VERIFY_NON_NULL_RET(env, TAG, "env", false);
    VERIFY_NON_NULL_RET(address, TAG, "address", false);
    VERIFY_NON_NULL_RET(set, TAG, "set", false);

    jclass jni_cls_set = (*env)->FindClass(env, "java/util/HashSet");
    if (!jni_cls_set)
    {
        OIC_LOG(ERROR, TAG, "jni_cls_set is null");
        return false;
    }

    jmethodID jni_mid_setMethod = NULL;
    switch (method_type)
    {
        case CM_CONTAINS:
            jni_mid_setMethod = (*env)->GetMethodID(env, jni_cls_set, "contains",
                                                    "(Ljava/lang/Object;)Z");
            break;
        case CM_ADD:
            jni_mid_setMethod = (*env)->GetMethodID(env, jni_cls_set, "add",
                                                    "(Ljava/lang/Object;)Z");
            break;
        case CM_REMOVE:
            jni_mid_setMethod = (*env)->GetMethodID(env, jni_cls_set, "remove",
                                                    "(Ljava/lang/Object;)Z");
            break;
        default:
            break;
    }
    if (!jni_mid_setMethod)
    {
        OIC_LOG(ERROR, TAG, "jni_mid_setMethod is null");
        return false;
    }

    jboolean res = (*env)->CallBooleanMethod(env, set, jni_mid_setMethod, address);
    return res;
}
