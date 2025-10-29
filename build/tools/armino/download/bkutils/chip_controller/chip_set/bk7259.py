import time
from serial import Timeout
from .bk7236 import BK7236
from .bk7259_security import BK7259_Security
from ...protocol.bootrom import v2_0 as V2_0_BOOTROM_PROTOCOL
from ...bk_log import BKLog
from ...common.common import FLASH_SIZE_DEF, LINKCHECK,FLASH_ERASE_CMD_EXT, FLASH_ERASE_CMD, safe_data,BinInfo, is_buf_all_0xff, BIT, BFD, crc32_ver2,crc32bytes, bytes_reverse,bytes_reverse_per4bytes
from ...chip_controller import BaseController,FlashController,RegController

#import binascii

class BK7259(BK7236):

    CHIP_ID = 0x7259
    BOOTROM_PROTOCOL = V2_0_BOOTROM_PROTOCOL

    def __init__(self):
        super().__init__()
           
    def pre_check_image_file(self, bin_list:list):
        '''
        check if image data add crc(2bytes) per 32 bytes, then remove it
        '''
        for bininfo in bin_list:
            if crc32bytes(bininfo.pfile[0:0x20])==bytearray(bininfo.pfile[0:0x22]):
                total_len = bininfo.iFileLength
                src_data = bininfo.pfile
                if total_len%0x22:
                    total_len += 0x22-total_len%0x22
                    src_data += b'0xff'*(0x22-total_len%0x22)
                idx=0
                nocrc_buf = bytearray()
                while idx<total_len-0x22:
                    nocrc_buf+=bytearray(src_data[idx:idx+0x20])
                    idx+=0x22
                bininfo.pfile = nocrc_buf
                bininfo.iFileLength = len(nocrc_buf)
            
    def pre_set_efuse_nocrc(self):
        security = BK7259_Security(serial_instance=self.ser)
        return security._write_efuse_by_bit(0x3,0x3,1)
############################################################################################################
# security operation
############################################################################################################

    def write_chip_otp(self, otp_addr:int, content:str, reverse_content=False, bigendian=False):
        '''
        write otp by address information
        '''
        security = BK7259_Security(serial_instance=self.ser)
        return security._write_chip_otp(otp_addr,content,"hex",reverse_content,bigendian)
    
               
    def set_otp_permission_oplen(self, reg_addr, oplen, permission):
        '''
        reg_addr: start OTP address
        oplen: reg count
        permission: wr/ro/na
        '''
        security = BK7259_Security(serial_instance=self.ser)
        return security._set_otp_permission_oplen(reg_addr,oplen,permission)
           

    def write_efuse_by_bit(self,reg_addr,bit,value):
        '''
        write one byte value, 8 bits
        'addr' is the pos of efuse
        '''
        security = BK7259_Security(serial_instance=self.ser)
        return security._write_efuse_by_bit(reg_addr,bit,value)
    
    def write_efuse(self,reg_addr,value):
        '''
        write one byte value, 8 bits
        'addr' is the pos of efuse
        '''
        security = BK7259_Security(serial_instance=self.ser)
        return security._write_efuse(reg_addr,value)

    
    def read_chip_otp(self,otp_addr, oplen, reverse_content=False,bigendian=False):
        security = BK7259_Security(serial_instance=self.ser)
        ret,msg = security._read_chip_otp(otp_addr,oplen,bigendian)
        if ret is not None:
            if reverse_content:
                return bytes_reverse_per4bytes(ret),msg
            else:
                return ret,msg
        else:
            return None,msg

    def read_chip_uid(self):
        '''
        read chip UID from OTP, length: 5 bytes
        '''
        security = BK7259_Security(serial_instance=self.ser)
        if not security._set_otp_write_enable():
            BKLog.e(f"Set OTP write enable fail.")
            return False, None
        time.sleep(0.1)
        ret, val = security._read_otp_by_regaddr(0x4b1007c0,5,True)
        if not ret:
            BKLog.e(f"Read OTP fail, address: 0x{security.security_otp_enable_addr:x}.")
            return False, None
        
        return True,val
    
    def read_efuse(self,addr,oplen):
        '''
        read chip efuse info
        '''
        security = BK7259_Security(serial_instance=self.ser)
        byteslen = int(security.security_efuse_length/8)
        
        ret,val = security._read_efuse(0,byteslen)
        if not ret:
            BKLog.e(f"Read efuse fail.")
            return False,None
        return True,val

    def write_safe_json(self, safeinfo: safe_data):
        '''
        write safe info set by json file
        '''
        security = BK7259_Security(serial_instance=self.ser)
        if safeinfo.user_operate_enable:
            for usrop in safeinfo.user_operate_list:
                if usrop.status=="true" and usrop.mode=="write":
                    #write otp info
                    if usrop.name.find("otp")!=-1:
                        ret,msg = security._write_otp_info(usrop,usrop.reverse,usrop.big_endian)
                        if not ret:
                            return False,msg
                    #write flash info
                    elif usrop.name.find("flash")!=-1:
                        pass
            BKLog.i("write otp ok.")
        if safeinfo.security_data_enable:
            for secdata in safeinfo.security_data_list:
                if secdata.status=="true" and secdata.mode=="write":
                    ret,msg = security._write_otp_info(secdata,usrop.reverse,usrop.big_endian)
                    if not ret:
                        return False,msg
            BKLog.i("write otp ok.")
        if safeinfo.efuse_enable:
            for efu in safeinfo.efuse_info_list:
                if efu.status=="true" and efu.mode=="write":
                    addr = int(efu.byte_addr,16)
                    value = int(efu.data,16)
                    ret,msg = security._write_efuse(addr,value)
                    if not ret:
                        return False,msg   
            BKLog.i("write efuse ok.")                
        if safeinfo.security_ctrl_enable:
            for sectrl in safeinfo.security_ctrl:
                slst = sectrl.split(',')
                if len(slst)==3:
                    if slst[2]=="1":
                        byte_idx = int(slst[0])
                        bit_idx = int(slst[1])
                        ret,msg = security._write_efuse_by_bit(byte_idx,bit_idx,1)
                        if not ret:
                            return False,msg
            BKLog.i("write efuse ok.")            
        return True,None
    

