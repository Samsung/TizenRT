# bk_loader
#
# Copyright (c) BekenCorp. All rights reserved.
#
# This software may be distributed under the terms of the BSD license.
# See README for more details.
import sys
import os
sys.path.append(os.path.dirname(__file__))

import click
from bkutils.bk_log.log_utils import *
from bkutils.common.common import BIN_TYPE,RESET_TYPE, LINKCHECK, AuthorizeInfo, DownloadInfo,ReadInfo,EraseInfo, BinInfo, parse_binfile_info, parse_device_name_info, parse_device_mac_info, parse_erase_info, parse_dump_info, available_serial_ports, crc32_ver2,crc32bytes, parse_safe_json_file
from bkutils.common.version import *
from bkutils.action_service.download import Download
from bkutils.action_service.erase import Erase
from bkutils.action_service.dump import Dump
from bkutils.action_service.authorize import Authorize
from bkutils.chip_controller.base_controller import BaseController
from bkutils.chip_controller.serial_helper import SerialHelper




@click.group()
@click.version_option(version=get_tool_version())
def cli():
    """Beken flash image loader tools - bk_loader"""
    pass


"""====================================================[ DOWNLOAD ]==============================================================="""

@cli.command()
#basic info
@click.option('-p', '--portnum', default='/dev/ttyUSB0', required=True, help="UART device, such as default '/dev/ttyUSB0'")
@click.option('-b', '--baudrate', default=2000000, type=int, help="Burn UART baudrate, defaults to 2000000")
@click.option('-r', '--reboot', is_flag=True,default = False, help="Reboot after download process")
@click.option('--link_type', type=click.Choice(['0', '1', '2']), default='0', help="link boot rom type")
@click.option('--reset_type', type=click.Choice(['0', '1', '2','3']), default='3', help="reset type,0=dtr/rts,1=command 'reboot',2=command '95 27',3=multi reset dtr/rts then cmd")
@click.option('--reset_baudrate', default=115200, type=int, help="app reset UART baudrate, defaults to 115200")
@click.option('-g','--getbus_times', default=100, type=int, help="get bus times, default is 100 times")
@click.option('--retrycnt', default=5, type=int, help="Retry times")
@click.option('--chipid', type=str, help="default none will read from chipset,0x3000 for generic BT project, 0x3436 is a special one")
@click.option('-c', '--chip_all', is_flag=True,default = False, help="erase/read all chip")
@click.option('-e', '--pre_erase', type=click.Choice(['0', '1']), default='1', help="erase flash before write")
@click.option('-d', '--debug', type=click.Choice(['0', '1', '2', '3', '999']), default='3',
        help="Set debug level: 0=nothing, 1=error message, 2=warning message, 3=info in test process + file, 999=full debug message + file")
@click.option('--link_key', type=str, help="Set link key, such as : --link_key 000102030405060708090a0b0c0d0e0f")

#bin file
@click.option('-i', '--infile', type=click.Path(),help="Input one for more existing binary files.")
@click.option('--del_after', is_flag=True,default = False, help="delete bin file after read buffer")
@click.option('-s', '--startaddr', default='0x0', help="Burn flash start address, defaults to 0x0")

#safe info
@click.option('--safe_json', type=click.Path(), help="Input safe json file path")
@click.option('--aes_key', type=str, help="Input aes key stream")
@click.option('--efuse', type=str,help="Write efuse as 1, address is efuse byte index, or write efuse u8 value, include: [address-val] or [1@address-bit], such as: --efuse 0x1-0x10,1@0x2-0x1")
@click.option('--otp', type=str,help="Write OTP,value must be hex format, include: [value@address-length], such as: --otp 73c7bf397f2ad6bf@0x4b100460-8,73c7bf397f2ad6bf@0x4b100460-8")
@click.option('--otp_pm', type=str,help="Set OTP permission,include: [pm@address-length], such as: --otp_pm ro@0x4b100460-32,ro@0x4b100460-32")
# BK7236 customer like signify/tuya need setting the two command behind
# BK7236 customer media and BK7259 do not need
@click.option('--otp_reverse', is_flag = True,default = False, help="Write OTP such as AES key, set reverse content per 4 bytes")
@click.option('--otp_big_endian', is_flag = True,default = False, help="Write OTP such as AES key if the OTP data which is writting is big-endian")

#device name,id
@click.option('--dev_name', type=str, help="Set device name,include : [data@startAddress-length],such as : --dev_name beken@0x2e000-5")
@click.option('-m','--mac', type=str, help="Set mac information,include : [data@startAddress-length],such as : --mac aabbccddeeff@0x1e000-6")
@click.option('--big_endian', is_flag = True,default = False, help="Write device ID(MAC),set big-endian")
#flash OTP
@click.option('--flash_otp', type=str, help="Set flash OTP information,include : [data@startAddress-length],such as : --flash_otp aabbccddeeff@0x0001ff-6")

@click.option('--check_words', type=str, help="check key words after reboot from bootrom, [words@baudrate],such as --check_words OTA:SUCCESS@115200")

@click.option('--custom_data', type=str, help="Set custom data,include : [data@startAddress-length],such as : --custom_data aabbccddeeff@0x1e000-6")


def download(portnum, baudrate,reboot,link_type,reset_type,reset_baudrate,getbus_times,retrycnt,chipid,chip_all,debug
             ,infile,startaddr,safe_json,aes_key,efuse,otp,otp_pm,dev_name,mac,big_endian,otp_reverse,otp_big_endian
             ,flash_otp,del_after,pre_erase,check_words,custom_data,link_key):
    try:

        startaddr = int(startaddr, 16)  
        if reset_type=='0':
            reset_type=RESET_TYPE.DTR_RTS
        elif reset_type=='1':
            reset_type=RESET_TYPE.CMD_ASCII
        elif reset_type=='2':
            reset_type=RESET_TYPE.CMD_HEX
        else:
            reset_type=RESET_TYPE.MULTI
        if link_type=='0':
            link_type = LINKCHECK.BOOTROM
        elif link_type=='1':
            link_type = LINKCHECK.BL2
        else:
            link_type = LINKCHECK.BOOTROM_PRE

        if link_key:
            link_key = bytes.fromhex(link_key)
            link_type = LINKCHECK.BOOTROM_EXT
            print('link_type:ext')

        if chipid:
            chipid = int(chipid,16)
        else:
            chipid = None    
        
        downloadinfo = DownloadInfo()
        downloadinfo.work_baudrate = baudrate
        downloadinfo.bAllFlash = chip_all       
        downloadinfo.aes_key = aes_key
        downloadinfo.efuse_info = efuse
        downloadinfo.otp_info = otp
        downloadinfo.otp_pm = otp_pm
        downloadinfo.bEnableBigEndian = big_endian
        downloadinfo.reverse_content = otp_reverse
        downloadinfo.otp_bigendian = otp_big_endian
        downloadinfo.pre_erase = int(pre_erase)
        if check_words:
            downloadinfo.check_words = check_words
        if dev_name:
            ret,devname,addr,oplen = parse_device_name_info(dev_name)
            if not ret:
                raise Exception("Input device name format error.")
            downloadinfo.bDownloadDeviceName=True
            downloadinfo.device_name = devname
            downloadinfo.device_name_Addr=addr
            downloadinfo.device_name_length = oplen
        if mac:
            ret,devid,addr,oplen = parse_device_mac_info(mac)
            if not ret:
                raise Exception("Input mac format error.")
                
            downloadinfo.bDownloadDeviceID = True
            downloadinfo.device_id = devid
            downloadinfo.device_id_Addr = addr
            downloadinfo.device_id_length = oplen
        if safe_json:
            downloadinfo.safeinfo = parse_safe_json_file(safe_json)
            downloadinfo.safeinfo.write_safe_config = True       
        if infile:                       
            bin_type,binlist,pathlist = parse_binfile_info(infile,startaddr)
            if len(binlist)>0:
                downloadinfo.bDownloadBin = True
                downloadinfo.vecbinInfo = binlist       
        else:
            bin_type= BIN_TYPE.TYPE_NORMAL
        if del_after:
            #the first bin file will be encrypted, delete it after parse bin info
            os.remove(pathlist[0])
            if downloadinfo.safeinfo.write_safe_config:
                os.remove(safe_json)

        portlist = [portnum]       
        if portnum.find(',')!=-1:
            portlist = portnum.split(',')

        for port in portlist:                      
            if bin_type == BIN_TYPE.TYPE_SECURE_FIX:                
                downloader = Download(port, 115200, None,reset_type,reset_baudrate,getbus_times,reboot,retrycnt,chipid,debug,link_key)
                #downloader.download_secure_task(downloadinfo)
                dl_thread = threading.Thread(target=downloader.download_secure_task,args=(downloadinfo,))
                dl_thread.start()
            elif bin_type == BIN_TYPE.TYPE_BL2_APP_BIN:                
                downloader = Download(port, 115200, LINKCHECK.BL2,reset_type,reset_baudrate,getbus_times,reboot,retrycnt,chipid,debug,link_key)
                dl_thread = threading.Thread(target=downloader.download_base_task,args=(downloadinfo,))
                dl_thread.start()
            elif bin_type == BIN_TYPE.TYPE_BL1_ALL_BIN or bin_type == BIN_TYPE.TYPE_OTA:               
                downloader = Download(port, 115200, link_type,reset_type,reset_baudrate,getbus_times,reboot,retrycnt,chipid,debug,link_key)
                dl_thread = threading.Thread(target=downloader.download_base_task,args=(downloadinfo,))
                dl_thread.start()
            elif bin_type == BIN_TYPE.TYPE_OUTTER_FLASH:              
                downloader = Download(port, 115200, LINKCHECK.BOOTROM,reset_type,reset_baudrate,getbus_times,reboot,retrycnt,chipid,debug,link_key)
                dl_thread = threading.Thread(target=downloader.download_with_outter_flash_task,args=(downloadinfo,))
                dl_thread.start()
            else:               
                downloader = Download(port, 115200, link_type,reset_type,reset_baudrate,getbus_times,reboot,retrycnt,chipid,debug,link_key)
                dl_thread = threading.Thread(target=downloader.download_base_task,args=(downloadinfo,))
                dl_thread.start()
        
    except Exception as e:           
            print(str(e))
            print("Test fail.")
            return


"""====================================================[ ERASE ]==============================================================="""

@cli.command()
@click.option('-p', '--portnum', default='/dev/ttyUSB0', required=True, help="UART device, such as default '/dev/ttyUSB0'")
@click.option('-b', '--baudrate', default=2000000, type=int, help="Burn UART baudrate, defaults to 2000000")
@click.option('-r', '--reboot', is_flag=True,default = False, help="Reboot after download process")
@click.option('--link_type', type=click.Choice(['0', '1', '2']), default='0', help="link boot rom type")
@click.option('--reset_type', type=click.Choice(['0', '1', '2']), default='0', help="reset type,0=dtr/rts,1=command 'reboot',2=command '95 27'")
@click.option('--reset_baudrate', default=115200, type=int, help="app reset UART baudrate, defaults to 115200")
@click.option('-g','--getbus_times', default=100, type=int, help="get bus times, default is 100 times")
@click.option('--retrycnt', default=5, type=int, help="Retry times")
@click.option('--chipid', type=str, help="default None will read from chipset,0x3000 for generic BT project, 0x3436 is a special one")
@click.option('-c', '--chip_all', is_flag=True,default = False, help="erase/read all chip")
@click.option('-d', '--debug', type=click.Choice(['0', '1', '2', '3', '999']), default='3',
        help="Set debug level: 0=nothing, 1=error message, 2=warning message, 3=info in test process + file, 999=full debug message + file")
@click.option('--link_key', type=str, help="Set link key, such as : --link_key 000102030405060708090a0b0c0d0e0f")
@click.option('-f','--freedom', type=str, help="Input freedom address and length,Hex data,such as \" [-f 0-10000] \"")
def erase(portnum, baudrate,reboot,link_type,reset_type,reset_baudrate,getbus_times,retrycnt, chipid,chip_all,debug,freedom,link_key):
    try:
               
        if reset_type=='0':
            reset_type=RESET_TYPE.DTR_RTS
        elif reset_type=='1':
            reset_type=RESET_TYPE.CMD_ASCII
        else:
            reset_type=RESET_TYPE.CMD_HEX
        if link_type=='0':
            link_type = LINKCHECK.BOOTROM
        elif link_type=='1':
            link_type = LINKCHECK.BL2
        else:
            link_type = LINKCHECK.BOOTROM_PRE

        if link_key:
            link_key = bytes.fromhex(link_key)
            link_type = LINKCHECK.BOOTROM_EXT

        if chipid:
            chipid = int(chipid,16)
        else:
            chipid = None
        eraser = Erase(portnum, 115200,link_type,reset_type,reset_baudrate,getbus_times,reboot,retrycnt,chipid,debug,link_key)
        eraseinfo = EraseInfo()
        eraseinfo.work_baudrate = baudrate
        if chip_all:
            eraseinfo.bAllFlash=True
        else:
            binlist=parse_erase_info(freedom)
            eraseinfo.vecbinInfo = binlist
            if binlist is None:
                raise Exception(f"Argument error, need input address and length such as [-f 0-1000]")
        
        erase_thread = threading.Thread(target=eraser.erase_task,args=(eraseinfo,))
        erase_thread.start()
               
    except Exception as e:           
        BKLog.e(str(e))
        BKLog.e("Test fail.")
        return
    


"""====================================================[ READ ]==============================================================="""

@cli.command()
@click.option('-p', '--portnum', default='/dev/ttyUSB0', required=True, help="UART device, such as default '/dev/ttyUSB0'")
@click.option('-b', '--baudrate', default=1000000, type=int, help="Burn UART baudrate, defaults to 1000000")
@click.option('-r', '--reboot', is_flag=True,default = False, help="Reboot after download process")
@click.option('--link_type', type=click.Choice(['0', '1', '2']), default='0', help="link boot rom type")
@click.option('--reset_type', type=click.Choice(['0', '1', '2']), default='0', help="reset type,0=dtr/rts,1=command 'reboot',2=command '95 27'")
@click.option('--reset_baudrate', default=115200, type=int, help="app reset UART baudrate, defaults to 115200")
@click.option('-g','--getbus_times', default=100, type=int, help="get bus times, default is 100 times")
@click.option('--retrycnt', default=5, type=int, help="Retry times")
@click.option('--chipid', type=str, help="default 0 will read from chipset,0x3000 for generic BT project, 0x3436 is a special one")
@click.option('-c', '--chip_all', is_flag=True,default = False, help="erase/read all chip")
@click.option('-d', '--debug', type=click.Choice(['0', '1', '2', '3', '999']), default='3',
        help="Set debug level: 0=nothing, 1=error message, 2=warning message, 3=info in test process + file, 999=full debug message + file")
@click.option('--link_key', type=str, help="Set link key, such as : --link_key 000102030405060708090a0b0c0d0e0f")
@click.option('-o', '--out_path', type=click.Path(),help="Input dump file path for read flash.")
@click.option('-s', '--startaddr', default='0x0', help="Read flash address, defaults to 0x0")
@click.option('-f','--freedom', type=str, help="Input freedom address and file length,Hex data,such as \"[-f x-1.bin@0-10000] | [-f D:\all.bin@0x10000-0x1000]\"")
@click.option('--console', is_flag=True,default = False, help="Print the content to console after reading info")
@click.option('--read_uid', is_flag=True,default = False, help="Read chip UID")
@click.option('--read_efuse', type=str, help="Read chip efuse with address and length, such as '[0x10-4]'")
@click.option('--read_otp', type=str, help="Read chip otp with reg address and length, such as '[0x4b010000-4]'")
@click.option('--reverse_content', is_flag = True,default = False, help="Read OTP such as AES key,set reverse content per 4 bytes")
@click.option('--read_flash_otp', type=str, help="Set read flash OTP information,include : [startAddress-length],such as : --read_flash_otp 0x0001ff-6")
@click.option('--read_otp_efuse_uid', type=str, help="Read chip otp/efuse/uid,otp need provide reg address and length, such as '[0x4b010000-4]'")


def read(portnum, baudrate,reboot,link_type,reset_type,reset_baudrate,getbus_times,retrycnt, chipid,chip_all,debug,out_path,startaddr,freedom,console,read_uid,read_efuse,read_otp,reverse_content,read_flash_otp,read_otp_efuse_uid,link_key):
    try:
        
        if reset_type=='0':
            reset_type=RESET_TYPE.DTR_RTS
        elif reset_type=='1':
            reset_type=RESET_TYPE.CMD_ASCII
        else:
            reset_type=RESET_TYPE.CMD_HEX
        if link_type=='0':
            link_type = LINKCHECK.BOOTROM
        elif link_type=='1':
            link_type = LINKCHECK.BL2
        else:
            link_type = LINKCHECK.BOOTROM_PRE
        
        if link_key:
            link_key = bytes.fromhex(link_key)
            link_type = LINKCHECK.BOOTROM_EXT

        if chipid:
            chipid = int(chipid,16)
        else:
            chipid = None

        dumper = Dump(portnum, 115200,link_type,reset_type,reset_baudrate,getbus_times,reboot,retrycnt,chipid,debug,link_key)
        
        readinfo = ReadInfo()
        readinfo.work_baudrate = baudrate
        readinfo.read_uid = read_uid
        readinfo.read_efuse = read_efuse
        readinfo.read_otp = read_otp
        readinfo.bConsole = console
        readinfo.reverse_content = reverse_content
        readinfo.read_path = out_path
        readinfo.read_otp_efuse_uid = read_otp_efuse_uid
        #read whole flash
        if chip_all:       
            readinfo.bAllFlash=True
            dumpinfo = BinInfo()
            dumpinfo.bCheck=True
            dumpinfo.sFilePath='dump_all.bin'
            dumpinfo.iStartAddress=int(startaddr,16)
            readinfo.vecbinInfo.append(dumpinfo)
        else:
            if freedom:
                binlist=parse_dump_info(freedom)
                readinfo.vecbinInfo = binlist
                if binlist is None:
                    raise Exception("Argument error, need input address and length such as [-f dump-1.bin@0-1000]")
                                  
        read_thread = threading.Thread(target=dumper.read_task,args=(readinfo,))
        read_thread.start()

    except Exception as e:           
        BKLog.e(str(e))
        BKLog.e("Test fail.")
        return

"""====================================================[ authorize ]==============================================================="""

@cli.command()
@click.option('-p', '--portnum', default='/dev/ttyUSB0', required=True, help="UART device, such as default '/dev/ttyUSB0'")
@click.option('-b', '--baudrate', default=1000000, type=int, help="Burn UART baudrate, defaults to 2000000")
@click.option('-r', '--reboot', is_flag=True,default = False, help="Reboot after download process")
@click.option('--link_type', type=click.Choice(['0', '1', '2']), default='0', help="link boot rom type")
@click.option('--reset_type', type=click.Choice(['0', '1', '2']), default='0', help="reset type,0=dtr/rts,1=command 'reboot',2=command '95 27'")
@click.option('--reset_baudrate', default=115200, type=int, help="app reset UART baudrate, defaults to 115200")
@click.option('-g','--getbus_times', default=100, type=int, help="get bus times, default is 100 times")
@click.option('--retrycnt', default=5, type=int, help="Retry times")
@click.option('--link_key', type=str, help="Set link key, such as : --link_key 000102030405060708090a0b0c0d0e0f")
@click.option('--chipid', type=str, help="default None will read from chipset,0x3000 for generic BT project, 0x3436 is a special one")
@click.option('-d', '--debug', type=click.Choice(['0', '1', '2', '3', '999']), default='3',
        help="Set debug level: 0=nothing, 1=error message, 2=warning message, 3=info in test process + file, 999=full debug message + file")
@click.option('--aes_key', type=str, help="Set aes128 ecb key stream")

def authorize(portnum, baudrate,reboot,link_type,reset_type,reset_baudrate,getbus_times,retrycnt, chipid,debug,aes_key,link_key):
    try:              
        if reset_type=='0':
            reset_type=RESET_TYPE.DTR_RTS
        elif reset_type=='1':
            reset_type=RESET_TYPE.CMD_ASCII
        else:
            reset_type=RESET_TYPE.CMD_HEX
        if link_type=='0':
            link_type = LINKCHECK.BOOTROM
        elif link_type=='1':
            link_type = LINKCHECK.BL2
        else:
            link_type = LINKCHECK.BOOTROM_PRE
        
        if link_key:
            link_key = bytes.fromhex(link_key)
            link_type = LINKCHECK.BOOTROM_EXT
        if chipid:
            chipid = int(chipid,16)
        else:
            chipid = None
        auth = Authorize(portnum, 115200,link_type,reset_type,reset_baudrate,getbus_times,reboot,retrycnt,chipid,debug)
        authinfo = AuthorizeInfo()
        authinfo.work_baudrate = baudrate
        if aes_key:
            authinfo.aes_key = aes_key
        
        auth_thread = threading.Thread(target=auth.authorize_task,args=(authinfo,))
        auth_thread.start()
        auth_thread.join()
               
    except Exception as e:           
        BKLog.e(str(e))
        BKLog.e("Test fail.")
        return




"""====================================================[ TOOLS ]==============================================================="""

@cli.group("tools")
def tools_group():
    """Manage tools"""
    pass

@tools_group.command("getbus")
@click.option('-p', '--portnum', default='/dev/ttyUSB0', required=True, help="UART device, such as default '/dev/ttyUSB0'")
@click.option('-g','--getbus_times', default=20, type=int, help="get bus times, default is 20 times")
def getbus(portnum,getbus_times):
    BKLog(logger_level=logging.INFO, log_folder_path='./') # 实例化log
    serial = SerialHelper()
    serial.connect(portnum, 115200, 0)
    base_ctrl = BaseController(serial_instance=serial)
    res,linkcheck_type = base_ctrl.get_bus(reset_type=RESET_TYPE.DTR_RTS,max_try_count=getbus_times)
    if res:
        BKLog.i('getbus done.\r\n')
    else:
        BKLog.i('getbus fail.\r\n')


@tools_group.command("addcrc")
@click.option('-i', '--infile', type=click.Path(),help="Input one existing binary file.")

def addcrc(infile: str):
    BKLog(logger_level=logging.INFO, log_folder_path='./') # 实例化log
    BKLog.i(f"Add crc for file {infile}")
    with open(infile,'rb') as f:
        src_data = f.read()
        f.close()
    total_len = len(src_data)
    #align 0x20
    if total_len%0x20:
        total_len += 0x20-total_len%0x20
        src_data += '0xff'*(0x20-total_len%0x20)
    loop_cnt = int(total_len/0x20)
    crcdata = bytearray()
    for idx in range(loop_cnt):
        data = bytearray(src_data[idx*0x20:idx*0x20+0x20])
        crcdata += crc32bytes(data=data)
    outfile = infile.replace('.bin','_crc.bin')
    with open(outfile,'wb') as of:
        of.write(crcdata)
        of.close()


@tools_group.command("rmcrc")
@click.option('-i', '--infile', type=click.Path(),help="Input one existing binary file.")

def rmcrc(infile: str):
    BKLog(logger_level=logging.INFO, log_folder_path='./') # 实例化log
    BKLog.i(f"Remove crc for file {infile}")
    with open(infile,'rb') as f:
        src_data = f.read()
        f.close()
    total_len = len(src_data)
    #align 0x22
    if total_len%0x22:
        total_len += 0x22-total_len%0x22
        src_data += '0xff'*(0x22-total_len%0x22)
               
    if crc32bytes(src_data[0:0x20])==bytearray(src_data[0:0x22]):                
        idx=0
        nocrc_data = bytearray()
        while idx<total_len-0x22:
            nocrc_data+=bytearray(src_data[idx:idx+0x20])
            idx+=0x22
    outfile = infile.replace('.bin','_rmcrc.bin')
    with open(outfile,'wb') as of:
        of.write(nocrc_data)
        of.close()     


@tools_group.command("encrypt")
@click.option('-p', '--portnum', default='/dev/ttyUSB0', help="UART device, default /dev/ttyUSB0")

def encrypt(portnum):
    BKLog(logger_level=logging.INFO, log_folder_path='./') # 实例化log
    BKLog.i(f"Encrypting data on port {portnum}")


@tools_group.command("package")
@click.option('-p', '--portnum', default='/dev/ttyUSB0', help="UART device, default /dev/ttyUSB0")

def package(portnum):
    BKLog(logger_level=logging.INFO, log_folder_path='./') # 实例化log
    BKLog.i(f"Packaging data on port {portnum}")

@tools_group.command("list_ports")
def list_ports():
    ports = available_serial_ports()
    BKLog(logger_level=logging.INFO, log_folder_path='./') # 实例化log
    BKLog.i(f"available ports: {ports}")

@tools_group.command("cal_crc32")
@click.option('-i', '--infile', type=click.Path(),help="Input one existing binary files.")
def cal_crc32(infile):   
    BKLog(logger_level=logging.INFO, log_folder_path='./') # 实例化log
    with open(infile, "rb") as f:
        buf = f.read()
        f.close()
    if buf:
        buf_len = len(buf)
        padding_len = buf_len%0x100
        if padding_len:
            buf += b'\xff'*(0x100-padding_len)
            buf_len += (0x100-padding_len)
        crc32 = crc32_ver2(0xffffffff,buf)
        BKLog.i(f"CRC32 is: 0x{crc32:x}")
    else:
        BKLog.w("Error, bin file is empty")

if __name__ == '__main__':
    cli()
