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

package oic.simulator.serviceprovider.view.dialogs;

import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.dialogs.TitleAreaDialog;
import org.eclipse.jface.window.Window;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.CCombo;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.List;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

import java.util.Iterator;
import java.util.Set;

import oic.simulator.serviceprovider.model.AttributeHelper;
import oic.simulator.serviceprovider.model.AttributeHelper.ValidValuesType;
import oic.simulator.serviceprovider.utils.Constants;

public class AddAttributeDialog extends TitleAreaDialog {

    private Text                 attNameTxt;
    private Text                 minRangeTxt;
    private Text                 maxRangeTxt;
    private CCombo               attTypeCmb;
    private Text                 dflValueTxt;
    private Button               rangeBtn;
    private Button               cusValuesBtn;
    private Button               noneBtn;
    private Button               addBtn;
    private Button               remBtn;
    private Label                minLbl;
    private Label                maxLbl;
    private List                 customValuesList;
    private Text                 detail;

    private AttributeHelper      attHelper;

    private AttributeHelper      attClone;

    private Set<AttributeHelper> attributes;

    private static final String  defaultMessage   = "Name, Type, and Default Value fields "
                                                          + "are mandatory.\n\nRange and custom fields allow to set the valid "
                                                          + "values of the attribute.\n\n";
    private static final String  msgForBoolType   = "Possible attribute values of Bool are "
                                                          + "true and false.\nSo range and custom options are disabled.";
    private static final String  msgForIntType    = "Valid values for Int type can either be "
                                                          + "of range type (Ex: 1 - 10) or custom values (Ex: 10, 20, 50, and 100).\n";
    private static final String  msgForDoubleType = "Valid values for Double type can either be "
                                                          + "of range type (Ex: 18.0 - 22.0) or custom values (Ex: 1.5, 2.5, 3.9, 4.8, etc).\n";
    private static final String  msgForStringType = "For String type, range option is not"
                                                          + "applicable. Hence it is disabled.\n\n"
                                                          + "Custom option is available to provide the valid values.\n\n"
                                                          + "Ex: low, mid, high, etc.";

    private Set<String>          attValueTypes;

    private boolean              editOperation;

    public AddAttributeDialog(Shell parentShell, AttributeHelper att,
            Set<String> attValueTypes, Set<AttributeHelper> attributes) {
        super(parentShell);
        if (null == att) {
            att = new AttributeHelper();
        } else {
            attClone = att.clone();
            editOperation = true;
        }
        attHelper = att;
        this.attValueTypes = attValueTypes;
        this.attributes = attributes;
    }

    @Override
    public void create() {
        super.create();
        setTitle("Add Attribute");
        setMessage("Fill the details for creating an attribute");
    }

    @Override
    protected Control createDialogArea(Composite parent) {
        Composite compLayout = (Composite) super.createDialogArea(parent);
        Composite container = new Composite(compLayout, SWT.NONE);
        container.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));
        GridLayout layout = new GridLayout(5, false);
        container.setLayout(layout);

        Label attNameLbl = new Label(container, SWT.NULL);
        attNameLbl.setText("Attribute Name:");

        attNameTxt = new Text(container, SWT.BORDER);
        GridData gd = new GridData();
        gd.horizontalAlignment = SWT.FILL;
        gd.horizontalSpan = 4;
        gd.grabExcessHorizontalSpace = true;
        attNameTxt.setLayoutData(gd);

        Label attTypeLbl = new Label(container, SWT.NULL);
        attTypeLbl.setText("Attribute Type:");

        attTypeCmb = new CCombo(container, SWT.READ_ONLY | SWT.BORDER);
        gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalSpan = 4;
        gd.horizontalAlignment = SWT.FILL;
        attTypeCmb.setLayoutData(gd);
        initTypes();

        Group valuesGrp = new Group(container, SWT.NULL);
        valuesGrp.setText("Attribute Values");
        gd = new GridData();
        gd.verticalIndent = 10;
        gd.horizontalSpan = 3;
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        gd.grabExcessVerticalSpace = true;
        gd.verticalAlignment = SWT.FILL;
        valuesGrp.setLayoutData(gd);
        layout = new GridLayout(4, false);
        valuesGrp.setLayout(layout);

        rangeBtn = new Button(valuesGrp, SWT.RADIO);
        rangeBtn.setText("Range");
        gd = new GridData();
        gd.horizontalSpan = 4;
        rangeBtn.setLayoutData(gd);

        minLbl = new Label(valuesGrp, SWT.NONE);
        minLbl.setText("Min:");
        gd = new GridData();
        gd.horizontalIndent = 25;
        minLbl.setLayoutData(gd);

        minRangeTxt = new Text(valuesGrp, SWT.BORDER);
        gd = new GridData();
        gd.widthHint = 70;
        minRangeTxt.setLayoutData(gd);

        maxLbl = new Label(valuesGrp, SWT.NONE);
        maxLbl.setText("Max:");
        gd = new GridData();
        gd.horizontalIndent = 25;
        maxLbl.setLayoutData(gd);

        maxRangeTxt = new Text(valuesGrp, SWT.BORDER);
        gd = new GridData();
        gd.widthHint = 70;
        maxRangeTxt.setLayoutData(gd);

        cusValuesBtn = new Button(valuesGrp, SWT.RADIO);
        cusValuesBtn.setText("Custom");
        gd = new GridData();
        gd.horizontalSpan = 4;
        cusValuesBtn.setLayoutData(gd);

        Composite cusValuesComp = new Composite(valuesGrp, SWT.NONE);
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        gd.horizontalSpan = 4;
        cusValuesComp.setLayoutData(gd);
        layout = new GridLayout(2, false);
        cusValuesComp.setLayout(layout);

        customValuesList = new List(cusValuesComp, SWT.BORDER | SWT.MULTI
                | SWT.V_SCROLL);
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        gd.heightHint = 75;
        gd.horizontalIndent = 25;
        customValuesList.setLayoutData(gd);

        Composite cusValuesActionsComp = new Composite(cusValuesComp, SWT.NONE);
        layout = new GridLayout();
        cusValuesActionsComp.setLayout(layout);
        gd = new GridData();
        gd.verticalAlignment = SWT.TOP;
        cusValuesActionsComp.setLayoutData(gd);

        addBtn = new Button(cusValuesActionsComp, SWT.PUSH);
        addBtn.setText("Add");
        gd = new GridData();
        gd.widthHint = 70;
        addBtn.setLayoutData(gd);

        remBtn = new Button(cusValuesActionsComp, SWT.PUSH);
        remBtn.setText("Remove");
        gd = new GridData();
        gd.widthHint = 70;
        remBtn.setLayoutData(gd);
        remBtn.setEnabled(false);

        noneBtn = new Button(valuesGrp, SWT.RADIO);
        noneBtn.setText("None");
        gd = new GridData();
        gd.horizontalSpan = 4;
        noneBtn.setLayoutData(gd);

        Composite detailsComp = new Composite(container, SWT.NULL);
        detailsComp.setLayout(new GridLayout());
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        gd.horizontalSpan = 2;
        gd.widthHint = 100;
        detailsComp.setLayoutData(gd);

        Label lbl = new Label(detailsComp, SWT.NULL);
        lbl.setText("Details");

        Group detailsGrp = new Group(detailsComp, SWT.NULL);
        detailsGrp.setLayout(new GridLayout());
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        detailsGrp.setLayoutData(gd);

        detail = new Text(detailsGrp, SWT.MULTI | SWT.READ_ONLY | SWT.BORDER
                | SWT.WRAP | SWT.V_SCROLL);
        detail.setBackground(detailsGrp.getBackground());
        detail.setText(defaultMessage);
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        detail.setLayoutData(gd);

        Label dflValueLbl = new Label(container, SWT.NULL);
        dflValueLbl.setText("Default value");

        dflValueTxt = new Text(container, SWT.BORDER);
        gd = new GridData();
        gd.horizontalSpan = 4;
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        dflValueTxt.setLayoutData(gd);

        setUiListeners();

        if (editOperation) {
            initData();
            attNameTxt.setFocus();
        } else {
            setInitialSelection();
        }

        return compLayout;
    }

    private void initTypes() {
        if (null != attValueTypes && attValueTypes.size() > 0) {
            Iterator<String> itr = attValueTypes.iterator();
            while (itr.hasNext()) {
                attTypeCmb.add(itr.next());
            }
        }
    }

    private void setInitialSelection() {
        enable(false);
        rangeOptionSelected(false);
        customOptionSelected(false);
    }

    private void initData() {
        if (editOperation) {
            // Populate the UI controls with the data.
            attNameTxt.setText(attHelper.getAttributeName());
            attTypeCmb.select(attTypeCmb.indexOf(attHelper.getAttributeType()));
            updateControls();
            dflValueTxt.setText(attHelper.getAttributeDflValue());
            ValidValuesType valuesType = attHelper.getValidValuesType();
            if (valuesType == ValidValuesType.RANGE) {
                rangeBtn.setSelection(true);
                noneBtn.setSelection(false);
                rangeOptionSelected(true);
                minRangeTxt.setText(attHelper.getMin());
                maxRangeTxt.setText(attHelper.getMax());
            } else if (valuesType == ValidValuesType.VALUESET) {
                cusValuesBtn.setSelection(true);
                noneBtn.setSelection(false);
                customOptionSelected(true);
                Set<String> allowedValues = attHelper.getAllowedValues();
                customValuesList.setItems(allowedValues.toArray(new String[1]));
            }
        }
    }

    private void setUiListeners() {
        rangeBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                rangeOptionSelected(true);
                customOptionSelected(false);
                minRangeTxt.setFocus();
            }
        });

        cusValuesBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                addBtn.setFocus();
                rangeOptionSelected(false);
                customOptionSelected(true);
            }
        });

        noneBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                rangeOptionSelected(false);
                customOptionSelected(false);
            }
        });

        attTypeCmb.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                updateControls();
            }
        });

        minRangeTxt.addListener(SWT.Verify, new Listener() {
            @Override
            public void handleEvent(Event e) {
                String string = e.text;
                char[] chars = new char[string.length()];
                string.getChars(0, chars.length, chars, 0);
                for (int i = 0; i < chars.length; i++) {
                    if (!(('0' <= chars[i] && chars[i] <= '9')
                            || chars[i] == '-' || chars[i] == '+')) {
                        if (attTypeCmb.getText().equals(Constants.INT)) {
                            e.doit = false;
                            return;
                        } else if (attTypeCmb.getText()
                                .equals(Constants.DOUBLE)) {
                            if (!(chars[i] == '.')) {
                                e.doit = false;
                                return;
                            }
                        }
                    }
                }
            }
        });

        maxRangeTxt.addListener(SWT.Verify, new Listener() {
            @Override
            public void handleEvent(Event e) {
                String string = e.text;
                char[] chars = new char[string.length()];
                string.getChars(0, chars.length, chars, 0);
                for (int i = 0; i < chars.length; i++) {
                    if (!(('0' <= chars[i] && chars[i] <= '9')
                            || chars[i] == '-' || chars[i] == '+')) {
                        if (attTypeCmb.getText().equals(Constants.INT)) {
                            e.doit = false;
                            return;
                        } else if (attTypeCmb.getText()
                                .equals(Constants.DOUBLE)) {
                            if (!(chars[i] == '.')) {
                                e.doit = false;
                                return;
                            }
                        }
                    }
                }
            }
        });

        addBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                SingleTextInputDialog dialog = new SingleTextInputDialog(
                        getShell(), "Add Attribute Value", "Attribute Value");
                if (dialog.open() == Window.OK) {
                    String value = dialog.getValue();
                    String type = attTypeCmb.getText();
                    if (!attHelper.isValueValid(value, type)) {
                        MessageDialog.openError(getParentShell(),
                                "Invalid value", "Attribute value is invalid.");
                    } else if (attHelper.isAllowedValueExist(
                            customValuesList.getItems(), value)) {
                        MessageDialog.openError(getParentShell(),
                                "Duplicate value",
                                "Attribute value already exists.");
                    } else {
                        customValuesList.add(value);
                        customValuesList.deselectAll();
                        customValuesList.select(customValuesList.getItemCount() - 1);
                        customValuesList.showSelection();
                        remBtn.setEnabled(true);
                    }
                }
            }
        });

        remBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                int[] selection = customValuesList.getSelectionIndices();
                if (null != selection && selection.length > 0) {
                    customValuesList.remove(selection);
                }

                changeRemBtnVisibility();
            }
        });

        customValuesList.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                changeRemBtnVisibility();
            }
        });
    }

    private void updateControls() {
        rangeBtn.setSelection(false);
        cusValuesBtn.setSelection(false);
        noneBtn.setSelection(true);

        cleanRangeAndCustomValues();
        dflValueTxt.setText("");

        String selected = attTypeCmb.getText();

        if (selected.equals(Constants.INT)) {
            enable(true);
            rangeOptionSelected(false);
            customOptionSelected(false);
            detail.setText(defaultMessage + msgForIntType);
        } else if (selected.equals(Constants.DOUBLE)) {
            enable(true);
            rangeOptionSelected(false);
            customOptionSelected(false);
            detail.setText(defaultMessage + msgForDoubleType);
        } else if (selected.equals(Constants.BOOL)) {
            enable(false);
            rangeOptionSelected(false);
            customOptionSelected(false);
            detail.setText(defaultMessage + msgForBoolType);
        } else if (selected.equals(Constants.STRING)) {
            rangeBtn.setEnabled(false);
            rangeOptionSelected(false);
            cusValuesBtn.setEnabled(true);
            noneBtn.setEnabled(true);
            detail.setText(defaultMessage + msgForStringType);
        } else {
            enable(false);
            rangeOptionSelected(false);
            customOptionSelected(false);
            detail.setText(defaultMessage);
        }
    }

    private void changeRemBtnVisibility() {
        if (cusValuesBtn.isEnabled() && cusValuesBtn.getSelection()) {
            int[] selection = customValuesList.getSelectionIndices();
            if (null != selection && selection.length > 0) {
                remBtn.setEnabled(true);
            } else {
                remBtn.setEnabled(false);
            }
        } else {
            remBtn.setEnabled(false);
        }
    }

    private void enable(boolean enable) {
        rangeBtn.setEnabled(enable);
        cusValuesBtn.setEnabled(enable);
        noneBtn.setEnabled(enable);
    }

    private void rangeOptionSelected(boolean enable) {
        minRangeTxt.setEnabled(enable);
        maxRangeTxt.setEnabled(enable);
        minLbl.setEnabled(enable);
        maxLbl.setEnabled(enable);
    }

    private void customOptionSelected(boolean enable) {
        customValuesList.setEnabled(enable);
        addBtn.setEnabled(enable);
        changeRemBtnVisibility();
    }

    private void cleanRangeAndCustomValues() {
        cleanRangeValues();
        cleanCustomValues();
    }

    private void cleanRangeValues() {
        minRangeTxt.setText("");
        maxRangeTxt.setText("");
    }

    private void cleanCustomValues() {
        customValuesList.removeAll();
    }

    public AttributeHelper getAttHelper() {
        return attHelper;
    }

    public AttributeHelper getAttClone() {
        return attClone;
    }

    @Override
    protected void okPressed() {
        // Attribute Name
        String attName = attNameTxt.getText();
        if (null == attName || attName.isEmpty()) {
            MessageDialog.openError(getParentShell(),
                    "Mandatory field is empty",
                    "Please fill the attribute name.");
            attNameTxt.setFocus();
            return;
        }
        attName = attName.trim();
        if (attName.length() < 1) {
            MessageDialog.openError(getParentShell(),
                    "Mandatory field is empty", "Attribute name is invalid.");
            attNameTxt.setFocus();
            return;
        }
        attHelper.setAttributeName(attName);

        // Duplicate check for attribute name if this is not editing operation
        if (!editOperation) {
            if (!attributes.isEmpty()) {
                Iterator<AttributeHelper> itr = attributes.iterator();
                AttributeHelper att;
                while (itr.hasNext()) {
                    att = itr.next();
                    if (att.getAttributeName().equals(attName)) {
                        MessageDialog
                                .openError(
                                        getParentShell(),
                                        "Duplicate Attribute",
                                        "Another attribute with the same attribute name exist. Please enter a new attribute name.");
                        attNameTxt.setFocus();
                        return;
                    }
                }
            }
        }

        // Attribute Type
        String attType = attTypeCmb.getText();
        if (null == attType || attType.isEmpty()) {
            MessageDialog.openError(getParentShell(),
                    "Mandatory field is empty",
                    "Please select an attribute type.");
            return;
        }
        attHelper.setAttributeType(attType);

        // Attribute values
        if (rangeBtn.isEnabled() && rangeBtn.getSelection()) { // Range option
            String min = minRangeTxt.getText();
            String max = maxRangeTxt.getText();
            if (null == min && null == max) {
                MessageDialog
                        .openError(getParentShell(), "Range is empty",
                                "Please enter the minimum and maximum value of the range.");
                minRangeTxt.setFocus();
                return;
            }
            if (null == min || null == max) {
                String msg;
                msg = "Please enter the "
                        + ((null == min) ? "minimum value" : "maximum value")
                        + "of the range.";
                MessageDialog
                        .openError(getParentShell(), "Range is empty", msg);
                ((null == min) ? minRangeTxt : maxRangeTxt).setFocus();
                return;
            }
            if (min.isEmpty() && max.isEmpty()) {
                MessageDialog
                        .openError(getParentShell(), "Range is empty",
                                "Please enter the minimum and maximum value of the range.");
                minRangeTxt.setFocus();
                return;
            }
            if (min.isEmpty()) {
                MessageDialog.openError(getParentShell(), "Range is empty",
                        "Please enter the minimum value for the range.");
                minRangeTxt.setFocus();
                return;
            }
            if (max.isEmpty()) {
                MessageDialog.openError(getParentShell(), "Range is empty",
                        "Please enter the maximum value for the range.");
                maxRangeTxt.setFocus();
                return;
            }

            if (!min.isEmpty() && !max.isEmpty()
                    && !attHelper.isRangeValid(min, max, attType)) {
                MessageDialog.openError(getParentShell(), "Invalid range",
                        "Range is invalid.");
                maxRangeTxt.setFocus();
                return;
            }
            attHelper.setValidValuesType(ValidValuesType.RANGE);
            attHelper.setMin(min);
            attHelper.setMax(max);

            if (editOperation) {
                // Remove all existing custom values
                attHelper.setAllowedValues(null);
            }
        } else if (cusValuesBtn.isEnabled() && cusValuesBtn.getSelection()) {
            String[] cusItems = customValuesList.getItems();
            if (null == cusItems || cusItems.length < 1) {
                MessageDialog.openError(getParentShell(),
                        "Custom list is empty.",
                        "No values are added to the custom list.");
                maxRangeTxt.setFocus();
                return;
            }
            attHelper.setValidValuesType(ValidValuesType.VALUESET);
            attHelper.setAllowedValuesByArray(cusItems);

            if (editOperation) {
                // Remove min and max values
                attHelper.setMin(null);
                attHelper.setMax(null);
            }
        } else if (noneBtn.isEnabled() && noneBtn.getSelection()) {
            attHelper.setValidValuesType(ValidValuesType.UNKNOWN);
            if (editOperation) {
                // Remove min, max and custom values
                attHelper.setAllowedValues(null);
                attHelper.setMin(null);
                attHelper.setMax(null);
            }
        }

        // Attribute Default Value
        String attDflValue = dflValueTxt.getText();
        if (null == attDflValue || attDflValue.isEmpty()) {
            MessageDialog.openError(getParentShell(),
                    "Mandatory field is empty",
                    "Please enter a default value for the attribute.");
            dflValueTxt.setFocus();
            return;
        }
        attDflValue = attDflValue.trim();
        if (attDflValue.length() < 1
                || !attHelper.isValueValid(attDflValue, attType)) {
            MessageDialog.openError(getParentShell(), "Invalid value",
                    "Default value is invalid.");
            dflValueTxt.setFocus();
            return;
        }
        if (!attHelper.isDefaultValueValid(attDflValue)) {
            MessageDialog.openError(getParentShell(), "Invalid value",
                    "Default value is not in the valid values.");
            dflValueTxt.setFocus();
            return;
        }

        attHelper.setAttributeDflValue(attDflValue.toLowerCase());

        close();
    }

    @Override
    public boolean isHelpAvailable() {
        return false;
    }

    @Override
    protected Point getInitialLocation(Point initialSize) {
        Rectangle shellBounds = Display.getDefault().getActiveShell()
                .getBounds();
        Point dialogSize = getInitialSize();

        return new Point(
                shellBounds.x + (shellBounds.width - dialogSize.x) / 2,
                shellBounds.y + (shellBounds.height - dialogSize.y) / 2);
    }
}