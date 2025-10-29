import struct
from .base_boot_rom_protocol import BaseBootRomProtocol



class ChipStartBL2UpgradeProtocol(BaseBootRomProtocol):

    def cmd(self):        
        return self.command_generate(0x17)

    @property
    def expect_length(self):
        return self.rx_expect_length(2)

    def response_check(self, response_content):
        return super().response_check(response_content) and self.get_response_cmd(response_content)==bytes([0x17]) and self.get_response_payload(response_content)[:1]==bytes([0x00])
    
    def get_boot_flag(self, response_content):      
        return int(self.get_response_payload(response_content)[1])
