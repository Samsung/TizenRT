# bk_loader
#
# Copyright (c) BekenCorp. All rights reserved.
#

from pickletools import stringnl_noescape_pair
import struct
import sys
import platform
import os
import glob
import serial
from enum import Enum
from dataclasses import dataclass, field
from typing import List
from ..bk_log.log_utils import *
import json
#import crc32c
#import zlib
#from numba import njit
#from concurrent.futures import ThreadPoolExecutor
#import crc32_cython
#from numba import jit

@dataclass
class user_operate:
    name:str=''
    mode:str=''
    permission:str=''
    start_addr:str=''
    end_addr:str=''
    byte_len:str=''
    data:str=''
    data_type:str=''
    encrypt:str=''
    status:str=''
    big_endian:bool=False
    reverse:bool=False

@dataclass
class efuse_info:
    name:str=''
    mode:str=''
    byte_addr:str=''
    end_addr:str=''
    length:str=''
    data:str=''
    status:str=''
    
@dataclass
class safe_data:
    write_safe_config: bool = False
    user_operate_enable:bool = False
    efuse_enable:bool = False
    security_ctrl_enable:bool = False
    security_data_enable:bool = False
    user_operate_list: List[user_operate] = field(default_factory=list)
    efuse_info_list: List[efuse_info] = field(default_factory=list)
    security_data_list: List[user_operate] = field(default_factory=list)
    security_ctrl:List[str] = field(default_factory=list)


@dataclass
class BinInfo:
    bCheck: bool = True
    bDataPort: bool = True
    iLinkCheckType: int = 0
    sFilePath: str = ''
    iStartAddress: int = 0
    iFileCRC: int = 0
    iFileLength: int = 0
    iPartition_start: int = 0
    iPartition_size: int = 0
    bOtpData: bool = False
    pfile: bytes = b''

@dataclass
class DownloadInfo:
    work_baudrate: int = 2000000
    vecbinInfo: List[BinInfo] = field(default_factory=list)
    safeinfo: safe_data = field(default_factory=safe_data)
    aes_key: str = ''  #aes key string, will replace the aes key in safe json if set
    otp_info: str = '' #string contains otp writing information
    otp_pm: str = ''  #string contains otp permission writing information
    efuse_info: str = '' #string contains efuse writing information
    bDownloadBin: bool = False
    bAllFlash: bool = False   #all chip operation such as chip erase in download process, chip erase in erase process, read all flash in read process
    bSecureboot: bool = False #enable secure boot
    
    bDownloadDeviceID: bool = False
    bDownloadDeviceName: bool = False
    bEnableBigEndian: bool = False
    device_id: bytes = field(default_factory=lambda: bytes([0]*6))
    device_id_Addr: int = 0
    device_id_length: int = 0
    
    device_name: bytes = field(default_factory=lambda: bytes(32))
    device_name_Addr: int = 0
    device_name_length: int = 0  
    reverse_content: bool = False
    otp_bigendian: bool = False
    pre_erase: int = 1
    check_words: str = ''
    

@dataclass
class ReadInfo:
    work_baudrate: int = 500000
    vecbinInfo: List[BinInfo] = field(default_factory=list)    
    bAllFlash: bool = False        
    read_uid: bool = False
    read_efuse: bool = False
    read_otp: str = ''
    bConsole: bool = False
    reverse_content: bool = False
    read_path: str = ''
    read_otp_efuse_uid: str = ''

@dataclass
class EraseInfo:
    work_baudrate: int = 1000000
    vecbinInfo: List[BinInfo] = field(default_factory=list)    
    bAllFlash: bool = False   
    
@dataclass
class AuthorizeInfo:
    work_baudrate: int = 1000000
    aes_key: str = ''

class PM_CPU_FREQ_LIST:
    PM_CPU_FREQ_26M = 1
    PM_CPU_FREQ_40M = 2
    PM_CPU_FREQ_80M = 3
    PM_CPU_FREQ_120M = 4
    PM_CPU_FREQ_240M = 5
    PM_CPU_FREQ_320M = 6
    PM_CPU_FREQ_480M = 7
    PM_CPU_FREQ_DEFAULT = 8

class PM_FLASH_FREQ_LIST:
    PM_FLASH_FREQ_26M = 1
    PM_FLASH_FREQ_40M = 2
    PM_FLASH_FREQ_80M = 3
    
class PROGRAMING_MODE_LIST:
    PROGRAMING_INVALID = 0
    PROGRAMING_DBUS = 1
    PROGRAMING_CBUS = 2
    PROGRAMING_ALL = 3

class FLASH_SIZE_DEF:
    SIZE_PAGE = 0x100
    SIZE_4K = 0x1000
    SIZE_32K = 0x8000
    SIZE_64K = 0x10000

class FLASH_ERASE_CMD:
    SIZE_4K = 0x20
    SIZE_32K = 0x52
    SIZE_64K = 0xd8

class FLASH_ERASE_CMD_EXT:
    SIZE_4K = 0x21
    SIZE_32K = 0x5c
    SIZE_64K = 0xdc

class LINKCHECK(Enum):
    BOOTROM = 0,
    BL2 = 1,
    BOOTROM_PRE = 2,
    BOOTROM_EXT = 3

class BIN_TYPE(Enum):
    TYPE_NORMAL = 0,
    TYPE_OTA = 1,   #'beken...'
    TYPE_BL1_ALL_BIN=2,  #'BL1DLV10'
    TYPE_BL2_APP_BIN=3, #'BKDLV10.'
    TYPE_SECURE_FIX=4, #'BLXDLV10'
    TYPE_OUTTER_FLASH=5, #'QSPI_DL.'
    TYPE_NORMAL_FIXED=6, #'BKFIXED.'
    TYPE_BL2_OTA=7, #'BL2DLV10'

class RESET_TYPE(Enum):
    DTR_RTS = 0,
    CMD_ASCII = 1,
    CMD_HEX = 2,
    MULTI = 3


class TASK_TYPE(Enum):
    DOWNLOAD = 0,
    ERASE = 1,
    READ = 2,
    TOOLS = 3


def BFD(v,bs,bl):
    return (v&((1<<(bl))-1))<<(bs)

def BIT(n):
    return 1<<n

def get_os_type():
    os_type = platform.system()
    if os_type == "Windows":
        return "Windows"
    elif os_type == "Linux":
        if is_raspberry_pi():
            return "Raspberry Pi"
        return "Linux"
    elif os_type == "Darwin":
        return "MacOS"
    else:
        return "Unknown"

def is_raspberry_pi():
    try:
        with open("/proc/cpuinfo", "r") as f:
            cpuinfo = f.read().lower()
            return "raspberry pi" in cpuinfo
    except FileNotFoundError:
        return False
    

#crc32_table = np.zeros(256, dtype=np.uint32)
# for i in range(256):
#     c = i
#     for j in range(8):
#         if c & 1:
#             c = 0xEDB88320 ^ (c >> 1)
#         else:
#             c = c >> 1
#     crc32_table[i] = c

# def crc32_ver2(crc, buf):
#     buf = np.frombuffer(buf, dtype=np.uint8)
#     for c in buf:
#         crc = (crc >> 8) ^ crc32_table[(crc ^ c) & 0xFF]
#     return crc

crc32_table=[0] * 256
def make_crc32_table():
    global crc32_table
    if crc32_table[255] != 0:
        return
    
    for i in range(256):
        c = i
        for bit in range(8):
            if c & 1:
                c = (c >> 1) ^ 0xEDB88320
            else:
                c = c >> 1
        crc32_table[i] = c

def crc32_ver2(crc, buf):
    make_crc32_table()
    for byte in buf:
        crc = (crc >> 8) ^ crc32_table[(crc ^ byte) & 0xFF]
    return crc

def crc32bytes(data: bytearray):
    '''
    data length must be 0x20
    '''
    #print(f'input data {data}')
    crc = 0xffff
    for d in data:
        for idx in range(8):
            bit = 7-idx
            dbit = ((d>>bit)&0x1)&0xffff
            b15 = ((crc>>15)&0x1)&0xffff
            crc <<= 1
            crc = crc&0xffff
            crc |= dbit
            crc = crc&0xffff
            crc ^= ((dbit<<15)|(dbit<<2))
            crc = crc&0xffff
            crc ^= ((b15<<15)|(b15<<2)|(b15<<0))
            crc = crc&0xffff
    
    crcdata = bytearray()
    crcdata = data+bytes([(crc>>8)&0xff])
    crcdata = crcdata+bytes([crc&0xff])
    #print(f'output data {crcdata}')
    return crcdata




def load_normal_bin(srcbuf:bytes)->BinInfo:
    bin_info = BinInfo()
    bin_info.bCheck = True
    bin_info.pfile = srcbuf
    buf_len = len(srcbuf)
    padding_len = buf_len%0x100
    if padding_len:
        bin_info.pfile += b'\xff'*(0x100-padding_len)
        buf_len += (0x100-padding_len)
    bin_info.iFileLength = buf_len
    bin_info.iFileCRC = crc32_ver2(0xffffffff, bin_info.pfile)
    bin_info.iLinkCheckType = LINKCHECK.BOOTROM
    return bin_info

def load_ota_bin(srcbuf:bytes):
    bin_list = []
    bin_num = srcbuf[18]<<8+srcbuf[19]  
    for idx in range(0,bin_num):
        bin_info = BinInfo()
        bin_info.bCheck = True
        pos = 0x20+0x20*idx
        file_len = srcbuf[pos]<<24+srcbuf[pos+1]<<16+srcbuf[pos+2]<<8+srcbuf[pos+3]
        pos = 0x20+0x20*idx+4
        image_offset = srcbuf[pos]<<24+srcbuf[pos+1]<<16+srcbuf[pos+2]<<8+srcbuf[pos+3]
        pos = 0x20+0x20*idx+8
        bin_info.iStartAddress = srcbuf[pos]<<24+srcbuf[pos+1]<<16+srcbuf[pos+2]<<8+srcbuf[pos+3]
        bin_info.pfile = srcbuf[image_offset:image_offset+file_len]
        padding_len = file_len%0x100
        if padding_len:
            bin_info.pfile += b'\xff'*(0x100-padding_len)
            file_len += (0x100-padding_len)
        bin_info.iFileLength = file_len
        bin_info.iFileCRC = crc32_ver2(0xffffffff, bin_info.pfile)
        bin_info.iLinkCheckType = LINKCHECK.BOOTROM
        bin_list.append(bin_info)
    return bin_list if len(bin_list)>0 else None

def load_bl2_ota_bin(srcbuf:bytes):
    # global_head 
    # bt_control 
    # partition
    # primary_manifest 
    # secondary_manifest
    # bta
    # btb

    head_len = 0x20
    big_endian = False
    bin_list = []
    start_section_index = 3
    
    sections = [
        ("primary_manifest", start_section_index * head_len),   
        ("secondary_manifest", (start_section_index+1) * head_len),  
        ("bta", (start_section_index+2) * head_len),                 
        ("btb", (start_section_index+3) * head_len)             
    ]
    
    for _, offset in sections:
        bin_info = combin_bl2_ota_bin(srcbuf, offset, big_endian)
        bin_list.append(bin_info)
        
    return bin_list

def combin_bl2_ota_bin(srcbuf:bytes,ptr:int,big_endian:bool)->BinInfo:
    order_format = '>I' if big_endian else '<I'

    bin_info = BinInfo()
    bin_info.iLinkCheckType = LINKCHECK.BL2
    # partition start addr
    bin_info.iPartition_start = struct.unpack_from(order_format,srcbuf,ptr)[0]
    # partition size
    ptr+=4
    bin_info.iPartition_size = struct.unpack_from(order_format,srcbuf,ptr)[0]
    # flash start addr
    ptr+=4
    bin_info.iStartAddress = struct.unpack_from(order_format,srcbuf,ptr)[0]
    # image offset
    ptr+=4
    offset_addr = struct.unpack_from(order_format,srcbuf,ptr)[0]
    # image length
    ptr+=4
    file_len = struct.unpack_from(order_format,srcbuf,ptr)[0]
    # image data
    bin_info.pfile = srcbuf[offset_addr:offset_addr+file_len]
    # crc
    padding_len = file_len%0x100
    if padding_len:
            bin_info.pfile += b'\xff'*(0x100-padding_len)
            file_len += (0x100-padding_len)
    bin_info.iFileLength = file_len
    bin_info.iFileCRC = crc32_ver2(0xffffffff, bin_info.pfile)   

    return bin_info

def load_bl1_all_app_bin(srcbuf:bytes):
    '''
    global header size = 32 bytes
    single header size = 32 bytes
    ''' 
    bin_list = []
    
    bin_num = (srcbuf[18]<<8)+srcbuf[19]  
    for idx in range(0,bin_num):
        bin_info = BinInfo()
        bin_info.bCheck = True
        pos = 0x20+0x20*idx
        bin_info.iPartition_start = (srcbuf[pos]<<24)+(srcbuf[pos+1]<<16)+(srcbuf[pos+2]<<8)+srcbuf[pos+3]
        pos = 0x20+0x20*idx+4
        bin_info.iPartition_size = (srcbuf[pos]<<24)+(srcbuf[pos+1]<<16)+(srcbuf[pos+2]<<8)+srcbuf[pos+3]
        pos = 0x20+0x20*idx+8
        bin_info.iStartAddress = (srcbuf[pos]<<24)+(srcbuf[pos+1]<<16)+(srcbuf[pos+2]<<8)+srcbuf[pos+3]
        pos = 0x20+0x20*idx+12
        image_offset = (srcbuf[pos]<<24)+(srcbuf[pos+1]<<16)+(srcbuf[pos+2]<<8)+srcbuf[pos+3]
        pos = 0x20+0x20*idx+16
        file_len = (srcbuf[pos]<<24)+(srcbuf[pos+1]<<16)+(srcbuf[pos+2]<<8)+srcbuf[pos+3]
        pos = 0x20+0x20*idx+28
        port_type = (srcbuf[pos]<<8)+srcbuf[pos+1]
        if port_type>0:
            bin_info.bDataPort=False
        else:
            bin_info.bDataPort=True
        bin_info.pfile = srcbuf[image_offset:image_offset+file_len]
        padding_len = file_len%0x100
        if padding_len:
            bin_info.pfile += b'\xff'*(0x100-padding_len)
            file_len += (0x100-padding_len)
        bin_info.iFileLength = file_len
        bin_info.iFileCRC = crc32_ver2(0xffffffff, bin_info.pfile)       
        bin_info.iLinkCheckType = LINKCHECK.BOOTROM
        
        bin_list.append(bin_info)
    return bin_list if len(bin_list)>0 else None

def load_bl2_app_bin(srcbuf:bytes):
    '''
    global header size = 32 bytes
    single header size = 32 bytes
    ''' 
    bin_list = []
    
    bin_num = (srcbuf[18]<<8)+srcbuf[19]  
    for idx in range(0,bin_num):
        bin_info = BinInfo()
        bin_info.bCheck = True
        pos = 0x20+0x20*idx
        bin_info.iPartition_start = (srcbuf[pos]<<24)+(srcbuf[pos+1]<<16)+(srcbuf[pos+2]<<8)+srcbuf[pos+3]
        pos = 0x20+0x20*idx+4
        bin_info.iPartition_size = (srcbuf[pos]<<24)+(srcbuf[pos+1]<<16)+(srcbuf[pos+2]<<8)+srcbuf[pos+3]
        pos = 0x20+0x20*idx+8
        bin_info.iStartAddress = (srcbuf[pos]<<24)+(srcbuf[pos+1]<<16)+(srcbuf[pos+2]<<8)+srcbuf[pos+3]
        pos = 0x20+0x20*idx+12
        image_offset = (srcbuf[pos]<<24)+(srcbuf[pos+1]<<16)+(srcbuf[pos+2]<<8)+srcbuf[pos+3]
        pos = 0x20+0x20*idx+16
        file_len = (srcbuf[pos]<<24)+(srcbuf[pos+1]<<16)+(srcbuf[pos+2]<<8)+srcbuf[pos+3]
        pos = 0x20+0x20*idx+28
        port_type = (srcbuf[pos]<<8)+srcbuf[pos+1]
        if port_type>0:
            bin_info.bDataPort=False
        else:
            bin_info.bDataPort=True
        bin_info.pfile = srcbuf[image_offset:image_offset+file_len]
        padding_len = file_len%0x100
        if padding_len:
            bin_info.pfile += b'\xff'*(0x100-padding_len)
            file_len += (0x100-padding_len)
        bin_info.iFileLength = file_len
        bin_info.iFileCRC = crc32_ver2(0xffffffff, bin_info.pfile)
        bin_info.iLinkCheckType = LINKCHECK.BL2        
        bin_list.append(bin_info)
    return bin_list if len(bin_list)>0 else None

def load_secure_boot_bin(srcbuf:bytes):
    '''
    global header size = 32 bytes
    single header size = 32 bytes
    ''' 
    bin_list = []   
    bin_num = (srcbuf[18]<<8)+srcbuf[19]  
    for idx in range(0,bin_num):
        bin_info = BinInfo()
        bin_info.bCheck = True
        pos = 0x20+0x20*idx
        bin_info.iPartition_start = (srcbuf[pos]<<24)+(srcbuf[pos+1]<<16)+(srcbuf[pos+2]<<8)+srcbuf[pos+3]
        pos = 0x20+0x20*idx+4
        bin_info.iPartition_size = (srcbuf[pos]<<24)+(srcbuf[pos+1]<<16)+(srcbuf[pos+2]<<8)+srcbuf[pos+3]
        pos = 0x20+0x20*idx+8
        bin_info.iStartAddress = (srcbuf[pos]<<24)+(srcbuf[pos+1]<<16)+(srcbuf[pos+2]<<8)+srcbuf[pos+3]
        pos = 0x20+0x20*idx+12
        image_offset = (srcbuf[pos]<<24)+(srcbuf[pos+1]<<16)+(srcbuf[pos+2]<<8)+srcbuf[pos+3]
        pos = 0x20+0x20*idx+16
        file_len = (srcbuf[pos]<<24)+(srcbuf[pos+1]<<16)+(srcbuf[pos+2]<<8)+srcbuf[pos+3]
        pos = 0x20+0x20*idx+28
        port_type = (srcbuf[pos]<<8)+srcbuf[pos+1]
        if port_type>0:
            bin_info.bDataPort=False
        else:
            bin_info.bDataPort=True
        pos = 0x20+0x20*idx+30
        data_type = (srcbuf[pos]<<8)+srcbuf[pos+1]
        if data_type>0:
            bin_info.bOtpData = True
        else:
            bin_info.bOtpData = False    
        bin_info.pfile = srcbuf[image_offset:image_offset+file_len]
        if not bin_info.bOtpData:
            padding_len = file_len%0x100
            if padding_len:
                bin_info.pfile += b'\xff'*(0x100-padding_len)
                file_len += (0x100-padding_len)
        bin_info.iFileLength = file_len
        bin_info.iFileCRC = crc32_ver2(0xffffffff, bin_info.pfile)       
        bin_info.iLinkCheckType = LINKCHECK.BOOTROM
        
        bin_list.append(bin_info)
    return bin_list if len(bin_list)>0 else None

def LoadBinFile(path:str,startaddr:int,oplen:int=0):
    with open(path, "rb") as f:
        pfile = f.read()                       
        f.close()
    file_len = len(pfile)
       
    if pfile[0:8]==b'beken...':
        bin_type = BIN_TYPE.TYPE_OTA
        listbin = load_ota_bin(pfile)
        return bin_type,listbin
    elif pfile[0:8]==b'BKDLV10.':
        bin_type = BIN_TYPE.TYPE_BL2_APP_BIN
        listbin = load_bl2_app_bin(pfile)
        return bin_type,listbin
    elif pfile[0:8]==b'BL1DLV10':
        bin_type = BIN_TYPE.TYPE_BL1_ALL_BIN
        listbin = load_bl1_all_app_bin(pfile)
        return bin_type,listbin
    elif pfile[0:8]==b'BLXDLV10':
        bin_type = BIN_TYPE.TYPE_SECURE_FIX
        listbin = load_secure_boot_bin(pfile)
        return bin_type,listbin
    elif pfile[0:8]==b'QSPI_DL.':
        bin_type = BIN_TYPE.TYPE_OUTTER_FLASH
        listbin = load_bl1_all_app_bin(pfile)
        return bin_type,listbin
    elif pfile[0:8]==b'BKFIXED.':
        bin_type = BIN_TYPE.TYPE_NORMAL_FIXED
        listbin = load_bl1_all_app_bin(pfile)
        return bin_type,listbin
    elif pfile[0:8]==b'BL2DLV10':
        bin_type = BIN_TYPE.TYPE_BL2_OTA
        listbin = load_bl2_ota_bin(pfile)
        return bin_type,listbin
    else:
        bin_type = BIN_TYPE.TYPE_NORMAL
        bininfo = load_normal_bin(pfile)
        bininfo.iStartAddress = startaddr       
        if oplen==0 or oplen>file_len:
            pass
        elif oplen<file_len:
            bininfo.iFileLength = oplen
            bininfo.pfile = pfile[0:oplen]
        else:
            bininfo.iFileLength = oplen           
        bininfo.sFilePath = path
        bin_list = [bininfo]
        return bin_type,bin_list
    

def parse_tizen_binfile_info(main_bin:str,start_addr:int):
    bin_list = []
    path_list = []
    if main_bin:
        separator = '@tizen_separator@'
        if main_bin.find(separator)== -1:
            bintype,binlist = LoadBinFile(main_bin,start_addr)               
            bin_list+=binlist
            path_list.append(main_bin)
        else:
            ftmp = main_bin.split(separator)
            startaddress = 0
            oplen = 0
            if ftmp[1].find('-')!=-1:
                stmp = ftmp[1].split('-')
                startaddress = int(stmp[0],16)
                oplen = int(stmp[1],16)
            bintype,binlist = LoadBinFile(ftmp[0],startaddress,oplen)
            path_list.append(ftmp[0])
            bin_list+=binlist
        return bintype, bin_list, path_list 
    else:
        return None, None, None


def parse_binfile_info(main_bin:str,start_addr:int):
    '''
    parse main_bin such as './all-app.bin' with start address
    or parse multi_bin such as './bootloader.bin,./bsp.bin@1100,./app.bin@5200-50000' 
    '''
    strlist = []
    bin_list = []
    path_list = []
    if main_bin:
        if main_bin.find(',')==-1:
            strlist = [main_bin]
        else:
            strlist = main_bin.split(",")

        for subbin in strlist:
            if subbin.find('.bin@')==-1:
                bintype,binlist = LoadBinFile(subbin,start_addr)               
                bin_list+=binlist
                path_list.append(subbin)
            else:
                ftmp = subbin.split(".bin@")                               
                startaddress = 0
                oplen = 0
                if ftmp[1].find('-')!=-1:
                    stmp = ftmp[1].split('-')
                    startaddress = int(stmp[0],16)
                    oplen = int(stmp[1],16)
                bintype,binlist = LoadBinFile(ftmp[0]+".bin",startaddress,oplen)
                path_list.append(ftmp[0]+".bin")
                bin_list+=binlist

        return bintype, bin_list, path_list
   
    else:
        return None, None, None
    

def parse_device_name_info(dev_name:str):
    '''
    string like "beken@0x2e000-5"
    return result,address,name,length
    '''
    if dev_name and dev_name.find("@")!=-1:
        ftmp = dev_name.split("@")
        if len(ftmp)<2:            
            return False,None,None,0
        name = bytes(ftmp[0],"ascii")
        stmp=ftmp[1].split("-")
        if len(stmp)<2:
            return False,None,None,0
        Addr = int(stmp[0],16)
        length = int(stmp[1])
        
    return True,name,Addr,length

def parse_device_mac_info(dev_mac:str):
    '''
    string like "aabbccddeeff@0x1e000-6"
    return result,address,mac,length
    '''

    if dev_mac and dev_mac.find("@")!=-1:
        ftmp = dev_mac.split("@")
        if len(ftmp)<2:            
            return False,None,None,0
        devid = bytes.fromhex(ftmp[0])
        stmp=ftmp[1].split("-")
        if len(stmp)<2:
            return False,None,None,0
        addr = int(stmp[0],16)
        length = int(stmp[1])
        
    return True,devid,addr,length

def parse_erase_info(freedom_erase:str):
    '''
    parse string such as '0-1000,2000-1000,3000-2000'
    '''
    binlist = []
    if freedom_erase:      
        splittmp = freedom_erase.split(',')
        for suberase in splittmp:
            stmp=suberase.split("-")
            if len(stmp)==2:
                eraseinfo = BinInfo()
                eraseinfo.bCheck=True
                eraseinfo.iStartAddress=int(stmp[0],16)
                eraseinfo.iFileLength = int(stmp[1],16)
                binlist.append(eraseinfo)
    return binlist if len(binlist)>0 else None
        
def parse_dump_info(freedom_read:str):
    '''
    parse string such as '0-1000,2000-1000,3000-2000'
    or './dump_1.bin@0-1000,./dump_2.bin@2000-1000,./dump_3.bin@3000-2000'
    '''
    binlist = []
    gen_bin_idx = 0
    if freedom_read:      
        splittmp = freedom_read.split(',')
        for subread in splittmp:
            readinfo = BinInfo()
            readinfo.bCheck=True
            if subread.find('@')==-1:
                #no dump file path, will use 'dump-x.bin'
                readinfo.sFilePath = f"dump{gen_bin_idx}.bin"                
                stmp = subread.split("-")
            else:
                ptmp=subread.split("@")
                if len(ptmp)==2:                 
                    readinfo.sFilePath = ptmp[0]
                    stmp = ptmp[1].split("-")
            if len(stmp)==2:
                readinfo.iStartAddress=int(stmp[0],16)
                readinfo.iFileLength = int(stmp[1],16)
                binlist.append(readinfo)
            gen_bin_idx+=1
    return binlist if len(binlist)>0 else None

def parse_safe_json_file(jsfile:str):   
    with open(jsfile,"r") as f:
        jess = f.read()
        jess = jess.lower()
        f.close()

    jroot = json.loads(jess)
    jroot = dict(jroot)
    safeinfo = safe_data() 
    if "user_operate_enable" in jroot.keys():
        safeinfo.user_operate_enable = True if jroot["user_operate_enable"]=="true" else False
    if "efuse_enable" in jroot.keys():
        safeinfo.efuse_enable = True if jroot["efuse_enable"]=="true" else False
    if "security_ctrl_enable" in jroot.keys():  
        safeinfo.security_ctrl_enable = True if jroot["security_ctrl_enable"]=="true" else False
    if "security_data_enable" in jroot.keys():
        safeinfo.security_data_enable = True if jroot["security_data_enable"]=="true" else False
    if "user_operate" in jroot.keys():
        for userinfo in jroot["user_operate"]:
            usr = user_operate()
            if "name" in userinfo.keys():
                usr.name = userinfo["name"]
            if "mode" in userinfo.keys():
                usr.mode = userinfo["mode"]
            if "permission" in userinfo.keys():
                usr.permission = userinfo["permission"] 
            if "start_addr" in userinfo.keys():    
                usr.start_addr = userinfo["start_addr"]
            if "last_valid_addr" in userinfo.keys():  
                usr.end_addr = userinfo["last_valid_addr"]
            if "byte_len" in userinfo.keys():  
                usr.byte_len = userinfo["byte_len"]
            if "data" in userinfo.keys():  
                usr.data = userinfo["data"]
            if "data_type" in userinfo.keys():  
                usr.data_type = userinfo["data_type"]
            if "status" in userinfo.keys():  
                usr.status = userinfo["status"]
            if "big_endian" in userinfo.keys():  
                usr.big_endian = userinfo["big_endian"]
            if "reverse" in userinfo.keys():  
                usr.reverse = userinfo["reverse"]    
            safeinfo.user_operate_list.append(usr)

    if "efuse" in jroot.keys():
        for efuseinfo in jroot["efuse"]:
            efs = efuse_info()
            if "name" in efuseinfo.keys(): 
                efs.name = efuseinfo["name"]
            if "mode" in efuseinfo.keys():
                efs.mode = efuseinfo["mode"]
            if "byte_addr" in efuseinfo.keys():
                efs.byte_addr = efuseinfo["byte_addr"]
            if "last_valid_addr" in efuseinfo.keys():
                efs.end_addr = efuseinfo["last_valid_addr"]
            if "length" in efuseinfo.keys():
                efs.length = efuseinfo["length"]
            if "data" in efuseinfo.keys():
                efs.data = efuseinfo["data"]  
            if "status" in efuseinfo.keys():      
                efs.status = efuseinfo["status"]
            safeinfo.efuse_info_list.append(efs)

    if "security_data" in jroot.keys():
        for secinfo in jroot["security_data"]:
            secu = user_operate()
            if "name" in secinfo.keys():
                secu.name = secinfo["name"]
            if "mode" in secinfo.keys():
                secu.mode = secinfo["mode"]
            if "permission" in secinfo.keys():
                secu.permission = secinfo["permission"]    
            if "start_addr" in secinfo.keys():
                secu.start_addr = secinfo["start_addr"]
            if "last_valid_addr" in secinfo.keys():
                secu.end_addr = secinfo["last_valid_addr"]
            if "byte_len" in secinfo.keys():
                secu.byte_len = secinfo["byte_len"]
            if "data" in secinfo.keys():
                secu.data = secinfo["data"]
            if "data_type" in secinfo.keys():
                secu.data_type = secinfo["data_type"]
            if "status" in secinfo.keys():
                secu.status = secinfo["status"]
            if "big_endian" in secinfo.keys():  
                secu.big_endian = secinfo["big_endian"]
            if "reverse" in secinfo.keys():  
                secu.reverse = secinfo["reverse"]
            safeinfo.security_data_list.append(secu)

    if "security_ctrl" in jroot.keys():
        ctrlitems = jroot["security_ctrl"][0]
        for items in ctrlitems.items():
            safeinfo.security_ctrl.append(items[1])
      
    return safeinfo


def parse_readinfo_otp(otpinfo:str):
    '''
    string like '0x4b010000-4'
    return list[address, length]
    '''
    otp_list = []
    splittmp = otpinfo.split(',')
    for info in splittmp:            
        tmp = info.split("-")
        if len(tmp) >= 2:
            address = int(tmp[0],16)
            oplen = int(tmp[1])
            otp_list.append([address,oplen])
    return otp_list if len(otp_list)>0 else None
    
def parse_writeinfo_otp(otpinfo:str):
    '''
    string like '73c7bf397f2ad6bf@0x4b100460-8'
    return list[value, address, length]
    '''
    info_list = []
    splittmp = otpinfo.split(',')
    for info in splittmp:
        tmp = info.split("@")
        if len(tmp) >= 2:
            value = tmp[0]                  
            tmp = tmp[1].split("-")
            if len(tmp) >= 2:
                address = int(tmp[0],16)
                oplen = int(tmp[1])
                info_list.append([value,address,oplen])
    
    return info_list if len(info_list)>0 else None


def parse_writeinfo_efuse(efusinfo:str):
    '''
    string like '0x1-0x7', byte index - bit index
    return list[address, bit]
    '''
    info_list = []
    splittmp = efusinfo.split(',')
    for info in splittmp:
        #1@0x1-0x7, set efuse 0x1 bit 7 as 1
        if info.find("@")!=-1:
            sp_tmp = efusinfo.split('@')
            v = int(sp_tmp[0])
            tmp = sp_tmp[1].split("-")
            if len(tmp) >= 2:
                address = int(tmp[0],16)
                bit = int(tmp[1],16)
                info_list.append([v,address,bit])
        #0x1-0x20, set efuse 0x1 as 0x20
        else:          
            tmp = info.split("-")
            if len(tmp) >= 2:
                address = int(tmp[0],16)
                v = tmp[1]#int(tmp[1],16)
                info_list.append([address,v])
    return info_list if len(info_list)>0 else None



# def is_buf_all_0xff(buf):
#     length = len(buf)
#     for i in range(length):
#         if buf[i] != 0xff:
#             return False
#     return True


def is_buf_all_0xff(buf):
    return buf.count(b'\xff')==len(buf)


def get_script_dir():
    script_dir = os.path.abspath(__file__)
    script_dir = os.path.dirname(script_dir)
    return script_dir


def bytes_reverse(buf:bytes):
    oplen = len(buf)
    tmp = bytearray()
    for i in range(oplen):
        tmp.append(buf[oplen-1-i])
    return tmp

def bytes_reverse_per4bytes(buf:bytes):
    oplen = len(buf)
    tmp = bytearray()
    idx=0
    while idx<oplen:
        byss = buf[oplen-idx-4:oplen-idx]
        tmp+=byss
        idx+=4
    return tmp

def available_serial_ports():
    '''
    lists serial port names
    :raises EnvironmentError:
        On unsupported or unknown platforms
    :returns:
        A list of the serial ports available on the system
    '''
    os = get_os_type()
    if os == 'Windows':
        ports = ['COM%s' % (i+1) for i in range(256)]
    elif os == 'Linux' or os == 'Raspberry Pi':
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif os == 'MacOS':
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')
    
    res = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            res.append(port)
        except (OSError, serial.SerialException):
            pass
    return res




