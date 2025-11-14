##########################################
Stateless Root of Trust Services Reference
##########################################

:Author: Mingyang Sun
:Organization: Arm Limited
:Contact: mingyang.sun@arm.com


************
Introduction
************

This document describes the implementation for the FF-M v1.1 feature -
'Stateless RoT Service', and the related references when developing RoT
services.

It is recommended to refer to the FF-M v1.0 specification [1]_ and FF-M v1.1
extension [2]_ for background and rationale details.


**********************
Implementation Details
**********************

This chapter describes the implementation-defined items, including stateless
handle value definition, tooling update, and programming API changes.

Stateless Handle Value Definition
=================================

The index, stateless indicator, and service version information are encoded into
a handle by the manifest tooling, and then generated to header file ``sid.h``.

.. list-table:: Bit Fields of Stateless Handle
    :header-rows: 1
    :widths: 20 80

    * - Bits
      - Field Description
    * - bit 31
      - reserved
    * - bit 30
      - stateless handle indicator bit, always 1
    * - bit 29 - bit 16
      - reserved
    * - bit 15 - bit 8
      - service version requested by client - for client version check
    * - bit 7 - bit 0
      - the handle index, [0, 31]

Since connection-based services and stateless services share the same PSA API
``psa_call()``, an indicator bit is set in the handle indicate the type of the
handle. If it is set, the handle is stateless, and definition is as described
in the table above. Maximum connection-based handle is 0x3FFFFFFF, thus the
indicator bit is always 0.

The index is used for organizing stateless services in manifest tool and
locating a stateless service in SPM logic. A range of index [0, 31] is the
initial implementation. Future expansion is possible.

Tooling Support
===============

TF-M provides a tool (``tools/tfm_parse_manifest_list.py``) to generate source
header files required by partition and services. For example, the generated
``sid.h`` contains service ID and version. The tooling is extended to generate
stateless handle from partition manifests automatically.

The ``stateless_handle`` attribute in manifest is only supported by partitions
with firmware framework version 1.1.

- If ``stateless_handle`` in manifest is set to an integer, the index is
  ``stateless_handle - 1``.
- If it is ``auto`` or not set, the first empty index in range [0, 31] is
  assigned.
- Other settings - tooling reports an error.

Finally, the tooling encodes the handle according to definitions in
`Stateless Handle Value Definition`_ section, and writes them to ``sid.h``
header file.

Changes in Programming API
==========================

This chapter describes the changes in programming API for stateless services.
The following APIs' bebavious and message data structure members are updated to
support the stateless service.

psa_connect()
-------------

According to FF-M v1.1, client calling ``psa_connect()`` with the SID of a
stateless RoT Service is a ``PROGRAMMER_ERROR``.

psa_close()
-----------

According to FF-M v1.1, client passing a stateless handle to call this API is a
``PROGRAMMER_ERROR``.

psa_call()
----------

.. code-block:: c

    psa_status_t psa_call(psa_handle_t handle, int32_t type,
                          const psa_invec *in_vec, size_t in_len,
                          psa_outvec *out_vec, size_t out_len)

API parameters and behaviour change:

1. The ``handle`` parameter must be a stateless handle defined in
   ``psa_manifest/sid.h`` when requesting a stateless service.
2. This API validates stateless handle, decodes index and service version
   information from it. SPM uses the index to know which stateless service is
   requested.
3. This API performs some operations as ``psa_connect()`` does, such as the
   authorization check, service and client version check, and handle space
   allocation.

Service behaviour change during a "psa_call":

Service does not accept connection and disconnection messages. After a
"psa_call" request is serviced, it calls ``psa_reply()``, frees the connection
handle to handle pool.

psa_set_rhandle()
-----------------

According to FF-M v1.1, stateless service calling this API on a message is a
``PROGRAMMER_ERROR`` and it will never return.

psa_msg_t type
--------------

The ``rhandle`` member of a ``psa_msg_t`` type received by a stateless RoT
Service is always ``NULL``.


**************************
Application Recommendation
**************************

There are particular services that do not need sessions. The access to the
service is a one-shot connection. These services provide standalone operations
that do not require any non-volatile state of resources to be maintained by the
RoT service itself or do not expose any kind of context or session to the
caller. Such services are recommended to be implemented as stateless, to provide
quick access and to avoid extra overheads.

In this case, ``rhandle`` access would be prohibited as it is used for saving
state or non-volatile resources while stateless services do not need them.

Update Feasibility of Existing Services
=======================================

TF-M native services are used widely. They only need standalone operations and
do not need to keep state between sessions. For example, the service in Crypto
partition does not do anything during ``psa_connect()`` or ``psa_close()``
process. Same for services in other partitions, thus all of them can be
implemented as stateless.

Analysis for them:

.. list-table:: TF-M Partition Services Update Possibility
    :header-rows: 1
    :widths: 30 30 40

    * - Partition
      - Number of Services
      - Can be Stateless
    * - ITS
      - 4
      - All
    * - PS
      - 5
      - All
    * - Crypto
      - 1
      - All
    * - FWU
      - 6
      - All
    * - Platform
      - 4
      - All
    * - Initial Attestation
      - 2
      - All

Other services are not analyzed here.

Grouping Services
=================

Stateless service table is stored statically, and TF-M supports 32 stateless
services currently.

Similar stateless services in a partition could be grouped, and assign one
``SID`` for the group. The ``type`` parameter in ``psa_call()`` could be
extended to identify the service in group. In this case, it is recommended to
use consecutive value for ``type``.

It is recommended that each Seccure Partition declares one stateless service
and uses the type parameter to distinguish different stateless services.
Therefore, more stateless services can be supported.

Migrating to Stateless Services
===============================

Please refer to Chapter 4 "Stateless Root of Trust services", Appendix B.3.2
"Using a stateless RoT Service", and Appendix D "Implementing session-less RoT
Services" in FF-M v1.1 document for details on which kind of service can be
stateless and how to implement a stateless service.


*********
Reference
*********

.. [1] `FF-M v1.0 Specification <https://developer.arm.com/-/media/Files/pdf/PlatformSecurityArchitecture/Architect/DEN0063-PSA_Firmware_Framework-1.0.0-2.pdf?revision=2d1429fa-4b5b-461a-a60e-4ef3d8f7f4b4>`__

.. [2] `FF-M v1.1 Extention <https://documentation-service.arm.com/static/600067c09b9c2d1bb22cd1c5?token=>`__

--------------

*Copyright (c) 2021, Arm Limited. All rights reserved.*
