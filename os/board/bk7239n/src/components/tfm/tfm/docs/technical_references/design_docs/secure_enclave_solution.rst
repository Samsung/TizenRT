##############################################
Secure Enclave solution for Trusted Firmware-M
##############################################

:Author: Mark Horvath
:Organization: Arm Limited
:Contact: Mark Horvath <mark.horvath@arm.com>

********
Abstract
********

This document summarizes the design goals and one possible implementation
of the TF-M Secure Enclave solution.

************
Introduction
************

If a separate subsystem can provide the PSA Root of Trust (RoT) in a system
then an additional physical separation exists between the most trusted and
other domains. In such a system at least two subsystems are present, a Secure
Enclave (SE) whose only task is to provide PSA RoT and an application core
where any other application specific functionality can be placed. The latter
core (or cores) are referred as *Host* in this document.

The current design assumes that Host is a v8-m core with security extension.

************
Requirements
************

- Secure Enclave shall implement secure boot-flow (start-up first at reset and
  validate its own and the Host image or images before release Host from reset)
- Secure Enclave shall provide the PSA RoT services
- Host shall provide not just the non-secure context but the Application RoT as
  well
- It shall be transparent to the (secure or non-secure) applications running on
  host whether the RoT services are provided by the same subsystem or by a
  Secure Enclave.

.. Note::

   In comparison, in a Dual Core system the whole secure context is placed on a
   separate subsystem, while a Secure Enclave only implements the PSA RoT
   security domain.

***************
Proposed design
***************

As the clients and the services are running on different cores only the IPC
model can be used on both Secure Enclave and Host.

Secure Enclave
==============

To provide the required functionality it is enough to run the current PSA RoT
secure partitions on the Secure Enclave, so no need for non-secure context
there. (It is enough if the Secure Enclave's architecture is v6-m, v7-m or v8-m
without the security extension.)

Secure Enclave can treat all clients running on Host as non-secure (even the
services running on Host's secure side). This means that fom Secure Enclave's
point of view all Host images, Host's RAM and shared memory between Host and
Secure Enclave if present are treated as non-secure. (Just like in the Dual CPU
solution.) But the clients need to be distinguished, otherwise some
functionalities are not working, for example:

- Protected Storage partition shall run on Host, but the PS area is handled by
  Internal Trusted Storage partition (running on Secure Enclave). ITS partition
  decides whether it should work on PS or ITS assets by checking the client ID.
- If a secure partition on host creates a crypto key, no other client shall be
  able to destroy it.

Communication
=============

To communicate between Host and Secure Enclave, the existing mailbox solution
can be reused as it is.

Host
====

On Host the current TF-M software architecture can be placed to provide
non-secure context and Application RoT domain.

One solution to forward a PSA RoT IPC message from a client running on Host to
the Secure Enclave is to add a proxy partition to the secure side. This PSA
Proxy partition can provide all the RoT services to the system by forwarding
the messages over the mailbox solution.

If the new partition's manifest contains all the PSA RoT service IDs SPM will
deliver all IPC messages there. Then the messages just must be blindly copied
into the mailbox. PSA proxy can use the non-secure interface of the mailbox,
but it is placed on the secure side of Host. (From SE's point of view this is
in fact the non-secure side of the mailbox as whole Host is treated as
non-secure.)

It is important to verify IOVECs before forwarding them to SE, otherwise a
malicous actor could use SE to access a memory area otherwise unaccessable. If
PSA proxy uses the current secure partition interface then this is ensured by
Host's SPM.

SE treats all clients of Host as non-secure, so all PSA messages shall have a
negative client ID when pushed into SE's SPM. This is given for the clients on
the non-secure side of Host, but the secure side clients of Host have positive
client IDs. The straightforward solution is to translate the positive client
IDs into a predefined negative range in PSA proxy, and push the translated
values into the mailbox. Of course this range shall be reserved for this use
only and no clients on non-secure side of Host shall have client ID from this
range.

To avoid blocking Host when a message is sent PSA Proxy shall handle the
service requests in non-blocking mode. And to maximize bandwidth PSA Proxy
shall be able to push new messages into the mailbox, while others still not
answered. To achieve these the mailbox interrupts needs to be handled in the
PSA Proxy partition.

--------------

*Copyright (c) 2020-2022, Arm Limited. All rights reserved.*
