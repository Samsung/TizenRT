from .base_boot_rom_flash_protocol import BaseBootRomFlashProtocol


class TsingtengFlashNVMReadProtocol(BaseBootRomFlashProtocol):

    def cmd(self, addr, size):
        payload = [0x48, addr&0xff, (addr>>8)&0xff, (addr>>16)&0xff,0x00] #+dummy
        for _ in range(size):
            payload.append(0x00)       
        return self.command_generate(0x0e, payload)  

    #@property
    def expect_length(self,size):
        return self.rx_expect_length(5+size)

    def response_check(self,response_content):
        return super().response_check(response_content) #and response_content[12:15] == bytes([flash_addr&0xff,(flash_addr>>8)&0xff,(flash_addr>>16)&0xff])
    
    def get_read_val(self,response_content,size):
        return response_content[16:16+size]
