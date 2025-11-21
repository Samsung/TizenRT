import struct
from .base_boot_rom_protocol import BaseBootRomProtocol


class ReadRegProtocol(BaseBootRomProtocol):

    def cmd(self, reg_addr):
        return self.command_generate(0x03, [reg_addr&0xff, (reg_addr>>8)&0xff, (reg_addr>>16)&0xff, (reg_addr>>24)&0xff])

    @property
    def expect_length(self):
        return self.rx_expect_length(8)

    def response_check(self,response_content,reg_addr):
        if super().response_check(response_content) and self.get_response_cmd(response_content) == bytes([0x03]):
            if response_content[7:11] == bytes([reg_addr&0xff, (reg_addr>>8)&0xff, (reg_addr>>16)&0xff, (reg_addr>>24)&0xff]) or \
                response_content[7:11] == bytes([(reg_addr&0xff) + 1, (reg_addr>>8)&0xff, (reg_addr>>16)&0xff, (reg_addr>>24)&0xff]): # work around bootrom bug
                return True
        return False
    
    def get_reg_val(self, response_content):
        return struct.unpack('<I', self.get_response_payload(response_content)[4:8])[0]