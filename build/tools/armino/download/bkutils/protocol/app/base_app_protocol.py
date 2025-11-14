from abc import ABC, abstractmethod


class BaseAppProtocol(ABC):

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
    
    @abstractmethod
    def cmd(self):
        pass

    @abstractmethod
    def expect_length(self):
        pass

    @abstractmethod
    def response_check(self):
        pass
