###################################
Glossary of terms and abbreviations
###################################

.. glossary::
    :sorted:

    Application RoT
        `PSA term`_. The security domain in which additional security services
        are implemented.

    HAL
        Hardware Abstraction Layer:
        Interface to abstract hardware-oriented operations and provides a set of
        APIs to the upper layers.

    RoT
        Root of Trust:
        `PSA term`_. This is the minimal set of software, hardware and data that
        is implicitly trusted in the platform — there is no software or hardware
        at a deeper level that can verify that the Root of Trust is authentic
        and unmodified.

    RoT Service
        `PSA term`_. A set of related security operations that are implemented
        in a Secure Partition.

    NSPE
        Non Secure Processing Enviroment:
        `PSA term`_. In TF-M this means non secure domain typically running an
        OS using services provided by TF-M.

    PSA
        `PSA term`_. Platform Security Architecture.

    PSA-FF
        `PSA term`_. Platform Security Architecture Firmware Framework.

    PSA-FF-M
        `PSA term`_. Platform Security Architecture Firmware Framework for M.

    PSA RoT
        `PSA term`_. This defines the most trusted security domain within a PSA
        system.

    SFN
        Secure Function:
        The function entry to a secure service. Multiple SFN per SS are
        permitted.

    SP
    Secure Partition
        A logical container for secure services.

    SPE
        Secure Processing Environment:
        `PSA term`_. In TF-M this means the secure domain protected by TF-M.

    SPM
    Secure Partition Manager
        The TF-M component responsible for enumeration, management and isolation
        of multiple Secure Partitions within the TEE.

    SPRT
        Secure Partition Runtime:
        The TF-M component responsible for Secure Partition runtime
        functionalities.

    SPRTL
        Secure Partition Runtime Library:
        A library contains the SPRT code and data.

    SS
        Secure Service:
        A component within the TEE that is atomic from a security/trust point of
        view, i.e. which is viewed as a single entity from a TF-M point of view.

    PS
    Protected Storage
       One of PSA services provided by TF-M.

    ITS
    Internal Trusted Storage
       One of PSA services provided by TF-M.

    TFM
    TF-M
        Trusted Firmware-M or Trusted Firmware for M-class.
        ARM TF-M provides a reference implementation of secure world software
        for ARMv8-M.

    TBSA-M
        Trusted Base System Architecture for Armv6-M, Armv7-M and Armv8-M.
        TBSA term. See `Trusted Base System Architecture for Armv6-M, Armv7-M
        and Armv8-M`_

    MPC
        Memory Protection Controller:
        Bus slave-side security controller for memory regions.

    PPC
        Peripheral Protection Controller:
        Bus slave-side security controller for peripheral access.

    S/NS
        Secure/Non-secure:
        The separation provided by TrustZone hardware components in the system.

    SAU
        Secure Attribution Unit:
        Hardware component providing isolation between Secure, Non-secure
        Callable and Non-secure addresses.

    AAPCS
        ARM Architecture Procedure Call Standard:
        The AAPCS defines how subroutines can be separately written, separately
        compiled, and separately assembled to work together. It describes a
        contract between a calling routine and a called routine

    MPU
        Memory Protection Unit:
        Hardware component providing privilege control.

    SVC
        SuperVisor Call:
        ARMv7M assembly instruction to call a privileged handler function

.. rubric:: Reference

| `PSA Firmware_Framework for M`_

.. _PSA Firmware_Framework for M: https://pages.arm.com/psa-resources-ff.html

.. _PSA term: `PSA Firmware_Framework for M`_

| `Trusted Base System Architecture for Armv6-M, Armv7-M and Armv8-M`_

.. _Trusted Base System Architecture for Armv6-M, Armv7-M and Armv8-M: https://pages.arm.com/psa-resources-tbsa-m.html

--------------

*Copyright (c) 2017-2020, Arm Limited. All rights reserved.*
