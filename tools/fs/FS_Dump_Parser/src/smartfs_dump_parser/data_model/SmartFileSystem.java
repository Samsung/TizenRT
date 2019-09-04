package smartfs_dump_parser.data_model;

import java.util.ArrayList;
import java.util.List;

public class SmartFileSystem {

	private static final int SECTOR_SIZE = 512;
	private static final int SECTOR_HEADER_SIZE = 5;
	private static final int CHAIN_HEADER_SIZE = 5;
	private static final int ENTRY_HEADER_SIZE = 8;
	private static final int FILE_NAME_LENGTH = 32;
	private static final int SECTOR_STATUS_NOT_COMMITTED = 0x80;
	private static final int SECTOR_STATUS_NOT_RELEASED = 0x40;
	private static final int ROOT_LOGICAL_SECTOR_ID = 3;
	private static final String SMARTFS_ROOT = "SMARTFS ROOT";
	private static final SmartFile dummy = new SmartFile("/", EntryType.DIRECTORY, null, new ArrayList<Sector>());

	private static List<Sector> sectorList = new ArrayList<Sector>();
	private static List<Sector> activeSectors = new ArrayList<Sector>();
	private static List<Sector> dirtySectors = new ArrayList<Sector>();
	private static List<Sector> cleanSectors = new ArrayList<Sector>();
	private static SmartFile rootDir;

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

		byte statusValue = sectorData[4];
		if ((statusValue & SECTOR_STATUS_NOT_COMMITTED) == 0x0 && (statusValue & SECTOR_STATUS_NOT_RELEASED) != 0x0) {
			SmartFileSystem.getActiveSectors().add(sector);
			if (logicalSectorId == ROOT_LOGICAL_SECTOR_ID) {
				List<Sector> sectorList = new ArrayList<Sector>();
				sectorList.add(sector);
				rootDir = new SmartFile(SMARTFS_ROOT, EntryType.DIRECTORY, null, sectorList);
			}
		} else if ((statusValue & SECTOR_STATUS_NOT_COMMITTED) == 0x0
				&& (statusValue & SECTOR_STATUS_NOT_RELEASED) == 0x0) {
			SmartFileSystem.getDirtySectors().add(sector);
		} else {
			SmartFileSystem.getCleanSectors().add(sector);
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
