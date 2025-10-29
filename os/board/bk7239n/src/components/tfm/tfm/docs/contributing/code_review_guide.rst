#####################
Code Review Guideline
#####################
The purpose of this document is to clarify design items to be reviewed during
the code review process.

Please contact :doc:`maintainers </contributing/maintainers>` or write an e-mail
thread on the `TF-M mailing list <mailto:tf-m@lists.trustedfirmware.org>`_ for
any questions.

**********************
List of the guidelines
**********************
The prerequisites before going to the review stage:

- Read the :doc:`Contributing Process </contributing/contributing_process>`
  to know basic concepts.
- Read the :doc:`Source Structure </technical_references/design_docs/source_structure>`
  for structure related reference.

The review guidelines consist of these items:

- `Exceptions`_.
- `Non-source items`_.
- `Namespace`_.
- `Assembler code`_.
- `Include`_.
- `Auto-doc`_.
- `Commit Message`_.

.. note::
  Each guideline item is assigned with a unique symbol in the format ``Rx.y``,
  while ``x`` and ``y`` are numeric symbols. These symbols are created for easy
  referencing during the code review process.

Exceptions
==========
Files under listed folders are fully or partially imported from 3rd party
projects, these files follow the original project defined guidelines and
styles:

.. important::
  - R1.1 ``ext`` and its subfolders.

Non-source items
================
For all non-source files such as build system files or configuration files:

.. important::
  - R2.1 Follow the existing style while making changes.

Namespace
=========
TF-M assign the namespace to files and symbols for an easier code reading. The
symbol here includes functions, variables, types and MACROs. The prerequisites:

.. important::
  - R3.1 Follow the documents or specifications if they propose namespaces
    ('psa\_' for PSA defined items E.g.,). Ask the contributor to tell the
    documents or specifications if the reviewer is not sure about a namespace.
  - R3.2 Assign TF-M specific namespace 'tfm\_' or 'TFM\_' for symbols
    implementing TF-M specific features. Ask the contributor to clarify the
    purpose of the patch if the reviewer is not sure about the namespace.

For the sources out of the above prerequisites (R3.1 and R3.2):

.. important::
  - R3.3 Do not assign a namespace for source files.
  - R3.4 Assigning a namespace for interface symbols is recommended. The
    namespace needs to be expressed in one word to be followed by other words
    into a phrase can represent the functionalities implemented. One word
    naming is allowed if the namespace can represent the functionality
    perfectly.
  - R3.5 Assign a namespace for private symbols is NOT recommended.
  - R3.6 Do not assign characters out of alphabets as the leading character.

Examples:

.. code-block:: c

  /* R3.1 FILE: s/spm/ffm/psa_client.c */

  /* R3.2 FILE: s/spm/cmsis_psa/tfm_secure_context.c */

  /* R3.3 FILE: s/spm/cmsis_psa/main.c */

  /* R3.4 FILE: s/spm/cmsis_psa/main.c, 'main' is a good entry name. */
  void main(void);
  /* R3.4 FILE: s/spm/ffm/spm.c, 'spm\_' as the namespace */
  void spm_init(void);

  /* R3.5 FILE: s/spm/ffm/main.c */
  static void init_functions(void);

  /* R3.6 Not permitted: */
  /* static uint32_t __count; */

Assembler code
==============

.. important::
  - R4.1 Pure assembler sources or inline assembler code are required to be put
    under the platform-independent or architecture-independent folders.
    The logic folders should not contain any assembler code, referring to
    external MACRO wrapped assembler code is allowed. Here is one example of the
    logic folder:

    - 'secure_fw/spm/ffm'.

Examples:

.. code-block:: c

  /*
   * R4.1 The following MACRO is allowed to be referenced under
   * 'secure_fw/spm/ffm'
   */
  #define SVC(code) __asm volatile("svc  %0", ::"I"(code))

Include
=======
This chapter describes the placement of the headers and including. There are
two types of headers: The ``interface`` headers contain symbols to be shared
between modules and the ``private`` headers contain symbols only for internal
usage.

.. important::
  - R5.1 Put the ``interface header`` of one module in the ``include`` folder
    under the root of this module. Deeper sub-folders can not have ``include``
    folders, which means only one ``include`` is allowed for one module.

  - R5.2 Creating sub-folders under ``include`` to represent the more granular
    scope of the interfaces is allowed.

  - R5.3 ``private header`` can be put at the same place with the implementation
    sources for the private symbols contained in the header. It also can be put
    at the place where the sources need it. The latter is useful when some
    "private header" contains abstracted interfaces, but these interfaces are
    not public interfaces so it won't be put under "include" folder.

  - R5.4 Use <> when including public headers.

  - R5.5 Use "" when including private headers.

  - R5.6 The module's ``include`` folder needs to be added into referencing
    module's header searching path.

  - R5.7 The module's ``include`` folder and the root folder needs to be added
    into its own header searching path and apply a hierarchy including with
    folder name.

  - R5.8 Path hierarchy including is allowed since there are sub-folders under
    ``include`` folder and the module folder.

  - R5.9 The including statement group order: the beginning group contains
    toolchain headers, then follows the public headers group and finally the
    private headers group.

  - R5.10 The including statement order inside a group: Compare the include
    statement as strings and sort by the string comparison result.

  - R5.11 The header for the referenced symbol or definition must be included
    even this header is included inside one of the existing included headers.
    This improves portability in case the existing header implementation
    changed.

Examples:

.. code-block:: c

  /*
   * The structure:
   *   module_a/include/func1.h
   *   module_a/include/func2/fmain.h
   *   module_a/func1.c
   *   module_a/func2/fmain.c
   *   module_b/include/funcx.h
   *   module_b/include/funcy/fmain.h
   *   module_b/funcx.c
   *   module_b/funcxi.h
   *   module_b/funcy/fmain.c
   *   module_b/funcy/fsub.c
   *   module_b/funcy/fsub.h
   * Here takes module_b/funcx.c as example:
   */
  #include <func1.h>
  #include <func2/fmain.h>
  #include <funcx.h>
  #include "funcxi.h"
  #include "funcy/fsub.h"

Auto-doc
========
Auto document system such as doxygen is useful for describing interfaces. While
it would be a development burden since the details are described in the design
documents already. The guidelines for auto-doc:

.. important::
  - R6.1 Headers and sources under these folders need to apply auto-doc style
    comments: ``*include``.
  - R6.2 Developers decide the comment style for sources out of listed folders.

Commit Message
==============
TF-M has the requirements on commit message:

.. important::
  - R7.1 Assign correct topic for a patch. Check the following table.

============== ====================================================
Topic          Justification
============== ====================================================
Boot           bl2/*
Build          For build system related purpose.
Docs           All \*.rst changes.
Dualcpu        Dual-cpu related changes.
HAL            Generic HAL interface/implementation changes.
Pack           For packing purpose.
Platform       Generic platform related changes under `platform/*`.
Platform Name  Specific platform changes.
Partition      Multiple partition related changes.
Partition Name Specific partition related changes.
Service        Multiple service related changes.
Service Name   Specific service related changes.
SPM            secure_fw/spm/*
SPRTL          secure-fw/partitions/lib/sprt/*
============== ====================================================

.. note::
  Ideally, one topic should cover one specific type of changes. For crossing
  topic changes, check the main part of the change and use the main part
  related topic as patch topic. If there is no suitable topics to cover the
  change, contact the community for an update.

--------------

*Copyright (c) 2020-2021, Arm Limited. All rights reserved.*
