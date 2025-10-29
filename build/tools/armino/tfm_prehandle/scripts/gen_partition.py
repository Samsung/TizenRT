#!/usr/bin/env python3

import logging

from .gen_license import get_license
from .common import *
from .partition import *

def gen_partitions_hdr_file(partition, out_partition_hdr_file):
    f = open(out_partition_hdr_file, 'w+')

    logging.debug(f'Create partition hdr file: {out_partition_hdr_file}')
    f.write(get_license())

    line = f'#include "security.h"\r\n'
    f.write(line)
 
    line = f'#define %-45s %s' %("KB(size)", "((size) << 10)\r\n")
    f.write(line)
    line = f'#define %-45s %s' %("MB(size)", "((size) << 20)\r\n\r\n")
    f.write(line)

    macro_name = f'CONFIG_TFM_S_JUMP_TO_TFM_NS'
    if partition.tfm_ns_exists == True:
        line = f'#define %-45s %d\r\n' %(macro_name, 1)
    else:
        line = f'#define %-45s %d\r\n' %(macro_name, 0)
    f.write(line)

    macro_name = f'CONFIG_TFM_S_JUMP_TO_CPU0_APP'
    if partition.primary_cpu0_app_exists == True:
        line = f'#define %-45s %d\r\n' %(macro_name, 1)
    else:
        line = f'#define %-45s %d\r\n' %(macro_name, 0)
    f.write(line)

    macro_name = f'CPU_VECTOR_ALIGN_SZ'
    line = f'#define %-45s %d\r\n' %(macro_name, partition.cpu_vector_align_bytes)
    f.write(line)

    partition_struct_array = f"#define PARTITION_MAP {{ \\\r\n"
    for partition in partition.partitions:

        partition_name = partition.partition_name
        partition_struct_array +=  f"    {{\"{partition_name}\""
        partition_name = partition_name.upper()
        partition_name = partition_name.replace(' ', '_')

        logging.debug(f'generate constants for partition {partition_name}')

        macro_name = f'CONFIG_{partition_name}_PHY_PARTITION_OFFSET'
        line = f'#define %-45s 0x%x\r\n' %(macro_name, partition.partition_offset)
        f.write(line)
        macro_name = f'CONFIG_{partition_name}_PHY_PARTITION_SIZE'
        line = f'#define %-45s 0x%x\r\n' %(macro_name, partition.partition_size)
        f.write(line)
        partition_struct_array += f""", CONFIG_{partition_name}_PHY_PARTITION_OFFSET, CONFIG_{partition_name}_PHY_PARTITION_SIZE}}, \\\r\n"""
        if partition.is_app_partition() and partition.bin_name != None:
            macro_name = f'CONFIG_{partition_name}_PHY_CODE_START'
            line = f'#define %-45s 0x%x\r\n' %(macro_name, partition.phy_code_offset)
            f.write(line)


            if (partition.bin_hdr_size > 0):
                macro_name = f'CONFIG_{partition_name}_PHY_HDR_SIZE'
                line = f'#define %-45s 0x%x\r\n' %(macro_name, partition.bin_hdr_size)
                f.write(line)

            if (partition.bin_tail_size > 0):
                macro_name = f'CONFIG_{partition_name}_PHY_TAIL_SIZE'
                line = f'#define %-45s 0x%x\r\n' %(macro_name, partition.bin_tail_size)
                f.write(line)

            if (partition.vir_partition_size > 0):
                macro_name = f'CONFIG_{partition_name}_VIRTUAL_PARTITION_SIZE'
                line = f'#define %-45s 0x%x\r\n' %(macro_name, partition.vir_partition_size)
                f.write(line)

                macro_name = f'CONFIG_{partition_name}_VIRTUAL_CODE_START'
                line = f'#define %-45s 0x%x\r\n' %(macro_name, partition.vir_code_offset)
                f.write(line)

                macro_name = f'CONFIG_{partition_name}_VIRTUAL_CODE_SIZE'
                line = f'#define %-45s 0x%x\r\n' %(macro_name, partition.vir_code_size)
                f.write(line)

        empty_line(f)
    partition_struct_array += f"}}\r\n"
    f.write(partition_struct_array)
    f.flush()
    f.close()

def gen_partitions_layout_file(partition, out_partition_layout_file):
    f = open(out_partition_layout_file, 'w+')

    logging.debug(f'Create partition layout file: {out_partition_layout_file}')
    line = f'%-24s   %-10s   %-8s\r\n' %("name", "offset", "size")
    f.write(line)
    logging.debug(f'{line}')
    line = f'------------------------   ----------   ----------------\r\n'
    f.write(line)

    for partition in partition.partitions:

        partition_name = partition.partition_name
        line = f'%-24s   0x%-8x   0x%-6x(%uK)\r\n' %(partition_name, partition.partition_offset, partition.partition_size, partition.partition_size>>10)
        f.write(line)
        logging.debug(f'{line}')

    f.flush()
    f.close()
