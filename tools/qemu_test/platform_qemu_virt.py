#!/usr/bin/env python3

from __future__ import annotations

from pathlib import Path

from platform_base import PlatformSpec, PromptCommandStep, TestcaseStep, collect_named_files


def _config_enabled(config: dict[str, str], key: str) -> bool:
    return config.get(key) == "y"


def _config_value(config: dict[str, str], key: str, default: str) -> str:
    value = config.get(key)
    return value if value else default


def _ensure_disk_image(topdir: Path, config: dict[str, str]) -> None:
    if not _config_enabled(config, "CONFIG_QEMU_VIRT_VIRTIO_BLK"):
        return

    disk_path = topdir / "disk.raw"
    if not disk_path.exists():
        with disk_path.open("wb") as handle:
            handle.truncate(64 * 1024 * 1024)


def build_platform(topdir: Path, config: dict[str, str]) -> PlatformSpec:
    qemu_flash = topdir / "qemu_flash.bin"
    if not qemu_flash.exists():
        raise RuntimeError(f"Flash image not found: {qemu_flash}")

    _ensure_disk_image(topdir, config)

    command = [
        "qemu-system-arm",
        "-machine",
        "virt,virtualization=off,gic-version=2",
        "-cpu",
        _config_value(config, "CONFIG_QEMU_VIRT_CPU_MODEL", "cortex-a15"),
        "-m",
        f"{_config_value(config, 'CONFIG_QEMU_VIRT_MEMORY_MB', '64')}M",
        "-nographic",
        "-monitor",
        "none",
        "-net",
        "none",
        "-drive",
        f"if=pflash,format=raw,file={qemu_flash}",
    ]

    second_flash = topdir / "qemu_flash2.bin"
    if _config_enabled(config, "CONFIG_SECOND_FLASH_PARTITION") and second_flash.exists():
        command.extend(["-drive", f"if=pflash,format=raw,file={second_flash}"])

    disk_path = topdir / "disk.raw"
    if _config_enabled(config, "CONFIG_QEMU_VIRT_VIRTIO_BLK"):
        command.extend(
            [
                "-drive",
                f"file={disk_path},format=raw,if=none,id=blk0",
                "-device",
                "virtio-blk-device,drive=blk0,bus=virtio-mmio-bus.0",
            ]
        )

    if _config_enabled(config, "CONFIG_SMP"):
        command.extend(["-smp", _config_value(config, "CONFIG_SMP_NCPUS", "2")])

    help_tokens: tuple[str, ...] = ("virtio_blk_test",) if _config_enabled(config, "CONFIG_QEMU_VIRT_VIRTIO_BLK") else ()

    steps = [
        PromptCommandStep("help", "help", help_tokens),
        PromptCommandStep("free", "free"),
        PromptCommandStep("ps", "ps"),
        PromptCommandStep("pwd-root", "pwd", ("/",)),
        PromptCommandStep("ls-mnt", "ls /mnt"),
        PromptCommandStep("mkdir", "mkdir /mnt/qemu-smoke"),
        PromptCommandStep("ls-created-dir", "ls /mnt", ("qemu-smoke",)),
        PromptCommandStep("write-file", "echo qemu-virt-smoke > /mnt/qemu-smoke.txt"),
        PromptCommandStep("read-file", "cat /mnt/qemu-smoke.txt", ("qemu-virt-smoke",)),
        PromptCommandStep("remove-file", "rm /mnt/qemu-smoke.txt"),
        PromptCommandStep("remove-dir", "rmdir /mnt/qemu-smoke"),
        PromptCommandStep("cd-mnt", "cd /mnt"),
        PromptCommandStep("pwd-mnt", "pwd", ("/mnt",)),
        PromptCommandStep("cd-root", "cd /"),
    ]

    if _config_enabled(config, "CONFIG_QEMU_VIRT_VIRTIO_BLK"):
        steps.append(
            PromptCommandStep(
                "virtio-blk",
                "virtio_blk_test",
                ("VIRTIO_BLK_TEST: PASS",),
                timeout=60,
            )
        )

    steps.extend(
        [
            TestcaseStep("drivers_tc"),
            TestcaseStep("filesystem_tc"),
            TestcaseStep("kernel_tc"),
        ]
    )

    metadata = collect_named_files(
        [
            ("qemu_flash.bin", qemu_flash),
            ("qemu_flash2.bin", second_flash),
            ("disk.raw", disk_path),
        ]
    )

    return PlatformSpec(
        board="qemu-virt",
        suite_name="qemu-virt-runtime",
        qemu_command=command,
        steps=tuple(steps),
        metadata=metadata,
    )
