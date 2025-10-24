from __future__ import annotations

import os
import struct
import click
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend

DATAUNIT_SIZE = 32
CRC_PACKET = 32
ENCRYPT_KEY = 914


def decrypt_random(s: str, key: int) -> str:
    data = bytearray()
    for i in range(0, len(s), 2):
        ch = (ord(s[i]) - 97) * 26 + (ord(s[i + 1]) - 97)
        data.append(ch)
    result = bytearray()
    for i in range(len(data)):
        ch = data[i] ^ (key >> 8)
        result.append(ch)
        key = ch + key
    return bytes(result).hex()


def str2key(hexstr: str) -> bytes:
    return bytes.fromhex(hexstr)


def generate_xts_tweak(unit_num: int) -> bytes:
    tweak = bytearray(16)
    for k in range(16):
        if 8 * k >= 32:
            break
        tweak[15 - k] = (unit_num >> (8 * k)) & 0xFF
    for j in range(8):
        tweak[j], tweak[15 - j] = tweak[15 - j], tweak[j]
    return bytes(tweak)


def xts_aes_process(data: bytes, key: bytes, start_address: int, mode: str) -> bytes:
    backend = default_backend()
    out = bytearray()
    key1 = key[:len(key)//2]
    key2 = key[len(key)//2:]
    full_key = key1 + key2
    total_units = len(data) // DATAUNIT_SIZE
    unit_num = start_address // DATAUNIT_SIZE

    for i in range(total_units):
        offset = i * DATAUNIT_SIZE
        block = data[offset: offset + DATAUNIT_SIZE]
        tweak = generate_xts_tweak(unit_num + i)

        cipher = Cipher(
            algorithms.AES(full_key),
            modes.XTS(tweak),
            backend=backend
        )

        operator = cipher.encryptor() if mode == 'ENC' else cipher.decryptor()
        processed = operator.update(block) + operator.finalize()
        out.extend(processed)
    return bytes(out)


def crc16_block(data: bytes) -> int:
    crc = 0xFFFFFFFF
    for b in data:
        crc ^= b << 8
        for _ in range(8):
            if crc & 0x8000:
                crc = (crc << 1) ^ 0x8005
            else:
                crc <<= 1
            crc &= 0xFFFF
    return crc


def append_crc_blocks(data: bytes) -> bytes:
    out = bytearray()
    total = len(data)
    i = 0
    while i < total:
        block = data[i:i + CRC_PACKET]
        if len(block) < CRC_PACKET:
            block = block + bytes([0xFF] * (CRC_PACKET - len(block)))
        crc = crc16_block(block)
        out.extend(block)
        out.extend(struct.pack(">H", crc))  # big endian
        i += CRC_PACKET
    return bytes(out)


@click.group()
def cli():
    pass


def load_key_from_file(path):
    with open(path, 'r') as f:
        for line in f:
            if '"data":' in line:
                enc = line.split('"')[3]
                return decrypt_random(enc[-128:], ENCRYPT_KEY)
    raise ValueError("No valid key found in keyfile")


def prepare_input_data(path: str) -> tuple[bytes, int]:
    with open(path, 'rb') as f:
        data = f.read()

    file_size = len(data)
    if file_size % DATAUNIT_SIZE:
        extended_size = (file_size + DATAUNIT_SIZE) - (file_size % DATAUNIT_SIZE)
    else:
        extended_size = file_size

    padded = bytearray([0xFF] * extended_size)
    padded[:file_size] = data
    return bytes(padded), file_size


@cli.command()
@click.option('-infile', required=True, type=click.Path(exists=True))
@click.option('-startaddress', required=True, type=str)
@click.option('-aes', required=True, type=click.Choice(['128', '256']))
@click.option('-keywords', type=str)
@click.option('-keyfile', type=click.Path(exists=True))
@click.option('-outfile', required=True, type=click.Path())
def encrypt(infile, startaddress, aes, keywords, keyfile, outfile):
    process('ENC', infile, startaddress, aes, keywords, keyfile, outfile)


@cli.command()
@click.option('-infile', required=True, type=click.Path(exists=True))
@click.option('-startaddress', required=True, type=str)
@click.option('-aes', required=True, type=click.Choice(['128', '256']))
@click.option('-keywords', type=str)
@click.option('-keyfile', type=click.Path(exists=True))
@click.option('-outfile', required=True, type=click.Path())
def decrypt(infile, startaddress, aes, keywords, keyfile, outfile):
    process('DEC', infile, startaddress, aes, keywords, keyfile, outfile)


def process(mode, infile, startaddress, aes, keywords, keyfile, outfile):
    start_addr = int(startaddress, 0)
    print(f"file path : {infile}")
    print(f"startAddr : 0x{start_addr:08X}")

    key_hex = None
    if keywords:
        if len(keywords) == 64:
            key_hex = keywords
        elif len(keywords) in [128, 132]:
            key_hex = decrypt_random(keywords[-128:], ENCRYPT_KEY)
    elif keyfile:
        key_hex = load_key_from_file(keyfile)
    else:
        raise ValueError("Either -keywords or -keyfile must be provided.")

    key_bytes = str2key(key_hex)
    if (aes == '128' and len(key_bytes) != 32) or (aes == '256' and len(key_bytes) != 64):
        raise ValueError("Key length mismatch with AES mode.")

    data, file_size = prepare_input_data(infile)
    result = xts_aes_process(data, key_bytes, start_addr, mode)

    with open(outfile, 'wb') as f:
        f.write(result)

    # --- CRC calc ---
    crc_output = append_crc_blocks(result)
    crc_file = outfile.replace('.bin', '_crc.bin')
    with open(crc_file, 'wb') as f:
        f.write(crc_output)

    print(f"{mode} complete: {outfile}, CRC file: {crc_file}")


if __name__ == '__main__':
    cli()
