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

import java.util.Map;

import org.oic.simulator.AttributeValue;
import org.oic.simulator.InvalidArgsException;
import org.oic.simulator.SimulatorException;
import org.oic.simulator.SimulatorResourceAttribute;

/**
 * This class represents a single type resource(Non-collection resource). It
 * provides APIs specific to single resource for manipulating the resource model
 * by adding/removing the attributes, and updating the attribute values manually
 * and automatically.
 */
public final class SimulatorSingleResource extends SimulatorResource {

    /**
     * API to get attribute of resource.
     *
     * @param attrName
     *            Name of the attribute
     *
     * @return An object of {@link SimulatorResourceAttribute}, or null if
     *         resource does not have attribute of this name.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if the attribute name is
     *             invalid.
     * @throws SimulatorException
     *             This exception will be thrown either if the resource model is
     *             not found or for some general errors.
     */
    public SimulatorResourceAttribute getAttribute(String attrName)
            throws InvalidArgsException, SimulatorException {
        return nativeGetAttribute(attrName);
    }

    /**
     * API to get all the attributes of the resource.
     *
     * @return Map of attributes with attribute name as key and its
     *         corresponding {@link SimulatorResourceAttribute} as value.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown if the attribute name is
     *             invalid.
     * @throws SimulatorException
     *             This exception will be thrown either if the resource model is
     *             not found or for some general errors.
     */
    public Map<String, SimulatorResourceAttribute> getAttributes()
            throws InvalidArgsException, SimulatorException {
        return nativeGetAttributes();
    }

    /**
     * API to add an attribute to resource's representation model. Observers
     * will be notified on success.
     *
     * @param attribute
     *            Attribute to be added to resource's representation model.
     *
     * @return True if the attribute is added properly.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown on invalid input.
     * @throws SimulatorException
     *             This exception will be thrown for other errors.
     */
    public boolean addAttribute(SimulatorResourceAttribute attribute)
            throws InvalidArgsException, SimulatorException {
        return nativeAddAttribute(attribute);
    }

    /**
     * API to remove an attribute from the simulated resource. Observers will be
     * notified on success.
     *
     * @param attrName
     *            Name of the attribute to be deleted.
     *
     * @return True if the attribute is removed properly.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown on invalid input.
     * @throws SimulatorException
     *             This exception will be thrown for other errors.
     */
    public boolean removeAttribute(String attrName)
            throws InvalidArgsException, SimulatorException {
        return nativeRemoveAttribute(attrName);
    }

    /**
     * API to update the value of an attribute. Observers will be notified on
     * success.
     *
     * @param attrName
     *            Name of the attribute.
     * @param value
     *            New value of the attribute.
     *
     * @return True if the attribute's value is updated properly.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown on invalid input.
     * @throws SimulatorException
     *             This exception will be thrown for other errors.
     */
    public boolean updateAttribute(String attrName, AttributeValue value)
            throws InvalidArgsException, SimulatorException {
        return nativeUpdateAttribute(attrName, value);
    }

    /**
     * API to start the resource level automation. This automation involves
     * automatically updating all the possible values for all the attributes
     * sequentially.
     *
     * @param type
     *            {@link AutoUpdateType} indicating whether the automation is
     *            one-time or recursive.
     * @param interval
     *            Interval time in milliseconds.
     * @param listener
     *            Listener to be notified when automation ends.
     *
     * @return Automation ID using which the automation can be stopped.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown on invalid input.
     * @throws SimulatorException
     *             This exception will be thrown for other errors.
     */
    public int startResourceUpdation(AutoUpdateType type, int interval,
            AutoUpdateListener listener) throws InvalidArgsException,
            SimulatorException {
        return nativeStartResourceUpdation(type, interval, listener);
    }

    /**
     * API to start the attribute level automation. This automation involves
     * automatically updating all the possible values for a given attribute
     * sequentially.
     *
     * @param attrName
     *            Name of the attribute to be automated.
     * @param type
     *            {@link AutoUpdateType} indicating whether the automation is
     *            one-time or recursive.
     * @param interval
     *            Interval time in milliseconds.
     * @param listener
     *            Listener to be notified when automation ends.
     *
     * @return Automation ID using which the automation can be stopped.
     *
     * @throws InvalidArgsException
     *             This exception will be thrown on invalid input.
     * @throws SimulatorException
     *             This exception will be thrown for other errors.
     */
    public int startAttributeUpdation(String attrName, AutoUpdateType type,
            int interval, AutoUpdateListener listener)
            throws InvalidArgsException, SimulatorException {
        return nativeStartAttributeUpdation(attrName, type, interval, listener);
    }

    /**
     * API to stop the automation based on automation id.
     *
     * @param id
     *            Using which a specific automation can be stopped.
     *
     * @throws SimulatorException
     *             This exception will be thrown for general errors.
     */
    public void stopUpdation(int id) throws SimulatorException {
        nativeStopUpdation(id);
    }

    private SimulatorSingleResource(long nativeHandle) {
        mNativeHandle = nativeHandle;
    }

    private native SimulatorResourceAttribute nativeGetAttribute(String attrName);

    private native Map<String, SimulatorResourceAttribute> nativeGetAttributes();

    private native boolean nativeAddAttribute(
            SimulatorResourceAttribute attribute);

    private native boolean nativeRemoveAttribute(String attrName);

    private native boolean nativeUpdateAttribute(String attrName,
            AttributeValue value);

    private native int nativeStartResourceUpdation(AutoUpdateType type,
            int interval, AutoUpdateListener listener);

    private native int nativeStartAttributeUpdation(String attrName,
            AutoUpdateType type, int interval, AutoUpdateListener listener);

    private native void nativeStopUpdation(int id);
}
