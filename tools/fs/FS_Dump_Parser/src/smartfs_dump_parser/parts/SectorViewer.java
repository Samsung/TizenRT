////////////////////////////////////////////////////////////////////
//
// Copyright 2019 Samsung Electronics All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied. See the License for the specific
// language governing permissions and limitations under the License.
//
////////////////////////////////////////////////////////////////////

package smartfs_dump_parser.parts;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import javax.annotation.PostConstruct;

import org.eclipse.e4.ui.di.Focus;

import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.viewers.ArrayContentProvider;
import org.eclipse.jface.viewers.ColumnLabelProvider;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.jface.viewers.TableViewerColumn;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;

import smartfs_dump_parser.data_model.Sector;
import smartfs_dump_parser.data_model.SectorStatus;
import smartfs_dump_parser.data_model.SmartFileSystem;

public class SectorViewer {

	private TableViewer tableViewer;
	private Button activeButton;
	private Button dirtyButton;
	private Button cleanButton;

	@PostConstruct
	public void createComposite(Composite parent) {
		parent.setLayout(new GridLayout(3, false));

		createCheckbox(parent);

		createSectorTable(parent);
	}

	@Focus
	public void setFocus() {
		tableViewer.getTable().setFocus();
	}

	public TableViewer getSectorViewer() {
		return tableViewer;
	}

	private void createCheckbox(final Composite parent) {
		activeButton = new Button(parent, SWT.CHECK);
		activeButton.setText("Active Sectors");
		activeButton.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				if (activeButton.getSelection()) {
					if (SmartFileSystem.getSectors().size() == 0) {
						MessageDialog.openError(parent.getShell(), "Error", "Please open a dump file first.");
						activeButton.setSelection(false);
						return;
					}
					Map<Integer, Sector> activeSectors = SmartFileSystem.getActiveSectorsMap();
					if (activeSectors == null) {
						System.out.println("Active sectors are not classified yet..");
					} else {
						tableViewer.setInput(activeSectors.values());
						tableViewer.refresh();
					}
					dirtyButton.setSelection(false);
					cleanButton.setSelection(false);
				}
			}
		});

		dirtyButton = new Button(parent, SWT.CHECK);
		dirtyButton.setText("Dirty Sectors");
		dirtyButton.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				if (SmartFileSystem.getSectors().size() == 0) {
					MessageDialog.openError(parent.getShell(), "Error", "Please open a dump file first.");
					dirtyButton.setSelection(false);
					return;
				}
				Map<Integer, Sector> dirtySectors = SmartFileSystem.getDirtySectorsMap();
				if (dirtySectors == null) {
					System.out.println("Dirty sectors are not classified yet..");
				} else {
					tableViewer.setInput(new ArrayList<Sector>(dirtySectors.values()));
					tableViewer.refresh();
				}
				activeButton.setSelection(false);
				cleanButton.setSelection(false);
			}
		});

		cleanButton = new Button(parent, SWT.CHECK);
		cleanButton.setText("Clean Sectors");
		cleanButton.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				if (SmartFileSystem.getSectors().size() == 0) {
					MessageDialog.openError(parent.getShell(), "Error", "Please open a dump file first.");
					cleanButton.setSelection(false);
					return;
				}
				Map<Integer, Sector> cleanSectors = SmartFileSystem.getCleanSectorsMap();
				if (cleanSectors == null) {
					System.out.println("Clean sectors are not classified yet..");
				} else {
					tableViewer.setInput(new ArrayList<Sector>(cleanSectors.values()));
					tableViewer.refresh();
				}
				activeButton.setSelection(false);
				dirtyButton.setSelection(false);
			}
		});
	}

	public static List<Sector> filterSectors(SectorStatus targetStatus, List<Sector> sectorList) {
		ArrayList<Sector> resultList = new ArrayList<Sector>();
		switch (targetStatus) {
		case ACTIVE:
			for (Sector s : sectorList) {
				int status = s.getHeader().getStatus();
				if (status < 128 && status >= 64) {
					resultList.add(s);
				}
			}
			break;
		case DIRTY:
			for (Sector s : sectorList) {
				if (s.getHeader().getStatus() < 64) {
					resultList.add(s);
				}
			}
			break;
		case CLEAN:
			for (Sector s : sectorList) {
				if (s.getHeader().getStatus() == 255) {
					resultList.add(s);
				}
			}
			break;
		default:
			break;
		}
		return resultList;
	}

	private void createSectorTable(final Composite parent) {
		tableViewer = new TableViewer(parent,
				SWT.MULTI | SWT.H_SCROLL | SWT.V_SCROLL | SWT.FULL_SELECTION | SWT.BORDER);

		createColumns(parent, tableViewer);
		final Table table = tableViewer.getTable();
		table.setHeaderVisible(true);
		table.setLinesVisible(true);

		tableViewer.setContentProvider(new ArrayContentProvider());
		// Set the content (Model) for the viewer
		tableViewer.setInput(SmartFileSystem.getSectors());

		GridData gridData = new GridData();
		gridData.verticalAlignment = GridData.FILL;
		gridData.horizontalSpan = 3;
		gridData.grabExcessHorizontalSpace = true;
		gridData.grabExcessVerticalSpace = true;
		gridData.horizontalAlignment = GridData.FILL;
		tableViewer.getControl().setLayoutData(gridData);
	}

	private void createColumns(final Composite parent, final TableViewer viewer) {
		String[] titles = new String[] { "Physical Sector #", "Sector #", "Seq. #", "CRC", "Status" };
		int[] bounds = { 128, 72, 60, 60, 60 };

		// First column is for the physical sector number
		TableViewerColumn col = createTableViewerColumn(titles[0], bounds[0], 0);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				Sector s = (Sector)element;
				return s.getHeader().getPhyicalSectorId() + "";
			}
		});

		// Second column is for the sector number
		col = createTableViewerColumn(titles[1], bounds[1], 1);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				Sector s = (Sector)element;
				return s.getHeader().getLogicalSectorId() + "";
			}
		});

		// Third column is for the sequence number
		col = createTableViewerColumn(titles[2], bounds[2], 2);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				Sector s = (Sector)element;
				return s.getHeader().getSequenceNumber() + "";
			}
		});

		// Now the crc8
		col = createTableViewerColumn(titles[3], bounds[3], 3);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				Sector s = (Sector)element;
				return s.getHeader().getCrc8() + "";
			}
		});

		// Now the status
		col = createTableViewerColumn(titles[4], bounds[4], 4);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				Sector s = (Sector)element;
				return s.getHeader().getStatus() + "";
			}
		});
	}

	private TableViewerColumn createTableViewerColumn(String title, int bound, final int colNumber) {
		final TableViewerColumn viewerColumn = new TableViewerColumn(tableViewer, SWT.NONE);
		final TableColumn column = viewerColumn.getColumn();
		column.setText(title);
		column.setWidth(bound);
		column.setResizable(true);
		column.setMoveable(true);

		return viewerColumn;
	}
}
