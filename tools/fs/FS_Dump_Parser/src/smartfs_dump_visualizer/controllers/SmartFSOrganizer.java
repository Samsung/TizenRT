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

package smartfs_dump_visualizer.controllers;

import java.io.File;
import java.io.FileInputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import smartfs_dump_parser.data_model.EntryType;
import smartfs_dump_parser.data_model.Header;
import smartfs_dump_parser.data_model.SmartFileSystem;
import smartfs_dump_parser.data_model.Sector;
import smartfs_dump_parser.data_model.SmartFile;

public class SmartFSOrganizer {

	private static int extractedSectorSize = 0;

	public static boolean organizeSmartFS(String filePath, String fileName) {
		SmartFile root = SmartFileSystem.getRootDirectory();
		if (root != null) {
			SmartFileSystem.clearFileSystem();
		}

		int candidateSectorSize = SmartFileSystem.getMinSectorSize();
		for (; candidateSectorSize <= SmartFileSystem.getMaxSectorSize(); candidateSectorSize <<= 1) {
			SmartFileSystem.clearFileSystem();

			if (analyzeDumpFile(filePath, fileName, candidateSectorSize)) {
				SmartFileSystem.setSectorSize(candidateSectorSize);
				System.out.println("SmartFS with sector size " + candidateSectorSize + " is found.\n");
				break;
			} else {
				System.out.println("SmartFS with sector size " + candidateSectorSize + " is not found.\n");
			}
		}

		if (!createDirectoryHierarchy()) {
			return false;
		}

		// Analyze Journaling Info.
		int journal_area = existJournalingArea();
		if (journal_area > 0) {

		}
		return true;
	}

	private static boolean analyzeDumpFile(String filePath, String fileName, int candidateSectorSize) {
		byte[] buffer = new byte[candidateSectorSize];
		try {
			FileInputStream fis = new FileInputStream(new File(filePath, fileName));
			int sectorId = 0;
			while (fis.read(buffer) > 0) {
				SmartFSOrganizer.addSector(sectorId, buffer);
				sectorId++;
				buffer = new byte[candidateSectorSize];
			}
			fis.close();
			SmartFileSystem.setNumberOfSectors(sectorId);
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("[ERROR] Parsing failed with the opened file..\nPlease check it.");
			SmartFileSystem.clearFileSystem();

			return false;
		}

		return (SmartFileSystem.isValidSmartFS() && (SmartFileSystem.getRootDirectory() != null)
				&& (candidateSectorSize == extractedSectorSize));
	}

	private static void addSector(int physicalSectorNum, byte[] sectorData) {
		Sector sector = new Sector(sectorData);
		int logicalSectorId = makePositiveValue(sectorData[0]) + (makePositiveValue(sectorData[1]) << 8);
		int status = makePositiveValue(sectorData[4]);
		Header header = new Header(physicalSectorNum, logicalSectorId, makePositiveValue(sectorData[2]),
				makePositiveValue(sectorData[3]), status);
		sector.setHeader(header);
		SmartFileSystem.getSectors().add(sector);

		byte statusValue = sectorData[4];
		if (SmartFileSystem.isActiveSector(statusValue)) {
			SmartFileSystem.getActiveSectorsMap().put(logicalSectorId, sector);

			if (SmartFileSystem.isSignatureLogicalSector(logicalSectorId)) {
				SmartFileSystem.setValidSmartFS(true);
				extractedSectorSize = (statusValue & SmartFileSystem.getSectorStatusSizebits());
				extractedSectorSize <<= 7;
			} else if (SmartFileSystem.isRootLogicalSector(logicalSectorId)) {
				Map<Integer, Sector> sectorMap = new HashMap<Integer, Sector>();
				sectorMap.put(logicalSectorId, sector);
				SmartFile rootDir = new SmartFile(SmartFileSystem.getSmartFSRoot(), EntryType.DIRECTORY, null,
						sectorMap);
				SmartFileSystem.setRootDirectory(rootDir);
			}
		} else if (SmartFileSystem.isDirtySector(statusValue)) {
			SmartFileSystem.getDirtySectorsMap().put(logicalSectorId, sector);
		} else {
			// TODO: Validate whether this sector is clean or corrupted.
			SmartFileSystem.getCleanSectorsMap().put(logicalSectorId, sector);
		}
	}

	private static int makePositiveValue(byte value) {
		return (value + 256) % 256;
	}

	public static String printSector(byte[] buffer, int sectorId) {
		StringBuffer stbuf = new StringBuffer();
		stbuf.append("Sector " + sectorId + "\n");
		for (int i = 0; i < SmartFileSystem.getSectorSize(); i++) {
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

	public static boolean createDirectoryHierarchy() {
		List<SmartFile> queue = new ArrayList<SmartFile>();
		SmartFile root = SmartFileSystem.getRootDirectory();
		if (root == null) {
			System.out.println("Root is null");
			return false;
		}
		SmartFileSystem.getTopDummyDirectory().getEntries().add(root);
		queue.add(root);

		while (!queue.isEmpty()) {
			SmartFile sf = queue.remove(0);
			if (analyzeChildrenEntries(sf) > 0) {
				queue.addAll(sf.getEntries());
			}
		}
		System.out.println("Directory hierarchy is organized.");

		return true;
	}

	private static int analyzeChildrenEntries(SmartFile sf) {
		if (sf.getSectorMap().size() == 0) {
			System.out.println("Warning: No sectors are specified for this entry: " + sf.getFileName());
			return -1;
		}

		List<Sector> sectorList = new ArrayList<Sector>(sf.getSectorMap().values());
		Sector currSector = sectorList.get(0);
		byte[] flashData = currSector.getFlashData();
		int offset = SmartFileSystem.getSectorHeaderSize();

		// Get the information of 'struct smartfs_chain_header_s'
		int type = makePositiveValue(flashData[offset]);
		if (type == 0x0) {
			System.out.println("File " + sf.getFileName() + " has no children..");
			return sf.getEntries().size();
		}

		// Get the information of 'struct smartfs_entry_header_s'
		offset += SmartFileSystem.getChainHeaderSize();
		while ((offset + SmartFileSystem.getEntryHeaderSize() < SmartFileSystem.getSectorSize())
				&& (makePositiveValue(flashData[offset]) != 255)) {
			boolean isDirectory = true;
			if ((flashData[offset] & 0x20) == 0x0) {
				isDirectory = false;
			}
			offset += SmartFileSystem.getEntryHeaderSize();

			int fileNameLength = SmartFileSystem.getFileNameLength();
			char[] fileNameArray = new char[fileNameLength];
			for (int i = 0; i < fileNameLength; i++) {
				int character = makePositiveValue(flashData[offset + i]);
				if (character > 0) {
					fileNameArray[i] = (char) ('a' + character - 97);
				} else {
					fileNameArray[i] = '\0';
				}
			}
			// TODO: Fill sectorList appropriately
			SmartFile child = new SmartFile(new String(fileNameArray).trim(),
					(isDirectory ? EntryType.DIRECTORY : EntryType.FILE), sf, new HashMap<Integer, Sector>());
			sf.getEntries().add(child);
			offset += fileNameLength;
		}

		return sf.getEntries().size();
	}

	private static int existJournalingArea() {
		Sector area1_start_sector = SmartFileSystem.getActiveSectorsMap()
				.get(SmartFileSystem.getJournalAreaStartSectorId());
		Sector area2_start_sector = SmartFileSystem.getActiveSectorsMap()
				.get(SmartFileSystem.getJournalAreaStartSectorId() + SmartFileSystem.getJournalSectorNumber());

		int[] area_status = new int[2];
		area_status[0] = makePositiveValue(area1_start_sector.getFlashData()[SmartFileSystem.getSectorHeaderSize()]);
		area_status[1] = makePositiveValue(area2_start_sector.getFlashData()[SmartFileSystem.getSectorHeaderSize()]);
		if (area1_start_sector == null && area2_start_sector == null) {
			System.out.println("Journaling area does not exist..T.T)\n");
			return -1;
		} else if (area1_start_sector == null && area_status[1] % 16 == 0) {
			return 1;
		} else if (area2_start_sector == null && area_status[0] % 16 == 0) {
			return 0;
		}

		int[] area_index = new int[2];
		for (int i = 0; i <= 1; i++) {
			if (area_status[i] == 255) {
				area_index[i] = 0;
			} else if (area_status[i] == 15 * 16) {
				area_index[i] = 1;
			} else if (area_status[i] == 0) {
				area_index[i] = 2;
			} else {
				area_index[i] = 3;
			}
		}

		return SmartFileSystem.getJournalArea(area_index[0], area_index[1]);
	}
}
