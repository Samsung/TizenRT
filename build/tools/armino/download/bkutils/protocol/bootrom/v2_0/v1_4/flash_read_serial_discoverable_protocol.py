from .base_boot_rom_flash_protocol import BaseBootRomFlashProtocol


class FlashReadSFDPProtocol(BaseBootRomFlashProtocol):

    def cmd(self, addr, oplen):
        payload = [0x5A, addr&0xff, (addr>>8)&0xff, (addr>>16)&0xff,0x00] #dummy
        for _ in range(oplen):
            payload.append(0x00)
        return self.command_generate(0x0e, payload)

    #@property
    def expect_length(self, oplen):
        return self.rx_expect_length(5+oplen)

    def response_check(self,response_content):
        return super().response_check(response_content) #and response_content[12:] == bytes([addr&0xff,(addr>>8)&0xff,(addr>>16)&0xff])

    def get_read_val(self,response_content,oplen):
        return response_content[16:16+oplen]