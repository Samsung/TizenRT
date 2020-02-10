////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Samsung Electronics All Rights Reserved.
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

import javax.annotation.PostConstruct;

import org.eclipse.e4.ui.di.Focus;

import org.eclipse.jface.viewers.ArrayContentProvider;
import org.eclipse.jface.viewers.ColumnLabelProvider;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.jface.viewers.TableViewerColumn;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;

import smartfs_dump_parser.data_model.JournalTransactionType;
import smartfs_dump_parser.data_model.JournalingInformation;
import smartfs_dump_parser.data_model.Sector;
import smartfs_dump_parser.data_model.SmartFileSystem;

public class JournalViewer {

	private TableViewer tableViewer;

	@PostConstruct
	public void createComposite(Composite parent) {
		parent.setLayout(new GridLayout(3, false));

		createSectorTable(parent);
	}

	@Focus
	public void setFocus() {
		tableViewer.getTable().setFocus();
	}

	public TableViewer getJournalViewer() {
		return tableViewer;
	}

	public static List<Sector> filterSectors(JournalTransactionType targetStatus, List<Sector> sectorList) {
		ArrayList<Sector> resultList = new ArrayList<Sector>();
		switch (targetStatus) {
		case WRITE:
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
		tableViewer.setInput(SmartFileSystem.getJournalingInformation());

		GridData gridData = new GridData();
		gridData.verticalAlignment = GridData.FILL;
		gridData.horizontalSpan = 3;
		gridData.grabExcessHorizontalSpace = true;
		gridData.grabExcessVerticalSpace = true;
		gridData.horizontalAlignment = GridData.FILL;
		tableViewer.getControl().setLayoutData(gridData);
	}

	private void createColumns(final Composite parent, final TableViewer viewer) {
		String[] titles = new String[] { "Logical Sector #", "Physical Sector #", "Type", "Started", "Finished",
				"Target Sector", "Offset", "Data Size", "Argument" };
		int[] bounds = { 120, 124, 70, 70, 72, 100, 56, 72, 80 };

		// The 1st column displays logical sector numbers.
		TableViewerColumn col = createTableViewerColumn(titles[0], bounds[0], 0);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				JournalingInformation j = (JournalingInformation)element;
				return j.getSector().getHeader().getLogicalSectorId() + "";
			}
		});

		// The 2nd column displays physical sector numbers.
		col = createTableViewerColumn(titles[1], bounds[1], 1);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				JournalingInformation j = (JournalingInformation)element;
				return j.getSector().getHeader().getPhyicalSectorId() + "";
			}
		});

		// The 3rd column displays journal transaction types.
		col = createTableViewerColumn(titles[2], bounds[2], 2);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				JournalingInformation j = (JournalingInformation)element;
				return j.getType() + "";
			}
		});

		// The 4th column displays whether this journal transaction is started.
		col = createTableViewerColumn(titles[3], bounds[3], 3);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				JournalingInformation j = (JournalingInformation)element;
				return j.isStarted() ? "true" : "false";
			}
		});

		// The 5th column displays whether this journal transaction is finished.
		col = createTableViewerColumn(titles[4], bounds[4], 4);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				JournalingInformation j = (JournalingInformation)element;
				return j.isFinished() ? "true" : "false";
			}
		});

		// The 6th column displays target sectors.
		col = createTableViewerColumn(titles[5], bounds[5], 5);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				JournalingInformation j = (JournalingInformation)element;
				return j.getTargetSectorId() + "";
			}
		});

		// The 7th column displays offsets.
		col = createTableViewerColumn(titles[6], bounds[6], 6);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				JournalingInformation j = (JournalingInformation)element;
				return j.getOffset() + "";
			}
		});

		// The 8th column displays journal data sizes.
		col = createTableViewerColumn(titles[7], bounds[7], 7);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				JournalingInformation j = (JournalingInformation)element;
				return j.getDataSize() + "";
			}
		});

		// The 9th column displays journal arguments.
		col = createTableViewerColumn(titles[8], bounds[8], 8);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				JournalingInformation j = (JournalingInformation)element;
				return j.getArgument() + "";
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
