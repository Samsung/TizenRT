package smartfs_dump_parser.handlers;

import java.io.File;
import java.io.FileInputStream;

import javax.inject.Inject;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.e4.ui.model.application.ui.basic.MPart;
import org.eclipse.e4.ui.workbench.modeling.EPartService;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Shell;

import smartfs_dump_parser.parts.DumpViewer;

public class OpenHandler {

	private static final int BUF_SIZE = 512;
	private static final int SECTOR_HEADER_SIZE = 5;

	@Inject
	EPartService partService;

	@Execute
	public void execute(Shell shell) {
		FileDialog dialog = new FileDialog(shell);
		if (dialog.open() != null) {
			String filePath = dialog.getFilterPath();
			String fileName = dialog.getFileName();
			System.out.println("filename = " + fileName);

			MPart mpart = partService.findPart("SmartFS_Dump_Parser.part.sample");
			DumpViewer dv = (DumpViewer) mpart.getObject();

			byte[] buffer = new byte[BUF_SIZE];
			byte[] headerData = new byte[SECTOR_HEADER_SIZE];
			try {
				FileInputStream fis = new FileInputStream(new File(filePath, fileName));
				int sectorId = 0;
				while (fis.read(buffer) > 0) {
					fis.read(buffer);
					System.arraycopy(buffer, 0, headerData, 0, SECTOR_HEADER_SIZE);
					dv.addSectorHeader(sectorId, headerData);
					sectorId++;
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	public String printSector(byte[] buffer, int sectorId) {
		StringBuffer stbuf = new StringBuffer();
		stbuf.append("Sector " + sectorId + "\n");
		for (int i = 0; i < BUF_SIZE; i++) {
			int value = (buffer[i] + 256) % 256;
			stbuf.append(value);
			for (int j = 0; j < 3 - new String(value + "").length(); j++)
				stbuf.append(" ");
			if ((i + 1) % 16 == 0)
				stbuf.append("\n");
			else
				stbuf.append(" ");
		}
		stbuf.append("\n");

		return stbuf.toString();
	}
}
