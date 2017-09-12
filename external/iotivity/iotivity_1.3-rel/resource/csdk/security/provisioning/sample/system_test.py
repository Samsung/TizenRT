import pexpect
import re
import time
import os
import signal
import shutil


class Service:
    def __init__(self, number):

        print("Servicing test suite #" + str(number))

    def save_config(self):

        if not os.path.exists("bak"):
            os.mkdir("bak")

            dir_src = "."
            dir_dst = "bak"

            for filename in os.listdir(dir_src):
                if filename.endswith(".dat"):
                    shutil.copy(os.path.join(dir_src, filename), os.path.join(dir_dst, filename))
                    print("Saved " + filename)

    def _safe_remove(self, filename):
        if os.path.isfile(filename):
            print ("Removing: " + filename)
            os.remove(filename)

    def restore_config(self):

        dir_src = "bak"
        dir_dst = "."

        for filename in os.listdir(dir_src):
            if filename.endswith(".dat"):
                shutil.copy(os.path.join(dir_src, filename), os.path.join(dir_dst, filename))
                print("Restored: " + filename)

        self._safe_remove("oic_prvn_mng.db")
        self._safe_remove("device_properties.dat")
        self._safe_remove("rootca.crt")
        self._safe_remove("PDM.dat")


class Test:
    _SAMPLE_JUSTWORKS = "./sampleserver_justworks"
    _JUSTWORKS_CHECKER = "SAMPLE_JUSTWORKS: Entering ocserver main loop"
    _JUSTWORKS_START_OK = "Sample Server Justworks started successfully"
    _JUSTWORKS_START_FAIL = "Sample Server Justworks start failed"

    _SAMPLE_RANDOMPIN = "./sampleserver_randompin"
    _RANDOMPIN_CHECKER = "SAMPLE_RANDOMPIN: Entering ocserver main loop"
    _RANDOMPIN_START_OK = "Sample Server Randompin started successfully"
    _RANDOMPIN_START_FAIL = "Sample Server Randompin start failed"

    _SAMPLE_MFG = "./sampleserver_mfg"
    _MFG_CHECKER = "SAMPLE_MANUFACTURER_CERT: Entering ocserver main loop"
    _MFG_START_OK = "Sample Server MFG started successfully"
    _MFG_START_FAIL = "Sample Server MFG start failed"

    _PROV_CLIENT = "./provisioningclient"
    _CLIENT_CHECKER = "Enter Menu Number:"
    _CLIENT_START_OK = "Provisioning client started successfully"
    _CLIENT_START_FAIL = "Provisioning client start failed"

    def __init__(self, number, prepare):

        print("Test case #" + str(number) + " start")
        prepare.restore_config()

    def _run_process(self, cmd, checker, delay):
        try:
            proc = pexpect.spawn(cmd)
            keyword = re.compile(checker)
            if proc.expect(keyword, timeout=delay) == 0:
                print("Proccess " + cmd + " succeeded!")
                return proc
            else:
                print("Proccess " + cmd + " failed!")
                return 0
        except:
            print("Proccess " + cmd + " failed!")

    def _kill_process(self, run_proc):

        pid = run_proc.pid
        try:
            run_proc.close
        except:
            os.kill(pid, signal.SIGKILL)

    def stop_instance(self, run_proc):
        print("Killing process " + str(run_proc.pid))
        self._kill_process(run_proc)

    def start_justworks(self):

        run = self._run_process(self._SAMPLE_JUSTWORKS, self._JUSTWORKS_CHECKER, 7)

        if run:
            print(self._JUSTWORKS_START_OK)
            return run
        else:
            print(self._JUSTWORKS_START_FAIL)
            self.flush_output(run)
            return 0

    def start_randompin(self):

        run = self._run_process(self._SAMPLE_RANDOMPIN, self._RANDOMPIN_CHECKER, 7)

        if run:
            print(self._RANDOMPIN_START_OK)
            return run
        else:
            print(self._RANDOMPIN_START_FAIL)
            self.flush_output(run)
            return 0

    def start_mfg(self):

        run = self._run_process(self._SAMPLE_MFG, self._MFG_CHECKER, 7)

        if run:
            print(self._MFG_START_OK)
            return run
        else:
            print(self._MFG_START_FAIL)
            self.flush_output(run)
            return 0

    def start_client(self):

        run = self._run_process(self._PROV_CLIENT, self._CLIENT_CHECKER, 7)

        if run:
            print(self._CLIENT_START_OK)
            return run
        else:
            print(self._CLIENT_START_FAIL)
            self.flush_output(run)
            return 0

    def discover_and_own(self, run_proc, secured, additional_servers):

        if self.send_command(run_proc, "11\r"):
            if self.wait_for_keyword(run_proc, "Discovered Unowned Devices", 40):
                if not self.wait_for_keyword(run_proc, "OIC Provisioning", 40):
                    return 0
                device_id = run_proc.before
                print ("Discovered Unowned Device " + device_id)
                device_1 = device_id.partition("[1]")[-1].split("\r")[0]
                if additional_servers > 0:
                    device_2 = device_id.partition("[2]")[-1].split("\r")[0]
                    if additional_servers == 2 or additional_servers == 4:
                        device_3 = device_id.partition("[2]")[-1].split("\r")[0]
                if self.send_command(run_proc, "20\r"):
                    if secured:
                        pin = self.get_pin(secured)
                        if pin:
                            time.sleep(1)
                            self.send_command(run_proc, pin)
                    if self.wait_for_keyword(run_proc, "Ownership Transfer SUCCEEDED", 40):
                        if self.send_command(run_proc, "12\r"):
                            if self.wait_for_keyword(run_proc, "Discovered Owned Devices", 40):
                                if not self.wait_for_keyword(run_proc, "OIC Provisioning", 40):
                                    return 0
                                discovered_devices = run_proc.before
                                if device_1 in discovered_devices:
                                    print("Device [1] " + device_1 + " is now owned")
                                else:
                                    print("Device [1] " + device_1 + " not found in owned devices list!")
                                    return 0
                                if additional_servers > 0:
                                    if device_2 in discovered_devices:
                                        print("Device [2] " + device_2 + " is now owned")
                                    else:
                                        print("Device [2] " + device_2 + " not found in owned devices list!")
                                        return 0
                                    if additional_servers == 2:
                                        if device_3 in discovered_devices:
                                            print("Device [3] " + device_3 + " is now owned")
                                        else:
                                            print("Device [3] " + device_3 + " not found in owned devices list!")
                                            return 0

                                if additional_servers == 3:
                                    return device_1
                                if additional_servers == 4:
                                    return device_2
                                return 1
        return 0

    def provide_acl(self, run_proc, cmd, mode):

        self.wait_for_keyword(run_proc, "Enter Menu Number:", 1)
        if self.send_command(run_proc, cmd + "\r"):
            self.wait_for_keyword(run_proc, ":", 1)
            self.send_command(run_proc, "1\r")
            self.wait_for_keyword(run_proc, ":", 1)
            self.send_command(run_proc, "2\r")

            if mode == "double":
                self.wait_for_keyword(run_proc, ":", 1)
                self.send_command(run_proc, "2\r")
            self.wait_for_keyword(run_proc, ":", 1)

            for x in xrange(3):
                self.send_command(run_proc, "1\r")
                self.wait_for_keyword(run_proc, ":", 1)
                self.send_command(run_proc, "light\r")
                self.wait_for_keyword(run_proc, ":", 1)

            for x in xrange(5):
                self.send_command(run_proc, "y")
                self.wait_for_keyword(run_proc, ":", 1)
                self.send_command(run_proc, "y \r")
                if x == 4:
                    break
                self.wait_for_keyword(run_proc, ":", 1)

            if mode == "double":
                self.wait_for_keyword(run_proc, ":", 5)
                self.send_command(run_proc, "1\r")
                self.wait_for_keyword(run_proc, ":", 1)
                for x in xrange(3):
                    self.send_command(run_proc, "1\r")
                    self.wait_for_keyword(run_proc, ":", 1)
                    self.send_command(run_proc, "light\r")
                    self.wait_for_keyword(run_proc, ":", 1)

                for x in xrange(5):
                    self.send_command(run_proc, "y")
                    self.wait_for_keyword(run_proc, ":", 1)
                    self.send_command(run_proc, "y \r")
                    if x == 4:
                        break
                    self.wait_for_keyword(run_proc, ":", 1)

            return 1
        else:
            return 0

    def get_pin(self, randompin_server):

        if self.wait_for_keyword(randompin_server, "PIN CODE :", 40):
            self.wait_for_keyword(randompin_server, "============================", 10)
            pin_dump = randompin_server.before
            return pin_dump[0:9]
        else:
            print ("Can not get PIN!")
            return 0

        pass

    def flush_output(self, run_proc):

        if run_proc:
            print(run_proc.before)
            print(run_proc.after)
            return 1
        else:
            print("Process" + str(run_proc.pid) + " not found")
            return 0

    def dump_output(self, run_proc):

        if run_proc:
            out = []
            out.append(run_proc.before)
            out.append(run_proc.after)
            return out
        else:
            print("Process" + str(run_proc.pid) + " not found")
            return 0

    def send_command(self, run_proc, cmd):

        if run_proc:
            run_proc.sendline(cmd)
            if cmd != "y \r":
                print("Sent " + re.sub("\r", "[ENTER]", cmd) + " to process " + str(run_proc.pid))
            return 1
        else:
            print("Process" + str(run_proc.pid) + " not found")
            return 0

    def wait_for_keyword(self, run_proc, keyword, delay):

        if run_proc:
            key = re.compile(keyword)
            if run_proc.expect(key, timeout=delay) == 0:
                if keyword != ":":
                    print("Keyword |" + keyword + "| found in output of process " + str(run_proc.pid))
                return 1
            else:
                print("Keyword |" + keyword + "| absent in process" + str(run_proc.pid))
                return 0
        else:
            print("Process" + str(run_proc.pid) + " not found")
            return 0


FAIL_COUNT = 0


def case01(suite):
    global FAIL_COUNT
    case = Test(01, suite)
    try:
        client = case.start_client()
        if client:
            print ("! |Test Case 01 passed| !")
        else:
            print("! |Test Case 01 FAILED| !")
            FAIL_COUNT += 1
    except:
        FAIL_COUNT += 1
        print "! |Test Case 01 FAILED| !"
    finally:
        case.stop_instance(client)


def case02(suite):
    global FAIL_COUNT
    case = Test(02, suite)
    try:
        server_justworks = case.start_justworks()
        if server_justworks:
            print ("! |Test Case 02 passed| !")
        else:
            print("! |Test Case 02 FAILED| !")
            FAIL_COUNT += 1

    except:
        FAIL_COUNT += 1
        print "! |Test Case 02 FAILED| !"
    finally:
        case.stop_instance(server_justworks)


def case03(suite):
    global FAIL_COUNT
    case = Test(03, suite)
    try:
        server_randompin = case.start_randompin()
        if server_randompin:
            print ("! |Test Case 03 passed| !")
        else:
            print("! |Test Case 03 FAILED| !")
            FAIL_COUNT += 1
    except:
        FAIL_COUNT += 1
        print "! |Test Case 03 FAILED| !"
    finally:
        case.stop_instance(server_randompin)


def case04(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(04, suite)
    try:
        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        if case.send_command(client, "11\r"):
            if not case.wait_for_keyword(client, "Discovered Unowned Devices", 40):
                FAIL_TRIGER = 1
                # if not case.wait_for_keyword(client, "[1]", 40):
                # FAIL_TRIGER = 1
            case.wait_for_keyword(client, "OIC Provisioning", 40)
            device_id = client.before[2:38]
            print ("Discovered Unowned Device " + device_id)
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 04 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 04 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 04 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_justworks)


def case05(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(05, suite)
    try:
        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        if case.send_command(client, "11\r"):
            if not case.wait_for_keyword(client, "Discovered Unowned Devices", 40):
                FAIL_TRIGER = 1
                # if not case.wait_for_keyword(client, "[1]", 10):
                # FAIL_TRIGER = 1
            case.wait_for_keyword(client, "OIC Provisioning", 40)
            device_id = client.before[2:38]
            print ("Discovered Unowned Device " + device_id)
        else:
            FAIL_TRIGER = 1

        if case.send_command(client, "20\r"):
            if not case.wait_for_keyword(client, "Ownership Transfer SUCCEEDED", 40):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 05 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 05 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 05 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_justworks)


def case06(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(06, suite)
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        if case.send_command(client, "11\r"):
            if not case.wait_for_keyword(client, "Discovered Unowned Devices", 40):
                FAIL_TRIGER = 1
                # if not case.wait_for_keyword(client, "[1]", 10):
                # FAIL_TRIGER = 1
            case.wait_for_keyword(client, "OIC Provisioning", 40)
            device_id = client.before[2:38]
            print ("Discovered Unowned Device " + device_id)
        else:
            FAIL_TRIGER = 1

        if case.send_command(client, "20\r"):
            pin = case.get_pin(server_randompin)
            case.send_command(client, pin)
            if not case.wait_for_keyword(client, "Ownership Transfer SUCCEEDED", 40):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 06 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 06 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 06 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)


def case07(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(07, suite)
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        if case.send_command(client, "11\r"):
            if not case.wait_for_keyword(client, "Discovered Unowned Devices", 40):
                FAIL_TRIGER = 1
                # if not case.wait_for_keyword(client,"[1]", 40):
                # FAIL_TRIGER = 1
            case.wait_for_keyword(client, "OIC Provisioning", 40)
            device_id = client.before[2:38]
            print ("Discovered Unowned Device " + device_id)
        else:
            FAIL_TRIGER = 1

        if case.send_command(client, "20\r"):
            case.wait_for_keyword(client, "INPUT PIN:", 40)
            for pin_attempts in range(1, 5):
                case.send_command(client, str(pin_attempts))
                case.wait_for_keyword(client, "INPUT PIN:", 40)
            case.send_command(client, str("should fail after fifth"))
            if not case.wait_for_keyword(client, "Ownership Transfer FAILED", 40):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 07 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 07 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 07 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)


def case08(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(8, suite)
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        if not case.discover_and_own(client, server_randompin, 1):
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 08 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 08 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 08 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case09(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(9, suite)
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        if not case.discover_and_own(client, server_randompin, 1):
            FAIL_TRIGER = 1

        if not case.provide_acl(client, "32", "single"):
            FAIL_TRIGER = 1

        if not case.wait_for_keyword(client, "Provision ACL SUCCEEDED", 40):
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 09 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 09 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 09 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case10(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(10, suite)
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        server_mfg = case.start_mfg()
        if not server_mfg:
            FAIL_TRIGER = 1

        if not case.discover_and_own(client, server_randompin, 2):
            FAIL_TRIGER = 1

        case.wait_for_keyword(client, "Enter Menu Number:", 40)
        if case.send_command(client, "32\r"):
            for x in xrange(2):
                case.wait_for_keyword(client, ":", 10)
                case.send_command(client, "1\r")
            case.wait_for_keyword(client, "Entered Wrong Number. Please Enter Again", 40)

        if FAIL_TRIGER:
            print("! |Test Case 10 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 10 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 10 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case11(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(11, suite)
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        if not case.discover_and_own(client, server_randompin, 1):
            FAIL_TRIGER = 1

        if not case.provide_acl(client, "30", "double"):
            FAIL_TRIGER = 1

        if not case.wait_for_keyword(client, "Provision Pairwise SUCCEEDED", 40):
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 11 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 11 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 11 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case12(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(12, suite)
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        if not case.discover_and_own(client, server_randompin, 1):
            FAIL_TRIGER = 1

        case.wait_for_keyword(client, "Enter Menu Number:", 3)
        if case.send_command(client, "31\r"):
            case.wait_for_keyword(client, ":", 1)
            case.send_command(client, "1\r")
            case.wait_for_keyword(client, ":", 1)
            case.send_command(client, "2\r")
            case.wait_for_keyword(client, "Select PSK length..", 10)
            case.send_command(client, "1")
            case.send_command(client, "1 \r")
            case.send_command(client, "1")

            case.wait_for_keyword(client, "Provision Credential SUCCEEDED", 40)

        if FAIL_TRIGER:
            print("! |Test Case 12 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 12 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 12 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case13(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(13, suite)
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        if not case.discover_and_own(client, server_randompin, 1):
            FAIL_TRIGER = 1

        case.wait_for_keyword(client, "Enter Menu Number:", 30)
        if case.send_command(client, "31\r"):
            case.wait_for_keyword(client, ":", 10)
            case.send_command(client, "1\r")
            case.wait_for_keyword(client, ":", 10)
            case.send_command(client, "2\r")
            case.wait_for_keyword(client, "Select PSK length..", 10)
            case.send_command(client, "2")
            case.send_command(client, "2 \r")
            case.send_command(client, "2")

            case.wait_for_keyword(client, "Provision Credential SUCCEEDED", 40)

        if FAIL_TRIGER:
            print("! |Test Case 13 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 13 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 13 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case14(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(14, suite)
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        if not case.discover_and_own(client, server_randompin, 2):
            FAIL_TRIGER = 1

        case.wait_for_keyword(client, "Enter Menu Number:", 30)
        if case.send_command(client, "31\r"):
            case.wait_for_keyword(client, ":", 10)
            case.send_command(client, "1\r")
            case.wait_for_keyword(client, ":", 10)
            case.send_command(client, "2\r")
            case.wait_for_keyword(client, "Select PSK length..", 10)
            case.send_command(client, "0")
            case.send_command(client, "0 \r")
            case.send_command(client, "0")

            case.wait_for_keyword(client, "Provision Credential SUCCEEDED", 40)

        if FAIL_TRIGER:
            print("! |Test Case 14 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 14 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 14 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case15(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(15, suite)
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        if not case.discover_and_own(client, server_randompin, 1):
            FAIL_TRIGER = 1

        case.wait_for_keyword(client, "Enter Menu Number:", 30)
        if case.send_command(client, "33\r"):
            case.wait_for_keyword(client, ":", 10)
            case.send_command(client, "1\r")

            case.wait_for_keyword(client, "Target Server NOT Support Direct-Pairing", 40)

        if FAIL_TRIGER:
            print("! |Test Case 15 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 15 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 15 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case16(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(16, suite)
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        device = case.discover_and_own(client, server_randompin, 4)

        if not case.provide_acl(client, "30", "double"):
            FAIL_TRIGER = 1

        if not case.wait_for_keyword(client, "Provision Pairwise SUCCEEDED", 40):
            FAIL_TRIGER = 1

        if device:
            if case.send_command(client, "34\r"):
                case.wait_for_keyword(client, "Status on PRVN DB", 10)
                case.send_command(client, "1\r")
                if case.wait_for_keyword(client, str(device)[1:], 30):
                    print("Checked device status OK " + str(device))
                else:
                    print("Device status check FAIL! " + str(device) + " not found!")
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 16 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 16 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 16 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case17(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(17, suite)
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        if not case.discover_and_own(client, server_randompin, 1):
            FAIL_TRIGER = 1

        if not case.provide_acl(client, "30", "double"):
            FAIL_TRIGER = 1

        if not case.wait_for_keyword(client, "Provision Pairwise SUCCEEDED", 40):
            FAIL_TRIGER = 1

        if case.send_command(client, "40\r"):
            case.wait_for_keyword(client, ":", 10)
            case.send_command(client, "1\r")
            case.wait_for_keyword(client, ":", 10)
            case.send_command(client, "2\r")
            if case.wait_for_keyword(client, "Unlinked Selected Pairwise Devices", 3):
                print("Devices unlinked")
            else:
                print("Device unlink FAIL!")

        if FAIL_TRIGER:
            print("! |Test Case 17 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 17 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 17 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case18(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(18, suite)
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        if not case.discover_and_own(client, server_randompin, 1):
            FAIL_TRIGER = 1

        if not case.provide_acl(client, "30", "double"):
            FAIL_TRIGER = 1

        if not case.wait_for_keyword(client, "Provision Pairwise SUCCEEDED", 40):
            FAIL_TRIGER = 1

        if case.send_command(client, "50\r"):
            case.wait_for_keyword(client, ":", 10)
            case.send_command(client, "1\r")
            if case.wait_for_keyword(client, "Remove Device SUCCEEDED", 40):
                print("Device removed")
            else:
                print("Device remove FAIL!")

        if FAIL_TRIGER:
            print("! |Test Case 18 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 18 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 18 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case19(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(19, suite)
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        if not case.discover_and_own(client, server_randompin, 1):
            FAIL_TRIGER = 1

        if not case.provide_acl(client, "32", "single"):
            FAIL_TRIGER = 1

        if not case.wait_for_keyword(client, "Provision ACL SUCCEEDED", 40):
            FAIL_TRIGER = 1

        if case.send_command(client, "60\r"):
            case.wait_for_keyword(client, ":", 10)
            case.send_command(client, "1\r")
            if case.wait_for_keyword(client, "Get Cred SUCCEEDED", 40):
                print("Credential provision ok")
            else:
                print("Credential provision FAIL!")

        if FAIL_TRIGER:
            print("! |Test Case 19 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 19 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 19 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case20(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(20, suite)
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        if not case.discover_and_own(client, server_randompin, 1):
            FAIL_TRIGER = 1

        if not case.provide_acl(client, "32", "single"):
            FAIL_TRIGER = 1

        if not case.wait_for_keyword(client, "Provision ACL SUCCEEDED", 40):
            FAIL_TRIGER = 1

        if case.send_command(client, "61\r"):
            case.wait_for_keyword(client, ":", 10)
            case.send_command(client, "1\r")
            if case.wait_for_keyword(client, "OIC Provisioning", 40):
                case.flush_output(client)
                print("Credential provision ok")
            else:
                print("Credential provision FAIL!")

        if FAIL_TRIGER:
            print("! |Test Case 20 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 20 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 20 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case21(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(21, suite)
    try:
        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        if case.send_command(client, "11\r"):
            if case.wait_for_keyword(client, "Discovered Unowned Devices", 40):
                if not case.wait_for_keyword(client, "OIC Provisioning", 40):
                    return 0
                device_id = client.before
                print ("Discovered Unowned Device " + device_id)
                device_1 = device_id.partition("[1]")[-1].split("\r")[0]

                if case.send_command(client, "20\r"):
                    if case.wait_for_keyword(client, "Ownership Transfer SUCCEEDED", 40):

                        server_randompin = case.start_randompin()
                        if not server_randompin:
                            FAIL_TRIGER = 1

                        case.wait_for_keyword(client, "Enter Menu Number:", 30)
                        case.send_command(client, "10\r")
                        if not case.wait_for_keyword(client, "OIC Provisioning", 40):
                            FAIL_TRIGER = 1

                        # case.flush_output(client)

                        discovered_devices = client.before
                        if device_1 in discovered_devices:
                            print("Device [1] " + device_1 + " is now owned")
                        else:
                            print("Device [1] " + device_1 + " not found in owned devices list!")
                            FAIL_TRIGER = 1

                        if "Discovered Unowned Devices\r\n     [1]" in discovered_devices:
                            print("Unowned randompin device found")
                        else:
                            print("Unowned randompin device not found!")
                            FAIL_TRIGER = 1

                    else:
                        FAIL_TRIGER = 1
                else:
                    FAIL_TRIGER = 1
            else:
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 21 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 21 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 21 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case22(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(22, suite)
    try:
        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        if case.send_command(client, "32\r"):
            if not case.wait_for_keyword(client, "Owned Device List, to Provision ACL, is Empty", 30):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 22 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 22 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 22 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case23(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(23, suite)
    try:
        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        if case.send_command(client, "31\r"):
            if not case.wait_for_keyword(client, "Owned Device List, to Provision Credentials, is Empty", 30):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 23 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 23 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 23 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case24(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(24, suite)
    try:
        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        if case.send_command(client, "30\r"):
            if not case.wait_for_keyword(client, "Owned Device List, to Provision the Pairwise, is Empty", 30):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 24 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 24 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 24 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case25(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(25, suite)
    try:
        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        if case.send_command(client, "33\r"):
            if not case.wait_for_keyword(client, "Owned Device List, to Provision ACL, is Empty", 30):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 25 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 25 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 25 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case26(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(26, suite)
    try:
        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        if case.send_command(client, "34\r"):
            if not case.wait_for_keyword(client, "Owned Device List, to Check Linked Status on PRVN DB, is Empty", 30):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 26 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 26 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 26 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case27(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(27, suite)
    try:
        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        if case.send_command(client, "40\r"):
            if not case.wait_for_keyword(client, "Owned Device List, to Unlink the Pairwise, is Empty", 30):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 27 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 27 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 27 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case28(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(28, suite)
    try:
        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        if case.send_command(client, "50\r"):
            if not case.wait_for_keyword(client, "Owned Device List, to Remove Device, is Empty", 30):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 28 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 28 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 28 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case29(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(29, suite)
    try:
        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        if case.send_command(client, "60\r"):
            if not case.wait_for_keyword(client, "Owned Device List, to Check Linked Status on PRVN DB, is Empty", 30):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 29 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 29 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 29 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case30(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(30, suite)
    try:
        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        if case.send_command(client, "61\r"):
            if not case.wait_for_keyword(client, "Owned Device List, to Check Linked Status on PRVN DB, is Empty", 30):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 30 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 30 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 30 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case31(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(18, suite)
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        device = case.discover_and_own(client, server_randompin, 3)
        if not device:
            FAIL_TRIGER = 1

        if not case.provide_acl(client, "30", "double"):
            FAIL_TRIGER = 1

        if not case.wait_for_keyword(client, "Provision Pairwise SUCCEEDED", 40):
            FAIL_TRIGER = 1

        if case.send_command(client, "51\r"):
            case.wait_for_keyword(client, "Input the UUID", 10)
            case.send_command(client, device)
            if case.wait_for_keyword(client, "Remove Device SUCCEEDED", 40):
                print("Device " + str(device) + " removed")
            else:
                print("Device" + str(device) + " remove FAIL!")

        if FAIL_TRIGER:
            print("! |Test Case 31 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 31 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 31 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


def case32(suite):
    FAIL_TRIGER = 0

    global FAIL_COUNT
    case = Test(32, suite)
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        if not case.discover_and_own(client, server_randompin, 1):
            FAIL_TRIGER = 1

        if case.send_command(client, "52\r"):
            case.wait_for_keyword(client, ":", 10)
            case.send_command(client, "1\r")
            if not case.wait_for_keyword(client, "Reset Selected Owned Device SUCCEEDED", 50):
                FAIL_TRIGER = 1

        case.wait_for_keyword(client, ":", 1)

        if case.send_command(client, "52\r"):
            case.wait_for_keyword(client, ":", 10)
            case.send_command(client, "1\r")
            if not case.wait_for_keyword(client, "Reset Selected Owned Device SUCCEEDED", 50):
                FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print("! |Test Case 32 FAILED| !")
            FAIL_COUNT += 1
        else:
            print("! |Test Case 32 passed| !")

    except:
        FAIL_COUNT += 1
        print "! |Test Case 32 FAILED| !"
    finally:
        case.stop_instance(client)
        case.stop_instance(server_randompin)
        case.stop_instance(server_justworks)


if __name__ == "__main__":
    global FAIL_COUNT
    suite01 = Service(01)
    suite01.save_config()
    case01(suite01)
    case02(suite01)
    case03(suite01)
    case04(suite01)
    case05(suite01)
    case06(suite01)
    case07(suite01)
    case08(suite01)
    case09(suite01)
    case10(suite01)
    case11(suite01)
    case12(suite01)
    case13(suite01)
    case14(suite01)
    case15(suite01)
    case16(suite01)
    case17(suite01)
    case18(suite01)
    case19(suite01)
    case20(suite01)
    case21(suite01)
    case22(suite01)
    case23(suite01)
    case24(suite01)
    case25(suite01)
    case26(suite01)
    case27(suite01)
    case28(suite01)
    case29(suite01)
    case30(suite01)
    case31(suite01)
    case32(suite01)
    print (str(32 - FAIL_COUNT)) + " out of 33 test cases passed!"
    print (str(FAIL_COUNT)) + " out of 33 test cases failed!"
    suite01.restore_config()
