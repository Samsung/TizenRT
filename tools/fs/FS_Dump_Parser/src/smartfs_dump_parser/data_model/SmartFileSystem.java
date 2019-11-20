package smartfs_dump_parser.data_model;

import java.util.ArrayList;
import java.util.List;

public class SmartFileSystem {

	private static final int SECTOR_SIZE = 1024;
	private static final int SECTOR_HEADER_SIZE = 5;
	private static final int CHAIN_HEADER_SIZE = 5;
	private static final int ENTRY_HEADER_SIZE = 8;
	private static final int FILE_NAME_LENGTH = 32;
	private static final int SECTOR_STATUS_NOT_COMMITTED = 0x80;
	private static final int SECTOR_STATUS_NOT_RELEASED = 0x40;
	private static final int SIGNATURE_LOGICAL_SECTOR_ID = 0;
	private static final int ROOT_LOGICAL_SECTOR_ID = 3;
	private static final String SMARTFS_ROOT = "SMARTFS ROOT";
	private static final SmartFile dummy = new SmartFile("/", EntryType.DIRECTORY, null, new ArrayList<Sector>());

	private static boolean isValidSmartFS = false;
	private static int numberOfSectors = 0;	
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

	public static String getSmartFSRoot() {
		return SMARTFS_ROOT;
	}

	public static SmartFile getTopDummyDirectory() {
		return dummy;
	}

	public static boolean isValidSmartFS() {
		return isValidSmartFS;
	}

	public static void setValidSmartFS(boolean isValid) {
		isValidSmartFS = isValid;
	}
	
	public static int getNumberOfSectors() {
		return numberOfSectors;
	}

	public static void setNumberOfSectors(int sectorNum) {
		numberOfSectors = sectorNum;
	}

	public static List<Sector> getSectors() {
		return sectorList;
	}

	public static void clearSectors() {
		sectorList.clear();
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

	public static SmartFile getRootDirectory() {
		return rootDir;
	}

	public static void setRootDirectory(SmartFile root) {
		rootDir = root;
	}

	public static int getNumberOfSectors(int smartfsSize) {
		return smartfsSize / SECTOR_SIZE;
	}
	public static boolean isActiveSector(byte status) {
		return ((status & SECTOR_STATUS_NOT_COMMITTED) == 0x0 && (status & SECTOR_STATUS_NOT_RELEASED) != 0x0);
	}

	public static boolean isDirtySector(byte status) {
		return ((status & SECTOR_STATUS_NOT_COMMITTED) == 0x0 && (status & SECTOR_STATUS_NOT_RELEASED) == 0x0);
	}

	public static boolean isSignatureLogicalSector(int logicalSectorId) {
		return (logicalSectorId == SIGNATURE_LOGICAL_SECTOR_ID);
	}

	public static boolean isRootLogicalSector(int logicalSectorId) {
		return (logicalSectorId == ROOT_LOGICAL_SECTOR_ID);
	}

	public static void clearFileSystem() {
		isValidSmartFS = false;
		sectorList.clear();
		activeSectors.clear();
		dirtySectors.clear();
		cleanSectors.clear();
		rootDir = null;
		dummy.getEntries().clear();
	}
}
