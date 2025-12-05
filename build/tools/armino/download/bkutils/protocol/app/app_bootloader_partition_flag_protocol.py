from bkutils.protocol.app.base_app_protocol import BaseAppProtocol


class AppBootloaderPartitionFlagProtocol(BaseAppProtocol):

    def cmd(self):
        return self.command_generate(0xF1)
    
    @property
    def expect_length(self):
        return self.rx_expect_length(1)

    def response_check(self, response_content):
        ret = response_content[:7] == bytes([0x04, 0x0E, 0x05, 0x01, 0xE0, 0xFC, 0xF1])
        flag = response_content[7]
        return ret, flag
