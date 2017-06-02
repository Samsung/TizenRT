/*
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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
 */

package org.oic.simulator.client;

import java.util.Map;
import java.util.Vector;

import org.oic.simulator.InvalidArgsException;
import org.oic.simulator.NoSupportException;
import org.oic.simulator.OperationInProgressException;
import org.oic.simulator.SimulatorException;
import org.oic.simulator.SimulatorResourceModel;
import org.oic.simulator.SimulatorResult;

/**
 * SimulatorRemoteResource represents a Resource running in the remote Simulator
 * Server. It comes with a well-defined contract or interface onto which you can
 * perform different operations or subscribe for event notifications.
 */
public final class SimulatorRemoteResource {
    private long           mNativeHandle;
    private String         mUri;
    private int            mConnType;
    private String         mHost;
    private String         mId;
    private Vector<String> mResTypes;
    private Vector<String> mResInterfaces;
    private boolean        mIsObservable;

    @Override
    protected void finalize() throws Throwable {
        try {
            nativeDispose();
        } catch (Throwable t) {
            throw t;
        } finally {
            super.finalize();
        }
    }

    /**
     * Enum to represent the verification types which can be used by the client
     * to verify the resource model of the remote resource.
     */
    public enum RequestType {
        UKNOWN, GET, PUT, POST, DELETE;
    }

    /**
     * API to get the URI for this resource.
     *
     * @return Resource URI.
     */
    public String getURI() {
        return mUri;
    }

    /**
     * API to get the address detail of the resource.
     *
     * @return Host address.
     */
    public String getHost() {
        return mHost;
    }

    /**
     * API to get a unique Id of the resource.
     *
     * @return Unique ID.
     */
    public String getId() {
        return mId;
    }

    /**
     * API to get the connectivity type for this resource.
     *
     * @return Connectivity type.
     */
    public SimulatorConnectivityType getConnectivityType() {
        return SimulatorConnectivityType.getConnectivityType(mConnType);
    }

    /**
     * API to get the list of resource types.
     *
     * @return Resource types.
     */
    public Vector<String> getResourceTypes() {
        return mResTypes;
    }

    /**
     * API to get the list of resource interfaces.
     *
     * @return Resource interfaces.
     */
    public Vector<String> getResourceInterfaces() {
        return mResInterfaces;
    }

    /**
     * API to get the observe policy of this resource.
     *
     * @return True if the resource is observable, otherwise false.
     */
    public boolean isObservable() {
        return mIsObservable;
    }

    /**
     * API to send GET request to the resource. Response will be notified
     * asynchronously via callback set for {@link GetResponseListener}.
     *
     * @param queryParams
     *            Map which can have the query parameter name and value.
     * @param onGetListener
     *            Event handler which will be invoked with the response for GET
     *            request with a map of attribute name and values.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if any parameter has invalid
     *             values.
     * @throws SimulatorException
     *             This exception will be thrown for other errors.
     */
    public void get(Map<String, String> queryParams,
            GetResponseListener onGetListener) throws InvalidArgsException,
            SimulatorException {
        nativeGet(null, queryParams, onGetListener);
    }

    /**
     * API to send GET request to the resource. Response will be notified
     * asynchronously via callback set for {@link GetResponseListener}.
     *
     * @param resourceInterface
     *            Interface type of the resource to operate on.
     * @param queryParams
     *            Map which can have the query parameter name and value.
     * @param onGetListener
     *            Event handler which will be invoked with the response for GET
     *            request with a map of attribute name and values.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if any parameter has invalid
     *             values.
     * @throws SimulatorException
     *             This exception will be thrown for other errors.
     */
    public void get(String resourceInterface, Map<String, String> queryParams,
            GetResponseListener onGetListener) throws InvalidArgsException,
            SimulatorException {
        if (null == resourceInterface || resourceInterface.isEmpty())
            throw new InvalidArgsException(
                    SimulatorResult.SIMULATOR_INVALID_PARAM,
                    "Invalid resource interface!");
        nativeGet(resourceInterface, queryParams, onGetListener);
    }

    /**
     * API to send PUT request to the resource. Response will be notified
     * asynchronously via callback set for {@link PutResponseListener}.
     *
     * @param queryParams
     *            Map which can have the query parameter name and value.
     * @param representation
     *            {@link SimulatorResourceModel} holding the representation of
     *            the resource.
     * @param onPutListener
     *            Event handler which will be invoked with the response for PUT
     *            request with a map of attribute name and values.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if any parameter has invalid
     *             value.
     * @throws SimulatorException
     *             This exception will be thrown for other errors.
     */
    public void put(Map<String, String> queryParams,
            SimulatorResourceModel representation,
            PutResponseListener onPutListener) throws InvalidArgsException,
            SimulatorException {
        nativePut(null, queryParams, representation, onPutListener);
    }

    /**
     * API to send PUT request to the resource. Response will be notified
     * asynchronously via callback set for {@link PutResponseListener}.
     *
     * @param resourceInterface
     *            Interface type of the resource to operate on.
     * @param queryParams
     *            Map which can have the query parameter name and value.
     * @param representation
     *            {@link SimulatorResourceModel} holding the representation of
     *            the resource.
     * @param onPutListener
     *            Event handler which will be invoked with the response for PUT
     *            request with a map of attribute name and values.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if any parameter has invalid
     *             value.
     * @throws SimulatorException
     *             This exception will be thrown for other errors.
     */
    public void put(String resourceInterface, Map<String, String> queryParams,
            SimulatorResourceModel representation,
            PutResponseListener onPutListener) throws InvalidArgsException,
            SimulatorException {
        if (null == resourceInterface || resourceInterface.isEmpty())
            throw new InvalidArgsException(
                    SimulatorResult.SIMULATOR_INVALID_PARAM,
                    "Invalid resource interface!");
        nativePut(resourceInterface, queryParams, representation, onPutListener);
    }

    /**
     * API to send POST request to the resource. Response will be notified
     * asynchronously via callback set for {@link PostResponseListener}.
     *
     * @param queryParams
     *            Map which can have the query parameter name and value.
     * @param representation
     *            {@link SimulatorResourceModel} holding the representation of
     *            the resource.
     * @param onPostListener
     *            Event handler which will be invoked with the response for POST
     *            request with a map of attribute name and values.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if any parameter has invalid
     *             value.
     * @throws SimulatorException
     *             This exception will be thrown for other errors.
     */
    public void post(Map<String, String> queryParams,
            SimulatorResourceModel representation,
            PostResponseListener onPostListener) throws InvalidArgsException,
            SimulatorException {
        nativePost(null, queryParams, representation, onPostListener);
    }

    /**
     * API to send POST request to the resource. Response will be notified
     * asynchronously via callback set for {@link PostResponseListener}.
     *
     * @param resourceInterface
     *            Interface type of the resource to operate on.
     * @param queryParams
     *            Map which can have the query parameter name and value.
     * @param representation
     *            {@link SimulatorResourceModel} holding the representation of
     *            the resource.
     * @param onPostListener
     *            Event handler which will be invoked with the response for POST
     *            request with a map of attribute name and values.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if any parameter has invalid
     *             value.
     * @throws SimulatorException
     *             This exception will be thrown for other errors.
     */
    public void post(String resourceInterface, Map<String, String> queryParams,
            SimulatorResourceModel representation,
            PostResponseListener onPostListener) throws InvalidArgsException,
            SimulatorException {
        if (null == resourceInterface || resourceInterface.isEmpty())
            throw new InvalidArgsException(
                    SimulatorResult.SIMULATOR_INVALID_PARAM,
                    "Invalid resource interface!");
        nativePost(resourceInterface, queryParams, representation,
                onPostListener);
    }

    /**
     * API to start observing the resource.
     *
     * @param onObserveListener
     *            The handler method which will be invoked with a map of
     *            attribute names and values whenever there is a change in
     *            resource model of the remote resource.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if any parameter has invalid
     *             values.
     * @throws SimulatorException
     *             This exception will be thrown for other errors.
     */
    public void observe(ObserveNotificationListener onObserveListener)
            throws InvalidArgsException, SimulatorException {
        nativeStartObserve(null, onObserveListener);
    }

    /**
     * API to start observing the resource.
     *
     * @param queryParams
     *            Map which can have the query parameter names and values.
     * @param onObserveListener
     *            The handler method which will be invoked with a map of
     *            attribute names and values whenever there is a change in
     *            resource model of the remote resource.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if any parameter has invalid
     *             values.
     * @throws SimulatorException
     *             This exception will be thrown for other errors.
     */
    public void observe(Map<String, String> queryParams,
            ObserveNotificationListener onObserveListener)
            throws InvalidArgsException, SimulatorException {
        if (null == queryParams)
            throw new InvalidArgsException(
                    SimulatorResult.SIMULATOR_INVALID_PARAM,
                    "Invalid Query Parameters!");
        nativeStartObserve(queryParams, onObserveListener);
    }

    /**
     * API to stop observing the resource.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if the native remote resource
     *             object is unavailable.
     * @throws SimulatorException
     *             This exception will be thrown for other errors.
     */
    public void stopObserve() throws InvalidArgsException, SimulatorException {
        nativeStopObserve();
    }

    /**
     * API to provide remote resource configure information, which is required
     * for using automation feature.
     *
     * @param path
     *            Path to RAML file.
     *
     * @return Representation {@link SimulatorResourceModel} holding the
     *         representation of the remote resource.
     *
     * @throws InvalidArgsException
     *             Thrown if the RAML configuration file path is invalid.
     * @throws SimulatorException
     *             Thrown for other errors.
     */
    public Map<RequestType, SimulatorRequestModel> setConfigInfo(String path)
            throws InvalidArgsException, SimulatorException {
        return nativeSetConfigInfo(path);
    }

    /**
     * API to send multiple requests for the resource, based on the configure
     * file provided from {@link setConfigInfo}. This verifies response received
     * as well.
     *
     * @param type
     *            Request type to verify.
     * @param onVerifyListener
     *            This event handler will be invoked with the current status of
     *            the automation.
     *
     * @return Automation ID.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if any parameter has invalid
     *             value.
     * @throws NoSupportException
     *             Thrown either if the resource does not support the request
     *             type or the resource is not configured with RAML.
     * @throws OperationInProgressException
     *             Thrown if another request generation session is already in
     *             progress.
     * @throws SimulatorException
     *             This exception will be thrown for other errors.
     */
    public int startVerification(RequestType type,
            VerificationListener onVerifyListener) throws InvalidArgsException,
            NoSupportException, OperationInProgressException,
            SimulatorException {
        return nativeStartAutoRequesting(type, onVerifyListener);
    }

    /**
     * API to stop sending requests which has been started using
     * {@link startVerification}.
     *
     * @param id
     *            Automation ID.
     *
     * @throws InvalidArgsException
     *             Thrown if the automation ID is invalid.
     * @throws SimulatorException
     *             Thrown for other errors.
     */
    public void stopVerification(int id) throws InvalidArgsException,
            SimulatorException {
        nativeStopAutoRequesting(id);
    }

    /**
     * Listener for receiving asynchronous response for GET request.
     */
    public interface GetResponseListener {
        /**
         * Method will be called when response for GET request arrives.
         *
         * @param uid
         *            Unique Id of the resource.
         * @param result
         *            Error code {@link SimulatorResult}.
         * @param resourceModel
         *            {@link SimulatorResourceModel}.
         */
        public void onGetResponse(String uid, SimulatorResult result,
                SimulatorResourceModel resourceModel);
    }

    /**
     * Listener for receiving asynchronous response for PUT request.
     */
    public interface PutResponseListener {
        /**
         * Method will be called when response for PUT request arrives.
         *
         * @param uid
         *            Unique Id of the resource.
         * @param result
         *            Error code {@link SimulatorResult}.
         * @param resourceModel
         *            {@link SimulatorResourceModel}.
         */
        public void onPutResponse(String uid, SimulatorResult result,
                SimulatorResourceModel resourceModel);
    }

    /**
     * Listener for receiving asynchronous response for POST request.
     */
    public interface PostResponseListener {
        /**
         * Method will be called when response for POST request arrives.
         *
         * @param uid
         *            Unique Id of the resource.
         * @param result
         *            Error code {@link SimulatorResult}.
         * @param resourceModel
         *            {@link SimulatorResourceModel}.
         */
        public void onPostResponse(String uid, SimulatorResult result,
                SimulatorResourceModel resourceModel);
    }

    /**
     * Listener for getting asynchronous notification whenever remote resource's
     * representation gets changed.
     */
    public interface ObserveNotificationListener {
        /**
         * This method will be called when there is a change in the resource
         * model of the remote resource.
         *
         * @param uid
         *            Unique Id of the resource.
         * @param resourceModel
         *            {@link SimulatorResourceModel}.
         * @param sequenceNumber
         *            Sequential number for ordering the model change
         *            notifications.
         */
        public void onObserveNotification(String uid,
                SimulatorResourceModel resourceModel, int sequenceNumber);
    }

    /**
     * Listener for receiving the verification session status.
     */
    public interface VerificationListener {
        /**
         * Called when the verification request is accepted and started.
         *
         * @param uid
         *            Unique Id of the resource.
         * @param id
         *            Verification Id.
         */
        public void onVerificationStarted(String uid, int id);

        /**
         * Called when the verification is stopped before its completion.
         *
         * @param uid
         *            Unique Id of the resource.
         * @param id
         *            Verification Id.
         */
        public void onVerificationAborted(String uid, int id);

        /**
         * Called when the verification is done.
         *
         * @param uid
         *            Unique Id of the resource.
         * @param id
         *            Verification Id.
         */
        public void onVerificationCompleted(String uid, int id);
    }

    private SimulatorRemoteResource(long nativeHandle) {
        mNativeHandle = nativeHandle;
    }

    private native void nativeGet(String resourceInterface,
            Map<String, String> queryParamsMap,
            GetResponseListener onGetListener);

    private native void nativePut(String resourceInterface,
            Map<String, String> queryParams,
            SimulatorResourceModel representation,
            PutResponseListener onPutListener);

    private native void nativePost(String resourceInterface,
            Map<String, String> queryParams,
            SimulatorResourceModel representation,
            PostResponseListener onPostListener);

    private native void nativeStartObserve(Map<String, String> queryParams,
            ObserveNotificationListener onObserveListener);

    private native void nativeStopObserve();

    private native Map<RequestType, SimulatorRequestModel> nativeSetConfigInfo(
            String path);

    private native int nativeStartAutoRequesting(RequestType type,
            VerificationListener onVerifyListener);

    private native void nativeStopAutoRequesting(int id);

    private native void nativeDispose();
}
