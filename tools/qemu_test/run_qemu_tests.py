#!/usr/bin/env python3

from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import shlex
import sys
import time
import xml.etree.ElementTree as ET
from dataclasses import dataclass
from pathlib import Path

import pexpect


PROMPT = "TASH>>"
FAIL_MARKERS = [
    "] FAIL",
    "unregistered",
    "Assertion failed",
    "up_assert",
    "panic",
]
TESTCASE_END_RE = re.compile(
    r"##########\s+(?P<name>.+?)\s+End\s+\[PASS\s+:\s+(?P<pass>\d+),\s+FAIL\s+:\s+(?P<fail>\d+)\]\s+##########"
)


class HarnessError(RuntimeError):
    pass


@dataclass
class StepResult:
    name: str
    status: str
    duration: float
    message: str = ""


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("--artifact-dir", required=True)
    parser.add_argument("--boot-timeout", type=int, default=120)
    parser.add_argument("--command-timeout", type=int, default=10)
    parser.add_argument("--tc-timeout", type=int, default=60)
    return parser.parse_args()


def read_config(config_path: Path) -> dict[str, str]:
    config: dict[str, str] = {}
    for line in config_path.read_text(encoding="utf-8").splitlines():
        if not line or line.startswith("#") or "=" not in line:
            continue
        key, value = line.split("=", 1)
        config[key] = value.strip().strip('"')
    return config


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        while True:
            chunk = handle.read(1024 * 1024)
            if not chunk:
                break
            digest.update(chunk)
    return digest.hexdigest()


def collect_flash_metadata(topdir: Path) -> dict[str, dict[str, object]]:
    metadata: dict[str, dict[str, object]] = {}
    for name in ("qemu_flash.bin", "qemu_flash2.bin", "disk.raw"):
        path = topdir / name
        if not path.exists():
            continue
        metadata[name] = {
            "path": str(path),
            "size": path.stat().st_size,
            "sha256": sha256_file(path),
        }
    return metadata


def ensure_disk_image(topdir: Path, config: dict[str, str]) -> None:
    if config.get("CONFIG_QEMU_VIRT_VIRTIO_BLK") != "y":
        return

    disk_path = topdir / "disk.raw"
    if not disk_path.exists():
        with disk_path.open("wb") as handle:
            handle.truncate(64 * 1024 * 1024)


def build_qemu_command(topdir: Path, config: dict[str, str]) -> list[str]:
    qemu_flash = topdir / "qemu_flash.bin"
    if not qemu_flash.exists():
        raise HarnessError(f"Flash image not found: {qemu_flash}")

    command = [
        "qemu-system-arm",
        "-machine",
        "virt,virtualization=off,gic-version=2",
        "-cpu",
        "cortex-a15",
        "-m",
        "64M",
        "-nographic",
        "-monitor",
        "none",
        "-net",
        "none",
        "-drive",
        f"if=pflash,format=raw,file={qemu_flash}",
    ]

    if config.get("CONFIG_SECOND_FLASH_PARTITION") == "y":
        second_flash = topdir / "qemu_flash2.bin"
        if second_flash.exists():
            command.extend(
                ["-drive", f"if=pflash,format=raw,file={second_flash}"]
            )

    if config.get("CONFIG_QEMU_VIRT_VIRTIO_BLK") == "y":
        disk_path = topdir / "disk.raw"
        command.extend(
            [
                "-drive",
                f"file={disk_path},format=raw,if=none,id=blk0",
                "-device",
                "virtio-blk-device,drive=blk0,bus=virtio-mmio-bus.0",
            ]
        )

    if config.get("CONFIG_SMP") == "y":
        command.extend(["-smp", config.get("CONFIG_SMP_NCPUS", "2")])

    return command


class QemuHarness:
    def __init__(
        self,
        child: pexpect.spawn,
        transcript_path: Path,
        boot_timeout: int,
        command_timeout: int,
        tc_timeout: int,
    ) -> None:
        self.child = child
        self.transcript_path = transcript_path
        self.boot_timeout = boot_timeout
        self.command_timeout = command_timeout
        self.tc_timeout = tc_timeout
        self.results: list[StepResult] = []

    def _expect(self, patterns: list[object], timeout: int, description: str) -> object:
        all_patterns: list[object] = list(patterns) + FAIL_MARKERS + [pexpect.EOF, pexpect.TIMEOUT]
        index = self.child.expect(all_patterns, timeout=timeout)
        if index < len(patterns):
            return all_patterns[index]
        marker_index = index - len(patterns)
        if marker_index < len(FAIL_MARKERS):
            raise HarnessError(f"{description}: detected failure marker '{all_patterns[index]}'")
        if all_patterns[index] is pexpect.EOF:
            raise HarnessError(f"{description}: QEMU exited unexpectedly")
        raise HarnessError(f"{description}: timed out after {timeout}s")

    def _record(self, name: str, started_at: float, error: Exception | None = None) -> None:
        duration = time.monotonic() - started_at
        if error is None:
            self.results.append(StepResult(name=name, status="passed", duration=duration))
        else:
            self.results.append(
                StepResult(
                    name=name,
                    status="failed",
                    duration=duration,
                    message=str(error),
                )
            )

    def wait_for_prompt(self) -> None:
        started_at = time.monotonic()
        error: Exception | None = None
        try:
            self._expect([PROMPT], self.boot_timeout, "boot")
        except Exception as exc:
            error = exc
            raise
        finally:
            self._record("boot", started_at, error)

    def run_prompt_command(
        self,
        name: str,
        command: str,
        expected_tokens: list[object] | None = None,
        timeout: int | None = None,
    ) -> None:
        started_at = time.monotonic()
        error: Exception | None = None
        try:
            self.child.sendline(command)
            for token in expected_tokens or []:
                self._expect([token], timeout or self.command_timeout, name)
            self._expect([PROMPT], timeout or self.command_timeout, name)
        except Exception as exc:
            error = exc
            raise
        finally:
            self._record(name, started_at, error)

    def run_testcase(self, command: str) -> None:
        started_at = time.monotonic()
        error: Exception | None = None
        try:
            self.child.sendline(command)
            matched = self._expect([TESTCASE_END_RE], self.tc_timeout, command)
            match = matched.match(self.child.after) if hasattr(matched, "match") else None
            if not match:
                raise HarnessError(f"{command}: testcase summary not found")
            fail_count = int(match.group("fail"))
            if fail_count != 0:
                raise HarnessError(f"{command}: testcase reported FAIL={fail_count}")
            self.child.sendline("")
            self._expect([PROMPT], self.command_timeout, f"{command} prompt")
        except Exception as exc:
            error = exc
            raise
        finally:
            self._record(command, started_at, error)


def write_junit(results: list[StepResult], output_path: Path) -> None:
    suite = ET.Element(
        "testsuite",
        name="qemu-virt-runtime",
        tests=str(len(results)),
        failures=str(sum(1 for item in results if item.status != "passed")),
    )
    for result in results:
        case = ET.SubElement(
            suite,
            "testcase",
            name=result.name,
            time=f"{result.duration:.3f}",
        )
        if result.status != "passed":
            failure = ET.SubElement(case, "failure", message=result.message)
            failure.text = result.message

    output_path.write_text(
        ET.tostring(suite, encoding="unicode"),
        encoding="utf-8",
    )


def write_summary(
    results: list[StepResult],
    output_path: Path,
    command: list[str],
    metadata: dict[str, dict[str, object]],
) -> None:
    lines = ["# QEMU Virt Runtime Test Summary", ""]
    lines.append("## Command")
    lines.append("")
    lines.append(f"`{' '.join(shlex.quote(part) for part in command)}`")
    lines.append("")
    lines.append("## Results")
    lines.append("")
    for result in results:
        lines.append(
            f"- `{result.name}`: {result.status} ({result.duration:.2f}s)"
            + (f" - {result.message}" if result.message else "")
        )
    lines.append("")
    lines.append("## Flash Metadata")
    lines.append("")
    for name, entry in metadata.items():
        lines.append(f"- `{name}`: {entry['size']} bytes, sha256 `{entry['sha256']}`")

    output_path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def main() -> int:
    args = parse_args()
    artifact_dir = Path(args.artifact_dir)
    artifact_dir.mkdir(parents=True, exist_ok=True)

    topdir = Path.cwd()
    config = read_config(topdir / "os" / ".config")
    ensure_disk_image(topdir, config)
    qemu_command = build_qemu_command(topdir, config)

    transcript_path = artifact_dir / "qemu-transcript.log"
    junit_path = artifact_dir / "qemu-tests.xml"
    summary_path = artifact_dir / "summary.md"
    metadata_path = artifact_dir / "flash-metadata.json"

    results: list[StepResult] = []
    child: pexpect.spawn | None = None
    harness: QemuHarness | None = None
    exit_code = 0

    with transcript_path.open("w", encoding="utf-8") as transcript:
        try:
            child = pexpect.spawn(
                qemu_command[0],
                qemu_command[1:],
                cwd=str(topdir),
                encoding="utf-8",
                codec_errors="ignore",
            )
            child.logfile_read = transcript

            harness = QemuHarness(
                child=child,
                transcript_path=transcript_path,
                boot_timeout=args.boot_timeout,
                command_timeout=args.command_timeout,
                tc_timeout=args.tc_timeout,
            )

            harness.wait_for_prompt()
            harness.run_prompt_command("help", "help", ["virtio_blk_test"])
            harness.run_prompt_command("free", "free")
            harness.run_prompt_command("ps", "ps")
            harness.run_prompt_command("pwd-root", "pwd", ["/"])
            harness.run_prompt_command("ls-mnt", "ls /mnt")
            harness.run_prompt_command("mkdir", "mkdir /mnt/qemu-smoke")
            harness.run_prompt_command("ls-created-dir", "ls /mnt", ["qemu-smoke"])
            harness.run_prompt_command("write-file", "echo qemu-virt-smoke > /mnt/qemu-smoke.txt")
            harness.run_prompt_command("read-file", "cat /mnt/qemu-smoke.txt", ["qemu-virt-smoke"])
            harness.run_prompt_command("remove-file", "rm /mnt/qemu-smoke.txt")
            harness.run_prompt_command("remove-dir", "rmdir /mnt/qemu-smoke")
            harness.run_prompt_command("cd-mnt", "cd /mnt")
            harness.run_prompt_command("pwd-mnt", "pwd", ["/mnt"])
            harness.run_prompt_command("cd-root", "cd /")
            harness.run_prompt_command("virtio-blk", "virtio_blk_test", ["VIRTIO_BLK_TEST: PASS"], timeout=args.tc_timeout)
            harness.run_testcase("drivers_tc")
            harness.run_testcase("filesystem_tc")
            harness.run_testcase("kernel_tc")
        except Exception as exc:
            exit_code = 1
            if harness is not None:
                results = list(harness.results)
            if child is not None and hasattr(child, "before"):
                transcript.write(f"\n[HARNESS] {exc}\n")
            if results:
                results.append(
                    StepResult(
                        name="suite",
                        status="failed",
                        duration=0.0,
                        message=str(exc),
                    )
                )
            else:
                results = [
                    StepResult(
                        name="suite",
                        status="failed",
                        duration=0.0,
                        message=str(exc),
                    )
                ]
        finally:
            if harness is not None and not results:
                results = list(harness.results)
            if child is not None:
                try:
                    child.terminate(force=True)
                except Exception:
                    pass

    metadata = collect_flash_metadata(topdir)
    metadata_path.write_text(json.dumps(metadata, indent=2), encoding="utf-8")
    write_junit(results, junit_path)
    write_summary(results, summary_path, qemu_command, metadata)
    return exit_code


if __name__ == "__main__":
    sys.exit(main())
