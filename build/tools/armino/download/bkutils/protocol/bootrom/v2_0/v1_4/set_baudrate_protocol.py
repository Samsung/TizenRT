from .base_boot_rom_protocol import BaseBootRomProtocol


class SetBaudrateProtocol(BaseBootRomProtocol):

    def cmd(self, baudrate, delay_ms):
        return self.command_generate(0x0f, [baudrate&0xff, (baudrate>>8)&0xff, (baudrate>>16)&0xff, (baudrate>>24)&0xff, delay_ms&0xff])

    @property
    def expect_length(self):
        return self.rx_expect_length(5)

    def response_check(self, response_content, baudrate):
        return super().response_check(response_content) and super().get_response_payload(response_content)[:4] == bytes([baudrate&0xff, (baudrate>>8)&0xff, (baudrate>>16)&0xff, (baudrate>>24)&0xff])
