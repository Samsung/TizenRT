package smartfs_dump_parser.handlers;

import java.io.File;

public class DumpFileFilter implements Runnable {

	private final String parsingScriptName = "Dump_File_fromSDV_Filter.py";

	private String parsedFileName;
	private File initialFile;

	public DumpFileFilter(File initialFile, String parsedFileName) {
		this.initialFile = initialFile;
		this.parsedFileName = parsedFileName;
	}

	@Override
	public void run() {
		long fileSize = initialFile.length();
		try {
			while (true) {
				long newFileSize = initialFile.length();
				if (newFileSize - fileSize >= 255000) {
					System.out.println("Dump file size = " + newFileSize + " bytes");

					transformToBinary(initialFile);

					break;
				}
				Thread.sleep(1000);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void transformToBinary(File file) {
		try {
			File parsedFile = new File(parsedFileName);
			long parsedFileSize = parsedFile.length();
			System.out.println("Parsed file size = " + parsedFileSize);

			String params[] = { "python", file.getParent() + "/" + parsingScriptName, initialFile.getCanonicalPath(),
					parsedFileName };
			Runtime runtime = Runtime.getRuntime();
			runtime.exec(params);

			Thread.sleep(3000);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
