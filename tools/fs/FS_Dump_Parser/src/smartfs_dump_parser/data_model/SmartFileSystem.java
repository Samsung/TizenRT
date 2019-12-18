package smartfs_dump_parser.data_model;

import java.util.ArrayList;
import java.util.List;

public class SmartFileSystem {

	private static final int SECTOR_HEADER_SIZE = 5;
	private static final int CHAIN_HEADER_SIZE = 5;
	private static final int ENTRY_HEADER_SIZE = 8;
	private static final int FILE_NAME_LENGTH = 32;
	private static final int SECTOR_STATUS_NOT_COMMITTED = 0x80;
	private static final int SECTOR_STATUS_NOT_RELEASED = 0x40;
	private static final int SECTOR_STATUS_SIZEBITS = 0x1C;
	private static final int SIGNATURE_LOGICAL_SECTOR_ID = 0;
	private static final int ROOT_LOGICAL_SECTOR_ID = 3;
	private static final int MIN_SECTOR_SIZE = 256; // bytes
	private static final int MAX_SECTOR_SIZE = 65536; // In TizenRT, 4 bytes are used for sector IDs in SmartFS.
	private static final String SMARTFS_ROOT = "/mnt";
	private static final SmartFile dummy = new SmartFile("/", EntryType.DIRECTORY, null, new ArrayList<Sector>());

	private static boolean isValidSmartFS = false;
	private static int mSectorSize = MIN_SECTOR_SIZE;
	private static int mNumberOfSectors = 0;
	private static List<Sector> mSectorList = new ArrayList<Sector>();
	private static List<Sector> mActiveSectors = new ArrayList<Sector>();
	private static List<Sector> mDirtySectors = new ArrayList<Sector>();
	private static List<Sector> mCleanSectors = new ArrayList<Sector>();
	private static SmartFile mRootDir = null;

	// Singleton Design Pattern
	private SmartFileSystem() {

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

	public static boolean isActiveSector(byte status) {
		return ((status & SECTOR_STATUS_NOT_COMMITTED) == 0x0 && (status & SECTOR_STATUS_NOT_RELEASED) != 0x0);
	}

	public static boolean isDirtySector(byte status) {
		return ((status & SECTOR_STATUS_NOT_COMMITTED) == 0x0 && (status & SECTOR_STATUS_NOT_RELEASED) == 0x0);
	}

	public static int getSectorStatusSizebits() {
		return SECTOR_STATUS_SIZEBITS;
	}

	public static boolean isSignatureLogicalSector(int logicalSectorId) {
		return (logicalSectorId == SIGNATURE_LOGICAL_SECTOR_ID);
	}

	public static boolean isRootLogicalSector(int logicalSectorId) {
		return (logicalSectorId == ROOT_LOGICAL_SECTOR_ID);
	}

	public static int getMinSectorSize() {
		return MIN_SECTOR_SIZE;
	}

	public static int getMaxSectorSize() {
		return MAX_SECTOR_SIZE;
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

	public static int getSectorSize() {
		return mSectorSize;
	}

	public static void setSectorSize(int sector_size) {
		SmartFileSystem.mSectorSize = sector_size;
	}

	public static int getNumberOfSectors() {
		return mNumberOfSectors;
	}

	public static void setNumberOfSectors(int sectorNum) {
		mNumberOfSectors = sectorNum;
	}

	public static List<Sector> getSectors() {
		return mSectorList;
	}

	public static void clearSectors() {
		mSectorList.clear();
	}

	public static List<Sector> getActiveSectors() {
		return mActiveSectors;
	}

	public static void setActiveSectors(List<Sector> sectors) {
		mActiveSectors = sectors;
	}

	public static List<Sector> getDirtySectors() {
		return mDirtySectors;
	}

	public static void setDirtySectors(List<Sector> sectors) {
		mDirtySectors = sectors;
	}

	public static List<Sector> getCleanSectors() {
		return mCleanSectors;
	}

	public static void setCleanSectors(List<Sector> sectors) {
		mCleanSectors = sectors;
	}

	public static SmartFile getRootDirectory() {
		return mRootDir;
	}

	public static void setRootDirectory(SmartFile root) {
		mRootDir = root;
	}

	public static int getNumberOfSectors(int smartfsSize) {
		return smartfsSize / mSectorSize;
	}

	public static void clearFileSystem() {
		isValidSmartFS = false;
		mSectorList.clear();
		mActiveSectors.clear();
		mDirtySectors.clear();
		mCleanSectors.clear();
		mRootDir = null;
		dummy.getEntries().clear();
	}
}
