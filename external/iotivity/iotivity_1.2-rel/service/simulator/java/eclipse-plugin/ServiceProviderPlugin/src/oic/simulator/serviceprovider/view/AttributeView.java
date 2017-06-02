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

package oic.simulator.serviceprovider.view;

import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.viewers.ILabelProviderListener;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.viewers.ITableLabelProvider;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.StructuredSelection;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.jface.viewers.TreeViewerColumn;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.jface.window.Window;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.MenuAdapter;
import org.eclipse.swt.events.MenuEvent;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.widgets.MenuItem;
import org.eclipse.swt.widgets.MessageBox;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.TreeColumn;
import org.eclipse.swt.widgets.TreeItem;
import org.eclipse.ui.part.ViewPart;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.oic.simulator.ArrayProperty;
import org.oic.simulator.AttributeProperty;
import org.oic.simulator.AttributeValue;
import org.oic.simulator.AttributeValue.TypeInfo;
import org.oic.simulator.AttributeValue.ValueType;
import org.oic.simulator.ILogger.Level;
import org.oic.simulator.ModelProperty;
import org.oic.simulator.SimulatorResourceAttribute;
import org.oic.simulator.SimulatorResourceModel;

import oic.simulator.serviceprovider.Activator;
import oic.simulator.serviceprovider.listener.IAutomationListener;
import oic.simulator.serviceprovider.listener.IDataChangeListener;
import oic.simulator.serviceprovider.listener.ISelectionChangedListener;
import oic.simulator.serviceprovider.manager.ResourceManager;
import oic.simulator.serviceprovider.manager.UiListenerHandler;
import oic.simulator.serviceprovider.model.AttributeElement;
import oic.simulator.serviceprovider.model.Resource;
import oic.simulator.serviceprovider.model.ResourceRepresentation;
import oic.simulator.serviceprovider.model.SingleResource;
import oic.simulator.serviceprovider.utils.Constants;
import oic.simulator.serviceprovider.utils.Utility;
import oic.simulator.serviceprovider.view.dialogs.ModelArrayAddItemDialog;

/**
 * This class manages and shows the attribute view in the perspective.
 */
public class AttributeView extends ViewPart {

    public static final String        VIEW_ID        = "oic.simulator.serviceprovider.view.attribute";

    private TreeViewer                attViewer;

    private AttributeEditingSupport   attributeEditor;

    private ISelectionChangedListener resourceSelectionChangedListener;
    private IAutomationListener       automationUIListener;
    private IDataChangeListener       dataChangeListener;

    private final String[]            attTblHeaders  = { "Name", "Value",
            "Automation"                            };
    private final Integer[]           attTblColWidth = { 150, 190, 150 };

    private ResourceManager           resourceManager;

    public AttributeView() {

        resourceManager = Activator.getDefault().getResourceManager();

        resourceSelectionChangedListener = new ISelectionChangedListener() {

            @Override
            public void onResourceSelectionChange(final Resource resource) {
                Display.getDefault().asyncExec(new Runnable() {
                    @Override
                    public void run() {
                        if (null != attViewer) {
                            Tree tree = attViewer.getTree();
                            if (null == tree || tree.isDisposed()) {
                                return;
                            }

                            // Enabling/disabling the tree based on the resource
                            // and automation status.
                            if (resource instanceof SingleResource)
                                if (((SingleResource) resource)
                                        .isResourceAutomationInProgress())
                                    tree.setEnabled(false);
                                else
                                    tree.setEnabled(true);

                            if (null != resource
                                    && null != resource
                                            .getResourceRepresentation()) {
                                attViewer.setInput(resource
                                        .getResourceRepresentation());
                                attViewer.expandAll();
                                tree.setLinesVisible(true);

                            } else {
                                attViewer.setInput(null);
                                tree.setLinesVisible(false);
                            }
                        }
                    }
                });
            }
        };

        dataChangeListener = new IDataChangeListener() {

            @Override
            public void add(final AttributeElement attribute) {
                Display.getDefault().asyncExec(new Runnable() {
                    @Override
                    public void run() {
                        attViewer.refresh(attribute.getParent());
                        attViewer.expandAll();
                    }
                });
            }

            @Override
            public void remove(final AttributeElement attribute) {
                Display.getDefault().asyncExec(new Runnable() {
                    @Override
                    public void run() {
                        attViewer.refresh(attribute.getParent());
                        attViewer.expandAll();
                    }
                });
            }

            @Override
            public void update(final AttributeElement attribute) {
                Display.getDefault().asyncExec(new Runnable() {
                    @Override
                    public void run() {
                        attViewer.update(attribute, null);
                        attViewer.expandAll();
                    }
                });
            }
        };

        automationUIListener = new IAutomationListener() {

            @Override
            public void onResourceAutomationStart(final SingleResource resource) {
                Display.getDefault().asyncExec(new Runnable() {

                    @Override
                    public void run() {
                        if (null == resource || null == attViewer) {
                            return;
                        }
                        Resource resourceInSelection = resourceManager
                                .getCurrentResourceInSelection();
                        if (null == resourceInSelection) {
                            return;
                        }
                        // Checking whether attributes view is currently
                        // displaying the attributes of the
                        // resource whose automation has just started
                        if (resource == resourceInSelection) {
                            Tree tree;
                            tree = attViewer.getTree();
                            if (!tree.isDisposed()) {
                                // Disabling the table to prevent interactions
                                // during the automation
                                tree.setEnabled(false);
                                tree.deselectAll();
                            }
                        }
                    }
                });
            }

            @Override
            public void onAutomationComplete(final SingleResource resource,
                    final String attName) {
                // This method notifies the completion of attribute level
                // automation.
                Display.getDefault().asyncExec(new Runnable() {

                    @Override
                    public void run() {
                        if (null == resource) {
                            return;
                        }
                        // Check if the given resourceURI is the uri of the
                        // resource whose attributes are currently being
                        // displayed by this view.
                        Resource resourceInSelection = resourceManager
                                .getCurrentResourceInSelection();
                        if (resource != resourceInSelection) {
                            return;
                        }
                        Tree tree;
                        tree = attViewer.getTree();
                        if (!tree.isDisposed()) {
                            tree.setEnabled(true);
                        }
                    }
                });
            }
        };
    }

    @Override
    public void createPartControl(Composite parent) {
        Color color = Display.getDefault().getSystemColor(SWT.COLOR_WHITE);

        parent.setLayout(new GridLayout());
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        parent.setLayoutData(gd);

        Group attGroup = new Group(parent, SWT.NONE);
        attGroup.setLayout(new GridLayout());
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        attGroup.setLayoutData(gd);
        attGroup.setText("Attributes");
        attGroup.setBackground(color);

        Tree addressTree = new Tree(attGroup, SWT.SINGLE | SWT.BORDER
                | SWT.H_SCROLL | SWT.V_SCROLL | SWT.FULL_SELECTION);
        addressTree.setHeaderVisible(true);

        attViewer = new TreeViewer(addressTree);

        createAttributeColumns(attViewer);

        // make lines and header visible
        Tree tree = attViewer.getTree();
        tree.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));
        tree.setHeaderVisible(true);
        tree.setLinesVisible(true);

        attViewer.setContentProvider(new AttributeContentProvider());
        attViewer.setLabelProvider(new AttributeLabelProvider());

        addManagerListeners();

        // Check whether there is any resource selected already
        Resource resource = resourceManager.getCurrentResourceInSelection();
        if (resource != null) {
            attViewer.setInput(resource.getResourceRepresentation());
        }
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
                .createAttributeValueEditor(attViewer, true));

        TreeColumn automation = new TreeColumn(tree, SWT.NONE);
        automation.setWidth(attTblColWidth[2]);
        automation.setText(attTblHeaders[2]);
        TreeViewerColumn automationVwrCol = new TreeViewerColumn(attViewer,
                automation);
        automationVwrCol.setEditingSupport(attributeEditor
                .createAutomationEditor(attViewer));

        addColumnListeners();

        addMenuItems();
    }

    private void addColumnListeners() {
        TreeColumn[] columns = attViewer.getTree().getColumns();
        for (TreeColumn column : columns) {
            column.addSelectionListener(new SelectionAdapter() {
                @Override
                public void widgetSelected(SelectionEvent e) {
                    // Refreshing the viewer. If combo list is open,
                    // then click events on other parts of the view or outside
                    // the combo should hide the editor.
                    // Refreshing the viewer hides the combo and other editors
                    // which are active.
                    attViewer.refresh();
                }
            });
        }
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
                    // Check whether a new item can be added to the array by
                    // checking
                    // the array property of the current attribute in
                    // selection(Model Array type attribute).
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

                    AttributeValue attValue = attribute.value();
                    AttributeProperty attProperty = attribute.property();
                    if (null != attProperty
                            && attProperty instanceof ArrayProperty) {
                        ArrayProperty prop = attProperty.asArray();
                        if (null != prop && !prop.isVariable()) {
                            SimulatorResourceModel[] model = (SimulatorResourceModel[]) attValue
                                    .get();
                            if (null != model
                                    && model.length >= prop.maxItems()) {
                                MessageDialog
                                        .openError(
                                                Display.getDefault()
                                                        .getActiveShell(),
                                                "Unable to perform the operation.",
                                                "Exceeding the maximum number of array elements allowed for this attribute.\n"
                                                        + "Maximum number of allowed array element(s): "
                                                        + prop.maxItems());
                                return;
                            }
                        }
                    }

                    ModelArrayAddItemDialog dialog = new ModelArrayAddItemDialog(
                            Display.getDefault().getActiveShell(),
                            representation);
                    if (Window.OK == dialog.open()) {
                        // Add the new item to the local resource
                        // representation.
                        AttributeElement newElement = (AttributeElement) representation
                                .getAttributes().values().toArray()[0];
                        SimulatorResourceAttribute newAttribute = newElement
                                .getSimulatorResourceAttribute();
                        SimulatorResourceModel newModel = (SimulatorResourceModel) newAttribute
                                .value().get();

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

                        attElement.update(newAttribute);

                        boolean updated = resourceManager.attributeValueUpdated(
                                (SingleResource) resourceManager
                                        .getCurrentResourceInSelection(),
                                attribute.name(), newValue);

                        if (!updated) {
                            attribute.setValue(new AttributeValue(modelArray));
                            attElement.update(newAttribute);

                            MessageDialog
                                    .openInformation(Display.getDefault()
                                            .getActiveShell(),
                                            "Operation failed",
                                            "Failed to insert a new item in the array.");
                        } else {
                            // Highlight the newly added item.
                            if (attElement.hasChildren()) {
                                AttributeElement addedElement = attElement
                                        .getChildren().get("[" + i + "]");
                                attViewer.setSelection(new StructuredSelection(
                                        addedElement), true);
                            }
                        }
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
                // Check whether any existing item can be removed from the array
                // by checking
                // the array property of the current attribute in
                // selection(Model Array type attribute).
                SimulatorResourceAttribute parentSRA = parentElement
                        .getSimulatorResourceAttribute();
                AttributeValue value = parentSRA.value();
                AttributeProperty attProperty = parentSRA.property();
                if (null != attProperty && attProperty instanceof ArrayProperty) {
                    ArrayProperty prop = attProperty.asArray();
                    if (null != prop) {
                        SimulatorResourceModel[] model = (SimulatorResourceModel[]) value
                                .get();
                        if (null != model && model.length <= prop.minItems()) {
                            MessageDialog
                                    .openError(
                                            Display.getDefault()
                                                    .getActiveShell(),
                                            "Unable to perform the operation.",
                                            "Violating the minimum number of array elements allowed for this attribute.\n"
                                                    + "Minimum number of allowed array element(s): "
                                                    + prop.minItems());
                            return;
                        }
                    }
                }

                MessageBox dialog = new MessageBox(menu.getShell(),
                        SWT.ICON_QUESTION | SWT.OK | SWT.CANCEL);
                dialog.setText("Confirm action");
                dialog.setMessage("Do you want to delete this item from the array?");
                int retval = dialog.open();
                if (retval != SWT.OK) {
                    return;
                }

                // Removing the element from the attribute value.
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

                resourceManager.attributeValueUpdated(
                        (SingleResource) resourceManager
                                .getCurrentResourceInSelection(), parentSRA
                                .name(), newValue);

                attViewer.refresh(parentElement);
            }
        });
    }

    private ResourceRepresentation getRepresentationForOneDimensionTopLevelAttribute() {
        ResourceRepresentation representation;

        AttributeValue value = null;
        ModelProperty property = null;
        SimulatorResourceAttribute attribute;

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
            Activator
                    .getDefault()
                    .getLogManager()
                    .log(Level.ERROR.ordinal(),
                            new Date(),
                            "There is an error while creating an instance of the model element.\n"
                                    + Utility.getSimulatorErrorString(e, null));
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

        attribute = new SimulatorResourceAttribute(modelArrayAtt.name(), value,
                property);

        Map<String, SimulatorResourceAttribute> attributes = new HashMap<String, SimulatorResourceAttribute>();
        attributes.put(attribute.name(), attribute);

        representation = new ResourceRepresentation(attributes);

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

    private void addManagerListeners() {
        UiListenerHandler.getInstance().addResourceSelectionChangedUIListener(
                resourceSelectionChangedListener);
        UiListenerHandler.getInstance().addDataChangeListener(
                dataChangeListener);
        UiListenerHandler.getInstance().addAutomationUIListener(
                automationUIListener);
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

    class AttributeLabelProvider implements ITableLabelProvider {

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
            if (col == 2) {
                if (element instanceof AttributeElement) {
                    // Ignore for non-single resource
                    Resource res = resourceManager
                            .getCurrentResourceInSelection();
                    if (res instanceof SingleResource) {
                        AttributeElement attrElement = (AttributeElement) element;
                        if (attrElement.isAutoUpdateSupport()
                                && !attrElement.isReadOnly()) {
                            if (attrElement.isAutoUpdateInProgress()) {
                                return Activator.getDefault()
                                        .getImageRegistry()
                                        .get(Constants.CHECKED);
                            } else {
                                return Activator.getDefault()
                                        .getImageRegistry()
                                        .get(Constants.UNCHECKED);
                            }
                        }
                    }
                }
            }
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

                    case 2: {
                        // Ignore for non-single resource
                        Resource res = resourceManager
                                .getCurrentResourceInSelection();
                        if (res instanceof SingleResource) {
                            SimulatorResourceAttribute attribute = attrElement
                                    .getSimulatorResourceAttribute();
                            TypeInfo type = attribute.value().typeInfo();
                            if (type.mType == AttributeValue.ValueType.ARRAY) {
                                if (type.mBaseType != AttributeValue.ValueType.RESOURCEMODEL) {
                                    return "NA";
                                }
                            } else if (type.mType != AttributeValue.ValueType.RESOURCEMODEL) {
                                Object parent = attrElement.getParent();
                                if (null != parent
                                        && !(parent instanceof ResourceRepresentation)) {
                                    return "NA";
                                } else if (attrElement.isReadOnly()) {
                                    return "NA";
                                } else if (attrElement.isAutoUpdateSupport()) {
                                    if (attrElement.isAutoUpdateInProgress())
                                        return Constants.ENABLED;
                                    else
                                        return Constants.DISABLED;
                                }
                            }
                        }

                        return "";
                    }
                }
            }

            return null;
        }

    }

    @Override
    public void dispose() {
        // Unregister the selection listener
        if (null != resourceSelectionChangedListener) {
            UiListenerHandler.getInstance()
                    .removeResourceSelectionChangedUIListener(
                            resourceSelectionChangedListener);
        }

        // Unregister the data model change listener
        if (null != dataChangeListener) {
            UiListenerHandler.getInstance().removeDataChangeListener(
                    dataChangeListener);
        }

        // Unregister the automation complete listener
        if (null != automationUIListener) {
            UiListenerHandler.getInstance().removeAutomationUIListener(
                    automationUIListener);
        }

        super.dispose();
    }

    @Override
    public void setFocus() {

    }
}