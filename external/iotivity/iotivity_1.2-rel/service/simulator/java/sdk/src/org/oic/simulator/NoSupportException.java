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

/**
 * Class for representing unsupported operations exception.
 */
@SuppressWarnings("serial")
public class NoSupportException extends SimulatorException {

    /**
     * Constructs {@link NoSupportException} with the given error message.
     *
     * @param message
     *            Error description.
     */
    public NoSupportException(String message) {
        super(SimulatorResult.SIMULATOR_NOT_SUPPORTED, message);
    }

    /**
     * Constructs {@link NoSupportException} with the given error code and
     * description.
     *
     * @param code
     *            {@link SimulatorResult} enum constant representing the error
     *            code.
     * @param message
     *            Error description.
     */
    public NoSupportException(SimulatorResult code, String message) {
        super(code, message);
    }
}