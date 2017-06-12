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
import org.eclipse.jface.viewers.EditingSupport;
import org.eclipse.jface.viewers.IStructuredContentProvider;
import org.eclipse.jface.viewers.StyledCellLabelProvider;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.jface.viewers.TableViewerColumn;
import org.eclipse.jface.viewers.TextCellEditor;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.jface.viewers.ViewerCell;
import org.eclipse.jface.window.Window;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.part.ViewPart;

import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.oic.simulator.SimulatorException;

import oic.simulator.serviceprovider.Activator;
import oic.simulator.serviceprovider.listener.ISelectionChangedListener;
import oic.simulator.serviceprovider.manager.ResourceManager;
import oic.simulator.serviceprovider.manager.UiListenerHandler;
import oic.simulator.serviceprovider.model.MetaProperty;
import oic.simulator.serviceprovider.model.Resource;
import oic.simulator.serviceprovider.model.SingleResource;
import oic.simulator.serviceprovider.utils.Constants;
import oic.simulator.serviceprovider.utils.Utility;
import oic.simulator.serviceprovider.view.dialogs.UpdateResourceInterfaceDialog;

/**
 * This class manages and shows the meta properties view in the perspective.
 */
public class MetaPropertiesView extends ViewPart {

    public static final String        VIEW_ID       = "oic.simulator.serviceprovider.view.metaproperties";

    private TableViewer               tableViewer;

    private final String[]            columnHeaders = { "Property", "Value" };

    private final Integer[]           columnWidth   = { 150, 150 };

    private ISelectionChangedListener resourceSelectionChangedListener;

    private ResourceManager           resourceManagerRef;

    private Set<String>               updatedIfSet;

    private List<MetaProperty>        properties;

    private boolean                   enable_edit;
    private Button                    editBtn;
    private Button                    cancelBtn;

    private Map<String, String>       ifTypes;

    public MetaPropertiesView() {

        resourceManagerRef = Activator.getDefault().getResourceManager();

        resourceSelectionChangedListener = new ISelectionChangedListener() {

            @Override
            public void onResourceSelectionChange(final Resource resource) {
                Display.getDefault().asyncExec(new Runnable() {

                    @Override
                    public void run() {
                        if (null != tableViewer) {
                            properties = getData(resource);
                            updateViewer(properties);
                        }
                        updateEditControls(resource);
                    }
                });
            }
        };
    }

    @Override
    public void createPartControl(final Composite parent) {
        parent.setLayout(new GridLayout(2, false));

        tableViewer = new TableViewer(parent, SWT.SINGLE | SWT.H_SCROLL
                | SWT.V_SCROLL | SWT.FULL_SELECTION | SWT.BORDER);

        createColumns(tableViewer);

        // Make lines and header visible
        final Table table = tableViewer.getTable();
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        gd.horizontalSpan = 2;
        table.setLayoutData(gd);
        table.setHeaderVisible(true);
        table.setLinesVisible(true);

        tableViewer.setContentProvider(new PropertycontentProvider());

        editBtn = new Button(parent, SWT.PUSH);
        editBtn.setText("Edit");
        gd = new GridData();
        gd.widthHint = 50;
        editBtn.setLayoutData(gd);
        editBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                if (editBtn.getText().equals("Edit")) {
                    cancelBtn.setEnabled(true);
                    editBtn.setText("Save");
                    setEnableEdit(true);

                    Resource resource = resourceManagerRef
                            .getCurrentResourceInSelection();
                    if (null != resource) {
                        updatedIfSet = resource.getResourceInterfaces();
                    }
                } else {
                    Resource resourceInSelection = resourceManagerRef
                            .getCurrentResourceInSelection();
                    if (null != resourceInSelection) {

                        // Null Check
                        boolean result = resourceManagerRef
                                .isPropertyValueInvalid(resourceInSelection,
                                        properties, Constants.RESOURCE_URI);
                        if (result) {
                            MessageDialog.openError(parent.getShell(),
                                    "Invalid Resource URI.",
                                    Constants.INVALID_URI_MESSAGE);
                            return;
                        }

                        result = resourceManagerRef.isPropertyValueInvalid(
                                resourceInSelection, properties,
                                Constants.RESOURCE_NAME);
                        if (result) {
                            MessageDialog.openError(parent.getShell(),
                                    "Invalid Input",
                                    "Resource Name is invalid.");
                            return;
                        }

                        result = resourceManagerRef.isPropertyValueInvalid(
                                resourceInSelection, properties,
                                Constants.RESOURCE_TYPE);
                        if (result) {
                            MessageDialog.openError(parent.getShell(),
                                    "Invalid Resource Type.",
                                    Constants.INVALID_RESOURCE_TYPE_MESSAGE);
                            return;
                        }

                        boolean update = false;
                        boolean uriChange = false;
                        boolean typeChange = false;
                        boolean nameChange = false;
                        boolean interfaceChange = false;

                        if (resourceManagerRef.isPropValueChanged(
                                resourceInSelection, properties,
                                Constants.RESOURCE_NAME)) {
                            update = true;
                            nameChange = true;
                        }

                        if (resourceManagerRef.isPropValueChanged(
                                resourceInSelection, properties,
                                Constants.RESOURCE_URI)) {
                            // Check whether the new URI is unique.
                            if (!resourceManagerRef.isUriUnique(properties)) {
                                MessageDialog.openError(parent.getShell(),
                                        "Resource URI in use",
                                        "Resource URI is in use. Please try a different URI.");
                                return;
                            }

                            update = true;
                            uriChange = true;
                        }

                        if (resourceManagerRef.isPropValueChanged(
                                resourceInSelection, properties,
                                Constants.RESOURCE_TYPE)) {
                            update = true;
                            typeChange = true;
                        }
                        // Checking whether any changes made in resource
                        // interfaces by
                        // comparing the current interface set and updated
                        // interface set.
                        Set<String> curIfSet = resourceInSelection
                                .getResourceInterfaces();
                        // Adding default interface to local set if removed.
                        updatedIfSet.add(Constants.BASELINE_INTERFACE);
                        if (null != curIfSet && null != updatedIfSet) {
                            if (curIfSet.size() != updatedIfSet.size()) {
                                update = true;
                                interfaceChange = true;
                            } else {
                                Iterator<String> itr = updatedIfSet.iterator();
                                while (itr.hasNext()) {
                                    if (!curIfSet.contains(itr.next())) {
                                        update = true;
                                        interfaceChange = true;
                                        break;
                                    }
                                }
                            }
                        }
                        if (update) {
                            if (uriChange || typeChange || interfaceChange) {
                                if (resourceManagerRef
                                        .isResourceStarted(resourceInSelection)) {
                                    update = MessageDialog.openQuestion(
                                            parent.getShell(),
                                            "Save Details",
                                            "Resource will be restarted to take the changes."
                                                    + " Do you want to continue?");
                                    if (!update) {
                                        return;
                                    }
                                }
                            }
                            try {
                                if (uriChange || nameChange || typeChange)
                                    result = Activator
                                            .getDefault()
                                            .getResourceManager()
                                            .updateResourceProperties(
                                                    resourceManagerRef
                                                            .getCurrentResourceInSelection(),
                                                    properties, uriChange,
                                                    nameChange, typeChange);
                                if (interfaceChange)
                                    result = Activator
                                            .getDefault()
                                            .getResourceManager()
                                            .updateResourceInterfaces(
                                                    resourceInSelection,
                                                    updatedIfSet);

                            } catch (SimulatorException ex) {
                                result = false;
                            }
                            if (!result) {
                                MessageDialog.openInformation(
                                        parent.getShell(), "Operation status",
                                        "Failed to update the resource properties.");

                                // Reset the old property values.
                                properties = getData(resourceManagerRef
                                        .getCurrentResourceInSelection());
                                updateViewer(properties);
                            }
                        }
                    }
                    cancelBtn.setEnabled(false);
                    editBtn.setText("Edit");
                    setEnableEdit(false);
                }
            }
        });

        cancelBtn = new Button(parent, SWT.PUSH);
        cancelBtn.setText("Cancel");
        cancelBtn.setEnabled(false);
        gd = new GridData();
        gd.widthHint = 70;
        cancelBtn.setLayoutData(gd);
        cancelBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                Resource res = resourceManagerRef
                        .getCurrentResourceInSelection();
                if (null != res) {
                    properties = getData(res);
                }
                updateViewer(properties);

                cancelBtn.setEnabled(false);
                editBtn.setText("Edit");
                setEnableEdit(false);
                if (null != updatedIfSet)
                    updatedIfSet.clear();
            }
        });

        // Get the supported interfaces.
        Map<String, String> ifTypesSupported = Utility
                .getResourceInterfaces(SingleResource.class);
        if (null != ifTypesSupported && !ifTypesSupported.isEmpty()) {
            ifTypes = new HashMap<String, String>();
            String key;
            for (Map.Entry<String, String> entry : ifTypesSupported.entrySet()) {
                key = entry.getValue() + " (" + entry.getKey() + ")";
                ifTypes.put(key, entry.getKey());
            }
        }

        addManagerListeners();

        // Check whether there is any resource selected already
        Resource resource = resourceManagerRef.getCurrentResourceInSelection();
        properties = getData(resource);
        if (null != properties) {
            updateViewer(properties);
        }
        updateEditControls(resource);
    }

    private void updateEditControls(Object obj) {
        if (!editBtn.isDisposed() && !cancelBtn.isDisposed()) {

            if (editBtn.getText().equals("Save")) {
                editBtn.setText("Edit");
                setEnableEdit(false);
            }

            if (null == obj) {
                editBtn.setEnabled(false);
            } else {
                editBtn.setEnabled(true);
            }
            cancelBtn.setEnabled(false);
        }
    }

    private List<MetaProperty> getData(Resource resource) {
        if (null != resource) {
            List<MetaProperty> metaPropertyList = resourceManagerRef
                    .getMetaProperties(resource);
            return metaPropertyList;
        } else {
            return null;
        }
    }

    private void updateViewer(List<MetaProperty> metaPropertyList) {
        if (null != tableViewer) {
            Table tbl = tableViewer.getTable();
            if (tbl.isDisposed()) {
                return;
            }
            if (null != metaPropertyList) {
                tableViewer.setInput(metaPropertyList.toArray());
                tbl.setLinesVisible(true);
            } else {
                tbl.removeAll();
                tbl.setLinesVisible(false);
            }
        }
    }

    public void createColumns(final TableViewer tableViewer) {
        TableViewerColumn propName = new TableViewerColumn(tableViewer,
                SWT.NONE);
        propName.getColumn().setWidth(columnWidth[0]);
        propName.getColumn().setText(columnHeaders[0]);
        propName.setLabelProvider(new StyledCellLabelProvider() {
            @Override
            public void update(ViewerCell cell) {
                MetaProperty prop = (MetaProperty) cell.getElement();
                cell.setText(prop.getPropName());
                super.update(cell);
            }
        });

        TableViewerColumn propValue = new TableViewerColumn(tableViewer,
                SWT.NONE);
        propValue.getColumn().setWidth(columnWidth[1]);
        propValue.getColumn().setText(columnHeaders[1]);
        propValue.setLabelProvider(new StyledCellLabelProvider() {
            @Override
            public void update(ViewerCell cell) {
                MetaProperty prop = (MetaProperty) cell.getElement();
                cell.setText(prop.getPropValue());
                super.update(cell);
            }
        });
        propValue.setEditingSupport(new PropValueEditor(tableViewer));
    }

    private void addManagerListeners() {
        UiListenerHandler.getInstance().addResourceSelectionChangedUIListener(
                resourceSelectionChangedListener);
    }

    private static class PropertycontentProvider implements
            IStructuredContentProvider {

        @Override
        public void dispose() {
        }

        @Override
        public void inputChanged(Viewer arg0, Object arg1, Object arg2) {
        }

        @Override
        public Object[] getElements(Object element) {
            return (Object[]) element;
        }
    }

    @Override
    public void dispose() {
        // Unregister the listener
        if (null != resourceSelectionChangedListener) {
            UiListenerHandler.getInstance()
                    .removeResourceSelectionChangedUIListener(
                            resourceSelectionChangedListener);
        }
        super.dispose();
    }

    class PropValueEditor extends EditingSupport {

        private final TableViewer viewer;

        public PropValueEditor(TableViewer viewer) {
            super(viewer);
            this.viewer = viewer;
        }

        @Override
        protected boolean canEdit(Object element) {
            return true;
        }

        @Override
        protected CellEditor getCellEditor(final Object element) {
            if (!getEnableEdit()) {
                return null;
            }
            String propName = ((MetaProperty) element).getPropName();
            CellEditor editor = new TextCellEditor(viewer.getTable());
            if (null != propName && propName.equals(Constants.INTERFACE_TYPES)) {
                editor.setStyle(SWT.READ_ONLY);
                final Text txt = (Text) editor.getControl();
                txt.addModifyListener(new ModifyListener() {
                    @Override
                    public void modifyText(ModifyEvent e) {
                        if (null == updatedIfSet) {
                            return;
                        }
                        // Form the result set of interfaces with check-box that
                        // will be shown in the dialog for editing.
                        Map<String, String> curResInterfaces = new HashMap<String, String>();
                        for (Map.Entry<String, String> entry : ifTypes
                                .entrySet()) {
                            if (updatedIfSet.contains(entry.getValue())) {
                                curResInterfaces.put(entry.getKey(),
                                        entry.getValue());
                            }
                        }

                        // Show the dialog for editing the resource interfaces.
                        UpdateResourceInterfaceDialog ad = new UpdateResourceInterfaceDialog(
                                Display.getDefault().getActiveShell(),
                                curResInterfaces, ifTypes);
                        if (ad.open() == Window.OK) {
                            // Update the local copy of the current resource
                            // interfaces to keep the state for save operation.
                            updatedIfSet.clear();
                            StringBuilder newPropValue = new StringBuilder();
                            for (Map.Entry<String, String> entry : curResInterfaces
                                    .entrySet()) {
                                if (!newPropValue.toString().isEmpty()) {
                                    newPropValue.append(", ");
                                }
                                String value = ifTypes.get(entry.getKey());
                                newPropValue.append(value);

                                updatedIfSet.add(value);
                            }
                            // Update the model
                            MetaProperty prop = (MetaProperty) element;
                            StringBuilder value = new StringBuilder();
                            value.append(Constants.BASELINE_INTERFACE);
                            if (newPropValue.length() > 0) {
                                value.append(", " + newPropValue.toString());
                            }
                            prop.setPropValue(value.toString());

                            // Update the viewer in a separate UI thread.
                            Display.getDefault().asyncExec(new Runnable() {
                                @Override
                                public void run() {
                                    viewer.refresh(element, true);
                                }
                            });
                        }
                    }
                });
            }
            return editor;
        }

        @Override
        protected Object getValue(Object element) {
            return ((MetaProperty) element).getPropValue();
        }

        @Override
        protected void setValue(Object element, Object value) {
            MetaProperty prop = (MetaProperty) element;
            if (prop.getPropName().equals(Constants.INTERFACE_TYPES)) {
                return;
            }
            prop.setPropValue(String.valueOf(value));
            viewer.update(element, null);
        }
    }

    private synchronized Boolean getEnableEdit() {
        return enable_edit;
    }

    private synchronized void setEnableEdit(boolean value) {
        enable_edit = value;
    }

    @Override
    public void setFocus() {
    }
}