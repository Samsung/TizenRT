from .base_boot_rom_protocol import BaseBootRomProtocol


class Bl2LinkCheckProtocol(BaseBootRomProtocol):

    def cmd(self):
        return self.command_generate(0x02)

    @property
    def expect_length(self):
        return self.rx_expect_length(1)

    def response_check(self, response_content):
        return super().response_check(response_content) and self.get_response_cmd(response_content) == bytes([0x03]) and self.get_response_payload(response_content) == bytes([0x00])

    def response_check_mixed(self, response_content):

        bl1_flag = 0x01
        bl2_flag = 0x03

        if self.check(response_content,bl1_flag):
            return True,False
        if self.check(response_content,bl2_flag):
            return True,True
        return False,False

    def check(self,response_content,flag):
        return super().response_check(response_content) and self.get_response_cmd(response_content) == bytes([flag]) and self.get_response_payload(response_content) == bytes([0x00])