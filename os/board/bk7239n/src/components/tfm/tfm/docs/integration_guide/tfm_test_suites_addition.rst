#################################
Adding TF-M Regression Test Suite
#################################

.. contents:: Table of Contents

*************************************
Introduction of TF-M regression tests
*************************************

TF-M regression tests test whether changes to TF-M code work as expected.
This documentation focuses on three parts in TF-M regression tests:

1. ``test suite``: A series of tests of a certain function.
2. ``test case``: A specific test instance in test suites.
3. ``test service``: An extra module works like secure partitions to support
   related test suites.

Source structure
================

TF-M tests source code are located in
`tf-m-tests <https://git.trustedfirmware.org/TF-M/tf-m-tests.git/>`__.

+---------------------------------------+---------------------------------------------------------------+
| Folder name                           | Description                                                   |
+=======================================+===============================================================+
| test/config                           | The CMAKE test configurations files.                          |
+---------------------------------------+---------------------------------------------------------------+
| test/framework                        | Source code for test framework code, managing test suites.    |
+---------------------------------------+---------------------------------------------------------------+
| test/secure_fw/suites                 | Test suites divided into subdirectories.                      |
+---------------------------------------+---------------------------------------------------------------+
| test/secure_fw/suites/<suite>/service | Test service divided into corresponding suite subdirectories. |
+---------------------------------------+---------------------------------------------------------------+
| test/secure_fw/common_test_services   | Common test services.                                         |
+---------------------------------------+---------------------------------------------------------------+

Test configuration
==================

A test configuration controls whether one or multiple test suites are enabled.
The doc :doc:`TF-M Build Instructions </technical_references/instructions/tfm_build_instruction>`
shows some test configurations which are already supported in current TF-M.
An example usage of test configuration shows below.

.. code-block:: bash

    cmake -S . -B cmake_build -DTFM_PLATFORM=arm/mps2/an521               \
                              -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake \
                              -DCMAKE_BUILD_TYPE=Release                  \
                              -DTEST_NS_ATTESTATION=ON

With this command, only non-secure initial attestation test suite will be built.

***********************
Adding a new test suite
***********************

This section introduces how to add a new test suite and control its compilation
with a test configuration in ``tf-m-tests`` repository.

Source code
===========

The test suite example subdirectory named ``<test_name>`` is located under the
path ``tf-m-tests/test/secure_fw/suites``. If the new test suite includes both
non-secure and secure parts, the source code shall be divided shared code and
specific code. An example test suite folder can be organized as the figure
below.

.. code-block:: bash

    .
    ├── CMakeLists.txt
    ├── <test_name>_tests_common.c
    ├── non_secure
    │   ├── <test_name>_ns_interface_testsuite.c
    │   └── <test_name>_ns_tests.h
    └── secure
        ├── <test_name>_s_interface_testsuite.c
        └── <test_name>_s_tests.h

Creating test configurations
============================

Developers shall assign corresponding test configurations to control the test
suites.

Naming test configurations
--------------------------

The test configurations of example test suites are ``TEST_NS_<TEST_NAME>``
in non-secure and ``TEST_S_<TEST_NAME>`` in secure.

.. Note::
    The test configurations must be named with the prefixes ``TEST_S_`` and
    ``TEST_NS_``, for secure regression tests and non-secure regression tests
    respectively. Otherwise, tf-m-tests build system may not recognize it.

Setting test configurations
---------------------------

When the test configurations have dependences, the default value need to be set.
The setting is performed in following four steps.

#. Command line input: The configuration can be enabled or disabled by the
   command ``-DTEST_NS_<TEST_NAME>=ON/OFF -DTEST_S_<TEST_NAME>=ON/OFF``, and
   the value cannot be changed throughout the whole compilation of TF-M tests.

#. ``tf-m-tests/config/set_config.cmake``: The test configurations shall be
   OFF if its dependences are not supported. The dependences are probably
   from:

    #. TF-M partitions configurations like ``TFM_PARTITION_CRYPTO``,
       ``TFM_PARTITION_INITIAL_ATTESTATION``, etc.
    #. TF-M build mode configuration like ``TFM_LIB_MODEL``.
    #. TF-M other configurations like ``TFM_PARTITION_FIRMWARE_UPDATE``,
       ``FORWARD_PROT_MSG``, etc.

#. ``tf-m-tests/config/default_ns_test_config.cmake`` or
   ``tf-m-tests/config/default_s_test_config.cmake``: It is required to give
   the default value of the new test configuration in these two files when
   ``TEST_NS`` or ``TEST_S`` is ON. The recommended value is ON unless the
   single test's code or data size is very large.

#. ``tf-m-tests/config/default_test_config.cmake``: It is required to give the
   default value of the new test configuration in the file when both
   ``TEST_NS`` and ``TEST_S`` are OFF. The default value must be OFF.

.. Note::
   The test configurations must be set as CACHE value in CMAKE files. The CACHE
   set cannot replace the value from command line, see
   `Set Cache Entry <https://cmake.org/cmake/help/latest/command/set.html#set-cache-entry>`__.

Checking test configurations
----------------------------

The new test configurations must be checked by function ``tfm_invalid_config()``
if they have any dependence. The value comes from command line may be wrong when
the dependences are conflicting. In addition to the dependences quoted in
``tf-m-tests/config/set_config.cmake``, some other test configurations may be
necessary.

Applicating test configurations
===============================

The mission of test configurations is to control the build. They are applied
in ``test/secure_fw/suites/<test_name>/CMakeLists.txt`` like the example below.

.. code-block:: cmake

    cmake_policy(SET CMP0079 NEW)

    if (NOT TEST_NS_<TEST_NAME> AND NOT TEST_S_<TEST_NAME>)
        return()
    endif()

    ####################### Non Secure #########################################

    if (TEST_NS_<TEST_NAME>)
        add_library(tfm_test_suite_<test_name>_ns STATIC EXCLUDE_FROM_ALL)
        # target_sources()
        # target_include_directories()
        target_compile_definitions(tfm_test_suite_<test_name>_ns
            INTERFACE
                TEST_NS_<TEST_NAME>
        )
        # target_link_libraries()
    endif()

    ####################### Secure #############################################

    if (TEST_S_<TEST_NAME>)
        add_library(tfm_test_suite_<test_name>_s STATIC EXCLUDE_FROM_ALL)
        # target_sources()
        # target_include_directories()
        target_compile_definitions(tfm_test_suite_<test_name>_s
            INTERFACE
                TEST_S_<TEST_NAME>
        )
        # target_link_libraries()
    endif()

The function ``target_compile_definitions`` will export the macros
``TEST_NS_<TEST_NAME>`` or ``TEST_S_<TEST_NAME>`` into source code. and in the
file ``tf-m-tests/framework/non_secure_suites.c`` or
``tests/framework/secure_suites.c``, the definitions of these macros will be
checked, and then the head file will be included and test cases will be
registered if the macro is defined.

.. code-block:: c

    #ifdef TEST_NS_<TEST_NAME>
    #include "<test_name>_ns_tests.h"
    #endif

    static struct test_suite_t test_suites[] = {
    /* Non-secure example test cases */
        // ......
    #ifdef TEST_NS_<TEST_NAME>
        {&register_testsuite_ns_<test_name>_interface, 0, 0, 0},
    #endif
    };

.. code-block:: c

    #ifdef TEST_S_<TEST_NAME>
    #include "<test_name>_s_tests.h"
    #endif

    static struct test_suite_t test_suites[] = {
    /* Secure example test cases */
        // ......
    #ifdef TEST_S_<TEST_NAME>
        {&register_testsuite_s_<test_name>_interface, 0, 0, 0},
    #endif
    };

.. Note::
    On most platforms non-secure tests and secure tests run on the same CPU
    core, but dual-core platform is an exception. So secure test library and
    secure sevices shall be linked together in the file
    ``tf-m-tests/test/secure_fw/secure_tests.cmake``. Thus they can be built on
    secure CPU core and non-secure tests library and RTOS are built on
    non-secure CPU core.

.. code-block:: cmake

    if (TEST_FRAMEWORK_S)
        # ...
        if (TEST_S_<TEST_NAME>)
            add_library(tfm_test_suite_<test_name>_s STATIC EXCLUDE_FROM_ALL)
        endif()
    endif()

************************************
Adding a new test case in test suite
************************************

The test cases usually express as a function in source code. They will be added
into an array with structure type called ``test_t`` defined in
``tf-m-tests/test/framework/test_framework.h``.

.. code-block:: c

    struct test_t {
        TEST_FUN * const test;         /*!< Test function to call */
        const char *name;              /*!< Test name */
        const char *desc;              /*!< Test description */
    };

For example, a new test case called ``TFM_NS_<TEST_NAME>_TEST_1001`` is created
and the function ``tfm_<test_name>_test_1001`` needs to be defined in file
``<test_name>_ns_interface_testsuite.c``. Then the function shall be appended
into the array which will be quoted in function
``register_testsuite_ns_<test_name>_interface``. See the reference code below.

.. code-block:: c

    /* List of test cases */
    static void tfm_<test_name>_test_1001(struct test_result_t *ret);

    /* Append test cases */
    static struct test_t <test_name>_tests[] = {
        {&tfm_<test_name>_test_1001, "TFM_NS_<TEST_NAME>_TEST_1001",
        "Example test case"},
    };

    /* Register test case into test suites */
    void register_testsuite_ns_<test_name>_interface(struct test_suite_t *p_test_suite)
    {
        uint32_t list_size;

        list_size = (sizeof(<test_name>_tests) / sizeof(<test_name>_tests[0]));

        set_testsuite("<TEST_NAME> non-secure interface test (TFM_NS_<TEST_NAME>_TEST_1XXX)",
                        <test_name>_tests, list_size, p_test_suite);
    }

    static void tfm_<test_name>_test_1001(struct test_result_t *ret)
    {
       /* test case code */
    }

********************
Adding test services
********************

Some test group may need specific test services. These test services may support
one or more groups thus developers shall determine the proper test scope.

Steps
=====

Adding a test service is same as adding a secure partition, generally the
process can be referenced from the document
:doc:`Adding Secure Partition </integration_guide/services/tfm_secure_partition_addition>`

.. Note::
    Each test service must have resource requirements declared in a manifest
    file, the contents of test services are the same as secure partitions, but
    their locations are different. Test service manifests shall be set in
    ``tf-m-tests/test/secure_fw/tfm_test_manifest_list.yaml``.

Test Partition Specific Manifest Attributes
===========================================
There are some test purpose attributes in Secure Partition manifests that are
**NOT** compatible with FF-M.
They should be used in Test Partitions only.

weak_dependencies
-----------------
A TF-M regression test Partition calls other RoT services for test. But it
can still run other tests if some of the RoT services are disabled.
TF-M defines a ``"weak_dependencies"`` attribute in partition manifests of
regression test partitions to describe test service access to other RoT
services. It *shall* be only used for TF-M regression test services.

model
-----
A TF-M regression test Partition may support both the SFN and IPC model.
The actual model being used follows the SPM backend enabled.

The TF-M build system supports this by allowing Secure Partitions to set
the ``model`` attribute to ``dual``.
The manifest tool will then change it to the corresponding value according
to the current backend selected.

The Test Partitions use the following definitions to know what model is being
built:

- ``<<partition_name>>_MODEL_IPC``, ``1`` if IPC model is used.
- ``<<partition_name>>_MODEL_SFN``, ``1`` if SFN model is used.

Test service implementation
===========================

Test service of individual test
-------------------------------

An individual test dedicated test service should be put under the corresponding
test folder ``test/secure_fw/suites/<test_name>``.

``add_subdirectory(suites/<test_name>/<service_dir>)`` shall be added into
``tf-m-tests/test/secure_fw/secure_tests.cmake`` to make sure that the test
service is built with secure side configuration.

Common test service
-------------------

If a new test service is required by multiple test suites, the code should be
put under ``test/secure_fw/common_test_services``. If the new test suite relies
on a common test service, please make sure that the build implementation of the
test service is linked correctly, including the header files and libraries.

**********************************
Out-of-tree regression test suites
**********************************

TF-M supports out-of-tree regression test suites build, whose source code
folders are maintained outside tf-m-tests repo. There are two configurations
for developers to include the source code.

- ``EXTRA_NS_TEST_SUITES_PATHS``

  A list of the absolute directories of the out-of-tree non-secure test suites
  source code folder(s). TF-M build system searches ``CMakeLists.txt`` of
  non-secure test suites in the source code folder(s).
  Use semicolons ``;`` to separate multiple out-of-tree non-secure test suites
  directorires.

- ``EXTRA_S_TEST_SUITES_PATHS``

  A list of the absolute directories of the out-of-tree secure test suites
  source code folder(s).

Example usage
=============

Take non-secure test as an example in
`tf-m-extras <https://git.trustedfirmware.org/TF-M/tf-m-extras.git/>`__.
A single out-of-tree test suite folder can be organized as the figure below:

.. code-block:: bash

    extra_ns
    ├── CMakeLists.txt
    ├── ns_test.c
    ├── ns_test_config.cmake
    └── ns_test.h

In the example above, ``EXTRA_NS_TEST_SUITES_PATHS`` in the build command can be
specified as listed below.

.. code-block:: bash

  -DEXTRA_NS_TEST_SUITES_PATHS=<Absolute-path-extra-test-folder>

Coding instructions
===================

This is a demo of source code so the structure has been simplified. Files like
``ns_test.c`` and ``ns_test.h`` can be expanded to ``src`` and ``include``
folders. The ``CMakeLists.txt`` is required in the root path and
``ns_test_config.cmake`` is optional.

Header Files
------------

The header file ``extra_ns_tests.h`` must be included by out-of-tree source
code. This file contains the definition of ``struct extra_tests_t``,
``int32_t register_extra_tests()`` and declaration of
``int32_t extra_ns_tests_init()``.

Source code
-----------

To connect the out-of-tree source code and tf-m-tests framework, the high-level
test function must be defined first. An example format is:

.. code-block:: c

    int32_t ns_test(void)
    {
        /* Add platform specific non-secure test suites code here. */

        return EXTRA_TEST_SUCCESS;
    }

This function is the main entry to test framework. All the out-of-tree's test
cases can be added into it. The returned error code of this function is
specified as ``int32_t``. This function shall return an expected value which is
same as the part ``expected_ret`` set in ``plat_ns_t`` below, take the macro
EXTRA_TEST_SUCCESS as an example.

After ``ns_test()`` is defined, a structure variable need to be created like:

.. code-block:: c

    const struct extra_tests_t plat_ns_t = {
        .test_entry = ns_test,
        .expected_ret = EXTRA_TEST_SUCCESS
    };

It will be used by function ``extra_ns_tests_init()`` to register the test by
function ``register_extra_tests()``:

.. code-block:: c

    int32_t extra_ns_tests_init(struct extra_tests_t *internal_test_t)
    {
        /* Add platform init code here. */

        return register_extra_tests(internal_test_t, &plat_ns_t);
    }

The platform initialization code can be added in this function because it runs
before ``ns_test()``.

.. Note::
    Function ``extra_ns_tests_init()`` is declared in tf-m-tests repository
    without definition. It is supplied to out-of-tree source code and need to be
    defined with no change of its format, like returns error code and parameter
    name.


CMakeLists.txt
--------------

In addition to the implementation of CMAKE target like ``example_test_ns`` of
out-of-tree source code, the configuration below needs to be appended:

.. code-block:: cmake

    # Example test must link tfm_test_suite_extra_common to use related interface
    target_link_libraries(example_test_ns
        PRIVATE
            tfm_test_suite_extra_common
    )

    # The example_test_ns library must be linked by tfm_test_suite_extra_common
    target_link_libraries(tfm_test_suite_extra_ns
        PRIVATE
            example_test_ns
    )

To use the interfaces come from tf-m-tests repository, library
``tfm_test_suite_extra_common`` must be linked by ``example_test_ns``.
To add out-of-tree test into TF-M, library ``example_test_ns`` must be linked
by ``tfm_test_suite_extra_ns``.

ns_test_config.cmake
--------------------

The CMAKE configuration file is optional. If out-of-tree source already exists
another configuration file, a new one can be ignored.

--------------

*Copyright (c) 2021-2022, Arm Limited. All rights reserved.*
