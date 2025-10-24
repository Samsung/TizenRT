import time
from serial import Timeout
from .base_chip import BaseChip
from .bk7236 import BK7236
from .bk7236N_security import BK7236N_Security
from ...protocol.bootrom import v2_0 as V2_0_BOOTROM_PROTOCOL
from ...bk_log import BKLog
from ...common.common import FLASH_SIZE_DEF, LINKCHECK,RESET_TYPE,FLASH_ERASE_CMD_EXT, FLASH_ERASE_CMD, safe_data, BinInfo,is_buf_all_0xff, BIT, BFD, crc32_ver2, bytes_reverse,bytes_reverse_per4bytes
from ...chip_controller import BaseController,FlashController,RegController

import binascii

class BK7236N(BK7236):

    CHIP_ID = 0x7236
    SECOND_ID = 0x24c00020
    BOOTROM_PROTOCOL = V2_0_BOOTROM_PROTOCOL

    def __init__(self):
        super().__init__()
    

############################################################################################################
# security operation
############################################################################################################

    
    def write_chip_otp(self, otp_addr:int, content:str, reverse_content=False,bigendian=False):
        '''
        write otp by address information
        '''
        security = BK7236N_Security(serial_instance=self.ser)
        return security._write_chip_otp(otp_addr,content,reverse_content,bigendian)
               
    def set_otp_permission_oplen(self, reg_addr, oplen, permission):
        '''
        reg_addr: start OTP address
        oplen: reg count
        permission: wr/ro/na
        '''
        security = BK7236N_Security(serial_instance=self.ser)
        return security._set_otp_permission_oplen(reg_addr,oplen,permission)
           

    def write_efuse_by_bit(self,reg_addr,bit,value):
        '''
        write one byte value, 8 bits
        'addr' is the pos of efuse
        '''
        security = BK7236N_Security(serial_instance=self.ser)
        return security._write_efuse_by_bit(reg_addr,bit,value)
    
    def write_efuse(self,reg_addr,value):
        '''
        write one byte value, 8 bits
        'addr' is the pos of efuse
        '''
        security = BK7236N_Security(serial_instance=self.ser)
        return security._write_efuse(reg_addr,value)

    
    def read_chip_otp(self,otp_addr, oplen, reverse_content=False,bigendian=False):
        security = BK7236N_Security(serial_instance=self.ser)
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
        security = BK7236N_Security(serial_instance=self.ser)
        if not security._set_otp_write_enable():
            BKLog.e(f"Set OTP write enable fail.")
            return False, None
        time.sleep(0.1)
        ret, val = security._read_otp_by_regaddr(0x4b1007c0,5)
        if not ret:
            BKLog.e(f"Read OTP fail, address: 0x{security.security_otp_enable_addr:x}.")
            return False, None
        
        return True,val
    
    def read_efuse(self,addr,oplen):
        '''
        read chip efuse info
        'addr' is start efuse address
        if 'oplen' set to 0, read all efuse value
        '''
        
        security = BK7236N_Security(serial_instance=self.ser)
        if oplen==0:
            byteslen = int(security.security_efuse_length/8)
        
        ret,val = security._read_efuse(addr,byteslen)
        if not ret:
            BKLog.e(f"Read efuse fail.")
            return False,None
        return True,val

    def write_safe_json(self, safeinfo: safe_data):
        '''
        write safe info set by json file
        '''
        security = BK7236N_Security(serial_instance=self.ser)
        if safeinfo.user_operate_enable:
            for usrop in safeinfo.user_operate_list:
                if usrop.status=="true":
                    if usrop.mode=="write":
                        #write otp info
                        if usrop.name.find("otp")!=-1:
                            ret,msg = security._write_otp_info(usrop)
                            if not ret:
                                return False,msg
                        #write flash info
                        elif usrop.name.find("flash")!=-1:
                            pass
                    else:
                        #read otp
                        if usrop.name.find("otp")!=-1:
                            ret,msg = security._read_otp_info(usrop)
                            if not ret:
                                return False,msg
                            BKLog.w(f'read {usrop.name}: {ret}')
                        #write flash info
                        elif usrop.name.find("flash")!=-1:
                            pass

            BKLog.i("write otp ok.")
        if safeinfo.security_data_enable:
            for secdata in safeinfo.security_data_list:                
                if secdata.status=="true":
                    if secdata.mode=="write":
                        ret,msg = security._write_otp_info(secdata)
                        if not ret:
                            return False,msg
                    else:
                        #read otp                        
                        ret,msg = security._read_otp_info(secdata)
                        if not ret:
                            return False,msg
                        BKLog.w(f'read {secdata.name}: {ret}')                        
            BKLog.i("write otp ok.")
        if safeinfo.efuse_enable:
            for efu in safeinfo.efuse_info_list:
                if efu.status=="true":
                    if efu.mode=="write":
                        addr = int(efu.byte_addr,16)
                        value = int(efu.data,16)
                        ret,msg = security._write_efuse(addr,value)
                        if not ret:
                            return False,msg
                    else:
                        addr = int(efu.byte_addr,16)
                        length = int(efu.length,16)
                        ret,efuse_value = security._read_efuse(addr,length)
                        if not ret:
                            return False,None
                        BKLog.w(f'read {efu.name}: {bytes.hex(bytes(efuse_value))}')  
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

    
