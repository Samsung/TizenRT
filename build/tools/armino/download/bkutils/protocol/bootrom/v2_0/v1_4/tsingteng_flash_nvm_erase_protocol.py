from .base_boot_rom_flash_protocol import BaseBootRomFlashProtocol


class TsingtengFlashNVMEraseProtocol(BaseBootRomFlashProtocol):

    def cmd(self, addr):
        return self.command_generate(0x0e, [0x44, addr&0xff, (addr>>8)&0xff, (addr>>16)&0xff])

    @property
    def expect_length(self):
        return self.rx_expect_length(4)

    def response_check(self,response_content):
        return super().response_check(response_content) #and response_content[12:] == bytes([addr&0xff,(addr>>8)&0xff,(addr>>16)&0xff])
