from enum import IntEnum

from ...bk_log import BKLog
from .base_app_protocol import BaseAppProtocol


class VerifyRandomBinResult(IntEnum):

    OK = 0
    STATE_ERROR = 1
    PARAM_ERROR = 2
    PUBKEY_ERROR = 3
    GROUP_ERROR = 4
    POINT_ERROR = 5
    HASH_ERROR = 6
    SIG_R_ERROR = 7
    SIG_S_ERROR = 8
    SIGN_ERROR = 9


_RX_LEN = 8
_RX_LEN_FIELD = 0x05
_CMD = 0x30
_RX_PREFIX = bytes([0x04, 0x0E, _RX_LEN_FIELD, 0x01, 0xE0, 0xFC, _CMD])


def _warn_if_ret_error(ret: int) -> None:
    if ret == 0:
        return
    try:
        msg = VerifyRandomBinResult(ret).name
    except ValueError:
        msg = 'unknown'
    BKLog.w('Check status error: err_code: {0}  err_msg: {1}'.format(ret, msg))


class VerifyRandomBinProtocol(BaseAppProtocol):

    CMD_CODE = _CMD

    def cmd(self, payload: bytes = b""):
        return self.command_generate(self.CMD_CODE, list(payload))

    @property
    def expect_length(self):
        return self.rx_expect_length(1)

    def response_check(self, response_content: bytes):
        if len(response_content) < _RX_LEN or response_content[:7] != _RX_PREFIX:
            return False, None
        ret = response_content[7]
        _warn_if_ret_error(ret)
        return True, ret
