/**
 * ***************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ****************************************************************
 */


package org.iotivity.service.easysetup.mediator;

import android.util.Log;

import org.iotivity.base.OcRepresentation;

/**
 * This class represents Remote Enrollee device instance. What operations the class provides:
 * 1) Ownership transfer for enabling secured communication between Mediator and Enrollee
 * devices.
 * 2) Provision WiFi AP information used for which Enrollee is going to connect to the AP
 * 3) Provision Device confiruation setting, i.e. language, country, and etc
 * 4) Provision Cloud information used for which Enrollee is going to register to the cloud
 */
public class RemoteEnrollee{

    public static final String TAG = RemoteEnrollee.class.getName();
    private long m_nativeHandle;

    private native void nativeGetStatus(GetStatusCallback callback);
    private native void nativeGetConfiguration(GetConfigurationCallback callback);
    private native void nativeProvisionSecurity(SecurityProvisioningCallback callback);
    private native void nativeProvisionDeviceProperties(OcRepresentation deviceProp,
                                                        DevicePropProvisioningCallback callback);
    private native void nativeProvisionCloudProperties(OcRepresentation cloudProp, String cloudID,
                                                       int CredID, CloudPropProvisioningCallback callback);

    /* constructor will be invoked from the native layer */
    private RemoteEnrollee(long nativeHandle){
        this.m_nativeHandle = nativeHandle;
    }

    /**
     * Get an Enrollee's status which includes a provisioning status and last error code
     *
     * @param callback will give the requested status
     *
     * @throws ESException If some errors happen in this function
     *
     * @see GetStatusCallback
     */
    public void getStatus(GetStatusCallback callback) throws ESException
    {
        if(callback != null)
        {
            nativeGetStatus(callback);
            return;
        }
        Log.d(TAG, "GetStatusCallback is null ");
    }

    /**
     * Get an Enrollee's configuration which includes WiFi supported frequency and device name
     *
     * @param callback will give the requested configuration
     *
     * @throws ESException If some errors happen in this function
     *
     * @see GetConfigurationCallback
     */
    public void getConfiguration(GetConfigurationCallback callback)
                                                        throws ESException
    {
        if(callback != null)
        {
            nativeGetConfiguration(callback);
            return;
        }
        Log.d(TAG, "GetConfigurationCallback is null ");
    }

    /**
     * Do security provisioning such as ownership tranfer to Enrollee.
     *
     * @param callback will give the result if the security provisioning succeeds or fails for some reasons
     *
     * @throws ESException If some errors happen in this function
     *
     * @see SecurityProvisioningCallback
     */
    public void provisionSecurity(SecurityProvisioningCallback callback)
                                                        throws ESException
    {
        if(callback != null)
        {
            nativeProvisionSecurity(callback);
            return;
        }
        Log.d(TAG, "SecurityProvisioningCallback is null ");
    }

    /**
     * Provision WiFi AP information and device configuration to Enrollee
     * 1. WiFi AP information includes a SSID, password, auth type, and encryption type.
     * 2. Device configuration includes a language (IETF language tags) and country (ISO 3166-1 Alpha-2)
     *
     * @param deviceProp a data structure storing the above information to be delivered
     * @param callback will give the result if the provisioning succeeds or fails
     *
     * @throws ESException If some errors happen in this function
     *
     * @see DeviceProp
     * @see DevicePropProvisioningCallback
     */
    public void provisionDeviceProperties(DeviceProp deviceProp,
                    DevicePropProvisioningCallback callback) throws ESException{
        if(callback != null)
        {
            nativeProvisionDeviceProperties(deviceProp.toOCRepresentation(),
                                                callback);
            return;
        }
        Log.d(TAG, "DevicePropProvisioningCallback is null ");
    }

    /**
     * Provision Cloud information to Enrollee, which includes Auth code, auth provider,
     * Cloud interface server URL, and etc.
     * In this function, Discovery for the Enrollee will happen again in a given network.
     * Because, this function is expected to call *AFTER* the Enrollee disconnects its Soft AP
     * and successfully connects to the certain WiFi AP. In that case, Mediator should discover
     * the Enrollee with a certain Device ID in the network.
     *
     * @param cloudProp a data structure storing the above information to be delivered
     * @param callback will give the result if the provisioning succeeds or fails
     *
     * @throws ESException If some errors happen in this function
     *
     * @see CloudProp
     * @see CloudPropProvisioningCallback
     */
    public void provisionCloudProperties(CloudProp cloudProp,
                    CloudPropProvisioningCallback callback) throws ESException{
        if(callback != null)
        {
            nativeProvisionCloudProperties(cloudProp.toOCRepresentation(),
                                            cloudProp.getCloudID(),
                                            cloudProp.getCredID(),
                                            callback);
            return;
        }
        Log.d(TAG, "CloudPropProvisioningCallback is null ");
    }
}
