package smartfs_dump_parser.handlers;

import javax.inject.Inject;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.e4.ui.model.application.ui.basic.MPart;
import org.eclipse.e4.ui.workbench.modeling.EPartService;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Shell;

import smartfs_dump_parser.data_model.SmartFileSystem;
import smartfs_dump_parser.parts.DumpViewer;
import smartfs_dump_visualizer.controllers.SmartFSOrganizer;

public class OpenHandler {

	@Inject
	EPartService partService;

	@Execute
	public void execute(Shell shell) {
		FileDialog dialog = new FileDialog(shell);
		if (dialog.open() != null) {
			if (SmartFileSystem.isValidSmartFS()) {
				MessageDialog.openInformation(shell, "Information",
						"A dump file " + " is already opened.");
				return;
			}
			String filePath = dialog.getFilterPath();
			String fileName = dialog.getFileName();
			System.out.println("filename = " + fileName);

			MPart mpart = partService.findPart("SmartFS_Dump_Parser.part.sample");
			DumpViewer dv = (DumpViewer) mpart.getObject();

			if (SmartFSOrganizer.organizeSmartFS(filePath, fileName)) {
				dv.getTableViewer().refresh();
				dv.getDirectoryViewer().refresh();
			}
		}
	}
}
