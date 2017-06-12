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
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.dialogs.TitleAreaDialog;
import org.eclipse.jface.viewers.ILabelProviderListener;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.viewers.ITableLabelProvider;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.jface.viewers.TreeViewerColumn;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.jface.window.Window;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.MenuAdapter;
import org.eclipse.swt.events.MenuEvent;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.widgets.MenuItem;
import org.eclipse.swt.widgets.MessageBox;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.TreeColumn;
import org.eclipse.swt.widgets.TreeItem;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import org.oic.simulator.ArrayProperty;
import org.oic.simulator.AttributeProperty;
import org.oic.simulator.AttributeValue;
import org.oic.simulator.AttributeValue.TypeInfo;
import org.oic.simulator.AttributeValue.ValueType;
import org.oic.simulator.ILogger.Level;
import org.oic.simulator.ModelProperty;
import org.oic.simulator.SimulatorResourceAttribute;
import org.oic.simulator.SimulatorResourceModel;
import org.oic.simulator.client.SimulatorRemoteResource.RequestType;

import oic.simulator.clientcontroller.Activator;
import oic.simulator.clientcontroller.remoteresource.AttributeElement;
import oic.simulator.clientcontroller.remoteresource.RemoteResource;
import oic.simulator.clientcontroller.remoteresource.ResourceRepresentation;
import oic.simulator.clientcontroller.utils.Constants;
import oic.simulator.clientcontroller.utils.Utility;
import oic.simulator.clientcontroller.view.AttributeEditingSupport;

/**
 * This dialog is used for generating a PUT request.
 */
public class PutRequestDialog extends TitleAreaDialog {

    private TreeViewer              attViewer;
    private Combo                   ifTypesCmb;

    private String                  ifType;

    private Map<String, String>     ifTypes;

    private AttributeEditingSupport attributeEditor;

    private ResourceRepresentation  updatedRepresentation;

    private final String[]          attTblHeaders  = { "Name", "Value" };
    private final Integer[]         attTblColWidth = { 200, 200 };

    public PutRequestDialog(Shell parentShell) {
        super(parentShell);
    }

    @Override
    public void create() {
        super.create();
        setTitle("Generate PUT Request");
        setMessage("Dialog which takes input and generates a put request.");
    }

    @Override
    protected Control createDialogArea(Composite parent) {
        Composite compLayout = (Composite) super.createDialogArea(parent);

        Composite rootContainer = new Composite(compLayout, SWT.NONE);
        GridLayout layout = new GridLayout();
        rootContainer.setLayout(layout);
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        rootContainer.setLayoutData(gd);

        Group paramsGrp = new Group(rootContainer, SWT.NONE);
        gd = new GridData();
        gd.horizontalAlignment = SWT.FILL;
        gd.grabExcessHorizontalSpace = true;
        gd.minimumHeight = 50;
        paramsGrp.setLayoutData(gd);
        layout = new GridLayout(2, false);
        layout.verticalSpacing = 10;
        layout.marginTop = 10;
        paramsGrp.setLayout(layout);

        Label ifTypeLbl = new Label(paramsGrp, SWT.NONE);
        ifTypeLbl.setText("Interface Type");

        ifTypesCmb = new Combo(paramsGrp, SWT.NULL);
        gd = new GridData();
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

        Composite container = new Composite(rootContainer, SWT.NONE);
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        container.setLayoutData(gd);
        layout = new GridLayout(1, false);
        layout.verticalSpacing = 10;
        layout.marginTop = 10;
        container.setLayout(layout);

        createTreeViewer(container);

        // Clone the resource model for maintaining a local copy of attributes
        // for PUT requests.
        SimulatorResourceModel resourceModel = null;
        try {
            AttributeValue attValue = Utility
                    .cloneAttributeValue(new AttributeValue(resource
                            .getResourceModelRef()));
            if (null != attValue)
                resourceModel = (SimulatorResourceModel) attValue.get();
        } catch (Exception e) {
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(),
                            new Date(),
                            "There is an error while forming an instance of the attribute value.\n"
                                    + Utility.getSimulatorErrorString(e, null));
        }

        if (null == resourceModel) {
            // Failed to clone. So taking the base copy and continuing the
            // operation.
            resourceModel = resource.getResourceModelRef();
        }

        updatedRepresentation = new ResourceRepresentation(resourceModel);

        if (resource.isConfigUploaded()) {
            try {
                // Request Type needs to be changed to PUT.
                updatedRepresentation.updateAttributeProperties(resource
                        .getRequestModels().get(RequestType.POST),
                        resourceModel);
            } catch (Exception e) {
                Activator
                        .getDefault()
                        .getLogManager()
                        .log(Level.ERROR.ordinal(),
                                new Date(),
                                "There is an error while forming an instance of the attribute value.\n"
                                        + Utility.getSimulatorErrorString(e,
                                                null));
            }
        }

        attViewer.setInput(updatedRepresentation);

        attViewer.expandAll();

        return compLayout;
    }

    private void createTreeViewer(Composite parent) {
        Tree addressTree = new Tree(parent, SWT.SINGLE | SWT.BORDER
                | SWT.H_SCROLL | SWT.V_SCROLL | SWT.FULL_SELECTION);
        addressTree.setHeaderVisible(true);

        attViewer = new TreeViewer(addressTree);

        createAttributeColumns(attViewer);

        // make lines and header visible
        Tree tree = attViewer.getTree();
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        tree.setLayoutData(gd);
        tree.setHeaderVisible(true);
        tree.setLinesVisible(true);

        attViewer.setContentProvider(new AttributeContentProvider());
        attViewer.setLabelProvider(new AttributeLabelProvider());
    }

    public void createAttributeColumns(TreeViewer viewer) {
        Tree tree = viewer.getTree();

        attributeEditor = new AttributeEditingSupport();

        TreeColumn attName = new TreeColumn(tree, SWT.NONE);
        attName.setWidth(attTblColWidth[0]);
        attName.setText(attTblHeaders[0]);

        TreeColumn attValue = new TreeColumn(tree, SWT.NONE);
        attValue.setWidth(attTblColWidth[1]);
        attValue.setText(attTblHeaders[1]);
        TreeViewerColumn attValueVwrCol = new TreeViewerColumn(attViewer,
                attValue);
        attValueVwrCol.setEditingSupport(attributeEditor
                .createAttributeValueEditor(attViewer, this));

        addMenuItems();
    }

    private void addMenuItems() {
        if (null != attViewer) {
            final Tree resourceTreeHead = attViewer.getTree();
            if (null != resourceTreeHead) {
                // Below code creates menu entries and shows them on right
                // clicking a resource
                final Menu menu = new Menu(resourceTreeHead);
                resourceTreeHead.setMenu(menu);
                menu.addMenuListener(new MenuAdapter() {
                    @Override
                    public void menuShown(MenuEvent e) {
                        // Clear existing menu items
                        MenuItem[] items = menu.getItems();
                        for (int index = 0; index < items.length; index++) {
                            items[index].dispose();
                        }

                        IStructuredSelection selection = ((IStructuredSelection) attViewer
                                .getSelection());
                        final AttributeElement attElement = (AttributeElement) selection
                                .getFirstElement();
                        if (null == attElement) {
                            return;
                        }

                        // Check the type of attribute.
                        SimulatorResourceAttribute attribute = attElement
                                .getSimulatorResourceAttribute();
                        if (null == attribute) {
                            return;
                        }

                        AttributeValue value = attribute.value();
                        if (null == value || null == value.get()) {
                            return;
                        }

                        TypeInfo type = value.typeInfo();

                        final Object parent = attElement.getParent();

                        if ((type.mType == ValueType.ARRAY
                                && type.mBaseType == ValueType.RESOURCEMODEL && type.mDepth == 1)
                                && (null == parent || parent instanceof ResourceRepresentation)) {
                            addMenuToOneDimensionalTopLevelModelAttributes(menu);
                            return;
                        }

                        if (null != parent
                                && parent instanceof AttributeElement) {
                            Object grandParent = ((AttributeElement) parent)
                                    .getParent();
                            if (null == grandParent
                                    || grandParent instanceof ResourceRepresentation) {
                                AttributeElement parentElement = (AttributeElement) parent;

                                // Check the type of attribute.
                                SimulatorResourceAttribute parentAttribute = parentElement
                                        .getSimulatorResourceAttribute();
                                if (null != parentAttribute
                                        && null != parentAttribute.value()
                                        && null != parentAttribute.value()
                                                .get()) {
                                    AttributeValue parentValue = parentAttribute
                                            .value();

                                    TypeInfo parentType = parentValue
                                            .typeInfo();
                                    if (parentType.mType == ValueType.ARRAY
                                            && parentType.mBaseType == ValueType.RESOURCEMODEL
                                            && parentType.mDepth == 1) {
                                        addDeleteMenuToArrayItemsOfOneDimensionalModelAttribute(
                                                menu, attElement, parentElement);
                                        return;
                                    }
                                }
                            }
                        }
                    }
                });
            }
        }
    }

    private void addMenuToOneDimensionalTopLevelModelAttributes(Menu menu) {
        // Menu to add items to the array.
        MenuItem addItems = new MenuItem(menu, SWT.NONE);
        addItems.setText("Add Items");
        addItems.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                // Get the attributes.
                ResourceRepresentation representation;
                representation = getRepresentationForOneDimensionTopLevelAttribute();
                if (null == representation) {
                    MessageDialog
                            .openError(Display.getDefault().getActiveShell(),
                                    "Unable to perform the operation.",
                                    "Failed to obtain the required data. Operation cannot be performed.");
                } else {
                    ModelArrayAddItemDialog dialog = new ModelArrayAddItemDialog(
                            Display.getDefault().getActiveShell(),
                            PutRequestDialog.this, representation);
                    if (Window.OK == dialog.open()) {
                        // Add the new item to the local resource
                        // representation.
                        AttributeElement newElement = (AttributeElement) representation
                                .getAttributes().values().toArray()[0];
                        SimulatorResourceAttribute newAttribute = newElement
                                .getSimulatorResourceAttribute();
                        SimulatorResourceModel newModel = (SimulatorResourceModel) newAttribute
                                .value().get();

                        AttributeElement attElement = getSelectedElement();
                        if (null == attElement) {
                            MessageDialog
                                    .openError(Display.getDefault()
                                            .getActiveShell(),
                                            "Unable to perform the operation.",
                                            "Failed to obtain the required data. Operation cannot be performed.");
                            return;
                        }

                        SimulatorResourceAttribute attribute = attElement
                                .getSimulatorResourceAttribute();
                        SimulatorResourceModel[] modelArray = (SimulatorResourceModel[]) attribute
                                .value().get();
                        SimulatorResourceModel[] newModelArray = new SimulatorResourceModel[modelArray.length + 1];

                        int i;
                        for (i = 0; i < modelArray.length; i++) {
                            newModelArray[i] = modelArray[i];
                        }
                        newModelArray[i] = newModel;

                        AttributeValue newValue = new AttributeValue(
                                newModelArray);

                        newAttribute.setValue(newValue);

                        newAttribute.setProperty(attribute.property());

                        attribute.setValue(newValue);

                        attElement.update(newAttribute);

                        attViewer.refresh(attElement);

                        attViewer.expandAll();
                    }
                }
            }
        });
    }

    private void addDeleteMenuToArrayItemsOfOneDimensionalModelAttribute(
            final Menu menu, final AttributeElement elementToDelete,
            final AttributeElement parentElement) {
        // Menu to add items to the array.
        MenuItem addItems = new MenuItem(menu, SWT.NONE);
        addItems.setText("Delete Item");
        addItems.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                MessageBox dialog = new MessageBox(menu.getShell(),
                        SWT.ICON_QUESTION | SWT.OK | SWT.CANCEL);
                dialog.setText("Confirm action");
                dialog.setMessage("Do you want to delete this item from the array?");
                int retval = dialog.open();
                if (retval != SWT.OK) {
                    return;
                }

                // Removing the element from the attribute value.
                SimulatorResourceAttribute parentSRA = parentElement
                        .getSimulatorResourceAttribute();
                AttributeValue value = parentSRA.value();
                SimulatorResourceModel[] modelArray = (SimulatorResourceModel[]) value
                        .get();

                String elementIndexName = elementToDelete
                        .getSimulatorResourceAttribute().name();
                int elementIndex = Integer.parseInt(elementIndexName.substring(
                        elementIndexName.indexOf('[') + 1,
                        elementIndexName.indexOf(']')));

                SimulatorResourceModel[] newModelArray = new SimulatorResourceModel[modelArray.length - 1];
                int sIndex = 0, dIndex = 0;
                for (SimulatorResourceModel model : modelArray) {
                    if (sIndex != elementIndex)
                        newModelArray[dIndex++] = model;
                    sIndex++;
                }

                // Setting the new model array in the attribute.
                AttributeValue newValue = new AttributeValue(newModelArray);
                parentSRA.setValue(newValue);

                // Removing the element from the child map.
                Map<String, AttributeElement> elements = parentElement
                        .getChildren();
                if (null == elements) {
                    MessageDialog
                            .openError(Display.getDefault().getActiveShell(),
                                    "Operation failed.",
                                    "There is an error while removing the array items.");
                    return;
                }

                List<AttributeElement> attElementList = new ArrayList<AttributeElement>();
                attElementList.addAll(elements.values());
                Collections.sort(attElementList, Utility.attributeComparator);

                // Renaming the index of the elements.
                AttributeElement[] attElementArray = attElementList
                        .toArray(new AttributeElement[0]);
                boolean deleted = false;
                int index, newIndex;
                for (index = 0, newIndex = 0; index < attElementArray.length; index++) {
                    if (index == elementIndex) {
                        elements.remove(elementIndexName);
                        deleted = true;
                    } else {
                        if (deleted) {
                            AttributeElement element = attElementArray[index];
                            String curIndexStr = "[" + index + "]";
                            String newIndexStr = "[" + newIndex + "]";

                            element.getSimulatorResourceAttribute().setName(
                                    newIndexStr);

                            elements.remove(curIndexStr);
                            elements.put(newIndexStr, element);
                        }
                        newIndex++;
                    }
                }

                attViewer.refresh(parentElement);
            }
        });
    }

    private ResourceRepresentation getRepresentationForOneDimensionTopLevelAttribute() {
        ResourceRepresentation representation;

        AttributeValue value;
        ModelProperty property = null;

        AttributeElement element = getSelectedElement();
        if (null == element)
            return null;

        SimulatorResourceAttribute modelArrayAtt = element
                .getSimulatorResourceAttribute();
        if (null == modelArrayAtt) {
            return null;
        }

        AttributeValue attValue = modelArrayAtt.value();
        if (null == attValue) {
            return null;
        }

        TypeInfo type = attValue.typeInfo();

        if (!(type.mType == ValueType.ARRAY
                && type.mBaseType == ValueType.RESOURCEMODEL && type.mDepth == 1)) {
            return null;
        }

        SimulatorResourceModel[] modelValue = (SimulatorResourceModel[]) attValue
                .get();
        if (null == modelValue || modelValue.length < 0) {
            return null;
        }

        // Clone an instance of model value.
        try {
            value = Utility.cloneAttributeValue(new AttributeValue(
                    modelValue[0]));
        } catch (Exception e) {
            return null;
        }

        if (null == value) {
            return null;
        }

        // Get the model property of the model value instance.
        AttributeProperty attProperty = modelArrayAtt.property();
        if (null != attProperty && attProperty instanceof ArrayProperty) {
            ArrayProperty prop = attProperty.asArray();
            if (null != prop) {
                AttributeProperty elementProperty = prop.getElementProperty();
                if (null != elementProperty && elementProperty.isModel()) {
                    property = elementProperty.asModel();
                }
            }
        }

        SimulatorResourceAttribute attribute = new SimulatorResourceAttribute(
                modelArrayAtt.name(), value, property);
        Map<String, SimulatorResourceAttribute> attributes = new HashMap<String, SimulatorResourceAttribute>();
        attributes.put(modelArrayAtt.name(), attribute);
        representation = new ResourceRepresentation(attributes, false);

        return representation;
    }

    private AttributeElement getSelectedElement() {
        IStructuredSelection selection = (IStructuredSelection) attViewer
                .getSelection();
        if (null == selection) {
            return null;
        }

        Object obj = selection.getFirstElement();
        if (null == obj) {
            return null;
        }

        Tree t = attViewer.getTree();
        TreeItem item = t.getSelection()[0];
        if (null == item) {
            return null;
        }

        if (!(item.getData() instanceof AttributeElement)) {
            return null;
        }

        return (AttributeElement) item.getData();
    }

    private static class AttributeContentProvider implements
            ITreeContentProvider {

        @Override
        public void dispose() {
        }

        @Override
        public void inputChanged(Viewer viewer, Object oldAttribute,
                Object newAttribute) {
        }

        @Override
        public Object[] getChildren(Object attribute) {
            if (attribute instanceof AttributeElement) {
                List<AttributeElement> attElementList = new ArrayList<AttributeElement>();
                Map<String, AttributeElement> children = ((AttributeElement) attribute)
                        .getChildren();
                if (null != children) {
                    attElementList.addAll(children.values());
                    Collections.sort(attElementList,
                            Utility.attributeComparator);
                    return attElementList.toArray();
                }
            }

            return new Object[0];
        }

        @Override
        public Object getParent(Object attribute) {
            if (attribute instanceof AttributeElement)
                return ((AttributeElement) attribute).getParent();
            return null;
        }

        @Override
        public boolean hasChildren(Object attribute) {
            if (attribute instanceof AttributeElement)
                return ((AttributeElement) attribute).hasChildren();
            return false;
        }

        @Override
        public Object[] getElements(Object resourceModel) {
            if (resourceModel instanceof ResourceRepresentation) {
                return ((ResourceRepresentation) resourceModel).getAttributes()
                        .values().toArray();
            }

            return new Object[0];
        }
    }

    private static class AttributeLabelProvider implements ITableLabelProvider {

        @Override
        public void addListener(ILabelProviderListener arg0) {
        }

        @Override
        public void dispose() {
        }

        @Override
        public boolean isLabelProperty(Object arg0, String arg1) {
            return false;
        }

        @Override
        public void removeListener(ILabelProviderListener arg0) {

        }

        @Override
        public Image getColumnImage(Object element, int col) {
            return null;
        }

        @Override
        public String getColumnText(Object element, int column) {
            if (element instanceof AttributeElement) {
                AttributeElement attrElement = (AttributeElement) element;
                switch (column) {
                    case 0: // Attribute name column
                    {
                        SimulatorResourceAttribute attribute = attrElement
                                .getSimulatorResourceAttribute();
                        return attribute.name();
                    }

                    case 1: // Attribute value column
                    {
                        SimulatorResourceAttribute attribute = attrElement
                                .getSimulatorResourceAttribute();

                        if (attribute.value().typeInfo().mBaseType != ValueType.RESOURCEMODEL) {
                            String value = Utility
                                    .getAttributeValueAsString(attribute
                                            .value());
                            if (null == value) {
                                value = "";
                            }
                            return value;
                        }
                        return null;
                    }
                }
            }
            return null;
        }
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
            label = "PUT";
        }
        return super.createButton(parent, id, label, defaultButton);
    }

    public ResourceRepresentation getUpdatedRepresentation() {
        return updatedRepresentation;
    }

    public String getIfType() {
        if (ifTypes.containsKey(ifType)) {
            return ifTypes.get(ifType);
        }
        return ifType;
    }

}
