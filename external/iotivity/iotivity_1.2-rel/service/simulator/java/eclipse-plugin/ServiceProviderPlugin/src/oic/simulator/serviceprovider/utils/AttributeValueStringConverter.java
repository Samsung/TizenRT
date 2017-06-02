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

package oic.simulator.serviceprovider.utils;

import java.util.Map;

import org.oic.simulator.AttributeValue;
import org.oic.simulator.AttributeValueVisitor;
import org.oic.simulator.SimulatorResourceModel;

public class AttributeValueStringConverter implements
        AttributeValueVisitor.VisitingMethods<String> {

    private AttributeValue mValue;

    public AttributeValueStringConverter(AttributeValue value) {
        mValue = value;
    }

    @Override
    public String toString() {
        AttributeValueVisitor visitor = new AttributeValueVisitor(mValue, this);
        return (String) visitor.visit();
    }

    @Override
    public String visitingValue(Integer value) {
        return value.toString();
    }

    @Override
    public String visitingValue(Double value) {
        return value.toString();
    }

    @Override
    public String visitingValue(Boolean value) {
        return value.toString();
    }

    @Override
    public String visitingValue(String value) {
        return value;
    }

    @Override
    public String visitingValue(SimulatorResourceModel value) {
        StringBuilder result = new StringBuilder();
        result.append("{");
        boolean first = true;
        for (Map.Entry<String, AttributeValue> entry : value.get().entrySet()) {
            if (!first)
                result.append(", ");
            first = false;
            result.append("\"" + entry.getKey() + "\":");
            AttributeValue attributeValue = entry.getValue();
            result.append(new AttributeValueStringConverter(attributeValue)
                    .toString());
        }

        result.append("}");
        return result.toString();
    }

    @Override
    public String visitingValue(Integer[] values) {
        StringBuilder result = new StringBuilder();
        result.append("[");
        boolean first = true;
        for (Integer value : values) {
            if (!first)
                result.append(", ");
            first = false;
            result.append(value.toString());
        }

        result.append("]");
        return result.toString();
    }

    @Override
    public String visitingValue(Double[] values) {
        StringBuilder result = new StringBuilder();
        result.append("[");
        boolean first = true;
        for (Double value : values) {
            if (!first)
                result.append(", ");
            first = false;
            result.append(value.toString());
        }

        result.append("]");
        return result.toString();
    }

    @Override
    public String visitingValue(Boolean[] values) {
        StringBuilder result = new StringBuilder();
        result.append("[");
        boolean first = true;
        for (Boolean value : values) {
            if (!first)
                result.append(", ");
            first = false;
            result.append(value.toString());
        }

        result.append("]");
        return result.toString();
    }

    @Override
    public String visitingValue(String[] values) {
        StringBuilder result = new StringBuilder();
        result.append("[");
        boolean first = true;
        for (String value : values) {
            if (!first)
                result.append(", ");
            first = false;
            result.append(value);
        }

        result.append("]");
        return result.toString();
    }

    @Override
    public String visitingValue(SimulatorResourceModel[] values) {
        StringBuilder result = new StringBuilder();
        result.append("[");
        boolean first = true;
        for (SimulatorResourceModel value : values) {
            if (!first)
                result.append(", ");
            first = false;
            AttributeValue attributeValue = new AttributeValue(value);
            result.append(new AttributeValueStringConverter(attributeValue)
                    .toString());
        }

        result.append("]");
        return result.toString();
    }

    @Override
    public String visitingValue(Integer[][] values) {
        StringBuilder result = new StringBuilder();
        result.append("[");
        boolean first = true;
        for (Integer[] value : values) {
            if (!first)
                result.append(", ");
            first = false;
            AttributeValue attributeValue = new AttributeValue(value);
            result.append(new AttributeValueStringConverter(attributeValue)
                    .toString());
        }

        result.append("]");
        return result.toString();
    }

    @Override
    public String visitingValue(Double[][] values) {
        StringBuilder result = new StringBuilder();
        result.append("[");
        boolean first = true;
        for (Double[] value : values) {
            if (!first)
                result.append(", ");
            first = false;
            AttributeValue attributeValue = new AttributeValue(value);
            result.append(new AttributeValueStringConverter(attributeValue)
                    .toString());
        }

        result.append("]");
        return result.toString();
    }

    @Override
    public String visitingValue(Boolean[][] values) {
        StringBuilder result = new StringBuilder();
        result.append("[");
        boolean first = true;
        for (Boolean[] value : values) {
            if (!first)
                result.append(", ");
            first = false;
            AttributeValue attributeValue = new AttributeValue(value);
            result.append(new AttributeValueStringConverter(attributeValue)
                    .toString());
        }

        result.append("]");
        return result.toString();
    }

    @Override
    public String visitingValue(String[][] values) {
        StringBuilder result = new StringBuilder();
        result.append("[");
        boolean first = true;
        for (String[] value : values) {
            if (!first)
                result.append(", ");
            first = false;
            AttributeValue attributeValue = new AttributeValue(value);
            result.append(new AttributeValueStringConverter(attributeValue)
                    .toString());
        }

        result.append("]");
        return result.toString();
    }

    @Override
    public String visitingValue(SimulatorResourceModel[][] values) {
        StringBuilder result = new StringBuilder();
        result.append("[");
        boolean first = true;
        for (SimulatorResourceModel[] value : values) {
            if (!first)
                result.append(", ");
            first = false;
            AttributeValue attributeValue = new AttributeValue(value);
            result.append(new AttributeValueStringConverter(attributeValue)
                    .toString());
        }

        result.append("]");
        return result.toString();
    }

    @Override
    public String visitingValue(Integer[][][] values) {
        StringBuilder result = new StringBuilder();
        result.append("[");
        boolean first = true;
        for (Integer[][] value : values) {
            if (!first)
                result.append(", ");
            first = false;
            AttributeValue attributeValue = new AttributeValue(value);
            result.append(new AttributeValueStringConverter(attributeValue)
                    .toString());
        }

        result.append("]");
        return result.toString();
    }

    @Override
    public String visitingValue(Double[][][] values) {
        StringBuilder result = new StringBuilder();
        result.append("[");
        boolean first = true;
        for (Double[][] value : values) {
            if (!first)
                result.append(", ");
            first = false;
            AttributeValue attributeValue = new AttributeValue(value);
            result.append(new AttributeValueStringConverter(attributeValue)
                    .toString());
        }

        result.append("]");
        return result.toString();
    }

    @Override
    public String visitingValue(Boolean[][][] values) {
        StringBuilder result = new StringBuilder();
        result.append("[");
        boolean first = true;
        for (Boolean[][] value : values) {
            if (!first)
                result.append(", ");
            first = false;
            AttributeValue attributeValue = new AttributeValue(value);
            result.append(new AttributeValueStringConverter(attributeValue)
                    .toString());
        }

        result.append("]");
        return result.toString();
    }

    @Override
    public String visitingValue(String[][][] values) {
        StringBuilder result = new StringBuilder();
        result.append("[");
        boolean first = true;
        for (String[][] value : values) {
            if (!first)
                result.append(", ");
            first = false;
            AttributeValue attributeValue = new AttributeValue(value);
            result.append(new AttributeValueStringConverter(attributeValue)
                    .toString());
        }

        result.append("]");
        return result.toString();
    }

    @Override
    public String visitingValue(SimulatorResourceModel[][][] values) {
        StringBuilder result = new StringBuilder();
        result.append("[");
        boolean first = true;
        for (SimulatorResourceModel[][] value : values) {
            if (!first)
                result.append(", ");
            first = false;
            AttributeValue attributeValue = new AttributeValue(value);
            result.append(new AttributeValueStringConverter(attributeValue)
                    .toString());
        }

        result.append("]");
        return result.toString();
    }

}
