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

package oic.simulator.clientcontroller.view.dialogs;

import org.eclipse.jface.wizard.WizardPage;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;

import java.util.Set;

import oic.simulator.clientcontroller.utils.Constants;
import oic.simulator.clientcontroller.utils.Utility;

/**
 * This class shows UI for finding resources.
 */
public class FindResourcePage extends WizardPage {

    private Button      allRbtn;
    private Button      resTypeRbtn;
    private Text        resTypeTxt;
    private Label       resTypeLbl;

    private Set<String> typesToSearch;

    protected FindResourcePage() {
        super("Find Resource");
    }

    @Override
    public void createControl(Composite parent) {
        setTitle(Constants.FIND_PAGE_TITLE);
        setMessage(Constants.FIND_PAGE_MESSAGE);

        Composite compContent = new Composite(parent, SWT.NONE);
        GridLayout gridLayout = new GridLayout();
        compContent.setLayout(gridLayout);
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        compContent.setLayoutData(gd);

        Group configGroup = new Group(compContent, SWT.NONE);
        gridLayout = new GridLayout(2, false);
        gridLayout.verticalSpacing = 10;
        gridLayout.marginTop = 5;
        configGroup.setLayout(gridLayout);
        gd = new GridData();
        gd.horizontalAlignment = SWT.FILL;
        gd.grabExcessHorizontalSpace = true;
        configGroup.setLayoutData(gd);
        configGroup.setText("Resource Type");

        allRbtn = new Button(configGroup, SWT.RADIO);
        allRbtn.setText("All");
        gd = new GridData();
        gd.horizontalSpan = 2;
        allRbtn.setLayoutData(gd);
        allRbtn.setSelection(true);

        resTypeRbtn = new Button(configGroup, SWT.RADIO);
        resTypeRbtn.setText("Specific Resource types (seperated by commas)");
        gd = new GridData();
        gd.horizontalSpan = 2;
        resTypeRbtn.setLayoutData(gd);

        resTypeLbl = new Label(configGroup, SWT.NONE);
        resTypeLbl.setText("Resource Types:");
        resTypeLbl.setEnabled(false);

        resTypeTxt = new Text(configGroup, SWT.BORDER);
        resTypeTxt.setToolTipText("Ex: sample.light, hall.fridge");
        resTypeTxt.setMessage("Ex: sample.light, hall.fridge");
        gd = new GridData();
        gd.minimumWidth = 200;
        gd.horizontalAlignment = SWT.FILL;
        gd.grabExcessHorizontalSpace = true;
        resTypeTxt.setLayoutData(gd);
        resTypeTxt.setEnabled(false);

        addUIListeners();

        setControl(compContent);
    }

    private void addUIListeners() {
        allRbtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                typesToSearch = null;
                setPageComplete(true);
                // Change the visibility of widgets
                changeVisibility(false);
            }
        });

        resTypeRbtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                String typeText = resTypeTxt.getText();
                if (null != typeText && typeText.length() > 0) {
                    typesToSearch = Utility.splitStringByComma(typeText);
                }

                if (null != typesToSearch && typesToSearch.size() > 0) {
                    setPageComplete(true);
                } else {
                    setPageComplete(false);
                }
                // Change the visibility of widgets
                changeVisibility(true);
                resTypeTxt.setFocus();
            }
        });

        resTypeTxt.addModifyListener(new ModifyListener() {
            @Override
            public void modifyText(ModifyEvent e) {
                String typeText = resTypeTxt.getText();
                typesToSearch = Utility.splitStringByComma(typeText);
                if (null != typesToSearch && typesToSearch.size() > 0) {
                    setPageComplete(true);
                } else {
                    setPageComplete(false);
                }
            }
        });
    }

    private void changeVisibility(boolean standard) {
        resTypeLbl.setEnabled(standard);
        resTypeTxt.setEnabled(standard);
    }

    public Set<String> getSearchTypes() {
        return typesToSearch;
    }
}
