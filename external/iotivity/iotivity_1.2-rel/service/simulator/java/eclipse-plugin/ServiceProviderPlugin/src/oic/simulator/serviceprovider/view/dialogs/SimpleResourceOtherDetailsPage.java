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
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;

import oic.simulator.serviceprovider.utils.Constants;

public class SimpleResourceOtherDetailsPage extends WizardPage {

    private Button getBtn;
    private Button putBtn;
    private Button postBtn;

    public boolean getChecked;
    public boolean putChecked;
    public boolean postChecked;

    protected SimpleResourceOtherDetailsPage() {
        super("Allowed Request Types");
    }

    @Override
    public void createControl(Composite parent) {
        setPageComplete(false);
        setTitle(Constants.SIMPLE_RESOURCE_OTHER_DETAILS_PAGE_TITLE);
        setMessage(Constants.SIMPLE_RESOURCE_OTHER_DETAILS_PAGE_MESSAGE);

        Composite container = new Composite(parent, SWT.NONE);
        GridLayout gridLayout = new GridLayout();
        container.setLayout(gridLayout);
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        container.setLayoutData(gd);

        Group resDetGrp = new Group(container, SWT.NONE);
        resDetGrp.setText("Allowed Request Types");
        gridLayout = new GridLayout();
        resDetGrp.setLayout(gridLayout);
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        resDetGrp.setLayoutData(gd);

        getBtn = new Button(resDetGrp, SWT.CHECK);
        getBtn.setText("GET Request");

        putBtn = new Button(resDetGrp, SWT.CHECK);
        putBtn.setText("PUT Request");

        postBtn = new Button(resDetGrp, SWT.CHECK);
        postBtn.setText("POST Request");

        Label lbl = new Label(container, SWT.NULL);
        lbl.setText("Details:");
        gd = new GridData();
        gd.verticalIndent = 20;
        lbl.setLayoutData(gd);

        Text desc = new Text(container, SWT.MULTI | SWT.READ_ONLY | SWT.BORDER
                | SWT.WRAP | SWT.V_SCROLL);
        desc.setBackground(container.getBackground());
        desc.setText("Shows the different types of request that can be used by the clients on the resource.\n"
                + "Resource will handle only the request types which are selected here.");
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        desc.setLayoutData(gd);

        addUiListeners();

        setControl(container);
    }

    private void addUiListeners() {
        getBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                getChecked = getBtn.getSelection();
            }
        });

        putBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                putChecked = putBtn.getSelection();
            }
        });

        postBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                postChecked = postBtn.getSelection();
            }
        });
    }

    @Override
    public IWizardPage getNextPage() {
        return null;
    }

    public boolean isGetChecked() {
        return getChecked;
    }

    public boolean isPutChecked() {
        return putChecked;
    }

    public boolean isPostChecked() {
        return postChecked;
    }

}
