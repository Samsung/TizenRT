#!/usr/bin/env python

import pexpect
import re
import os
import signal
import shutil
import system_test
import sys
import getopt
import time
from time import gmtime, strftime, sleep

DEBUG = False
LOGGING = 1

# Class to handle test suites initialization, as well as common file operations
class Service:
    def __init__(self, number):

        print("Servicing test suite #" + str(number))

# Basically mimics git init && git add *.dat && git commit -m1, backs up .dat db files for sample servers
# Without clean db files, devices will remain in owned state, forcing a rebuild of iotivity
    def save_config(self):
        try:
            if not os.path.exists("bak"):
                os.mkdir("bak")

                dir_src = "."
                dir_dst = "bak"

                for filename in os.listdir(dir_src):
                    if filename.endswith(".dat"):
                        shutil.copy(os.path.join(dir_src, filename), os.path.join(dir_dst, filename))
                        if DEBUG:
                            print("Saved " + filename)
        except:
            print "Save config file io error!"

# Function to delete .dat and other db files to restore device state
    def _safe_remove(self, filename):
        try:
            if os.path.isfile(filename):
                if DEBUG:
                    print ("Removing: " + filename)
                os.remove(filename)
        except:
            print "Safe removal of files failed!"

# Reset device state to clean (after build) state. Must be performed before each new test case
    def restore_config(self):
        try:
            dir_src = "bak"
            dir_dst = "."

            for filename in os.listdir(dir_src):
                if filename.endswith(".dat"):
                    shutil.copy(os.path.join(dir_src, filename), os.path.join(dir_dst, filename))
                    if DEBUG:
                        print("Restored: " + filename)

            self._safe_remove("oic_prvn_mng.db")
            self._safe_remove("device_properties.dat")
            self._safe_remove("rootca.crt")
            self._safe_remove("PDM.dat")
        except:
            print "Restore config file io error!"

# Simataneously write all output to a timestamped log file, use -n to disable
class Logger(object):
    def __init__(self):
        try:
            self.terminal = sys.stdout
            self.name = strftime("%Y-%m-%d_%H-%M-%S", gmtime())+"_systest.log"
            self.log = open(self.name, "a")
        except:
            print "Log file creation failed!"

    def write(self, message):
        try:
            self.terminal.write(message)
            self.log.write(message)
        except:
            print "Write to screen or logfile error!"

# Main class for system test cases
class Test:
    # Sample servers execution commands and expected output strings
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

    _SAMPLE_PRECONFPIN = "./sampleserver_preconfpin"
    _PRECONFPIN_CHECKER = "SAMPLE_PRECONF_PIN: Entering ocserver main loop"
    _PRECONFPIN_START_OK = "Sample Server PRECONFPIN started successfully"
    _PRECONFPIN_START_FAIL = "Sample Server PRECONFPIN start failed"

    _PROV_CLIENT = "./provisioningclient"
    _CLIENT_CHECKER = "Enter Menu Number:"
    _CLIENT_START_OK = "Provisioning client started successfully"
    _CLIENT_START_FAIL = "Provisioning client start failed"

    _SUBOWN_CLIENT = "./subownerclient"
    _SUBOWN_CHECKER = "Enter Menu Number:"
    _SUBOWN_START_OK = "Subownerlient client started successfully"
    _SUBOWN_START_FAIL = "Subownerlient client start failed"

    _1234_PIN = "12341234\r"

    _ACL_OK = "Provision ACL SUCCEEDED"
    _OWN_TRANS_OK = "Ownership Transfer SUCCEEDED"
    _FOUND_UNOWN_DEVS = "Discovered Unowned Devices"
    _FOUND_OWN_DEVS = "Discovered Owned Devices"
    _OIC_PROV = "OIC Provisioning"
    _IS_OWN = " is now owned"
    _NOT_OWN = " not found in owned devices list!"

    # ANSII color characters for color coding. To make output more readable.
    _FAIL = '\033[91m'
    _OKGREEN = '\033[92m'
    _OKBLUE = '\033[94m'
    _ENDC = '\033[0m'

    # Timeout in seconds for expecting ceratin text outputs from pexpect. A balance between performance and wrong output
    # should be considered
    _MAIN_TIMEOUT = 40
    _SHORT_TIMEOUT = 30
    _LONG_TIMEOUT = 50
    # Timeout for waiting for process start
    _EXEC_TIMEOUT = 7
    # Timeout for waiting for client to ask for user input. Usually this take less than 1 second, but slowdowns happened
    _INPUT_TIMEOUT = 10

    def __init__(self, number, prepare):

        self.kill_all()
        # Test case counter, with failsafe db files restoration
        print(self._OKBLUE + "Test case #" + str(number) + " start" + self._ENDC)
        prepare.restore_config()

    # Main pexpect process execute wrapper. [cmd] is the shell execute command, [cheker] is the expected text output
    # to confirm that the process started ok, [delay] is how long will will the checker message be waited for before an
    # exception is thrown. Returns proc handle.
    def _run_process(self, cmd, checker, delay):
        try:
            proc = pexpect.spawn(cmd)
            keyword = re.compile(checker)
            if proc.expect(keyword, timeout=delay) == 0:
                if DEBUG:
                    print("Process " + cmd + " succeeded!")
                return proc
            else:
                print(self._FAIL + "Process " + cmd + " failed!" + self._ENDC)
                return 0
        except:
            print(self._FAIL + "Process " + cmd + " failed!" + self._ENDC)

    # To close pexpect spawns, and sigkill process anyway if spawn close fails. [run_proc] is pexpect file hanlde

    def kill_all(self):
        from re import compile
        server_bin = compile('sampleserver_.*')
        try:
            from psutil import process_iter
            for proc in process_iter():
                name = None
                if callable(getattr(proc, 'name')):
                    name = proc.name()
                else:
                    name = proc.name
                if server_bin.match(name):
                    proc.kill()
        except:
            print "KillAll error!"


    def _kill_process(self, run_proc):

        if run_proc:
            pid = run_proc.pid
        else:
            pid = 0
        try:
            run_proc.close
        except:
            if pid:
                os.kill(pid, signal.SIGKILL)

    # Convinience wrapper for terminating spawned processes. [run_proc] is pexpect file hanlde
    def stop_instance(self, run_proc):
        if run_proc:
            print("Killing process " + str(run_proc.pid))
            self._kill_process(run_proc)
        else:
           print "Stop instance received empty proc handle!"

    # Launch sample server justworks, return pexpect proc handle
    def start_justworks(self):

        run = self._run_process(self._SAMPLE_JUSTWORKS, self._JUSTWORKS_CHECKER, self._EXEC_TIMEOUT)

        if run:
            print(self._JUSTWORKS_START_OK)
            return run
        else:
            print(self._JUSTWORKS_START_FAIL)
            # try to flush any info about proc start failure, sometimes catches something
            self.flush_output(run)
            return 0

    # Launch sample server randompin, return pexpect proc handle
    def start_randompin(self):

        run = self._run_process(self._SAMPLE_RANDOMPIN, self._RANDOMPIN_CHECKER, self._EXEC_TIMEOUT)

        if run:
            print(self._RANDOMPIN_START_OK)
            return run
        else:
            print(self._RANDOMPIN_START_FAIL)
            self.flush_output(run)
            return 0

    # Launch sample server mfg, return pexpect proc handle
    def start_mfg(self):

        run = self._run_process(self._SAMPLE_MFG, self._MFG_CHECKER, self._EXEC_TIMEOUT)

        if run:
            print(self._MFG_START_OK)
            return run
        else:
            print(self._MFG_START_FAIL)
            self.flush_output(run)
            return 0

    # Launch sample server preconfpin, return pexpect proc handle
    def start_preconfpin(self):

        run = self._run_process(self._SAMPLE_PRECONFPIN, self._PRECONFPIN_CHECKER, self._EXEC_TIMEOUT)

        if run:
            print(self._PRECONFPIN_START_OK)
            return run
        else:
            print(self._PRECONFPIN_START_FAIL)
            self.flush_output(run)
            return 0

    # Launch provisioning client, return pexpect proc handle
    def start_client(self):

        run = self._run_process(self._PROV_CLIENT, self._CLIENT_CHECKER, self._EXEC_TIMEOUT)

        if run:
            print(self._CLIENT_START_OK)
            return run
        else:
            print(self._CLIENT_START_FAIL)
            self.flush_output(run)
            return 0

    # Launch subowner client, return pexpect proc handle
    def start_subown(self):

        run = self._run_process(self._SUBOWN_CLIENT, self._SUBOWN_CHECKER, self._EXEC_TIMEOUT)

        if run:
            print(self._SUBOWN_START_OK)
            return run
        else:
            print(self._SUBOWN_START_FAIL)
            self.flush_output(run)
            return 0

    # Execute a common for many test cases, [11]+[20]+[12] action. It doscovers unowned devices, transfers ownership,
    # then discovers owned devices. [run_proc] is the provisioning client handle, [secured] toggles weather randompin
    # server is launched and is his file handle if so, [additional_servers] is a macro to specify any other servers
    # that are launched, as well as whic server UID should be returned.
    def discover_and_own(self, run_proc, secured, additional_servers):

        pin = 0
        device_1 = 0
        device_2 = 0
        device_3 = 0

        if self.send_command(run_proc, "11\r"):
            if self.wait_for_keyword(run_proc, self._FOUND_UNOWN_DEVS, self._MAIN_TIMEOUT):
                if not self.wait_for_keyword(run_proc, self._OIC_PROV, self._MAIN_TIMEOUT):
                    return 0
                device_id = run_proc.before
                print ("Discovered Unowned Device " + device_id)
                device_1 = device_id.partition("[1]")[-1].split("\r")[0]
                # Something other than justworks is launched
                if additional_servers > 0 and additional_servers != 5:
                    device_2 = device_id.partition("[2]")[-1].split("\r")[0]
                    # MFG server is launched
                    if additional_servers == 2 or additional_servers == 4:
                        device_3 = device_id.partition("[2]")[-1].split("\r")[0]
                if self.send_command(run_proc, "20\r"):
                    # grab pin from randompin server output
                    if secured:
                        pin = self.get_pin(secured)
                        if pin:
                            time.sleep(1)
                            self.send_command(run_proc, pin)
                    if self.wait_for_keyword(run_proc, self._OWN_TRANS_OK, self._MAIN_TIMEOUT):
                        if self.send_command(run_proc, "12\r"):
                            if self.wait_for_keyword(run_proc, self._FOUND_OWN_DEVS, self._MAIN_TIMEOUT):
                                if not self.wait_for_keyword(run_proc, self._OIC_PROV, self._MAIN_TIMEOUT):
                                    return 0
                                discovered_devices = run_proc.before
                                if device_1 in discovered_devices:
                                    print("Device [1] " + device_1 + self._IS_OWN)
                                else:
                                    print(self._FAIL + "Device [1] " + device_1 + self._NOT_OWN + self._ENDC)
                                    return 0
                                if additional_servers > 0 and additional_servers != 5:
                                    if device_2 in discovered_devices:
                                        print("Device [2] " + device_2 + self._IS_OWN)
                                    else:
                                        print(self._FAIL + "Device [2] " + device_2 + self._NOT_OWN + self._ENDC)
                                        return 0
                                    if additional_servers == 2:
                                        if device_3 in discovered_devices:
                                            print("Device [3] " + device_3 + self._IS_OWN)
                                        else:
                                            print(self._FAIL + "Device [3] " + device_3 + self._NOT_OWN + self._ENDC)
                                            return 0
                                # return justworks UID
                                if additional_servers == 3:
                                    return device_1
                                # return randompin UID
                                if additional_servers == 4:
                                    return device_2
                                if additional_servers == 5 and pin:
                                    return device_1 , pin
                                # return default device1
                                return device_1
        return 0

    # Execute a common for subwoner test cases, [10]+[20]+[11] action.
    def discover_and_subown(self, run_proc, secured, additional_servers):

        if self.send_command(run_proc, "10\r"):
            if self.wait_for_keyword(run_proc, "Discovered Multiple Ownership Transfer Enabled Devices", self._MAIN_TIMEOUT):
                if not self.wait_for_keyword(run_proc, self._OIC_PROV, self._MAIN_TIMEOUT):
                    return 0
                device_id = run_proc.before
                print ("Discovered Multiple Ownership Transfer Enabled Device " + device_id)
                device_1 = device_id.partition("[1]")[-1].split("\r")[0]
                # Something other than justworks is launched
                if additional_servers > 0:
                    device_2 = device_id.partition("[2]")[-1].split("\r")[0]
                    # MFG server is launched
                    if additional_servers == 2 or additional_servers == 4:
                        device_3 = device_id.partition("[2]")[-1].split("\r")[0]
                if self.send_command(run_proc, "20\r"):
                    # grab pin from randompin server output
                    if secured:
                        pin = self.get_subpin(secured)
                        if pin:
                            self.wait_for_keyword(run_proc, "INPUT PIN:", self._MAIN_TIMEOUT)
                            self.send_command(run_proc, pin)
                    if self.wait_for_keyword(run_proc, "Multiple Ownership Transfer SUCCEEDED", self._MAIN_TIMEOUT):
                        if self.send_command(run_proc, "11\r"):
                            if self.wait_for_keyword(run_proc, "Discovered Multiple Owned Devices", self._MAIN_TIMEOUT):
                                if not self.wait_for_keyword(run_proc, self._OIC_PROV, self._MAIN_TIMEOUT):
                                    return 0
                                discovered_devices = run_proc.before
                                if device_1 in discovered_devices:
                                    print("Device [1] " + device_1 + " is now multiple owned")
                                else:
                                    print(
                                    self._FAIL + "Device [1] " + device_1 + " not found in multiple owned devices list!" + self._ENDC)
                                    return 0
                                if additional_servers > 0:
                                    if device_2 in discovered_devices:
                                        print("Device [2] " + device_2 + " is now multiple owned")
                                    else:
                                        print(
                                        self._FAIL + "Device [2] " + device_2 + " not found in multiple owned devices list!" + self._ENDC)
                                        return 0
                                    if additional_servers == 2:
                                        if device_3 in discovered_devices:
                                            print("Device [3] " + device_3 + " is now multiple owned")
                                        else:
                                            print(
                                            self._FAIL + "Device [3] " + device_3 + " not found in multiple owned devices list!" + self._ENDC)
                                            return 0
                                # return justworks UID
                                if additional_servers == 3:
                                    return device_1
                                # return randompin UID
                                if additional_servers == 4:
                                    return device_2
                                # return default device1
                                return device_1
        return 0
    # Block to execute [30] and similar ACL menu fuctions. [run_proc] is provisioning client file handle, [cmd] is actual
    # menu number. [mode] specifies weather 1 or 2 devices acl permissions are specified
    def provide_acl(self, run_proc, cmd, mode):

        self.wait_for_keyword(run_proc, self._CLIENT_CHECKER, 1)
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
            # flag that everythong is ok
            return 1
        else:
            return 0

    # Grab pin from randompin server output. [randompin_server] is the randompin server file handle. Returns pin
    def get_pin(self, randompin_server):

        if self.wait_for_keyword(randompin_server, "PIN CODE :", self._MAIN_TIMEOUT):
            self.wait_for_keyword(randompin_server, "============================", self._INPUT_TIMEOUT)
            pin_dump = randompin_server.before
            return pin_dump[0:9]
        else:
            print (self._FAIL + "Can not get PIN!" +  self._ENDC)
            return 0

    def get_subpin(self, randompin_server):

        if randompin_server == self._1234_PIN:
            return "12341234\r"
        else:
            return randompin_server

    # Dispay any output available during function call, for debugging
    def flush_output(self, run_proc):

        if run_proc:
            print(run_proc.before)
            print(run_proc.after)
            return 1
        else:
            print(self._FAIL + "Process" + str(run_proc.pid) + " not found" + self._ENDC)
            return 0

    # Return any output available during function call, for error catching
    def dump_output(self, run_proc):

        if run_proc:
            out = []
            out.append(run_proc.before)
            out.append(run_proc.after)
            return out
        else:
            print(self._FAIL + "Process" + str(run_proc.pid) + " not found" + self._ENDC)
            return 0

    # Send input to pexpect spawned process. Menu numbers in this case. [run_proc] is file handle, [cmd] is actual imput.
    # return carrigae added for safety on some linux systems
    def send_command(self, run_proc, cmd):
        try:
            if run_proc:
                run_proc.sendline(cmd)
                if cmd != "y \r":
                    print("Sent " + '\033[93m' + re.sub("\r", "[ENTER]", cmd) + '\033[0m' + " to process " + str(run_proc.pid))
                return 1
            else:
                print(self._FAIL + "Process" + str(run_proc.pid) + " not found" + self._ENDC)
                return 0
        except:
            print "send command [" + cmd + "]  error!"

    # Await expected text output[keyword] from process[runproc] for [delay] seconds.
    # This is the most common place for a test case to fail.
    def wait_for_keyword(self, run_proc, keyword, delay):
        try:
            if run_proc:
                key = re.compile(keyword)
                if run_proc.expect(key, timeout=delay) == 0:
                    if keyword != ":":
                        print("Keyword |" + '\033[96m' + keyword + '\033[0m' "| found in output of process " + str(run_proc.pid))
                    return 1
                else:
                    print(self._FAIL + "Keyword |" + '\033[91m' + keyword + '\033[0m' + "| absent in process" + str(run_proc.pid) + self._ENDC)
                    return 0
            else:
                print(self._FAIL + "Process" + str(run_proc.pid) + " not found" + self._ENDC)
                return 0
        except:
            print "Timeout during [" + keyword + "] await"
            print "|Attempting internal output dump|:\n\n\n"
            self.flush_output(run_proc)
            print "| Internal Output Dump end. Attempting aditional output exctaction| \n\n\n"


def case1(suite):
    case = Test(1, suite)
    print ("StartProvisioningClient: Positive")
    client = 0
    try:
        client = case.start_client()
        if client:
            print (case._OKGREEN + "! |Test Case 01 passed| !" + case._ENDC)
            return 0
        else:
            print(case._FAIL + "! |Test Case 01 FAILED| !" + case._ENDC)
            return 1
    except:
        print case._FAIL + "! |Test Case 01 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)


def case2(suite):
    case = Test(2, suite)
    print ("StartJustWorksServer: Positive")
    server_justworks = 0
    try:
        server_justworks = case.start_justworks()
        if server_justworks:
            print (case._OKGREEN + "! |Test Case 02 passed| !" + case._ENDC)
            return 0
        else:
            print(case._FAIL + "! |Test Case 02 FAILED| !" + case._ENDC)
            return 1

    except:
        print case._FAIL + "! |Test Case 02 FAILED| !" + case._ENDC
        return 1
    finally:
        if server_justworks:
            case.stop_instance(server_justworks)


def case3(suite):
    case = Test(3, suite)
    print ("StartRandomPinServer: Positive")
    server_randompin = 0
    try:
        server_randompin = case.start_randompin()
        if server_randompin:
            print (case._OKGREEN + "! |Test Case 03 passed| !" + case._ENDC)
            return 0
        else:
            print(case._FAIL + "! |Test Case 03 FAILED| !" + case._ENDC)
            return 1
    except:
        print case._FAIL + "! |Test Case 03 FAILED| !" + case._ENDC
        return 1
    finally:
        if server_randompin:
            case.stop_instance(server_randompin)


def case4(suite):
    FAIL_TRIGER = 0
    case = Test(4, suite)
    print ("DisocverUnownedDevices: Positive")
    server_justworks = 0
    client = 0
    try:
        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        if case.send_command(client, "11\r"):
            if not case.wait_for_keyword(client, case._FOUND_UNOWN_DEVS, case._MAIN_TIMEOUT):
                FAIL_TRIGER = 1
                # if not case.wait_for_keyword(client, "[1]", case._MAIN_TIMEOUT):
                # FAIL_TRIGER = 1
            case.wait_for_keyword(client, case._OIC_PROV, case._MAIN_TIMEOUT)
            device_id = client.before[2:38]
            print ("Discovered Unowned Device " + device_id)
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL+ "! |Test Case 04 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 04 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 04 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_justworks:
            case.stop_instance(server_justworks)


def case5(suite):
    FAIL_TRIGER = 0
    case = Test(5, suite)
    print ("DoOwnershipTransfer_JustWorKs Positive")
    server_justworks = 0
    client = 0
    try:
        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        if case.send_command(client, "11\r"):
            if not case.wait_for_keyword(client, case._FOUND_UNOWN_DEVS, case._MAIN_TIMEOUT):
                FAIL_TRIGER = 1
                # if not case.wait_for_keyword(client, "[1]", 10):
                # FAIL_TRIGER = 1
            case.wait_for_keyword(client, case._OIC_PROV, case._MAIN_TIMEOUT)
            device_id = client.before[2:38]
            print ("Discovered Unowned Device " + device_id)
        else:
            FAIL_TRIGER = 1

        if case.send_command(client, "20\r"):
            if not case.wait_for_keyword(client, case._OWN_TRANS_OK, case._MAIN_TIMEOUT):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 05 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 05 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 05 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_justworks:
            case.stop_instance(server_justworks)


def case6(suite):
    FAIL_TRIGER = 0
    case = Test(6, suite)
    print ("DoOwnershipTransfer_RandomPin: Positive")
    server_randompin = 0
    client = 0
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        if case.send_command(client, "11\r"):
            if not case.wait_for_keyword(client, case._FOUND_UNOWN_DEVS, case._MAIN_TIMEOUT):
                FAIL_TRIGER = 1
                # if not case.wait_for_keyword(client, "[1]", 10):
                # FAIL_TRIGER = 1
            case.wait_for_keyword(client, case._OIC_PROV, case._MAIN_TIMEOUT)
            device_id = client.before[2:38]
            print ("Discovered Unowned Device " + device_id)
        else:
            FAIL_TRIGER = 1

        if case.send_command(client, "20\r"):
            pin = case.get_pin(server_randompin)
            case.send_command(client, pin)
            if not case.wait_for_keyword(client, case._OWN_TRANS_OK, case._MAIN_TIMEOUT):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL+ "! |Test Case 06 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 06 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 06 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)


def case7(suite):
    FAIL_TRIGER = 0
    case = Test(7, suite)
    print ("DoOwnershipTransfer_RandomPin_WrongPin: Negative")
    server_randompin = 0
    client = 0
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        if case.send_command(client, "11\r"):
            if not case.wait_for_keyword(client, case._FOUND_UNOWN_DEVS, case._MAIN_TIMEOUT):
                FAIL_TRIGER = 1
            case.wait_for_keyword(client, case._OIC_PROV, case._MAIN_TIMEOUT)
            device_id = client.before[2:38]
            print ("Discovered Unowned Device " + device_id)
        else:
            FAIL_TRIGER = 1

        if case.send_command(client, "20\r"):
            case.wait_for_keyword(client, "INPUT PIN:", case._MAIN_TIMEOUT)
            for pin_attempts in range(1, 5):
                case.send_command(client, str(pin_attempts))
                case.wait_for_keyword(client, "INPUT PIN:", case._MAIN_TIMEOUT)
            case.send_command(client, str("should fail after fifth"))
            if not case.wait_for_keyword(client, "Ownership Transfer FAILED", case._MAIN_TIMEOUT):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 07 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 07 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 07 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)


def case8(suite):
    FAIL_TRIGER = 0
    case = Test(8, suite)
    print ("DisocverOwnedDevices: Positive")
    server_randompin = 0
    client = 0
    server_justworks = 0
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
            print(case._FAIL + "! |Test Case 08 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 08 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 08 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case9(suite):
    FAIL_TRIGER = 0
    case = Test(9, suite)
    print ("ProvisionACL: Positive")
    server_randompin = 0
    client = 0
    server_justworks = 0
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

        if not case.wait_for_keyword(client, case._ACL_OK, case._MAIN_TIMEOUT):
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 09 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 09 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 09 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case10(suite):
    FAIL_TRIGER = 0
    case = Test(10, suite)
    print ("ProvisionACL: Negative")
    server_randompin = 0
    client = 0
    server_justworks = 0
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

        case.wait_for_keyword(client, case._CLIENT_CHECKER, case._MAIN_TIMEOUT)
        if case.send_command(client, "32\r"):
            for x in xrange(2):
                case.wait_for_keyword(client, ":", 10)
                case.send_command(client, "1\r")
            case.wait_for_keyword(client, "Entered Wrong Number. Please Enter Again", case._MAIN_TIMEOUT)

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 10 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 10 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 10 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case11(suite):
    FAIL_TRIGER = 0
    case = Test(11, suite)
    print ("ProvisionPairwiseDevs: Positive")
    server_randompin = 0
    client = 0
    server_justworks = 0
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

        if not case.wait_for_keyword(client, "Provision Pairwise SUCCEEDED", case._MAIN_TIMEOUT):
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 11 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 11 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 11 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case12(suite):
    FAIL_TRIGER = 0
    case = Test(12, suite)
    print ("ProvisionCredential_Keysize_128: Positive")
    server_randompin = 0
    client = 0
    server_justworks = 0
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

        case.wait_for_keyword(client, case._CLIENT_CHECKER, 3)
        if case.send_command(client, "31\r"):
            case.wait_for_keyword(client, ":", 1)
            case.send_command(client, "1\r")
            case.wait_for_keyword(client, ":", 1)
            case.send_command(client, "2\r")
            case.wait_for_keyword(client, "Select PSK length..", case._INPUT_TIMEOUT)
            case.send_command(client, "1")
            case.send_command(client, "1 \r")
            case.send_command(client, "1")

            case.wait_for_keyword(client, "Provision Credential SUCCEEDED", case._MAIN_TIMEOUT)

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 12 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 12 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 12 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case13(suite):
    FAIL_TRIGER = 0
    case = Test(13, suite)
    print ("ProvisionCredential_Keysize_256: Positive")
    server_randompin = 0
    client = 0
    server_justworks = 0
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

        case.wait_for_keyword(client, case._CLIENT_CHECKER, case._SHORT_TIMEOUT)
        if case.send_command(client, "31\r"):
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "1\r")
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "2\r")
            case.wait_for_keyword(client, "Select PSK length..", case._INPUT_TIMEOUT)
            case.send_command(client, "2")
            case.send_command(client, "2 \r")
            case.send_command(client, "2")

            case.wait_for_keyword(client, "Provision Credential SUCCEEDED", case._MAIN_TIMEOUT)

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 13 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 13 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 13 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case14(suite):
    FAIL_TRIGER = 0
    case = Test(14, suite)
    print ("ProvisionCredential_Keysize_Default: Positive")
    server_randompin = 0
    client = 0
    server_justworks = 0
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

        case.wait_for_keyword(client, case._CLIENT_CHECKER, case._SHORT_TIMEOUT)
        if case.send_command(client, "31\r"):
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "1\r")
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "2\r")
            case.wait_for_keyword(client, "Select PSK length..", case._INPUT_TIMEOUT)
            case.send_command(client, "0")
            case.send_command(client, "0 \r")
            case.send_command(client, "0")

            case.wait_for_keyword(client, "Provision Credential SUCCEEDED", case._MAIN_TIMEOUT)

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 14 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 14 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 14 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case15(suite):
    print "Test deprecated"
    return 0


def case16(suite):
    FAIL_TRIGER = 0
    case = Test(16, suite)
    print ("GetLinkedStatus: Positive")
    server_randompin = 0
    client = 0
    server_justworks = 0
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

        if not case.wait_for_keyword(client, "Provision Pairwise SUCCEEDED", case._MAIN_TIMEOUT):
            FAIL_TRIGER = 1

        if device:
            if case.send_command(client, "33\r"):
                case.wait_for_keyword(client, "Status on PRVN DB", case._INPUT_TIMEOUT)
                case.send_command(client, "1\r")
                time.sleep(1)
                if case.wait_for_keyword(client, "Checked Selected Link Status on PRVN DB", case._SHORT_TIMEOUT):
                    print("Checked device status OK " + str(device))
                else:
                    print("Device status check FAIL! " + str(device) + " not found!" + case._ENDC)
                    FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 16 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 16 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 16 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case17(suite):
    FAIL_TRIGER = 0
    case = Test(17, suite)
    print ("UnlinkDev: Positive")
    server_randompin = 0
    client = 0
    server_justworks = 0
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

        if not case.wait_for_keyword(client, "Provision Pairwise SUCCEEDED", case._MAIN_TIMEOUT):
            FAIL_TRIGER = 1

        if case.send_command(client, "40\r"):
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "1\r")
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "2\r")
            if case.wait_for_keyword(client, "Unlinked Selected Pairwise Devices", case._MAIN_TIMEOUT):
                print("Devices unlinked")
            else:
                print("Device unlink FAIL!" + case._ENDC)
                FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 17 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 17 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 17 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case18(suite):
    FAIL_TRIGER = 0
    case = Test(18, suite)
    print ("RemoveDevs: Positive")
    server_randompin = 0
    client = 0
    server_justworks = 0
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

        if not case.wait_for_keyword(client, "Provision Pairwise SUCCEEDED", case._MAIN_TIMEOUT):
            FAIL_TRIGER = 1

        case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)

        if case.send_command(client, "50\r"):
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "1\r")
            if case.wait_for_keyword(client, "Remove Device SUCCEEDED", case._MAIN_TIMEOUT):
                print("Device removed")
            else:
                print("Device remove FAIL!" + case._ENDC)
                FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 18 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 18 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 18 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case19(suite):
    FAIL_TRIGER = 0
    case = Test(19, suite)
    print ("GetCredentialResources: Positive")
    server_randompin = 0
    client = 0
    server_justworks = 0
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

        if not case.wait_for_keyword(client, case._ACL_OK, case._MAIN_TIMEOUT):
            FAIL_TRIGER = 1

        if case.send_command(client, "60\r"):
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "1\r")
            if case.wait_for_keyword(client, "Get Cred SUCCEEDED", case._MAIN_TIMEOUT):
                print("Credential provision ok")
            else:
                print("Credential provision FAIL!" + case._ENDC)
                FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 19 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 19 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 19 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case20(suite):
    FAIL_TRIGER = 0
    case = Test(20, suite)
    print ("GetAclResources: Positive")
    server_randompin = 0
    client = 0
    server_justworks = 0
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

        if not case.wait_for_keyword(client, case._ACL_OK, case._MAIN_TIMEOUT):
            FAIL_TRIGER = 1

        if case.send_command(client, "61\r"):
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "1\r")
            if case.wait_for_keyword(client, case._OIC_PROV, case._MAIN_TIMEOUT):
                case.flush_output(client)
                print("Credential provision ok")
            else:
                print("Credential provision FAIL!" + case._ENDC)
                FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 20 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 20 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 20 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case21(suite):
    FAIL_TRIGER = 0
    case = Test(21, suite)
    print ("DiscoverAllDevices: Positive")
    server_randompin = 0
    client = 0
    server_justworks = 0
    try:
        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        if case.send_command(client, "11\r"):
            if case.wait_for_keyword(client, case._FOUND_UNOWN_DEVS, case._MAIN_TIMEOUT):
                if not case.wait_for_keyword(client, case._OIC_PROV, case._MAIN_TIMEOUT):
                    return 0
                device_id = client.before
                print ("Discovered Unowned Device " + device_id)
                device_1 = device_id.partition("[1]")[-1].split("\r")[0]

                if case.send_command(client, "20\r"):
                    if case.wait_for_keyword(client, case._OWN_TRANS_OK, case._MAIN_TIMEOUT):

                        server_randompin = case.start_randompin()
                        if not server_randompin:
                            FAIL_TRIGER = 1

                        case.wait_for_keyword(client, case._CLIENT_CHECKER, case._SHORT_TIMEOUT)
                        case.send_command(client, "10\r")
                        if not case.wait_for_keyword(client, case._OIC_PROV, case._MAIN_TIMEOUT):
                            FAIL_TRIGER = 1

                        # case.flush_output(client)

                        discovered_devices = client.before
                        if device_1 in discovered_devices:
                            print("Device [1] " + device_1 + case._IS_OWN)
                        else:
                            print("Device [1] " + device_1 + case._NOT_OWN + case._ENDC)
                            FAIL_TRIGER = 1

                        if "Discovered Unowned Devices\r\n     [1]" in discovered_devices:
                            print("Unowned randompin device found")
                        else:
                            print("Unowned randompin device not found!" + case._ENDC)
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
            print(case._FAIL + "! |Test Case 21 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 21 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 21 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case22(suite):
    FAIL_TRIGER = 0
    case = Test(22, suite)
    print("ProvisionAclWithoutDiscoveringOwnedDev: Negative")
    server_randompin = 0
    client = 0
    server_justworks = 0
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
            if not case.wait_for_keyword(client, "Owned Device List, to Provision ACL, is Empty", case._SHORT_TIMEOUT):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 22 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 22 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 22 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case23(suite):
    FAIL_TRIGER = 0
    case = Test(23, suite)
    print ("ProvisionCredentialWithoutDiscoveringOwnedDev: Negative")
    server_randompin = 0
    client = 0
    server_justworks = 0
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
            if not case.wait_for_keyword(client, "Owned Device List, to Provision Credentials, is Empty", case._SHORT_TIMEOUT):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 23 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 23 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 23 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case24(suite):
    FAIL_TRIGER = 0
    case = Test(24, suite)
    print ("ProvisionPairwiseDevsWithoutDiscoveringOwnedDev: Negative")
    server_randompin = 0
    client = 0
    server_justworks = 0
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
            if not case.wait_for_keyword(client, "Owned Device List, to Provision the Pairwise, is Empty", case._SHORT_TIMEOUT):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 24 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 24 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 24 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case25(suite):
    print "Test deprecated"
    return 0


def case26(suite):
    FAIL_TRIGER = 0
    case = Test(26, suite)
    print ("GetLinkedStatusWithoutDiscoveringOwnedDev: Negative")
    server_randompin = 0
    client = 0
    server_justworks = 0
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
            if not case.wait_for_keyword(client, "Owned Device List, to Check Linked Status on PRVN DB, is Empty", case._SHORT_TIMEOUT):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 26 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 26 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 26 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case27(suite):
    FAIL_TRIGER = 0
    case = Test(27, suite)
    print ("UnlinkWithoutDiscoveringOwnedDev: Negative")
    server_randompin = 0
    client = 0
    server_justworks = 0
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
            if not case.wait_for_keyword(client, "Owned Device List, to Unlink the Pairwise, is Empty", case._SHORT_TIMEOUT):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 27 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 27 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 27 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case28(suite):
    FAIL_TRIGER = 0
    case = Test(28, suite)
    print ("RemoveWithoutDiscoveringOwnedDev: Negative")
    server_randompin = 0
    client = 0
    server_justworks = 0
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
            if not case.wait_for_keyword(client, "Owned Device List, to Remove Device, is Empty", case._SHORT_TIMEOUT):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 28 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 28 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 28 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case29(suite):
    FAIL_TRIGER = 0
    case = Test(29, suite)
    print ("GetCredentialResources: Negative")
    server_randompin = 0
    client = 0
    server_justworks = 0
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
            if not case.wait_for_keyword(client, "Owned Device List, to Check Linked Status on PRVN DB, is Empty", case._SHORT_TIMEOUT):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 29 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 29 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 29 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case30(suite):
    FAIL_TRIGER = 0
    case = Test(30, suite)
    print ("GetAclResources: Negative")
    server_randompin = 0
    client = 0
    server_justworks = 0
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
            if not case.wait_for_keyword(client, "Owned Device List, to Check Linked Status on PRVN DB, is Empty", case._SHORT_TIMEOUT):
                FAIL_TRIGER = 1
        else:
            FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 30 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 30 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 30 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case31(suite):
    FAIL_TRIGER = 0
    case = Test(31, suite)
    print ("RemoveDeviceWithUUID: Positive")
    server_randompin = 0
    client = 0
    server_justworks = 0
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

        if not case.wait_for_keyword(client, "Provision Pairwise SUCCEEDED", case._MAIN_TIMEOUT):
            FAIL_TRIGER = 1

        if case.send_command(client, "51\r"):
            case.wait_for_keyword(client, "Input the UUID", case._INPUT_TIMEOUT)
            case.send_command(client, device)
            if case.wait_for_keyword(client, "Remove Device SUCCEEDED", case._MAIN_TIMEOUT):
                print("Device " + str(device) + " removed")
            else:
                print(case._FAIL + "Device" + str(device) + " remove FAIL!" + case._ENDC)
                FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 31 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 31 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 31 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)


def case32(suite):
    FAIL_TRIGER = 0
    case = Test(32, suite)
    print ("ResetDev: Positive")
    server_randompin = 0
    client = 0
    server_justworks = 0
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
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "1\r")
            if not case.wait_for_keyword(client, "Reset Selected Owned Device SUCCEEDED", case._LONG_TIMEOUT):
                FAIL_TRIGER = 1

        case.wait_for_keyword(client, ":", 1)

        if case.send_command(client, "52\r"):
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "1\r")
            if not case.wait_for_keyword(client, "Reset Selected Owned Device SUCCEEDED", case._LONG_TIMEOUT):
                FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 32 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 32 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 32 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if server_justworks:
            case.stop_instance(server_justworks)

def case33(suite):
    FAIL_TRIGER = 0
    case = Test(33, suite)
    print ("ChangeProtocolToDTLS: Positive")
    client = 0
    server_justworks = 0
    try:
        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        server_justworks = case.start_justworks()
        if not server_justworks:
            FAIL_TRIGER = 1

        if not case.discover_and_own(client, 0, 0):
            FAIL_TRIGER = 1

        case.wait_for_keyword(client, case._CLIENT_CHECKER, case._MAIN_TIMEOUT)
        if case.send_command(client, "35\r"):
            if not case.wait_for_keyword(client, "Provision Credential SUCCEEDED", case._MAIN_TIMEOUT):
                FAIL_TRIGER = 1
            if not case.wait_for_keyword(client, "Provision Credential SUCCEEDED", case._MAIN_TIMEOUT):
                FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 33 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 33 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 33 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_justworks:
            case.stop_instance(server_justworks)


def case34(suite):
    FAIL_TRIGER = 0
    case = Test(34, suite)
    print ("ChangeProtocolToTLS: Positive")
    client = 0
    try:
        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        if case.send_command(client, "34\r"):
            if not case.wait_for_keyword(client, "Select Number, for Subject UUID of new ACE", case._MAIN_TIMEOUT):
                FAIL_TRIGER = 1
            if case.send_command(client, "1\r"):
                if not case.wait_for_keyword(client, "Saved Selected ACL", case._MAIN_TIMEOUT):
                    FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 34 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 34 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 34 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)

def case35(suite):
    FAIL_TRIGER = 0
    case = Test(35, suite)
    print ("Change_MOT_Mode: Positive")
    server_randompin = 0
    client = 0
    server_justworks = 0
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

        case.wait_for_keyword(client, case._CLIENT_CHECKER, case._MAIN_TIMEOUT)
        if case.send_command(client, "34\r"):
            if not case.wait_for_keyword(client, "Select Number, for Subject UUID of new ACE", case._INPUT_TIMEOUT):
                FAIL_TRIGER = 1
        if case.send_command(client, "2\r"):
            if not case.wait_for_keyword(client, "Input the UUID", case._INPUT_TIMEOUT):
                FAIL_TRIGER = 1
        if case.send_command(client, device):
            if not case.wait_for_keyword(client, "Saved Selected ACL", case._MAIN_TIMEOUT):
                FAIL_TRIGER = 1

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 35 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 35 passed| !" + case._ENDC)
            return 0

    except:
        print case._FAIL + "! |Test Case 35 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_justworks:
            case.stop_instance(server_justworks)
        if server_randompin:
            case.stop_instance(server_randompin)

def case36(suite):
    case = Test(36, suite)
    print ("Provision_Preconfigured_PIN: Positive")
    client_subown = 0
    try:
        client_subown = case.start_subown()
        if client_subown:
            print (case._OKGREEN + "! |Test Case 36 passed| !" + case._ENDC)
            return 0
        else:
            print(case._FAIL + "! |Test Case 36 FAILED| !" + case._ENDC)
            return 1
    except:
        print case._FAIL + "! |Test Case 36 FAILED| !" + case._ENDC
        return 1
    finally:
        if client_subown:
            case.stop_instance(client_subown)

def case37(suite):
    case = Test(37, suite)
    print ("Change_MOT_Method: Positive")
    server_preconfpin = 0
    try:
        server_preconfpin = case.start_preconfpin()
        if server_preconfpin:
            print (case._OKGREEN + "! |Test Case 37 passed| !" + case._ENDC)
            return 0
        else:
            print(case._FAIL + "! |Test Case 37 FAILED| !" + case._ENDC)
            return 1

    except:
        print case._FAIL + "! |Test Case 37 FAILED| !" + case._ENDC
        return 1
    finally:
        if server_preconfpin:
            case.stop_instance(server_preconfpin)

def case38(suite):
    FAIL_TRIGER = 0
    case = Test(38, suite)
    print ("Start_SubownerClient: Positive")
    server_preconfpin = 0
    client = 0
    client_subown = 0
    try:
        server_preconfpin = case.start_preconfpin()
        if not server_preconfpin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        device = case.discover_and_own(client, 0, 0)
        if not device:
            FAIL_TRIGER = 1

        case.wait_for_keyword(client, case._CLIENT_CHECKER, case._SHORT_TIMEOUT)
        if case.send_command(client, "70\r"):
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "1\r")
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "1\r")
            if not case.wait_for_keyword(client, "POST 'doxm' SUCCEEDED", case._MAIN_TIMEOUT):
                FAIL_TRIGER = 1

        case.wait_for_keyword(client, case._CLIENT_CHECKER, case._SHORT_TIMEOUT)
        if case.send_command(client, "13\r"):
            if not case.wait_for_keyword(client, device, case._LONG_TIMEOUT):
                FAIL_TRIGER = 1

        case.wait_for_keyword(client, case._CLIENT_CHECKER, case._SHORT_TIMEOUT)
        if case.send_command(client, "71\r"):
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "1\r")
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "12341234\r")
            if not case.wait_for_keyword(client, "Provision Credential SUCCEEDED", case._LONG_TIMEOUT):
                FAIL_TRIGER = 1

        case.wait_for_keyword(client, case._CLIENT_CHECKER, case._SHORT_TIMEOUT)
        if case.send_command(client, "72\r"):
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "1\r")
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "65280\r")
            if not case.wait_for_keyword(client, "POST 'doxm' SUCCEEDED", case._MAIN_TIMEOUT):
                FAIL_TRIGER = 1

        client_subown = case.start_subown()
        if not client:
            FAIL_TRIGER = 1

        if not case.discover_and_subown(client_subown, case._1234_PIN, 0):
            FAIL_TRIGER = 1

        case.wait_for_keyword(client, case._CLIENT_CHECKER, case._SHORT_TIMEOUT)

        if case.send_command(client_subown, "40\r"):
            case.wait_for_keyword(client_subown, "Enter Device Number", case._INPUT_TIMEOUT)
            case.send_command(client_subown, "1\r")
            if case.wait_for_keyword(client_subown, case._ACL_OK, case._MAIN_TIMEOUT):
                print("ACL provisioning OK")
            else:
                FAIL_TRIGER = 1
                print(case._FAIL + "ACL provisioning FAIL!" + case._ENDC)

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 38 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 38 passed| !" + case._ENDC)
            return 0
    except:
        print case._FAIL + "! |Test Case 38 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_preconfpin:
            case.stop_instance(server_preconfpin)
        if client_subown:
            case.stop_instance(client_subown)

def case39(suite):
    FAIL_TRIGER = 0
    pin = 0
    case = Test(39, suite)
    print ("Do_Multiple_Ownership_Transfer_JW_Server: Positive")
    server_randompin = 0
    client = 0
    client_subown = 0
    try:
        server_randompin = case.start_randompin()
        if not server_randompin:
            FAIL_TRIGER = 1

        client = case.start_client()
        if not client:
            FAIL_TRIGER = 1

        device, pin = case.discover_and_own(client, server_randompin, 5)
        if not device:
            FAIL_TRIGER = 1

        case.wait_for_keyword(client, case._CLIENT_CHECKER, case._SHORT_TIMEOUT)
        if case.send_command(client, "70\r"):
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "1\r")
            case.wait_for_keyword(client, ":", case._INPUT_TIMEOUT)
            case.send_command(client, "1\r")
            if not case.wait_for_keyword(client, "POST 'doxm' SUCCEEDED", case._MAIN_TIMEOUT):
                FAIL_TRIGER = 1

        case.wait_for_keyword(client, case._CLIENT_CHECKER, case._SHORT_TIMEOUT)
        if case.send_command(client, "13\r"):
            if not case.wait_for_keyword(client, device, case._LONG_TIMEOUT):
                FAIL_TRIGER = 1

        client_subown = case.start_subown()
        if not client:
            FAIL_TRIGER = 1

        if not case.discover_and_subown(client_subown, pin, 0):
            FAIL_TRIGER = 1

        case.wait_for_keyword(client, case._CLIENT_CHECKER, case._SHORT_TIMEOUT)

        if case.send_command(client_subown, "40\r"):
            case.wait_for_keyword(client_subown, "Enter Device Number", case._INPUT_TIMEOUT)
            case.send_command(client_subown, "1\r")
            if case.wait_for_keyword(client_subown, case._ACL_OK, case._MAIN_TIMEOUT):
                print("ACL provisioning OK")
            else:
                FAIL_TRIGER = 1
                print(case._FAIL + "ACL provisioning FAIL!" + case._ENDC)

        if FAIL_TRIGER:
            print(case._FAIL + "! |Test Case 39 FAILED| !" + case._ENDC)
            return 1
        else:
            print(case._OKGREEN + "! |Test Case 39 passed| !" + case._ENDC)
            return 0
    except:
        print case._FAIL + "! |Test Case 39 FAILED| !" + case._ENDC
        return 1
    finally:
        if client:
            case.stop_instance(client)
        if server_randompin:
            case.stop_instance(server_randompin)
        if client_subown:
            case.stop_instance(client_subown)


# Control which cases to run, display help, amount of written test cases, disable logging
def parse_args(args):
    global DEBUG
    range = ''
    list = ''
    global LOGGING
    try:
        opts, args = getopt.getopt(args, "dhsnr:l:", ["a_range=", "a_list="])
    except getopt.GetoptError:
        print 'usage: system_test.py -r <range> -l <list>\n-s show avilable test cases\n-h show help\n-n disable logging'
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print 'usage: system_test.py -r <range> -l <list>\n-s show avilable test cases\n-h show help\n-n disable logging\n-d enable debug'
            sys.exit()
        elif opt == '-s':
            print '39 Test cases in database'
            sys.exit()
        elif opt == '-n':
            print 'Runing with logging disabled'
            LOGGING = 0
        elif opt == '-d':
            print 'Runing with debug info'
            DEBUG = True
        elif opt in ("-r", "--a_range"):
            range = arg
        elif opt in ("-l", "--a_list"):
            list = arg
    if LOGGING:
        sys.stdout = Logger()
    return range, list

#release is the working dir only,
#TODO support debug and logging
workingDir = 'out/linux/x86_64/release/resource/csdk/security/provisioning/sample'
#clean and build
buildScript = """cd $(git rev-parse --show-toplevel) && rm -rf out && scons -c && scons resource/csdk/security/provisioning/sample -j4
SECURED=1
MULTIPLE_OWNER=1
WITH_TCP=1
WITH_CLOUD=1"""

# Main runer. Will run all defs named caseX where X is consecutive number.
if __name__ == "__main__":
    try:
        if not os.path.exists(workingDir):
            try:
                from subprocess import call
                call(buildScript, shell = True)
            except:
                print "Unexpected error:", sys.exc_info()[0]

        if not os.path.exists(workingDir):
            print "There is no build for testing. exit."
            exit(1)

        os.chdir(workingDir)
        os.environ['LD_LIBRARY_PATH'] = "../../../../.."

        FAIL_LIST = []
        FAIL_COUNT = 0
        EXECUTED = 0
        r, l = parse_args(sys.argv[1:])
        suite01 = Service(01)
        # Will save .dat DB files if no dir named bak exists
        suite01.save_config()
        if r:
            arg_range = r.split("-")
            for x in range(int(arg_range[0]), int(arg_range[1]) + 1, 1):
                res = getattr(system_test, "case" + str(x))(suite01)
                EXECUTED += 1
                FAIL_COUNT += res
                if res == 1:
                    FAIL_LIST.append(x)
        if l:
            arg_list = l.split(",")
            for x in arg_list:
                res = getattr(system_test, "case" + x)(suite01)
                EXECUTED += 1
                FAIL_COUNT += res
                if res == 1:
                    FAIL_LIST.append(x)
        if not r and not l:
            for x in range(1, 40, 1):
                res = getattr(system_test, "case" + str(x))(suite01)
                EXECUTED += 1
                FAIL_COUNT += res
                if res == 1:
                    FAIL_LIST.append(x)
        # Basic statistics
        print '\033[94m' + str(EXECUTED) + " test cases executed" + '\033[0m'
        print '\033[92m' + str(EXECUTED - FAIL_COUNT) + " passed!" + '\033[0m'
        print '\033[91m' + str(FAIL_COUNT) + " failed!" + '\033[0m'
        if FAIL_COUNT > 0:
            print '\033[91m' + "Failed cases:" + '\033[0m'
            print '\033[91m' + "# " + ", ".join([str(x) for x in FAIL_LIST] ) + '\033[0m'
        if LOGGING:
            print "The tests log: %s/%s" % (workingDir, sys.stdout.name)
        # Cleanup
        suite01.restore_config()
    except KeyboardInterrupt:
        print "User called test termination!"
        exit(0)
