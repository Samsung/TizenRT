from pathlib import Path
import shutil
import sys

TOPDIR = Path(sys.argv[1])
WORKDIR = Path(sys.argv[2])
INSTALL_DIR2 = Path(sys.argv[3])

def copy_bootparam():
    """Copy bootparam.bin to install_for_deployment directory"""

    bootparam_bin = WORKDIR / "bootparam.bin"
    if not bootparam_bin.exists():
        print(f"Warning: {bootparam_bin} not found, skipping...")
        return

    # Create install_for_deployment directory
    install_dir = WORKDIR / "install_for_deployment"
    try:
        install_dir.mkdir(parents=True, exist_ok=True)
        INSTALL_DIR2.mkdir(parents=True, exist_ok=True)
        print(f"Install directory: {install_dir}")
    except Exception as e:
        print(f"Error: Failed to create directory {install_dir}: {e}")
        raise

    # Copy bootparam.bin
    dest_file = install_dir / "bootparam.bin"
    try:
        shutil.copy2(bootparam_bin, dest_file)
        shutil.copy2(bootparam_bin, INSTALL_DIR2 / "bootparam.bin")
        print(f"Successfully copied: {bootparam_bin.name} -> {dest_file}")
    except Exception as e:
        print(f"Error: Failed to copy {bootparam_bin} to {dest_file}: {e}")
        raise

def copy_board_metadata():
    """Copy board_metadata.txt to install_for_deployment directory"""

    # board_metadata.txt is in build/configs/<board>/ directory
    # topdir points to TizenRT/os, so we need to go up and into build
    board_metadata = TOPDIR.parent / "build/configs/bk7239n/board_metadata.txt"
    if not board_metadata.exists():
        print(f"Warning: {board_metadata} not found, skipping...")
        return

    # Create install_for_deployment directory
    install_dir = WORKDIR / "install_for_deployment"
    try:
        install_dir.mkdir(parents=True, exist_ok=True)
        INSTALL_DIR2.mkdir(parents=True, exist_ok=True)
        print(f"Install directory: {install_dir}")
    except Exception as e:
        print(f"Error: Failed to create directory {install_dir}: {e}")
        raise

    # Copy board_metadata.txt
    dest_file = install_dir / "board_metadata.txt"
    try:
        shutil.copy2(board_metadata, dest_file)
        shutil.copy2(board_metadata, INSTALL_DIR2 / "board_metadata.txt")
        print(f"Successfully copied: {board_metadata.name} -> {dest_file}")
    except Exception as e:
        print(f"Error: Failed to copy {board_metadata} to {dest_file}: {e}")
        raise

def copy_configfile_for_makessheader():
    """Copy config and bininfo files to install_for_deployment directory"""
    # Create install_for_deployment directory
    install_dir = WORKDIR / "install_for_deployment"
    try:
        install_dir.mkdir(parents=True, exist_ok=True)
        INSTALL_DIR2.mkdir(parents=True, exist_ok=True)
        print(f"Install directory: {install_dir}")
    except Exception as e:
        print(f"Error: Failed to create directory {install_dir}: {e}")
        raise
    # Copy config and bininfo files
    config_file = TOPDIR / ".config"
    bininfo_file = TOPDIR / ".bininfo"
    if not config_file.exists():
        print(f"Warning: {config_file} not found, skipping...")
        return
    if not bininfo_file.exists():
        print(f"Warning: {bininfo_file} not found, skipping...")
        return
    shutil.copy2(config_file, install_dir / ".config")
    shutil.copy2(config_file, INSTALL_DIR2 / ".config")
    shutil.copy2(bininfo_file, install_dir / ".bininfo")
    shutil.copy2(bininfo_file, INSTALL_DIR2 / ".bininfo")
    print(f"Successfully copied: {config_file.name} -> {install_dir / '.config'}")
    print(f"Successfully copied: {bininfo_file.name} -> {install_dir / '.bininfo'}")

def copy_partition_json():
    """Copy partition.json to install_for_deployment directory"""
    partition_json = WORKDIR / "partition.json"
    if not partition_json.exists():
        print(f"Warning: {partition_json} not found, skipping...")
        return
    install_dir = WORKDIR / "install_for_deployment"
    try:
        install_dir.mkdir(parents=True, exist_ok=True)
        INSTALL_DIR2.mkdir(parents=True, exist_ok=True)
        print(f"Install directory: {install_dir}")
    except Exception as e:
        print(f"Error: Failed to create directory {install_dir}: {e}")
        raise
    shutil.copy2(partition_json, install_dir / "partition.json")
    shutil.copy2(partition_json, INSTALL_DIR2 / "partition.json")
    print(f"Successfully copied: {partition_json.name} -> {install_dir / 'partition.json'}")


if __name__ == "__main__":
    print("copying build files to")
    copy_bootparam()
    copy_board_metadata()
    copy_configfile_for_makessheader()
    # copy_partition_json()
