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

import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.operation.IRunnableWithProgress;
import org.eclipse.jface.wizard.IWizardPage;
import org.eclipse.jface.wizard.WizardPage;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.KeyAdapter;
import org.eclipse.swt.events.KeyEvent;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.PlatformUI;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.Date;
import java.util.Set;

import org.oic.simulator.ILogger.Level;
import org.oic.simulator.SimulatorException;
import org.oic.simulator.server.SimulatorResource;
import org.oic.simulator.server.SimulatorResource.Type;

import oic.simulator.serviceprovider.Activator;
import oic.simulator.serviceprovider.model.Resource;
import oic.simulator.serviceprovider.model.SingleResource;
import oic.simulator.serviceprovider.utils.Constants;
import oic.simulator.serviceprovider.utils.Utility;
import oic.simulator.serviceprovider.view.dialogs.MainPage.Option;

/**
 * This class shows UI for creating resources.
 */
public class LoadRamlPage extends WizardPage {

    private Text                   locationTxt;
    private Button                 btnBrowse;
    private Text                   noOfInstancesText;
    private Label                  noOfInstancesLbl;
    private Label                  locationLbl;

    private String                 configFilePath = null;
    private int                    resourceCount;

    private Resource               resource;
    private Set<SingleResource>    multiInstanceResourceSet;

    private SimulatorResource.Type typeOfResource;

    private IProgressMonitor       progressMonitor;

    protected LoadRamlPage() {
        super("Create Resource");
        resourceCount = -1;
    }

    public void initialSetup(SimulatorResource.Type type) {
        this.typeOfResource = type;
        if (!noOfInstancesLbl.isDisposed()) {
            noOfInstancesLbl.setVisible(type == Type.SINGLE);
        }
        if (!noOfInstancesText.isDisposed()) {
            noOfInstancesText.setVisible(type == Type.SINGLE);
        }
    }

    @Override
    public void createControl(Composite parent) {
        setPageComplete(false);
        setTitle(Constants.CREATE_PAGE_TITLE);
        setMessage(Constants.CREATE_PAGE_MESSAGE);
        Composite compContent = new Composite(parent, SWT.NONE);
        GridLayout gridLayout = new GridLayout();
        compContent.setLayout(gridLayout);
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        compContent.setLayoutData(gd);

        Group configGroup = new Group(compContent, SWT.NONE);
        gridLayout = new GridLayout(1, false);
        gridLayout.verticalSpacing = 10;
        gridLayout.marginTop = 5;
        configGroup.setLayout(gridLayout);
        gd = new GridData();
        gd.horizontalAlignment = SWT.FILL;
        gd.grabExcessHorizontalSpace = true;
        configGroup.setLayoutData(gd);
        configGroup.setText("Load RAML File");

        Composite cusConfigComp = new Composite(configGroup, SWT.NONE);
        cusConfigComp.setLayout(new GridLayout(3, false));
        gd = new GridData();
        gd.horizontalAlignment = SWT.FILL;
        gd.grabExcessHorizontalSpace = true;
        cusConfigComp.setLayoutData(gd);

        locationLbl = new Label(cusConfigComp, SWT.NONE);
        locationLbl.setText("Location:");

        locationTxt = new Text(cusConfigComp, SWT.BORDER);
        gd = new GridData();
        gd.minimumWidth = 300;
        gd.horizontalAlignment = SWT.FILL;
        gd.grabExcessHorizontalSpace = true;
        locationTxt.setLayoutData(gd);

        btnBrowse = new Button(cusConfigComp, SWT.NONE);
        btnBrowse.setText("Browse");
        gd = new GridData();
        gd.widthHint = 80;
        gd.horizontalAlignment = SWT.FILL;
        btnBrowse.setLayoutData(gd);
        btnBrowse.setFocus();

        Composite instanceComp = new Composite(compContent, SWT.NONE);
        instanceComp.setLayout(new GridLayout(2, false));
        gd = new GridData();
        gd.horizontalAlignment = SWT.FILL;
        gd.grabExcessHorizontalSpace = true;
        instanceComp.setLayoutData(gd);

        noOfInstancesLbl = new Label(instanceComp, SWT.NONE);
        noOfInstancesLbl.setText("Number of Instances");

        noOfInstancesText = new Text(instanceComp, SWT.BORDER);
        gd = new GridData();
        gd.widthHint = 50;
        gd.horizontalAlignment = SWT.FILL;
        noOfInstancesText.setLayoutData(gd);

        // Adding the default value. It can be changed by the user.
        noOfInstancesText.setText("1");

        addUIListeners();

        setControl(compContent);
    }

    private void addUIListeners() {
        btnBrowse.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                FileDialog fileDialog = new FileDialog(PlatformUI
                        .getWorkbench().getDisplay().getActiveShell(), SWT.NONE);
                fileDialog
                        .setFilterExtensions(Constants.BROWSE_RAML_FILTER_EXTENSIONS);
                String configFileAbsolutePath = fileDialog.open();
                if (null != configFileAbsolutePath)
                    locationTxt.setText(configFileAbsolutePath);
            }
        });

        locationTxt.addModifyListener(new ModifyListener() {
            @Override
            public void modifyText(ModifyEvent e) {
                configFilePath = locationTxt.getText();
                getWizard().getContainer().updateButtons();
            }
        });

        noOfInstancesText.addListener(SWT.Verify, new Listener() {
            @Override
            public void handleEvent(Event e) {
                String string = e.text;
                char[] chars = new char[string.length()];
                string.getChars(0, chars.length, chars, 0);
                for (int i = 0; i < chars.length; i++) {
                    if (!('0' <= chars[i] && chars[i] <= '9')) {
                        e.doit = false;
                        return;
                    }
                }
            }
        });

        noOfInstancesText.addKeyListener(new KeyAdapter() {
            @Override
            public void keyReleased(KeyEvent e) {
                getWizard().getContainer().updateButtons();
            }
        });
    }

    @Override
    public boolean canFlipToNextPage() {
        boolean done = false;
        try {
            resourceCount = Integer.parseInt(noOfInstancesText.getText());
        } catch (Exception e) {
            resourceCount = -1;
        }
        if (null != configFilePath && configFilePath.trim().length() > 0) {
            if (resourceCount >= 1) {
                done = true;
            }
        }
        return done;
    }

    public boolean isSelectionDone() {
        boolean done = false;
        try {
            resourceCount = Integer.parseInt(noOfInstancesText.getText());
        } catch (Exception e) {
            resourceCount = -1;
        }
        if (null != configFilePath && configFilePath.trim().length() > 0) {
            if (resourceCount >= 1) {
                done = true;
            }
        }
        return done;
    }

    public boolean isMultiResourceCreation() {
        if (typeOfResource == Type.SINGLE && resourceCount > 1) {
            return true;
        }
        return false;
    }

    @Override
    public IWizardPage getNextPage() {
        // Validate the file path.
        FileInputStream fileStream = null;
        try {
            fileStream = new FileInputStream(configFilePath);
        } catch (FileNotFoundException e) {
            MessageDialog
                    .openError(getShell(), "Invalid File",
                            "File doesn't exist. Either the file path or file name is invalid.");
            return null;
        } finally {
            if (null != fileStream)
                try {
                    fileStream.close();
                } catch (IOException e) {
                    Activator
                            .getDefault()
                            .getLogManager()
                            .log(Level.ERROR.ordinal(),
                                    new Date(),
                                    "There is an error while closing the file stream.\n"
                                            + Utility.getSimulatorErrorString(
                                                    e, null));
                }
        }

        // Checking the resource count
        if ((Activator.getDefault().getResourceManager().getResourceCount() + resourceCount) > Constants.MAX_RESOURCE_COUNT) {
            MessageDialog
                    .openInformation(Display.getDefault().getActiveShell(),
                            "Resource limit exceeded",
                            "Exceeded the limit of resources that can exist in the server.");
            return null;
        }

        IWizardPage nextPage;
        String resName;
        String resURI;
        String resType;
        final CreateResourceWizard wizard = ((CreateResourceWizard) getWizard());
        if (isMultiResourceCreation()) {
            final int[] resCreatedCount = new int[1];
            try {
                getContainer().run(true, true, new IRunnableWithProgress() {

                    @Override
                    public void run(final IProgressMonitor monitor)
                            throws InvocationTargetException,
                            InterruptedException {
                        progressMonitor = monitor;
                        try {
                            monitor.beginTask(
                                    "Single Resource Creation(multi-instance) With RAML",
                                    wizard.getLoadRamlPage().getResourceCount());
                            resCreatedCount[0] = createMultiInstanceSingleResourceWithRAML();

                        } finally {
                            monitor.done();
                        }
                    }
                });
            } catch (InvocationTargetException e) {
                Activator.getDefault().getLogManager()
                        .log(Level.ERROR.ordinal(), new Date(), e.getMessage());
                e.printStackTrace();
            } catch (InterruptedException e) {
                Activator.getDefault().getLogManager()
                        .log(Level.ERROR.ordinal(), new Date(), e.getMessage());
                e.printStackTrace();
            }

            if (0 == resCreatedCount[0]) {
                wizard.setStatus("Failed to create Resources.");
                wizard.getWizardDialog().close();
                return null;
            }

            boolean canceled = false;
            if (null != progressMonitor && progressMonitor.isCanceled()) {
                canceled = true;
            }
            progressMonitor = null;
            if (canceled) {
                return null;
            }

            // It is guaranteed that will be at least one resource at this
            // point.
            SingleResource resourceInstance = (SingleResource) multiInstanceResourceSet
                    .toArray()[0];
            resName = resourceInstance.getResourceName();
            resType = resourceInstance.getResourceType();
            resURI = resourceInstance.getResourceURI();
            UpdateMultiInstanceCreationPropertiesPage updatePageRef = wizard
                    .getUpdateMultiInstanceCreationPropPage();
            updatePageRef.setResName(resName);
            updatePageRef.setResType(resType);
            nextPage = updatePageRef;
        } else {
            try {
                getContainer().run(true, false, new IRunnableWithProgress() {
                    @Override
                    public void run(IProgressMonitor monitor)
                            throws InvocationTargetException,
                            InterruptedException {
                        try {
                            monitor.beginTask("Resource Creation With RAML", 2);
                            monitor.worked(1);
                            resource = Activator.getDefault()
                                    .getResourceManager()
                                    .createResourceByRAML(configFilePath);
                            monitor.worked(1);
                        } catch (SimulatorException e) {
                            wizard.setStatus("Failed to create resource.\n"
                                    + Utility.getSimulatorErrorString(e, null));
                        } finally {
                            monitor.done();
                        }
                    }
                });
            } catch (InvocationTargetException e) {
                Activator.getDefault().getLogManager()
                        .log(Level.ERROR.ordinal(), new Date(), e.getMessage());
                e.printStackTrace();
            } catch (InterruptedException e) {
                Activator.getDefault().getLogManager()
                        .log(Level.ERROR.ordinal(), new Date(), e.getMessage());
                e.printStackTrace();
            }
            if (null == resource) {
                wizard.setStatus("Failed to create Resource.");
                wizard.getWizardDialog().close();
                return null;
            } else {
                // Checking whether the resource is of type single.
                Option intendedResource = wizard.getMainPage().getOption();
                if ((intendedResource == Option.SIMPLE_FROM_RAML && !(resource instanceof SingleResource))) {
                    MessageDialog
                            .openError(
                                    getShell(),
                                    "Invalid RAML",
                                    "Uploaded RAML is not of simple type. "
                                            + "Please upload the proper RAML of simple type.");
                    return null;
                }
            }
            resName = resource.getResourceName();
            resURI = resource.getResourceURI();
            resType = resource.getResourceType();
            UpdatePropertiesPage updatePageRef = wizard.getUpdatePropPage();
            updatePageRef.setResName(resName);
            updatePageRef.setResURI(resURI);
            updatePageRef.setResType(resType);
            nextPage = updatePageRef;
        }
        return nextPage;
    }

    public int createMultiInstanceSingleResourceWithRAML() {
        int resCreatedCount = 0;
        try {
            multiInstanceResourceSet = Activator
                    .getDefault()
                    .getResourceManager()
                    .createSingleResourceMultiInstances(configFilePath,
                            resourceCount, progressMonitor);
            if (null != progressMonitor && progressMonitor.isCanceled()) {
                try {
                    Activator.getDefault().getResourceManager()
                            .removeSingleResources(multiInstanceResourceSet);
                } catch (SimulatorException e) {
                    Activator
                            .getDefault()
                            .getLogManager()
                            .log(Level.ERROR.ordinal(),
                                    new Date(),
                                    "There is an error while handling the cancellation request.\n"
                                            + Utility.getSimulatorErrorString(
                                                    e, null));
                }
                return 0;
            }
            if (null != multiInstanceResourceSet)
                resCreatedCount = multiInstanceResourceSet.size();
        } catch (SimulatorException e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(),
                            new Date(),
                            "Failed to create resource(s).\n"
                                    + Utility.getSimulatorErrorString(e, null));
        }
        return resCreatedCount;
    }

    public String getConfigFilePath() {
        return configFilePath;
    }

    public int getResourceCount() {
        return resourceCount;
    }

    public Resource getResource() {
        return resource;
    }

    public void setResource(Resource resource) {
        this.resource = resource;
    }

    public Set<SingleResource> getMultiInstanceResourceSet() {
        return multiInstanceResourceSet;
    }

    public void setMultiInstanceResourceSet(Set<SingleResource> resource) {
        multiInstanceResourceSet = resource;
    }
}