package smartfs_dump_parser.handlers;

import javax.inject.Inject;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.e4.ui.model.application.ui.basic.MPart;
import org.eclipse.e4.ui.workbench.modeling.EPartService;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Shell;

import smartfs_dump_parser.parts.GeometryViewer;
import smartfs_dump_parser.data_model.SmartFileSystem;
import smartfs_dump_parser.parts.DirectoryViewer;
import smartfs_dump_parser.parts.SectorViewer;
import smartfs_dump_visualizer.controllers.SmartFSOrganizer;

public class OpenHandler {

	@Inject
	EPartService partService;

	@Execute
	public void execute(Shell shell) {
		FileDialog dialog = new FileDialog(shell);
		if (dialog.open() != null) {
			String filePath = dialog.getFilterPath();
			String fileName = dialog.getFileName();
			System.out.println("filename = " + fileName + "\n");

			MPart geometry_part = partService.findPart("smartfs_dump_parser.part.geometry_information");
			GeometryViewer gv = (GeometryViewer) geometry_part.getObject();
			MPart directory_part = partService.findPart("smartfs_dump_parser.part.directory_structure");
			DirectoryViewer dv = (DirectoryViewer) directory_part.getObject();
			MPart sector_part = partService.findPart("smartfs_dump_parser.part.sector_list");
			SectorViewer sv = (SectorViewer) sector_part.getObject();
			
			if (SmartFSOrganizer.organizeSmartFS(filePath, fileName)) {
				gv.updateGeometry(SmartFileSystem.getSectorSize(), SmartFileSystem.getNumberOfSectors());				
				dv.getDirectoryViewer().refresh();
				sv.getSectorViewer().refresh();
			}
		}
	}
}
