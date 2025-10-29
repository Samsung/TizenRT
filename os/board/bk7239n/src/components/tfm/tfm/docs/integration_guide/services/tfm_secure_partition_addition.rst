#######################
Adding Secure Partition
#######################

***********************
Terms and abbreviations
***********************
This document uses the following terms and abbreviations.

.. table:: term table
   :widths: auto

   ================== ==================================
    **Term**          **Meaning**
   ================== ==================================
   FF-M               Firmware Framework for M
   ID                 Identifier
   IPC                Interprocess communication
   IPC model          The secure IPC framework
   irqs               Interrupt requests
   Library model      The secure function call framework
   MMIO               Memory Mapped I/O
   PSA                Platform Security Architecture
   RoT                Root of Trust
   SFN                Secure Function
   SFN model          Secure Function model
   SID                RoT Service ID
   SP                 Secure Partition
   SPM                Secure Partition Manager
   TF-M               Trusted firmware M
   ================== ==================================

************
Introduction
************
Secure Partition is an execution environment that provides the following
functions to Root of Trust (RoT) Services:

- Access to resources, protection of its own code and data.
- Mechanisms to interact with other components in the system.

Each Secure Partition is a single thread of execution and the smallest unit of
isolation.

This document mainly describes how to add a secure partition in TF-M and
focuses on the configuration, manifest, implement rules. The actual
source-level implementation is not included in this document.

.. Note::
   If not otherwise specified, the steps are identical for Library, IPC and SFN
   model.

   The IPC and SFN model conforms to the *PSA Firmware Framework for M (FF-M) v
   1.1* changes. Refer to `PSA Firmware Framework specification`_ and
   `Firmware Framework for M 1.1 Extensions`_ for details.

*******
Process
*******
The main steps to add a secure partition are as follows:

- `Add source folder`_
- `Add manifest`_
- `Add configuration`_
- `Implement the RoT services`_

Add source folder
=================
Add a source folder under ``<TF-M base folder>/secure_fw/partitions`` for the
new secure partition (Let's take ``example`` as the folder name):

This folder should include those parts:

- Manifest file
- CMake configuration files
- Source code files

Add manifest
============
Each Secure Partition must have resource requirements declared in a manifest
file. The Secure Partition Manager (SPM) uses the manifest file to assemble and
allocate resources within the SPE. The manifest includes the following:

- A Secure Partition name.
- A list of implemented RoT Services.
- Access to other RoT Services.
- Memory requirements.
- Scheduling hints.
- Peripheral memory-mapped I/O regions and interrupts.

.. Note::
   The current manifest format in TF-M is "yaml" which is different from the
   requirement of PSA FF.

Here is a manifest reference example for the IPC model:

.. Note::
    To use SFN model, the user needs to replace ``"model": "IPC"`` to
    ``"model": "SFN"``. The user also needs to remove the attribute
    ``"entry_point"``, and optionally replace it with ``"entry_init"``.

.. code-block:: yaml

  {
    "psa_framework_version": 1.1,
    "name": "TFM_SP_EXAMPLE",
    "type": "APPLICATION-ROT",
    "priority": "NORMAL",
    "model": "IPC",
    "entry_point": "tfm_example_main",
    "stack_size": "0x0200",
    "services" : [
      {
        "name": "ROT_A",
        "sid": "0x000000E0",
        "non_secure_clients": true,
        "connection_based": true,
        "version": 1,
        "version_policy": "STRICT"
        "mm_iovec": "disable"
      }
    ],
    "mmio_regions": [
      {
        "name": "TFM_PERIPHERAL_A",
        "permission": "READ-WRITE"
      }
    ],
    "irqs": [
      {
        "source": "TFM_A_IRQ",
        "name": "A_IRQ",
        "handling": "SLIH"
      }
    ]
    "dependencies": [
      "TFM_CRYPTO",
      "TFM_INTERNAL_TRUSTED_STORAGE_SERVICE"
    ]
  }

Secure Partition ID Distribution
--------------------------------
Every Secure Partition has an identifier (ID). TF-M will generate a header file
that includes definitions of the Secure Partition IDs. The header file is
``<TF-M build folder>generated/interface/include/psa_manifest/pid.h``. Each
definition uses the ``name`` attribute in the manifest as its name and the
value is allocated by SPM.

The Partition ID can be set to a fixed value or omitted to be auto allocated.

.. code-block:: c

   #define name id-value

.. table:: PID table
   :widths: auto

   ==================================== ======================
   **Secure Partitions**                **PID Range**
   ==================================== ======================
   TF-M Internal Partitions             0 - 255
   PSA and user Partitions              256 - 2999
   TF-M test Partitions                 3000 - 4999
   Firmware Framework test Partitions   5000 - 5999
   Reserved                             6000 -
   ==================================== ======================

Please refer to ``<TF-M base folder>/tools/tfm_manifest_list.yaml``,
``<TF-M test repo>/test/secure_fw/tfm_test_manifest_list.yaml`` and
``<TF-M base folder>/tools/tfm_psa_ff_test_manifest_list.yaml`` for the detailed
PID allocations.

About where to add the definition, please refer to the chapter `Add
configuration`_.

RoT Service ID (SID) Distribution
---------------------------------
An RoT Service is identified by its RoT Service ID (SID). A SID is a 32-bit
number that is associated with a symbolic name in the Secure Partition
manifest. The bits [31:12] uniquely identify the vendor of the RoT Service.
The remaining bits [11:0] can be used at the discretion of the vendor.

Here is the RoT Service ID table used in TF-M.

.. table:: SID table
   :widths: auto

   =========================== ====================== ========================
   **Partitions**              **Vendor ID(20 bits)** **Function ID(12 bits)**
   =========================== ====================== ========================
   audit_logging               0x00000                0x000-0x01F
   initial_attestation         0x00000                0x020-0x03F
   platform                    0x00000                0x040-0x05F
   protected_storage           0x00000                0x060-0x06F
   internal_trusted_storage    0x00000                0x070-0x07F
   crypto                      0x00000                0x080-0x09F
   firmware_update             0x00000                0x0A0-0x0BF
   tfm_secure_client           0x0000F                0x000-0x01F
   core_test                   0x0000F                0x020-0x03F
   core_test_2                 0x0000F                0x040-0x05F
   tfm_ipc_client              0x0000F                0x060-0x07F
   tfm_ipc_service             0x0000F                0x080-0x09F
   tfm_slih_test_service       0x0000F                0x0A0-0x0AF
   tfm_flih_test_service       0x0000F                0x0B0-0x0BF
   tfm_ps_test_service         0x0000F                0x0C0-0x0DF
   tfm_secure_client_2         0x0000F                0x0E0-0x0FF
   tfm_sfn_test_service_1      0x0000F                0x100-0x11F
   tfm_sfn_test_service_2      0x0000F                0x120-0x13F
   tfm_attest_test_service     0x0000F                0x140-0x15F
   =========================== ====================== ========================

RoT Service Stateless Handle Distribution
-----------------------------------------
A Secure partition may include stateless services. They are distinguished and
referenced by stateless handles. In manifest, a ``stateless_handle`` attribute
is set for indexing stateless services. It must be either ``"auto"`` or a
number in the range [1, 32] in current implementation and may extend. Also the
``connection-based`` attribute must be set to ``false`` for stateless services.

Here is the stateless handle allocation for partitions in TF-M. Partitions not
listed are not applied to stateless mechanism yet.

.. table:: Stateless Handle table
   :widths: auto

   =============================== =======================
    **Partition name**              **Stateless Handle**
   =============================== =======================
   TFM_SP_CRYPTO                   1
   TFM_SP_PS                       2
   TFM_SP_ITS                      3
   TFM_SP_INITIAL_ATTESTATION      4
   =============================== =======================

mmio_regions
------------
This attribute is a list of MMIO region objects which the Secure Partition
needs access to. TF-M only supports the ``named_region`` current. Please refer
to PSA FF for more details about it. The user needs to provide a name macro to
indicate the variable of the memory region.

TF-M uses the below structure to indicate a peripheral memory.

.. code-block:: c

  struct platform_data_t {
    uint32_t periph_start;
    uint32_t periph_limit;
    int16_t periph_ppc_bank;
    int16_t periph_ppc_loc;
  };

.. Note::
   This structure is not expected by TF-M, it's only that the current
   implementations are using. Other peripherals that need different information
   to create isolation need to define a different structure with the same name.

Here is an example for it:

.. code-block:: c

   struct platform_data_t tfm_peripheral_A;
   #define TFM_PERIPHERAL_A                 (&tfm_peripheral_A)


Library model support
---------------------
For the library model, the user needs to add a ``secure_functions`` item. The
main difference between ``secure_function`` and ``services`` is the extra
``signal`` key for secure function entry. This is not required in FF-M v1.0.

The ``signal`` must be the upper case of the secure function name.

.. code-block:: yaml

  "secure_functions": [
    {
      "name": "TFM_EXAMPLE_A",
      "signal": "EXAMPLE_A_FUNC",
      "sid": "0x00000000",
      "non_secure_clients": true,
      "version": 1,
      "version_policy": "STRICT"
    },

Add configuration
=================
The following configuration tasks are required for the newly added secure
partition:

Add CMake configure files
-------------------------
- ``<TF-M base folder>/secure_fw/partitions/example/CMakeLists.txt``, which is
  the compilation configuration for this module. Add library
  ``tfm_app_rot_partition_example`` and associated source files.

Here is a reference example for `CMakeLists.txt`_

.. _CMakeLists.txt: https://git.trustedfirmware.org/TF-M/tf-m-tools.git/tree/
  example_partition/CMakeLists.txt

.. Note::
   The secure partition must be built as a standalone static library, and the
   name of the library must follow this pattern, as it affects how the linker
   script will lay the partition in memory:
   - ``tfm_psa_rot_partition*`` in case of a PSA RoT partition
   - ``tfm_app_rot_partition*`` in case of an Application RoT partition

The current CMake configuration should also be updated, by updating
``<TF-M base folder>/config/config_default.cmake``
to include the CMake configuration variable of the newly added Secure
Partition, e.g, TFM_PARTITION_EXAMPLE and adding the relevant
subdirectory in ``<TF-M base folder>/secure_fw/CMakeLists.txt``, e.g.
``add_subdirectory(partitions/example)``.
Please refer to the source code of TF-M for more detail.

Update manifest list
--------------------
The ``<TF-M base folder>/tools/tfm_manifest_list.yaml`` is used to collect
necessary information of secure partition.
The manifest tool ``tools/tfm_parse_manifest_list.py`` processes it and
generates necessary files while building.

- ``name``: The name string of the secure partition.
- ``short_name``: should be the same as the ``name`` in the secure partition
  manifest file.
- ``manifest``: the relative path of the manifest file to TF-M root.
  In out-of-tree secure partition, ``manifest`` can be an absolute path or the
  relative path to the current manifest list file.
- ``conditional``: Optional configuration to enable or disable this partition.
  If it is not set, the Secure Partition is always enabled.
  The value of this attribute must be a CMake variable surrounded by ``@``.
  The value of the CMake variable must be:

  - ``ON``, ``TRUE`` or ``ENABLED`` - the Partition is enabled.
  - ``OFF``, ``FALSE`` or ``DISABLED`` - the Partition is disabled.
  - unset - the Partition is disabled.

  The build system relies on the CMake command ``configure_file()`` to replace
  the CMake variables with the corresponding values before the manifest tool
  processes it.
  If you are using the manifest tool out of the CMake build system, you can also
  set this attribute to the values allowed above to make the tool work.

- ``version_major``: major version the partition manifest.
- ``version_minor``: minor version the partition manifest.
- ``pid``: Secure Partition ID value distributed in chapter `Secure Partition
  ID Distribution`_.
- ``output_dir``: Optional path to hold the generated files.
  The files are generated to:

  - ``<build_dir>/generated/<output_dir>``, if ``output_dir`` is relative path.
  - ``<output_dir>``, if ``output_dir`` is absolute path.
  - ``<build_dir>/generated/``, if ``output_dir`` is not set.
- ``linker_pattern``: contains the information for linker to place the symbols
  of the Secure Partition. The following patterns are supported:

    - ``library_list`` - the library defined by CMake in `Add configuration`_
      section. It must be ``*tfm_*partition_<name>.*``, the ``<name>>`` must
      match the CMake library name.

    - ``object_list`` - Any object files containing symbols belonging to the
      Secure Partition but are not included in the Secure Partitions library.

Reference configuration example:

.. code-block:: yaml

    {
      "name": "TFM Example Service",
      "short_name": "TFM_SP_EXAMPLE",
      "manifest": "secure_fw/partitions/example/tfm_example_partition.yaml",
      "conditional": "@TFM_PARTITION_EXAMPLE@",
      "output_path": "partitions/example",
      "version_major": 0,
      "version_minor": 1,
      "pid": 290,
      "linker_pattern": {
        "library_list": [
          "*tfm_*partition_example*"
         ]
      }
    }

TF-M also supports out-of-tree Secure Partition build where you can have your
own manifest lists.
Please refer to `Out-of-tree Secure Partition build`_ for details.

Implement the RoT services
==========================
To implement RoT services, the partition needs a source file which contains the
implementations of the services, as well as the partition entry point. The user
can create this source file under
``<TF-M base folder>/secure_fw/partitions/example/tfm_example_partition.c``.

As an example, the RoT service with SID **ROT_A** will be implemented.

Entry point for IPC Model Partitions
------------------------------------
This function must have a loop that repeatedly waits for input signals and
then processes them, following the Secure Partition initialization.

.. code-block:: c

    #include "psa_manifest/tfm_example.h"
    #include "psa/service.h"

    void tfm_example_main(void)
    {
        psa_signal_t signals = 0;

        /* Secure Partition initialization */
        example_init();

        /*
         * Continually wait for one or more of the partition's RoT Service or
         * interrupt signals to be asserted and then handle the asserted
         * signal(s).
         */
        while (1) {
            signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
            if (signals & ROT_A_SIGNAL) {
                rot_A();
            } else {
                /* Should not come here */
                psa_panic();
            }
        }
    }

Entry init for SFN Model Partitions
-----------------------------------
In the SFN model, the Secure Partition consists of one optional initialization
function, which is declared as the ``entry_init`` symbol as mentioned in
section `Add manifest`_. After initialization, the entry_init function
returns the following values:

    - Return ``PSA_SUCCESS`` if initialization succeeds.

    - Return ``PSA_SUCCESS`` if initialization is partially successful,
      and you want some SFNs to receive messages. RoT Services that are
      non-operational must respond to connection requests with
      ``PSA_ERROR_CONNECTION_REFUSED``.

    - Return an error status if the initialization failed, and no SFNs
      within the Secure Partition must be called.

Service implementation for IPC Model
------------------------------------
The service is implemented by the ``rot_A()`` function, which is called upon an
incoming signal. This implementation is up to the user, however an example
service has been included for reference. The following example sends a message
"Hello World" when called.

.. code-block:: c

    #include "psa_manifest/tfm_example.h"
    #include "psa/service.h"

    static void rot_A(void)
    {
        const int BUFFER_LEN = 32;
        psa_msg_t msg;
        int i;
        uint8_t rec_buf[BUFFER_LEN];
        uint8_t send_buf[BUFFER_LEN] = "Hello World";

        psa_get(ROT_A_SIGNAL, &msg);
        switch (msg.type) {
        case PSA_IPC_CONNECT:
        case PSA_IPC_DISCONNECT:
            /*
             * This service does not require any setup or teardown on connect
             * or disconnect, so just reply with success.
             */
            psa_reply(msg.handle, PSA_SUCCESS);
            break;
        case PSA_IPC_CALL:
            for (i = 0; i < PSA_MAX_IOVEC; i++) {
                if (msg.in_size[i] != 0) {
                    psa_read(msg.handle, i, rec_buf, BUFFER_LEN);
                }
                if (msg.out_size[i] != 0) {
                    psa_write(msg.handle, i, send_buf, BUFFER_LEN);
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
            break;
        default:
            /* cannot get here [broken SPM] */
            psa_panic();
            break;
        }
    }

Service implementation for SFN Model
------------------------------------
SFN model consists of a set of Secure Functions (SFN), one for each RoT
Service. The connection, disconnection and request messages do not cause a
Secure Partition signal to be asserted for SFN Secure Partitions. Instead,
the Secure Function (SFN) for the RoT Service is invoked by the framework,
with the message details provided as a parameter to the SFN. To add a secure
function (SFN) to process messages for each RoT Service, each SFN will have
following prototype.

.. code-block:: c

  psa_status_t <<name>>_sfn(const psa_msg_t *msg);

A connection-based example service has been included for reference which
sends a message "Hello World" when called.

.. code-block:: c

    #include "psa_manifest/tfm_example.h"
    #include "psa/service.h"

    psa_status_t rot_a_sfn(const psa_msg_t *msg)
    {
        const int BUFFER_LEN = 32;
        int i;
        uint8_t rec_buf[BUFFER_LEN];
        uint8_t send_buf[BUFFER_LEN] = "Hello World";

        switch (msg->type) {
        case PSA_IPC_CONNECT:
        case PSA_IPC_DISCONNECT:
            /*
             * This service does not require any setup or teardown on connect
             * or disconnect, so just reply with success.
             */
            return PSA_SUCCESS;
        case PSA_IPC_CALL:
            for (i = 0; i < PSA_MAX_IOVEC; i++) {
                if (msg->in_size[i] != 0) {
                    psa_read(msg->handle, i, rec_buf, BUFFER_LEN);
                }
                if (msg.->out_size[i] != 0) {
                    psa_write(msg->handle, i, send_buf, BUFFER_LEN);
                }
            }
            return PSA_SUCCESS;
        default:
            /* cannot get here [broken SPM] */
            return PSA_ERROR_PROGRAMMER_ERROR;
        }
    }

Test connection
---------------
To test that the service has been implemented correctly, the user needs to call
it from somewhere. One option is to create a new testsuite, such as
``<TF-M-test base folder>/test/secure_fw/suites/example/non_secure/example_ns_
interface_testsuite.c``.

The process of adding test connection is explained in the specification
:doc:`Adding TF-M Regression Test Suite </integration_guide/tfm_test_suites_addition>`

.. code-block:: c

    #include "psa_manifest/sid.h"
    #include "psa/client.h"

    #include "test_framework.h"
    #include "test_log.h"

    static void tfm_example_test_1001(struct test_result_t *ret)
    {
        char str1[] = "str1";
        char str2[] = "str2";
        char str3[128], str4[128];
        struct psa_invec invecs[2] = {{str1, sizeof(str1)},
                                      {str2, sizeof(str2)}};
        struct psa_outvec outvecs[2] = {{str3, sizeof(str3)},
                                        {str4, sizeof(str4)}};
        psa_handle_t handle;
        psa_status_t status;
        uint32_t version;

        version = psa_version(ROT_A_SID);
        TEST_LOG("TFM service support version is %d.\r\n", version);
        handle = psa_connect(ROT_A_SID, ROT_A_VERSION);
        status = psa_call(handle, PSA_IPC_CALL, invecs, 2, outvecs, 2);
        if (status >= 0) {
            TEST_LOG("psa_call is successful!\r\n");
        } else {
            TEST_FAIL("psa_call is failed!\r\n");
            return;
        }

        TEST_LOG("outvec1 is: %s\r\n", outvecs[0].base);
        TEST_LOG("outvec2 is: %s\r\n", outvecs[1].base);
        psa_close(handle);
        ret->val = TEST_PASSED;
    }

Once the test and service has been implemented, the project can be built and
executed. The user should see the "Hello World" message in the console as
received by the testsuite.

Out-of-tree Secure Partition build
----------------------------------

TF-M supports out-of-tree Secure Partition build, whose source code folders
are maintained outside TF-M repo. Developers can configure
``TFM_EXTRA_MANIFEST_LIST_FILES`` and ``TFM_EXTRA_PARTITION_PATHS`` in build
command line to include out-of-tree Secure Partitions.

- ``TFM_EXTRA_MANIFEST_LIST_FILES``

  A list of the absolute path(s) of the manifest list(s) provided by out-of-tree
  Secure Partition(s).
  Use semicolons ``;`` to separate multiple manifest lists. Wrap the multiple
  manifest lists with double quotes.

- ``TFM_EXTRA_PARTITION_PATHS``

  A list of the absolute directories of the out-of-tree Secure Partition source
  code folder(s). TF-M build system searches ``CMakeLists.txt`` of partitions in
  the source code folder(s).
  Use semicolons ``;`` to separate multiple out-of-tree Secure Partition
  directories. Wrap the multiple directories with double quotes.

A single out-of-tree Secure Partition folder can be organized as the figure
below.

::

  secure partition folder
        ├── CMakeLists.txt
        ├── manifest_list.yaml
        ├── out_of_tree_partition_manifest.yaml
        └── source code

In the example above, ``TFM_EXTRA_MANIFEST_LIST_FILES`` and
``TFM_EXTRA_PARTITION_PATHS`` in the build command can be configured as listed
below.

.. code-block:: bash

  -DTFM_EXTRA_MANIFEST_LIST_FILES=<Absolute-path-sp-folder/manifest_list.yaml>
  -DTFM_EXTRA_PARTITION_PATHS=<Absolute-path-sp-folder>

Multiple out-of-tree Secure Partitions can be organized in diverse structures.
For example, multiple Secure Partitions can be maintained under the same
directory as shown below.

::

  top-level folder
        ├── Partition 1
        │       ├── CMakeLists.txt
        │       ├── partition_1_manifest.yaml
        │       └── source code
        ├── Partition 2
        │       └── ...
        ├── Partition 3
        │       └── ...
        ├── manifest_list.yaml
        └── Root CMakeLists.txt

In the example above, a root CMakeLists.txt includes all the partitions'
CMakLists.txt, for example via ``add_subdirectory()``. The manifest_list.yaml
lists all partitions' manifest files.
``TFM_EXTRA_MANIFEST_LIST_FILES`` and ``TFM_EXTRA_PARTITION_PATHS`` in build
command line can be configured as listed below.

.. code-block:: bash

  -DTFM_EXTRA_MANIFEST_LIST_FILES=<Absolute-path-top-level-folder/manifest_list.yaml>
  -DTFM_EXTRA_PARTITION_PATHS=<Absolute-path-top-level-folder>

Alternatively, out-of-tree Secure Partitions can be separated in different
folders.

::

    partition 1 folder                    partition 2 folder
        ├── CMakeLists.txt                    ├── CMakeLists.txt
        ├── manifest_list.yaml                ├── manifest_list.yaml
        ├── partition_1_manifest.yaml         ├── partition_2_manifest.yaml
        └── source code                       └── source code

In the example above, each Secure Partition manages its own manifest files and
CMakeLists.txt. ``TFM_EXTRA_MANIFEST_LIST_FILES`` and
``TFM_EXTRA_PARTITION_PATHS`` in build command line can be configured as listed
below. Please note those input shall be wrapped with double quotes.

.. code-block:: bash

  -DTFM_EXTRA_MANIFEST_LIST_FILES="<Absolute-path-part-1-folder/manifes_list.yaml>;<Absolute-path-part-2-folder/manifes_list.yaml>"
  -DTFM_EXTRA_PARTITION_PATHS="<Absolute-path-part-1-folder>;<Absolute-path-part-2-folder>"

.. Note::

   Manifest list paths in ``TFM_EXTRA_MANIFEST_LIST_FILES`` do NOT have to be
   one-to-one mapping to Secure Partition directories in
   ``TFM_EXTRA_PARTITION_PATHS``. The orders don't matter either.

Further Notes
-------------

- In the IPC model, Use PSA FF proposed memory accessing mechanism. SPM
  provides APIs and checking between isolation boundaries, a free accessing
  of memory can cause program panic.
- In the IPC model, the memory checking inside partition runtime is
  unnecessary. SPM handles the checking while memory accessing APIs are
  called.
- In the IPC model, the client ID had been included in the message structure
  and secure partition can get it when calling psa_get() function. The secure
  partition does not need to call ``tfm_core_get_caller_client_id()`` to get
  the caller client ID anymore.
- In the IPC model, SPM will check the security policy and partition
  dependence between client and service. So the service does not need to
  validate the secure caller anymore.

*********
Reference
*********

| `PSA Firmware Framework specification`_
| `Firmware Framework for M 1.1 Extensions`_

.. _PSA Firmware Framework specification:
  https://www.arm.com/architecture/security-features/platform-security

.. _Firmware Framework for M 1.1 Extensions: https://developer.arm.com/
  documentation/aes0039/latest

--------------

*Copyright (c) 2019-2022, Arm Limited. All rights reserved.*
