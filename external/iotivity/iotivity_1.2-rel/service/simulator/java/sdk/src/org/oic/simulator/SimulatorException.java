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
 * Base class for simulator exceptions that will be thrown to indicate general
 * exceptions.
 */
@SuppressWarnings("serial")
public class SimulatorException extends Exception {

    private SimulatorResult code;
    private String          message;

    /**
     * Constructs {@link SimulatorException} with the given ordinal representing
     * the error code and description.
     *
     * @param code
     *            Ordinal of an error code.
     * @param message
     *            Error description.
     */
    public SimulatorException(int code, String message) {
        this.code = SimulatorResult.get(code);
        this.message = message;
    }

    /**
     * Constructs {@link SimulatorException} with the given error code and
     * description.
     *
     * @param code
     *            {@link SimulatorResult} enum constant representing the error
     *            code.
     * @param message
     *            Error description.
     */
    public SimulatorException(SimulatorResult code, String message) {
        this.code = code;
        this.message = message;
    }

    /**
     * API to get the error code.
     *
     * @return {@link SimulatorResult} enum constant representing the error
     *         code.
     */
    public SimulatorResult code() {
        return code;
    }

    /**
     * API to get the error description.
     *
     * @return Error description.
     */
    public String message() {
        return message;
    }
}