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
