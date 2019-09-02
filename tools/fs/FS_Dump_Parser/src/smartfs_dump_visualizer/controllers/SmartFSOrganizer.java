package smartfs_dump_visualizer.controllers;

import java.util.ArrayList;
import java.util.List;

import smartfs_dump_parser.data_model.EntryType;
import smartfs_dump_parser.data_model.SmartFileSystem;
import smartfs_dump_parser.data_model.Sector;
import smartfs_dump_parser.data_model.SectorStatus;
import smartfs_dump_parser.data_model.SmartFile;

public class SmartFSOrganizer {

	public static void createDirectoryHierarchy() {
		List<SmartFile> queue = new ArrayList<SmartFile>();
		SmartFile root = SmartFileSystem.getRootDir();
		if (root == null) {
			System.out.println("Root is null");
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
	}

	private static int analyzeChildrenEntries(SmartFile sf) {
		if (sf.getSectorList().size() == 0) {
			System.out.println("Warning: No sectors are specified for this entry: " + sf.getFileName());
			return -1;
		}

		Sector currSector = sf.getSectorList().get(0);
		byte[] flashData = currSector.getFlashData();
		int offset = SmartFileSystem.getSectorHeaderSize();

		// Get the information of 'struct smartfs_chain_header_s'
		int type = SmartFileSystem.makePositiveValue(flashData[offset]);
		if (type == 0x0) {
			System.out.println("File " + sf.getFileName() + " has no children..");
			return sf.getEntries().size();
		}

		// Get the information of 'struct smartfs_entry_header_s'
		offset += SmartFileSystem.getChainHeaderSize();
		while ((offset + SmartFileSystem.getEntryHeaderSize() < SmartFileSystem.getSectorSize())
				&& (SmartFileSystem.makePositiveValue(flashData[offset]) != 255)) {
			boolean isDirectory = true;
			if ((flashData[offset] & 0x20) == 0x0) {
				isDirectory = false;
			}
			offset += SmartFileSystem.getEntryHeaderSize();

			char[] fileNameArray = new char[SmartFileSystem.getFileNameLength()];
			for (int i = 0; i < SmartFileSystem.getFileNameLength(); i++) {
				int character = SmartFileSystem.makePositiveValue(flashData[offset + i]);
				if (character > 0) {
					fileNameArray[i] = (char) ('a' + character - 97);
				} else {
					fileNameArray[i] = '\0';
				}
			}
			// TODO: Fill sectorList appropriately
			SmartFile child = new SmartFile(new String(fileNameArray).trim(),
					(isDirectory ? EntryType.DIRECTORY : EntryType.FILE), sf, new ArrayList<Sector>());
			sf.getEntries().add(child);
			offset += +SmartFileSystem.getFileNameLength();
		}
		return sf.getEntries().size();
	}

	public static List<Sector> filterSectors(SectorStatus targetStatus, List<Sector> sectorList) {
		ArrayList<Sector> resultList = new ArrayList<Sector>();
		switch (targetStatus) {
		case ACTIVE:
			for (Sector s : sectorList) {
				int status = s.getHeader().getStatus();
				if (status < 128 && status >= 64) {
					resultList.add(s);
				}
			}
			break;
		case DIRTY:
			for (Sector s : sectorList) {
				if (s.getHeader().getStatus() < 64) {
					resultList.add(s);
				}
			}
			break;
		case CLEAN:
			for (Sector s : sectorList) {
				if (s.getHeader().getStatus() == 255) {
					resultList.add(s);
				}
			}
			break;
		default:
			break;
		}
		return resultList;
	}
}
