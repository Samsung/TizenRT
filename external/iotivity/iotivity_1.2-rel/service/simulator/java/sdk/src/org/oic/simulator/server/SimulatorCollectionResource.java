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
import org.oic.simulator.SimulatorException;

/**
 * This class represents a collection type resource which provides a set of APIs
 * that are specific to collection resource for adding/removing child resources
 * and obtaining all child resources from the collection.
 */
public final class SimulatorCollectionResource extends SimulatorResource {

    /**
     * API to add child resource to collection.
     *
     * @param resource
     *            Child resource to be added to collection.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown on invalid input.
     * @throws SimulatorException
     *             This exception will be thrown on occurrence of error in
     *             native.
     */
    public void addChildResource(SimulatorResource resource)
            throws InvalidArgsException, SimulatorException {
        nativeAddChildResource(resource);
    }

    /**
     * API to remove child resource from collection.
     *
     * @param resource
     *            Child resource to be removed from collection.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown on invalid input.
     * @throws SimulatorException
     *             This exception will be thrown on occurrence of error in
     *             native.
     */
    public void removeChildResource(SimulatorResource resource)
            throws InvalidArgsException, SimulatorException {
        nativeRemoveChildResource(resource);
    }

    /**
     * API to remove child resource from collection.
     *
     * @param uri
     *            URI of child resource to be removed from collection.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown on invalid input.
     * @throws SimulatorException
     *             This exception will be thrown on occurrence of error in
     *             native.
     */
    public void removeChildResourceByUri(String uri)
            throws InvalidArgsException, SimulatorException {
        nativeRemoveChildResourceByUri(uri);
    }

    /**
     * API to get list of child resources.
     *
     * @return Vector of child resources {@link SimulatorResource}.
     *
     * @throws SimulatorException
     *             This exception will be thrown on occurrence of error in
     *             native.
     */
    public Vector<SimulatorResource> getChildResources()
            throws SimulatorException {
        return nativeGetChildResources();
    }

    private SimulatorCollectionResource(long nativeHandle) {
        mNativeHandle = nativeHandle;
    }

    private native void nativeAddChildResource(SimulatorResource resource);

    private native void nativeRemoveChildResource(SimulatorResource resource);

    private native void nativeRemoveChildResourceByUri(String uri);

    private native Vector<SimulatorResource> nativeGetChildResources();
}
