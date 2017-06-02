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

import org.eclipse.jface.dialogs.IDialogConstants;
import org.eclipse.jface.dialogs.TitleAreaDialog;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Vector;

import oic.simulator.clientcontroller.Activator;
import oic.simulator.clientcontroller.remoteresource.RemoteResource;
import oic.simulator.clientcontroller.utils.Constants;
import oic.simulator.clientcontroller.utils.Utility;

/**
 * This dialog is used for generating a PUT request.
 */
public class GetRequestDialog extends TitleAreaDialog {
    private Text                queryTxt;
    private Combo               ifTypesCmb;

    private String              otherFilters;
    private String              ifType;

    private Map<String, String> ifTypes;

    public GetRequestDialog(Shell parentShell) {
        super(parentShell);
    }

    @Override
    public void create() {
        super.create();
        setTitle("Generate GET Request");
        setMessage("Dialog which takes query filters and generates a GET request.");
    }

    @Override
    protected Control createDialogArea(Composite parent) {
        Composite compLayout = (Composite) super.createDialogArea(parent);

        Group paramsGrp = new Group(compLayout, SWT.NONE);
        paramsGrp.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));
        GridLayout layout = new GridLayout(2, false);
        layout.verticalSpacing = 10;
        layout.marginTop = 10;
        paramsGrp.setLayout(layout);

        Label ifTypeLbl = new Label(paramsGrp, SWT.NONE);
        ifTypeLbl.setText("Interface Type");

        ifTypesCmb = new Combo(paramsGrp, SWT.NULL);
        GridData gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        ifTypesCmb.setLayoutData(gd);
        ifTypesCmb.addModifyListener(new ModifyListener() {
            @Override
            public void modifyText(ModifyEvent e) {
                ifType = ifTypesCmb.getText();
            }
        });

        RemoteResource resource = Activator.getDefault().getResourceManager()
                .getCurrentResourceInSelection();

        // Set the interface types in combo box.
        Map<String, String> ifTypes = Utility.getResourceInterfaces();
        this.ifTypes = new HashMap<String, String>();
        String key;
        for (Map.Entry<String, String> entry : ifTypes.entrySet()) {
            key = entry.getValue() + " (" + entry.getKey() + ")";
            this.ifTypes.put(key, entry.getKey());
            ifTypesCmb.add(key);
        }

        // Select the default value to be shown in the interface types combo.
        Vector<String> ifTypesSupportedByResource = resource
                .getRemoteResourceRef().getResourceInterfaces();
        if (null != ifTypesSupportedByResource) {
            int index = -1;
            if (ifTypesSupportedByResource
                    .contains(Constants.BASELINE_INTERFACE)
                    && ifTypes.containsKey(Constants.BASELINE_INTERFACE)) {
                // Baseline interface is given preference to be shown in the if
                // types combo.
                String value = ifTypes.get(Constants.BASELINE_INTERFACE);
                index = ifTypesCmb.indexOf(value + " ("
                        + Constants.BASELINE_INTERFACE + ")");
                if (index != -1)
                    ifTypesCmb.select(index);
            }
            if (index == -1) {
                // Baseline interface is not selected so selecting some other
                // interface supported by the resource.
                Iterator<String> itr = ifTypesSupportedByResource.iterator();
                while (itr.hasNext() && index == -1) {
                    key = itr.next();
                    if (ifTypes.containsKey(key)) {
                        String value = ifTypes.get(key);
                        index = ifTypesCmb.indexOf(value + " (" + key + ")");
                        if (index != -1) {
                            ifTypesCmb.select(index);
                            break;
                        }
                    }
                }
                if (index == -1 && !ifTypesSupportedByResource.isEmpty()) {
                    // Resource has custom interfaces.
                    ifTypesCmb.setText(ifTypesSupportedByResource.get(0));
                }
            }
        }

        Label otherFilterLbl = new Label(paramsGrp, SWT.NONE);
        otherFilterLbl.setText("Other filters");

        queryTxt = new Text(paramsGrp, SWT.BORDER);
        gd = new GridData();
        gd.grabExcessHorizontalSpace = true;
        gd.horizontalAlignment = SWT.FILL;
        queryTxt.setLayoutData(gd);
        queryTxt.addModifyListener(new ModifyListener() {
            @Override
            public void modifyText(ModifyEvent e) {
                otherFilters = queryTxt.getText();
            }
        });

        return compLayout;
    }

    @Override
    protected boolean isResizable() {
        return true;
    }

    @Override
    public boolean isHelpAvailable() {
        return false;
    }

    @Override
    protected Button createButton(Composite parent, int id, String label,
            boolean defaultButton) {
        if (id == IDialogConstants.OK_ID) {
            label = "GET";
        }
        return super.createButton(parent, id, label, defaultButton);
    }

    public String getOtherFilters() {
        return otherFilters;
    }

    public String getIfType() {
        if (ifTypes.containsKey(ifType)) {
            return ifTypes.get(ifType);
        }
        return ifType;
    }

}
