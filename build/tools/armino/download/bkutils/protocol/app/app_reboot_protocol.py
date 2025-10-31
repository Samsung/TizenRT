from .base_app_protocol import BaseAppProtocol


class AppRebootProtocol(BaseAppProtocol):

    def cmd(self, reboot_type='at'):
        if reboot_type == 'at':
            return b'reboot\r\n'
        elif reboot_type == 'hex':
            return self.command_generate(0xfe, [0x95, 0x27, 0x95, 0x27])
        else:
            raise Exception('not support')

    @property
    def expect_length(self):
        raise Exception('not support')

    def response_check(self):
        raise Exception('not support')
