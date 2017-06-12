/*
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

import org.eclipse.jface.dialogs.TitleAreaDialog;
import org.eclipse.jface.viewers.ILabelProviderListener;
import org.eclipse.jface.viewers.ITableLabelProvider;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.jface.viewers.TreeViewerColumn;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.TreeColumn;

import java.util.Map;

import org.oic.simulator.AttributeValue.ValueType;
import org.oic.simulator.SimulatorResourceAttribute;

import oic.simulator.clientcontroller.Activator;
import oic.simulator.clientcontroller.manager.ResourceManager;
import oic.simulator.clientcontroller.remoteresource.AttributeElement;
import oic.simulator.clientcontroller.remoteresource.RemoteResource;
import oic.simulator.clientcontroller.remoteresource.ResourceRepresentation;
import oic.simulator.clientcontroller.utils.Utility;
import oic.simulator.clientcontroller.view.AttributeEditingSupport;

/**
 * This class manages and shows the automation settings dialog from the
 * attribute view.
 */
public class ModelArrayAddItemDialog extends TitleAreaDialog {

    private TreeViewer              attViewer;

    private AttributeEditingSupport attributeEditor;

    private final String[]          attTblHeaders  = { "Attribute Name",
            "Attribute Value"                     };
    private final Integer[]         attTblColWidth = { 200, 300 };

    private ResourceManager         resourceManager;

    private ResourceRepresentation  representation;

    private TitleAreaDialog         dialog;

    public ModelArrayAddItemDialog(Shell parentShell, TitleAreaDialog dialog,
            ResourceRepresentation representation) {
        super(parentShell);
        resourceManager = Activator.getDefault().getResourceManager();
        this.dialog = dialog;
        this.representation = representation;
    }

    @Override
    public void create() {
        super.create();
        setTitle("Add Items To Complex Array");
        setMessage("Add one or more items in the complex array type attribute");
    }

    @Override
    protected Control createDialogArea(Composite parent) {
        Composite compLayout = (Composite) super.createDialogArea(parent);

        compLayout.setLayout(new GridLayout());
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        compLayout.setLayoutData(gd);

        Group attGroup = new Group(compLayout, SWT.NONE);
        attGroup.setLayout(new GridLayout());
        gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        attGroup.setLayoutData(gd);
        attGroup.setText("Attributes");

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

        // Check whether there is any resource selected already
        RemoteResource resource = resourceManager
                .getCurrentResourceInSelection();
        if (resource != null) {
            attViewer.setInput(representation);
            attViewer.expandAll();
        }

        return compLayout;
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
                .createAttributeValueEditor(attViewer, dialog));

        addColumnListeners();
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
                Map<String, AttributeElement> children = ((AttributeElement) attribute)
                        .getChildren();
                if (null != children) {
                    return children.values().toArray();
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
    protected Point getInitialSize() {
        Point initialPoint = super.getInitialSize();
        initialPoint.y += 100;
        return initialPoint;
    }

    @Override
    protected boolean isResizable() {
        return true;
    }

    @Override
    public boolean isHelpAvailable() {
        return false;
    }
}
