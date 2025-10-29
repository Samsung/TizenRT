from .base_boot_rom_protocol import BaseBootRomProtocol


class ChipEnterSecureDebugProtocol(BaseBootRomProtocol):

    def cmd(self, pubkey:list):
        payload = [len(pubkey)]
        payload.extend(pubkey)
        return self.command_generate(0x50, payload)

    @property
    def expect_length(self):
        return self.rx_expect_length(32)

    def response_check(self):
        return True
