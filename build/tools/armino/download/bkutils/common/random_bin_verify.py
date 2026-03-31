
import os
import time
from typing import Optional

from bkutils.bk_log.log_utils import BKLog
from bkutils.common.common import LINKCHECK
from bkutils.protocol.app.verify_random_bin_protocol import (
    VerifyRandomBinProtocol,
    VerifyRandomBinResult,
)

RANDOM_BIN_MAX_BYTES = 192


def _effective_random_bin_path(random_bin_path: Optional[str]) -> Optional[str]:
    if random_bin_path is None:
        return None
    s = random_bin_path.strip()
    return s if s else None


def maybe_verify_random_bin_after_get_bus(
    ser,
    linktype,
    random_bin_path: Optional[str],
) -> None:
    if linktype != LINKCHECK.BL2:
        return
    eff = _effective_random_bin_path(random_bin_path)
    if eff is None:
        raise Exception(
            "random.bin verify: BL2 requires --random_bin_path (parameter not passed or empty)."
        )
    if not os.path.isfile(eff):
        abs_path = os.path.abspath(eff)
        BKLog.e(f"random.bin verify: file not found, path={abs_path}")
        raise Exception(
            f"random.bin verify: random.bin file not found at path: {abs_path}"
        )
    with open(eff, "rb") as f:
        data = f.read()
    if len(data) > RANDOM_BIN_MAX_BYTES:
        raise Exception(
            f"random.bin exceeds {RANDOM_BIN_MAX_BYTES} bytes (got {len(data)})"
        )
    time.sleep(0.1)
    proto = VerifyRandomBinProtocol()
    cmd = proto.cmd(data)
    max_attempts = 5
    retry_delay_sec = 0.1
    last_error = ""
    for attempt in range(max_attempts):
        if attempt > 0:
            time.sleep(retry_delay_sec)
        ok, resp = ser.write_cmd_and_wait_response(
            cmd, proto.expect_length, timeout_sec=2.0
        )
        if not ok or not resp:
            last_error = "random.bin verify: no valid response from device"
            BKLog.w(f"{last_error} (attempt {attempt + 1}/{max_attempts})")
            continue
        valid, ret = proto.response_check(resp)
        if not valid:
            last_error = "random.bin verify: invalid response frame"
            BKLog.w(f"{last_error} (attempt {attempt + 1}/{max_attempts})")
            continue
        if ret != VerifyRandomBinResult.OK:
            last_error = f"random.bin verify failed: ret={int(ret)}"
            BKLog.w(f"{last_error} (attempt {attempt + 1}/{max_attempts})")
            continue
        BKLog.i("random.bin verify OK")
        return
    raise Exception(f"{last_error} (after {max_attempts} attempts)")
