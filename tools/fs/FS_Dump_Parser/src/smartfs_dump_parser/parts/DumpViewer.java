package smartfs_dump_parser.parts;

import java.util.List;

import javax.annotation.PostConstruct;
import javax.inject.Inject;

import org.eclipse.e4.ui.di.Focus;
import org.eclipse.e4.ui.di.Persist;
import org.eclipse.e4.ui.model.application.ui.basic.MPart;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.viewers.ArrayContentProvider;
import org.eclipse.jface.viewers.ColumnLabelProvider;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.LabelProvider;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.jface.viewers.TableViewerColumn;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;

import smartfs_dump_parser.data_model.SmartFileSystem;
import smartfs_dump_parser.data_model.Sector;
import smartfs_dump_parser.data_model.SectorStatus;
import smartfs_dump_parser.data_model.SmartFile;
import smartfs_dump_visualizer.controllers.SmartFSOrganizer;

public class DumpViewer {

	private TreeViewer treeViewer;
	private TableViewer tableViewer;
	private Button activeButton;
	private Button dirtyButton;
	private Button cleanButton;

	@Inject
	private MPart part;

	@PostConstruct
	public void createComposite(Composite parent) {
		parent.setLayout(new GridLayout(3, false));

		createDirectoryTreeViewer(parent);

		createCheckbox(parent);

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

	@Focus
	public void setFocus() {
		tableViewer.getTable().setFocus();
	}

	@Persist
	public void save() {
		part.setDirty(false);
	}

	public TableViewer getTableViewer() {
		return tableViewer;
	}

	public TreeViewer getDirectoryViewer() {
		return treeViewer;
	}

	private void createDirectoryTreeViewer(final Composite parent) {
		treeViewer = new TreeViewer(parent, SWT.MULTI | SWT.H_SCROLL | SWT.V_SCROLL | SWT.FULL_SELECTION | SWT.BORDER);

		treeViewer.setContentProvider(new ITreeContentProvider() {
			@Override
			public Object[] getElements(Object inputElement) {
				return getChildren(inputElement);
			}

			@Override
			public Object[] getChildren(Object parentElement) {
				return ((SmartFile) parentElement).getEntries().toArray();
			}

			@Override
			public Object getParent(Object element) {
				return ((SmartFile) element).getParent();
			}

			@Override
			public boolean hasChildren(Object element) {
				return !(((SmartFile) element).getEntries().isEmpty());
			}
		});

		treeViewer.setLabelProvider(new LabelProvider() {
			@Override
			public String getText(Object element) {
				return ((SmartFile) element).getFileName();
			}
		});

		treeViewer.setInput(SmartFileSystem.getTopDummyDirectory());

		GridData gridData = new GridData();
		gridData.verticalAlignment = GridData.FILL;
		gridData.horizontalSpan = 3;
		gridData.grabExcessHorizontalSpace = true;
		gridData.horizontalAlignment = GridData.FILL;
		treeViewer.getControl().setLayoutData(gridData);
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
					List<Sector> activeSectors = SmartFileSystem.getActiveSectors();
					if (activeSectors == null) {
						activeSectors = SmartFSOrganizer.filterSectors(SectorStatus.ACTIVE,
								SmartFileSystem.getSectors());
						SmartFileSystem.setActiveSectors(activeSectors);
					}
					tableViewer.setInput(activeSectors);
					tableViewer.refresh();

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
				List<Sector> dirtySectors = SmartFileSystem.getDirtySectors();
				if (dirtySectors == null) {
					dirtySectors = SmartFSOrganizer.filterSectors(SectorStatus.DIRTY,
							SmartFileSystem.getSectors());
					SmartFileSystem.setDirtySectors(dirtySectors);
				}
				tableViewer.setInput(dirtySectors);
				tableViewer.refresh();

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
				List<Sector> cleanSectors = SmartFileSystem.getCleanSectors();
				if (cleanSectors == null) {
					cleanSectors = SmartFSOrganizer.filterSectors(SectorStatus.CLEAN,
							SmartFileSystem.getSectors());
					SmartFileSystem.setCleanSectors(cleanSectors);
				}
				tableViewer.setInput(cleanSectors);
				tableViewer.refresh();

				activeButton.setSelection(false);
				dirtyButton.setSelection(false);
			}
		});
	}

	private void createColumns(final Composite parent, final TableViewer viewer) {
		String[] titles = new String[] { "Physical Sector #", "Sector #", "Seq. #", "CRC", "Status" };
		int[] bounds = { 150, 80, 80, 80, 80 };

		// First column is for the physical sector number
		TableViewerColumn col = createTableViewerColumn(titles[0], bounds[0], 0);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				Sector s = (Sector) element;
				return s.getHeader().getPhyicalSectorId() + "";
			}
		});

		// Second column is for the sector number
		col = createTableViewerColumn(titles[1], bounds[1], 1);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				Sector s = (Sector) element;
				return s.getHeader().getLogicalSectorId() + "";
			}
		});

		// Third column is for the sequence number
		col = createTableViewerColumn(titles[2], bounds[2], 2);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				Sector s = (Sector) element;
				return s.getHeader().getSequenceNumber() + "";
			}
		});

		// Now the crc8
		col = createTableViewerColumn(titles[3], bounds[3], 3);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				Sector s = (Sector) element;
				return s.getHeader().getCrc8() + "";
			}
		});

		// Now the status
		col = createTableViewerColumn(titles[4], bounds[4], 4);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				Sector s = (Sector) element;
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
