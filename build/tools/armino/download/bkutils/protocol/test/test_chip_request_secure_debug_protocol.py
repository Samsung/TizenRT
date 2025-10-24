from bkutils.protocol.bootrom.v2_0 import ChipRequestSecureDebugProtocol
import unittest


class TestChipRequestSecureDebugProtocol(unittest.TestCase):

    def setUp(self):
        self.protocol = ChipRequestSecureDebugProtocol()

    def test_cmd(self):
        debug_mode = 0
        sign_data = [0x12, 0x34, 0x56, 0x78]
        cmd = self.protocol.cmd(debug_mode, sign_data)
        cmd_byte = 0x51
        payload = [debug_mode ,len(sign_data)]
        payload.extend(sign_data)
        length = 1 + len(payload)
        expected_cmd = bytearray([0x01, 0xe0, 0xfc, length, cmd_byte])
        expected_cmd.extend(payload)
        self.assertEqual(cmd, expected_cmd, "cmd method did not generate the expected command")

    def test_expect_length(self):
        expected_length = 8    # 7 + 1
        self.assertEqual(self.protocol.expect_length, expected_length, "expect_length did not return the expected length")

    def test_response_check(self):
        # 合法的响应内容
        CMD = bytes([0x51])
        payload = bytearray([0x00])
        for i in range(32):
            payload.append(0x00+i*8)
        response_content = bytearray([0x04, 0x0e, 3+len(CMD)+len(payload), 0x01, 0xe0, 0xfc])
        response_content.extend(CMD)
        response_content.extend(payload)
        self.assertTrue(self.protocol.response_check(response_content), "response_check failed for a valid response")

if __name__ == '__main__':
    unittest.main()