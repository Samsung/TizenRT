package smartfs_dump_parser.data_model;

import java.util.ArrayList;
import java.util.List;

public class SmartFile {
	private String fileName;
	private EntryType type;
	private SmartFile parent;
	private List<SmartFile> entries;
	private List<Sector> sectorList;

	public SmartFile(String fileName, EntryType type, SmartFile parent, List<Sector> sectorList) {
		this.fileName = fileName;
		this.type = type;
		this.parent = parent;
		this.sectorList = sectorList;
		entries = new ArrayList<SmartFile>();
	}

	public String getFileName() {
		return fileName;
	}

	public EntryType getType() {
		return type;
	}

	public SmartFile getParent() {
		return parent;
	}

	public List<SmartFile> getEntries() {
		return entries;
	}

	public List<Sector> getSectorList() {
		return sectorList;
	}
}
