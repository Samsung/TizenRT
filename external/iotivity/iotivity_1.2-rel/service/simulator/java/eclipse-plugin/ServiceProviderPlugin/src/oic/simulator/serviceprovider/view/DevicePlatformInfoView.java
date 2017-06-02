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
import org.eclipse.jface.viewers.ColumnLabelProvider;
import org.eclipse.jface.viewers.EditingSupport;
import org.eclipse.jface.viewers.IStructuredContentProvider;
import org.eclipse.jface.viewers.StyledCellLabelProvider;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.jface.viewers.TableViewerColumn;
import org.eclipse.jface.viewers.TextCellEditor;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.jface.viewers.ViewerCell;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.CTabFolder;
import org.eclipse.swt.custom.CTabItem;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Table;
import org.eclipse.ui.part.ViewPart;

import java.util.List;

import oic.simulator.serviceprovider.Activator;
import oic.simulator.serviceprovider.listener.IDeviceInfoListener;
import oic.simulator.serviceprovider.manager.UiListenerHandler;
import oic.simulator.serviceprovider.model.MetaProperty;
import oic.simulator.serviceprovider.utils.Constants;

/**
 * This class is used to set the platform and device information. The
 * information is only one per stack and hence only one instance can be
 * stored/used at a time.
 */
public class DevicePlatformInfoView extends ViewPart {

    public static final String VIEW_ID       = "oic.simulator.serviceprovider.view.devplatinfo";

    private final String[]     columnHeaders = { "Property", "Value" };

    private final Integer[]    columnWidth   = { 150, 150 };

    enum PropertiesType {
        DEVICE, PLATFORM
    }

    private List<MetaProperty>  deviceProperties;
    private List<MetaProperty>  platformProperties;

    private boolean             enableDeviceEdit;
    private boolean             enablePlatformEdit;

    private Button              devEditBtn;
    private Button              devCancelBtn;
    private Button              platEditBtn;
    private Button              platCancelBtn;

    private TableViewer         platformTblViewer;
    private TableViewer         deviceTblViewer;

    private CTabFolder          folder;
    private CTabItem            devicePropTab;
    private CTabItem            platformPropTab;

    private IDeviceInfoListener deviceInfoUIListener;

    public DevicePlatformInfoView() {
        deviceInfoUIListener = new IDeviceInfoListener() {

            @Override
            public void onDeviceInfoFound() {
                Display.getDefault().asyncExec(new Runnable() {

                    @Override
                    public void run() {
                        if (null != deviceTblViewer) {
                            updateViewer(deviceTblViewer, getDevicePropData());
                        }
                    }
                });
            }
        };

        addManagerListeners();
    }

    @Override
    public void createPartControl(Composite parent) {
        parent.setLayout(new GridLayout());
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        parent.setLayoutData(gd);

        // Create a Tab Folder.
        folder = new CTabFolder(parent, SWT.BORDER);
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        folder.setLayoutData(gd);
        folder.setSimple(false);
        folder.setUnselectedCloseVisible(false);
        folder.setUnselectedImageVisible(false);

        createDevicePropertiesTab();

        createPlatformPropertiesTab();

        folder.setSelection(devicePropTab);

        platformProperties = getPlatformPropData();

        updateViewer(platformTblViewer, platformProperties);

        deviceProperties = getDevicePropData();

        updateViewer(deviceTblViewer, deviceProperties);
    }

    private void addManagerListeners() {
        UiListenerHandler.getInstance().addDeviceInfoUIListener(
                deviceInfoUIListener);
    }

    private void createDevicePropertiesTab() {
        devicePropTab = new CTabItem(folder, SWT.NULL);
        devicePropTab.setText("Device");

        // Adding the group to the folder.
        Group propGroup = new Group(folder, SWT.NONE);

        Color color = Display.getDefault().getSystemColor(SWT.COLOR_WHITE);
        propGroup.setBackground(color);

        propGroup.setLayout(new GridLayout(2, false));
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        propGroup.setLayoutData(gd);

        deviceTblViewer = new TableViewer(propGroup, SWT.SINGLE | SWT.H_SCROLL
                | SWT.V_SCROLL | SWT.FULL_SELECTION | SWT.BORDER);

        createColumns(deviceTblViewer, PropertiesType.DEVICE);

        // Make lines and header visible
        final Table table = deviceTblViewer.getTable();
        table.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));
        table.setHeaderVisible(true);
        table.setLinesVisible(true);
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        gd.horizontalSpan = 2;
        table.setLayoutData(gd);

        deviceTblViewer.setContentProvider(new PropertycontentProvider());

        devEditBtn = new Button(propGroup, SWT.PUSH);
        devEditBtn.setText("Edit");
        gd = new GridData();
        gd.widthHint = 50;
        devEditBtn.setLayoutData(gd);
        devEditBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                if (devEditBtn.getText().equals("Edit")) {
                    devCancelBtn.setEnabled(true);
                    devEditBtn.setText("Save");
                    enableDeviceEdit = true;
                } else {
                    if (Activator.getDefault().getResourceManager()
                            .isDeviceInfoValid(deviceProperties)) {
                        Activator.getDefault().getResourceManager()
                                .setDeviceInfo(deviceProperties);
                        devCancelBtn.setEnabled(false);
                        devEditBtn.setText("Edit");
                        enableDeviceEdit = false;
                    } else {
                        MessageDialog.openError(Display.getDefault()
                                .getActiveShell(), "Invalid value",
                                "Property value cannot be empty.");
                    }
                }
            }
        });

        devCancelBtn = new Button(propGroup, SWT.PUSH);
        devCancelBtn.setText("Cancel");
        devCancelBtn.setEnabled(false);
        gd = new GridData();
        gd.widthHint = 70;
        devCancelBtn.setLayoutData(gd);
        devCancelBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                deviceProperties = getDevicePropData();
                updateViewer(deviceTblViewer, deviceProperties);

                devCancelBtn.setEnabled(false);
                devEditBtn.setText("Edit");
                enableDeviceEdit = false;
            }
        });

        devicePropTab.setControl(propGroup);
    }

    private void createPlatformPropertiesTab() {
        platformPropTab = new CTabItem(folder, SWT.NULL);
        platformPropTab.setText("Platform");

        // Adding the group to the folder.
        Group propGroup = new Group(folder, SWT.NONE);

        Color color = Display.getDefault().getSystemColor(SWT.COLOR_WHITE);
        propGroup.setBackground(color);

        propGroup.setLayout(new GridLayout(2, false));
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        propGroup.setLayoutData(gd);

        platformTblViewer = new TableViewer(propGroup, SWT.SINGLE
                | SWT.H_SCROLL | SWT.V_SCROLL | SWT.FULL_SELECTION | SWT.BORDER);

        createColumns(platformTblViewer, PropertiesType.PLATFORM);

        // Make lines and header visible
        final Table table = platformTblViewer.getTable();
        table.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));
        table.setHeaderVisible(true);
        table.setLinesVisible(true);
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        gd.horizontalSpan = 2;
        table.setLayoutData(gd);

        platformTblViewer.setContentProvider(new PropertycontentProvider());

        platEditBtn = new Button(propGroup, SWT.PUSH);
        platEditBtn.setText("Edit");
        gd = new GridData();
        gd.widthHint = 50;
        platEditBtn.setLayoutData(gd);
        platEditBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                if (platEditBtn.getText().equals("Edit")) {
                    platCancelBtn.setEnabled(true);
                    platEditBtn.setText("Save");
                    enablePlatformEdit = true;
                } else {
                    if (Activator.getDefault().getResourceManager()
                            .isPlatformInfoValid(platformProperties)) {
                        Activator.getDefault().getResourceManager()
                                .setPlatformInfo(platformProperties);
                        platCancelBtn.setEnabled(false);
                        platEditBtn.setText("Edit");
                        enablePlatformEdit = false;
                    } else {
                        MessageDialog.openError(Display.getDefault()
                                .getActiveShell(), "Invalid value",
                                "Property value cannot be empty.");
                    }
                }
            }
        });

        platCancelBtn = new Button(propGroup, SWT.PUSH);
        platCancelBtn.setText("Cancel");
        platCancelBtn.setEnabled(false);
        gd = new GridData();
        gd.widthHint = 70;
        platCancelBtn.setLayoutData(gd);
        platCancelBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                platformProperties = getPlatformPropData();
                updateViewer(platformTblViewer, platformProperties);

                platCancelBtn.setEnabled(false);
                platEditBtn.setText("Edit");
                enablePlatformEdit = false;
            }
        });

        platformPropTab.setControl(propGroup);
    }

    public void createColumns(TableViewer tableViewer, PropertiesType propType) {
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
        propValue.setLabelProvider(new ColumnLabelProvider() {
            @Override
            public String getText(Object element) {
                MetaProperty prop = (MetaProperty) element;
                if (null != prop) {
                    return prop.getPropValue();
                } else {
                    return "";
                }
            }
        });
        if (propType == PropertiesType.DEVICE) {
            propValue.setEditingSupport(new PropValueEditor(deviceTblViewer,
                    propType));
        } else {
            propValue.setEditingSupport(new PropValueEditor(platformTblViewer,
                    propType));
        }
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

    private List<MetaProperty> getPlatformPropData() {
        List<MetaProperty> metaPropertyList = Activator.getDefault()
                .getResourceManager().getPlatformInfo();
        return metaPropertyList;
    }

    private List<MetaProperty> getDevicePropData() {
        List<MetaProperty> metaPropertyList = Activator.getDefault()
                .getResourceManager().getDeviceInfo();
        return metaPropertyList;
    }

    private void updateViewer(TableViewer tableViewer,
            List<MetaProperty> metaPropertyList) {
        if (null != tableViewer) {
            Table tbl = tableViewer.getTable();
            if (null != metaPropertyList) {
                tableViewer.setInput(metaPropertyList.toArray());
                if (!tbl.isDisposed()) {
                    tbl.setLinesVisible(true);
                }
            } else {
                if (!tbl.isDisposed()) {
                    tbl.removeAll();
                    tbl.setLinesVisible(false);
                }
            }
        }
    }

    class PropValueEditor extends EditingSupport {

        private final TableViewer    viewer;

        private final PropertiesType propType;

        public PropValueEditor(TableViewer viewer, PropertiesType propType) {
            super(viewer);
            this.viewer = viewer;
            this.propType = propType;
        }

        @Override
        protected boolean canEdit(Object element) {
            return true;
        }

        @Override
        protected CellEditor getCellEditor(Object element) {
            String propName = ((MetaProperty) element).getPropName();
            if (null == propName) {
                return null;
            }

            if (propType == PropertiesType.DEVICE) {
                if (!enableDeviceEdit)
                    return null;
                // Only device name property is editable
                if (!propName.equals(Constants.DEVICE_NAME)) {
                    return null;
                }
            } else {
                if (!enablePlatformEdit) {
                    return null;
                }
            }

            CellEditor editor = new TextCellEditor(viewer.getTable());
            return editor;
        }

        @Override
        protected Object getValue(Object element) {
            return ((MetaProperty) element).getPropValue();
        }

        @Override
        protected void setValue(Object element, Object value) {
            MetaProperty prop = (MetaProperty) element;
            prop.setPropValue(String.valueOf(value));
            viewer.update(element, null);
        }

    }

    @Override
    public void setFocus() {
    }
}