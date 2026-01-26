from __future__ import annotations
from re import T
from bk_packager import bk_packager_linear
from pathlib import Path
import json
import shutil


curr_path = Path(__file__).parent.resolve()


def make_bin(pack_dict, workdir:Path, output_file_name:Path):
    pack_json = curr_path / "partition.json"

    frame_dict = {
        "magic": "beken",
        "crc_enable": False,
        "count": len(pack_dict),
        "section": pack_dict
    }

    with open(pack_json, "w") as f:
        json.dump(frame_dict, f, indent=4)
    packager = bk_packager_linear(workdir, pack_json, output_file_name)
    packager.pack()
    pack_json.unlink()

def make_bootloader_bin():
    pack_dict = [
        {
            "partition": "bl1_control",
            "firmware": "bl1_control.bin",
            "start_addr": 0,
            "size": 0x1000
        },
        {
            "partition": "partition",
            "firmware": "partition.bin",
            "start_addr": 0x2000,
            "size": 0x1000
        },
        {
            "partition": "bl2",
            "firmware": "bl2.bin",
            "start_addr": 0x4000,
            "size": 76 * 1024
        }
    ]
        
    workdir = curr_path.parents[2] / "output/bin/install_for_deployment"
    output_bin = workdir / "bootloader.bin"
    make_bin(pack_dict, workdir, workdir / "bootloader.bin")

    src = output_bin
    dst = workdir.parent
    shutil.copy2(src, dst)    
    

def make_kernel_bin():
    pack_dict = [
        {
            "partition": "tfm_s",
            "firmware": "tfm_s.bin",
            "start_addr": 0,
            "size": 188 * 1024
        },
        {
            "partition": "cpu0_app",
            "firmware": "cpu0_app.bin",
            "start_addr": 188 * 1024,
            "size": 1024 * 1024 * 4
        }
    ]
    workdir = curr_path.parents[2] / "output/bin/install_for_deployment"
    output_bin =workdir / "kernel.bin"
    make_bin(pack_dict, workdir, output_bin)
    orig_kernel_data = output_bin.read_bytes()
    with output_bin.open("wb") as f:
        f.write(bytes([0xFF]) * (4096 - 16))
        f.write(orig_kernel_data)

    src = output_bin
    dst = workdir.parent
    shutil.copy2(src, dst)


def make_bl1_control_bin():
    """
    Generate a 4K file filled with 0xFF, write first 64 bytes of bl2.bin to it,
    write "BEKEN" at offset 0x100, and copy to BINDIR.
    """

    # Locate workdir (output/bin/install_for_deployment)
    workdir = curr_path.parents[2] / "output/bin/install_for_deployment"
    bl1_control_bin = workdir / "bl1_control.bin"
    bl2_bin = workdir / "bl2.bin"

    # Generate a 4K file filled with 0xFF
    bl1_control_data = bytearray([0xFF] * 4096)

    # Copy first 64 bytes of bl2.bin (if present)
    if bl2_bin.exists():
        with bl2_bin.open("rb") as f:
            bl2_data = f.read(64)
            bl1_control_data[:len(bl2_data)] = bl2_data
    else:
        print("Warning: bl2.bin not found, cannot copy first 64 bytes")

    # Write "BEKEN" at offset 0x100
    bl1_control_data[0x100:0x100+5] = b"BEKEN"

    # Write to file

    # Ensure the parent directory exists before writing the file
    bl1_control_bin.parent.mkdir(parents=True, exist_ok=True)
    with open(bl1_control_bin, "wb") as f:
        f.write(bl1_control_data)
    print(f"Wrote bl1_control.bin to {bl1_control_bin}")
    # Copy to BINDIR (output/bin)
    bindir = workdir.parent
    shutil.copy2(bl1_control_bin, bindir / "bl1_control.bin")



if __name__ == "__main__":
    print("Making bl1_control bin...")
    make_bl1_control_bin()
    print("Making bootloader bin...")
    make_bootloader_bin()
    print("Making kernel bin...")
    make_kernel_bin()
