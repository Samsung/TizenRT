from .base_boot_rom_protocol import BaseBootRomProtocol


class ChipRequestSecureDebugProtocol(BaseBootRomProtocol):

    def cmd(self, debug_mode, sign_data:list):
        payload = [debug_mode]
        payload.extend(sign_data)
        return self.command_generate(0x51, payload)

    @property
    def expect_length(self):
        return self.rx_expect_length(1)

    def response_check(self):
        return True
