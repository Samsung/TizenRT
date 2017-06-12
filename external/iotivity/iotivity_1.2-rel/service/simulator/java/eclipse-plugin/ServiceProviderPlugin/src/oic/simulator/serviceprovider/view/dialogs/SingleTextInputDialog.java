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

import org.eclipse.jface.dialogs.TrayDialog;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

/**
 * This class shows a dialog for filtering logs based on severity levels.
 */
public class SingleTextInputDialog extends TrayDialog {

    private Text   txt;
    private String value;
    private String lblTxt;
    private String shellTitle;

    public SingleTextInputDialog(Shell shell, String title, String lblTxt) {
        super(shell);
        shellTitle = title;
        this.lblTxt = lblTxt;
    }

    @Override
    protected void configureShell(Shell shell) {
        super.configureShell(shell);
    }

    @Override
    protected Control createDialogArea(Composite parent) {
        Composite composite = (Composite) super.createDialogArea(parent);
        getShell().setText(shellTitle);

        Composite content = new Composite(parent, SWT.NULL);
        GridLayout gridLayout = new GridLayout();
        content.setLayout(gridLayout);
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        content.setLayoutData(gd);

        Label lbl = new Label(content, SWT.NONE);
        lbl.setText(lblTxt);

        txt = new Text(content, SWT.BORDER);
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        txt.setLayoutData(gd);
        txt.addModifyListener(new ModifyListener() {

            @Override
            public void modifyText(ModifyEvent e) {
                value = txt.getText();
            }
        });

        return composite;
    }

    @Override
    protected void okPressed() {
        if (null != value) {
            value = value.trim();
            if (value.length() > 0) {
                close();
                return;
            }
        }
        txt.setFocus();
    }

    @Override
    public boolean isHelpAvailable() {
        return false;
    }

    public String getValue() {
        return value;
    }
}