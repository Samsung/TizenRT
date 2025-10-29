from .base_boot_rom_flash_protocol import BaseBootRomFlashProtocol


class FlashCustomReadExtProtocol(BaseBootRomFlashProtocol):

    def cmd(self, addr, oplen):
        return self.command_generate(0xe8, [addr&0xff, (addr>>8)&0xff, (addr>>16)&0xff, (addr>>24)&0xff, oplen&0xff])

    #@property
    def expect_length(self, oplen):
        return self.rx_expect_length(4+oplen)

    def response_check(self,response_content,flash_addr):
        return super().response_check(response_content) and response_content[9:10]==bytes([0xe8]) and response_content[11:15] == bytes([flash_addr&0xff,(flash_addr>>8)&0xff,(flash_addr>>16)&0xff,(flash_addr>>24)&0xff])
    
    def get_read_content(self,response_content):
        return self.get_response_payload(response_content)[4:] 