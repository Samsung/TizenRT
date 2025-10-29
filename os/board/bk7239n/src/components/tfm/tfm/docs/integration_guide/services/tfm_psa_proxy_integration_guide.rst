#####################################
PSA Proxy Partition Integration Guide
#####################################

************
Introduction
************
TF-M PSA Proxy partition is responsible for forwarding all the PSA RoT messages
to a Secure Enclave, this way virtually providing all the PSA RoT services.
Proxy can only be used in IPC model, for context and design details please
check the
:doc:`Secure Enclave design document </technical_references/design_docs/secure_enclave_solution>`.

Currently to forward the PSA Client call parameters Proxy must read them with
``psa_read`` into a memory area shared with the Secure Enclave. (Similarily
``psa_write`` is used to give back the results to the caller.) By default this
memory is allocated from BSS, but if that is not accessible to the Secure
Enclave other memory area can be used. To communicate with the Secure Enclave
the mailbox solution is used, and Proxy uses the Non-secure side of mailbox.
(The secure side of the mailbox is handled by the Secure Enclave.)

***************************************
Current PSA Proxy partition limitations
***************************************
- Client IDs are not forwarded to Secure Enclave. For Non-secure clients this
  is straightforward, but for calls coming from other secure partitions the IDs
  must be translated to a negative value. The reason is all clients on Host
  are treated as non-secure from Secure Enclave's point of view. (This is the
  cause why Protected Storage messages also forwarded. Protected Storage uses
  Internal Trusted Storage partition to manage the PS flash area. But as client
  IDs are not forwarded the ITS partition running on Secure Enclave can not
  know whether should work on ITS or PS flash.)
- Sending of the mailbox messages is a blocking call in Proxy, so control is
  not given back to Host's SPM while waiting for Secure Enclave's answer.
- Only one message can be put into the mailbox at a time.
- Current platform partition provides Non Volatile (NV) counter, System Reset,
  and IOCTL services. But while NV counters and System Reset shall be provided
  by the Secure Enclave, IOCTL probably shall be provided by Host, as the
  underlaying HW probably placed in Host subsystem. So the current platform
  partition should be split into two halves by conditional compilation, and
  Proxy should forward only the calls provided by Secure Enclave.
- PSA Proxy can only get the IPC parameters by PSA read, so the parameters need
  to be copied to a shared memory, because the partition cannot forward the
  original pointers. This copy might be omitted on platforms where Secure
  Enclave has access to all Host memory areas, if all security risks are
  addressed. Note that IOVECs shall be verified by Host's SPM and sent to SE
  with the mailbox message.

**************
Code Structure
**************
PSA Proxy partition code is located in ``secure_fw/partitions/psa_proxy/``.
As Proxy can be treated as an alternative implementation of all the PSA RoT
services, the Secure and Non-secure interface implementations of the forwarded
services are reused without modification.

Files
=====
- ``psa_proxy.c`` - Handles IPC messages and manages communication with the
  Secure Enclave.

- ``psa_proxy_shared_mem_mngr.c`` - Responsible to manage the shared memory
  area used to share the input and output parameters with Secure Enclave.

*****************
Integration Guide
*****************
- Non-secure mailbox interface must be provided.
- Shared memory must be configured:

  - If Secure Enclave can access TF-M's BSS section it is enough to set the
    area's size by the ``SHARED_BUFFER_SIZE`` macro.
  - If a special memory region must be used as the shared memory the
    ``PSA_PROXY_SHARED_MEMORY_BASE`` and ``PSA_PROXY_SHARED_MEMORY_SIZE``
    macros must be set. (Not just for compilation but for linking as well,
    becuase these macros used in the linker script/scatter file too.)

- If memories are mapped to different addresses for Host and Secure Enclave
  address translation can be turned on by setting
  ``PSA_PROXY_ADDR_TRANSLATION`` macro and implementing the interface defined
  by ``platform/include/tfm_plat_psa_proxy_addr_trans.h`` header.

--------------

*Copyright (c) 2020-2021, Arm Limited. All rights reserved.*
