from bkutils.protocol.bootrom.v2_0.v1_4 import RebootProtocol
import unittest


class TestRebootProtocol(unittest.TestCase):

    def setUp(self):
        self.protocol = RebootProtocol()

    def test_cmd(self):
        cmd = self.protocol.cmd()
        cmd_byte = 0x0E
        payload = bytes([0xA5])
        length = 1 + len(payload)
        expected_cmd = bytearray([0x01, 0xe0, 0xfc, length, cmd_byte])
        expected_cmd.extend(payload)
        self.assertEqual(cmd, expected_cmd, "cmd method did not generate the expected command")

    def test_expect_length(self):
        # expected_length = 7    # 7 + 
        # self.assertEqual(self.protocol.expect_length, expected_length, "expect_length did not return the expected length")
        pass
    
    def test_response_check(self):
        # 合法的响应内容
        CMD = bytes([])
        payload = bytes([])
        response_content = bytearray([0x04, 0x0e, 3+len(CMD)+len(payload), 0x01, 0xe0, 0xfc])
        response_content.extend(CMD)
        response_content.extend(payload)
        self.assertTrue(self.protocol.response_check(response_content), "response_check failed for a valid response")

if __name__ == '__main__':
    unittest.main()