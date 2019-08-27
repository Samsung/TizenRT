package smartfs_dump_parser.parts;

import java.util.List;
import java.util.ArrayList;

import javax.annotation.PostConstruct;
import javax.inject.Inject;

import org.eclipse.e4.ui.di.Focus;
import org.eclipse.e4.ui.di.Persist;
import org.eclipse.e4.ui.model.application.ui.basic.MPart;
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

import smartfs_dump_parser.data_model.Header;
import smartfs_dump_parser.data_model.ModelProvider;

enum SectorStatus {
	ACTIVE, DIRTY, CLEAN
}

public class DumpViewer {

	private TableViewer tableViewer;
	private Button activeButton;
	private Button dirtyButton;
	private Button cleanButton;

	@Inject
	private MPart part;

	@PostConstruct
	public void createComposite(Composite parent) {
		parent.setLayout(new GridLayout(3, false));

		createCheckbox(parent);

		tableViewer = new TableViewer(parent,
				SWT.MULTI | SWT.H_SCROLL | SWT.V_SCROLL | SWT.FULL_SELECTION | SWT.BORDER);

		createColumns(parent, tableViewer);
		final Table table = tableViewer.getTable();
		table.setHeaderVisible(true);
		table.setLinesVisible(true);

		tableViewer.setContentProvider(new ArrayContentProvider());
		// Set the content (Model) for the viewer
		tableViewer.setInput(ModelProvider.INSTANCE.getHeaders());

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

	private void createCheckbox(final Composite parent) {
		activeButton = new Button(parent, SWT.CHECK);
		activeButton.setText("Active Sectors");
		activeButton.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				if (activeButton.getSelection()) {
					if (ModelProvider.INSTANCE.getHeaders().size() == 0) {
						activeButton.setSelection(false);
						return;
					}
					List<Header> activeSectors = ModelProvider.INSTANCE.getActiveSectors();
					if (activeSectors == null) {
						activeSectors = filterSectors(SectorStatus.ACTIVE, ModelProvider.INSTANCE.getHeaders());
						ModelProvider.INSTANCE.setActiveSectors(activeSectors);
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
				if (ModelProvider.INSTANCE.getHeaders().size() == 0) {
					dirtyButton.setSelection(false);
					return;
				}
				List<Header> dirtySectors = ModelProvider.INSTANCE.getDirtySectors();
				if (dirtySectors == null) {
					dirtySectors = filterSectors(SectorStatus.DIRTY, ModelProvider.INSTANCE.getHeaders());
					ModelProvider.INSTANCE.setDirtySectors(dirtySectors);
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
				if (ModelProvider.INSTANCE.getHeaders().size() == 0) {
					cleanButton.setSelection(false);
					return;
				}
				List<Header> cleanSectors = ModelProvider.INSTANCE.getCleanSectors();
				if (cleanSectors == null) {
					cleanSectors = filterSectors(SectorStatus.CLEAN, ModelProvider.INSTANCE.getHeaders());
					ModelProvider.INSTANCE.setCleanSectors(cleanSectors);
				}
				tableViewer.setInput(cleanSectors);
				tableViewer.refresh();

				activeButton.setSelection(false);
				dirtyButton.setSelection(false);
			}
		});
	}

	private List<Header> filterSectors(SectorStatus targetStatus, List<Header> headerList) {
		ArrayList<Header> resultList = new ArrayList<Header>();
		switch (targetStatus) {
		case ACTIVE:
			for (Header h : headerList) {
				int status = h.getStatus();
				if (status < 128 && status >= 64) {
					resultList.add(h);
				}
			}
			break;
		case DIRTY:
			for (Header h : headerList) {
				if (h.getStatus() < 64) {
					resultList.add(h);
				}
			}
			break;
		case CLEAN:
			for (Header h : headerList) {
				if (h.getStatus() == 255) {
					resultList.add(h);
				}
			}
			break;
		default:
			break;
		}
		return resultList;
	}

	private void createColumns(final Composite parent, final TableViewer viewer) {
		String[] titles = new String[] { "Physical Sector #", "Sector #", "Seq. #", "CRC", "Status" };
		int[] bounds = { 150, 80, 80, 80, 80 };

		// First column is for the physical sector number
		TableViewerColumn col = createTableViewerColumn(titles[0], bounds[0], 0);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				Header h = (Header) element;
				return h.getPhyicalSectorNum() + "";
			}
		});

		// Second column is for the sector number
		col = createTableViewerColumn(titles[1], bounds[1], 1);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				Header h = (Header) element;
				return h.getSector_num() + "";
			}
		});

		// Third column is for the sequence number
		col = createTableViewerColumn(titles[2], bounds[2], 2);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				Header h = (Header) element;
				return h.getSeq_num() + "";
			}
		});

		// Now the crc8
		col = createTableViewerColumn(titles[3], bounds[3], 3);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				Header h = (Header) element;
				return h.getCrc8() + "";
			}
		});

		// Now the status
		col = createTableViewerColumn(titles[4], bounds[4], 4);
		col.setLabelProvider(new ColumnLabelProvider() {
			@Override
			public String getText(Object element) {
				Header h = (Header) element;
				return h.getStatus() + "";
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

	public void addSectorHeader(int physicalSectorNum, byte[] headerData) {
		Header header = new Header(physicalSectorNum,
				(makePositiveValue(headerData[0]) + makePositiveValue(headerData[1]) * 256),
				makePositiveValue(headerData[2]), makePositiveValue(headerData[3]), makePositiveValue(headerData[4]));
		ModelProvider.INSTANCE.addSectorHeader(header);
		tableViewer.refresh();
	}

	private int makePositiveValue(byte value) {
		return (value + 256) % 256;
	}
}