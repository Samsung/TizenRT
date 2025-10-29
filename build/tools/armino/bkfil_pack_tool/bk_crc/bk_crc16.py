import logging
import shutil
from pathlib import Path

logger = logging.getLogger(__package__)


def crc16(data: bytes, offset: int, length: int):
    if offset < 0 or offset + length > len(data):
        return 0
    crc = 0xFFFFFFFF

    # beken poly:8005
    for i in range(length):
        crc ^= data[offset + i] << 8
        for _ in range(8):
            crc = (crc << 1) ^ 0x8005 if (crc & 0x8000) else (crc << 1)
    return crc & 0xFFFF


class bk_crc16:
    CRC_PACK_LEN = 32
    PADDING_BYTE = bytes([0xFF])

    def compute_crced_size(self, raw_size: int) -> int:
        return (
            (raw_size + self.CRC_PACK_LEN - 1)
            // self.CRC_PACK_LEN
            * (self.CRC_PACK_LEN + 2)
        )

    def crc_file(self, raw_file: Path, output_file: Path):
        self.file_path = raw_file.parent

        operate_file = self._backup_file(raw_file)
        self._file_align_handle(operate_file)
        self._crc16_handle(operate_file, output_file)
        operate_file.unlink()

    def crc16_data(self, raw_data: bytes) -> bytes:
        raw_data = self._data_align_handle(raw_data)
        output_data = bytearray()
        for i in range(0, len(raw_data), self.CRC_PACK_LEN):
            chunk = raw_data[i : i + self.CRC_PACK_LEN]
            output_data.extend(chunk)
            crc = crc16(chunk, 0, self.CRC_PACK_LEN).to_bytes(2, byteorder="big")
            output_data.extend(crc)

        return bytes(output_data)

    def _file_align_handle(self, operate_file: Path):
        file_size = operate_file.stat().st_size
        if file_size % self.CRC_PACK_LEN == 0:
            return

        padding_len = self.CRC_PACK_LEN - (file_size % self.CRC_PACK_LEN)
        logger.info(f"{padding_len=}")
        with operate_file.open("ab") as f:
            f.write(self.PADDING_BYTE * padding_len)

    def _backup_file(self, raw_file: Path) -> Path:
        raw_file_name = raw_file.parent
        operate_file_name = "bk_crc_" + raw_file_name.name
        operate_file_path = self.file_path / operate_file_name
        if operate_file_path.exists():
            operate_file_path.unlink()
        shutil.copy(raw_file, operate_file_path)
        return operate_file_path

    def _crc16_handle(self, operate_file: Path, output_file: Path):
        output_bytes = bytearray()
        chunk_size = 4096
        with operate_file.open("rb") as file:
            while True:
                data_chunk = file.read(chunk_size)
                if not data_chunk:
                    break
                output_bytes.extend(self.crc16_data(data_chunk))

        if output_file.exists():
            output_file.unlink()

        output_file.write_bytes(output_bytes)

    def _data_align_handle(self, raw_data: bytes):
        data_len = len(raw_data)
        if data_len % self.CRC_PACK_LEN == 0:
            return raw_data

        padding_len = self.CRC_PACK_LEN - (data_len % self.CRC_PACK_LEN)
        logger.debug(f"{padding_len=}")

        return raw_data + self.PADDING_BYTE * padding_len
