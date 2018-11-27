examples/testcase
^^^^^^^^^^^^^^^^^

  This is an example to test working APIs of ST_THINGS on artik053 board. It has unit,functional and regression testcases 
  of ST_THINGS on artik053 board.
  You can select below configurations to test.

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_TESTCASE

  test configurations:

Step1:
in file build/configs/artik053/tc/defconfig, set below configurations

CONFIG_EXAMPLES_TESTCASE_ST_THINGS_STC=y
CONFIG_ST_THINGS=y
CONFIG_ENABLE_IOTIVITY=y
CONFIG_LIB_BOARDCTL=y
#
# IOTIVITY Config Parameters
#
CONFIG_IOTIVITY_RELEASE_VERSION="1.2-rel"
CONFIG_IOTIVITY_ROUTING="EP"
CONFIG_IOTIVITY_PTHREAD_STACKSIZE=8192
CONFIG_IOTIVITY_QUEING_PTHREAD_STACKSIZE=3072
CONFIG_IOTIVITY_RETRANSMIT_PTHREAD_STACKSIZE=1024
CONFIG_IOTIVITY_TCPRECEIVE_PTHREAD_STACKSIZE=8192
CONFIG_IOTIVITY_RECEIVEHANDLER_PTHREAD_STACKSIZE=8192
CONFIG_ENABLE_IOTIVITY_SECURED=y
CONFIG_ENABLE_IOTIVITY_CLOUD=y
CONFIG_IOTIVITY_RELEASE=y
CONFIG_NETUTILS_JSON=y
CONFIG_TIME_EXTENDED=y

Step 2:
cd os : make distclean
cd os/tools : ./configure.sh artik053/tc
cd os : make
cd os : make download ALL

Step 3:
Start the board by pressing reset key
type help on >>TASH
"st_things_stc"  option will be displayed.
type TASH >> st_things_stc
Tests will be executed and results will be displayed.



