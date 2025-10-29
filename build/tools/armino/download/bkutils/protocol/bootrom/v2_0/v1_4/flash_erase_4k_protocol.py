from .base_boot_rom_flash_protocol import BaseBootRomFlashProtocol


class FlashErase4kProtocol(BaseBootRomFlashProtocol):

    def cmd(self, addr):
        return self.command_generate(0x0b, [addr&0xff, (addr>>8)&0xff, (addr>>16)&0xff, (addr>>24)&0xff])

    @property
    def expect_length(self):
        return self.rx_expect_length(4)

    def response_check(self,response_content,flash_addr):
        return super().response_check(response_content=response_content) and response_content[11:15] == bytes([flash_addr&0xff, (flash_addr>>8)&0xff, (flash_addr>>16)&0xff, (flash_addr>>24)&0xff])
