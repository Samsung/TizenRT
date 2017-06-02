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

import org.eclipse.jface.viewers.ISelectionChangedListener;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.SelectionChangedEvent;
import org.eclipse.jface.viewers.StyledCellLabelProvider;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.jface.viewers.TableViewerColumn;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.jface.viewers.ViewerCell;
import org.eclipse.jface.window.Window;
import org.eclipse.jface.wizard.WizardPage;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Table;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

import oic.simulator.serviceprovider.model.AttributeHelper;
import oic.simulator.serviceprovider.model.AttributeHelper.ValidValuesType;
import oic.simulator.serviceprovider.utils.Constants;
import oic.simulator.serviceprovider.utils.Utility;

public class SimpleResourceAddAttributePage extends WizardPage {

    private TableViewer          attTblViewer;
    private Button               addBtn;
    private Button               remBtn;
    private Button               editBtn;

    private final String[]       attTblHeaders  = { "Name", "Type", "Range",
            "Allowed Values", "Default Value"  };
    private final Integer[]      attTblColWidth = { 90, 75, 75, 150, 75 };

    private Set<AttributeHelper> attributes;

    protected SimpleResourceAddAttributePage() {
        super("Add Attributes");
        attributes = new HashSet<AttributeHelper>();
    }

    @Override
    public void createControl(Composite parent) {
        setPageComplete(false);
        setTitle(Constants.SIMPLE_RESOURCE_ADD_ATTRIBUTE_PAGE_TITLE);
        setMessage(Constants.SIMPLE_RESOURCE_ADD_ATTRIBUTE_PAGE_MESSAGE);
        Composite compContent = new Composite(parent, SWT.NONE);
        GridLayout gridLayout = new GridLayout(2, false);
        compContent.setLayout(gridLayout);
        GridData gd = new GridData(SWT.FILL, SWT.FILL, true, true);
        compContent.setLayoutData(gd);

        Label lbl = new Label(compContent, SWT.NULL);
        lbl.setText("Attributes:");
        gd = new GridData();
        gd.horizontalSpan = 2;
        lbl.setLayoutData(gd);

        setupAttributeTable(compContent);

        setupAttributeControls(compContent);

        setUiListeners(compContent);

        setControl(compContent);
    }

    private void setupAttributeTable(Composite attComp) {
        attTblViewer = new TableViewer(attComp, SWT.MULTI | SWT.H_SCROLL
                | SWT.V_SCROLL | SWT.FULL_SELECTION | SWT.BORDER);

        createAttributeColumns(attTblViewer);

        // Make lines and header visible
        Table table = attTblViewer.getTable();
        table.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));
        table.setHeaderVisible(true);
        table.setLinesVisible(true);

        attTblViewer.setContentProvider(new AttributeContentProvider());
    }

    public void createAttributeColumns(TableViewer tableViewer) {

        TableViewerColumn attName = new TableViewerColumn(tableViewer, SWT.NONE);
        attName.getColumn().setWidth(attTblColWidth[0]);
        attName.getColumn().setText(attTblHeaders[0]);
        attName.setLabelProvider(new StyledCellLabelProvider() {
            @Override
            public void update(ViewerCell cell) {
                Object e = cell.getElement();
                if (null == e) {
                    return;
                }
                AttributeHelper att = (AttributeHelper) e;
                cell.setText(att.getAttributeName());

            }
        });

        TableViewerColumn attType = new TableViewerColumn(tableViewer, SWT.NONE);
        attType.getColumn().setWidth(attTblColWidth[1]);
        attType.getColumn().setText(attTblHeaders[1]);
        attType.setLabelProvider(new StyledCellLabelProvider() {
            @Override
            public void update(ViewerCell cell) {
                Object e = cell.getElement();
                if (null == e) {
                    return;
                }
                AttributeHelper att = (AttributeHelper) e;
                cell.setText(att.getAttributeType());
            }
        });

        TableViewerColumn attRange = new TableViewerColumn(tableViewer,
                SWT.NONE);
        attRange.getColumn().setWidth(attTblColWidth[2]);
        attRange.getColumn().setText(attTblHeaders[2]);
        attRange.setLabelProvider(new StyledCellLabelProvider() {
            @Override
            public void update(ViewerCell cell) {
                Object e = cell.getElement();
                if (null == e) {
                    return;
                }
                AttributeHelper att = (AttributeHelper) e;
                if (att.getValidValuesType() != ValidValuesType.RANGE) {
                    cell.setText("Nil");
                } else {
                    String min = att.getMin();
                    String max = att.getMax();
                    if (null != min && null != max) {
                        cell.setText("[" + att.getMin() + " to " + att.getMax()
                                + "]");
                    } else {
                        cell.setText("Nil");
                    }
                }
            }
        });

        TableViewerColumn attAllwdValues = new TableViewerColumn(tableViewer,
                SWT.NONE);
        attAllwdValues.getColumn().setWidth(attTblColWidth[3]);
        attAllwdValues.getColumn().setText(attTblHeaders[3]);
        attAllwdValues.setLabelProvider(new StyledCellLabelProvider() {
            @Override
            public void update(ViewerCell cell) {
                Object e = cell.getElement();
                if (null == e) {
                    return;
                }
                AttributeHelper att = (AttributeHelper) e;
                if (att.getValidValuesType() != ValidValuesType.VALUESET) {
                    cell.setText("Nil");
                } else {
                    cell.setText(att.getAllowedValues().toString());
                }
            }
        });

        TableViewerColumn attDflValue = new TableViewerColumn(tableViewer,
                SWT.NONE);
        attDflValue.getColumn().setWidth(attTblColWidth[4]);
        attDflValue.getColumn().setText(attTblHeaders[4]);
        attDflValue.setLabelProvider(new StyledCellLabelProvider() {
            @Override
            public void update(ViewerCell cell) {
                Object e = cell.getElement();
                if (null == e) {
                    return;
                }
                AttributeHelper att = (AttributeHelper) e;
                cell.setText(att.getAttributeDflValue());
            }
        });
    }

    private void setupAttributeControls(Composite compContent) {

        Composite innerComp = new Composite(compContent, SWT.NULL);
        innerComp.setLayout(new GridLayout());
        GridData gd = new GridData();
        gd.verticalAlignment = SWT.TOP;
        innerComp.setLayoutData(gd);

        addBtn = new Button(innerComp, SWT.PUSH);
        addBtn.setText("Add");
        gd = new GridData();
        gd.widthHint = 70;
        addBtn.setLayoutData(gd);

        editBtn = new Button(innerComp, SWT.PUSH);
        editBtn.setText("Edit");
        editBtn.setEnabled(false);
        gd = new GridData();
        gd.widthHint = 70;
        editBtn.setLayoutData(gd);

        remBtn = new Button(innerComp, SWT.PUSH);
        remBtn.setText("Remove");
        remBtn.setEnabled(false);
        gd = new GridData();
        gd.widthHint = 70;
        remBtn.setLayoutData(gd);
    }

    private void setUiListeners(Composite compContent) {
        addBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                Set<String> rTypes = Utility.getAttributeTypes();
                AddAttributeDialog dialog = new AddAttributeDialog(Display
                        .getDefault().getActiveShell(), null, rTypes,
                        attributes);
                if (dialog.open() == Window.OK) {
                    AttributeHelper att = dialog.getAttHelper();
                    attributes.add(att);
                    remBtn.setEnabled(true);
                    AttributeContentProvider provider = (AttributeContentProvider) attTblViewer
                            .getContentProvider();
                    provider.addAttribute(att);
                    attTblViewer.add(att);
                    attTblViewer.getTable().deselectAll();
                    attTblViewer.getTable().select(
                            attTblViewer.getTable().getItemCount() - 1);
                    editBtn.setEnabled(true);
                }
            }
        });

        editBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                IStructuredSelection selection = (IStructuredSelection) attTblViewer
                        .getSelection();
                if (null == selection) {
                    editBtn.setEnabled(false);
                    return;
                }
                AttributeHelper att = (AttributeHelper) selection
                        .getFirstElement();
                if (null == att) {
                    editBtn.setEnabled(false);
                    return;
                }
                // Opening the dialog for edit operation.
                // Passing the attribute to populate UI controls with data.
                Set<String> rTypes = Utility.getAttributeTypes();
                AddAttributeDialog dialog = new AddAttributeDialog(Display
                        .getDefault().getActiveShell(), att, rTypes, attributes);
                if (dialog.open() != Window.OK) {
                    AttributeHelper newAtt = dialog.getAttClone();
                    if (null != newAtt) {
                        att.setAttributeName(newAtt.getAttributeName());
                        att.setAttributeType(newAtt.getAttributeType());
                        att.setAttributeDflValue(newAtt.getAttributeDflValue());
                        att.setValidValuesType(newAtt.getValidValuesType());
                        att.setMin(newAtt.getMin());
                        att.setMax(newAtt.getMax());
                        att.setAllowedValues(newAtt.getAllowedValues());
                    }
                }
                attTblViewer.update(att, null);
            }
        });

        remBtn.addSelectionListener(new SelectionAdapter() {
            @Override
            public void widgetSelected(SelectionEvent e) {
                IStructuredSelection selection = (IStructuredSelection) attTblViewer
                        .getSelection();
                if (null != selection) {
                    Iterator<?> itr = selection.iterator();
                    while (itr.hasNext()) {
                        AttributeHelper att = (AttributeHelper) itr.next();
                        attTblViewer.remove(att);
                        attributes.remove(att);
                        AttributeContentProvider provider = (AttributeContentProvider) attTblViewer
                                .getContentProvider();
                        provider.removeAttribute(att);
                    }
                    if (attributes.size() < 1) {
                        remBtn.setEnabled(false);
                    }
                    editBtn.setEnabled(false);
                }
            }
        });

        attTblViewer
                .addSelectionChangedListener(new ISelectionChangedListener() {

                    @Override
                    public void selectionChanged(SelectionChangedEvent event) {
                        remBtn.setEnabled(true);
                        IStructuredSelection selection = (IStructuredSelection) event
                                .getSelection();
                        if (null == selection) {
                            return;
                        }
                        if (selection.size() > 1) {
                            editBtn.setEnabled(false);
                        } else {
                            editBtn.setEnabled(true);
                        }
                    }
                });
    }

    private static class AttributeContentProvider implements
            ITreeContentProvider {

        List<AttributeHelper> attList = new ArrayList<AttributeHelper>();

        @SuppressWarnings("unchecked")
        @Override
        public void inputChanged(Viewer viewer, Object oldInput, Object newInput) {
            attList = (List<AttributeHelper>) newInput;
        }

        @Override
        public Object[] getChildren(Object element) {
            return new Object[0];
        }

        @Override
        public Object[] getElements(Object input) {
            return attList.toArray();
        }

        @Override
        public Object getParent(Object element) {
            return null;
        }

        @Override
        public boolean hasChildren(Object element) {
            return false;
        }

        @Override
        public void dispose() {
        }

        public synchronized void addAttribute(AttributeHelper newElement) {
            attList.add(newElement);
        }

        public synchronized void removeAttribute(AttributeHelper element) {
            attList.remove(element);
        }
    }

    @Override
    public boolean canFlipToNextPage() {
        return false;
    }

    public Set<AttributeHelper> getAttributes() {
        return attributes;
    }
}
