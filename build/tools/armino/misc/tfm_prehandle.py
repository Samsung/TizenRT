from __future__ import annotations

import shutil
import sys
import re
from pathlib import Path
import os


def copy_files(postfix: str, src_dir: Path, dst_dir: Path):
    # logging.debug(f'copy *{postfix} from {src_dir}')
    # logging.debug(f'to {dst_dir}')
    for f in src_dir.iterdir():
        if f.suffix == postfix:
            shutil.copy(f, dst_dir / f.name)


def install_configs(cfg_dir: Path, install_dir: Path, flash_encrypt_enable: int):
    # logging.debug(f'install configs from: {cfg_dir}')
    # logging.debug(f'to: {install_dir}')
    # logging.debug(f"flash_encrypt_enable value: {flash_encrypt_enable}, type: {type(flash_encrypt_enable)}")
    if not cfg_dir.exists():
        return

    copy_files(".bin", cfg_dir, install_dir)
    copy_files(".json", cfg_dir, install_dir)
    copy_files(".pem", cfg_dir, install_dir)
    copy_files(".csv", cfg_dir, install_dir)

    if (cfg_dir / "key").exists():
        copy_files(".pem", cfg_dir / "key", install_dir)
    if (cfg_dir / "csv").exists():
        copy_files(".csv", cfg_dir / "csv", install_dir)
    if (cfg_dir / "regs").exists():
        copy_files(".csv", cfg_dir / "regs", install_dir)

    if flash_encrypt_enable == 1:
        sec_csv = install_dir / "security.csv"
        if sec_csv.exists():
            lines = []
            with sec_csv.open("r", encoding="utf-8") as f:
                for line in f:
                    if line.strip().startswith("flash_aes_type,"):
                        lines.append("flash_aes_type,FIXED\n")
                    else:
                        lines.append(line)
            with sec_csv.open("w", encoding="utf-8") as f:
                f.writelines(lines)
        
def _gen_partitions_gen_hdr_file(workdir: Path):
    """
    Generate partitions_gen.h with only fixed configuration partitions.
    Fixed partitions are those that are always at fixed locations:
    - bl1_control, boot_flag, partition, manifest, bl2
    - sys_its, sys_ps, ss
    - rf, net, easyflash, bootparam
    
    Dynamic partitions (TFM_S, CPU0_APP, APP1, APP2, etc.) are read from partition.bin at runtime.
    
    Note: This function should not affect the generation of partitions.csv or partitions_partition.h.
    It only reads partitions_partition.h and generates partitions_gen.h.
    """
    partitions_partition_h = workdir / "partitions_partition.h"
    partitions_gen_h = workdir / "partitions_gen.h"
    
    if not partitions_partition_h.exists():
        return  # partitions_partition.h not generated yet
    
    try:
        # Set of fixed partition macro prefixes (in uppercase, as they appear in macros)
        # These match the partition names in partitions_partition.h
        fixed_partition_prefixes = {
            'BL1_CONTROL',
            'BOOT_FLAG',
            'PARTITION',  # Exact match only (not PARTITION_B)
            'PRIMARY_MANIFEST',
            'SECONDARY_MANIFEST',
            'BL2',  # Exact match only (not BL2_B)
            'SYS_ITS',
            'SYS_PS',
            'SS',
            'RF',
            'NET',
            'EASYFLASH',
            'BOOTPARAM'
        }
        
        # Map from partition name (as in PARTITION_MAP) to macro prefix
        partition_name_to_prefix = {
            'bl1_control': 'BL1_CONTROL',
            'boot_flag': 'BOOT_FLAG',
            'partition': 'PARTITION',
            'primary_manifest': 'PRIMARY_MANIFEST',
            'secondary_manifest': 'SECONDARY_MANIFEST',
            'bl2': 'BL2',
            'sys_its': 'SYS_ITS',
            'sys_ps': 'SYS_PS',
            'ss': 'SS',
            'rf': 'RF',
            'net': 'NET',
            'easyflash': 'EASYFLASH',
            'bootparam': 'BOOTPARAM'
        }
        
        # Read partitions_partition.h line by line
        with open(partitions_partition_h, 'r', encoding='utf-8') as f:
            lines = f.readlines()
        
        output_lines = []
        partition_map_lines = []
        i = 0
        
        # Copy header until we reach the first #define
        while i < len(lines) and not lines[i].strip().startswith('#define'):
            output_lines.append(lines[i])
            i += 1
        
        # Add KB and MB macro definitions (needed for linker scripts)
        # These are used in flash_layout.h for TOTAL_RAM_SIZE
        output_lines.append('#define KB(size)                                      ((size) << 10)\n')
        output_lines.append('#define MB(size)                                      ((size) << 20)\n')
        output_lines.append('\n')
        
        # Process partition definitions
        while i < len(lines):
            line = lines[i]
            
            # Check if this is a fixed configuration flag
            if 'CONFIG_TFM_S_JUMP_TO_TFM_NS' in line or \
               'CONFIG_TFM_S_JUMP_TO_CPU0_APP' in line or \
               'CPU_VECTOR_ALIGN_SZ' in line:
                output_lines.append(line)
                i += 1
                continue
            
            # Check if this is a partition definition line
            # Pattern: #define CONFIG_<PARTITION>_PHY_PARTITION_OFFSET
            match = re.search(r'#define CONFIG_([A-Z0-9_]+)_PHY_PARTITION_OFFSET', line)
            if match:
                prefix = match.group(1)
                
                # Check if this is a fixed partition
                # Handle special cases: PARTITION and BL2 need exact match (not PARTITION_B or BL2_B)
                is_fixed = False
                if prefix == 'PARTITION':
                    # Only match exact PARTITION, not PARTITION_B
                    is_fixed = 'PARTITION' in fixed_partition_prefixes
                elif prefix == 'BL2':
                    # Only match exact BL2, not BL2_B
                    is_fixed = 'BL2' in fixed_partition_prefixes
                else:
                    # For other partitions, check if prefix is in the set
                    is_fixed = prefix in fixed_partition_prefixes
                
                if is_fixed:
                    # Collect all definitions for this fixed partition
                    output_lines.append(line)
                    i += 1
                    # Continue collecting related definitions until next partition or PARTITION_MAP
                    while i < len(lines):
                        next_line = lines[i]
                        # Stop if we hit another partition definition
                        if next_line.strip().startswith('#define CONFIG_') and \
                           '_PHY_PARTITION_OFFSET' in next_line:
                            break
                        # Stop if we hit PARTITION_MAP
                        if next_line.strip().startswith('#define PARTITION_MAP'):
                            break
                        # Include lines that belong to current partition
                        # Check if the line contains the partition prefix
                        # or is empty/comments
                        line_contains_prefix = False
                        if prefix == 'BL2':
                            # Match BL2 but not BL2_B (BL2_B is handled separately)
                            line_contains_prefix = ('CONFIG_BL2_' in next_line and 'CONFIG_BL2_B_' not in next_line)
                        elif prefix == 'PARTITION':
                            # Match PARTITION but not PARTITION_B
                            line_contains_prefix = ('CONFIG_PARTITION_' in next_line and 'CONFIG_PARTITION_B_' not in next_line)
                        else:
                            # For other partitions, check if prefix appears in the line
                            line_contains_prefix = f'CONFIG_{prefix}_' in next_line
                        
                        if line_contains_prefix or \
                           next_line.strip() == '' or \
                           next_line.strip().startswith('//'):
                            output_lines.append(next_line)
                            i += 1
                        else:
                            # If line doesn't contain prefix and is not empty/comment, it might be
                            # the end of this partition's definitions
                            break
                else:
                    # Skip dynamic partition, find where it ends
                    i += 1
                    while i < len(lines):
                        next_line = lines[i]
                        if next_line.strip().startswith('#define CONFIG_') and \
                           '_PHY_PARTITION_OFFSET' in next_line:
                            break
                        if next_line.strip().startswith('#define PARTITION_MAP'):
                            break
                        i += 1
            elif line.strip().startswith('#define PARTITION_MAP'):
                # Extract PARTITION_MAP and filter to only fixed partitions
                partition_map_lines.append(line)  # #define PARTITION_MAP { \
                i += 1
                # Collect all entries until closing brace
                while i < len(lines):
                    next_line = lines[i]
                    # Check if this is a partition entry
                    # Pattern: {"partition_name", CONFIG_XXX_PHY_PARTITION_OFFSET, CONFIG_XXX_PHY_PARTITION_SIZE}, \
                    entry_match = re.search(r'\{"([^"]+)"', next_line)
                    if entry_match:
                        partition_name = entry_match.group(1)
                        # Only include fixed partitions
                        if partition_name in partition_name_to_prefix:
                            partition_map_lines.append(next_line)
                    elif next_line.strip() == '}':
                        partition_map_lines.append(next_line)
                        i += 1
                        break
                    else:
                        # Skip this line (might be empty or comment)
                        pass
                    i += 1
            else:
                # Not a partition definition line, skip it
                i += 1
        
        # Append PARTITION_MAP to output
        if partition_map_lines:
            output_lines.append('\n')
            output_lines.extend(partition_map_lines)
        
        # Write partitions_gen.h
        with open(partitions_gen_h, 'w', encoding='utf-8') as f:
            f.writelines(output_lines)
    except Exception as e:
        # If there's any error reading or processing partitions_partition.h,
        # log it but don't fail the entire build process
        # This ensures that partitions.csv and partitions_partition.h generation
        # are not affected by issues in this function
        import logging
        logging.warning(f"Failed to generate partitions_gen.h: {e}")
        # Don't raise the exception - let the build continue
        pass

def pre_handle(middleware_dir: Path, workdir: Path, flash_encrypt_enable: int):
    from partitions_adapter import adapt_partitions

    install_configs(middleware_dir, workdir, flash_encrypt_enable)
    config_file = Path(__file__).parents[4] / "os/.config"
    output_csv = workdir / "partitions.csv"
    adapt_partitions(config_file, output_csv) # TODO use preset partitions csv now.
    tfm_prehandle_script = Path(__file__).parent.parent / "tfm_prehandle/main.py"
    os.chdir(workdir)
    ret = os.system(f"python3 -B {tfm_prehandle_script} gen all")
    if ret != 0:
        raise RuntimeError("gen config failed")
        
    # Copy partitions_partition.h to partitions_gen_ns.h for non-secure environment
    # partitions_gen_ns.h contains all partition macros (both fixed and dynamic partitions)
    # This is needed by files in non-secure environment (e.g., middleware/driver/flash/flash_driver.c)
    # that need access to all partition information at compile time, including dynamic partitions
    # like PRIMARY_TFM_S, PRIMARY_ALL, SECONDARY_ALL, APP1, USERFS, etc.
    # Note: partitions_gen.h only contains fixed partitions for secure environment use
    shutil.copy(workdir / "partitions_partition.h", workdir / "partitions_gen_ns.h")
    
    # Generate partitions_gen.h with only fixed configuration partitions
    # Fixed partitions are those that are always at fixed locations (bl1, boot_flag, partition, 
    # manifest, bl2, bl2_B, partition_B, sys_its, sys_ps, ss, rf, net, easyflash, bootparam)
    # Dynamic partitions (TFM_S, CPU0_APP, APP1, APP2, etc.) are read from partition.bin at runtime
    # Note: This function is called after partitions_partition.h is generated and copied to partitions_gen_ns.h
    # It should not affect the generation of partitions.csv or partitions_partition.h
    _gen_partitions_gen_hdr_file(workdir)


if __name__ == "__main__":
    middleware_dir = Path(sys.argv[1]).absolute()
    workdir = Path(sys.argv[2]).absolute()
    flash_encrypt_enable = int(sys.argv[3])
    assert middleware_dir.exists()
    assert workdir.exists()
    pre_handle(middleware_dir, workdir, flash_encrypt_enable)
