package smartfs_dump_parser.parts;

import javax.annotation.PostConstruct;

import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.LabelProvider;
import org.eclipse.jface.viewers.TreeViewer;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.widgets.Composite;

import smartfs_dump_parser.data_model.SmartFileSystem;
import smartfs_dump_parser.data_model.SmartFile;

public class DirectoryViewer {

	private TreeViewer treeViewer;

	@PostConstruct
	public void createComposite(Composite parent) {
		createDirectoryTreeViewer(parent);
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
		gridData.horizontalSpan = 2;
		gridData.grabExcessHorizontalSpace = true;
		gridData.horizontalAlignment = GridData.FILL;
		treeViewer.getControl().setLayoutData(gridData);
	}
}
