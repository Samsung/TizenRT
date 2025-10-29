from __future__ import annotations

from pathlib import Path
from typing import BinaryIO
from bk_ssblock import ssblock, BlockType, get_region_offset
import csv
import sys


def read_ss_config(config_file: Path) -> list[ssblock]:
    ssblocks: list[ssblock] = []
    with open(config_file, "r", newline="") as f:
        csv_reader = csv.reader(f)
        next(csv_reader)
        key_line = next(csv_reader)
        enc_key = key_line[5].strip()
        enc_key = bytes.fromhex(enc_key)
        if len(enc_key) != 32:
            raise RuntimeError(f"ss enc_key len must be 32, len:{len(enc_key)}")
        for row in csv_reader:
            nonce = bytes.fromhex(row[4])
            if row[3].strip().upper() == "HEX":
                plainbytes = bytes.fromhex(row[5])
            elif row[3].strip().upper() == "TEXT":
                plainbytes = row[5].encode(encoding="utf-8")
            else:
                raise RuntimeError("encoding unknwon")
            block = ssblock(int(row[0]), plainbytes, enc_key, row[1], row[2], nonce)
            ssblocks.append(block)
    return ssblocks


def export_binary(config_file: Path, bin_file: Path):
    def offset_align(file_stream: BinaryIO, align_size: int):
        current_pos = file_stream.tell()
        if current_pos < align_size:
            fill_size = align_size - current_pos
            file_stream.write(bytes([0xFF] * fill_size))

    ssblocks: list[ssblock] = read_ss_config(config_file)
    ssblocks.sort(key=lambda block: (block.block_type.value, block.id))
    key_blocks: list[ssblock] = [
        block for block in ssblocks if block.block_type == BlockType.KEY
    ]
    cert_blocks: list[ssblock] = [
        block for block in ssblocks if block.block_type == BlockType.CERT
    ]
    data_blocks: list[ssblock] = [
        block for block in ssblocks if block.block_type == BlockType.DATA
    ]
    key_offset = get_region_offset(BlockType.KEY)
    cert_offset = get_region_offset(BlockType.CERT)
    data_offset = get_region_offset(BlockType.DATA)
    block_groups = [
        (key_offset, key_blocks),
        (cert_offset, cert_blocks),
        (data_offset, data_blocks),
    ]
    with open(bin_file, "wb") as f:
        for offset, blocks in block_groups:
            if len(blocks) == 0:
                continue
            offset_align(f, offset)
            for block in blocks:
                f.write(block.get_block_bytes())


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python ss.py <csv_file> <ssblock_bin>")
        sys.exit(1)

    csv_file = Path(sys.argv[1])
    assert csv_file.exists()
    ssblock_bin = Path(sys.argv[2])
    assert ssblock_bin.parent.exists()
    assert not ssblock_bin.is_dir()
    export_binary(csv_file, ssblock_bin)

    # python3 security_storage.py security_storage.csv ss.bin
