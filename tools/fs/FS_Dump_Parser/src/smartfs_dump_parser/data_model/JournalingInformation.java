////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Samsung Electronics All Rights Reserved.
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

public class JournalingInformation extends Sector {

	private Sector sector;
	private JournalTransactionType type;
	private boolean started;
	private boolean finished;
	private boolean needSync;
	private int targetSectorId;
	private int offset;
	private int dataSize;
	private int argument;

	public JournalingInformation(Sector s) {
		super();
		this.sector = s;
	}

	public Sector getSector() {
		return sector;
	}

	public JournalTransactionType getType() {
		return type;
	}

	public void setType(JournalTransactionType type) {
		this.type = type;
	}

	public boolean isStarted() {
		return started;
	}

	public void setStarted(boolean started) {
		this.started = started;
	}

	public boolean isFinished() {
		return finished;
	}

	public void setFinished(boolean finished) {
		this.finished = finished;
	}

	public boolean isNeedSync() {
		return needSync;
	}

	public void setNeedSync(boolean needSync) {
		this.needSync = needSync;
	}

	public int getTargetSectorId() {
		return targetSectorId;
	}

	public void setTargetSectorId(int targetSectorId) {
		this.targetSectorId = targetSectorId;
	}

	public int getOffset() {
		return offset;
	}

	public void setOffset(int offset) {
		this.offset = offset;
	}

	public int getDataSize() {
		return dataSize;
	}

	public void setDataSize(int dataSize) {
		this.dataSize = dataSize;
	}

	public int getArgument() {
		return argument;
	}

	public void setArgument(int argument) {
		this.argument = argument;
	}
}
