#!/usr/local/bin/python3
#
# Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#



# This utility builds enabler debug certificate package:
#       If key certificate exists it should be located before the enabler certificate
#       x509 header containing: version of certificate
#                               serial number
#                               certificate algorithm identifier of the issuer signature
#                               Certificate Issuer name
#                               Validity period
#                               Subject name
#       certificate public key: public key algorithm ID
#                               public key - 3072 bits
#       certificate extensions: ARM certificate header: token, 
#                                                       version, 
#                                                       length, 
#                                                       flags: HBK use, valid LCS, isRma
#                               Barret Tag of public key (Np)
#                               Enabler certificate body: debug mask value - 128 bit
#                                                         debug lock value - 128 bit
#                                                         SHA256 digest of developer public key
#       certificate signature: certificate algorithm identifier of the issuer signature
#                              signature of (x509 header + certificate public key + certificate extensions) - 3072 bits
#


import configparser
import sys

# Definitions for paths
#######################
if sys.platform != "win32" :
    path_div = "//"    
else : #platform = win32
    path_div = "\\"


CURRENT_PATH = sys.path[0]
# In case the scripts were run from current directory
CURRENT_PATH_SCRIPTS = path_div + 'common_utils'
    
# this is the scripts local path, from where the program was called
sys.path.append(CURRENT_PATH+CURRENT_PATH_SCRIPTS)


from cert_cfg_parser_util import *
from global_defines import *
from cert_basic_utilities import *
from x509_util_helper import *

# this is the path of the proj config file
PROJ_CFG_PATH = "src" + path_div
PROJ_CONFIG = CURRENT_PATH + path_div + ".." + path_div + PROJ_CFG_PATH + 'proj.cfg'


# Parse script parameters
def parse_shell_arguments ():
    len_arg =  len(sys.argv)
    if len_arg < 2:
        print_sync("len " + str(len_arg) + " invalid. Usage:" + sys.argv[0] + "<test configuration file>\n")
        for i in range(1,len_arg):
            print_sync("i " + str(i) + " arg " + sys.argv[i] + "\n")
        sys.exit(1)
    config_fname = sys.argv[1]
    if len_arg == 3:
        log_fname = sys.argv[2]
    else:
        log_fname = "sb_dbg1_cert.log"
    return config_fname, log_fname

def exit_main_func(log_file, config_fname, rc):
    log_file.close()
    config_fname.close()
    sys.exit(rc)


def CreateCertUtility(sysArgsList):
    try:          

        config_fname, log_fname = parse_shell_arguments()
        log_file = create_log_file(log_fname)
        print_and_log(log_file, str(datetime.now()) + ": Enabler Debug Certificate Utility started (Logging to " + log_fname + ")\n")
    
        
        data_dict, config = enabler_cert_config_file_parser(config_fname, log_file)
        if data_dict == None:
            log_file.close()
            exit(1) 
            
        DLLHandle = LoadDLLGetHandle()    
            
        if data_dict == None:
            print_and_log(log_file, "**** x509_cert_config_file_parser failed ****\n")
            exit_main_func(log_file, config_fname, result)

        # if key package exists need to insert it into the enabler certificate 
        if data_dict['key_cert_pkg'] != "":
            keyStr, keyStrSize = parse_bin_file(data_dict['key_cert_pkg'])
        else:
            keyStr = None
            keyStrSize = 0

        print_and_log(log_file, "**** calling SBU_x509_CreateAndSetHeader **** \n") 
        result = DLLHandle.SBU_x509_CreateAndSetHeader(int(CC_X509_CERT_TYPE_ENABLER_DBG)) 
        if result != 0:
            print_and_log(log_file, "**** SBU_x509_CreateAndSetHeader failed ****\n")
            exit_main_func(log_file, config_fname, result)
    
        print_and_log(log_file, "**** calling SBU_x509_AddPropHeaderExtension **** \n") 
        propHeader = build_certificate_header(DEBUG_ENABLER_TOKEN, PrjDefines, LIST_OF_CONF_PARAMS, data_dict['rma_mode'], data_dict['hbk_id'], data_dict['lcs'])
        result = DLLHandle.SBU_x509_AddPropHeaderExtension(propHeader.encode('iso-8859-1')) 
        if result != 0:
            print_and_log(log_file, "**** SBU_x509_AddPropHeaderExtension failed ****\n")
            exit_main_func(log_file, config_fname, result)
    
        print_and_log(log_file, "**** calling SBU_x509_AddPubKeyNpBuffExtension **** \n") 
        result = DLLHandle.SBU_x509_AddPubKeyNpBuffExtension(str.encode(data_dict["cert_keypair"]), str.encode(data_dict["cert_keypair_pwd"])) 
        if result != 0:
            print_and_log(log_file, "**** SBU_x509_AddPubKeyNpBuffExtension failed ****\n")
            exit_main_func(log_file, config_fname, result)

        debugStr = str()
        debugStr = byte2string(struct.pack('<I', data_dict['debug_mask0']))
        debugStr = debugStr + byte2string(struct.pack('<I', data_dict['debug_mask1']))
        debugStr = debugStr + byte2string(struct.pack('<I', data_dict['debug_mask2']))
        debugStr = debugStr + byte2string(struct.pack('<I', data_dict['debug_mask3']))
    
        debugStr1 = str()
        debugStr1 = byte2string(struct.pack('<I', data_dict['debug_lock0']))
        debugStr1 = debugStr1 + byte2string(struct.pack('<I', data_dict['debug_lock1']))
        debugStr1 = debugStr1 + byte2string(struct.pack('<I', data_dict['debug_lock2']))
        debugStr1 = debugStr1 + byte2string(struct.pack('<I', data_dict['debug_lock3']))

        print_and_log(log_file, "**** calling SBU_x509_AddEnablerCertBodyExtension **** \n") 
        result = DLLHandle.SBU_x509_AddEnablerCertBodyExtension(debugStr.encode('iso-8859-1'), debugStr1.encode('iso-8859-1'), str.encode(data_dict["next_cert_pubkey"]))
        if result != 0:
            print_and_log(log_file, "**** SBU_x509_AddDebugMaskExtension failed ****\n")
            exit_main_func(log_file, config_fname, result)
        
        print_and_log(log_file, "**** calling SBU_x509_SetKeyAndSign **** \n") 
        result = DLLHandle.SBU_x509_SetKeyAndSign(str.encode(data_dict["cert_keypair"]), str.encode(data_dict["cert_keypair_pwd"]))
        if result != 0:
            print_and_log(log_file, "**** SBU_x509_SetKeyAndSign failed ****\n")
            exit_main_func(log_file, config_fname, result)
 
        print_and_log(log_file, "**** calling SBU_x509_BuildCertPkg **** \n") 
        if keyStr == None:
            result = DLLHandle.SBU_x509_BuildCertPkg(0, 0, 1, data_dict["cert_pkg"])
        else:
            result = DLLHandle.SBU_x509_BuildCertPkg(byref(keyStr), keyStrSize, 1, data_dict["cert_pkg"])
        if result != 0:
            print_and_log(log_file, "**** SBU_x509_BuildCertPkg failed ****\n")
            exit_main_func(log_file, config_fname, result)
    
        print_and_log(log_file, "\n**** Certificate file creation has been completed successfully ****")

    except IOError as Error8: 
        (errno, strerror) = Error8.args 
        print_and_log(log_file, "I/O error(%s): %s" % (errno, strerror))
        raise
    except NameError:
        print_and_log(log_file, "Unexpected error, exiting program")
        raise  # Debug info
    except ValueError:
        print_and_log(log_file, "Illegal variable type")
        raise # Debug info


##################################
#       Main function
##################################
        
if __name__ == "__main__":

    import sys
    if sys.version_info<(3,0,0):
        print("You need python 3.0 or later to run this script")
        exit(1)

    if "-cfg_file" in sys.argv:
        PROJ_CONFIG = sys.argv[sys.argv.index("-cfg_file") + 1]
    print("Config File  - %s\n" %PROJ_CONFIG)

    # Get the project configuration values
    PrjDefines = parseConfFile(PROJ_CONFIG,LIST_OF_CONF_PARAMS)
    
    CreateCertUtility(sys.argv)




    



######################################## END OF FILE ########################################

