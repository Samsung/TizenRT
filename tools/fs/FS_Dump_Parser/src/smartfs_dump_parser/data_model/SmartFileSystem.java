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

package smartfs_dump_parser.data_model;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

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
	private static final int JOURNAL_AREA_START_SECTOR_ID = 12;
	private static final int JOURNAL_SECTOR_NUMBER = (8 + 1) * 2;
	private static final int MIN_SECTOR_SIZE = 256; // bytes
	private static final int MAX_SECTOR_SIZE = 65536; // In TizenRT, 4 bytes are used for sector IDs in SmartFS.
	private static final int[][] JOURNAL_AREA_MAP = { { -1, 1, 1, -1 }, { 0, 0, 0, 0 }, { 0, 1, 0, 0 },
			{ -1, 1, 1, -1 } };

	private static final String SMARTFS_ROOT = "/mnt";
	private static final SmartFile dummy = new SmartFile("/", EntryType.DIRECTORY, null,
			new HashMap<Integer, Sector>());

	private static boolean isValidSmartFS = false;
	private static int mSectorSize = MIN_SECTOR_SIZE;
	private static int mNumberOfSectors = 0;
	private static List<Sector> mSectorList = new ArrayList<Sector>();
	private static Map<Integer, Sector> mActiveSectorsMap = new HashMap<Integer, Sector>();
	private static Map<Integer, Sector> mDirtySectorsMap = new HashMap<Integer, Sector>();
	private static Map<Integer, Sector> mCleanSectorsMap = new HashMap<Integer, Sector>();
	private static List<JournalingInformation> mJournalingInformation = new ArrayList<JournalingInformation>();
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

	public static int getJournalAreaStartSectorId() {
		return JOURNAL_AREA_START_SECTOR_ID;
	}

	public static int getJournalSectorNumber() {
		return JOURNAL_SECTOR_NUMBER;
	}

	public static int getMinSectorSize() {
		return MIN_SECTOR_SIZE;
	}

	public static int getMaxSectorSize() {
		return MAX_SECTOR_SIZE;
	}

	public static int getJournalArea(int area_index_row, int area_index_col) {
		return JOURNAL_AREA_MAP[area_index_row][area_index_col];
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

	public static Map<Integer, Sector> getActiveSectorsMap() {
		return mActiveSectorsMap;
	}

	public static void setActiveSectorsMap(Map<Integer, Sector> sectorsMap) {
		mActiveSectorsMap = sectorsMap;
	}

	public static Map<Integer, Sector> getDirtySectorsMap() {
		return mDirtySectorsMap;
	}

	public static void setDirtySectorsMap(Map<Integer, Sector> sectorsMap) {
		mDirtySectorsMap = sectorsMap;
	}

	public static Map<Integer, Sector> getCleanSectorsMap() {
		return mCleanSectorsMap;
	}

	public static void setCleanSectorsMap(Map<Integer, Sector> sectorsMap) {
		mCleanSectorsMap = sectorsMap;
	}

	public static List<JournalingInformation> getJournalingInformation() {
		return mJournalingInformation;
	}

	public static void clearJouranlingInformation() {
		mJournalingInformation.clear();
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
		mActiveSectorsMap.clear();
		mDirtySectorsMap.clear();
		mCleanSectorsMap.clear();
		mRootDir = null;
		dummy.getEntries().clear();
	}
}
