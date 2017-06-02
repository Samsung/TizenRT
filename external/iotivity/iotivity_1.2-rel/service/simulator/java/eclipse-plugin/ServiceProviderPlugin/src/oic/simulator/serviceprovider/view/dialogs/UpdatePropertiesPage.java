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
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;

import oic.simulator.serviceprovider.utils.Constants;

public class UpdatePropertiesPage extends WizardPage {

    private Text   resNameTxt;
    private Text   resUriTxt;
    private Text   resTypeTxt;

    private String resName;
    private String resURI;
    private String resType;

    protected UpdatePropertiesPage() {
        super("Update Properties");
    }

    @Override
    public void createControl(Composite parent) {
        setPageComplete(true);
        setTitle(Constants.UPDATE_PROP_PAGE_TITLE);
        setMessage(Constants.UPDATE_PROP_PAGE_MESSAGE);

        Composite comp = new Composite(parent, SWT.NONE);
        GridLayout gridLayout = new GridLayout();
        comp.setLayout(gridLayout);
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        comp.setLayoutData(gd);

        Group grp = new Group(comp, SWT.NONE);
        gridLayout = new GridLayout(2, false);
        grp.setLayout(gridLayout);
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        grp.setLayoutData(gd);

        Label resNameLbl = new Label(grp, SWT.NULL);
        resNameLbl.setText("Resource Name");
        gd = new GridData();
        gd.verticalIndent = 20;
        resNameLbl.setLayoutData(gd);

        resNameTxt = new Text(grp, SWT.BORDER);
        resNameTxt.setFocus();
        gd = new GridData();
        gd.widthHint = 300;
        gd.verticalIndent = 20;
        resNameTxt.setLayoutData(gd);

        Label resUriLbl = new Label(grp, SWT.NULL);
        resUriLbl.setText("Resource URI");
        gd = new GridData();
        gd.verticalIndent = 10;
        resUriLbl.setLayoutData(gd);

        resUriTxt = new Text(grp, SWT.BORDER);
        gd = new GridData();
        gd.widthHint = 300;
        gd.verticalIndent = 10;
        resUriTxt.setLayoutData(gd);

        Label resTypeLbl = new Label(grp, SWT.NULL);
        resTypeLbl.setText("Resource Type");
        gd = new GridData();
        gd.verticalIndent = 10;
        resTypeLbl.setLayoutData(gd);

        resTypeTxt = new Text(grp, SWT.BORDER);
        gd = new GridData();
        gd.widthHint = 300;
        gd.verticalIndent = 10;
        resTypeTxt.setLayoutData(gd);

        Label descLbl = new Label(comp, SWT.NONE);
        descLbl.setText("Description:");
        gd = new GridData();
        descLbl.setLayoutData(gd);

        final Text text = new Text(comp, SWT.MULTI | SWT.READ_ONLY | SWT.BORDER
                | SWT.WRAP | SWT.V_SCROLL);
        text.setText("These properties can be changed later from properties view.");
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        text.setLayoutData(gd);

        addUIListeners();

        // Initialize data
        if (resUriTxt.getText().length() < 1 && null != resURI) {
            resUriTxt.setText(resURI);
        }
        if (resNameTxt.getText().length() < 1 && null != resName) {
            resNameTxt.setText(resName);
        }
        if (resTypeTxt.getText().length() < 1 && null != resType) {
            resTypeTxt.setText(resType);
        }

        setControl(comp);
    }

    private void addUIListeners() {
        resNameTxt.addModifyListener(new ModifyListener() {
            @Override
            public void modifyText(ModifyEvent e) {
                resName = resNameTxt.getText();
                setPageComplete(isSelectionDone());
            }
        });

        resUriTxt.addModifyListener(new ModifyListener() {
            @Override
            public void modifyText(ModifyEvent e) {
                resURI = resUriTxt.getText();
                if (null == resURI) {
                    return;
                }

                setPageComplete(isSelectionDone());
            }
        });

        resTypeTxt.addModifyListener(new ModifyListener() {
            @Override
            public void modifyText(ModifyEvent e) {
                resType = resTypeTxt.getText();
                if (null == resType) {
                    return;
                }

                setPageComplete(isSelectionDone());
            }
        });
    }

    @Override
    public boolean canFlipToNextPage() {
        return false;
    }

    public boolean isSelectionDone() {
        boolean done = false;
        if (null != resName && resName.trim().length() > 0 && null != resURI
                && resURI.trim().length() > 0 && null != resType
                && resType.trim().length() > 0) {
            done = true;
        }
        return done;
    }

    @Override
    public IWizardPage getNextPage() {
        return null;
    }

    public void setResName(String resName) {
        if (null == resName) {
            resName = "";
        }

        this.resName = resName;
        if (null != resName && !resNameTxt.isDisposed())
            resNameTxt.setText(resName);
    }

    public void setResURI(String resURI) {
        if (null == resURI) {
            resURI = "";
        }

        this.resURI = resURI;
        if (null != resUriTxt && !resUriTxt.isDisposed())
            resUriTxt.setText(resURI);
    }

    public void setResType(String resType) {
        if (null == resType) {
            resType = "";
        }

        this.resType = resType;
        if (null != resType && !resTypeTxt.isDisposed())
            resTypeTxt.setText(resType);
    }

    public String getResName() {
        return resName;
    }

    public String getResURI() {
        return resURI;
    }

    public String getResType() {
        return resType;
    }
}
