from .base_boot_rom_flash_protocol import BaseBootRomFlashProtocol


class FlashReadSRProtocol(BaseBootRomFlashProtocol):

    def cmd(self, reg_addr):
        return self.command_generate(0x0c, [reg_addr&0xff])

    @property
    def expect_length(self):
        return self.rx_expect_length(2)

    def response_check(self, response_content, reg_addr):
        return super().response_check(response_content=response_content) and response_content[11:12] == bytes([reg_addr])

    def get_status_regist_val(self, response_content):
        return response_content[12]
