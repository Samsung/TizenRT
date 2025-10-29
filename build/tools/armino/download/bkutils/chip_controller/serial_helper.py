import serial
import binascii
import time
from serial import Timeout

from ..common.common import get_os_type
from ..bk_log.log_utils import BKLog


class SerialHelper(object):
     
    def __init__(self):
        self.ser = None

    def connect(self, port, baudrate, timeout):
        os_type = get_os_type()
        if os_type == 'Windows':
            self.ser = serial.Serial(f'COM{port}', baudrate, timeout=timeout)
        else:
            self.ser = serial.Serial(port, baudrate, timeout=timeout)
    
    @property
    def baudrate(self):
        return self.ser.baudrate

    def switch_baudrate(self, baudrate):
        if baudrate != self.ser.baudrate:
            self.ser.baudrate = baudrate

    def disconnect(self):
        self.ser.close()

    def reset(self, baudrate=None):
        port = self.ser.port
        if baudrate is None:
            baudrate = self.ser.baudrate
        self.disconnect()
        time.sleep(0.01)
        os_type = get_os_type()
        if os_type == 'Windows':
            self.ser = serial.Serial(port, baudrate, timeout=0)
        else:
            self.ser = serial.Serial(port, baudrate, timeout=0)

    def write_cmd(self, cmd):
        BKLog.d('TX: {0}'.format(binascii.b2a_hex(cmd)))
        self.ser.write(cmd)

    def drain(self):
        self.ser.reset_input_buffer()

    
    def wait_for_cmd_response_free(self, expect_length, timeout_sec=1):
        read_buf = b''
        strat_time = time.perf_counter()
        while time.perf_counter()-strat_time<timeout_sec:           
            buf = self.ser.read(expect_length-len(read_buf))              
            read_buf += buf        
            if len(read_buf) >= expect_length:
                break
                    
        BKLog.d('SERIAL EXPECT LENTH: {0}, RECEIVED LENTH: {1}, CONTENT: {2}'.format(expect_length, len(read_buf), binascii.b2a_hex(read_buf)))
        return read_buf
    
    def wait_for_cmd_response(self, expect_length, timeout_sec=1):
        read_buf = b''
        strat_time = time.perf_counter()
        valid_head = False
        while time.perf_counter()-strat_time<timeout_sec:

            buf = self.ser.read(expect_length-len(read_buf))                     
            read_buf += buf
            if not valid_head and read_buf!=bytes([0x04]):
                for idx in range(len(read_buf)-1):
                    if read_buf[idx:idx+2]==bytes([0x04,0x0e]):
                        valid_head=True
                        read_buf=read_buf[idx:]
                        break
                if not valid_head:
                    read_buf=b''
            if len(read_buf) >= expect_length:
                break
                    
        BKLog.d('SERIAL EXPECT LENTH: {0}, RECEIVED LENTH: {1}, CONTENT: {2}'.format(expect_length, len(read_buf), binascii.b2a_hex(read_buf)))
        return read_buf
    
    

    def write_cmd_and_wait_response(self, cmd, expect_length, timeout_sec=0.1):
        self.drain()
        self.write_cmd(cmd)
        tmp_res = False       
        ret_content = self.wait_for_cmd_response(expect_length=expect_length, timeout_sec=timeout_sec)
        if len(ret_content) == expect_length:
            tmp_res = True

        return tmp_res, ret_content 
    

    def write_cmd_and_wait_response_without_drain(self, cmd, expect_length, timeout_sec=0.1):
        #self.drain()
        self.write_cmd(cmd)
        tmp_res = False       
        ret_content = self.wait_for_cmd_response(expect_length=expect_length, timeout_sec=timeout_sec)
        if len(ret_content) == expect_length:
            tmp_res = True

        return tmp_res, ret_content
    