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

import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.jface.dialogs.TrayDialog;
import org.eclipse.jface.window.Window;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.List;
import org.eclipse.swt.widgets.Shell;

import java.util.ArrayList;
import java.util.Map;

/**
 * Dialog for starting and stopping the automatic verifications.
 */
public class UpdateResourceInterfaceDialog extends TrayDialog {

    private Button              addIfTypeBtn;
    private Button              removeIfTypeBtn;

    private List                ifTypesList;

    private Map<String, String> supportedResInterfaces;
    private Map<String, String> updatedResInterfaces;

    public UpdateResourceInterfaceDialog(Shell shell,
            Map<String, String> updatedResInterfaces,
            Map<String, String> supportedResInterfaces) {
        super(shell);
        this.updatedResInterfaces = updatedResInterfaces;
        this.supportedResInterfaces = supportedResInterfaces;
    }

    @Override
    protected void configureShell(Shell shell) {
        super.configureShell(shell);
    }

    @Override
    protected Control createDialogArea(Composite parent) {
        Composite composite = (Composite) super.createDialogArea(parent);
        getShell().setText("Resource Interfaces");

        Composite content = new Composite(parent, SWT.NULL);
        GridLayout gridLayout = new GridLayout(3, false);
        content.setLayout(gridLayout);
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        content.setLayoutData(gd);

        ifTypesList = new List(content, SWT.BORDER | SWT.MULTI | SWT.V_SCROLL
                | SWT.H_SCROLL);
        gd = new GridData();
        gd.horizontalAlignment = SWT.FILL;
        gd.grabExcessHorizontalSpace = true;
        gd.verticalAlignment = SWT.FILL;
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalSpan = 2;
        ifTypesList.setLayoutData(gd);
        ifTypesList.setBackground(Display.getDefault().getSystemColor(
                SWT.COLOR_WHITE));

        Composite ifTypesOpComp = new Composite(content, SWT.NONE);
        gridLayout = new GridLayout();
        ifTypesOpComp.setLayout(gridLayout);
        gd = new GridData();
        gd.verticalAlignment = SWT.TOP;
        ifTypesOpComp.setLayoutData(gd);

        addIfTypeBtn = new Button(ifTypesOpComp, SWT.PUSH);
        addIfTypeBtn.setText("Add");
        gd = new GridData();
        gd.widthHint = 70;
        addIfTypeBtn.setLayoutData(gd);
        if (null != updatedResInterfaces && null != supportedResInterfaces
                && updatedResInterfaces.size() == supportedResInterfaces.size()) {
            addIfTypeBtn.setEnabled(false);
        }

        removeIfTypeBtn = new Button(ifTypesOpComp, SWT.PUSH);
        removeIfTypeBtn.setText("Remove");
        gd = new GridData();
        gd.widthHint = 70;
        removeIfTypeBtn.setLayoutData(gd);
        removeIfTypeBtn.setEnabled(false);

        initInterfaceList();

        addUiListeners();

        return composite;
    }

    private void initInterfaceList() {
        if (null != updatedResInterfaces) {
            for (Map.Entry<String, String> entry : updatedResInterfaces
                    .entrySet()) {
                ifTypesList.add(entry.getKey());
            }
        }
    }

    private void addUiListeners() {
        addIfTypeBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                boolean addAll = false;
                int itemsAddedCount = ifTypesList.getItemCount();
                if (itemsAddedCount < supportedResInterfaces.size()) {
                    String key;
                    ArrayList<String> pendingItems = new ArrayList<String>();
                    if (0 == itemsAddedCount) {
                        addAll = true;
                    }
                    for (Map.Entry<String, String> entry : supportedResInterfaces
                            .entrySet()) {
                        key = entry.getKey();
                        if (addAll || -1 == ifTypesList.indexOf(key)) {
                            pendingItems.add(key);
                        }
                    }

                    AddInterfaceTypeDialog addIfTypeDlg = new AddInterfaceTypeDialog(
                            getShell(), pendingItems);
                    if (Window.CANCEL == addIfTypeDlg.open()) {
                        return;
                    }
                    String ifType = addIfTypeDlg.getValue();
                    if (null == ifType || ifType.isEmpty()) {
                        return;
                    }

                    ifTypesList.add(ifType);
                    ifTypesList.deselectAll();
                    ifTypesList.select(ifTypesList.getItemCount() - 1);
                    ifTypesList.showSelection();
                    removeIfTypeBtn.setEnabled(true);

                    if (itemsAddedCount + 1 == supportedResInterfaces.size()) {
                        addIfTypeBtn.setEnabled(false);
                    }
                }
            }
        });

        removeIfTypeBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                int[] selection = ifTypesList.getSelectionIndices();
                if (null != selection && selection.length > 0) {
                    ifTypesList.remove(selection);
                }

                ifTypesList.deselectAll();
                removeIfTypeBtn.setEnabled(false);
                addIfTypeBtn.setEnabled(true);
            }
        });

        ifTypesList.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                int[] selection = ifTypesList.getSelectionIndices();
                if (null != selection && selection.length > 0) {
                    removeIfTypeBtn.setEnabled(true);
                }
            }
        });
    }

    @Override
    protected void okPressed() {
        // Clearing the map to freshly add selected items.
        updatedResInterfaces.clear();

        String[] items = ifTypesList.getItems();
        if (null != items && items.length > 0) {
            for (String item : items) {
                String value = supportedResInterfaces.get(item);
                updatedResInterfaces.put(item, value);
            }
        }
        close();
    }

    @Override
    protected Point getInitialSize() {
        Point actualSize = super.getInitialSize();
        return new Point(actualSize.x + 100, actualSize.y);
    }

    @Override
    protected Button createButton(Composite parent, int id, String label,
            boolean defaultButton) {
        if (id == IDialogConstants.CANCEL_ID) {
            return null;
        }
        return super.createButton(parent, id, label, defaultButton);
    }

    @Override
    public boolean isHelpAvailable() {
        return false;
    }
}
