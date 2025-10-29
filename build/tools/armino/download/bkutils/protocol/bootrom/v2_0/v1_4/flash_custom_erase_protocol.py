from .base_boot_rom_flash_protocol import BaseBootRomFlashProtocol


class FlashCustomEraseProtocol(BaseBootRomFlashProtocol):

    def cmd(self, addr, size):
        return self.command_generate(0x0f, [size, addr&0xff, (addr>>8)&0xff, (addr>>16)&0xff, (addr>>24)&0xff])

    @property
    def expect_length(self):
        return self.rx_expect_length(5)

    def response_check(self,response_content,size_cmd,flash_addr):
        return super().response_check(response_content) and response_content[11:12]==bytes([size_cmd]) and response_content[12:] == bytes([flash_addr&0xff,(flash_addr>>8)&0xff,(flash_addr>>16)&0xff,(flash_addr>>24)&0xff])
