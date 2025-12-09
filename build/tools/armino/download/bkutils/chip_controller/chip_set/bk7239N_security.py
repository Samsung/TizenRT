import time
from serial import Timeout
from ...bk_log import BKLog
from ...common.common import user_operate, bytes_reverse, BIT, BFD, bytes_reverse_per4bytes
from ...common.encrypt_helper import decrypt_random, encrypt_random
from .bk7236_security import BK7236_Security

class BK7239N_Security(BK7236_Security):
    def __init__(self, serial_instance):
        super().__init__(serial_instance)
        self.ser = serial_instance
        self.efuse_use_otp_addr = 0x4b10043C
        
    
    
    def _read_efuse(self,reg_addr,length):
        '''
        'length' is byte numbers
        'reg_addr' is pos of efuse
        '''
        ret,rv = self._read_otp_by_regaddr(self.efuse_use_otp_addr,4,False)
        if not ret:
            return False,None
        if length>int(self.security_efuse_length/8):
            length = int(self.security_efuse_length/8)
        return True, rv[reg_addr:reg_addr+length]
  
    def _write_efuse_by_bit(self,reg_addr,bit,value):
        '''
        write one byte value, 8 bits
        'addr' is the pos of efuse
        '''       
        msg = None
        ret, fullreadv = self._read_efuse(0,4)    
        if not ret:
            #BKLog.e(f"Read efuse fail, address: 0x{reg_addr:x}.")
            msg = f"Read efuse fail, address: 0x{reg_addr:x}."
            return False,msg
       
        readv = fullreadv[0]
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
        writev = fullreadv
        writev[reg_addr] = readv
        if not self._write_otp_by_regaddr(self.efuse_use_otp_addr,writev,False):          
            return False,"Write efuse fail, address: 0x{reg_addr:x}."
        
        ret, fullreadv = self._read_efuse(0,4)
        
        if not ret:
            #BKLog.e(f"Read efuse fail, address: 0x{reg_addr:x}.")
            msg = f"Read efuse fail, address: 0x{reg_addr:x}."
            return False,msg
               
        if writev != fullreadv:           
            msg = f"Compare efuse fail, address: 0x{reg_addr:x}."
            return False,msg

        return True,None
    
    def _write_efuse(self,reg_addr,value):
        '''
        write one byte value, 8 bits
        'addr' is the pos of efuse
        '''

        msg = None
        ret, fullreadv = self._read_efuse(0,4)    
        if not ret:
            #BKLog.e(f"Read efuse fail, address: 0x{reg_addr:x}.")
            msg = f"Read efuse fail, address: 0x{reg_addr:x}."
            return False,msg
                
        readv = fullreadv[reg_addr:reg_addr+1]       
        readv &= 0xff
        BKLog.i(f"Read efuse original value: 0x{readv:x}, address: 0x{reg_addr:x}.")
        
        for bit in range(8):
            #1->0, return fail
            if value&BIT(bit)==0 and readv&BIT(bit)==BIT(bit):
                BKLog.i(f"address: 0x{reg_addr:x}, bit {bit}.")                          
                return False,"Write efuse ignore, already written to 1."
        if readv== value:
            BKLog.i(f"address: 0x{reg_addr:x}, bit {bit}.")
            BKLog.w("Write efuse ignore, already written same data.")
            return True,None
                    
        readv |= value   
        writev = fullreadv
        writev[reg_addr] = readv
        if not self._write_otp_by_regaddr(self.efuse_use_otp_addr,writev,False):          
            return False,"Write efuse fail, address: 0x{reg_addr:x}."
        
        ret, fullreadv = self._read_efuse(0,4)        
        if not ret:
            #BKLog.e(f"Read efuse fail, address: 0x{reg_addr:x}.")
            msg = f"Read efuse fail, address: 0x{reg_addr:x}."
            return False,msg

        if fullreadv != writev:
            #BKLog.e(f"Compare efuse fail, address: 0x{reg_addr:x}.")
            msg = f"Compare efuse fail, address: 0x{reg_addr:x}."
            return False,msg

        return True,None
    
 

 