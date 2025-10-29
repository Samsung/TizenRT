from .base_boot_rom_protocol import BaseBootRomProtocol


class writeRegProtocol(BaseBootRomProtocol):

    def cmd(self, regAddr, val):
        return self.command_generate(0x01, [regAddr&0xff, (regAddr>>8)&0xff, (regAddr>>16)&0xff, (regAddr>>24)&0xff, val&0xff, (val>>8)&0xff, (val>>16)&0xff, (val>>24)&0xff])

    @property
    def expect_length(self):
        return self.rx_expect_length(8)

    def response_check(self, response_content, reg_addr):
        if super().response_check(response_content) and self.get_response_cmd(response_content)==bytes([0x01]):
            if response_content[7:11] == bytes([reg_addr&0xff, (reg_addr>>8)&0xff, (reg_addr>>16)&0xff, (reg_addr>>24)&0xff]) or \
                response_content[7:11] == bytes([(reg_addr&0xff) + 1, (reg_addr>>8)&0xff, (reg_addr>>16)&0xff, (reg_addr>>24)&0xff]): # work around bootrom bug
                return True
            
        return False
