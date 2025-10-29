from .base_boot_rom_protocol import BaseBootRomProtocol


class LinkCheckExtProtocol(BaseBootRomProtocol):

    def cmd(self,link_key,timeout=0x60):
        payload = []
        payload.extend(link_key)
        payload.append(timeout)
        return self.command_generate(0x00,payload)

    @property
    def expect_length(self):
        return self.rx_expect_length(1)

    def response_check(self, response_content):
        return super().response_check(response_content) and self.get_response_cmd(response_content) == bytes([0x01]) and self.get_response_payload(response_content) == bytes([0x00])
    
