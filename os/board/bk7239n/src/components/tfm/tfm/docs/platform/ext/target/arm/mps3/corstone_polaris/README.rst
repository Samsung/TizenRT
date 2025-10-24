Corstone-Polaris
================

Introduction
------------

Corstone-Polaris is an Arm reference subsystem for secure System on
Chips containing an Armv8.1-M Olympus processor and an Ethos-U55 neural
network processor. It is an MPS3 based platform with the usual MPS3
peripherals.

This platform port supports all TF-M regression tests (Secure and Non-secure)
with Isolation Level 1 and 2, but building without BL2 is not supported.

Building TF-M
-------------

Follow the instructions in :doc:`Building instructions </technical_references/instructions/tfm_build_instruction>`.
Build instructions with platform name: arm/mps3/corstone_polaris (-DTFM_PLATFORM=arm/mps3/corstone_polaris).

*Copyright (c) 2021, Arm Limited. All rights reserved.*
