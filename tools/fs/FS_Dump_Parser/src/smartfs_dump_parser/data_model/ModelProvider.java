package smartfs_dump_parser.data_model;

import java.util.ArrayList;
import java.util.List;

public enum ModelProvider {
	INSTANCE;
	
	private List<Header> headerList;
	
	private ModelProvider() {
		headerList = new ArrayList<Header>();		
	}
	
	public List<Header> getHeaders() {
		return headerList;
	}
	
	public void addSectorHeader(Header header) {		
		headerList.add(header);
	}
}
