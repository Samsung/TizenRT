/*
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

import org.oic.simulator.ModelProperty;
import org.oic.simulator.client.SimulatorRemoteResource.RequestType;

public class SimulatorRequestModel {
    private RequestType                 mType         = RequestType.UKNOWN;
    private Map<String, Vector<String>> mQueryParams  = null;
    private ModelProperty               mReqBodyModel = null;

    public SimulatorRequestModel(RequestType type) {
        mType = type;
    }

    public SimulatorRequestModel(RequestType type,
            Map<String, Vector<String>> queryParams) {
        mType = type;
        mQueryParams = queryParams;
    }

    public SimulatorRequestModel(RequestType type,
            Map<String, Vector<String>> queryParams, ModelProperty reqBodyModel) {
        mType = type;
        mQueryParams = queryParams;
        mReqBodyModel = reqBodyModel;
    }

    public RequestType getType() {
        return mType;
    }

    public Map<String, Vector<String>> getQueryParams() {
        return mQueryParams;
    }

    public ModelProperty getRequestBodyModel() {
        return mReqBodyModel;
    }
}
