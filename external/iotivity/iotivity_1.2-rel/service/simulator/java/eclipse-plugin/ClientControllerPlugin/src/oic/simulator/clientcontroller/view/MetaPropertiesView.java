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

import org.eclipse.jface.viewers.ColumnLabelProvider;
import org.eclipse.jface.viewers.IStructuredContentProvider;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.jface.viewers.TableViewerColumn;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.CTabFolder;
import org.eclipse.swt.custom.CTabItem;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Table;
import org.eclipse.ui.part.ViewPart;

import java.util.List;

import oic.simulator.clientcontroller.Activator;
import oic.simulator.clientcontroller.listener.IDevicePlatformInfoUIListener;
import oic.simulator.clientcontroller.listener.IResourceSelectionChangedUIListener;
import oic.simulator.clientcontroller.manager.ResourceManager;
import oic.simulator.clientcontroller.manager.UiListenerHandler;
import oic.simulator.clientcontroller.remoteresource.MetaProperty;
import oic.simulator.clientcontroller.remoteresource.RemoteResource;

/**
 * This class manages and shows the meta properties view in the perspective.
 */
public class MetaPropertiesView extends ViewPart {

    public static final String                  VIEW_ID       = "oic.simulator.clientcontroller.view.metaproperties";

    private TableViewer                         defaultTblViewer;
    private TableViewer                         deviceTblViewer;
    private TableViewer                         platformTblViewer;

    private final String[]                      columnHeaders = { "Property",
            "Value"                                          };

    private final Integer[]                     columnWidth   = { 150, 150 };

    private IResourceSelectionChangedUIListener resourceSelectionChangedListener;
    private IDevicePlatformInfoUIListener       devicePlatformInfoUIListener;

    private ResourceManager                     resourceManager;

    private CTabFolder                          folder;
    private CTabItem                            defaultPropTab;
    private CTabItem                            devicePropTab;
    private CTabItem                            platformPropTab;

    public MetaPropertiesView() {

        resourceManager = Activator.getDefault().getResourceManager();

        resourceSelectionChangedListener = new IResourceSelectionChangedUIListener() {

            @Override
            public void onResourceSelectionChange(final RemoteResource resource) {
                Display.getDefault().asyncExec(new Runnable() {

                    @Override
                    public void run() {
                        updateUI(resource);
                    }
                });
            }
        };

        devicePlatformInfoUIListener = new IDevicePlatformInfoUIListener() {

            @Override
            public void onPlatformInfoFound() {
                Display.getDefault().asyncExec(new Runnable() {

                    @Override
                    public void run() {
                        if (null != platformTblViewer) {
                            updateViewer(platformTblViewer,
                                    getPlatformPropData());
                        }
                    }
                });

            }

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
    }

    private void updateUI(final RemoteResource resource) {
        if (null != defaultTblViewer) {
            updateViewer(defaultTblViewer, getDefaultPropData(resource));
        }
        if (null != deviceTblViewer) {
            updateViewer(deviceTblViewer, getDevicePropData());
        }
        if (null != platformTblViewer) {
            updateViewer(platformTblViewer, getPlatformPropData());
        }
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

        createDefaultPropertiesTab();

        createDevicePropertiesTab();

        createPlatformPropertiesTab();

        folder.setSelection(defaultPropTab);

        addManagerListeners();

        // Check whether there is any resource selected already
        updateUI(null);
    }

    private void createDefaultPropertiesTab() {
        defaultPropTab = new CTabItem(folder, SWT.NULL);
        defaultPropTab.setText("Default");

        // Adding the group to the folder.
        Group propGroup = new Group(folder, SWT.NONE);

        Color color = Display.getDefault().getSystemColor(SWT.COLOR_WHITE);
        propGroup.setBackground(color);

        propGroup.setLayout(new GridLayout());
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        propGroup.setLayoutData(gd);

        defaultTblViewer = new TableViewer(propGroup, SWT.SINGLE | SWT.H_SCROLL
                | SWT.V_SCROLL | SWT.FULL_SELECTION | SWT.BORDER);

        createColumns(defaultTblViewer);

        // Make lines and header visible
        final Table table = defaultTblViewer.getTable();
        table.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));
        table.setHeaderVisible(true);
        table.setLinesVisible(true);

        defaultTblViewer.setContentProvider(new PropertycontentProvider());

        defaultPropTab.setControl(propGroup);
    }

    private void createDevicePropertiesTab() {
        devicePropTab = new CTabItem(folder, SWT.NULL);
        devicePropTab.setText("Device");

        // Adding the group to the folder.
        Group propGroup = new Group(folder, SWT.NONE);

        Color color = Display.getDefault().getSystemColor(SWT.COLOR_WHITE);
        propGroup.setBackground(color);

        propGroup.setLayout(new GridLayout());
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        propGroup.setLayoutData(gd);

        deviceTblViewer = new TableViewer(propGroup, SWT.SINGLE | SWT.H_SCROLL
                | SWT.V_SCROLL | SWT.FULL_SELECTION | SWT.BORDER);

        createColumns(deviceTblViewer);

        // Make lines and header visible
        final Table table = deviceTblViewer.getTable();
        table.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));
        table.setHeaderVisible(true);
        table.setLinesVisible(true);

        deviceTblViewer.setContentProvider(new PropertycontentProvider());

        devicePropTab.setControl(propGroup);
    }

    private void createPlatformPropertiesTab() {
        platformPropTab = new CTabItem(folder, SWT.NULL);
        platformPropTab.setText("Platform");

        // Adding the group to the folder.
        Group propGroup = new Group(folder, SWT.NONE);

        Color color = Display.getDefault().getSystemColor(SWT.COLOR_WHITE);
        propGroup.setBackground(color);

        propGroup.setLayout(new GridLayout());
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        propGroup.setLayoutData(gd);

        platformTblViewer = new TableViewer(propGroup, SWT.SINGLE
                | SWT.H_SCROLL | SWT.V_SCROLL | SWT.FULL_SELECTION | SWT.BORDER);

        createColumns(platformTblViewer);

        // Make lines and header visible
        final Table table = platformTblViewer.getTable();
        table.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));
        table.setHeaderVisible(true);
        table.setLinesVisible(true);

        platformTblViewer.setContentProvider(new PropertycontentProvider());

        platformPropTab.setControl(propGroup);
    }

    private List<MetaProperty> getDefaultPropData(RemoteResource resource) {
        if (null == resource) {
            resource = Activator.getDefault().getResourceManager()
                    .getCurrentResourceInSelection();
        }
        List<MetaProperty> metaPropertyList = resourceManager
                .getDefaultProperties(resource);
        return metaPropertyList;
    }

    private List<MetaProperty> getDevicePropData() {
        List<MetaProperty> metaPropertyList = resourceManager
                .getDeviceProperties();
        return metaPropertyList;
    }

    private List<MetaProperty> getPlatformPropData() {
        List<MetaProperty> metaPropertyList = resourceManager
                .getPlatformProperties();
        return metaPropertyList;
    }

    private void updateViewer(TableViewer tblViewer,
            List<MetaProperty> metaPropertyList) {
        if (null != tblViewer) {
            Table tbl = tblViewer.getTable();
            if (null != metaPropertyList) {
                tblViewer.setInput(metaPropertyList.toArray());
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

    public void createColumns(TableViewer tableViewer) {
        TableViewerColumn propName = new TableViewerColumn(tableViewer,
                SWT.NONE);
        propName.getColumn().setWidth(columnWidth[0]);
        propName.getColumn().setText(columnHeaders[0]);
        propName.setLabelProvider(new ColumnLabelProvider() {
            @Override
            public String getText(Object element) {
                MetaProperty prop = (MetaProperty) element;
                if (null != prop) {
                    return prop.getPropName();
                } else {
                    return "";
                }
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
    }

    private void addManagerListeners() {
        UiListenerHandler.getInstance().addResourceSelectionChangedUIListener(
                resourceSelectionChangedListener);
        UiListenerHandler.getInstance().addDevicePlatformInfoUIListener(
                devicePlatformInfoUIListener);
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

    @Override
    public void setFocus() {
    }
}