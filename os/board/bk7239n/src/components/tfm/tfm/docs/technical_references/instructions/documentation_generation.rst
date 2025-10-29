########################
Documentation generation
########################

Two documents are available for generation:

- Reference Manual (HTML, PDF)
- User Guide (HTML, PDF)

The documentation build is independent from building the binary artifacts.

******************************
Tools and building environment
******************************

These tools are used to generate TF-M documentation:

    - Doxygen v1.8.0 or later
    - Graphviz dot v2.38.0 or later
    - PlantUML v1.2018.11 or later
    - Java runtime environment v1.8 or later (for running PlantUML)
    - Sphinx and other python modules, listed in ``tools/requirements_docs.txt``

Additionally, for PDFs format:

    - LaTeX
    - PdfLaTeX

There are two ways of building TF-M reference manual:

    1. As a custom target of TF-M CMake build system
    2. Directly, using the command line tools


To prepare your building environment execute the following steps:

.. tabs::

    .. group-tab:: Linux

        .. code-block:: bash

            sudo apt-get install -y doxygen graphviz default-jre
            mkdir ~/plantuml
            curl -L http://sourceforge.net/projects/plantuml/files/plantuml.jar/download --output ~/plantuml/plantuml.jar
            export PLANTUML_JAR_PATH=~/plantuml/plantuml.jar

            # For PDF generation
            sudo apt-get install -y doxygen-latex

            # Install the required Python modules
            pip3 install --upgrade pip
            cd trusted-firmware-m
            pip3 install -r tools/requirements_docs.txt

    .. group-tab:: Windows

        Download and install the following tools:

            - `Doxygen 1.8.8 <https://sourceforge.net/projects/doxygen/files/snapshots/doxygen-1.8-svn/windows/doxygenw20140924_1_8_8.zip/download>`__
            - `Graphviz 2.38 <https://graphviz.gitlab.io/_pages/Download/windows/graphviz-2.38.msi>`__
            - The Java runtime is part of the Arm DS installation or can be
               downloaded from `here <https://www.java.com/en/download/>`__
            - `PlantUML <http://sourceforge.net/projects/plantuml/files/plantuml.jar/download>`__
            -  `MikTeX <https://miktex.org/download>`__ - for PDF generation only

        Set the environment variables, assuming that:

            - doxygen, dot, and MikTeX binaries are available on the PATH.
            - Java JVM is used from Arm DS installation.

        .. code-block:: bash

            set PLANTUML_JAR_PATH=<plantuml_Path>\plantuml.jar
            set PATH=$PATH;<ARM_DS_PATH>\sw\java\bin

            # Install the required Python modules
            pip3 install --upgrade pip
            cd trusted-firmware-m
            pip3 install -r tools\requirements_docs.txt

***************************
Build TF-M Reference Manual
***************************

The Reference Manual will be generated in the ``build_docs/reference_manual``.

.. tabs::

    .. group-tab:: Linux

        .. code-block:: bash

            cd <TF-M base folder>
            cmake -S docs -B build_docs
            cmake --build build_docs -- tfm_docs_refman_html tfm_docs_refman_pdf

    .. group-tab:: Windows

        .. code-block:: bash

            cd <TF-M base folder>
            cmake -S docs -B build_docs -G"Unix Makefiles"
            cmake --build build_docs -- tfm_docs_refman_html tfm_docs_refman_pdf

*********************
Build TF-M User Guide
*********************

The User Manual will be available under the directory ``build_docs/user_guide``.

.. tabs::

    .. group-tab:: Linux

        .. code-block:: bash

            cd <TF-M base folder>
            cmake -S docs -B build_docs
            cmake --build build_docs -- tfm_docs_userguide_html tfm_docs_userguide_pdf

    .. group-tab:: Windows

        .. code-block:: bash

            cd <TF-M base folder>
            cmake -S docs -B build_docs -G"Unix Makefiles"
            cmake --build build_docs -- tfm_docs_userguide_html tfm_docs_userguide_pdf


***************************************
Direct build using a command line tools
***************************************

The direct build will build both user_guide and reference_manual.


.. tabs::

    .. group-tab:: Linux

        .. code-block:: bash

            # Build the documentation from build_docs directory
            cd <TF-M base folder>
            mkdir build_docs
            cp docs/conf.py build_docs/conf.py
            cd build_docs
            sphinx-build ./ user_guide

    .. group-tab:: Windows

        .. code-block:: bash

            # Command line tools is yet not available for Windows

************
Dependencies
************

.. uml::

   @startuml
    skinparam state {
      BackgroundColor #92AEE0
      FontColor black
      FontSize 16
      AttributeFontColor black
      AttributeFontSize 16
      BackgroundColor<<pdf>> #A293E2
      BackgroundColor<<doc>> #90DED6
    }
    state u_guide as "User Guide" <<doc>>
    state refman as "Reference Manual" <<doc>>
    state rtd_theme as "sphinx-rtd-theme" <<doc>>
    state tabs as "sphinx-tabs" <<doc>>
    state sphnix_puml as "sphinxcontrib-plantuml" <<doc>>
    state sphnix_svg as "sphinxcontrib-svg2pdfconverter" <<doc>>
    state JRE as "JRE" <<doc>> : Java Runtime Environment
    state gwiz as "Graphwiz dot" <<doc>>
    state Sphinx as "Sphinx" <<doc>>
    state python as "Python v3" <<doc>>
    state m2r as "m2r2" <<doc>>
    state PlantUML as "PlantUML" <<doc>>
    state LaTex as "LaTex" <<pdf>>
    state PdfLaTex as "PdfLaTex" <<pdf>>
    state Doxygen as "Doxygen" <<doc>>
    state librsvg as "librsvg2-bin" <<doc>>

    [*] --> u_guide
    u_guide --> Sphinx
    Sphinx --> m2r
    Sphinx --> rtd_theme
    Sphinx --> tabs
    Sphinx --> sphnix_puml
    Sphinx --> sphnix_svg
    m2r --> python
    rtd_theme --> python
    tabs --> python
    sphnix_puml --> python
    sphnix_svg --> python
    sphnix_svg --> librsvg
    Sphinx --> PlantUML
    PlantUML --> JRE
    PlantUML --> gwiz
    Sphinx --> LaTex
    LaTex --> PdfLaTex

    [*] --> refman
    refman --> Doxygen
    Doxygen --> PlantUML
    Doxygen --> LaTex
    state Legend {
      state x as "For PDF generation only" <<pdf>>
    }

   @enduml


.. _sphinx-build: https://www.sphinx-doc.org/en/master/man/sphinx-build.html
.. _Doxygen: https://www.doxygen.nl

--------------

*Copyright (c) 2017-2021, Arm Limited. All rights reserved.*
