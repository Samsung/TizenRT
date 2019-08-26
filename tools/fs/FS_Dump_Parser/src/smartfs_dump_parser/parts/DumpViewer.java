package smartfs_dump_parser.parts;

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
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;

import smartfs_dump_parser.data_model.Header;
import smartfs_dump_parser.data_model.ModelProvider;

public class DumpViewer {

	private TableViewer tableViewer;

	@Inject
	private MPart part;

	@PostConstruct
	public void createComposite(Composite parent) {
		parent.setLayout(new GridLayout(1, false));

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
		gridData.horizontalSpan = 2;
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