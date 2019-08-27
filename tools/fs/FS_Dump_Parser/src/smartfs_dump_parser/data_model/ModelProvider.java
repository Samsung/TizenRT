package smartfs_dump_parser.data_model;

import java.util.ArrayList;
import java.util.List;

public enum ModelProvider {
	INSTANCE;

	private List<Header> headerList;
	private List<Header> activeSectors;
	private List<Header> dirtySectors;
	private List<Header> cleanSectors;

	private ModelProvider() {
		headerList = new ArrayList<Header>();
	}

	public List<Header> getHeaders() {
		return headerList;
	}

	public void addSectorHeader(Header header) {
		headerList.add(header);
	}

	public List<Header> getActiveSectors() {
		return activeSectors;
	}

	public void setActiveSectors(List<Header> activeSectors) {
		this.activeSectors = activeSectors;
	}

	public List<Header> getDirtySectors() {
		return dirtySectors;
	}

	public void setDirtySectors(List<Header> dirtySectors) {
		this.dirtySectors = dirtySectors;
	}

	public List<Header> getCleanSectors() {
		return cleanSectors;
	}

	public void setCleanSectors(List<Header> cleanSectors) {
		this.cleanSectors = cleanSectors;
	}

}
