from .base_app_protocol import BaseAppProtocol

class AppEnterOutterFlashModeProtocol(BaseAppProtocol):

    def cmd(self):       
        return self.command_generate(0xbb, [0x95, 0x28, 0x95, 0x28])
        

    @property
    def expect_length(self):
        return self.rx_expect_length(4)

    def response_check(self, response_content):
        return response_content[6]==0xbb and response_content[7:] == bytes([0x95, 0x28, 0x95, 0x28])
