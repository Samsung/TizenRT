#########################################################
Generic OS migration from Armv7-M to Armv8-M architecture
#########################################################
The purpose of this document is to list a set of requirements needed for
migrating a generic OS kernel running on Armv7-M to the Armv8-M architecture.

********************
List of requirements
********************
- If the same OS codebase is used for both Secure and Non Secure builds, it is
  suggested to put specific code targeted to the Non Secure build under a
  compile time switch, e.g. ``#if (DOMAIN_NS == 1U)``. The OS build system in
  this case needs to be amended accordingly to support this new switch.
- If the OS implements stack limit checking, the ``PSPLIM`` register
  needs to be initialized and properly handled during thread context switch
  operations.
- If the OS manipulates directly the Link Register, the default Link Register
  value used in Handler mode transitions needs to be differentiated between
  Secure and Non Secure builds, i.e. ``0xFD`` and ``0xBC``, respectively.
- If the OS manages the non-secure client identification, pelase check the
  :doc:`Non-secure Client Extension Integration Guide </integration_guide/non-secure_client_extension_integration_guide>`.

--------------

*Copyright (c) 2018-2021, Arm Limited. All rights reserved.*
