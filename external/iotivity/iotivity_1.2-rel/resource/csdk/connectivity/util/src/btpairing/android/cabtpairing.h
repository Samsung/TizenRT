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

#include "jni.h"
/* Header for class org_iotivity_ca_CaBtPairingInterface */

#ifndef Included_org_iotivity_ca_CaBtPairingInterface
#define Included_org_iotivity_ca_CaBtPairingInterface
#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Class:     org_iotivity_ca_CaBtPairingInterface
 * Method:    oicEdrStateChangedCallback
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
org_iotivity_ca_CaBtPairingInterface_oicEdrStateChangedCallback
(JNIEnv *, jobject, jint);

/*
 * Class:     org_iotivity_ca_OicBtUtilClientInterface
 * Method:    oicEdrBondStateChangedCallback
 * Signature: (java/lang/String;)V
 */
JNIEXPORT void JNICALL
org_iotivity_ca_CaBtPairingInterface_oicEdrBondStateChangedCallback
(JNIEnv *, jobject, jstring);

/*
 * Class:     org_iotivity_ca_OicBtUtilClientInterface
 * Method:    oicEdrFoundDeviceCallback
 * Signature: (Landroid/bluetooth/BluetoothDevice;)V
 */
JNIEXPORT void JNICALL
org_iotivity_ca_CaBtPairingInterface_oicEdrFoundDeviceCallback
(JNIEnv *, jobject, jobject);


#ifdef __cplusplus
}
#endif
#endif
