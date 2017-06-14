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

package oic.simulator.clientcontroller.view.dialogs;

import org.eclipse.jface.dialogs.TitleAreaDialog;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.MessageBox;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

import java.util.Date;

import org.oic.simulator.ArrayProperty;
import org.oic.simulator.AttributeProperty;
import org.oic.simulator.AttributeValue;
import org.oic.simulator.DoubleProperty;
import org.oic.simulator.ILogger.Level;
import org.oic.simulator.IntegerProperty;
import org.oic.simulator.SimulatorResourceAttribute;
import org.oic.simulator.StringProperty;

import oic.simulator.clientcontroller.Activator;
import oic.simulator.clientcontroller.utils.AttributeValueBuilder;
import oic.simulator.clientcontroller.utils.AttributeValueStringConverter;
import oic.simulator.clientcontroller.utils.Utility;

/**
 * This class manages and shows the automation settings dialog from the
 * attribute view.
 */
public class UpdatePrimitiveArrayAttributeDialog extends TitleAreaDialog {

    private Text                       attNameTxt;
    private Text                       currentValueTxt;
    private Text                       newValueTxt;
    private Text                       allowedValuesTxt;
    private Text                       minRangeTxt;
    private Text                       maxRangeTxt;
    private Text                       allowDuplicatesTxt;
    private Text                       additionalItemsTxt;
    private SimulatorResourceAttribute attribute;
    private String                     newValue;

    private AttributeValue             newAttValueObj;

    public UpdatePrimitiveArrayAttributeDialog(Shell parentShell,
            SimulatorResourceAttribute attribute) {
        super(parentShell);
        this.attribute = attribute;
    }

    @Override
    public void create() {
        super.create();
        setTitle("Modify Attribute's Value");
        setMessage("Change the value of the array type attribute");
    }

    @Override
    protected Control createDialogArea(Composite parent) {
        Composite compLayout = (Composite) super.createDialogArea(parent);

        Composite container = new Composite(compLayout, SWT.NONE);
        container.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));
        GridLayout layout = new GridLayout(2, false);
        layout.verticalSpacing = 10;
        layout.marginTop = 10;
        container.setLayout(layout);

        GridData gd;

        Label attNameLbl = new Label(container, SWT.NONE);
        attNameLbl.setText("Attribute Name");

        attNameTxt = new Text(container, SWT.READ_ONLY | SWT.BORDER);
        attNameTxt.setBackground(container.getBackground());
        gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        attNameTxt.setLayoutData(gd);

        Group subGroup = new Group(container, SWT.NONE);
        subGroup.setText("Array Properties");
        layout = new GridLayout(2, true);
        subGroup.setLayout(layout);
        gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        gd.horizontalSpan = 2;
        subGroup.setLayoutData(gd);

        Composite minRangeContainer = new Composite(subGroup, SWT.NONE);
        gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        minRangeContainer.setLayoutData(gd);
        layout = new GridLayout(2, true);
        minRangeContainer.setLayout(layout);

        Label minRangeLbl = new Label(minRangeContainer, SWT.NONE);
        minRangeLbl.setText("Minimum Items");

        minRangeTxt = new Text(minRangeContainer, SWT.BORDER | SWT.READ_ONLY);
        gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        minRangeTxt.setLayoutData(gd);
        minRangeTxt.setBackground(container.getBackground());

        Composite maxRangeContainer = new Composite(subGroup, SWT.NONE);
        gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        maxRangeContainer.setLayoutData(gd);
        layout = new GridLayout(2, true);
        maxRangeContainer.setLayout(layout);

        Label maxRangeLbl = new Label(maxRangeContainer, SWT.NONE);
        maxRangeLbl.setText("Maximum Items");

        maxRangeTxt = new Text(maxRangeContainer, SWT.BORDER | SWT.READ_ONLY);
        gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        maxRangeTxt.setLayoutData(gd);
        maxRangeTxt.setBackground(container.getBackground());

        Composite uniqueContainer = new Composite(subGroup, SWT.NONE);
        gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        uniqueContainer.setLayoutData(gd);
        layout = new GridLayout(2, true);
        uniqueContainer.setLayout(layout);

        Label uniqueLbl = new Label(uniqueContainer, SWT.NONE);
        uniqueLbl.setText("Allow Duplicates");

        allowDuplicatesTxt = new Text(uniqueContainer, SWT.BORDER
                | SWT.READ_ONLY);
        gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        allowDuplicatesTxt.setLayoutData(gd);
        allowDuplicatesTxt.setBackground(container.getBackground());

        Composite addlItemsContainer = new Composite(subGroup, SWT.NONE);
        gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        addlItemsContainer.setLayoutData(gd);
        layout = new GridLayout(2, true);
        addlItemsContainer.setLayout(layout);

        Label addlItemsLbl = new Label(addlItemsContainer, SWT.NONE);
        addlItemsLbl.setText("Allow Extra Items");

        additionalItemsTxt = new Text(addlItemsContainer, SWT.BORDER
                | SWT.READ_ONLY);
        gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        additionalItemsTxt.setLayoutData(gd);
        additionalItemsTxt.setBackground(container.getBackground());

        Group elementPropGroup = new Group(container, SWT.NONE);
        elementPropGroup.setText("Element Property");
        layout = new GridLayout(2, false);
        elementPropGroup.setLayout(layout);
        gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        gd.horizontalSpan = 2;
        elementPropGroup.setLayoutData(gd);

        Label allowedValuesLbl = new Label(elementPropGroup, SWT.NONE);
        allowedValuesLbl.setText("Allowed Values");

        allowedValuesTxt = new Text(elementPropGroup, SWT.MULTI | SWT.READ_ONLY
                | SWT.BORDER | SWT.WRAP | SWT.V_SCROLL);
        allowedValuesTxt.setBackground(container.getBackground());
        gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        gd.grabExcessVerticalSpace = true;
        gd.verticalAlignment = SWT.FILL;
        gd.minimumHeight = 30;
        allowedValuesTxt.setLayoutData(gd);

        Label curValueLbl = new Label(container, SWT.NONE);
        curValueLbl.setText("Current Value");

        currentValueTxt = new Text(container, SWT.MULTI | SWT.READ_ONLY
                | SWT.BORDER | SWT.WRAP | SWT.V_SCROLL);
        currentValueTxt.setBackground(container.getBackground());
        gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        gd.grabExcessVerticalSpace = true;
        gd.verticalAlignment = SWT.FILL;
        gd.minimumHeight = 30;
        currentValueTxt.setLayoutData(gd);

        Label attNewValue = new Label(container, SWT.NONE);
        attNewValue.setText("New Value");

        newValueTxt = new Text(container, SWT.MULTI | SWT.BORDER | SWT.WRAP
                | SWT.V_SCROLL);
        gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        gd.grabExcessVerticalSpace = true;
        gd.verticalAlignment = SWT.FILL;
        gd.minimumHeight = 60;
        newValueTxt.setLayoutData(gd);
        newValueTxt.setFocus();

        Group hintGroup = new Group(container, SWT.NONE);
        hintGroup.setText("Note");
        layout = new GridLayout();
        hintGroup.setLayout(layout);
        gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        gd.horizontalSpan = 2;
        hintGroup.setLayoutData(gd);

        Label hint = new Label(hintGroup, SWT.NULL);
        hint.setText("Array values should be comma-seperated and surrounded by square brackets.\n"
                + "Ex: \"[value]\", \"[value1,value2]\", \"[[value1], [value2]]\"");
        gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        gd.horizontalSpan = 2;
        gd.horizontalIndent = 40;
        hint.setLayoutData(gd);

        populateData();

        return compLayout;
    }

    private void populateData() {
        // Set the Attribute Name.
        attNameTxt.setText(attribute.name());

        // Set the allowed values.
        ArrayProperty arrProp = attribute.property().asArray();
        AttributeProperty elementProp = arrProp.getElementProperty();

        StringBuilder values = new StringBuilder();
        String value = "";
        if (elementProp.isInteger()) {
            IntegerProperty intProp = elementProp.asInteger();
            if (intProp.hasRange()) {
                value = intProp.min() + " - " + intProp.max();
            } else if (intProp.hasValues()) {
                int[] arr = intProp.getValues();
                for (int i = 0; i < arr.length; i++) {
                    values.append(arr[i]);
                    if (i + 1 < arr.length) {
                        values.append(", ");
                    }
                }

                if (!values.toString().isEmpty()) {
                    value = "[" + values + "]";
                }
            }
        } else if (elementProp.isDouble()) {
            DoubleProperty dblProp = elementProp.asDouble();
            if (dblProp.hasRange()) {
                value = dblProp.min() + " - " + dblProp.max();
            } else if (dblProp.hasValues()) {
                double[] arr = dblProp.getValues();
                for (int i = 0; i < arr.length; i++) {
                    values.append(arr[i]);
                    if (i + 1 < arr.length) {
                        values.append(", ");
                    }
                }

                if (!values.toString().isEmpty()) {
                    value = "[" + values + "]";
                }
            }
        } else if (elementProp.isBoolean()) {
            value = "[true, false]";
        } else if (elementProp.isString()) {
            StringProperty strProp = elementProp.asString();
            if (strProp.hasValues()) {
                String[] arr = strProp.getValues();
                for (int i = 0; i < arr.length; i++) {
                    values.append(arr[i]);
                    if (i + 1 < arr.length) {
                        values.append(", ");
                    }
                }

                if (!values.toString().isEmpty()) {
                    value = "[" + values + "]";
                }
            }
        }

        allowedValuesTxt.setText(value);

        // Set the current value.
        currentValueTxt.setText(new AttributeValueStringConverter(attribute
                .value()).toString());

        minRangeTxt.setText(String.valueOf(arrProp.minItems()));

        maxRangeTxt.setText(String.valueOf(arrProp.maxItems()));

        allowDuplicatesTxt.setText(!arrProp.isUnique() ? "Yes" : "No");

        additionalItemsTxt.setText(arrProp.isVariable() ? "Yes" : "No");
    }

    public String getNewValue() {
        return newValue;
    }

    public AttributeValue getNewValueObj() {
        return newAttValueObj;
    }

    @Override
    protected void okPressed() {
        // Validate the value
        newValue = newValueTxt.getText();
        if (null == newValue || newValue.isEmpty()) {
            return;
        }

        try {
            String value = Utility.removeWhiteSpacesInArrayValues(newValue);
            if (null != value && value.startsWith("[") && value.endsWith("]")) {
                newAttValueObj = AttributeValueBuilder.build(value, attribute
                        .value().typeInfo().mBaseType);
                if (null != newAttValueObj) {
                    close();
                    return;
                }
            }
        } catch (Exception e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(),
                            new Date(),
                            "There is an error while building the new attribute value.\n"
                                    + Utility.getSimulatorErrorString(e, null));
        }

        MessageBox dialog = new MessageBox(Display.getDefault()
                .getActiveShell(), SWT.ICON_ERROR | SWT.OK);
        dialog.setText("Invalid Value");
        dialog.setMessage("Either the value of its format is invalid.\n"
                + "Format: Array values should be comma-seperated and surrounded by square brackets.\n"
                + "Ex: \"[value]\", \"[value1,value2]\", \"[[value1], [value2]]\"");
        dialog.open();

        newValueTxt.setFocus();
    }

    @Override
    protected boolean isResizable() {
        return true;
    }

    @Override
    public boolean isHelpAvailable() {
        return false;
    }
}
