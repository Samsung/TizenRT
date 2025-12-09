from __future__ import annotations

from dataclasses import dataclass
from enum import Enum

from Crypto.Cipher import AES

SSBLOCK_MAGIC = 0x73736B62  # bkss


class Permission(Enum):
    RO = 1  # Read-only
    RW = 2  # Read-write
    RE = 3  # Read-exposed


class BlockType(Enum):
    KEY = 1  # Key
    CERT = 2  # Certificate
    DATA = 3  # Data


KEY_MAX =  32   # Key
CERT_MAX = 32  # Certificate
DATA_MAX = 33  # Data
def get_region_offset(block_type: BlockType) -> int:
    if block_type == BlockType.KEY:
        return 0
    elif block_type == BlockType.CERT:
        return (4 * 1024) * KEY_MAX
    elif block_type == BlockType.DATA:
        return (4 * 1024) * (KEY_MAX + CERT_MAX)
    else:
        raise ValueError("invalid block type")


@dataclass
class ssblock_header:
    id: int
    magic: int
    version: int
    permision: Permission
    type: BlockType
    reserved: int
    iv: bytes
    tag: bytes
    cipher_len: int

    def get_header_bytes(self) -> bytes:
        if self.magic != SSBLOCK_MAGIC:
            raise ValueError("magic must be bkss")
        if len(self.iv) != 12:
            raise ValueError("IV must be 12 bytes")
        if len(self.tag) != 16:
            raise ValueError("tag must be 16 bytes")
        return (
            self.magic.to_bytes(4, byteorder="little")
            + self.version.to_bytes(1, byteorder="little")
            + self.permision.value.to_bytes(1, byteorder="little")
            + self.type.value.to_bytes(1, byteorder="little")
            + self.reserved.to_bytes(1, byteorder="little")
            + self.iv
            + self.tag
            + self.cipher_len.to_bytes(4, byteorder="little")
        )


def aes_gcm_encrypt(plaintext: bytes, key: bytes, nonce: bytes):
    cipher = AES.new(key, AES.MODE_GCM, nonce=nonce)  # pyright: ignore
    ciphertext, tag = cipher.encrypt_and_digest(plaintext)
    return ciphertext, tag


class ssblock:
    def __init__(
        self,
        id: int,
        plaintext: bytes,
        enc_key: bytes,
        block_type: str,
        permision: str,
        nonce: bytes,
    ) -> None:
        self._id = id
        self.plaintext = plaintext
        self.enc_key = enc_key
        self.nonce = nonce
        self._block_type = BlockType[block_type]
        self._permision = Permission[permision]

    def get_block_bytes(self) -> bytes:
        self._encrypt()
        header = self._get_header()
        header_bytes = header.get_header_bytes()
        total_len = (
            1024 * 12 if header.type == BlockType.DATA and header.id == 0 else 1024 * 4
        )
        padding_len = total_len - len(header_bytes) - self.cipher_len
        return header_bytes + self.ciphertext + bytes([0xFF] * padding_len)

    @property
    def block_type(self) -> BlockType:
        return self._block_type

    @property
    def id(self) -> int:
        return self._id

    def _check_id(self):
        if self._block_type == BlockType.KEY:
            if self._id > KEY_MAX:
                raise ValueError(
                    f"key id must be less than {KEY_MAX}"
                )
        elif self._block_type == BlockType.CERT:
            if self._id > CERT_MAX:
                raise ValueError(
                    f"cert id must be less than {CERT_MAX}"
                )
        elif self._block_type == BlockType.DATA:
            if self._id > DATA_MAX:
                raise ValueError(
                    f"data id must be less than {DATA_MAX}"
                )

    def _encrypt(self):
        self.ciphertext, self.tag = aes_gcm_encrypt(
            self.plaintext, self.enc_key, self.nonce
        )
        self.cipher_len = len(self.ciphertext)

    def _get_header(self) -> ssblock_header:
        return ssblock_header(
            self._id,
            SSBLOCK_MAGIC,
            0,
            self._permision,
            self._block_type,
            0xFF,
            self.nonce,
            self.tag,
            self.cipher_len,
        )
