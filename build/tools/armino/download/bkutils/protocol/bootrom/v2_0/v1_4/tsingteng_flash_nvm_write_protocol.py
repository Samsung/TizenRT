from .base_boot_rom_flash_protocol import BaseBootRomFlashProtocol


class TsingtengFlashNVMWriteProtocol(BaseBootRomFlashProtocol):

    def cmd(self, addr, data):
        payload = [0x42, addr&0xff, (addr>>8)&0xff, (addr>>16)&0xff]
        payload.extend(data)
        return self.command_generate(0x0e, payload)

    #@property
    def expect_length(self,oplen):
        return self.rx_expect_length(4+oplen)

    def response_check(self,response_content):
        return super().response_check(response_content) #and response_content[12:] == bytes([flash_addr&0xff,(flash_addr>>8)&0xff,(flash_addr>>16)&0xff])
