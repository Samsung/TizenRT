#!/usr/bin/env python3

from __future__ import annotations

import argparse
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

from platform_base import PlatformSpec, PromptCommandStep, TestcaseStep
import platform_qemu_mps2_an505
import platform_qemu_virt


TESTCASE_END_RE = re.compile(
    r"##########\s+(?P<name>.+?)\s+End\s+\[PASS\s+:\s+(?P<pass>\d+),\s+FAIL\s+:\s+(?P<fail>\d+)\]\s+##########"
)


class HarnessError(RuntimeError):
    pass


class SanitizedTranscriptWriter:
    def __init__(self, handle) -> None:
        self._handle = handle
        self._line: list[str] = []
        self._col = 0
        self._pending_cr = False
        self._last_blank = False

    def _emit_line(self) -> None:
        text = "".join(self._line).rstrip()
        if text or not self._last_blank:
            self._handle.write(text + "\n")
        self._last_blank = not text
        self._line = []
        self._col = 0

    def write(self, data: str) -> int:
        written = 0
        for ch in data:
            if self._pending_cr:
                if ch == "\n":
                    self._emit_line()
                    self._pending_cr = False
                    written += 1
                    continue

                self._line = []
                self._col = 0
                self._pending_cr = False

            if ch == "\x00":
                continue
            if ch == "\r":
                self._pending_cr = True
                continue
            if ch == "\n":
                self._emit_line()
                written += 1
                continue

            if self._col < len(self._line):
                self._line[self._col] = ch
            else:
                self._line.append(ch)
            self._col += 1
            written += 1
        return written

    def flush(self) -> None:
        if self._pending_cr:
            self._emit_line()
            self._pending_cr = False
        if self._line:
            self._emit_line()
        self._handle.flush()


@dataclass
class StepResult:
    name: str
    status: str
    duration: float
    message: str = ""


PLATFORM_BUILDERS = {
    "qemu-virt": platform_qemu_virt.build_platform,
    "qemu-mps2-an505": platform_qemu_mps2_an505.build_platform,
}


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("--results-dir")
    parser.add_argument("--artifact-dir")
    parser.add_argument("--board", choices=sorted(PLATFORM_BUILDERS))
    parser.add_argument("--boot-timeout", type=int, default=120)
    parser.add_argument("--command-timeout", type=int, default=10)
    parser.add_argument("--tc-timeout", type=int, default=60)
    args = parser.parse_args()
    if not args.results_dir and not args.artifact_dir:
        parser.error("one of --results-dir or --artifact-dir is required")
    return args


def read_config(config_path: Path) -> dict[str, str]:
    config: dict[str, str] = {}
    for line in config_path.read_text(encoding="utf-8").splitlines():
        if not line or line.startswith("#") or "=" not in line:
            continue
        key, value = line.split("=", 1)
        config[key] = value.strip().strip('"')
    return config

def resolve_platform(topdir: Path, config: dict[str, str], board: str | None) -> PlatformSpec:
    board_name = board or config.get("CONFIG_ARCH_BOARD")
    if not board_name:
        raise HarnessError("CONFIG_ARCH_BOARD is not defined and --board was not provided")

    builder = PLATFORM_BUILDERS.get(board_name)
    if builder is None:
        raise HarnessError(f"Unsupported board '{board_name}'")
    return builder(topdir, config)


class QemuHarness:
    def __init__(
        self,
        child: pexpect.spawn,
        prompt: str,
        fail_markers: list[str],
        boot_timeout: int,
        command_timeout: int,
        tc_timeout: int,
        post_boot_delay: float = 0.0,
        prompt_settle_delay: float = 0.0,
        send_char_delay: float = 0.0,
    ) -> None:
        self.child = child
        self.prompt = prompt
        self.fail_markers = fail_markers
        self.boot_timeout = boot_timeout
        self.command_timeout = command_timeout
        self.tc_timeout = tc_timeout
        self.post_boot_delay = post_boot_delay
        self.prompt_settle_delay = prompt_settle_delay
        self.send_char_delay = send_char_delay
        self.results: list[StepResult] = []

    def _settle_prompt(self) -> None:
        if self.prompt_settle_delay > 0:
            time.sleep(self.prompt_settle_delay)

    def _send_command(self, command: str) -> None:
        self._settle_prompt()
        if self.send_char_delay > 0:
            for ch in command:
                self.child.send(ch)
                time.sleep(self.send_char_delay)
            self.child.send("\r\n")
        else:
            self.child.sendline(command)

    def _expect(self, patterns: list[object], timeout: int, description: str) -> object:
        all_patterns: list[object] = list(patterns) + self.fail_markers + [pexpect.EOF, pexpect.TIMEOUT]
        index = self.child.expect(all_patterns, timeout=timeout)
        if index < len(patterns):
            return all_patterns[index]
        marker_index = index - len(patterns)
        if marker_index < len(self.fail_markers):
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
            self._expect([self.prompt], self.boot_timeout, "boot")
            if self.post_boot_delay > 0:
                time.sleep(self.post_boot_delay)
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
            self._send_command(command)
            for token in expected_tokens or []:
                self._expect([token], timeout or self.command_timeout, name)
            self._expect([self.prompt], timeout or self.command_timeout, name)
        except Exception as exc:
            error = exc
            raise
        finally:
            self._record(name, started_at, error)

    def run_testcase(self, command: str) -> None:
        started_at = time.monotonic()
        error: Exception | None = None
        try:
            self._send_command(command)
            matched = self._expect([TESTCASE_END_RE], self.tc_timeout, command)
            match = matched.match(self.child.after) if hasattr(matched, "match") else None
            if not match:
                raise HarnessError(f"{command}: testcase summary not found")
            fail_count = int(match.group("fail"))
            if fail_count != 0:
                raise HarnessError(f"{command}: testcase reported FAIL={fail_count}")
            self.child.sendline("")
            self._expect([self.prompt], self.command_timeout, f"{command} prompt")
        except Exception as exc:
            error = exc
            raise
        finally:
            self._record(command, started_at, error)


def run_platform_steps(harness: QemuHarness, platform: PlatformSpec) -> None:
    for step in platform.steps:
        if isinstance(step, PromptCommandStep):
            harness.run_prompt_command(
                step.name,
                step.command,
                list(step.expected_tokens),
                timeout=step.timeout,
            )
        elif isinstance(step, TestcaseStep):
            harness.run_testcase(step.command)
        else:
            raise HarnessError(f"Unsupported step type: {type(step)!r}")


def write_junit(results: list[StepResult], output_path: Path, suite_name: str) -> None:
    suite = ET.Element(
        "testsuite",
        name=suite_name,
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
    platform: PlatformSpec,
    metadata: dict[str, dict[str, object]],
) -> None:
    lines = [f"# {platform.suite_name} Summary", ""]
    lines.append(f"- Board: `{platform.board}`")
    lines.append("")
    lines.append("## Command")
    lines.append("")
    lines.append(f"`{' '.join(shlex.quote(part) for part in platform.qemu_command)}`")
    lines.append("")
    lines.append("## Results")
    lines.append("")
    for result in results:
        lines.append(
            f"- `{result.name}`: {result.status} ({result.duration:.2f}s)"
            + (f" - {result.message}" if result.message else "")
        )
    lines.append("")
    lines.append("## Runtime Metadata")
    lines.append("")
    for name, entry in metadata.items():
        lines.append(f"- `{name}`: {entry['size']} bytes, sha256 `{entry['sha256']}`")

    output_path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def main() -> int:
    args = parse_args()
    results_dir = Path(args.results_dir or args.artifact_dir)
    results_dir.mkdir(parents=True, exist_ok=True)

    topdir = Path.cwd()
    config = read_config(topdir / "os" / ".config")
    platform = resolve_platform(topdir, config, args.board)

    transcript_path = results_dir / "qemu-transcript.log"
    junit_path = results_dir / "qemu-tests.xml"
    summary_path = results_dir / "summary.md"
    metadata_path = results_dir / "runtime-metadata.json"

    results: list[StepResult] = []
    child: pexpect.spawn | None = None
    harness: QemuHarness | None = None
    exit_code = 0

    with transcript_path.open("w", encoding="utf-8") as transcript:
        transcript_writer = SanitizedTranscriptWriter(transcript)
        try:
            child = pexpect.spawn(
                platform.qemu_command[0],
                platform.qemu_command[1:],
                cwd=str(topdir),
                encoding="utf-8",
                codec_errors="ignore",
            )
            child.logfile_read = transcript_writer

            harness = QemuHarness(
                child=child,
                prompt=platform.prompt,
                fail_markers=list(platform.fail_markers),
                boot_timeout=args.boot_timeout,
                command_timeout=args.command_timeout,
                tc_timeout=args.tc_timeout,
                post_boot_delay=platform.post_boot_delay,
                prompt_settle_delay=platform.prompt_settle_delay,
                send_char_delay=platform.send_char_delay,
            )

            harness.wait_for_prompt()
            run_platform_steps(harness, platform)
        except Exception as exc:
            exit_code = 1
            if harness is not None:
                results = list(harness.results)
            if child is not None and hasattr(child, "before"):
                transcript_writer.write(f"\n[HARNESS] {exc}\n")
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
            transcript_writer.flush()

    metadata = platform.metadata
    metadata_path.write_text(json.dumps(metadata, indent=2), encoding="utf-8")
    write_junit(results, junit_path, platform.suite_name)
    write_summary(results, summary_path, platform, metadata)
    return exit_code


if __name__ == "__main__":
    sys.exit(main())
