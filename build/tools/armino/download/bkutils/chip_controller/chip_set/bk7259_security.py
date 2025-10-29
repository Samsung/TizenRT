import time
from serial import Timeout
from .bk7236_security import BK7236_Security
from ...bk_log import BKLog
from ...common.common import user_operate, bytes_reverse, BIT, BFD
from ..reg_controller import RegController

class BK7259_Security(BK7236_Security):
    def __init__(self, serial_instance):
        super().__init__(serial_instance)
        self.ser = serial_instance
        self.security_efuse_base = 0x44850010
        self.security_efuse_vddpos = 0x1F
        self.security_otp_enable_addr = 0x44010040
        self.security_otp_enable_pos = 0x2
        self.security_efuse_start_addr = 0x0 #bit address
        self.security_efuse_end_addr = 0x1F #bit address
        self.security_efuse_length = 0x20 #bit number
        self.security_otp_base = 0x42100000
        self.security_otp_apb_start_addr = 0x42100400
        self.security_otp_apb_end_addr = 0x421007FC
        self.security_otp_apb_length = 0x200
        self.security_otp_ahb_start_addr = 0x42010000
        self.security_otp_ahb_end_addr = 0x42010BFC
        self.security_otp_ahb_length = 0x300
        self.security_otp_busy_addr = 0x421002C4
        self.security_otp_busy_pos = 0x0
        

    
 

 