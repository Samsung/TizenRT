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

public class Header {
	private int phyicalSectorId;
	private int logicalSectorId;
	private int sequenceNumer;
	private int crc8;
	private int status;

	public Header(int phyicalSectorId, int logicalSectorId, int sequenceNumer, int crc8, int status) {
		this.phyicalSectorId = phyicalSectorId;
		this.logicalSectorId = logicalSectorId;
		this.sequenceNumer = sequenceNumer;
		this.crc8 = crc8;
		this.status = status;
	}

	public int getPhyicalSectorId() {
		return phyicalSectorId;
	}

	public void setPhyicalSectorId(int phyicalSectorId) {
		this.phyicalSectorId = phyicalSectorId;
	}

	public int getLogicalSectorId() {
		return logicalSectorId;
	}

	public void setLogicalSectorId(int logicalSectorId) {
		this.logicalSectorId = logicalSectorId;
	}

	public int getSequenceNumber() {
		return sequenceNumer;
	}

	public void setSeq_num(int sequenceNumer) {
		this.sequenceNumer = sequenceNumer;
	}

	public int getCrc8() {
		return crc8;
	}

	public void setCrc8(int crc8) {
		this.crc8 = crc8;
	}

	public int getStatus() {
		return status;
	}

	public void setStatus(int status) {
		this.status = status;
	}
}
