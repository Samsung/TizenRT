import time
from serial import Timeout
from .serial_helper import SerialHelper
from ..protocol.bootrom import v2_0 as V2_0_BOOTROM_PROTOCOL


class RegController(object):
    # 与寄存器读写相关的控制操作
    BOOTROM_PROTOCOL = V2_0_BOOTROM_PROTOCOL
    def __init__(self, serial_instance: SerialHelper) -> None:
        self.serial_instance = serial_instance
        
  
    
    def read_reg(self, reg_addr, delay_ms=500, retry=3):
        reg_read_protocol = self.BOOTROM_PROTOCOL.ReadRegProtocol()
        cnt = retry
        while cnt>=0:
            tmp_res, content = self.serial_instance.write_cmd_and_wait_response(reg_read_protocol.cmd(reg_addr),
                                                                    reg_read_protocol.expect_length,
                                                                    delay_ms/1000)
            if tmp_res and reg_read_protocol.response_check(content,reg_addr):
                return reg_read_protocol.get_reg_val(content)
            cnt -= 1
        return None
    
    def write_reg(self, reg_addr, reg_val, delay_ms=500, retry=3):
        reg_write_protocol = self.BOOTROM_PROTOCOL.writeRegProtocol()
        cnt = retry
        while cnt>=0:
            tmp_res, content = self.serial_instance.write_cmd_and_wait_response(reg_write_protocol.cmd(reg_addr, reg_val),
                                                                    reg_write_protocol.expect_length,
                                                                    delay_ms/1000)
            if tmp_res and reg_write_protocol.response_check(content,reg_addr):
                return True
            cnt -= 1
        return False
    
    

    def _set_reg_bit_val(self,addr,pos,mask,value):
        res = self.read_reg(addr)
        if res is None:
            return False
        res &= ~(mask<<pos)
        res |= ((value&mask)<<pos)
        return self.write_reg(addr,res)
    
    def _wait_for_busy_bit(self,addr,pos,timeout=10):
        '''
        wait for otp busy=0, default time = 10 s
        '''
        timeout = Timeout(timeout)
        while not timeout.expired():
            res = self.read_reg(addr)
            if res is None:
                return False
            if res&(1<<pos)==0:
                return True
            time.sleep(0.05)
        return False
    