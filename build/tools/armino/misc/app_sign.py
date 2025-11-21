from __future__ import annotations

import hashlib
import logging
import struct
import sys
import zlib
from pathlib import Path

from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import ec

logger = logging.getLogger(Path(__file__).name)


def load_keys(
    private_key_file: Path, public_key_file: Path
) -> tuple[ec.EllipticCurvePrivateKey, ec.EllipticCurvePublicKey]:
    """Load private and public keys from files"""
    try:
        # Load private key
        private_pem = private_key_file.read_bytes()
        private_key = serialization.load_pem_private_key(
            private_pem,
            password=None,
        )

        # Load public key
        public_pem = public_key_file.read_bytes()
        public_key = serialization.load_pem_public_key(public_pem, backend=None)

        # Type assertions
        assert isinstance(private_key, ec.EllipticCurvePrivateKey)
        assert isinstance(public_key, ec.EllipticCurvePublicKey)

        return private_key, public_key
    except FileNotFoundError:
        print("Key files not found. Please ensure private and public key files exist.")
        raise


def public_key_to_64bytes(public_key: ec.EllipticCurvePublicKey) -> bytes:
    public_numbers = public_key.public_numbers()
    x_bytes = public_numbers.x.to_bytes(32, byteorder="big")
    y_bytes = public_numbers.y.to_bytes(32, byteorder="big")
    return x_bytes + y_bytes


def ecdsa_sign(private_key: ec.EllipticCurvePrivateKey, data: bytes) -> bytes:
    """Sign data with private key using ECDSA"""
    # Sign the data using SHA256 hash algorithm
    signature = private_key.sign(data, ec.ECDSA(hashes.SHA256()))
    return signature


# Convert ECDSA signature from ASN.1 DER encoding to 64-byte sequence
def signature_to_64bytes(signature: bytes) -> bytes:
    # Simple ASN.1 DER parsing (may need more robust parsing in production)
    # Skip sequence tag and length bytes
    if signature[0] != 0x30:  # ASN.1 sequence tag
        raise ValueError("Invalid signature format")

    # Extract r and s values
    # This is a simplified version, may need dedicated ASN.1 parsing library in production
    offset = 2  # Skip sequence tag and length
    if signature[offset] != 0x02:  # Integer tag
        raise ValueError("Invalid signature format")
    r_len = signature[offset + 1]
    r = int.from_bytes(signature[offset + 2 : offset + 2 + r_len], byteorder="big")

    offset += 2 + r_len
    if signature[offset] != 0x02:  # Integer tag
        raise ValueError("Invalid signature format")
    s_len = signature[offset + 1]
    s = int.from_bytes(signature[offset + 2 : offset + 2 + s_len], byteorder="big")

    # Convert to 32-byte big-endian
    r_bytes = r.to_bytes(32, byteorder="big")
    s_bytes = s.to_bytes(32, byteorder="big")

    return r_bytes + s_bytes


class Signature:
    SHA256_HASH_SIZE = 32
    ECDSA256_SIGNATURE_SIZE = 64
    ECDSA256_PUBLIC_KEY_SIZE = 64

    def __init__(
        self, firmware_bytes: bytes, private_key_path: Path, public_key_path: Path
    ):
        self.firmware_bytes = firmware_bytes
        self.private_key_path = private_key_path
        self.public_key_path = public_key_path
        self.magic = 0x5349474E  # 'SIGN' in ASCII
        self.sig_version = 0

    def generate_signature(self) -> bytes:
        return self._generate_firmware_signature()

    def set_magic(self, magic: int):
        self.magic = magic
        logger.info(f"set magic to {magic}")

    def set_sig_version(self, sig_version: int):
        self.sig_version = sig_version
        logger.info(f"set sig version to {sig_version}")

    def _generate_firmware_signature(self) -> bytes:
        # Load keys
        private_key, public_key = load_keys(self.private_key_path, self.public_key_path)

        firmware_size = len(self.firmware_bytes)
        logger.info(f"firmware size: {firmware_size}")
        firmware_hash = hashlib.sha256(self.firmware_bytes).digest()
        logger.info(f"firmware hash: {firmware_hash.hex()}")

        # Get public key in 64-byte format
        public_key_64bytes = public_key_to_64bytes(public_key)
        logger.info(f"public key 64bytes: {public_key_64bytes.hex()}")

        # Sign the firmware hash
        hash_signature = ecdsa_sign(private_key, firmware_hash)
        logger.info(f"hash signature: {hash_signature.hex()}")
        signature_64bytes = signature_to_64bytes(hash_signature)
        logger.info(f"signature 64bytes: {signature_64bytes.hex()}")

        # Pack the structure (excluding CRC32)
        struct_without_crc = struct.pack(
            f"<III{self.SHA256_HASH_SIZE}s{self.ECDSA256_PUBLIC_KEY_SIZE}s{self.ECDSA256_SIGNATURE_SIZE}s",
            self.magic,
            self.sig_version,
            firmware_size,
            firmware_hash,
            public_key_64bytes,
            signature_64bytes,
        )
        logger.info(f"struct without crc: {struct_without_crc.hex()}")

        # Calculate CRC32 checksum
        crc32 = zlib.crc32(struct_without_crc)
        crc32_bytes = crc32.to_bytes(4, byteorder="little")
        logger.debug(f"crc32: {crc32}")
        logger.info(f"crc32 bytes: {crc32_bytes.hex()}")
        return struct_without_crc + crc32_bytes


def set_logging():
    log_format = "[%(name)s|%(levelname)s] %(message)s"
    logging.basicConfig(format=log_format, level=logging.INFO)


def main():
    firmware_bin = Path(sys.argv[1])
    private_key_path = Path(sys.argv[2])
    public_key_path = Path(sys.argv[3])
    assert firmware_bin.exists()
    assert private_key_path.exists()
    assert public_key_path.exists()
    firmware_bytes = firmware_bin.read_bytes()
    padding_length = (32 - (len(firmware_bytes) % 32)) % 32
    if padding_length > 0:
        firmware_bytes += b"\xff" * padding_length
    signature = Signature(firmware_bytes, private_key_path, public_key_path)
    sign_bytes = signature.generate_signature()
    signed_firmware = sign_bytes + firmware_bytes
    firmware_bin.write_bytes(signed_firmware)
    logger.info(f"Signed firmware saved to {firmware_bin}")


if __name__ == "__main__":
    set_logging()
    main()
