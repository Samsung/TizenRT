import time
from serial import Timeout
from .bk7236 import BK7236
from .bk7236Q_security import BK7236Q_Security
from ...protocol.bootrom import v2_0 as V2_0_BOOTROM_PROTOCOL
from ...bk_log import BKLog
from ...common.common import FLASH_SIZE_DEF, LINKCHECK,FLASH_ERASE_CMD_EXT, FLASH_ERASE_CMD, safe_data,BinInfo, is_buf_all_0xff, BIT, BFD, crc32_ver2,crc32bytes, bytes_reverse,bytes_reverse_per4bytes
from ...chip_controller import BaseController,FlashController,RegController

#import binascii

class BK7236Q(BK7236):

    CHIP_ID = 0x7236
    SECOND_ID = 0x25300020
    BOOTROM_PROTOCOL = V2_0_BOOTROM_PROTOCOL

    def __init__(self):
        super().__init__()
           
    
############################################################################################################
# security operation
############################################################################################################

    def write_chip_otp(self, otp_addr:int, content:str, reverse_content=False, bigendian=False):
        '''
        write otp by address information
        '''
        raise Exception("not support operation.")
    
               
    def set_otp_permission_oplen(self, reg_addr, oplen, permission):
        '''
        reg_addr: start OTP address
        oplen: reg count
        permission: wr/ro/na
        '''
        raise Exception("not support operation.")
           

    def write_efuse_by_bit(self,reg_addr,bit,value):
        '''
        write one byte value, 8 bits
        'addr' is the pos of efuse
        '''
 
        raise Exception("not support operation.")
    
    def write_efuse(self,efuse_addr,value):
        '''
        write several bytes value
        'addr' is the efuse index
        '''
        security = BK7236Q_Security(serial_instance=self.ser)
        return security._write_efuse(efuse_addr,value)

    
    def read_chip_otp(self,otp_addr, oplen, reverse_content=False,bigendian=False):
        raise Exception("not support operation.")


    def read_chip_uid(self):
        '''
        read chip UID from OTP, length: 5 bytes
        '''
        raise Exception("not support operation.")
    
    def read_efuse(self,addr,oplen):
        '''
        read chip efuse info
        'addr' is start efuse address
        if 'oplen' set to 0, read all efuse value
        '''
        
        security = BK7236Q_Security(serial_instance=self.ser)
        if oplen==0:
            oplen = int(security.security_efuse_length/8)
        return security._read_efuse(addr,oplen)
        

    def write_safe_json(self, safeinfo: safe_data):
        '''
        write safe info set by json file
        '''
        security = BK7236Q_Security(serial_instance=self.ser)
        if safeinfo.user_operate_enable:
            for usrop in safeinfo.user_operate_list:
                if usrop.status=="true":
                    raise Exception('Not support operation!')

            BKLog.i("write otp ok.")
        if safeinfo.security_data_enable:
            for secdata in safeinfo.security_data_list:                
                if secdata.status=="true":
                    if secdata.mode=="write":
                        ret,msg = security._write_efuse_info(secdata)
                        if not ret:
                            return False,msg
                    else:
                        #read otp                        
                        ret,msg = security._read_efuse_info(secdata)
                        if not ret:
                            return False,msg
                        BKLog.w(f'read {secdata.name}: {ret}')                        
            BKLog.i("write security data ok.")
        if safeinfo.efuse_enable:
            for efu in safeinfo.efuse_info_list:
                if efu.status=="true":
                    if efu.mode=="write":
                        addr = int(efu.byte_addr,16)
                        value = int(efu.data,16)
                        ret,msg = security._write_efuse_security_ctrl(addr,value)
                        if not ret:
                            return False,msg
                    else:
                        addr = int(efu.byte_addr,16)
                        length = int(efu.length,16)
                        fullv,efuse_value,msg = security._read_efuse_security_ctrl(addr)
                        if efuse_value is None:
                            return False,msg
                        BKLog.w(f'read {efu.name}: 0x{efuse_value:x}')  
            BKLog.i("write efuse ok.")                
        if safeinfo.security_ctrl_enable:
            for sectrl in safeinfo.security_ctrl:
                slst = sectrl.split(',')
                if len(slst)==3:
                    if slst[2]=="1":
                        byte_idx = int(slst[0])
                        bit_idx = int(slst[1])
                        ret,msg = security._write_efuse_security_ctrl_bit(byte_idx,bit_idx,1)
                        if not ret:
                            return False,msg
            BKLog.i("write security ctrl ok.")            
        return True,None
    

