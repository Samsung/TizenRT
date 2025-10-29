from abc import ABC, abstractmethod


class BaseBootRomProtocol(ABC):

    def __init__(self):
        self.base_tx_type_and_opcode = [0x01, 0xe0, 0xfc]
        self.rx_header_and_event = [0x04, 0x0e]

    def command_generate(self, cmd, payload=[]):
        command = bytearray()
        command.extend(self.base_tx_type_and_opcode)
        command.append(1 + len(payload))
        command.append(cmd)
        command.extend(payload)
        return command
    
    def rx_expect_length(self, payload_lenth):
        return len(self.rx_header_and_event) + 1 + len(self.base_tx_type_and_opcode) + 1 + payload_lenth
    
    def get_response_cmd(self, response_content):
        return response_content[6:7]
    
    def get_response_payload(self, response_content):
        return response_content[7:]
    
    def check_response_header_seg(self, response_content):
        return response_content.startswith(bytes(self.rx_header_and_event))
    
    def check_response_length_seg(self, response_content):
        return response_content[2] == len(response_content) - 3
    
    def check_response_tx_header_seg(self, response_content):
        return response_content[3:6] == bytes(self.base_tx_type_and_opcode)
    
    @abstractmethod
    def cmd(self):
        pass

    @abstractmethod
    def expect_length(self):
        pass

    def response_check(self, response_content):
        return self.check_response_header_seg(response_content) and self.check_response_length_seg(response_content) and self.check_response_tx_header_seg(response_content)
