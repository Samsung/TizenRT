from math import fabs
import time
from serial import Timeout

from bkutils.chip_controller import reg_controller
#from .base_chip import BaseChip
from .bk7236 import BK7236
from .bk7239N_security import BK7239N_Security
from ...protocol.bootrom import v2_0 as V2_0_BOOTROM_PROTOCOL
from ...bk_log import BKLog
from ...common.common import LINKCHECK,RESET_TYPE, safe_data, PM_CPU_FREQ_LIST, PM_FLASH_FREQ_LIST, \
    PROGRAMING_MODE_LIST, BIT, bytes_reverse_per4bytes, crc32bytes,BinInfo
from ...chip_controller import BaseController,RegController,AdvancedController

import binascii

class BK7239N(BK7236):

    CHIP_ID = 0x7236
    SECOND_ID = [0x24400030,0x24c00020,0x24c00030,0x25750920,0x25750820,0x25730020]
    BOOTROM_PROTOCOL = V2_0_BOOTROM_PROTOCOL

    def __init__(self):
        super().__init__()
        self.sys_reg_base = 0x44010000
        self.uart_0_reg_base = 0x44820000
        self.uart_config_clk_divid = 8

    
    def uart_select_clock(self, new_freq:int,old_freq:int, baudrate:int):
        regctrl = RegController(self.ser)
        #start_time = time.perf_counter()
        val_r = regctrl.read_reg(self.uart_0_reg_base+4*4)
        if val_r is None:
            return False
        #print(f'spend time: {time.perf_counter()-start_time} s')
        val_r &= ~(0xFFFF00)
        baud_div = int(new_freq/baudrate) - 1
        val_r |= (baud_div << self.uart_config_clk_divid)
        regctrl.write_reg(self.uart_0_reg_base+4*4,val_r,delay_ms=50,retry=1)
            #return False
        #print(f'spend time: {time.perf_counter()-start_time} s')
        actual_baud = int(old_freq/(baud_div+1))       
        self.ser.reset(actual_baud)
        #print(f'spend time: {time.perf_counter()-start_time} s')
        time.sleep(0.01)
        val_actual_r = regctrl.read_reg(self.uart_0_reg_base+4*4)
        if val_actual_r is None or val_actual_r!= val_r:
            return False
        #uart change to apll
        val_r = regctrl.read_reg(self.sys_reg_base+0xa*4)
        if val_r is None:
            return False
        #print(f'spend time: {time.perf_counter()-start_time} s')
        val_r |= (1<<12)
        regctrl.write_reg(self.sys_reg_base+0xa*4,val_r,delay_ms=50,retry=1)
            #return False
        #print(f'spend time: {time.perf_counter()-start_time} s')
        self.ser.reset(baudrate)
        time.sleep(0.01)
        val_actual_r = regctrl.read_reg(self.sys_reg_base+0xa*4)
        if val_actual_r is None or val_actual_r!= val_r:
            return False
        #print(f'spend time: {time.perf_counter()-start_time} s')
        return True

    def set_high_baudrate(self, baudrate):
        adctrl = AdvancedController(self.ser)
        if not adctrl.high_freq_enable(PROGRAMING_MODE_LIST.PROGRAMING_DBUS,PM_CPU_FREQ_LIST.PM_CPU_FREQ_120M,PM_FLASH_FREQ_LIST.PM_FLASH_FREQ_40M):
            return False
        if not self.uart_select_clock(80000000,40000000,baudrate):
            return False
        return True

    def set_baudrate(self, baudrate, delay_ms=20,max_retry_cnt=5,isBl1:bool=True):
        if isBl1:
            if baudrate != self.ser.baudrate:  
                if baudrate<=2600000:         
                    cnt = max_retry_cnt
                    while cnt>0:
                        self.ser.drain()
                        sbp = self.BOOTROM_PROTOCOL.SetBaudrateProtocol()
                        self.ser.write_cmd(sbp.cmd(baudrate, delay_ms))
                        time.sleep(delay_ms/2000)
                        self.ser.switch_baudrate(baudrate)
                        ret_content = self.ser.wait_for_cmd_response(sbp.expect_length, delay_ms/1000 + 0.5)
                        if len(ret_content) == sbp.expect_length:
                            if sbp.response_check(ret_content, baudrate):
                                return True
                        base_ctrl = BaseController(serial_instance=self.ser)
                        if base_ctrl.do_link_check(linkcheck_obj=LINKCHECK.BOOTROM,max_try_count=10):
                            return True
                        
                        BKLog.w('reconnect bus to retry')
                        if not base_ctrl.reconnect_bus(reset_type=RESET_TYPE.DTR_RTS,linkcheck_obj=LINKCHECK.BOOTROM,max_retry_count=20):
                            BKLog.e('reconnect bus fail')
                            BKLog.e('baudrate switch fail')
                            return False
                        cnt-=1
                else:
                    BKLog.w('set high baudrate enable')
                    if self.set_high_baudrate(baudrate):
                        BKLog.w(f'set high baudrate:{baudrate} OK')
                        return True 
                                    
                BKLog.e('baudrate switch fail')
                return False
            else:
                BKLog.i('ignore baudrate switch')
                return True
        else:
            if baudrate != self.ser.baudrate:           
                cnt = max_retry_cnt
                while cnt>0:
                    self.ser.drain()
                    sbp = self.BOOTROM_PROTOCOL.SetBaudrateProtocol()
                    self.ser.write_cmd(sbp.cmd(baudrate, delay_ms))
                    time.sleep(delay_ms/2000)
                    self.ser.switch_baudrate(baudrate)
                    ret_content = self.ser.wait_for_cmd_response(sbp.expect_length, delay_ms/1000 + 0.5)
                    if len(ret_content) == sbp.expect_length:
                        if sbp.response_check(ret_content, baudrate):
                            return True
                    base_ctrl = BaseController(serial_instance=self.ser)
                    if base_ctrl.do_link_check(linkcheck_obj= BaseController.link_check_obj if BaseController.link_check_obj else LINKCHECK.BOOTROM):
                        return True
                    cnt-=1
                    BKLog.e('reconnect bus to retry')
                    if not base_ctrl.reconnect_bus(reset_type=RESET_TYPE.MULTI,linkcheck_obj=BaseController.link_check_obj if BaseController.link_check_obj else LINKCHECK.BOOTROM,max_retry_count=20):
                        BKLog.e('reconnect bus fail')
                        BKLog.e('baudrate switch fail')
                        return False                   
                BKLog.e('baudrate switch fail')
                return False

    def verify_chip_product_id(self,binfile:BinInfo):
        '''
        read 16 bytes from bin 0x108(0x118 if with crc)
        compare with the otp product id (ahb offset: 0x58c~0x59c)
        '''
        offset = 0x108
        if crc32bytes(binfile.pfile[0:0x20])==bytearray(binfile.pfile[0:0x22]):
            offset = 0x118
        binid = binfile.pfile[offset:offset+16]
        #read from otp
        chipsec = BK7239N_Security(serial_instance=self.ser)
        otpid,msg = self.read_chip_otp(chipsec.security_otp_ahb_start_addr+0x58c,16)
        #BKLog.w(f'product id in otp is: {bytes.hex(bytes(otpid))}')
        if otpid is None:
            raise Exception(f'read chip otp fail: 0x{chipsec.security_otp_ahb_start_addr+0x58c}')
        
        if binid != b'\x00'*16 and otpid!=b'\x00'*16 and binid!=otpid:
            raise Exception('verify the product id fail, stop download.')
        

############################################################################################################
# security operation
############################################################################################################
    def check_aes_key_enable(self):
        security = BK7239N_Security(serial_instance=self.ser)        
        ret,val = security._read_efuse(3,1)
        if not ret:
            BKLog.e(f"Read efuse fail.")
            return False
        if val[0]&BIT(5)!=BIT(5):
            return 0      
        return 1
    
    def check_aes_key_otp_data(self):
        '''
        return False for execute fail, 0 for empty data, 1 for data is written
        '''
        security = BK7239N_Security(serial_instance=self.ser)
        ret,msg = security._read_chip_otp(security.security_aes_key_addr,0x20)
        if ret is None:
            BKLog.e(f"Read otp fail.")
            return False
        for idx in range(0x20):
            if ret[idx]!=0:
                return 1
        return 0
    
    def check_aes_key_otp_permission(self):
        '''
        return False for execute fail, 0 for all permission is 0, 1 for permission is 0xf
        '''
        security = BK7239N_Security(serial_instance=self.ser)
        
        pm = security._read_ot_permission_value(security.security_aes_key_addr)
        if pm is None:
            BKLog.e(f"Read otp permission fail.")
            return False
        if pm == 0xf:
            return 1
        else:
            return 0
    
    
    def write_chip_otp(self, otp_addr:int, content:str, reverse_content=False,bigendian=False):
        '''
        write otp by address information
        '''
        security = BK7239N_Security(serial_instance=self.ser)
        return security._write_chip_otp(otp_addr,content,reverse_content,bigendian)
               
    def set_otp_permission_oplen(self, reg_addr, oplen, permission):
        '''
        reg_addr: start OTP address
        oplen: reg count
        permission: wr/ro/na
        '''
        security = BK7239N_Security(serial_instance=self.ser)
        return security._set_otp_permission_oplen(reg_addr,oplen,permission)
           

    def write_efuse_by_bit(self,reg_addr,bit,value):
        '''
        write one byte value, 8 bits
        'addr' is the pos of efuse
        '''
        security = BK7239N_Security(serial_instance=self.ser)
        return security._write_efuse_by_bit(reg_addr,bit,value)
    
    def write_efuse(self,reg_addr,value):
        '''
        write one byte value, 8 bits
        'addr' is the pos of efuse
        '''
        security = BK7239N_Security(serial_instance=self.ser)
        return security._write_efuse(reg_addr,int(value,16))

    
    def read_chip_otp(self,otp_addr, oplen, reverse_content=False,bigendian=False):
        security = BK7239N_Security(serial_instance=self.ser)
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
        security = BK7239N_Security(serial_instance=self.ser)
        if not security._set_otp_write_enable():
            BKLog.e(f"Set OTP write enable fail.")
            return False, None
        time.sleep(0.1)
        ret, val = security._read_otp_by_regaddr(0x4b0105F8,5)
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
        
        security = BK7239N_Security(serial_instance=self.ser)
        if oplen==0:
            byteslen = int(security.security_efuse_length/8)
        else:
            byteslen = oplen
        
        ret,val = security._read_efuse(addr,byteslen)
        if not ret:
            BKLog.e(f"Read efuse fail.")
            return False,None
        return True,val

    def write_safe_json(self, safeinfo: safe_data):
        '''
        write safe info set by json file
        '''
        security = BK7239N_Security(serial_instance=self.ser)
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

    
