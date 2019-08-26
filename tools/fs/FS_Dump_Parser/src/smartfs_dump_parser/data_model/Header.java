package smartfs_dump_parser.data_model;

public class Header {
	private int phyicalSectorNum;
	private int sectorNum;
	private int seqNum;
	private int crc8;
	private int status;

	public Header(int physicalSectorNum, int sectorNum, int seqNum, int crc8, int status) {
		this.phyicalSectorNum = physicalSectorNum;
		this.sectorNum = sectorNum;
		this.seqNum = seqNum;
		this.crc8 = crc8;
		this.status = status;
	}

	public int getPhyicalSectorNum() {
		return phyicalSectorNum;
	}

	public void setPhyicalSectorNum(int phyicalSectorNum) {
		this.phyicalSectorNum = phyicalSectorNum;
	}

	public int getSector_num() {
		return sectorNum;
	}

	public void setSector_num(int sectorNum) {
		this.sectorNum = sectorNum;
	}

	public int getSeq_num() {
		return seqNum;
	}

	public void setSeq_num(int seqNum) {
		this.seqNum = seqNum;
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
