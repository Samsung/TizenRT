from .base_boot_rom_flash_protocol import BaseBootRomFlashProtocol


class FlashCustomWriteExtProtocol(BaseBootRomFlashProtocol):

    def cmd(self, addr, data):
        payload = [addr&0xff, (addr>>8)&0xff, (addr>>16)&0xff, (addr>>24)&0xff]
        payload.extend(data)
        return self.command_generate(0xe6, payload)

    @property
    def expect_length(self):
        return self.rx_expect_length(4+1)

    def response_check(self):
        return True
