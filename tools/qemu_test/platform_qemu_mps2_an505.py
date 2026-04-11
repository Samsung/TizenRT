#!/usr/bin/env python3

from __future__ import annotations

import struct
from pathlib import Path

from platform_base import PlatformSpec, PromptCommandStep, TestcaseStep, collect_named_files


def _config_enabled(config: dict[str, str], key: str) -> bool:
    return config.get(key) == "y"


def _resolve_kernel_path(topdir: Path) -> Path:
    image_dir = topdir / "qemu_mps2_an505_images"
    preferred = [
        image_dir / "kernel-runtime.elf",
        topdir / "build" / "output" / "bin" / "kernel_without_header.bin",
        image_dir / "kernel-runtime-raw.bin",
        image_dir / "flash-runtime.bin",
        image_dir / "kernel-runtime.bin",
        topdir / "build" / "output" / "bin" / "kernel",
        topdir / "build" / "output" / "bin" / "tinyara",
        topdir / "build" / "output" / "bin" / "tinyara.bin",
    ]

    for candidate in preferred:
        if candidate.exists():
            return candidate

    raise RuntimeError("No runnable qemu-mps2-an505 kernel image found. Run build/download first.")


def _userspace_text_field_offset(config: dict[str, str]) -> int:
    offset = 4
    if not _config_enabled(config, "CONFIG_DISABLE_PTHREAD"):
        offset += 4
    if not _config_enabled(config, "CONFIG_DISABLE_SIGNALS"):
        offset += 4
    return offset


def _xip_exec_base(image_path: Path, config: dict[str, str], header_size: int) -> int:
    image_offset = 4 + header_size
    uspace_offset = image_offset + 4
    text_field_offset = _userspace_text_field_offset(config)
    data = image_path.read_bytes()
    text_start = struct.unpack_from("<I", data, uspace_offset + text_field_offset)[0]
    return text_start - image_offset


def build_platform(topdir: Path, config: dict[str, str]) -> PlatformSpec:
    kernel_path = _resolve_kernel_path(topdir)
    image_dir = topdir / "qemu_mps2_an505_images"
    flash_runtime_path = image_dir / "flash-runtime.bin"

    command = [
        "qemu-system-arm",
        "-machine",
        "mps2-an505",
        "-cpu",
        "cortex-m33",
        "-nographic",
        "-monitor",
        "none",
        "-nic",
        "none",
        "-kernel",
        str(kernel_path),
    ]

    if config.get("CONFIG_APP_BINARY_SEPARATION") == "y" and flash_runtime_path.exists():
        flash_addr = config.get("CONFIG_FLASH_START_ADDR", "0x28000000")
        command.extend(
            [
                "-device",
                f"loader,file={flash_runtime_path},addr={flash_addr},force-raw=on",
            ]
        )

    if _config_enabled(config, "CONFIG_XIP_ELF"):
        common_image = image_dir / "common.trpk"
        app_image = image_dir / "app1.trpk"
        if common_image.exists():
            command.extend(
                [
                    "-device",
                    f"loader,file={common_image},addr=0x{_xip_exec_base(common_image, config, 12):08x},force-raw=on",
                ]
            )
        if app_image.exists():
            command.extend(
                [
                    "-device",
                    f"loader,file={app_image},addr=0x{_xip_exec_base(app_image, config, 44):08x},force-raw=on",
                ]
            )

    steps = [
        PromptCommandStep("help", "help", ("filesystem_tc", "kernel_tc")),
        PromptCommandStep("free", "free"),
        PromptCommandStep("ps", "ps"),
        PromptCommandStep("pwd-root", "pwd", ("/",)),
        PromptCommandStep("ls-mnt", "ls /mnt"),
        PromptCommandStep("mkdir", "mkdir /mnt/qemu-smoke"),
        PromptCommandStep("ls-created-dir", "ls /mnt", ("qemu-smoke",)),
        PromptCommandStep("write-file", "echo qemu-mps2-smoke > /mnt/qemu-smoke.txt"),
        PromptCommandStep("read-file", "cat /mnt/qemu-smoke.txt", ("qemu-mps2-smoke",)),
        PromptCommandStep("remove-file", "rm /mnt/qemu-smoke.txt"),
        PromptCommandStep("remove-dir", "rmdir /mnt/qemu-smoke"),
        PromptCommandStep("cd-mnt", "cd /mnt"),
        PromptCommandStep("pwd-mnt", "pwd", ("/mnt",)),
        PromptCommandStep("cd-root", "cd /"),
        TestcaseStep("drivers_tc"),
        TestcaseStep("filesystem_tc"),
        TestcaseStep("kernel_tc"),
    ]

    metadata_entries = [
        ("kernel", kernel_path),
        ("kernel-runtime.elf", image_dir / "kernel-runtime.elf"),
        ("kernel-runtime-raw.bin", image_dir / "kernel-runtime-raw.bin"),
        ("kernel-runtime.bin", image_dir / "kernel-runtime.bin"),
        ("common.trpk", image_dir / "common.trpk"),
        ("app1.trpk", image_dir / "app1.trpk"),
        ("bootparam.bin", image_dir / "bootparam.bin"),
        ("flash-runtime.bin", image_dir / "flash-runtime.bin"),
    ]

    if _config_enabled(config, "CONFIG_XIP_ELF"):
        fail_markers = PlatformSpec.__dataclass_fields__["fail_markers"].default + ("MemManage", "UsageFault")
    else:
        fail_markers = PlatformSpec.__dataclass_fields__["fail_markers"].default

    return PlatformSpec(
        board="qemu-mps2-an505",
        suite_name="qemu-mps2-an505-runtime",
        qemu_command=command,
        steps=tuple(steps),
        metadata=collect_named_files(metadata_entries),
        post_boot_delay=0.5,
        prompt_settle_delay=0.1,
        send_char_delay=0.0,
        fail_markers=fail_markers,
    )
