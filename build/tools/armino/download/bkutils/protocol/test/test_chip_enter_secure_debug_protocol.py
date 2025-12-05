from bkutils.protocol.bootrom.v2_0 import ChipEnterSecureDebugProtocol
import unittest


class TestChipEnterSecureDebugProtocol(unittest.TestCase):

    def setUp(self):
        self.protocol = ChipEnterSecureDebugProtocol()

    def test_cmd(self):
        PUBKEY = [0x12, 0x34, 0x56, 0x78]
        cmd = self.protocol.cmd(PUBKEY)
        cmd_byte = 0x50
        payload = [len(PUBKEY)]
        payload.extend(PUBKEY)
        length = 1 + len(payload)
        expected_cmd = bytearray([0x01, 0xe0, 0xfc, length, cmd_byte])
        expected_cmd.extend(payload)
        self.assertEqual(cmd, expected_cmd, "cmd method did not generate the expected command")

    def test_expect_length(self):
        expected_length = 40    # 7 + 1 + 32
        self.assertEqual(self.protocol.expect_length, expected_length, "expect_length did not return the expected length")

    def test_response_check(self):
        # 合法的响应内容
        CMD = bytes([0x50])
        payload = bytearray([0x00])
        for i in range(32):
            payload.append(0x00+i*8)
        response_content = bytearray([0x04, 0x0e, 3+len(CMD)+len(payload), 0x01, 0xe0, 0xfc])
        response_content.extend(CMD)
        response_content.extend(payload)
        self.assertTrue(self.protocol.response_check(response_content), "response_check failed for a valid response")

if __name__ == '__main__':
    unittest.main()