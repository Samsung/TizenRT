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

			String params[] = { "python3", file.getParent() + "/" + parsingScriptName, initialFile.getCanonicalPath(),
					parsedFileName };
			Runtime runtime = Runtime.getRuntime();
			runtime.exec(params);

			Thread.sleep(3000);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
