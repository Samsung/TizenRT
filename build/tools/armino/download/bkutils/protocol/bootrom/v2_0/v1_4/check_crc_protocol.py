from .base_boot_rom_protocol import BaseBootRomProtocol


class CheckCrcProtocol(BaseBootRomProtocol):

    def cmd(self, start_addr, end_addr):
        return self.command_generate(0x10, [start_addr&0xff, (start_addr>>8)&0xff, (start_addr>>16)&0xff, (start_addr>>24)&0xff, end_addr&0xff, (end_addr>>8)&0xff, (end_addr>>16)&0xff, (end_addr>>24)&0xff])

    @property
    def expect_length(self):
        return self.rx_expect_length(4)

    def response_check(self, response_content):
        return super().response_check(response_content)

    def get_crc_value(self, response_content):
        return response_content[7] + (response_content[8] << 8) + (response_content[9] << 16) + (response_content[10] << 24)