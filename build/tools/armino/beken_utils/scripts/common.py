#!/usr/bin/env python3
import re
import logging
import subprocess
import os
import shutil

s_base_addr = 0
SZ_16M = 0x1000000
CRC_SIZE = 0X02
CRC_PAKEIT = 0X20

def ceil_align(value, alignment):
    return (((value + alignment - 1) // alignment) * alignment)

def floor_align(value, alignment):
    return ((value // alignment) * alignment)

def p2v(addr):
    return (addr - s_base_addr)


def hex2int(str):
    return int(str, base=16) % (2**32)


def decimal2int(str):
    return int(str, base=10) % (2**32)


def crc_size(size, crc_en):
    if crc_en == True:
        return (((size + 31) >> 5) * 34)
    else:
        return size

def virtual2phy(addr, crc_en):
    if crc_en == True:
        return ((addr % 32) + ((addr >> 5) * 34))
    else:
        return addr

def phy2virtual(addr, crc_en):
    if crc_en == True:
        return ((addr % 34) + ((addr // 34) * 32))
    else:
        return addr

def crc_addr(addr):
    return crc_size(addr)


def size2int(str):
    size_str = re.findall(r"\d+", str)
    size = decimal2int(size_str[0])

    unit = re.findall(r"[k|K|m|M|g|G|b|B]+", str)
    if (unit[0] == 'b') or (unit[0] == 'B'):
        return size
    if (unit[0] == 'k') or (unit[0] == 'K'):
        return size * (1 << 10)
    elif (unit[0] == 'm') or (unit[0] == 'M'):
        return size * (1 << 20)
    elif (unit[0] == 'g') or (unit[0] == 'G'):
        return size * (1 << 30)
    else:
        logging.error(f'invalid size unit {unit[0]}, must be "b/B/k/K/m/M/g/G"')
        exit(1)


def is_out_of_range(addr, size):
    if ((addr + size) >= SZ_16M):
        return True
    return False

def crc16(data : bytearray, offset , length):
    if data is None or offset < 0 or offset > len(data)- 1 and offset+length > len(data):
        return 0
    crc = 0xFFFFFFFF
    for i in range(0, length):
        crc ^= data[offset + i] << 8
        for j in range(0,8):
            if (crc & 0x8000) > 0:
                crc =(crc << 1) ^ 0x8005 #for beken poly:8005
            else:
                crc = crc << 1
    return crc & 0xFFFF

def run_cmd(cmd):
    p = subprocess.Popen(cmd, shell=True)
    ret = p.wait()
    if (ret):
        logging.error(f'failed to run "{cmd}"')
        exit(1)

def run_cmd_not_check_ret(cmd):
    p = subprocess.Popen(cmd, shell=True)
    p.wait()

def pattern_match(string, pattern):
    match = re.search(pattern, string)
    if match:
        return True 
    else:
        return False

def int2hexstr2(v):
    return (f'%04x' %(v))

def int2hexstr4(v):
    return (f'%08x' %(v))

def parse_bool(v):
    if v == 'TRUE':
        return True
    return False

def clear_dir(directory):
    for filename in os.listdir(directory):
        file_path = os.path.join(directory, filename)
        try:
            if os.path.isfile(file_path) or os.path.islink(file_path):
                os.unlink(file_path)
            elif os.path.isdir(file_path):
                clear_files_in_directory(file_path)
                pass
        except Exception as e:
            print(f'Failed to delete {file_path}. Reason: {e}')

def copy_files(src_directory, dest_directory):
    os.makedirs(dest_directory, exist_ok=True)
    for filename in os.listdir(src_directory):
        src_file = os.path.join(src_directory, filename)
        if os.path.isfile(src_file):
            shutil.copy(src_file, dest_directory)

def empty_line(f):
    line = f'\r\n'
    f.write(line)

def get_script_dir():
    script_dir = os.path.abspath(__file__)
    script_dir = os.path.dirname(script_dir)
    return script_dir
