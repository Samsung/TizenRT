from .base_boot_rom_flash_protocol import BaseBootRomFlashProtocol


class TsingtengFlashFactoryCmdProtocol(BaseBootRomFlashProtocol):

    def cmd(self, user_cmd):       
        return self.command_generate(0x0e,[user_cmd])

    @property
    def expect_length(self):
        return self.rx_expect_length(1)

    def response_check(self, response_content):
        return super().response_check(response_content)
