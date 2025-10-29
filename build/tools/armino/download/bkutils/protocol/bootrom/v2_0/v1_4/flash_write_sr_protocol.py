from .base_boot_rom_flash_protocol import BaseBootRomFlashProtocol


class FlashWriteSRProtocol(BaseBootRomFlashProtocol):

    def cmd(self, reg_addr, val):
        payload = [reg_addr]
        payload.extend(val)
        return self.command_generate(0x0d, payload)

    def expect_length(self, sr_size):
        return self.rx_expect_length(1 + sr_size)

    def response_check(self, response_content, reg_addr):
        return super().response_check(response_content) and response_content[11:12] == bytes([reg_addr]) 
