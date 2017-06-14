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

package oic.simulator.clientcontroller.remoteresource;

import java.util.HashMap;
import java.util.Map;

import org.oic.simulator.AttributeValue;
import org.oic.simulator.InvalidArgsException;
import org.oic.simulator.ModelProperty;
import org.oic.simulator.SimulatorResourceAttribute;
import org.oic.simulator.SimulatorResourceModel;
import org.oic.simulator.client.SimulatorRequestModel;

public class ResourceRepresentation {
    private Map<String, AttributeElement> mAttributes = new HashMap<String, AttributeElement>();

    public ResourceRepresentation(SimulatorResourceModel resourceModel) {
        if (resourceModel != null && resourceModel.size() > 0) {
            for (Map.Entry<String, AttributeValue> entry : resourceModel.get()
                    .entrySet())
                if (isCoreAttribute(entry.getKey())) {
                    mAttributes.put(entry.getKey(), new AttributeElement(this,
                            new SimulatorResourceAttribute(entry.getKey(),
                                    entry.getValue())));
                }
        }
    }

    public ResourceRepresentation(
            Map<String, SimulatorResourceAttribute> attributes,
            boolean onlyCoreAttributes) {
        if (attributes != null && attributes.size() > 0) {
            for (Map.Entry<String, SimulatorResourceAttribute> entry : attributes
                    .entrySet()) {
                if (onlyCoreAttributes && !isCoreAttribute(entry.getKey())) {
                    // Skip this attribute.
                    continue;
                }

                mAttributes.put(entry.getKey(), new AttributeElement(this,
                        entry.getValue()));
            }
        }
    }

    private boolean isCoreAttribute(String attName) {
        if (null == attName || attName.isEmpty()) {
            return false;
        }

        if (attName.equalsIgnoreCase("if") || attName.equalsIgnoreCase("rt")
                || attName.equalsIgnoreCase("p")
                || attName.equalsIgnoreCase("n")
                || attName.equalsIgnoreCase("id")) {
            return false;
        }

        return true;
    }

    public Map<String, AttributeElement> getAttributes() {
        return mAttributes;
    }

    public boolean updateAttributeProperties(
            SimulatorRequestModel requestModel,
            SimulatorResourceModel resourceModelRef)
            throws NullPointerException {
        if (null == requestModel || null == resourceModelRef) {
            return false;
        }

        ModelProperty modelProp = requestModel.getRequestBodyModel();
        if (null == modelProp) {
            return false;
        }

        for (Map.Entry<String, AttributeValue> entry : resourceModelRef.get()
                .entrySet()) {
            if (isCoreAttribute(entry.getKey())) {
                AttributeElement attributeElement = mAttributes.get(entry
                        .getKey());
                if (attributeElement != null) {
                    attributeElement
                            .setAttributeProperty(new SimulatorResourceAttribute(
                                    entry.getKey(), entry.getValue(), modelProp
                                            .get(entry.getKey())));
                }
            }
        }

        return true;
    }

    public SimulatorResourceModel getModel() {
        if (null == mAttributes || mAttributes.isEmpty()) {
            return null;
        }
        SimulatorResourceModel model = new SimulatorResourceModel();
        for (Map.Entry<String, AttributeElement> entry : mAttributes.entrySet()) {
            AttributeElement attributeElement = mAttributes.get(entry.getKey());
            if (attributeElement != null) {
                try {
                    model.set(entry.getKey(), attributeElement
                            .getSimulatorResourceAttribute().value());
                } catch (InvalidArgsException e) {
                    e.printStackTrace();
                }
            }
        }
        return model;
    }

    public SimulatorResourceModel getSelectedModel() {
        if (null == mAttributes || mAttributes.isEmpty()) {
            return null;
        }
        SimulatorResourceModel model = new SimulatorResourceModel();
        for (Map.Entry<String, AttributeElement> entry : mAttributes.entrySet()) {
            AttributeElement attributeElement = mAttributes.get(entry.getKey());
            if (attributeElement != null && attributeElement.getPostState()) {
                try {
                    model.set(entry.getKey(), attributeElement
                            .getSimulatorResourceAttribute().value());
                } catch (InvalidArgsException e) {
                    e.printStackTrace();
                }
            }
        }
        return model;
    }
}