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
import org.eclipse.jface.operation.IRunnableWithProgress;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.wizard.Wizard;

import java.lang.reflect.InvocationTargetException;
import java.net.URL;
import java.util.Date;
import java.util.Set;

import org.oic.simulator.ILogger.Level;
import org.oic.simulator.SimulatorException;

import oic.simulator.serviceprovider.Activator;
import oic.simulator.serviceprovider.manager.ResourceManager;
import oic.simulator.serviceprovider.model.Resource;
import oic.simulator.serviceprovider.model.SingleResource;

/**
 * This class creates a UI wizard for delete resource operation.
 */
public class DeleteResourceWizard extends Wizard {

    private DeleteResourcePage page;

    private boolean            success;

    public DeleteResourceWizard() {
        setWindowTitle("Delete resources");
        IPath path = new Path("/icons/oic_logo_64x64.png");
        URL find = FileLocator.find(Activator.getDefault().getBundle(), path,
                null);
        setDefaultPageImageDescriptor(ImageDescriptor.createFromURL(find));

        setNeedsProgressMonitor(true);
    }

    @Override
    public void addPages() {
        page = new DeleteResourcePage();
        addPage(page);
    }

    @Override
    public boolean performFinish() {
        if (null == page) {
            return false;
        }
        try {
            getContainer().run(true, false, new IRunnableWithProgress() {

                @Override
                public void run(IProgressMonitor monitor)
                        throws InvocationTargetException, InterruptedException {
                    ResourceManager resourceManager = Activator.getDefault()
                            .getResourceManager();
                    try {
                        monitor.beginTask("Resource Deletion", 2);
                        monitor.worked(1);
                        Set<SingleResource> singleResources = page
                                .getSelectedSingleResourcesList();
                        if (null != singleResources
                                && singleResources.size() > 0) {
                            Activator.getDefault().getResourceManager()
                                    .removeSingleResources(singleResources);

                            Resource res = resourceManager
                                    .getCurrentResourceInSelection();
                            if (null != res && res instanceof SingleResource) {
                                if (singleResources
                                        .contains((SingleResource) res)) {
                                    resourceManager
                                            .resourceSelectionChanged(null);
                                }
                            }
                        }
                        monitor.worked(1);
                        success = true;
                    } catch (SimulatorException e) {
                        success = false;
                    } finally {
                        monitor.done();
                    }
                }
            });
        } catch (InvocationTargetException e) {
            Activator.getDefault().getLogManager()
                    .log(Level.ERROR.ordinal(), new Date(), e.getMessage());
        } catch (InterruptedException e) {
            Activator.getDefault().getLogManager()
                    .log(Level.ERROR.ordinal(), new Date(), e.getMessage());
        }
        return true;
    }

    public boolean getStatus() {
        return success;
    }
}