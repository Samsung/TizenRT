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

import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.Path;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.operation.IRunnableWithProgress;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.wizard.IWizardPage;
import org.eclipse.jface.wizard.Wizard;
import org.eclipse.jface.wizard.WizardDialog;
import org.eclipse.swt.widgets.Display;

import java.lang.reflect.InvocationTargetException;
import java.net.URL;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

import org.oic.simulator.ILogger.Level;
import org.oic.simulator.SimulatorException;
import org.oic.simulator.SimulatorResourceAttribute;

import oic.simulator.serviceprovider.Activator;
import oic.simulator.serviceprovider.manager.UiListenerHandler;
import oic.simulator.serviceprovider.model.AttributeHelper;
import oic.simulator.serviceprovider.model.Resource;
import oic.simulator.serviceprovider.model.ResourceType;
import oic.simulator.serviceprovider.model.SingleResource;
import oic.simulator.serviceprovider.utils.Constants;
import oic.simulator.serviceprovider.utils.Utility;
import oic.simulator.serviceprovider.view.dialogs.MainPage.Option;

/**
 * This class creates a UI wizard for create resource operation.
 */
public class CreateResourceWizard extends Wizard {

    private MainPage                                  mainPage;
    private SimpleResourceBasicDetailsPage            simpleResourceBasicDetailsPage;
    private SimpleResourceAddAttributePage            simpleResourceAddAttributePage;
    private SimpleResourceOtherDetailsPage            simpleResourceOtherDetailsPage;
    private LoadRamlPage                              loadRamlPage;
    private UpdatePropertiesPage                      updatePropPage;
    private UpdateMultiInstanceCreationPropertiesPage updateMultiInstanceCreationPropPage;

    private String                                    status;

    private WizardDialog                              wizDialog;
    private boolean                                   dlgForceClosed;

    private Resource                                  createdResource;
    private Set<SingleResource>                       multiInstanceResourceSet;

    public CreateResourceWizard() {
        setWindowTitle("Create resources");
        IPath path = new Path("/icons/oic_logo_64x64.png");
        URL find = FileLocator.find(Activator.getDefault().getBundle(), path,
                null);
        setDefaultPageImageDescriptor(ImageDescriptor.createFromURL(find));

        setNeedsProgressMonitor(true);
    }

    @Override
    public void addPages() {
        mainPage = new MainPage();
        simpleResourceBasicDetailsPage = new SimpleResourceBasicDetailsPage();
        simpleResourceAddAttributePage = new SimpleResourceAddAttributePage();
        simpleResourceOtherDetailsPage = new SimpleResourceOtherDetailsPage();
        loadRamlPage = new LoadRamlPage();
        updatePropPage = new UpdatePropertiesPage();
        updateMultiInstanceCreationPropPage = new UpdateMultiInstanceCreationPropertiesPage();

        addPage(mainPage);
        addPage(simpleResourceBasicDetailsPage);
        addPage(simpleResourceAddAttributePage);
        addPage(simpleResourceOtherDetailsPage);
        addPage(loadRamlPage);
        addPage(updatePropPage);
        addPage(updateMultiInstanceCreationPropPage);
    }

    public void setWizardDialog(WizardDialog dlg) {
        wizDialog = dlg;
    }

    public WizardDialog getWizardDialog() {
        return wizDialog;
    }

    public boolean isDlgForceClosed() {
        return dlgForceClosed;
    }

    public void setDlgForceClosed(boolean dlgForceClosed) {
        this.dlgForceClosed = dlgForceClosed;
    }

    public String getConfigFilePath() {
        if (null == loadRamlPage) {
            return null;
        }
        return loadRamlPage.getConfigFilePath();
    }

    public int getResourceCount() {
        if (null == loadRamlPage) {
            return 0;
        }
        return loadRamlPage.getResourceCount();
    }

    public Resource getCreatedResource() {
        return createdResource;
    }

    public Set<SingleResource> getCreatedMultiInstanceResourceSet() {
        return multiInstanceResourceSet;
    }

    @Override
    public boolean canFinish() {
        IWizardPage curPage = this.getContainer().getCurrentPage();
        if ((curPage == updateMultiInstanceCreationPropPage || curPage == updatePropPage
                && ((mainPage.getOption() == Option.SIMPLE_FROM_RAML) || !Activator
                        .getDefault().getResourceManager().isAnyResourceExist()))
                || curPage == simpleResourceAddAttributePage) {
            return true;
        }
        return false;
    }

    @Override
    public boolean performFinish() {
        final IWizardPage curPage = this.getContainer().getCurrentPage();
        // Handling Simple Resource Creation without RAML
        if (curPage == simpleResourceAddAttributePage) {
            try {
                getContainer().run(true, false, new IRunnableWithProgress() {

                    @Override
                    public void run(IProgressMonitor monitor)
                            throws InvocationTargetException,
                            InterruptedException {
                        try {
                            monitor.beginTask(
                                    "Single Resource Creation Without RAML", 2);
                            monitor.worked(1);
                            createSingleResourceWithoutRAML();
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
        } else if (curPage == updatePropPage
                || curPage == updateMultiInstanceCreationPropPage) {
            multiInstanceResourceSet = loadRamlPage
                    .getMultiInstanceResourceSet();
            createdResource = loadRamlPage.getResource();

            String resName;
            String resURI;
            String resType;
            boolean multiInstanceCreation = loadRamlPage
                    .isMultiResourceCreation();

            if (curPage == updatePropPage) {
                resName = updatePropPage.getResName();
                resURI = updatePropPage.getResURI();
                resType = updatePropPage.getResType();
            } else {
                resName = updateMultiInstanceCreationPropPage.getResName();
                resURI = "";
                resType = updateMultiInstanceCreationPropPage.getResType();
            }

            if (null == resName || resName.trim().length() < 1) {
                MessageDialog.openError(Display.getDefault().getActiveShell(),
                        "Invalid Resource Name.", "Resource name is invalid");
                return false;
            }

            if (!multiInstanceCreation && !Utility.isUriValid(resURI)) {
                MessageDialog.openError(Display.getDefault().getActiveShell(),
                        "Invalid Resource URI.", Constants.INVALID_URI_MESSAGE);
                return false;
            }

            // Creation of simple resource with RAML
            // Checking whether the URI is used by any other resource.
            if (!multiInstanceCreation
                    && Activator.getDefault().getResourceManager()
                            .isResourceExist(resURI)) {
                MessageDialog
                        .openError(getShell(), "Resource URI in use",
                                "Entered resource URI is in use. Please try a different one.");
                return false;
            }

            if (!Utility.isResourceTypeValid(resType)) {
                MessageDialog.openError(Display.getDefault().getActiveShell(),
                        "Invalid Resource Type.",
                        Constants.INVALID_RESOURCE_TYPE_MESSAGE);
                return false;
            }

            try {
                getContainer().run(true, false, new IRunnableWithProgress() {

                    @Override
                    public void run(IProgressMonitor monitor)
                            throws InvocationTargetException,
                            InterruptedException {
                        try {
                            monitor.beginTask("Completing Resource Creation", 2);
                            monitor.worked(1);
                            if (loadRamlPage.isMultiResourceCreation()) {
                                completeMultiInstanceResourceCreationWithRAML();
                            } else {
                                completeSingleInstanceResourceCreationWithRAML();
                            }
                            monitor.worked(1);
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
        }
        return true;
    }

    public MainPage getMainPage() {
        return mainPage;
    }

    public SimpleResourceBasicDetailsPage getSimpleResourceBasicDetailsPage() {
        return simpleResourceBasicDetailsPage;
    }

    public SimpleResourceAddAttributePage getSimpleResourceAddAttributePage() {
        return simpleResourceAddAttributePage;
    }

    public SimpleResourceOtherDetailsPage getSimpleResourceOtherDetailsPage() {
        return simpleResourceOtherDetailsPage;
    }

    public LoadRamlPage getLoadRamlPage() {
        return loadRamlPage;
    }

    public UpdatePropertiesPage getUpdatePropPage() {
        return updatePropPage;
    }

    public UpdateMultiInstanceCreationPropertiesPage getUpdateMultiInstanceCreationPropPage() {
        return updateMultiInstanceCreationPropPage;
    }

    public void setUpdateMultiInstanceCreationPropPage(
            UpdateMultiInstanceCreationPropertiesPage updateMultiInstanceCreationPropPage) {
        this.updateMultiInstanceCreationPropPage = updateMultiInstanceCreationPropPage;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public String getStatus() {
        return status;
    }

    private void createSingleResourceWithoutRAML() {
        SingleResource resource = new SingleResource();
        // Basic resource details
        resource.setResourceURI(simpleResourceBasicDetailsPage.getResURI());
        resource.setResourceName(simpleResourceBasicDetailsPage.getResName());
        resource.setResourceType(simpleResourceBasicDetailsPage.getResType());
        resource.setObservable(simpleResourceBasicDetailsPage.isObservable());
        resource.setDiscoverable(simpleResourceBasicDetailsPage
                .isDiscoverable());
        resource.setResourceInterfaces(simpleResourceBasicDetailsPage
                .getInterfaceTypes());

        // Resource Attributes
        Map<String, SimulatorResourceAttribute> attributes = new HashMap<String, SimulatorResourceAttribute>();
        Set<AttributeHelper> attributeSet = simpleResourceAddAttributePage
                .getAttributes();
        if (null != attributeSet && !attributeSet.isEmpty()) {
            Iterator<AttributeHelper> itr = attributeSet.iterator();
            AttributeHelper attHelper;
            SimulatorResourceAttribute attribute;
            while (itr.hasNext()) {
                attHelper = itr.next();
                if (null != attHelper) {
                    attribute = attHelper.convertToSimulatorResourceAttribute();
                    attributes.put(attribute.name(), attribute);
                }
            }
        }

        // Request types
        resource.setGetAllowed(simpleResourceOtherDetailsPage.isGetChecked());
        resource.setPutAllowed(simpleResourceOtherDetailsPage.isPutChecked());
        resource.setPostAllowed(simpleResourceOtherDetailsPage.isPostChecked());

        // Call method of ResourceManager and update the response in the status
        // string.
        try {
            boolean result = Activator.getDefault().getResourceManager()
                    .createSingleResource(resource, attributes);
            if (result) {
                status = "Resource created.";
                createdResource = resource;
            } else {
                status = "Failed to create resource.";
                createdResource = null;
            }
        } catch (Exception e) {
            status = "Failed to create resource.\n"
                    + Utility.getSimulatorErrorString(e, null);
            createdResource = null;
        }
    }

    private void completeSingleInstanceResourceCreationWithRAML() {
        String resName = updatePropPage.getResName();
        String resURI = updatePropPage.getResURI();
        String resType = updatePropPage.getResType();

        try {
            boolean result = false;
            if (createdResource instanceof SingleResource) {
                result = Activator
                        .getDefault()
                        .getResourceManager()
                        .completeSingleResourceCreationByRAML(createdResource,
                                resURI, resName, resType, false);
            }

            if (result) {
                status = "Resource created.";
            } else {
                status = "Failed to create resource.";
                createdResource = null;
            }
        } catch (Exception e) {
            status = "Failed to create resource.\n"
                    + Utility.getSimulatorErrorString(e, null);
            createdResource = null;
        }
    }

    private void completeMultiInstanceResourceCreationWithRAML() {
        String resName = updateMultiInstanceCreationPropPage.getResName();
        String resType = updateMultiInstanceCreationPropPage.getResType();

        if (null == multiInstanceResourceSet)
            return;

        boolean result = false;
        int count = loadRamlPage.getResourceCount();
        int successCount = 0;
        SingleResource resource;
        Iterator<SingleResource> itr = multiInstanceResourceSet.iterator();
        while (itr.hasNext()) {
            resource = itr.next();
            try {
                result = Activator
                        .getDefault()
                        .getResourceManager()
                        .completeSingleResourceCreationByRAML(resource, null,
                                resName, resType, true);
                if (result) {
                    successCount++;
                }

            } catch (SimulatorException e) {
                // Do Nothing.
            }
        }
        if (successCount > 0) {
            UiListenerHandler.getInstance().resourceCreatedUINotification(
                    ResourceType.SINGLE);

            status = "[" + successCount + " out of " + count + "]";
            status += ((successCount == 1) ? "resource" : "resources")
                    + " created successfully.";
        } else {
            status = "Failed to create resources.";
        }
    }
}