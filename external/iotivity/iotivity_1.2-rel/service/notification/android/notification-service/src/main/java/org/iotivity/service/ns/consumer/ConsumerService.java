//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
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

package org.iotivity.service.ns.consumer;

import android.util.Log;
import org.iotivity.service.ns.common.*;
import java.util.Vector;

/**
 *
 * This class provides a set of Java APIs for Notification ConsumerService.
 *
 */
public class ConsumerService {

    private static final String LOG_TAG = "ConsumerService";

    static {
        System.loadLibrary("gnustl_shared");
        System.loadLibrary("oc_logger");
        System.loadLibrary("connectivity_abstraction");
        System.loadLibrary("ca-interface");
        System.loadLibrary("octbstack");
        System.loadLibrary("oc");
        System.loadLibrary("ocstack-jni");
        System.loadLibrary("notification_consumer");
        System.loadLibrary("notification_consumer_wrapper");
        System.loadLibrary("notification_consumer_jni");
    }

    private static ConsumerService instance;
    static {
        instance = new ConsumerService();
    }

    /**
     * API for getting instance of ConsumerService
     *
     * @return ConsumerService singleton instance created
     */
    public static ConsumerService getInstance() {
        return instance;
    }

    /**
     * This API will Start ConsumerService
     *
     * @param onProviderDiscoveredListener
     *            OnProviderDiscoveredListener Callback Interface
     *
     * @throws NSException
     *             if the parameter passed is null
     */
    public void start(OnProviderDiscoveredListener onProviderDiscoveredListener)
            throws NSException {
        nativeStart(onProviderDiscoveredListener);
    }

    /**
     * This API will Stop ConsumerService
     */
    public void stop() throws NSException {
        nativeStop();
    }

    /**
     * Request to publish resource to cloud server
     *
     * @param serverAddress
     *            serverAddress combined with IP address and port number using
     *            delimiter
     */
    public void enableRemoteService(String serverAddress) throws NSException {
        nativeEnableRemoteService(serverAddress);
    }

    /**
     * Request to subscribe to MQ server
     *
     * @param servAdd
     *            servAdd combined with IP address and port number and MQ broker
     *            uri using delimiter
     * @param topicName
     *            the interest Topic name for subscription
     *
     * @throws NSException failed to subscribe to MQ server
     */
    public void subscribeMQService(String servAdd, String topicName)
            throws NSException {
        nativeSubscribeMQService(servAdd, topicName);
    }

    /**
     * This API is called to request discovery manually
     */
    public void rescanProvider() throws NSException {
        nativeRescanProvider();
    }

    /**
     * Interface to implement callback function to receive provider on discovery
     */
    public interface OnProviderDiscoveredListener {

        /**
         * Callback function to receive provider on discovery
         *
         * @param provider
         *            Provider object
         */
        public void onProviderDiscovered(Provider provider);
    }

    private native void nativeStart(
            OnProviderDiscoveredListener onProviderDiscoveredListener)
            throws NSException;

    private native void nativeStop() throws NSException;

    private native void nativeEnableRemoteService(String serverAddress)
            throws NSException;

    private native void nativeSubscribeMQService(String servAdd,
            String topicName) throws NSException;

    private native void nativeRescanProvider() throws NSException;
}
