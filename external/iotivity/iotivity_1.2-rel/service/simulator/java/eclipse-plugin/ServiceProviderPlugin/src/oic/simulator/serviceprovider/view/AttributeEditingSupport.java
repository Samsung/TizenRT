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

package oic.simulator.serviceprovider.view;

import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.viewers.CellEditor;
import org.eclipse.jface.viewers.CheckboxCellEditor;
import org.eclipse.jface.viewers.ComboBoxCellEditor;
import org.eclipse.jface.viewers.EditingSupport;
import org.eclipse.jface.viewers.TextCellEditor;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.jface.window.Window;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.CCombo;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.MessageBox;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.TreeItem;
import org.eclipse.ui.IPartListener2;
import org.eclipse.ui.IWorkbenchPartReference;

import java.util.Date;
import java.util.List;

import org.oic.simulator.AttributeValue;
import org.oic.simulator.AttributeValue.TypeInfo;
import org.oic.simulator.AttributeValue.ValueType;
import org.oic.simulator.ILogger.Level;
import org.oic.simulator.InvalidArgsException;
import org.oic.simulator.SimulatorResourceAttribute;
import org.oic.simulator.server.SimulatorResource.AutoUpdateType;

import oic.simulator.serviceprovider.Activator;
import oic.simulator.serviceprovider.manager.ResourceManager;
import oic.simulator.serviceprovider.model.AttributeElement;
import oic.simulator.serviceprovider.model.AutomationSettingHelper;
import oic.simulator.serviceprovider.model.Resource;
import oic.simulator.serviceprovider.model.ResourceRepresentation;
import oic.simulator.serviceprovider.model.SingleResource;
import oic.simulator.serviceprovider.utils.AttributeValueBuilder;
import oic.simulator.serviceprovider.utils.Utility;
import oic.simulator.serviceprovider.view.dialogs.AutomationSettingDialog;
import oic.simulator.serviceprovider.view.dialogs.UpdatePrimitiveArrayAttributeDialog;

/**
 * This class provides editing support to the resources attributes table in the
 * attributes view.
 */
public class AttributeEditingSupport {

    private AttributeValueEditor attValueEditor;
    private AutomationEditor     automationEditor;

    private static class SyncValueUpdate {
        private Boolean valueChangeInProgress;

        public boolean isValueChangeInProgress() {
            return valueChangeInProgress;
        }

        public void setValueChangeInProgress(boolean value) {
            valueChangeInProgress = value;
        }
    }

    private final SyncValueUpdate syncValueChange = new SyncValueUpdate();

    public AttributeValueEditor createAttributeValueEditor(TreeViewer viewer,
            Boolean nativeUpdateValueCall) {
        attValueEditor = new AttributeValueEditor(viewer, nativeUpdateValueCall);
        syncValueChange.setValueChangeInProgress(false);
        return attValueEditor;
    }

    public AutomationEditor createAutomationEditor(TreeViewer viewer) {
        automationEditor = new AutomationEditor(viewer);
        return automationEditor;
    }

    class AttributeValueEditor extends EditingSupport {

        private final TreeViewer viewer;
        private CCombo           comboBox;

        private Boolean          callNativeUpdateValue;

        public AttributeValueEditor(TreeViewer viewer,
                Boolean nativeUpdateValueCall) {
            super(viewer);
            this.viewer = viewer;
            callNativeUpdateValue = nativeUpdateValueCall;
            // Using the part listener to refresh the viewer on various part
            // events.
            // If combo list is open, then click events on other parts of the
            // view or outside the combo should hide the editor.
            // Refreshing the viewer hides the combo and other editors which are
            // active.
            IPartListener2 partListener;
            partListener = new IPartListener2() {

                @Override
                public void partVisible(IWorkbenchPartReference partRef) {
                }

                @Override
                public void partOpened(IWorkbenchPartReference partRef) {
                }

                @Override
                public void partInputChanged(IWorkbenchPartReference partRef) {
                }

                @Override
                public void partHidden(IWorkbenchPartReference partRef) {
                }

                @Override
                public void partDeactivated(IWorkbenchPartReference partRef) {
                    String viewId = partRef.getId();
                    if (viewId.equals(AttributeView.VIEW_ID)) {
                        synchronized (syncValueChange) {
                            updateUnSavedData();
                        }
                    }
                }

                @Override
                public void partClosed(IWorkbenchPartReference partRef) {
                }

                @Override
                public void partBroughtToTop(IWorkbenchPartReference partRef) {
                }

                @Override
                public void partActivated(IWorkbenchPartReference partRef) {
                    String viewId = partRef.getId();
                    if (viewId.equals(AttributeView.VIEW_ID)) {
                        if (null == AttributeValueEditor.this.viewer)
                            return;

                        Tree tree = AttributeValueEditor.this.viewer.getTree();
                        if (null == tree || tree.isDisposed())
                            return;

                        AttributeValueEditor.this.viewer.refresh();
                    }
                }
            };

            try {
                Activator.getDefault().getWorkbench()
                        .getActiveWorkbenchWindow().getActivePage()
                        .addPartListener(partListener);
            } catch (Exception e) {
                Activator
                        .getDefault()
                        .getLogManager()
                        .log(Level.ERROR.ordinal(),
                                new Date(),
                                "There is an error while configuring the listener for UI.\n"
                                        + Utility.getSimulatorErrorString(e,
                                                null));
            }
        }

        public void updateUnSavedData() {
            if (null == viewer || null == comboBox)
                return;

            Tree tree = viewer.getTree();
            if (null == tree || tree.isDisposed())
                return;

            TreeItem[] selectedItems = tree.getSelection();
            if (null == selectedItems || 1 != selectedItems.length)
                return;

            Object element = selectedItems[0].getData();
            if (null == element || !(element instanceof AttributeElement))
                return;

            AttributeElement attElement = (AttributeElement) element;
            SimulatorResourceAttribute att = attElement
                    .getSimulatorResourceAttribute();
            if (null == att)
                return;

            if (Activator.getDefault().getResourceManager()
                    .isAttHasRangeOrAllowedValues(att)) {
                viewer.refresh();
                return;
            }

            synchronized (syncValueChange) {
                if (!syncValueChange.isValueChangeInProgress()) {

                    AttributeValue value = att.value();
                    if (value == null)
                        return;

                    TypeInfo type = value.typeInfo();

                    if (type.mBaseType == ValueType.RESOURCEMODEL
                            || type.mType == ValueType.ARRAY) {
                        return;
                    }

                    if (null == value.get()) {
                        return;
                    }

                    String oldValue = String.valueOf(Utility
                            .getAttributeValueAsString(value));

                    attElement.setEditLock(true);
                    compareAndUpdateAttribute(oldValue, comboBox.getText(),
                            (AttributeElement) element, att, type);
                    attElement.setEditLock(false);

                    viewer.refresh();
                }
            }
        }

        @Override
        protected boolean canEdit(Object arg0) {
            return true;
        }

        @Override
        protected CellEditor getCellEditor(final Object element) {
            ResourceManager resourceManager = Activator.getDefault()
                    .getResourceManager();

            Resource res = resourceManager.getCurrentResourceInSelection();
            if (null == res) {
                return null;
            }

            // If selected resource is not a single resource, then editor
            // support is not required.
            if (!(res instanceof SingleResource)) {
                return null;
            }

            final SimulatorResourceAttribute attribute;
            if (!(element instanceof AttributeElement)) {
                return null;
            }

            final AttributeElement attributeElement = ((AttributeElement) element);
            attribute = attributeElement.getSimulatorResourceAttribute();
            if (null == attribute) {
                return null;
            }

            // CellEditor is not required as the automation is in progress.
            if (attributeElement.isAutoUpdateInProgress()) {
                return null;
            }

            final AttributeValue val = attribute.value();
            if (null == val) {
                return null;
            }

            final TypeInfo type = val.typeInfo();
            if (type.mBaseType == ValueType.RESOURCEMODEL) {
                return null;
            }

            synchronized (syncValueChange) {
                syncValueChange.setValueChangeInProgress(false);
            }

            CellEditor editor;
            if (type.mType == ValueType.ARRAY) {
                editor = new TextCellEditor(viewer.getTree());
                editor.setStyle(SWT.READ_ONLY);
                final Text txt = (Text) editor.getControl();
                txt.addModifyListener(new ModifyListener() {
                    @Override
                    public void modifyText(ModifyEvent e) {
                        String currentAttValue = txt.getText();
                        UpdatePrimitiveArrayAttributeDialog dialog = new UpdatePrimitiveArrayAttributeDialog(
                                Display.getDefault().getActiveShell(),
                                attribute);
                        if (dialog.open() == Window.OK) {
                            updateAttributeValue(attributeElement, attribute,
                                    dialog.getNewValueObj());

                            if (callNativeUpdateValue) {
                                ResourceManager resourceManager;
                                resourceManager = Activator.getDefault()
                                        .getResourceManager();

                                Resource resource = resourceManager
                                        .getCurrentResourceInSelection();

                                SimulatorResourceAttribute result = getResultantAttribute(attributeElement);
                                if (null == result) {
                                    Activator
                                            .getDefault()
                                            .getLogManager()
                                            .log(Level.ERROR.ordinal(),
                                                    new Date(),
                                                    "There is an error while updating the attribute value.\n");
                                    return;
                                }

                                boolean updated = resourceManager
                                        .attributeValueUpdated(
                                                (SingleResource) resource,
                                                result.name(), result.value());
                                if (!updated) {
                                    try {
                                        updateAttributeValue(attributeElement,
                                                attribute,
                                                AttributeValueBuilder.build(
                                                        currentAttValue,
                                                        type.mBaseType));
                                    } catch (Exception ex) {
                                        Activator
                                                .getDefault()
                                                .getLogManager()
                                                .log(Level.ERROR.ordinal(),
                                                        new Date(),
                                                        "There is an error while updating the attribute value.\n"
                                                                + Utility
                                                                        .getSimulatorErrorString(
                                                                                ex,
                                                                                null));
                                    }
                                    MessageDialog
                                            .openInformation(Display
                                                    .getDefault()
                                                    .getActiveShell(),
                                                    "Operation failed",
                                                    "Failed to update the attribute value.");
                                }
                            }
                        }

                        // Update the viewer in a separate UI thread.
                        Display.getDefault().asyncExec(new Runnable() {
                            @Override
                            public void run() {
                                viewer.refresh(element, true);
                            }
                        });
                    }
                });
            } else {
                String values[] = null;
                List<String> valueSet = resourceManager
                        .getAllValuesOfAttribute(attribute);
                values = Utility.convertListToStringArray(valueSet);

                boolean hasValueSet = resourceManager
                        .isAttHasRangeOrAllowedValues(attribute);

                if (hasValueSet) {
                    editor = new ComboBoxCellEditor(viewer.getTree(), values,
                            SWT.READ_ONLY);
                } else {
                    editor = new ComboBoxCellEditor(viewer.getTree(), values);
                }

                comboBox = (CCombo) editor.getControl();
                if (hasValueSet) {
                    comboBox.addModifyListener(new ModifyListener() {

                        @Override
                        public void modifyText(ModifyEvent event) {
                            String oldValue = String.valueOf(Utility
                                    .getAttributeValueAsString(val));
                            if (null == oldValue) {
                                oldValue = "";
                            }
                            String newValue = comboBox.getText();

                            attributeElement.setEditLock(true);
                            compareAndUpdateAttribute(oldValue, newValue,
                                    (AttributeElement) element, attribute, type);
                            attributeElement.setEditLock(false);

                            comboBox.setVisible(false);
                        }
                    });
                }
            }
            return editor;
        }

        @Override
        protected Object getValue(Object element) {
            int indexOfItem = 0;
            SimulatorResourceAttribute att = null;

            if (element instanceof AttributeElement) {
                att = ((AttributeElement) element)
                        .getSimulatorResourceAttribute();
            }

            if (att == null) {
                return 0;
            }

            final AttributeValue val = att.value();
            if (null == val) {
                return null;
            }

            final TypeInfo type = val.typeInfo();
            if (type.mBaseType == ValueType.RESOURCEMODEL) {
                return null;
            }

            String valueString = Utility.getAttributeValueAsString(att.value());
            if (null == valueString) {
                valueString = "";
            }

            if (type.mType == ValueType.ARRAY) {
                return valueString;
            }

            List<String> valueSet = Activator.getDefault().getResourceManager()
                    .getAllValuesOfAttribute(att);
            if (null != valueSet) {
                indexOfItem = valueSet.indexOf(valueString);
            }
            if (indexOfItem == -1) {
                indexOfItem = 0;
            }
            return indexOfItem;
        }

        @Override
        protected void setValue(Object element, Object value) {
            synchronized (syncValueChange) {
                if (syncValueChange.isValueChangeInProgress()) {
                    return;
                }

                SimulatorResourceAttribute att = null;
                if (element instanceof AttributeElement) {
                    att = ((AttributeElement) element)
                            .getSimulatorResourceAttribute();
                }

                if (att == null) {
                    return;
                }

                if (Activator.getDefault().getResourceManager()
                        .isAttHasRangeOrAllowedValues(att)) {
                    return;
                }

                AttributeValue val = att.value();
                if (null == val) {
                    return;
                }

                TypeInfo type = val.typeInfo();

                String oldValue = String.valueOf(Utility
                        .getAttributeValueAsString(val));
                if (null == oldValue) {
                    oldValue = "";
                }

                String newValue = comboBox.getText();

                ((AttributeElement) element).setEditLock(true);
                compareAndUpdateAttribute(oldValue, newValue,
                        (AttributeElement) element, att, type);
                ((AttributeElement) element).setEditLock(false);

            }

        }

        public void compareAndUpdateAttribute(String oldValue, String newValue,
                AttributeElement attElement, SimulatorResourceAttribute att,
                TypeInfo type) {
            if (null == oldValue || null == newValue || null == attElement
                    || null == att || null == type) {
                return;
            }

            synchronized (syncValueChange) {
                syncValueChange.setValueChangeInProgress(true);
            }

            if (!oldValue.equals(newValue)) {
                boolean invalid = false;

                // Get the AttriuteValue from the string
                AttributeValue attValue = null;
                try {
                    attValue = AttributeValueBuilder.build(newValue,
                            type.mBaseType);
                } catch (Exception e) {
                    Activator
                            .getDefault()
                            .getLogManager()
                            .log(Level.ERROR.ordinal(),
                                    new Date(),
                                    "There is an error while creating the new attribute value.\n"
                                            + Utility.getSimulatorErrorString(
                                                    e, null));
                }

                if (null == attValue) {
                    invalid = true;
                } else {
                    TypeInfo resTypeInfo = attValue.typeInfo();
                    if (type.mDepth != resTypeInfo.mDepth
                            || type.mType != resTypeInfo.mType
                            || type.mBaseType != resTypeInfo.mBaseType) {
                        invalid = true;
                    }
                }
                if (invalid) {
                    MessageBox dialog = new MessageBox(viewer.getTree()
                            .getShell(), SWT.ICON_ERROR | SWT.OK);
                    dialog.setText("Invalid Value");
                    dialog.setMessage("Given value is invalid");
                    dialog.open();
                } else {

                    // To show the new value till decision made.
                    updateAttributeValue(attElement, att, attValue);
                    viewer.update(attElement, null);

                    if (callNativeUpdateValue) {
                        MessageBox dialog = new MessageBox(viewer.getTree()
                                .getShell(), SWT.ICON_QUESTION | SWT.OK
                                | SWT.CANCEL);
                        dialog.setText("Confirm action");
                        dialog.setMessage("Do you want to modify the value?");
                        int retval = dialog.open();
                        if (retval != SWT.OK) {
                            try {
                                attValue = AttributeValueBuilder.build(
                                        oldValue, type.mBaseType);
                                updateAttributeValue(attElement, att, attValue);
                            } catch (Exception e) {
                                Activator
                                        .getDefault()
                                        .getLogManager()
                                        .log(Level.ERROR.ordinal(),
                                                new Date(),
                                                "There is an error while updating the attribute value.\n"
                                                        + Utility
                                                                .getSimulatorErrorString(
                                                                        e, null));
                            }
                        } else {
                            ResourceManager resourceManager;
                            resourceManager = Activator.getDefault()
                                    .getResourceManager();

                            Resource resource = resourceManager
                                    .getCurrentResourceInSelection();

                            SimulatorResourceAttribute result = getResultantAttribute(attElement);
                            if (null == result) {
                                Activator
                                        .getDefault()
                                        .getLogManager()
                                        .log(Level.ERROR.ordinal(), new Date(),
                                                "There is an error while updating the attribute value.\n");
                                return;
                            }

                            boolean updated = resourceManager
                                    .attributeValueUpdated(
                                            (SingleResource) resource,
                                            result.name(), result.value());
                            if (!updated) {
                                try {
                                    attValue = AttributeValueBuilder.build(
                                            oldValue, type.mBaseType);
                                    updateAttributeValue(attElement, att,
                                            attValue);
                                } catch (Exception e) {
                                    Activator
                                            .getDefault()
                                            .getLogManager()
                                            .log(Level.ERROR.ordinal(),
                                                    new Date(),
                                                    "There is an error while updating the attribute value.\n"
                                                            + Utility
                                                                    .getSimulatorErrorString(
                                                                            e,
                                                                            null));
                                }
                                MessageDialog
                                        .openInformation(Display.getDefault()
                                                .getActiveShell(),
                                                "Operation failed",
                                                "Failed to update the attribute value.");
                            }
                        }
                    }
                }
            }
            viewer.update(attElement, null);
        }

        public void updateAttributeValue(AttributeElement attributeElement,
                SimulatorResourceAttribute att, AttributeValue value) {
            if (null == attributeElement || null == att || null == value) {
                return;
            }

            attributeElement.getSimulatorResourceAttribute().setValue(value);

            Object parent = attributeElement.getParent();
            if (null != parent && parent instanceof AttributeElement) {
                AttributeElement parentElement = (AttributeElement) parent;
                try {
                    parentElement.deepSetChildValue(att);
                } catch (InvalidArgsException e) {
                    Activator
                            .getDefault()
                            .getLogManager()
                            .log(Level.ERROR.ordinal(),
                                    new Date(),
                                    "There is an error while updating the attribute value.\n"
                                            + Utility.getSimulatorErrorString(
                                                    e, null));
                }
            }
        }

        public SimulatorResourceAttribute getResultantAttribute(
                AttributeElement attElement) {
            if (null == attElement) {
                return null;
            }

            SimulatorResourceAttribute result;
            Object parent = attElement.getParent();

            while (parent != null && parent instanceof AttributeElement) {
                attElement = (AttributeElement) parent;
                parent = ((AttributeElement) parent).getParent();
            }
            result = attElement.getSimulatorResourceAttribute();

            return result;
        }
    }

    private static class AutomationEditor extends EditingSupport {

        private final TreeViewer viewer;

        public AutomationEditor(TreeViewer viewer) {
            super(viewer);
            this.viewer = viewer;
        }

        @Override
        protected boolean canEdit(Object arg0) {
            return true;
        }

        @Override
        protected CellEditor getCellEditor(Object element) {
            // CellEditor is not required as the automation is in progress.
            ResourceManager resourceManager = Activator.getDefault()
                    .getResourceManager();
            Resource resource = resourceManager.getCurrentResourceInSelection();

            if (null == resource) {
                return null;
            }

            if (!(resource instanceof SingleResource)) {
                return null;
            }
            if (((SingleResource) resource).isResourceAutomationInProgress()) {
                return null;
            }

            SimulatorResourceAttribute att = null;
            if (element instanceof AttributeElement) {
                att = ((AttributeElement) element)
                        .getSimulatorResourceAttribute();
            }

            if (null == att) {
                return null;
            }

            AttributeValue val = att.value();
            if (null == val) {
                return null;
            }

            TypeInfo type = val.typeInfo();

            if (type.mType == ValueType.RESOURCEMODEL
                    || type.mType == ValueType.ARRAY) {
                return null;
            }

            Object parent = ((AttributeElement) element).getParent();
            if (null != parent && !(parent instanceof ResourceRepresentation)) {
                return null;
            }

            if (((AttributeElement) element).isReadOnly()) {
                return null;
            }

            return new CheckboxCellEditor(null, SWT.CHECK | SWT.READ_ONLY);
        }

        @Override
        protected Object getValue(Object element) {
            if (element instanceof AttributeElement) {
                return ((AttributeElement) element).isAutoUpdateInProgress();
            }

            return false;
        }

        @Override
        protected void setValue(Object element, Object value) {
            if (!(element instanceof AttributeElement)) {
                return;
            }

            ResourceManager resourceManager = Activator.getDefault()
                    .getResourceManager();
            // As automation depends on the current resource in selection, its
            // presence is being checked.
            Resource resource = resourceManager.getCurrentResourceInSelection();
            if (null == resource) {
                return;
            }

            AttributeElement att = (AttributeElement) element;
            boolean checked = (Boolean) value;
            if (checked) {
                // Start the automation
                // Fetch the settings data
                List<AutomationSettingHelper> automationSettings;
                automationSettings = AutomationSettingHelper
                        .getAutomationSettings(att);

                // Open the settings dialog
                AutomationSettingDialog dialog = new AutomationSettingDialog(
                        viewer.getTree().getShell(), automationSettings);
                dialog.create();
                if (dialog.open() == Window.OK) {
                    String automationType = dialog.getAutomationType();
                    String updateFreq = dialog.getUpdateFrequency();

                    AutoUpdateType autoType = AutoUpdateType
                            .valueOf(automationType);
                    int updFreq = Utility
                            .getUpdateIntervalFromString(updateFreq);
                    int autoId = resourceManager.startAutomation(
                            (SingleResource) resource, att, autoType, updFreq);
                    if (autoId == -1) {
                        MessageDialog.openInformation(Display.getDefault()
                                .getActiveShell(), "Automation Status",
                                "Automation start failed!!");
                    }
                }
            } else {
                // Stop the automation
                resourceManager.stopAutomation((SingleResource) resource, att,
                        att.getAutoUpdateId());
            }
        }
    }
}
