package smartfs_dump_parser.handlers;

import java.io.File;

public class PythonRunner implements Runnable {

	private final String pythonScriptName = "dump_board_log.py";
	private String pythonScriptPath;
	private File outputFile;

	public PythonRunner(String sourcePath, File outputFile) {
		pythonScriptPath = sourcePath; // = SmartFS_Dump_Parser/python/
		this.outputFile = outputFile;
	}

	public void run() {
		try {
			String params[] = { "python", pythonScriptPath + pythonScriptName, "/dev/ttyACM0" };	// NXP
//			String params[] = { "python", pythonScriptPath + pythonScriptName, "/dev/ttyUSB1" };	// ARTIK053
			ProcessBuilder builder = new ProcessBuilder(params);
//			builder.redirectOutput(Redirect.INHERIT);		// Print out to console.
			builder.redirectOutput(outputFile);
			builder.start();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
