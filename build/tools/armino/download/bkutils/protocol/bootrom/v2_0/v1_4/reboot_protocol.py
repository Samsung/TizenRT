from .base_boot_rom_protocol import BaseBootRomProtocol


class RebootProtocol(BaseBootRomProtocol):

    def cmd(self):
        return self.command_generate(0x0e, [0xa5])

    @property
    def expect_length(self):
        raise Exception('no support')

    def response_check(self):
        return True
