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

import org.eclipse.jface.wizard.IWizardPage;
import org.eclipse.jface.wizard.WizardPage;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;

import org.oic.simulator.server.SimulatorResource.Type;

import oic.simulator.serviceprovider.utils.Constants;

public class MainPage extends WizardPage {

    private Button simpleBtn;
    private Button simpleFromRamlBtn;
    private Text   description;

    public enum Option {
        SIMPLE, SIMPLE_FROM_RAML
    }

    private Option option;

    protected MainPage() {
        super("Main Page");
    }

    @Override
    public void createControl(Composite parent) {
        setPageComplete(false);
        setTitle(Constants.MAIN_PAGE_TITLE);
        setMessage(Constants.MAIN_PAGE_MESSAGE);
        Composite compContent = new Composite(parent, SWT.NONE);
        GridLayout gridLayout = new GridLayout();
        compContent.setLayout(gridLayout);
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        compContent.setLayoutData(gd);

        simpleBtn = new Button(compContent, SWT.RADIO);
        simpleBtn.setText("Simple resource");

        simpleFromRamlBtn = new Button(compContent, SWT.RADIO);
        simpleFromRamlBtn.setText("Simple resource(From RAML)");

        Label lbl = new Label(compContent, SWT.NULL);
        lbl.setText("Details:");
        gd = new GridData();
        gd.verticalIndent = 20;
        lbl.setLayoutData(gd);

        description = new Text(compContent, SWT.MULTI | SWT.READ_ONLY
                | SWT.BORDER | SWT.WRAP | SWT.V_SCROLL);
        description.setBackground(compContent.getBackground());
        description.setText("[Select an option to view more details]");
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        description.setLayoutData(gd);

        addUIListeners();

        setControl(compContent);
    }

    private void addUIListeners() {
        simpleBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                description
                        .setText("Create a simple resource by manually entering all the details given below.\n"
                                + "\t1. Basic resource details: URI, Name, Resource Type, and Interface types.\n"
                                + "\t2. Options such as Observable and Discoverable.\n"
                                + "\t3. Adding simple attributes.");
                option = Option.SIMPLE;
                getWizard().getContainer().updateButtons();
            }
        });

        simpleFromRamlBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                description
                        .setText("Create a simple resource from RAML configuration file.\n"
                                + "\t1. Resource details, attributes, and other properties will be read from RAML.\n"
                                + "\t2. Supports multi-instance creation.\n"
                                + "\t3. For single instance creation, allows editing the URI and Name of the resource.");
                option = Option.SIMPLE_FROM_RAML;
                getWizard().getContainer().updateButtons();
            }
        });
    }

    @Override
    public boolean canFlipToNextPage() {
        return true;
    }

    @Override
    public IWizardPage getPreviousPage() {
        return null;
    }

    @Override
    public IWizardPage getNextPage() {
        CreateResourceWizard createWizard = (CreateResourceWizard) getWizard();
        if (simpleBtn.getSelection()) {
            return createWizard.getSimpleResourceBasicDetailsPage();
        }
        if (simpleFromRamlBtn.getSelection()) {
            LoadRamlPage page = createWizard.getLoadRamlPage();
            page.initialSetup(Type.SINGLE);
            return page;
        }
        return null;
    }

    public Option getOption() {
        return option;
    }
}
