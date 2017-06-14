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

package org.oic.simulator.server;

import java.util.Vector;

import org.oic.simulator.InvalidArgsException;
import org.oic.simulator.NoSupportException;
import org.oic.simulator.SimulatorException;
import org.oic.simulator.SimulatorResourceModel;

/**
 * This class serves as a base class type for all the resources in the
 * simulator. It provides APIs to get and update the resource details, and
 * register listeners to get notifications for resource model changes and
 * observe requests from clients, and APIs to register/ unregister the resource
 * with the platform.
 */
public class SimulatorResource {

    protected long mNativeHandle;

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
     * Enum to represent the type of resource.
     */
    public enum Type {
        SINGLE, COLLECTION
    }

    /**
     * Enum to represent the update type of automation.
     */
    public enum AutoUpdateType {
        ONE_TIME, REPEAT
    }

    /**
     * API which indicates whether the resource is collection or single
     * resource.
     *
     * @return True if the resource is collection, otherwise false.
     */
    public boolean isCollection() {
        return (this instanceof SimulatorCollectionResource);
    }

    /**
     * API to get the type which indicates whether resource is single or
     * collection resource.
     *
     * @return Type of resource.
     */
    public Type getType() {
        if (this instanceof SimulatorSingleResource) {
            return Type.SINGLE;
        } else {
            return Type.COLLECTION;
        }
    }

    /**
     * API to get the name of the resource.
     *
     * @return Name of the resource.
     *
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or for some general errors.
     */
    public String getName() throws SimulatorException {
        return nativeGetName();
    }

    /**
     * API to get the resource URI.
     *
     * @return Resource URI.
     *
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or for some general errors.
     */
    public String getURI() throws SimulatorException {
        return nativeGetURI();
    }

    /**
     * API to get the resource type.
     *
     * @return Resource type.
     *
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or for some general errors.
     */
    public String getResourceType() throws SimulatorException {
        return nativeGetResourceType();
    }

    /**
     * API to get the interfaces resource is bound with.
     *
     * @return Interface types.
     *
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or for some general errors.
     */
    public Vector<String> getInterface() throws SimulatorException {
        return nativeGetInterface();
    }

    /**
     * API to get the observable state of resource.
     *
     * @return Observable state - true if resource is observable, otherwise
     *         false.
     *
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or for some general errors.
     */
    public boolean isObservable() throws SimulatorException {
        return nativeIsObservable();
    }

    /**
     * API to get the discoverable state of resource.
     *
     * @return Discoverable state - true if resource is discoverable, otherwise
     *         false.
     *
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or for some general errors.
     */
    public boolean isDiscoverable() throws SimulatorException {
        return nativeIsDiscoverable();
    }

    /**
     * API to get the start state of resource.
     *
     * @return Start state - true if resource is started, otherwise false.
     *
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or for some general errors.
     */
    public boolean isStarted() throws SimulatorException {
        return nativeIsStarted();
    }

    /**
     * API to set the name of the resource.
     *
     * @param name
     *            Name to be set.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if the resource name is
     *             invalid.
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or for some general errors.
     */
    public void setName(String name) throws InvalidArgsException,
            SimulatorException {
        nativeSetName(name);
    }

    /**
     * API to set the resource URI.
     *
     * @param uri
     *            URI to be set.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if the resource URI is invalid.
     * @throws SimulatorException
     *             A resource needs to be stopped by calling the stop() before
     *             calling this API. This exception will be thrown if the native
     *             resource object has not yet been stopped.
     */
    public void setURI(String uri) throws InvalidArgsException,
            SimulatorException {
        nativeSetURI(uri);
    }

    /**
     * API to set the resource type.
     *
     * @param resourceType
     *            Resource type string.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if the resource type is
     *             invalid.
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or for some general errors.
     */
    public void setResourceType(String resourceType)
            throws InvalidArgsException, SimulatorException {
        nativeSetResourceType(resourceType);
    }

    /**
     * API to set interface to resource. Resource should be stopped before
     * calling this API.
     *
     * @param interfaceType
     *            Interface to be set.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if the interface type is
     *             invalid.
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or resource is still running or for some
     *             general errors.
     */
    public void setInterface(String interfaceType) throws InvalidArgsException,
            SimulatorException {
        nativeSetInterface(interfaceType);
    }

    /**
     * API to set a list of interfaces to resource. Resource should be stopped
     * before calling this API.
     *
     * @param interfaceTypes
     *            Interfaces to be set.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if the interface type is
     *             invalid.
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or resource is still running or for some
     *             general errors.
     */
    public void setInterface(Vector<String> interfaceTypes)
            throws InvalidArgsException, SimulatorException {
        nativeSetInterfaces(interfaceTypes);
    }

    /**
     * API to add interface type for resource.
     *
     * @param interfaceType
     *            Interface to be added for resource.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if the interface type is
     *             invalid.
     * @throws NoSupportException
     *             This exception will be thrown if the interface type is not
     *             supported by the resource.
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or for some general errors.
     */
    public void addInterface(String interfaceType) throws InvalidArgsException,
            NoSupportException, SimulatorException {
        nativeAddInterface(interfaceType);
    }

    /**
     * API to make the resource observable or not observable.
     *
     * @param state
     *            True makes the resource observable, otherwise non-observable.
     *
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or for some general errors.
     */
    public void setObservable(boolean state) throws SimulatorException {
        nativeSetObservable(state);
    }

    /**
     * API to make the resource discoverable or not discoverable.
     *
     * @param state
     *            True makes the resource discoverable, otherwise
     *            non-discoverable.
     *
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or for some general errors.
     */
    public void setDiscoverable(boolean state) throws SimulatorException {
        nativeSetDiscoverable(state);
    }

    /**
     * API to set the listener for receiving the notifications when observer is
     * registered or unregistered with resource.
     *
     * @param listener
     *            Callback to be set for receiving the notifications.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if the listener is invalid.
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or for some general errors.
     */
    public void setObserverListener(ObserverListener listener)
            throws InvalidArgsException, SimulatorException {
        nativeSetObserverListener(listener);
    }

    /**
     * API to set listener for receiving notifications when resource's model
     * gets changed.
     *
     * @param listener
     *            {@link ResourceModelChangeListener}.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown on invalid input.
     * @throws SimulatorException
     *             This exception will be thrown for other errors.
     */
    public void setResourceModelChangeListener(
            ResourceModelChangeListener listener) throws InvalidArgsException,
            SimulatorException {
        nativeSetResourceModelChangeListener(listener);
    }

    /**
     * API to start(register) the resource.
     *
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or for some general errors.
     */
    public void start() throws SimulatorException {
        nativeStart();
    }

    /**
     * API to stop(unregister) the resource.
     *
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or for some general errors.
     */
    public void stop() throws SimulatorException {
        nativeStop();
    }

    /**
     * API to get the {@link SimulatorResourceModel} of the simulated resource.
     *
     * @return {@link SimulatorResourceModel} object on success, otherwise null.
     *
     * @throws SimulatorException
     *             This exception will be thrown if simulated resource is not
     *             proper.
     */
    public SimulatorResourceModel getResourceModel() throws SimulatorException {
        return nativeGetResourceModel();
    }

    /**
     * API to get observers which are registered with resource.
     *
     * @return observers as an array of {@link Observer}.
     *
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or for some general errors.
     */
    public Vector<Observer> getObservers() throws SimulatorException {
        return nativeGetObservers();
    }

    /**
     * API to notify current resource model to a specific observer.
     *
     * @param observerId
     *            Observer ID to notify.
     *
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or for some general errors.
     */
    public void notifyObserver(int observerId) throws SimulatorException {
        nativeNotify(observerId);
    }

    /**
     * API to notify current resource model to all registered observers.
     *
     * @throws SimulatorException
     *             This exception will be thrown if the native resource object
     *             does not exist or for some general errors.
     */
    public void notifyAllObservers() throws SimulatorException {
        nativeNotifyAll();
    }

    /**
     * Listener for receiving notification when observer is registered or
     * unregistered with the resource.
     */
    public interface ObserverListener {
        /**
         * Method will be invoked when an observer is registered with resource.
         *
         * @param resourceURI
         *            URI of the resource.
         * @param observer
         *            {@link Observer} object containing the details of
         *            observer.
         */
        public void onObserverAdded(String resourceURI, Observer observer);

        /**
         * Method will be invoked when an observer is unregistered from the
         * resource.
         *
         * @param resourceURI
         *            URI of the resource.
         * @param observer
         *            {@link Observer} object containing the details of
         *            observer.
         */
        public void onObserverRemoved(String resourceURI, Observer observer);
    }

    /**
     * Listener for receiving notification on completion of automatically
     * updating attribute value from its range or value set property.
     */
    public interface AutoUpdateListener {
        /**
         * Method for receiving automation complete notifications.
         *
         * @param uri
         *            URI of resource.
         * @param id
         *            Identifier for auto resource/attribute update session.
         */
        public void onUpdateComplete(String uri, int id);
    }

    /**
     * Listener for receiving notifications whenever there is a change in the
     * resource model.
     */
    public interface ResourceModelChangeListener {
        /**
         * Method will be invoked to notify about the changes in the resource
         * model.
         *
         * @param uri
         *            URI of resource.
         * @param resourceModel
         *            {@link SimulatorResourceModel} of the resource.
         */
        public void onResourceModelChanged(String uri,
                SimulatorResourceModel resourceModel);
    }

    protected SimulatorResource() {
    }

    private native String nativeGetName();

    private native String nativeGetURI();

    private native String nativeGetResourceType();

    private native Vector<String> nativeGetInterface();

    private native boolean nativeIsObservable();

    private native boolean nativeIsDiscoverable();

    private native boolean nativeIsStarted();

    private native void nativeSetName(String name);

    private native void nativeSetURI(String uri);

    private native void nativeSetResourceType(String resourceType);

    private native void nativeSetInterface(String interfaceType);

    private native void nativeSetInterfaces(Vector<String> interfaceTypes);

    private native void nativeAddInterface(String interfaceType);

    private native void nativeSetObservable(boolean state);

    private native void nativeSetDiscoverable(boolean state);

    private native void nativeSetObserverListener(ObserverListener listener);

    private native void nativeSetResourceModelChangeListener(
            ResourceModelChangeListener listener);

    private native void nativeStart();

    private native void nativeStop();

    private native SimulatorResourceModel nativeGetResourceModel();

    private native Vector<Observer> nativeGetObservers();

    private native void nativeNotify(int observerId);

    private native void nativeNotifyAll();

    private native void nativeDispose();
}
