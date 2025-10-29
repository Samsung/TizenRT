import time
from serial import Timeout
from ...bk_log import BKLog
from ...common.common import user_operate, bytes_reverse, BIT, BFD, bytes_reverse_per4bytes
from ...common.encrypt_helper import decrypt_random, encrypt_random
from ..reg_controller import RegController

class BK7236_Security(RegController):
    def __init__(self, serial_instance):
        super().__init__(serial_instance)
        self.ser = serial_instance
        self.security_efuse_base = 0x44880010
        self.security_efuse_vddpos = 0x1F
        self.security_otp_enable_addr = 0x44010040
        self.security_efuse_start_addr = 0x0 #bit address
        self.security_efuse_end_addr = 0x1F #bit address
        self.security_efuse_length = 0x20 #bit number
        self.security_otp_base = 0x4b100000
        self.security_otp_apb_start_addr = 0x4b100400
        self.security_otp_apb_end_addr = 0x4b1007FC
        self.security_otp_apb_length = 0x200
        self.security_otp_ahb_start_addr = 0x4b010000
        self.security_otp_ahb_end_addr = 0x4b010BFC
        self.security_otp_ahb_length = 0x300
        self.security_otp_busy_addr = 0x4b1002C4
        self.security_otp_busy_pos = 0x0
        

    def _set_efuse_write_enable(self,enable:bool):
        
        if enable:
            tmp=1
        else:
            tmp=0
        if not self._set_reg_bit_val(self.security_efuse_base,self.security_efuse_vddpos,1,tmp):
            return False
        return True

    def _set_otp_write_enable(self):
        if not self._set_reg_bit_val(self.security_efuse_base,self.security_efuse_vddpos,1,1):
            return False
        
        time.sleep(0.05)

        res = self.read_reg(self.security_otp_enable_addr)
        if res is None:
            BKLog.e(f"Read reg fail, address: 0x{self.security_otp_enable_addr:x}.")
            return False
        res &= ~0x8
        if not self.write_reg(self.security_otp_enable_addr,res):
            BKLog.e(f"Write reg fail, address: 0x{self.security_otp_enable_addr:x}.")
            return False
        
        if not self._wait_for_busy_bit(self.security_otp_busy_addr,self.security_otp_busy_pos):
            return False
        return True
    
    def _set_otp_write_disable(self):
        if not self._set_reg_bit_val(self.security_efuse_base,self.security_efuse_vddpos,1,0):
            return False
        
        if not self._wait_for_busy_bit(self.security_otp_busy_addr,self.security_otp_busy_pos):
            return False
        
        if not self.write_reg(0x4B1002C8,0):
            BKLog.e(f"Write reg fail, address: 0x4B1002C8.")
            return False
        
        time.sleep(0.01)

        res = self.read_reg(self.security_otp_enable_addr)
        if res is None:
            BKLog.e(f"Read reg fail, address: 0x{self.security_otp_enable_addr:x}.")
            return False
        res |= 0x8
        if not self.write_reg(self.security_otp_enable_addr,res):
            BKLog.e(f"Write reg fail, address: 0x{self.security_otp_enable_addr:x}.")
            return False
        
        return True
    

    def _write_otp_by_regaddr(self,reg_addr,value:bytes,big_endian=False):
        '''
        write 4 bytes to each otp address
        'addr' is the start register address
        '''
        cnt = len(value)
        idx = 0
        while cnt>0:
            if big_endian:               
                wrv = (value[4*idx+3])+(value[4*idx+2]<<8)+(value[4*idx+1]<<16)+(value[4*idx]<<24)
            else:
                wrv = (value[4*idx])+(value[4*idx+1]<<8)+(value[4*idx+2]<<16)+(value[4*idx+3]<<24)
            if not self.write_reg(reg_addr+4*idx, wrv):
                BKLog.e(f"Write reg fail, address: 0x{reg_addr+4*idx:x}.")
                return False
            res = self.read_reg(reg_addr+4*idx)
            if res is None:
                BKLog.e(f"Read reg fail, address: 0x{reg_addr+4*idx:x}.")
                return False
            if res!=wrv:
                BKLog.e(f"Compare reg read and write fail, address: 0x{reg_addr+4*idx:x}.")
                return False
            cnt -= 4
            idx += 1

        return True
    
    def _read_otp_by_regaddr(self,reg_addr,length,big_endian=False):
        '''
        read each otp get 4 bytes length
        'addr' is the start register address
        '''
        cnt = length
        idx = 0
        value = bytearray()
        while cnt>0:
            res = self.read_reg(reg_addr+4*idx)
            if res is None:
                return False,None
            if big_endian:
                value.append((res>>24)&0xff)
                value.append((res>>16)&0xff)
                value.append((res>>8)&0xff)      
                value.append(res&0xff)              
            else:                                   
                value.append(res&0xff)
                value.append((res>>8)&0xff)
                value.append((res>>16)&0xff)
                value.append((res>>24)&0xff)            
            cnt -= 4
            idx += 1
        
        return True, value 
    
    def _read_ot_permission_value(self,reg_addr):
        '''
        reg_addr: OTP address
        return: permission value
        '''
        #msg = None
        if reg_addr>= self.security_otp_apb_start_addr and reg_addr<=self.security_otp_apb_end_addr:
            #per otp use 4 bits to set permission
            offset = int((reg_addr - self.security_otp_apb_start_addr)/4)
            pm_addr = self.security_otp_base+0x20*4+int(offset/8)*4
            bitoff = (offset%8)*4
        elif reg_addr>=self.security_otp_ahb_start_addr and reg_addr<=self.security_otp_ahb_end_addr:
            #32 otp use 4 bits to set permission
            offset = int((reg_addr - self.security_otp_ahb_start_addr)/4)
            pm_addr = self.security_otp_base+0x4*4+int(offset/256)*4
            bitoff = (int(offset/32)%8)*4
        else:
            #BKLog.e("Error, invalid OTP address: 0x{:x}.".format(reg_addr))           
            raise Exception("Error, invalid OTP address: 0x{:x}.".format(reg_addr))
        res = self.read_reg(pm_addr)
        if res is None:
            BKLog.e(f"Read reg fail, address: 0x{pm_addr:x}.")
            #msg = f"Read reg fail, address: 0x{pm_addr:x}."
            # return False,msg
            return None
        return (res>>bitoff) & 0xf
        
    def _set_otp_permission(self, reg_addr, permission):
        '''
        reg_addr: OTP address
        permission: wr/ro/na
        '''
        msg = None
        if reg_addr>= self.security_otp_apb_start_addr and reg_addr<=self.security_otp_apb_end_addr:
            #per otp use 4 bits to set permission
            offset = int((reg_addr - self.security_otp_apb_start_addr)/4)
            pm_addr = self.security_otp_base+0x20*4+int(offset/8)*4
            bitoff = (offset%8)*4
        elif reg_addr>=self.security_otp_ahb_start_addr and reg_addr<=self.security_otp_ahb_end_addr:
            #32 otp use 4 bits to set permission
            offset = int((reg_addr - self.security_otp_ahb_start_addr)/4)
            pm_addr = self.security_otp_base+0x4*4+int(offset/256)*4
            bitoff = (int(offset/32)%8)*4
        else:
            #BKLog.e("Error, invalid OTP address: 0x{:x}.".format(reg_addr))
            msg = "Error, invalid OTP address: 0x{:x}.".format(reg_addr)
            return False,msg
        
        if permission == "wr":
            perm = 0x0
        elif permission == "ro":
            perm = 0x3
        elif permission == "na":
            perm = 0xf
        else:
            #BKLog.e(f"Error, invalid OTP mode: {permission}.")
            msg = f"Error, invalid OTP mode: {permission}."
            return False,msg
        
        res = self.read_reg(pm_addr)
        if res is None:
            #BKLog.e(f"Read reg fail, address: 0x{pm_addr:x}.")
            msg = f"Read reg fail, address: 0x{pm_addr:x}."
            return False,msg
        current_pm = (res>>bitoff) & 0xf
        if perm < current_pm:
            BKLog.i(f"OTP permission address: 0x{pm_addr:x}, value: {current_pm}.")
            #BKLog.w("Set OTP permission ignore, already written different permission.")
            msg = "Set OTP permission ignore, already written different permission."
            return False,msg
        
        if perm == current_pm:
            BKLog.i(f"OTP permission address: 0x{pm_addr:x}, value: {current_pm}.")
            BKLog.w("Set OTP permission ignore, already written same permission.")   
            #msg = "Set OTP permission ignore, already written same permission." 
            return True,None
        
        res &= ~(0xf<<bitoff)
        res |= perm<<bitoff
        if not self.write_reg(pm_addr,res):
            #BKLog.e(f"Write reg fail, address: 0x{pm_addr:x}.")
            msg = f"Write reg fail, address: 0x{pm_addr:x}."
            return False,msg
        
        rdperm = self.read_reg(pm_addr)
        if rdperm is None:
            #BKLog.e(f"Read reg fail, address: 0x{pm_addr:x}.")
            msg = f"Read reg fail, address: 0x{pm_addr:x}."
            return False,msg
        if rdperm != res:
            msg = f"Compare write and read fail, address: 0x{pm_addr:x}"
            return False,msg
        
        return True,None

    def _check_otp_permission(self, reg_addr, read_write):
        '''
        reg_addr: OTP address
        read_write: True for read and False for write
        '''
        msg = None
        if reg_addr>= self.security_otp_apb_start_addr and reg_addr<=self.security_otp_apb_end_addr:
            #per otp use 4 bits to set permission
            offset = int((reg_addr - self.security_otp_apb_start_addr)/4)
            pm_addr = self.security_otp_base+0x20*4+int(offset/8)*4
            bitoff = (offset%8)*4
        elif reg_addr>=self.security_otp_ahb_start_addr and reg_addr<=self.security_otp_ahb_end_addr:
            #32 otp use 4 bits to set permission
            offset = int((reg_addr - self.security_otp_ahb_start_addr)/4)
            pm_addr = self.security_otp_base+0x4*4+int(offset/256)*4
            bitoff = (int(offset/32)%8)*4
        else:
            msg = "Error, invalid OTP address: 0x{:x}.".format(reg_addr)
            return False,msg
              
        res = self.read_reg(pm_addr)
        if res is None:           
            msg = f"Read reg fail, address: 0x{pm_addr:x}."
            return False,msg
        current_pm = (res>>bitoff) & 0xf
        if current_pm == 0x0 or current_pm == 0x3 or current_pm == 0xf:
            BKLog.d(f"permission address: 0x{pm_addr:x}, value: {current_pm:x}")       
        else:
            msg = f"permission error, OTP permission address @0x{pm_addr:x}, value: {current_pm:x}."
            return False,msg

        if read_write:
            if current_pm ==0xf:
                msg = f"check OTP permission fail, OTP address: 0x{reg_addr:x} permission is na."
                return False,msg
        else:
            if current_pm==0x3 or current_pm==0xf:
                #BKLog.e(f"check OTP permission fail, OTP address: 0x{reg_addr:x} permission is ro/na.")
                msg = f"check OTP permission fail, OTP address: 0x{reg_addr:x} permission is ro/na."
                return False,msg
               
        return True,msg
    

    def _check_otp_permission_oplen(self, reg_addr, oplen, read_write):
        '''
        reg_addr: start OTP address
        oplen: reg count
        read_write: True for read and False for write
        '''
        BKLog.i(f"check OTP permission @0x{reg_addr:x}, length: {oplen}")
        for i in range(oplen):
            ret,msg = self._check_otp_permission(reg_addr+i*4,read_write)
            if not ret:
                return False,msg       
        return True,None
    
    def _set_otp_permission_oplen(self, reg_addr, oplen, permission):
        '''
        reg_addr: start OTP address
        oplen: reg count
        permission: wr/ro/na
        '''        
        BKLog.i(f"set OTP permission @0x{reg_addr:x}, length: {oplen}")
        msg = None
        for i in range(oplen):
            ret,msg = self._set_otp_permission(reg_addr+i*4,permission)
            if not ret:
                return False,msg
        
        return True,None

    def _read_chip_otp(self, otp_addr, oplen, bigendian=False):
        
        msg = None
        if not self._set_otp_write_enable():
            #BKLog.e("set_otp_write_enable fail.")
            msg = "set_otp_write_enable fail."
            return None,msg
        ret,msg = self._check_otp_permission_oplen(otp_addr,oplen, read_write=True)
        if not ret:           
            return None,msg
        ret,otp = self._read_otp_by_regaddr(otp_addr,oplen,bigendian)
        if ret:           
            return otp[:oplen],None
        else:
            msg = f'read otp fail, address: 0x{otp_addr:x}'
            return None,msg

    def _read_otp_info(self,usr_info:user_operate):
        '''
        read otp by the 'user_operate' information
        '''  
        address = int(usr_info.start_addr,16)
        length = int(usr_info.byte_len,16)
        ret,msg = self._read_chip_otp(address,length,usr_info.big_endian)

        if ret is not None:
            ss = ret
            if usr_info.reverse:
                ss = bytes_reverse_per4bytes(ret)
            sstr = bytes.hex(bytes(ss))
            if usr_info.data_type=="ascii":
                sstr = encrypt_random(sstr)
            return sstr,msg
        else:
            return None,msg


    def _write_otp_info(self,usr_info:user_operate):
        '''
        write otp by the 'user_operate' information
        '''
        ss = usr_info.data
        if usr_info.data_type=="hex":
            ss = usr_info.data
        elif usr_info.data_type=="ascii":
            ss = decrypt_random(usr_info.data)
        else:
            msg = "Error, OTP data type not support: {0}.".format(usr_info.data_type)
            return False,msg
        msg = None
        address = int(usr_info.start_addr,16)
        length = int(usr_info.byte_len,16)
        if length!=len(ss)/2:
            msg = "Error, OTP data length(0x{:x}) is not 0x{:x}.".format(len(usr_info.data),length)
            return False,msg       
        
        return self._write_chip_otp(address,ss,usr_info.reverse,usr_info.big_endian)
        
    
    def _write_chip_otp(self, otp_addr:int, content:str, reverse=False, bigendian=False):
        '''
        write otp by address information
        content: otp data to write (hex string)
        '''        
        msg = None       
        ss = bytes.fromhex(content)
        if reverse:
            ss = bytes_reverse_per4bytes(ss)
        length = len(ss)
        if otp_addr>=self.security_otp_apb_start_addr and otp_addr<=self.security_otp_apb_end_addr:
            if (otp_addr+length)>self.security_otp_apb_end_addr:
                #BKLog.e("Error, OTP address(0x{:x}) + length(0x{:x}) over the OTP end.".format(otp_addr,length))
                msg = "Error, OTP address(0x{:x}) + length(0x{:x}) over the OTP end.".format(otp_addr,length)
                return False,msg
        elif otp_addr>=self.security_otp_ahb_start_addr and otp_addr<=self.security_otp_ahb_end_addr:
            if (otp_addr+length)>self.security_otp_ahb_end_addr:
                #BKLog.e("Error, OTP address(0x{:x}) + length(0x{:x}) over the OTP end.".format(otp_addr,length))
                msg = "Error, OTP address(0x{:x}) + length(0x{:x}) over the OTP end.".format(otp_addr,length)
                return False,msg
        else:
            #BKLog.e("Error, invalid OTP address: 0x{:x}.".format(otp_addr))
            msg = "Error, invalid OTP address: 0x{:x}.".format(otp_addr)
            return False,msg
        
        #read otp data to check if need write
        ret,msg = self._read_chip_otp(otp_addr,length,bigendian)
        if ret is None:            
            return False,msg
        
        if ret==ss:
            BKLog.w("Write OTP ignore, already written same data.")
            #msg = "Write OTP ignore, already written same data."
            return True,None
        else:
            for idx in range(length):               
                if ret[idx]!=0:
                    #BKLog.w("Write OTP ignore, already written different data.")
                    msg = "Write OTP ignore, already written different data."
                    return False,msg

        BKLog.i("write OTP data: {0}.".format(ss))
        if not self._set_otp_write_enable():
            BKLog.e("set_otp_write_enable fail, address: 0x{:x}.".format(otp_addr))
            return False
        time.sleep(0.05)
        ret,msg = self._check_otp_permission_oplen(otp_addr,length,read_write=False)
        if not ret:            
            return False,msg
        if not self._write_otp_by_regaddr(otp_addr,ss,bigendian):
            #BKLog.e("write OTP fail, address: 0x{:x}.".format(otp_addr))
            msg = "write OTP fail, address: 0x{:x}.".format(otp_addr)
            return False,msg
        time.sleep(0.05)
        if not self._set_otp_write_disable():
            #BKLog.e("set OTP write disable fail, address: 0x{:x}.".format(otp_addr))
            msg = "set OTP write disable fail, address: 0x{:x}.".format(otp_addr)
            return False,msg

        return True,None

    def _read_efuse(self,reg_addr,length):
        '''
        'length' is byte numbers
        'reg_addr' is pos of efuse
        '''
        out_value = bytearray()
        for _ in range(length):
            reg_w = (reg_addr &0x3f)<<8
            reg_w |= (1<<0)
            reg_w |= (1<<31)
            if not self.write_reg(self.security_efuse_base,reg_w):
                BKLog.e(f"Write reg fail, address: 0x{self.security_efuse_base:x}.")
                return False, None 
            
            timeout = Timeout(1)
            while not timeout.expired():
                res = self.read_reg(self.security_efuse_base)
                if res is None:
                    BKLog.e(f"Read reg fail, address: 0x{self.security_efuse_base:x}.")
                    return False, None 
                if res&(1<<0) == 0:
                    break
                time.sleep(0.1)
            if res&(1<<0) != 0:
                BKLog.e(f"Read efuse timeout, address: 0x{self.security_efuse_base:x}.")
                return False, None 
            res = self.read_reg(self.security_efuse_base+1*4)
            if res is None:
                BKLog.e(f"Read reg fail, address: 0x{self.security_efuse_base+1*4:x}.")
                return False, None
            #BKLog.i(f"Read reg: 0x{res:x}, address: 0x{self.security_efuse_base+1*4:x}.") 
            if not (res & (1<<8)):
                BKLog.e(f"Read efuse fail, address: 0x{self.security_efuse_base+1*4:x}.")
                return False, None            
            out_value.append(res&0xff)
            reg_addr += 1

        return True, out_value
  
    def _write_efuse_by_bit(self,reg_addr,bit,value):
        '''
        write one byte value, 8 bits
        'addr' is the pos of efuse
        '''       
        msg = None
        ret, readv = self._read_efuse(reg_addr,1)    
        if not ret:
            #BKLog.e(f"Read efuse fail, address: 0x{reg_addr:x}.")
            msg = f"Read efuse fail, address: 0x{reg_addr:x}."
            return False,msg
       
        readv = readv[0]
        readv &= 0xff
        BKLog.i(f"Read efuse original value: 0x{readv:x}, address: 0x{reg_addr:x}.")
        if value==0:
            if readv&BIT(bit)==BIT(bit):
                BKLog.i(f"address: 0x{reg_addr:x}, bit {bit}.")
                #BKLog.e("Write efuse ignore, already written to 1.")
                msg = "Write efuse ignore, already written to 1."
                return False,msg
            else:
                BKLog.i(f"address: 0x{reg_addr:x}, bit {bit}.")
                BKLog.e("Write efuse ignore, already written same data.")
                return True,None
        else:
            if readv&BIT(bit)==BIT(bit):
                BKLog.i(f"address: 0x{reg_addr:x}, bit {bit}.")
                BKLog.e("Write efuse ignore, already written same data.")
                return True,None
                    
        readv |= BIT(bit)
        backup_value = readv
        writev = ((reg_addr & 0x3F)<<8)
        writev |= BFD(readv,16,8)
        writev |= BIT(1)
        writev |= BIT(self.security_efuse_vddpos)
        if not self.write_reg(self.security_efuse_base,writev):
            #BKLog.e(f"Write reg fail, address: 0x{self.efuse_base:x}.")
            msg = f"Write reg fail, address: 0x{self.security_efuse_base:x}."
            return False,msg
        
        writev |= BIT(0)
        if not self.write_reg(self.security_efuse_base,writev):
            #BKLog.e(f"Write reg fail, address: 0x{self.efuse_base:x}.")
            msg = f"Write reg fail, address: 0x{self.security_efuse_base:x}."
            return False,msg
        
        timeout = Timeout(1)
        while not timeout.expired():
            res = self.read_reg(self.security_efuse_base)
            if res is None:
                #BKLog.e(f"Read reg fail, address: 0x{self.efuse_base:x}.")
                msg = f"Read reg fail, address: 0x{self.security_efuse_base:x}."
                return False,msg
            if res&(1<<0) == 0:
                break
            time.sleep(0.1)
        if res&(1<<0) != 0:
            #BKLog.e(f"Read efuse timeout, address: 0x{self.efuse_base:x}.")
            msg = f"Read efuse timeout, address: 0x{self.security_efuse_base:x}."
            return False,msg
        
        ret, readv = self._read_efuse(reg_addr,1)
        
        if not ret:
            #BKLog.e(f"Read efuse fail, address: 0x{reg_addr:x}.")
            msg = f"Read efuse fail, address: 0x{reg_addr:x}."
            return False,msg
        readv = readv[0]
        #BKLog.e(f"read value {readv:x}, address: 0x{reg_addr:x}.")
        #BKLog.e(f"write value {backup_value:x}, address: 0x{reg_addr:x}.")
        if readv&0xff != backup_value:
            #BKLog.e(f"Compare efuse fail, address: 0x{reg_addr:x}.")
            msg = f"Compare efuse fail, address: 0x{reg_addr:x}."
            return False,msg

        return True,None
    
    def _write_efuse(self,reg_addr,value):
        '''
        write one byte value, 8 bits
        'addr' is the pos of efuse
        '''

        msg = None
        ret, readv = self._read_efuse(reg_addr,1)    
        if not ret:
            #BKLog.e(f"Read efuse fail, address: 0x{reg_addr:x}.")
            msg = f"Read efuse fail, address: 0x{reg_addr:x}."
            return False,msg
                
        readv = readv[0]       
        readv &= 0xff
        BKLog.i(f"Read efuse original value: 0x{readv:x}, address: 0x{reg_addr:x}.")
        
        for bit in range(8):
            #1->0, return fail
            if value&BIT(bit)==0 and readv&BIT(bit)==BIT(bit):
                BKLog.i(f"address: 0x{reg_addr:x}, bit {bit}.")
                #BKLog.e("Write efuse ignore, already written to 1.")   
                msg = BKLog.e("Write efuse ignore, already written to 1.")            
                return False,msg
        if readv== value:
            BKLog.i(f"address: 0x{reg_addr:x}, bit {bit}.")
            BKLog.w("Write efuse ignore, already written same data.")
            return True,None
                    
        readv |= value
        backup_value = readv
        writev = ((reg_addr & 0x3F)<<8)
        writev |= BFD(readv,16,8)
        writev |= BIT(1)
        writev |= BIT(self.security_efuse_vddpos)
        if not self.write_reg(self.security_efuse_base,writev):
            #BKLog.e(f"Write reg fail, address: 0x{self.efuse_base:x}.")
            msg = f"Write reg fail, address: 0x{self.security_efuse_base:x}."
            return False,msg
        
        writev |= BIT(0)
        if not self.write_reg(self.security_efuse_base,writev):
            #BKLog.e(f"Write reg fail, address: 0x{self.efuse_base:x}.")
            msg = f"Write reg fail, address: 0x{self.security_efuse_base:x}."
            return False,msg
        
        timeout = Timeout(1)
        while not timeout.expired():
            res = self.read_reg(self.security_efuse_base)
            if res is None:
                #BKLog.e(f"Read reg fail, address: 0x{self.efuse_base:x}.")
                msg = f"Read reg fail, address: 0x{self.security_efuse_base:x}."
                return False,msg
            if res&(1<<0) == 0:
                break
            time.sleep(0.1)
        if res&(1<<0) != 0:
            #BKLog.e(f"Read efuse timeout, address: 0x{self.efuse_base:x}.")
            msg = f"Read efuse timeout, address: 0x{self.security_efuse_base:x}."
            return False,msg
        
        ret, readv = self._read_efuse(reg_addr,1)        
        if not ret:
            #BKLog.e(f"Read efuse fail, address: 0x{reg_addr:x}.")
            msg = f"Read efuse fail, address: 0x{reg_addr:x}."
            return False,msg
        readv = readv[0]
        if readv&0xff != backup_value:
            #BKLog.e(f"Compare efuse fail, address: 0x{reg_addr:x}.")
            msg = f"Compare efuse fail, address: 0x{reg_addr:x}."
            return False,msg

        return True,None
    
 

 