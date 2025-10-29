from .base_boot_rom_flash_protocol import BaseBootRomFlashProtocol


class FlashEraseAllExtProtocol(BaseBootRomFlashProtocol):

    def cmd(self, timeout_sec=7):
        return self.command_generate(0xea, [timeout_sec])

    @property
    def expect_length(self):
        return self.rx_expect_length(1)

    def response_check(self):
        return True
