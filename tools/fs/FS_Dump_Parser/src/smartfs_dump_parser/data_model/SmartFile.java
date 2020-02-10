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
import java.util.List;
import java.util.Map;

public class SmartFile {
	private String fileName;
	private EntryType type;
	private SmartFile parent;
	private List<SmartFile> entries;
	private Map<Integer, Sector> sectorMap;

	public SmartFile(String fileName, EntryType type, SmartFile parent, Map<Integer, Sector> sectorMap) {
		this.fileName = fileName;
		this.type = type;
		this.parent = parent;
		this.sectorMap = sectorMap;
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

	public Map<Integer, Sector> getSectorMap() {
		return sectorMap;
	}
}
