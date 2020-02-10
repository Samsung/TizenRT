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

import javax.inject.Inject;

import org.eclipse.e4.core.di.annotations.Execute;
import org.eclipse.e4.ui.model.application.ui.basic.MPart;
import org.eclipse.e4.ui.workbench.modeling.EPartService;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Shell;

import smartfs_dump_parser.parts.GeometryViewer;
import smartfs_dump_parser.parts.JournalViewer;
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
			GeometryViewer gv = (GeometryViewer)geometry_part.getObject();
			MPart directory_part = partService.findPart("smartfs_dump_parser.part.directory_structure");
			DirectoryViewer dv = (DirectoryViewer)directory_part.getObject();
			MPart sector_part = partService.findPart("smartfs_dump_parser.part.sector_list");
			SectorViewer sv = (SectorViewer)sector_part.getObject();
			MPart journal_part = partService.findPart("smartfs_dump_parser.part.journal_information");
			JournalViewer jv = (JournalViewer)journal_part.getObject();

			if (SmartFSOrganizer.organizeSmartFS(filePath, fileName)) {
				gv.updateGeometry(SmartFileSystem.getSectorSize(), SmartFileSystem.getNumberOfSectors());
				dv.getDirectoryViewer().refresh();
				sv.getSectorViewer().refresh();
				jv.getJournalViewer().refresh();
			}
		}
	}
}
