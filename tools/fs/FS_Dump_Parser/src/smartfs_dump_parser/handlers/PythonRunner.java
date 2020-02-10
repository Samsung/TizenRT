////////////////////////////////////////////////////////////////////
//
// Copyright 2019 Samsung Electronics All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied. See the License for the specific
// language governing permissions and limitations under the License.
//
////////////////////////////////////////////////////////////////////

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
//			String params[] = { "python3", pythonScriptPath + pythonScriptName, "/dev/ttyACM0" };	// NXP
			String params[] = { "python3", pythonScriptPath + pythonScriptName, "/dev/ttyUSB1" };	// ARTIK053
			ProcessBuilder builder = new ProcessBuilder(params);
//			builder.redirectOutput(Redirect.INHERIT);		// Print out to console.
			builder.redirectOutput(outputFile);
			builder.start();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
