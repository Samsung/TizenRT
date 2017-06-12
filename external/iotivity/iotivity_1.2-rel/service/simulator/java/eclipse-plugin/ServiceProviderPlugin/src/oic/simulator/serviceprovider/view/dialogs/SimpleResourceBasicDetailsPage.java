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

import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.window.Window;
import org.eclipse.jface.wizard.IWizardPage;
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
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.List;
import org.eclipse.swt.widgets.Text;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import oic.simulator.serviceprovider.Activator;
import oic.simulator.serviceprovider.model.SingleResource;
import oic.simulator.serviceprovider.utils.Constants;
import oic.simulator.serviceprovider.utils.Utility;

public class SimpleResourceBasicDetailsPage extends WizardPage {

    private Text                resNameTxt;
    private Text                resUriTxt;
    private Text                resTypeTxt;

    private Button              observeBtn;
    private Button              discoverBtn;
    private Button              addIfTypeBtn;
    private Button              removeIfTypeBtn;

    private List                ifTypesList;

    private String              resName;
    private String              resURI;
    private String              resType;
    private boolean             observable;
    private boolean             discoverable;
    private Map<String, String> ifTypes;
    private Set<String>         selectedIfTypes;

    protected SimpleResourceBasicDetailsPage() {
        super("Basic Details");
        selectedIfTypes = new HashSet<String>();
    }

    @Override
    public void createControl(Composite parent) {
        setPageComplete(false);
        setTitle(Constants.SIMPLE_RESOURCE_BASIC_DETAILS_PAGE_TITLE);
        setMessage(Constants.SIMPLE_RESOURCE_BASIC_DETAILS_PAGE_MESSAGE);

        Composite container = new Composite(parent, SWT.NONE);
        GridLayout gridLayout = new GridLayout();
        container.setLayout(gridLayout);
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        container.setLayoutData(gd);

        Group resDetGrp = new Group(container, SWT.NONE);
        resDetGrp.setText("Resource Details");
        gridLayout = new GridLayout(4, false);
        resDetGrp.setLayout(gridLayout);
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        resDetGrp.setLayoutData(gd);

        Label resNameLbl = new Label(resDetGrp, SWT.NULL);
        resNameLbl.setText(Constants.RESOURCE_NAME);
        gd = new GridData();
        resNameLbl.setLayoutData(gd);

        resNameTxt = new Text(resDetGrp, SWT.BORDER);
        gd = new GridData();
        gd.horizontalAlignment = SWT.FILL;
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalSpan = 3;
        resNameTxt.setLayoutData(gd);
        resNameTxt.setFocus();

        Label resUriLbl = new Label(resDetGrp, SWT.NULL);
        resUriLbl.setText(Constants.RESOURCE_URI);

        resUriTxt = new Text(resDetGrp, SWT.BORDER);
        gd = new GridData();
        gd.horizontalSpan = 3;
        gd.horizontalAlignment = SWT.FILL;
        gd.grabExcessHorizontalSpace = true;
        resUriTxt.setLayoutData(gd);

        Label resTypeLbl = new Label(resDetGrp, SWT.NULL);
        resTypeLbl.setText(Constants.RESOURCE_TYPE);

        resTypeTxt = new Text(resDetGrp, SWT.BORDER);
        gd = new GridData();
        gd.horizontalSpan = 3;
        gd.horizontalAlignment = SWT.FILL;
        gd.grabExcessHorizontalSpace = true;
        resTypeTxt.setLayoutData(gd);

        Label ifTypesLbl = new Label(resDetGrp, SWT.NULL);
        ifTypesLbl.setText(Constants.INTERFACE_TYPES);
        gd = new GridData();
        ifTypesLbl.setLayoutData(gd);

        ifTypesList = new List(resDetGrp, SWT.BORDER | SWT.MULTI | SWT.V_SCROLL
                | SWT.H_SCROLL);
        gd = new GridData();
        gd.horizontalAlignment = SWT.FILL;
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalSpan = 2;
        ifTypesList.setLayoutData(gd);
        ifTypesList.setBackground(Display.getDefault().getSystemColor(
                SWT.COLOR_WHITE));

        Composite ifTypesOpComp = new Composite(resDetGrp, SWT.NONE);
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

        removeIfTypeBtn = new Button(ifTypesOpComp, SWT.PUSH);
        removeIfTypeBtn.setText("Remove");
        gd = new GridData();
        gd.widthHint = 70;
        removeIfTypeBtn.setLayoutData(gd);
        removeIfTypeBtn.setEnabled(false);

        Group otherOptionsGrp = new Group(container, SWT.NONE);
        otherOptionsGrp.setText("Other options");
        gridLayout = new GridLayout();
        otherOptionsGrp.setLayout(gridLayout);
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        otherOptionsGrp.setLayoutData(gd);

        discoverBtn = new Button(otherOptionsGrp, SWT.CHECK);
        discoverBtn.setText("Discoverable");
        discoverBtn.setSelection(true);
        discoverable = true;

        observeBtn = new Button(otherOptionsGrp, SWT.CHECK);
        observeBtn.setText("Observable");
        observeBtn.setSelection(true);
        observable = true;

        addUiListeners();

        // Set the interface types.
        Map<String, String> ifTypes = Utility
                .getResourceInterfaces(SingleResource.class);
        if (null != ifTypes && !ifTypes.isEmpty()) {
            this.ifTypes = new HashMap<String, String>();
            String key;
            for (Map.Entry<String, String> entry : ifTypes.entrySet()) {
                key = entry.getValue() + " (" + entry.getKey() + ")";
                this.ifTypes.put(key, entry.getKey());
            }
        }

        setControl(container);
    }

    private void addUiListeners() {

        resUriTxt.addModifyListener(new ModifyListener() {
            @Override
            public void modifyText(ModifyEvent arg0) {
                resURI = resUriTxt.getText();
                if (null == resURI) {
                    return;
                }

                getWizard().getContainer().updateButtons();
            }
        });

        resNameTxt.addModifyListener(new ModifyListener() {
            @Override
            public void modifyText(ModifyEvent arg0) {
                resName = resNameTxt.getText();
                getWizard().getContainer().updateButtons();
            }
        });

        resTypeTxt.addModifyListener(new ModifyListener() {
            @Override
            public void modifyText(ModifyEvent arg0) {
                resType = resTypeTxt.getText();
                getWizard().getContainer().updateButtons();
            }
        });

        discoverBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                discoverable = discoverBtn.getSelection();
            }
        });

        observeBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                observable = observeBtn.getSelection();
            }
        });

        addIfTypeBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                boolean addAll = false;
                int itemsAddedCount = ifTypesList.getItemCount();
                if (itemsAddedCount < ifTypes.size()) {
                    String key;
                    ArrayList<String> pendingItems = new ArrayList<String>();
                    if (0 == itemsAddedCount) {
                        addAll = true;
                    }
                    for (Map.Entry<String, String> entry : ifTypes.entrySet()) {
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
                    ifTypesList.add(ifType);
                    ifTypesList.deselectAll();
                    ifTypesList.select(ifTypesList.getItemCount() - 1);
                    ifTypesList.showSelection();
                    removeIfTypeBtn.setEnabled(true);

                    if (itemsAddedCount + 1 == ifTypes.size()) {
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
    public boolean canFlipToNextPage() {
        if (null == resName || null == resURI || null == resType) {
            return false;
        }
        resName = resName.trim();
        resURI = resURI.trim();
        resType = resType.trim();
        if (resName.length() < 1 || resURI.length() < 1 || resType.length() < 1) {
            return false;
        }
        return true;
    }

    @Override
    public IWizardPage getNextPage() {
        if (!Utility.isUriValid(resURI)) {
            MessageDialog.openError(Display.getDefault().getActiveShell(),
                    "Invalid Resource URI.", Constants.INVALID_URI_MESSAGE);
            return null;
        }

        if (!Utility.isResourceTypeValid(resType)) {
            MessageDialog.openError(Display.getDefault().getActiveShell(),
                    "Invalid Resource Type.",
                    Constants.INVALID_RESOURCE_TYPE_MESSAGE);
            return null;
        }

        // Checking whether the uri is used by any other resource.
        if (Activator.getDefault().getResourceManager().isResourceExist(resURI)) {
            MessageDialog
                    .openError(getShell(), "Resource URI in use",
                            "Entered resource URI is in use. Please try a different one.");
            return null;
        }

        String[] items = ifTypesList.getItems();
        if (null == items || items.length == 0) {
            MessageDialog
                    .openInformation(
                            getShell(),
                            "Default Interface Type Selection",
                            "As no interface types are added, the resource will be "
                                    + "configured with the default interface type(oic.if.baseline).");
            ifTypesList.add("Baseline" + " ("
                    + Constants.DEFAULT_SINGLE_RESOURCE_INTERFACE + ")");
        }
        selectedIfTypes.clear();
        for (String item : ifTypesList.getItems()) {
            selectedIfTypes.add(ifTypes.get(item));
        }

        return ((CreateResourceWizard) getWizard())
                .getSimpleResourceAddAttributePage();
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

    public Set<String> getInterfaceTypes() {
        return selectedIfTypes;
    }

    public boolean isObservable() {
        return observable;
    }

    public boolean isDiscoverable() {
        return discoverable;
    }
}
