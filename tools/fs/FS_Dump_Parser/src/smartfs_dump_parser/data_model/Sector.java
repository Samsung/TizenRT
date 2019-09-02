package smartfs_dump_parser.data_model;

public class Sector {
	private Header header;
	private byte[] flashData;

	public Sector(byte[] flashData) {
		this.flashData = flashData;
	}

	public Header getHeader() {
		return header;
	}

	public void setHeader(Header header) {
		this.header = header;
	}

	public byte[] getFlashData() {
		return flashData;
	}
}
