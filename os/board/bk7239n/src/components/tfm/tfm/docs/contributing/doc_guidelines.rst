#####################################
Documentation Contribution Guidelines
#####################################

The Trusted Firmware-M project uses `Sphinx`_ to generated the Official
Documentation from `Restructed Text`_ `.rst` source files,

The aim is to align as much as possible with the official
`Python Documentation Guidelines`_ while keeping the consistency with the
already existing files.

The guidance below is provided as a help. It is not meant to be a definitive
list.

********
Overview
********

The following short-list provides a quick summary of the rules.

- If the patch modifies a present file, the file's style should be followed
- If creating a new file,
  :doc:`integration guide </integration_guide/index>` can be used as a reference.
- When a new style is to be expressed, consult the `Python Documentation Guidelines`_

*************
List of rules
*************
The following rules should be considered:

#. H1 document title headers should be expressed by # with over-line
   (rows on top and bottom) of the text
#. Only ONE H1 header should allowed per document, ideally placed
   on top of the page.
#. H2 headers should be expressed by * with over-line
#. H2 header's text should be UNIQUE in per document basis
#. H3 headers should be expressed by an underline of '='
#. H4 headers should be expressed by an underline of '-'
#. H3 and H4 headers have no limitation about naming.
   They can have similar names on the same document, as long as
   they have different parents.
#. H5 headers should be expressed by an underline of '^'
#. H5 headers will be rendered in document body but not in
   menus on the navigation bar
#. Do not use more than 5 levels of heading
#. When writing guides, which are expected to be able to be readable by
   command line tools, it would be best practice to add long complicated
   tables, and UML diagrams in the bottom of the page and using internal
   references(auto-label)
#. No special formatting should be allowed in Headers
   (code, underline, strike-through etc)
#. Long URLs and external references should be placed at the bottom of the
   page and referenced in the body of the document
#. New introduced terms and abbreviations should be added to Glossary and
   directly linked by the `:term:` notation across all documents using it.


**********************
Platform Documentation
**********************

The Documentation Build system provides an interface with the platform directory
allowing maintainers to bundle platform specific documentation. **This behaviour
needs to be explicitly enabled for each platform's space** by
modifying the `platform/ext/index.rst` (responsible for generating the
:doc:`Platform Index File </platform/ext/index>`) and adding a table of
contents entry for the corresponding platform space.

The format and structure of this entry is not strictly defined, and allows
flexible control of the platform's documentation space. In most cases it can be
set to recursively match all documents under that directory.

.. code-block:: restructuredtext

    .. toctree::
        :maxdepth: 4
        :caption: PLATFORM_X_CAPTION
        :glob:

        target/PLATFORM_X/**

****************
Common Use Cases
****************

The section below describes with examples, a rule compliant implementation
for most common documentation elements.

Headers
=======


.. code-block:: restructuredtext

    ###################
    Document Title (H1)
    ###################

    ******************
    Chapter Ttitle(H2)
    ******************

    Chapter Section (H3)
    ====================

    Chapter Sub-Section (H4)
    ------------------------

    Subsection of Chapter sub-section (H5)
    ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Code Blocks
===========


The recommendation for code content, is to use the explicit code-block directive,
ideally with a defined lexer for the language the block contains.

A list of compatible lexers can be found at `Pygments Lexers`_

.. code-block:: restructuredtext

    .. code-block:: bash

        ls
        pwd

    .. code-block:: doscon

        dir

    .. code-block:: c

        static struct rn_object_t;

    .. code-block:: python3

        print("Hello TF-M")


Restructured Text supports implicit code-blocks by indenting a section of text,
surrounded by new lines. While this formatting is
allowed, it should be avoided if possible.

.. code-block:: restructuredtext

    The quick brown fox jumps over the lazy dog

        ls
        pwd

.. Note::

    Mixing two different code-block formats in the same document will break
    the whole document's rendering. When editing an existing document, please
    follow the existing format.

    **New documents should always use the explicit format.**

Tables
======

For adding new tables the `table::` notation should be used.

.. code-block:: restructuredtext

    .. table:: Components table
       :widths: auto

       +--------------+--------------+-------------+
       | **Title A**  | **Title B**  | **Title C** |
       +==============+==============+=============+
       | Text A       | Text B       | Text C      |
       +--------------+--------------+-------------+


While the equivalent simple table code will render correctly in the output, it
will not be added to the index (So it cannot be referenced if needed)

.. code-block:: restructuredtext

   +--------------+--------------+-------------+
   | **Title A**  | **Title B**  | **Title C** |
   +==============+==============+=============+
   | Text A       | Text B       | Text C      |
   +--------------+--------------+-------------+

Other types of tables such as list-tables and csv-tables are also permitted, as
seen on :doc:`/getting_started/tfm_getting_started` and
:doc:`/releases/1.0`


External Links
==============

External links should be placed in the bottom of a document.

.. code-block:: restructuredtext

    The quick brown fox jumps over the lazy dog according to `Link_A`_

    .. _Link_A: https://www.aaa.org
    .. _Link_B: https://www.bbb.org

    --------------

    *Copyright (c) XYZ *

Creating in-line links is permitted, but should be avoided if possible. It
should be only used for consistency purposes or for a small ammount
of short links.

.. code-block:: restructuredtext

    The quick brown fox jumps over the lazy dog according to `Link_A <https://www.aaa.org>`_

If for the purposes of content, a link is to be referenced by multiple
labels, internal linking is the recommended approach.

.. code-block:: restructuredtext

    The quick brown fox jumps over the lazy dog according to `Link_A_New`_

    .. _Link_A: https://www.aaa.org
    .. _Link_A_New: `Link_A`_

    --------------

    *Copyright (c) XYZ *


Document Links
==============

A document included in the documentation can be referenced by the `doc:` notation

.. code-block:: restructuredtext

    :doc:`integration guide </integration_guide/tfm_integration_guide>`

The path is relative to the root of the Trusted Firmware-M code.

Reference specific section of a document
========================================

In order to reference a specific section of a document, up to level 4 headers
(if they are included in the index), the `ref:` keyword can be used

.. code-block:: restructuredtext

    :ref:`docs/getting_started/tfm_getting_started:Tool & Dependency overview`

This can also be used to quickly scroll to the specific section of the current
document. This technique can be used to add complex table in the bottom of a
document and create clickable quick access references to it for improved user
experience.


Glossary term
=============

For technical terms and abbreviations, the recommended guidance is to add an
entry to the :doc:`/glossary` and refer to it, using the `term:`
directive


.. code-block:: restructuredtext

    HAL
    Hardware Abstraction Layer
        Interface to abstract hardware-oriented operations and provides a set of
        APIs to the upper layers.

    .....

    As described in the design document :term:`HAL` abstracts the
    hardware-oriented and platform specific
    .......

.. Note::

   The ":term:" directive does not work when used in special formatting.
   Using \*:term:`HAL`\* **will not link to the glossary term**.

References
==========

#. `Sphinx`_
#. `Restructed Text`_
#. `Python Documentation Guidelines`_
#. `Pygments Lexers`_

.. _Sphinx: https://www.sphinx-doc.org/en/master/
.. _Restructed Text: https://docutils.sourceforge.io/rst.html
.. _Python Documentation Guidelines: https://devguide.python.org/documenting/#style-guide
.. _Pygments Lexers: https://pygments.org/lexers/

--------------

*Copyright (c) 2020-2021, Arm Limited. All rights reserved.*
