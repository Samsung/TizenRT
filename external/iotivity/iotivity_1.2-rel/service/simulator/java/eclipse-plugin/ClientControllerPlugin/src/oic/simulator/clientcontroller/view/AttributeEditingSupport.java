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

package oic.simulator.clientcontroller.view;

import org.eclipse.jface.dialogs.TitleAreaDialog;
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

import java.util.Date;
import java.util.List;

import org.oic.simulator.ArrayProperty;
import org.oic.simulator.AttributeProperty;
import org.oic.simulator.AttributeValue;
import org.oic.simulator.AttributeValue.TypeInfo;
import org.oic.simulator.AttributeValue.ValueType;
import org.oic.simulator.ILogger.Level;
import org.oic.simulator.InvalidArgsException;
import org.oic.simulator.SimulatorResourceAttribute;

import oic.simulator.clientcontroller.Activator;
import oic.simulator.clientcontroller.manager.ResourceManager;
import oic.simulator.clientcontroller.remoteresource.AttributeElement;
import oic.simulator.clientcontroller.remoteresource.RemoteResource;
import oic.simulator.clientcontroller.remoteresource.ResourceRepresentation;
import oic.simulator.clientcontroller.utils.AttributeValueBuilder;
import oic.simulator.clientcontroller.utils.Utility;
import oic.simulator.clientcontroller.view.dialogs.PostRequestDialog;
import oic.simulator.clientcontroller.view.dialogs.UpdatePrimitiveArrayAttributeDialog;

/**
 * This class provides editing support to the resources attributes table in the
 * attributes view.
 */
public class AttributeEditingSupport {

    private AttributeValueEditor attValueEditor;
    private PostSelectionEditor  postSelectionEditor;

    public AttributeValueEditor createAttributeValueEditor(TreeViewer viewer,
            TitleAreaDialog dialog) {
        attValueEditor = new AttributeValueEditor(viewer, dialog);
        return attValueEditor;
    }

    public PostSelectionEditor createPostSelectionEditor(TreeViewer viewer) {
        postSelectionEditor = new PostSelectionEditor(viewer);
        return postSelectionEditor;
    }

    class AttributeValueEditor extends EditingSupport {

        private final TreeViewer viewer;
        private CCombo           comboBox;
        private TitleAreaDialog  dialog;

        public AttributeValueEditor(TreeViewer viewer, TitleAreaDialog dialog) {
            super(viewer);
            this.viewer = viewer;
            this.dialog = dialog;
        }

        @Override
        protected boolean canEdit(Object arg0) {
            return true;
        }

        @Override
        protected CellEditor getCellEditor(final Object element) {
            ResourceManager resourceManager = Activator.getDefault()
                    .getResourceManager();

            RemoteResource res = resourceManager
                    .getCurrentResourceInSelection();
            if (null == res) {
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

            final AttributeValue val = attribute.value();
            if (null == val) {
                return null;
            }

            final TypeInfo type = val.typeInfo();
            if (type.mBaseType == ValueType.RESOURCEMODEL) {
                return null;
            }

            CellEditor editor;
            if (type.mType == ValueType.ARRAY && res.isConfigUploaded()
                    && isArrayAttributeValid(attribute)) {
                editor = new TextCellEditor(viewer.getTree());
                editor.setStyle(SWT.READ_ONLY);
                final Text txt = (Text) editor.getControl();
                txt.addModifyListener(new ModifyListener() {
                    @Override
                    public void modifyText(ModifyEvent e) {
                        UpdatePrimitiveArrayAttributeDialog dialog = new UpdatePrimitiveArrayAttributeDialog(
                                Display.getDefault().getActiveShell(),
                                attribute);
                        if (dialog.open() == Window.OK) {
                            updateAttributeValue(attributeElement, attribute,
                                    dialog.getNewValueObj());
                        }

                        // Update the viewer in a separate UI thread.
                        Display.getDefault().asyncExec(new Runnable() {
                            @Override
                            public void run() {
                                // Set the post state of the top-level
                                // attribute.
                                AttributeElement rootElement = getRootElement(attributeElement);
                                rootElement.setPostState(true);
                                viewer.refresh(rootElement, true);
                            }
                        });
                    }
                });
            } else {
                String values[] = null;
                List<String> valueSet = resourceManager
                        .getAllValuesOfAttribute(attribute);
                values = convertListToStringArray(valueSet);

                editor = new ComboBoxCellEditor(viewer.getTree(), values);
                comboBox = (CCombo) editor.getControl();
                comboBox.addModifyListener(new ModifyListener() {

                    @Override
                    public void modifyText(ModifyEvent event) {
                        // Set the post state of the top-level attribute.
                        AttributeElement rootElement = getRootElement(attributeElement);
                        rootElement.setPostState(true);
                        if (AttributeValueEditor.this.dialog instanceof PostRequestDialog) {
                            viewer.update(rootElement, null);
                        }
                    }
                });
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
                ResourceManager resourceManager = Activator.getDefault()
                        .getResourceManager();

                RemoteResource res = resourceManager
                        .getCurrentResourceInSelection();
                if (null != res && res.isConfigUploaded()
                        && isArrayAttributeValid(att)) {
                    return valueString;
                }
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
            SimulatorResourceAttribute att = null;
            if (element instanceof AttributeElement) {
                att = ((AttributeElement) element)
                        .getSimulatorResourceAttribute();
            }

            if (att == null) {
                return;
            }

            AttributeValue val = att.value();
            if (null == val) {
                return;
            }

            TypeInfo type = val.typeInfo();

            if (type.mBaseType == ValueType.RESOURCEMODEL) {
                return;
            }

            if (type.mType == ValueType.ARRAY) {
                ResourceManager resourceManager = Activator.getDefault()
                        .getResourceManager();

                RemoteResource res = resourceManager
                        .getCurrentResourceInSelection();
                if (null != res && res.isConfigUploaded()
                        && isArrayAttributeValid(att)) {
                    return;
                }
            }

            String oldValue = String.valueOf(Utility
                    .getAttributeValueAsString(val));
            if (null == oldValue) {
                oldValue = "";
            }

            String newValue = comboBox.getText();

            if (type.mType == ValueType.ARRAY
                    && type.mBaseType != ValueType.RESOURCEMODEL) {
                newValue = Utility.removeWhiteSpacesInArrayValues(newValue);
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
                    updateAttributeValue((AttributeElement) element, att,
                            attValue);
                }
            }

            viewer.update(element, null);
        }

        private boolean isArrayAttributeValid(
                SimulatorResourceAttribute attribute) {
            if (null == attribute)
                return false;

            AttributeValue val = attribute.value();
            if (null == val)
                return false;

            AttributeProperty prop = attribute.property();
            if (null == prop || !prop.isArray())
                return false;

            ArrayProperty arrProp = prop.asArray();
            if (null == arrProp)
                return false;

            AttributeProperty elementProp = arrProp.getElementProperty();
            if (null == elementProp)
                return false;

            TypeInfo info = val.typeInfo();
            if (info.mBaseType == ValueType.RESOURCEMODEL)
                return false;

            return true;
        }

        public String[] convertListToStringArray(List<String> values) {
            String[] strArr;
            if (null != values && values.size() > 0) {
                strArr = values.toArray(new String[1]);
            } else {
                strArr = new String[1];
            }
            return strArr;
        }

        public void updateAttributeValue(AttributeElement attributeElement,
                SimulatorResourceAttribute att, AttributeValue value) {
            // Update the post status.
            Object parent = attributeElement.getParent();
            AttributeElement rootElement = attributeElement;
            while (parent != null && parent instanceof AttributeElement) {
                rootElement = (AttributeElement) parent;
                parent = ((AttributeElement) parent).getParent();
            }
            rootElement.setPostState(true);

            // Set the attribute value.
            attributeElement.getSimulatorResourceAttribute().setValue(value);

            // Update the hierarchy.
            parent = attributeElement.getParent();
            if (null != parent && parent instanceof AttributeElement) {
                try {
                    ((AttributeElement) parent).deepSetChildValue(att);
                } catch (InvalidArgsException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private static class PostSelectionEditor extends EditingSupport {

        private final TreeViewer viewer;

        public PostSelectionEditor(TreeViewer viewer) {
            super(viewer);
            this.viewer = viewer;
        }

        @Override
        protected boolean canEdit(Object arg0) {
            return true;
        }

        @Override
        protected CellEditor getCellEditor(Object element) {
            if (element instanceof AttributeElement
                    && ((AttributeElement) element).getParent() instanceof ResourceRepresentation) {
                return new CheckboxCellEditor(null, SWT.CHECK | SWT.READ_ONLY);
            }

            return null;
        }

        @Override
        protected Object getValue(Object element) {
            if (element instanceof AttributeElement) {
                return ((AttributeElement) element).getPostState();
            }

            return false;
        }

        @Override
        protected void setValue(Object element, Object value) {
            if (!(element instanceof AttributeElement)) {
                return;
            }

            boolean status = (Boolean) value;
            ((AttributeElement) element).setPostState(status);
            viewer.update(element, null);

            Tree t = viewer.getTree();
            TreeItem item = t.getSelection()[0];
            if (null == item) {
                return;
            }

            // Update the post state of the top-most parent of this attribute.
            TreeItem parent = item.getParentItem();
            if (null != parent) {
                while (parent.getParentItem() != null) {
                    parent = parent.getParentItem();
                }
                Object data = parent.getData();
                ((AttributeElement) data).setPostState(status);
            }
        }
    }

    private AttributeElement getRootElement(AttributeElement element) {
        AttributeElement root = null;

        Object parent = element.getParent();
        if (parent instanceof ResourceRepresentation) {
            return element;
        }

        while (!(parent instanceof ResourceRepresentation)) {
            root = (AttributeElement) parent;
            parent = ((AttributeElement) parent).getParent();
        }

        return root;
    }
}
