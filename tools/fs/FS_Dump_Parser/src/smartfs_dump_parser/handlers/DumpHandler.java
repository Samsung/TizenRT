package smartfs_dump_parser.handlers;

import java.io.File;

import javax.inject.Inject;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.e4.ui.model.application.ui.basic.MPart;
import org.eclipse.e4.ui.workbench.modeling.EPartService;
import org.eclipse.swt.widgets.Shell;

import smartfs_dump_parser.data_model.SmartFileSystem;
import smartfs_dump_parser.parts.DirectoryViewer;
import smartfs_dump_parser.parts.GeometryViewer;
import smartfs_dump_parser.parts.SectorViewer;
import smartfs_dump_visualizer.controllers.SmartFSOrganizer;

public class DumpHandler {

	private String initialFileName = "Dump_File.txt";
	private String filteredFileName = initialFileName + "_parsed";
	private String parsedFileName = filteredFileName + "_binary";
	private String srcPath;
	private File outputFile;

	@Inject
	EPartService partService;

	@Execute
	public void execute(Shell shell) {
		try {
			File file = new File(this.getClass().getProtectionDomain().getCodeSource().getLocation().toURI());
			srcPath = file.getCanonicalPath() + "/python/";

			outputFile = new File(srcPath, initialFileName);
			String filteredFilePath = outputFile.getCanonicalPath() + "_parsed";
			String parsedFilePath = filteredFilePath + "_binary";

			Thread pythonRunner = new Thread(new PythonRunner(srcPath, outputFile));
			pythonRunner.start();

			Thread dumpFileFilter = new Thread(new DumpFileFilter(outputFile, parsedFilePath));
			dumpFileFilter.start();

			pythonRunner.join();
			System.out.println("Python script for SmartFS dumping is executed!!\n");

			dumpFileFilter.join();
			System.out.println("Dumped SmartFS file is parsed!!\n");

			MPart geometry_part = partService.findPart("smartfs_dump_parser.part.geometry_information");
			GeometryViewer gv = (GeometryViewer) geometry_part.getObject();
			MPart directory_part = partService.findPart("smartfs_dump_parser.part.directory_structure");
			DirectoryViewer dv = (DirectoryViewer) directory_part.getObject();
			MPart sector_part = partService.findPart("smartfs_dump_parser.part.sector_list");
			SectorViewer sv = (SectorViewer) sector_part.getObject();

			if (SmartFSOrganizer.organizeSmartFS(srcPath, parsedFileName)) {
				gv.updateGeometry(SmartFileSystem.getSectorSize(), SmartFileSystem.getNumberOfSectors());
				dv.getDirectoryViewer().refresh();
				sv.getSectorViewer().refresh();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
