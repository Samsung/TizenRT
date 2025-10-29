from abc import ABC, abstractmethod
from .....bk_log import BKLog


class BaseBootRomFlashProtocol(ABC):

    def __init__(self):
        self.base_tx_header = [0x01, 0xe0, 0xfc, 0xff, 0xf4]
        self.base_rx_header = [0x04, 0x0e, 0xff, 0x01, 0xe0, 0xfc, 0xf4]
        self.STATUS_INFO = [
            {
                'code': 0x0,
                'desc': 'normal'
            },
            {
                'code': 0x1,
                'desc': 'FLASH_STATUS_BUSY'
            },
            {
                'code': 0x2,
                'desc': 'spi timeout'
            },
            {
                'code': 0x3,
                'desc': 'flash operate timeout'
            },
            {
                'code': 0x4,
                'desc': 'package payload lenth error'
            },
            {
                'code': 0x5,
                'desc': 'package lenth error'
            },
            {
                'code': 0x6,
                'desc': 'flash operate PARAM_ERROR'
            },
            {
                'code': 0x7,
                'desc': 'unkown cmd'
            },{
                'code': 0xA,
                'desc': 'permission denied'
            }
        ]

    def command_generate(self, cmd, payload=[]):
        command = bytearray()
        command.extend(self.base_tx_header)
        command.extend([(1 + len(payload))&0xff, ((1 + len(payload))>>8)&0xff])
        command.append(cmd)
        command.extend(payload)
        return command
    
    def rx_expect_length(self, payload_lenth):
        #                                len  operate  status
        return len(self.base_rx_header) + 2 +   1  +     1     + payload_lenth
    
    def get_response_payload(self, response_content):
        return response_content[11:]
    
    def get_response_cmd(self, response_content):
        # operate
        return response_content[9:10]
    
    def check_response_base_header(self, response_content):
        return response_content.startswith(bytes(self.base_rx_header))
    
    def check_response_status(self, response_content):
        status_code = response_content[10]
        if status_code == 0x0:
            return True
        else:
            for tmp_status in self.STATUS_INFO:
                if status_code == tmp_status['code']:
                    BKLog.w('Check status error: err_code: {0}  err_msg: {1}'.format(status_code, tmp_status['desc']))
                    return False
            BKLog.w('Check status error: err_code: {0}  err_msg: unknown'.format(status_code))
            return False
    
    def check_response_length_seg(self, response_content):
        return response_content[7:9] == bytes([(len(response_content) - 9) & 0xff, ((len(response_content) - 9)>>8) & 0xff])

    def response_check(self, response_content):
        return self.check_response_base_header(response_content) and self.check_response_length_seg(response_content) and self.check_response_status(response_content)
    
    @abstractmethod
    def cmd(self):
        pass

    @abstractmethod
    def expect_length(self):
        pass
