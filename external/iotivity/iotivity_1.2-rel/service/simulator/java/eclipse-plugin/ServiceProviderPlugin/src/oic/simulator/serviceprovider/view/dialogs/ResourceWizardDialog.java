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

import org.eclipse.jface.wizard.IWizard;
import org.eclipse.jface.wizard.IWizardPage;
import org.eclipse.jface.wizard.WizardDialog;
import org.eclipse.swt.widgets.Shell;

/**
 * Dialog for handling wizards.
 */
public class ResourceWizardDialog extends WizardDialog {

    private IWizard wizard;

    public ResourceWizardDialog(Shell parentShell, IWizard newWizard) {
        super(parentShell, newWizard);
        wizard = newWizard;
    }

    @Override
    public boolean isHelpAvailable() {
        return false;
    }

    @Override
    public void finishPressed() {
        super.finishPressed();
    }

    @Override
    protected void backPressed() {
        if (wizard.getClass() == CreateResourceWizard.class) {
            CreateResourceWizard createWizard = (CreateResourceWizard) wizard;

            MainPage mainPage = createWizard.getMainPage();
            SimpleResourceBasicDetailsPage simpleResourceBasicDetailsPage = createWizard
                    .getSimpleResourceBasicDetailsPage();
            SimpleResourceOtherDetailsPage simpleResourceOtherDetailsPage = createWizard
                    .getSimpleResourceOtherDetailsPage();
            SimpleResourceAddAttributePage simpleResourceAddAttributesPage = createWizard
                    .getSimpleResourceAddAttributePage();
            LoadRamlPage loadRamlPage = createWizard.getLoadRamlPage();
            UpdatePropertiesPage updatePropPage = createWizard
                    .getUpdatePropPage();
            UpdateMultiInstanceCreationPropertiesPage updateMultiInstanceCreationPropPage = createWizard
                    .getUpdateMultiInstanceCreationPropPage();

            IWizardPage curPage = wizard.getContainer().getCurrentPage();
            IWizardPage prevPage = null;

            if (curPage == loadRamlPage
                    || curPage == simpleResourceBasicDetailsPage) {
                prevPage = mainPage;
            } else if (curPage == updatePropPage
                    || curPage == updateMultiInstanceCreationPropPage) {
                loadRamlPage.setResource(null);
                loadRamlPage.setMultiInstanceResourceSet(null);
                prevPage = loadRamlPage;
            } else if (curPage == simpleResourceAddAttributesPage) {
                prevPage = simpleResourceBasicDetailsPage;
            } else if (curPage == simpleResourceOtherDetailsPage) {
                prevPage = simpleResourceAddAttributesPage;
            }
            if (null != prevPage)
                showPage(prevPage);
        }
    }
}