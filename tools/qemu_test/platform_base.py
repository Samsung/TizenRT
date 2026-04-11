#!/usr/bin/env python3

from __future__ import annotations

import hashlib
from dataclasses import dataclass, field
from pathlib import Path
from typing import Pattern


ExpectedToken = str | Pattern[str]


@dataclass(frozen=True)
class PromptCommandStep:
    name: str
    command: str
    expected_tokens: tuple[ExpectedToken, ...] = ()
    timeout: int | None = None


@dataclass(frozen=True)
class TestcaseStep:
    command: str


Step = PromptCommandStep | TestcaseStep


@dataclass(frozen=True)
class PlatformSpec:
    board: str
    suite_name: str
    qemu_command: list[str]
    steps: tuple[Step, ...]
    metadata: dict[str, dict[str, object]] = field(default_factory=dict)
    prompt: str = "TASH>>"
    post_boot_delay: float = 0.0
    prompt_settle_delay: float = 0.0
    send_char_delay: float = 0.0
    fail_markers: tuple[str, ...] = (
        "] FAIL",
        "unregistered",
        "Assertion failed",
        "up_assert",
        "panic",
        "HardFault",
        "Lockup:",
    )


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        while True:
            chunk = handle.read(1024 * 1024)
            if not chunk:
                break
            digest.update(chunk)
    return digest.hexdigest()


def file_metadata(path: Path) -> dict[str, object]:
    return {
        "path": str(path),
        "size": path.stat().st_size,
        "sha256": sha256_file(path),
    }


def collect_named_files(entries: list[tuple[str, Path]]) -> dict[str, dict[str, object]]:
    metadata: dict[str, dict[str, object]] = {}
    for name, path in entries:
        if path.exists():
            metadata[name] = file_metadata(path)
    return metadata
