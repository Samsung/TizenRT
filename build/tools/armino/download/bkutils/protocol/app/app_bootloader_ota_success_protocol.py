from bkutils.protocol.app.base_app_protocol import BaseAppProtocol


class AppBootloaderOtaSuccessProtocol(BaseAppProtocol):

    def cmd(self):
        return self.command_generate(0xF0)

    @property
    def expect_length(self):
        return self.rx_expect_length(0)

    def response_check(self, response_content):
        return response_content == bytes([0x04, 0x0E, 0x04, 0x01, 0xE0, 0xFC, 0xF0])
