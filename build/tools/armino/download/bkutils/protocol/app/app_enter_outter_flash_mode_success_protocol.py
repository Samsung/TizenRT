from .base_app_protocol import BaseAppProtocol

class AppEnterOutterFlashModeSuccessProtocol(BaseAppProtocol):

    def cmd(self):       
        return self.command_generate(0xbc)
        

    @property
    def expect_length(self):
        return self.rx_expect_length(1)

    def response_check(self, response_content):
        return response_content[6]==0xbc
