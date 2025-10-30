#-------------------------------------------------------------------------------
# Copyright (c) 2001-2019, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

#!/usr/bin/env bash
cd ../../../../bin/

cp ../tester/util_assisted/get_pub_key/get_pub_key.sh .
cp ../tester/cc3x_secure_debug_test/dat/dev_keypair1_3k.pem ./dev_keypair1.pem
cp ../tester/cc3x_secure_debug_test/dat/oem_keypair1_3k.pem ./oem_keypair1.pem
cp ../src/cc3x_boot_cert/examples/enabler_cert/sb_enabler_dbg_cert_no_pwd.cfg .
cp ../src/cc3x_boot_cert/examples/enabler_cert/sb_enabler_dbg_cert_rma_no_pwd.cfg .


echo
echo " Gen: developer public key... "
echo "================================================"
./get_pub_key.sh dev_keypair1.pem pwd.txt dev_pubkey1.pem

echo
echo " Run: generate enabler debug certificate "
echo "================================================"
./cert_dbg_enabler_util.py sb_enabler_dbg_cert_no_pwd.cfg

echo
echo " Run: generate enabler RMA certificate "
echo "================================================"
./cert_dbg_enabler_util.py sb_enabler_dbg_cert_rma_no_pwd.cfg

