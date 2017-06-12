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
import org.eclipse.jface.viewers.ISelectionChangedListener;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.LabelProvider;
import org.eclipse.jface.viewers.SelectionChangedEvent;
import org.eclipse.jface.viewers.StructuredSelection;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.jface.window.Window;
import org.eclipse.jface.wizard.WizardDialog;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.MenuAdapter;
import org.eclipse.swt.events.MenuEvent;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.widgets.MenuItem;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.dialogs.FilteredTree;
import org.eclipse.ui.dialogs.PatternFilter;
import org.eclipse.ui.part.ViewPart;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.oic.simulator.ILogger.Level;
import org.oic.simulator.SimulatorException;
import org.oic.simulator.server.SimulatorResource.AutoUpdateType;

import oic.simulator.serviceprovider.Activator;
import oic.simulator.serviceprovider.listener.IPropertiesChangedListener;
import oic.simulator.serviceprovider.listener.IResourceListChangedListener;
import oic.simulator.serviceprovider.manager.ResourceManager;
import oic.simulator.serviceprovider.manager.UiListenerHandler;
import oic.simulator.serviceprovider.model.AutomationSettingHelper;
import oic.simulator.serviceprovider.model.Resource;
import oic.simulator.serviceprovider.model.ResourceType;
import oic.simulator.serviceprovider.model.SingleResource;
import oic.simulator.serviceprovider.utils.Constants;
import oic.simulator.serviceprovider.utils.Utility;
import oic.simulator.serviceprovider.view.dialogs.AutomationSettingDialog;
import oic.simulator.serviceprovider.view.dialogs.CreateResourceWizard;
import oic.simulator.serviceprovider.view.dialogs.DeleteResourceWizard;
import oic.simulator.serviceprovider.view.dialogs.MainPage;
import oic.simulator.serviceprovider.view.dialogs.MainPage.Option;
import oic.simulator.serviceprovider.view.dialogs.ResourceWizardDialog;

/**
 * This class manages and shows the resource manager view in the perspective.
 */
public class ResourceManagerView extends ViewPart {

    public static final String           VIEW_ID = "oic.simulator.serviceprovider.view.resourcemanager";

    private Button                       createButton;
    private Button                       deleteButton;

    private TreeViewer                   singleResTreeViewer;

    private IResourceListChangedListener resourceListChangedListener;

    private IPropertiesChangedListener   resourcePropertiesChangedListener;

    private ResourceManager              resourceManager;

    public ResourceManagerView() {

        resourceManager = Activator.getDefault().getResourceManager();

        resourceListChangedListener = new IResourceListChangedListener() {

            @Override
            public void onResourceCreation(final ResourceType type) {
                Display.getDefault().syncExec(new Runnable() {

                    @Override
                    public void run() {
                        if (type == ResourceType.SINGLE) {
                            if (null != singleResTreeViewer) {
                                singleResTreeViewer.refresh();
                            }
                        }

                        // Trigger the visibility of delete button
                        changeDeleteVisibility();
                    }
                });
            }

            @Override
            public void onResourceDeletion(final ResourceType type) {
                Display.getDefault().asyncExec(new Runnable() {

                    @Override
                    public void run() {
                        switch (type) {
                            case SINGLE:
                                if (null != singleResTreeViewer)
                                    singleResTreeViewer.refresh();
                                break;
                        }

                        // Trigger the visibility of delete button
                        changeDeleteVisibility();
                    }
                });
            }

            @Override
            public void onResourceListUpdate(final ResourceType type) {
                Display.getDefault().asyncExec(new Runnable() {

                    @Override
                    public void run() {
                        switch (type) {
                            case SINGLE:
                                if (null != singleResTreeViewer)
                                    singleResTreeViewer.refresh();
                                break;
                        }
                    }
                });
            }
        };

        resourcePropertiesChangedListener = new IPropertiesChangedListener() {

            @Override
            public void onResourcePropertyChange() {
                Display.getDefault().asyncExec(new Runnable() {
                    @Override
                    public void run() {
                        if (null != singleResTreeViewer)
                            singleResTreeViewer.refresh();
                    }
                });
            }
        };
    }

    public void changeDeleteVisibility() {
        boolean visibility = resourceManager.isAnyResourceExist();
        if (null != deleteButton && !deleteButton.isDisposed()) {
            deleteButton.setEnabled(visibility);
        }
    }

    @Override
    public void createPartControl(Composite parent) {
        Composite compContent = new Composite(parent, SWT.NONE);
        GridLayout baseLayout = new GridLayout(1, false);
        compContent.setLayout(baseLayout);

        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        compContent.setLayoutData(gd);

        Composite buttonComp = new Composite(compContent, SWT.NONE);
        buttonComp.setLayout(new GridLayout(2, false));

        gd = new GridData();
        gd.horizontalAlignment = SWT.FILL;
        gd.grabExcessHorizontalSpace = true;

        buttonComp.setLayoutData(gd);

        createButton = new Button(buttonComp, SWT.PUSH);
        createButton.setText("Create");
        createButton.setToolTipText("Create Simulator Resource(s)");

        gd = new GridData();
        gd.widthHint = 90;
        createButton.setLayoutData(gd);

        deleteButton = new Button(buttonComp, SWT.PUSH);
        deleteButton.setText("Delete");
        deleteButton.setToolTipText("Delete Simulator Resource(s)");

        gd = new GridData();
        gd.widthHint = 90;
        deleteButton.setLayoutData(gd);

        createSimpleResourcesArea(compContent);

        addUIListeners();

        addManagerListeners();

        // If there is at least one resource exist, then enable the delete
        // resource button
        changeDeleteVisibility();
    }

    private void createSimpleResourcesArea(Composite compContent) {
        // Create a group to show all the discovered resources.
        // Adding the group to the folder.
        Group resourceGroup = new Group(compContent, SWT.NONE);

        Color color = Display.getDefault().getSystemColor(SWT.COLOR_WHITE);
        resourceGroup.setBackground(color);

        resourceGroup.setLayout(new GridLayout(1, false));
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        resourceGroup.setLayoutData(gd);

        PatternFilter filter = new PatternFilter();
        FilteredTree filteredTree = new FilteredTree(resourceGroup,
                SWT.H_SCROLL | SWT.V_SCROLL | SWT.SINGLE, filter, true);
        singleResTreeViewer = filteredTree.getViewer();
        singleResTreeViewer.getTree().setLayoutData(
                new GridData(SWT.FILL, SWT.FILL, true, true));
        singleResTreeViewer
                .setContentProvider(new SingleResourceContentProvider());
        singleResTreeViewer.setLabelProvider(new TreeLabelProvider());
        singleResTreeViewer.setInput(new Object());

        singleResTreeViewer
                .addSelectionChangedListener(new ISelectionChangedListener() {
                    @Override
                    public void selectionChanged(SelectionChangedEvent e) {
                        IStructuredSelection selection = (IStructuredSelection) e
                                .getSelection();
                        if (null == selection) {
                            return;
                        }

                        Resource res = null;
                        int size = selection.size();
                        if (size == 1) {
                            res = (Resource) selection.getFirstElement();
                        }
                        resourceManager.resourceSelectionChanged(res);
                    }
                });

        addMenuToSimpleResources();
    }

    private void addMenuToSimpleResources() {
        if (null != singleResTreeViewer) {
            final Tree resourceTreeHead = singleResTreeViewer.getTree();
            if (null != resourceTreeHead) {
                // Below code creates menu entries and shows them on right
                // clicking a resource
                final Menu menu = new Menu(resourceTreeHead);
                resourceTreeHead.setMenu(menu);
                menu.addMenuListener(new MenuAdapter() {
                    @Override
                    public void menuShown(MenuEvent e) {
                        // Clear existing menu items
                        MenuItem[] items = menu.getItems();
                        for (int index = 0; index < items.length; index++) {
                            items[index].dispose();
                        }

                        IStructuredSelection selection = ((IStructuredSelection) singleResTreeViewer
                                .getSelection());
                        final SingleResource resource = (SingleResource) selection
                                .getFirstElement();
                        if (null == resource) {
                            return;
                        }

                        addAutomationMenu(menu, resource);

                        // Menu to remove the resource.
                        MenuItem deleteResource = new MenuItem(menu, SWT.NONE);
                        deleteResource.setText("Delete");
                        deleteResource
                                .addSelectionListener(new SelectionAdapter() {
                                    @Override
                                    public void widgetSelected(SelectionEvent e) {
                                        try {
                                            resourceManager
                                                    .removeResource(resource);
                                            singleResTreeViewer.refresh();

                                            resourceManager
                                                    .resourceSelectionChanged(null);
                                        } catch (SimulatorException e1) {
                                            MessageDialog
                                                    .openInformation(Display
                                                            .getDefault()
                                                            .getActiveShell(),
                                                            "Deletion Failed",
                                                            "Failed to delete the resource.");
                                        }
                                        changeDeleteVisibility();
                                    }
                                });
                    }
                });
            }
        }
    }

    private void addAutomationMenu(final Menu menu,
            final SingleResource selectedResource) {
        MenuItem startItem = new MenuItem(menu, SWT.NONE);
        startItem.setText(Constants.START_RESOURCE_AUTOMATION);
        startItem.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                // Block starting resource level
                // automation if any attribute level
                // automation is in progress for the
                // selected resource
                boolean started = resourceManager
                        .isAttributeAutomationStarted(selectedResource);
                if (started) {
                    MessageDialog
                            .openInformation(
                                    Display.getDefault().getActiveShell(),
                                    "Attribute automation is in progress",
                                    "Attribute level automation for this resource is already in progress!!!\nPlease stop all "
                                            + "running attribute level automations to start resource level automation.");
                } else {
                    // Start the automation
                    // Fetch the settings data
                    List<AutomationSettingHelper> automationSettings;
                    automationSettings = AutomationSettingHelper
                            .getAutomationSettings(null);

                    // Open the settings dialog
                    AutomationSettingDialog dialog = new AutomationSettingDialog(
                            Activator.getDefault().getWorkbench()
                                    .getActiveWorkbenchWindow().getShell(),
                            automationSettings);
                    dialog.create();
                    if (dialog.open() == Window.OK) {
                        String automationType = dialog.getAutomationType();
                        String updateFreq = dialog.getUpdateFrequency();

                        AutoUpdateType autoType = AutoUpdateType
                                .valueOf(automationType);
                        int updFreq = Utility
                                .getUpdateIntervalFromString(updateFreq);
                        boolean status = resourceManager
                                .startResourceAutomationUIRequest(autoType,
                                        updFreq, selectedResource);
                        if (!status) {
                            String statusMsg = "Automation request failed!!!";
                            MessageDialog.openInformation(Display.getDefault()
                                    .getActiveShell(), "Automation Status",
                                    statusMsg);
                        }
                    }
                }
            }
        });

        MenuItem stopItem = new MenuItem(menu, SWT.NONE);
        stopItem.setText(Constants.STOP_RESOURCE_AUTOMATION);
        stopItem.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                boolean status = resourceManager
                        .stopResourceAutomationUIRequest(selectedResource);
                if (!status) {
                    String statusMsg = "Automation stop failed.";
                    MessageDialog.openInformation(Display.getDefault()
                            .getActiveShell(), "Automation Status", statusMsg);
                }
            }
        });

        // Set the initial visibility of menu items
        boolean status = resourceManager
                .isResourceAutomationStarted(selectedResource);
        startItem.setEnabled(!status);
        stopItem.setEnabled(status);
    }

    private void addUIListeners() {

        createButton.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                if (resourceManager.getResourceCount() >= Constants.MAX_RESOURCE_COUNT) {
                    MessageDialog.openInformation(Display.getDefault()
                            .getActiveShell(), "Resource limit exceeded",
                            Constants.RESOURCE_LIMIT_EXCEEDED_MSG);
                    return;
                }
                PlatformUI.getWorkbench().getDisplay().syncExec(new Runnable() {

                    @Override
                    public void run() {
                        CreateResourceWizard createWizard = new CreateResourceWizard();
                        ResourceWizardDialog wizardDialog = new ResourceWizardDialog(
                                Display.getDefault().getActiveShell(),
                                createWizard);
                        createWizard.setWizardDialog(wizardDialog);
                        int open = wizardDialog.open();
                        if (open == WizardDialog.OK
                                || createWizard.isDlgForceClosed()) {

                            MainPage mainPage = createWizard.getMainPage();
                            if (null == mainPage) {
                                Activator
                                        .getDefault()
                                        .getLogManager()
                                        .log(Level.ERROR.ordinal(), new Date(),
                                                "There is an error while reading information from the UI wizard.\n");
                                return;
                            }

                            Option option = mainPage.getOption();
                            if (option == Option.SIMPLE_FROM_RAML
                                    || option == Option.SIMPLE) {
                                SingleResource res = (SingleResource) createWizard
                                        .getCreatedResource();
                                if (null != res) {
                                    boolean canSelect = true;
                                    if (option == Option.SIMPLE_FROM_RAML
                                            && createWizard.getResourceCount() > 1) {
                                        canSelect = false;
                                    }
                                    if (canSelect) {
                                        singleResTreeViewer.setSelection(
                                                new StructuredSelection(res),
                                                true);
                                        resourceManager
                                                .resourceSelectionChanged(res);
                                    }
                                }
                            }

                            MessageDialog.openInformation(Display.getDefault()
                                    .getActiveShell(),
                                    "Resource Creation Status", createWizard
                                            .getStatus());
                        }
                    }
                });
            }
        });

        deleteButton.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                PlatformUI.getWorkbench().getDisplay().syncExec(new Runnable() {

                    @Override
                    public void run() {
                        boolean exist = resourceManager.isAnyResourceExist();
                        if (!exist) {
                            return;
                        }
                        DeleteResourceWizard deleteWizard = new DeleteResourceWizard();
                        ResourceWizardDialog wizardDialog = new ResourceWizardDialog(
                                PlatformUI.getWorkbench().getDisplay()
                                        .getActiveShell(), deleteWizard);
                        int open = wizardDialog.open();
                        if (open == Window.OK) {
                            singleResTreeViewer.refresh();

                            if (!deleteWizard.getStatus()) {
                                MessageDialog
                                        .openInformation(Display.getDefault()
                                                .getActiveShell(),
                                                "Resource Deletion Failed",
                                                "Failed to delete the resources. Please try again.");
                            }
                            changeDeleteVisibility();
                        }
                    }
                });
            }
        });
    }

    public void addManagerListeners() {
        UiListenerHandler.getInstance().addResourceListChangedUIListener(
                resourceListChangedListener);
        UiListenerHandler.getInstance().addResourcePropertiesChangedUIListener(
                resourcePropertiesChangedListener);
    }

    @Override
    public void setFocus() {
    }

    @Override
    public void dispose() {
        // Unregister the listener
        if (null != resourceListChangedListener) {
            UiListenerHandler.getInstance()
                    .removeResourceListChangedUIListener(
                            resourceListChangedListener);
            resourceManager.resourceSelectionChanged(null);
        }
        if (null != resourcePropertiesChangedListener) {
            UiListenerHandler.getInstance()
                    .removeResourcePropertiesChangedUIListener(
                            resourcePropertiesChangedListener);
        }
        super.dispose();
    }
}

class SingleResourceContentProvider implements ITreeContentProvider {

    @Override
    public void dispose() {
    }

    @Override
    public void inputChanged(Viewer arg0, Object arg1, Object arg2) {
    }

    @Override
    public Object[] getChildren(Object parent) {
        return null;
    }

    @Override
    public Object[] getElements(Object parent) {
        List<SingleResource> resList;
        resList = Activator.getDefault().getResourceManager()
                .getSingleResourceList();
        if (null == resList) {
            resList = new ArrayList<SingleResource>();
        }
        return resList.toArray();
    }

    @Override
    public Object getParent(Object child) {
        return null;
    }

    @Override
    public boolean hasChildren(Object parent) {
        return false;
    }
}

class TreeLabelProvider extends LabelProvider {
    @Override
    public String getText(Object element) {
        if (element instanceof Resource) {
            Resource res = (Resource) element;
            return res.getResourceName();
        }
        return null;
    }

    @Override
    public Image getImage(Object element) {
        if (element instanceof SingleResource) {
            return Activator.getDefault().getImageRegistry()
                    .get(Constants.SINGLE_RESOURCE);
        }
        return null;
    }
}