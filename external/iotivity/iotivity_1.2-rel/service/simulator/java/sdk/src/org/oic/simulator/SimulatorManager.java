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

package org.oic.simulator;

import java.util.Vector;

import org.oic.simulator.client.FindResourceListener;
import org.oic.simulator.server.SimulatorResource;

/**
 * This class provides a set of methods for creation, discovery and deletion of
 * resources.
 */
public class SimulatorManager {

    static {
        System.loadLibrary("connectivity_abstraction");
        System.loadLibrary("oc_logger");
        System.loadLibrary("octbstack");
        System.loadLibrary("oc");
        System.loadLibrary("RamlParser");
        System.loadLibrary("SimulatorManager");
    }

    /**
     * API for creating a resource from a RAML configuration file.
     *
     * @param configPath
     *            Path to RAML configuration file.
     *
     * @return {@link SimulatorResource} - Created resource on success,
     *         otherwise null.
     *
     * @throws InvalidArgsException
     *             Thrown if the input parameters are empty.
     * @throws SimulatorException
     *             Thrown for other errors.
     */
    public static SimulatorResource createResource(String configPath)
            throws InvalidArgsException, SimulatorException {
        return nativeCreateResource(configPath);
    }

    /**
     * API for creating a set of resources from a RAML configuration file.
     *
     * @param configPath
     *            Path to RAML configuration file.
     * @param count
     *            Number of resources to be created.
     *
     * @return Returns an array of {@link SimulatorResource} objects one for
     *         each created resource on success, otherwise null.
     *
     * @throws InvalidArgsException
     *             Thrown if the input parameters are empty.
     * @throws SimulatorException
     *             Thrown for other errors.
     */
    public static Vector<SimulatorResource> createResource(String configPath,
            int count) throws InvalidArgsException, SimulatorException {
        return nativeCreateResources(configPath, count);
    };

    /**
     * API for creating a resource either single or collection type.
     *
     * @param type
     *            Indicates whether single or collection type.
     * @param name
     *            Resource Name.
     * @param uri
     *            Resource URI.
     * @param resourceType
     *            Resource Type.
     *
     * @return {@link SimulatorResource} - Created resource on success,
     *         otherwise null.
     *
     * @throws InvalidArgsException
     *             Thrown if the input parameters are empty.
     * @throws SimulatorException
     *             Thrown for other errors.
     */
    public static SimulatorResource createResource(SimulatorResource.Type type,
            String name, String uri, String resourceType)
            throws InvalidArgsException, SimulatorException {
        SimulatorResource resource;
        if (type == SimulatorResource.Type.SINGLE)
            resource = nativeCreateSingleResource(name, uri, resourceType);
        else
            resource = nativeCreateCollectionResource(name, uri, resourceType);
        return resource;
    }

    /**
     * API for discovering all types of resources in the network. Callback is
     * called when a resource is discovered in the network.
     *
     * @param listener
     *            Interface to receive the discovered remote resources.
     *
     * @throws InvalidArgsException
     *             Thrown if the input parameter is empty.
     * @throws SimulatorException
     *             Thrown for other errors.
     */
    public static void findResource(FindResourceListener listener)
            throws InvalidArgsException, SimulatorException {
        nativeSearchResource(null, listener);
    }

    /**
     * API for discovering specific type of resources in the network. Callback
     * is called when a resource is discovered in the network.
     *
     * @param resourceType
     *            Required resource type.
     * @param listener
     *            Interface to receive the discovered remote resources.
     *
     * @throws InvalidArgsException
     *             Thrown if the input parameter is empty.
     * @throws SimulatorException
     *             Thrown for other errors.
     */
    public static void findResource(String resourceType,
            FindResourceListener listener) throws InvalidArgsException,
            SimulatorException {
        if (null == resourceType || resourceType.isEmpty()) {
            throw new InvalidArgsException(
                    SimulatorResult.SIMULATOR_INVALID_PARAM,
                    "Invalid resource type!");
        }

        nativeSearchResource(resourceType, listener);
    }

    /**
     * API to set the device information.
     *
     * @param deviceInfo
     *            Device information.
     *
     * @throws InvalidArgsException
     *             Thrown if the input parameter is empty.
     * @throws SimulatorException
     *             Thrown for other errors.
     */
    public static void setDeviceInfo(String deviceInfo)
            throws InvalidArgsException, SimulatorException {
        nativeSetDeviceInfo(deviceInfo);
    }

    /**
     * API to search for devices on the given host in the network.
     *
     * @param hostUri
     *            URI of the host device.
     * @param listener
     *            Listener for receiving the device information.
     *
     * @throws InvalidArgsException
     *             Thrown if the input parameter is empty.
     * @throws SimulatorException
     *             Thrown for other errors.
     */
    public static void findDevices(String hostUri, DeviceListener listener)
            throws InvalidArgsException, SimulatorException {
        nativeFindDevices(hostUri, listener);
    }

    /**
     * API to set the platform information.
     *
     * @param platformInfo
     *            {@link PlatformInfo} - Platform information.
     *
     * @throws InvalidArgsException
     *             Thrown if the input parameter is empty.
     * @throws SimulatorException
     *             Thrown for other errors.
     */
    public static void setPlatformInfo(PlatformInfo platformInfo)
            throws InvalidArgsException, SimulatorException {
        nativeSetPlatformInfo(platformInfo);
    }

    /**
     * API to find the platform information of the given host in the network.
     *
     * @param hostUri
     *            URI of the host device.
     * @param listener
     *            Listener for receiving the platform information.
     *
     * @throws InvalidArgsException
     *             Thrown if the input parameter is empty.
     * @throws SimulatorException
     *             Thrown for other errors.
     */
    public static void getPlatformInformation(String hostUri,
            PlatformListener listener) throws InvalidArgsException,
            SimulatorException {
        nativeGetPlatformInformation(hostUri, listener);
    }

    /**
     * API to set the listener for receiving log messages.
     *
     * @param logger
     *            {@link ILogger} to receive the log messages.
     */
    public static void setLogger(ILogger logger) {
        nativeSetLogger(logger);
    }

    private SimulatorManager() {
    }

    private static native SimulatorResource nativeCreateResource(
            String configPath);

    private static native Vector<SimulatorResource> nativeCreateResources(
            String configPath, int count);

    private static native SimulatorResource nativeCreateSingleResource(
            String name, String uri, String resourceType);

    private static native SimulatorResource nativeCreateCollectionResource(
            String name, String uri, String resourceType);

    private static native void nativeSearchResource(String resourceType,
            FindResourceListener listener);

    private static native void nativeSetDeviceInfo(String deviceInfo);

    private static native void nativeFindDevices(String hostUri,
            DeviceListener listener);

    private static native void nativeSetPlatformInfo(PlatformInfo platformInfo);

    private static native void nativeGetPlatformInformation(String hostUri,
            PlatformListener listener);

    private static native void nativeSetLogger(ILogger logger);
}