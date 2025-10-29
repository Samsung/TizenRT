from .base_boot_rom_flash_protocol import BaseBootRomFlashProtocol


class FlashEraseAllProtocol(BaseBootRomFlashProtocol):

    def cmd(self, timeout_sec=20):
        return self.command_generate(0x0a, [timeout_sec])

    @property
    def expect_length(self):
        return self.rx_expect_length(1)

    def response_check(self,response_content):
        return super().response_check(response_content)
