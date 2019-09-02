package smartfs_dump_parser.data_model;

import java.util.ArrayList;
import java.util.List;

public class SmartFileSystem {
	
	private static final int SECTOR_SIZE = 512;
	private static final int SECTOR_HEADER_SIZE = 5;
	private static final int CHAIN_HEADER_SIZE = 5;
	private static final int ENTRY_HEADER_SIZE = 8;
	private static final int FILE_NAME_LENGTH = 32;
	private static final int ACTIVE_SECTOR_STATUS_VALUE = 101;
	private static final int ROOT_LOGICAL_SECTOR_ID = 3;
	private static final String SMARTFS_ROOT = "SMARTFS ROOT";

	private static List<Sector> sectorList = new ArrayList<Sector>();
	private static List<Sector> activeSectors;
	private static List<Sector> dirtySectors;
	private static List<Sector> cleanSectors;
	private static SmartFile rootDir;

	private static SmartFile dummy = new SmartFile("/", EntryType.DIRECTORY, null, new ArrayList<Sector>());

	// Singleton Design Pattern
	private SmartFileSystem() {
		
	}
	
	public static int getSectorSize() {
		return SECTOR_SIZE;
	}

	public static int getSectorHeaderSize() {
		return SECTOR_HEADER_SIZE;
	}

	public static int getChainHeaderSize() {
		return CHAIN_HEADER_SIZE;
	}

	public static int getEntryHeaderSize() {
		return ENTRY_HEADER_SIZE;
	}

	public static int getFileNameLength() {
		return FILE_NAME_LENGTH;
	}

	public static SmartFile getRootDir() {
		return rootDir;
	}

	public static List<Sector> getSectors() {
		return sectorList;
	}

	public static void addSector(int physicalSectorNum, byte[] sectorData) {
		Sector sector = new Sector(sectorData);
		int logicalSectorId = makePositiveValue(sectorData[0]) + (makePositiveValue(sectorData[1]) << 8);
		int status = makePositiveValue(sectorData[4]);
		Header header = new Header(physicalSectorNum, logicalSectorId, makePositiveValue(sectorData[2]),
				makePositiveValue(sectorData[3]), status);
		sector.setHeader(header);
		sectorList.add(sector);

		if (logicalSectorId == ROOT_LOGICAL_SECTOR_ID && status == ACTIVE_SECTOR_STATUS_VALUE) {
			List<Sector> sectorList = new ArrayList<Sector>();
			sectorList.add(sector);
			rootDir = new SmartFile(SMARTFS_ROOT, EntryType.DIRECTORY, null, sectorList);
		}
	}

	public static int makePositiveValue(byte value) {
		return (value + 256) % 256;
	}

	public static List<Sector> getActiveSectors() {
		return activeSectors;
	}

	public static void setActiveSectors(List<Sector> sectors) {
		activeSectors = sectors;
	}

	public static List<Sector> getDirtySectors() {
		return dirtySectors;
	}

	public static void setDirtySectors(List<Sector> sectors) {
		dirtySectors = sectors;
	}

	public static List<Sector> getCleanSectors() {
		return cleanSectors;
	}

	public static void setCleanSectors(List<Sector> sectors) {
		cleanSectors = sectors;
	}

	public static SmartFile getTopDummyDirectory() {
		return dummy;
	}
}
