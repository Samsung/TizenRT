import struct
from .base_boot_rom_protocol import BaseBootRomProtocol



class ChipStopBL2UpgradeProtocol(BaseBootRomProtocol):

    def cmd(self):        
        return self.command_generate(0x18)

    @property
    def expect_length(self):
        return self.rx_expect_length(1)

    def response_check(self, response_content):
        return super().response_check(response_content) and self.get_response_cmd(response_content)==bytes([0x18]) and self.get_response_payload(response_content)==bytes([0x00])
    
