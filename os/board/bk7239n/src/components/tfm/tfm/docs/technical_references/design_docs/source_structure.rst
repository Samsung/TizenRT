###################################
Trusted Firmware-M Source Structure
###################################

:Organization: Arm Limited
:Contact: tf-m@lists.trustedfirmware.org

.. note::
  Reference the document :doc:`Glossary </glossary>` for terms
  and abbreviations.

************
Introduction
************
TF-M is designed to provide a user-friendly source structure to ease
integration and service development. This document introduces the source
structure and its design intention of TF-M.

.. note::
  - This document goes ahead of the implementation so the existing source
    structure may be different from the description listed here. It is
    recommended to refer to this document for ongoing code structure changes.
  - By getting feedback from the practical implementation, this document is
    updated continuously before it is detailed enough to be a user guide.

****************
Source Structure
****************
The description of the source structure is broken down into subsections, each
subsection introduces one detailed folder.

Root Directory
==============
This table describes the structure under the root directory with part of
possible folders. Note that the ``Detailed`` field indicates if the folder is
described in details here in this document:

============= ==================================== ====================
Folder name   Purpose                              Detailed
============= ==================================== ====================
bl2           The 2nd stage bootloader.            No.
cmake         Cmake files.                         No.
configs       Configuration system files.          No.
docs          The documentations.                  No.
lib           The 3rd party library.               No.
**platform**  Platform intermedia files.           See `'platform'`_.
**secure_fw** The secure firmware.                 See `'secure_fw'`_.
tools         Tools in scripts for building.       No.
============= ==================================== ====================

'platform'
==========
The platform sources contain necessary platform sources or intermedia files
pointing to the sources out of TF-M folder.

========================= =============================================
Folder name               Purpose
========================= =============================================
common/\*                 Common HAL implementation.
include/\*                Platform public headers.
<vendor>                  Vendor specific folders.
========================= =============================================

.. note::
  The ``common`` folder contains the example implementation of the ``HAL`` API
  bases on common ``system`` (CMSIS e.g.). The platform could reference to
  sources in the ``common`` folder directly if applicable or apply a
  customized HAL implementation.
  A ``system`` can have a standalone folder under ``common``, depends on how
  'common' this system is. Each ``platform vendor`` is assigned with one
  folder for usage. As the content of the ``vendor`` folder comes by the
  contribution of vendors, the ``vendor`` manages the structure inside it.

'secure_fw'
===========
This folder contains components needed by secure firmware, and the exported
interfaces for application, service development and HAL integration:

================= ===================================== ======================
Folder name       Purpose                               Detailed
================= ===================================== ======================
**include**       Public headers of 'secure_fw'.        See `'include'`_
**partitions**    Default services and supportings.     See `'partitions'`_
**spm**           PSA-FF-M SPM implementation. [1]      See `'spm'`_
================= ===================================== ======================

.. note::
  1. A PSA-FF-M complied SPM implementation.

  The headers referenced by other modules are public headers and put under the
  'include' folder of the current module. Do not put headers not referenced by
  other modules in this 'include' folder.

'include'
---------
This folder holds headers for client, services and platform integration usage.

=========================== ===================================================
Folder name                 Purpose
=========================== ===================================================
psa/\*                      PSA FF Client API.
=========================== ===================================================

.. note::
  TFM applies an explicit including policy. Each module's headers are put into
  a specific folder which follows the pattern '\*/inc', this folder needs to be
  added into the project's searching path if this project needs to include
  headers in this folder. The 'inc' in a path indicates the end of
  including-path. If there are subfolders under folder 'inc', apply a
  hierarchy including.

'partitions'
------------
This folder contains default services implemented as partitions and necessary
partition runtime utilities provided by TF-M.

================================= =============================================
Folder name                       Purpose
================================= =============================================
lib/sprt/\*                       The SPRTL sources and intermedia files. [1]
lib/sprt/shared\*                 Sources can be shared by out of SPRTL. [2]
<partition_x>/\*                  Files for 'partition_x'.
<partition_x>/include/\*          RoT Service API headers of 'partition_x'. [3]
<partition_y>/\*                  Files for 'partition_y'.
<partition_y>/include/\*          RoT Service API headers of 'partition_y'. [3]
================================= =============================================

.. note::
  1. The SPRTL sources and intermediate files. SPRTL contains sources from
     other folders, such as necessary RoT Service API implementation from
     'partitions' folder.
  2. The sources here can be referenced by the building system out of SPRTL.
     Generally, they are runtime and PSA APIs.
  3. Here takes 'partition_x' and 'partition_y' as an example, and showcases
     a detailed structure of them. The `<interface>` contains the RoT Service
     API for client calls.  The folder name of this client-orient folder is
     decided by the service developer.

'spm'
-----
The SPM is the core component to provide a mechanism for providing secure
services.

=================================== ===========================================
Folder name                         Purpose
=================================== ===========================================
include/\*                          SPM public headers.
ffm/\*                              SPM logic complies with PSA-FF-M and
                                    its necessary supporting functionalities,
                                    such as the runtime API and the thread
                                    operation, etc.
cmsis_psa/\*                        CMSIS implementation for PSA-FF-M SPM. [1]
cmsis_func/\*                       The library model implementation. [2]
\*                                  Implementation sources.
=================================== ===========================================

.. note::
  1. CMSIS is the first implementation system.
  2. This folder contains a function call based secure firmware implementation.
     This model is the prototype model which would be updated after the PSA
     model. Create a standalone folder to hold it.

--------------

*Copyright (c) 2020-2021, Arm Limited. All rights reserved.*
