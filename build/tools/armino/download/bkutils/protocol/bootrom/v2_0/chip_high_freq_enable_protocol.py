from .base_boot_rom_protocol import BaseBootRomProtocol


class ChipHighFreqEnableProtocol(BaseBootRomProtocol):

    def cmd(self, mode:int, cpu_freq:int, flash_freq:int):        
        payload = [mode&0xff,cpu_freq&0xff,flash_freq&0xff]
        return self.command_generate(0x16, payload)

    @property
    def expect_length(self):
        return self.rx_expect_length(1)

    def response_check(self, response_content):
        return super().response_check(response_content) and self.get_response_cmd(response_content)==bytes([0x16]) and self.get_response_payload(response_content)==bytes([0x00])
