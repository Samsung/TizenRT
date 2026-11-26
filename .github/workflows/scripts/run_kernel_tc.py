#!/usr/bin/env python3
"""Run kernel_tc through the QEMU serial console and record its final counts."""
import sys
from pathlib import Path

import pexpect


def run(qemu, kernel, log):
    args = ["-M", "lm3s6965evb", "-kernel", kernel, "-nographic",
            "-monitor", "none", "-net", "none"]
    with pexpect.spawn(qemu, args, echo=False, logfile=None) as guest:
        guest.logfile_read = log
        stage = "TASH prompt"
        try:
            guest.expect_exact(b"TASH>>", timeout=60)
            guest.send(b"kernel_tc\r")
            stage = "Kernel TC Start"
            guest.expect_exact(b"########## Kernel TC Start ##########", timeout=60)
            stage = "Kernel TC End"
            end = rb"########## Kernel TC End \[PASS : (\d+), FAIL : (\d+)\] ##########"
            fatal = rb"up_assert: Assertion failed|qemu: fatal:"
            if guest.expect([end, fatal], timeout=1200) == 1:
                # Capture the fault details that follow the first diagnostic.
                guest.expect([pexpect.EOF, pexpect.TIMEOUT], timeout=5)
                return False, "Guest assertion or QEMU fatal error during kernel_tc"
        except pexpect.EOF:
            return False, f"QEMU exited before {stage}"
        except pexpect.TIMEOUT:
            return False, f"Timed out waiting for {stage}"
        passed, failed = map(int, guest.match.groups())
        return passed > 0 and failed == 0, f"PASS {passed}, FAIL {failed}"


def main(qemu, kernel, output):
    directory = Path(output)
    with (directory / "kernel-tc-uart.log").open("wb") as log:
        success, result = run(qemu, kernel, log)
    print(result)
    (directory / "kernel-tc.txt").write_text(result + "\n")
    return 0 if success else 1


if __name__ == "__main__":
    sys.exit(main(*sys.argv[1:]))
