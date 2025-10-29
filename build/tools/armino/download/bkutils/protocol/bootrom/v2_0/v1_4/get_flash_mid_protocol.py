import struct
from .base_boot_rom_flash_protocol import BaseBootRomFlashProtocol


class GetFlashMidProtocol(BaseBootRomFlashProtocol):

    def cmd(self, reg_addr):
        return self.command_generate(0x0e, [reg_addr&0xff, 0, 0, 0])

    @property
    def expect_length(self):
        return self.rx_expect_length(4)

    def check_response_length_seg(self, response_content):
        # fix bootrom bug
        return (response_content[7:9] == bytes([(len(response_content) - 9) & 0xff, ((len(response_content) - 9)>>8) & 0xff])) or \
            (response_content[7:9] == bytes([(len(response_content) - 9) & 0xff + 1, ((len(response_content) - 9)>>8) & 0xff]))

    def response_check(self, response_content):
        return super().response_check(response_content) and self.get_response_cmd(response_content) == bytes([0x0e])
    
    def get_mid(self, response_content):
        return struct.unpack("<I", response_content[11:])[0]>>8
