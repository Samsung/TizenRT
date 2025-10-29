##########################################
Mailbox Design in TF-M on Dual-core System
##########################################

:Author: David Hu
:Organization: Arm Limited
:Contact: david.hu@arm.com

************
Introduction
************

This document proposes a generic design of the mailbox communication for Trusted
Firmware-M (TF-M) on a dual-core system. The mailbox functionalities transfer
PSA Client requests and results between Non-secure Processing Environment (NSPE)
and Secure Processing Environment (SPE).

The dual-core system should satisfy the following requirements

- NSPE and SPE are properly isolated and protected following PSA specifications.
- An Arm M-profile core locates in SPE and acts as the Secure core.
- TF-M runs on the Secure core with platform specific drivers support.
- Inter-Processor Communication hardware module in system for communication
  between Secure core and Non-secure core. Mailbox communication calls the
  Inter-Processor Communication to transfer notifications.
- Non-secure memory shared by NSPE and SPE.

Scope
=====

This design document focuses on the mailbox functionalities in NSPE and SPE on a
dual-core system. The mailbox functionalities include the initialization of
mailbox in NSPE and SPE, and the transfer of PSA Client requests and replies
between NSPE and SPE.

Data types and mailbox APIs are defined in this document.

Some details of interactions between mailbox and other modules are specified in
other documents.
Communication prototype design [1]_ defines a general communication prototype
between NSPE and SPE on a dual-core system. It describes how TF-M interacts with
mailbox functionalities and the general requirements on mailbox.
Dual-core booting sequence [2]_ describes a synchronization step of mailbox
between NSPE and SPE during system booting.

Organization of the document
============================

- `Mailbox architecture`_ provides an overview on the mailbox architecture.
- `Mailbox communication for PSA Client calls`_ discusses about the mailbox
  communication for PSA Client calls.
- `Mailbox initialization`_ introduces the initialization of mailbox.
- `Mailbox APIs and data structures`_ lists mailbox data types and APIs.

********************
Mailbox architecture
********************

The mailbox consists of two parts sitting in NSPE and SPE respectively.
NSPE mailbox provides mailbox functionalities in NSPE and SPE mailbox provides
mailbox functionalities in TF-M in SPE.

PSA Client APIs called in NSPE are implemented by NSPE mailbox functions on
dual-core systems, to send PSA Client request and receive the result. The
implementation can vary in diverse NSPE OSs or use cases.

TF-M provides a reference implementation of NSPE mailbox. The NSPE mailbox
delivers the PSA Client requests to SPE mailbox. After the PSA Client result is
replied from SPE, NSPE mailbox fetches the result and returns it to PSA Client
APIs.

NSPE mailbox objects are managed by NSPE mailbox in non-secure memory to hold
PSA Client call parameters, return result and other mailbox data.
NSPE mailbox relies on platform specific Inter-Process Communication to process
notifications between NSPE and SPE.

The SPE mailbox in TF-M receives PSA Client requests from NSPE mailbox. It
parses the requests and invokes TF-M Remote Procedure Call (RPC) layer.
RPC layer delivers the requests to TF-M core/Secure Partition Manager (SPM).
After the PSA Client call is completed, TF-M core/SPM invokes RPC layer to
return results to NSPE, via SPE mailbox.
SPE mailbox objects are managed by SPE mailbox in secure memory.
SPE mailbox relies on platform specific Inter-Process Communication to process
notifications between SPE and NSPE.

The architecture is showed in following figure.

.. figure:: dual_core_mailbox_arch.png

******************************************
Mailbox communication for PSA Client calls
******************************************

This section describes the transfer of PSA Client request and reply between NSPE
and SPE via mailbox.

Mailbox objects
===============

This section lists the mailbox objects required in NSPE and SPE.

NSPE mailbox objects are managed by NSPE mailbox in non-secure memory. But NSPE
mailbox objects can be accessed by both NSPE mailbox and SPE mailbox.

SPE mailbox objects are managed by SPE mailbox in secure memory. SPE mailbox
objects should be protected from NSPE accesses by system specific isolation.

NSPE Mailbox queue
------------------

NSPE mailbox maintains a mailbox queue in non-secure memory. Please refer to the
structure definition in `NSPE mailbox queue structure`_.

NSPE mailbox queue contains one or more slots. The number of slots should be
aligned with that in SPE mailbox queue.

Each slot in NSPE mailbox queue consists of a pair of a mailbox message
structure and a mailbox reply structure. Each slot might contain additional
fields, such as identification of non-secure task which initiates the PSA Client
request. Each slot serves a PSA Client call from non-secure task.

The parameters of PSA Client request are hosted in the mailbox message inside
the slot. `Mailbox messages`_ describes the details of mailbox message.

The mailbox reply structure is used to receive the PSA Client result from SPE.
`Mailbox replies`_ describes the details of mailbox reply.

Mailbox messages
----------------

A mailbox message contains the parameters of a PSA Client request from a
non-secure task. Please refer to the structure definition in
`Mailbox message structure`_.

Inside PSA Client API implementation, NSPE mailbox selects an empty mailbox
queue slot for the PSA Client request. The parameters of that PSA Client request
are organized into the mailbox message belonging to the selected slot.
SPE mailbox will parse those parameters from the mailbox message.

More fields can be defined in mailbox message to transfer additional information
from NSPE to SPE for processing in TF-M.

Mailbox replies
---------------

A mailbox reply structure in non-secure memory receives the PSA Client result
replied from SPE mailbox. Please refer to the structure definition in
`Mailbox reply structure`_.

SPE Mailbox queue
-----------------

SPE mailbox maintains a mailbox queue to store SPE mailbox objects.
Please refer to the structure definition in `SPE mailbox queue structure`_.

SPE mailbox queue contains one or more slots. The number of slots should be
aligned with that in NSPE mailbox queue. After SPE is notified that a PSA Client
request is pending, SPE mailbox can assign an empty slot, copy the corresponding
PSA Client call parameters from non-secure memory to that slot and parse the
parameters.

Each slot in SPE mailbox queue can contain the following fields

- An optional field to hold mailbox message content copied from non-secure
  memory.
- Index of NSPE mailbox queue slot containing the mailbox message.
- A handle to the mailbox message. Optional. Identify the owner slot of PSA
  Client result when multiple mailbox messages are under processing.

More fields can be defined in the slot structure to support mailbox processing
in SPE.

Overall workflow
================

The overall workflow of transferring PSA Client requests and results between
NSPE and SPE via mailbox is shown below.

#. Non-secure task initiates a service request by calling PSA Developer APIs,
   which eventually invoke PSA Client APIs.
   PSA Client APIs call NSPE mailbox functions to transmit PSA Client call to
   SPE.

#. NSPE mailbox assigns an empty slot from NSPE mailbox queue for that PSA
   client call.

#. NSPE mailbox prepares the parameters of PSA Client call in the dedicated
   mailbox message inside the assigned slot.

#. After the mailbox message is ready, NSPE mailbox invokes platform specific
   Inter-Processor Communication driver to notify SPE.
   The notification mechanism of Inter-Processor Communication is platform
   specific.

#. After the notification is completed, non-secure task waits for the reply from
   SPE.

#. Platform specific Inter-Processor Communication interrupt for mailbox is
   asserted in SPE. The interrupt handler activates SPE mailbox to process the
   request(s).

#. During mailbox processing in TF-M, the handling routine can include the
   following steps:

    #. SPE mailbox checks and validates NSPE mailbox queue status.
    #. SPE mailbox fetches PSA Client call parameters from NSPE mailbox queue.
    #. SPE mailbox parses the parameters.
    #. SPE mailbox invokes the TF-M RPC APIs to deliver the PSA Client
       request to TF-M SPM.
    #. The PSA Client call is handled in TF-M SPM and target Secure Partition if
       necessary.

   If multiple ongoing mailbox messages are pending in the SPE, SPE mailbox can
   process mailbox messages one by one.

#. After the PSA Client call is completed, TF-M RPC layer notifies SPE mailbox
   to reply PSA Client result to NSPE.

#. SPE mailbox writes the PSA Client result to the dedicated mailbox reply
   structure in non-secure memory. The related SPE mailbox objects should be
   invalidated or cleaned.

#. SPE mailbox notifies NSPE by invoking Inter-Processor Communication driver to
   send a notification to NSPE.
   The notification mechanism of Inter-Processor Communication is platform
   specific.

#. NSPE mailbox is activated to handle the PSA Client result in the mailbox
   reply structure. Related mailbox objects should be invalidated or cleaned by
   NSPE mailbox after the return results is extracted out.

#. NSPE mailbox returns the result to PSA Client API implementation.
   The result is eventually returned to the non-secure task.

The following sections discuss more details of key steps in above sequence.

Mailbox notifications between NSPE and SPE
==========================================

As shown in `Overall workflow`_, NSPE mailbox asserts mailbox notification to
trigger SPE to handle PSA Client request. SPE mailbox asserts mailbox
notification to notify NSPE that PSA Client result is written. The notification
implementation is based on platform specific Inter-Processor Communication.

It is recommended to assign one independent set of Inter-Processor Communication
channel to each notification routine respectively, to implement a *full-duplex*
notification mechanism between NSPE and SPE.
If both notification routines share the same Inter-Processor Communication
channel, proper synchronization should be implemented to prevent conflicts
between two notification routines.

In SPE, the Inter-Processor Communication interrupt handler should deal with the
incoming notification from NSPE and activate the subsequent mailbox handling in
SPE. Communication prototype design [1]_ defines the behavior of Inter-Processor
Communication interrupt handler.

NSPE can implement an interrupt handler or a polling of notification status to
handle Inter-Processor Communication notification from SPE.

Implement PSA Client API with NSPE Mailbox
==========================================

PSA Client APIs are implemented with NSPE mailbox API
``tfm_ns_mailbox_client_call()``.

The pseudo code below shows a reference implementation of
``psa_framework_version()``.

.. code-block:: c

  uint32_t psa_framework_version(void)
  {
      ...
      int32_t ret;

      ret = tfm_ns_mailbox_client_call(...);
      if (ret != MAILBOX_SUCCESS) {
          version = PSA_VERSION_NONE;
      }

      ...
  }

``tfm_ns_mailbox_client_call()`` implementation can vary according to usage
scenario. TF-M reference implementation provides implementations for NS OS and
NS bare metal environment respectively. Refer to
`TF-M reference implementation of NSPE mailbox`_ for details.

As PSA Firmware Framework-M (FF-M) requests, a PSA Client API function should be
blocked until the result is returned. To comply with FF-M, NSPE mailbox requires
proper mechanism(s) to keep current caller waiting for PSA Client result or an
empty mailbox queue slot.

.. note::

  ``tfm_ns_mailbox_client_call()`` may trap the current exception in sleep and
  therefore it must not be called in interrupt service routine.

Refer to `Mailbox APIs and data structures`_ for details of
``tfm_ns_mailbox_client_call()``.

TF-M reference implementation of NSPE mailbox
=============================================

TF-M NS interface provides a reference implementation of NS mailbox.

This reference implementation defines several NS mailbox HAL APIs. Please refer
to `NSPE mailbox HAL APIs`_ for details.

Integration with NSPE
---------------------

TF-M reference implementation provides several mailbox build flags to control
the integration with NS software.

  .. _mailbox_os_flag:

  - ``TFM_MULTI_CORE_NS_OS``

    When integrating NS mailbox with NS OS, such as NS RTOS, that flag can be
    selected to enable NS OS support in NS mailbox, such as thread management
    to fulfill thread wait and wake-up.
    Please refer to `NSPE mailbox RTOS abstraction APIs`_ for NS OS support
    details.

    With NS OS support, multiple outstanding PSA Client calls can be supported
    in NS mailbox when number of mailbox queue slots configured in
    ``NUM_MAILBOX_QUEUE_SLOT`` is greater than 1.

   If ``TFM_MULTI_CORE_NS_OS`` is enabled, when a NS client starts a PSA Client
   call:

    - ``tfm_ns_mailbox_client_call()`` selects an empty NSPE mailbox queue slot
      to organize received PSA client call parameters into a mailbox message.

    - Then it sends those parameters to SPE mailbox and waits for results from
      SPE. During waiting for the result, the NS client thread may be switched
      out by NS OS scheduler.

    - When the result arrives, the NS client thread will be woken up inside
      NS mailbox interrupt handler.

    - The result is then written back to NS client finally.

    When that flag is disabled, NS mailbox runs as being integrated with NS bare
    metal environment. NS mailbox simply loops mailbox message status while
    waiting for results.

  .. _mailbox_os_thread_flag:

  - ``TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD``

    When ``TFM_MULTI_CORE_NS_OS`` is enabled, this flag can be selected to
    enable another NS mailbox thread model which relies on a NS mailbox
    dedicated thread.

    - It requires NS OS to create a dedicated thread to perform NS mailbox
      functionalities. This dedicated thread invokes
      ``tfm_ns_mailbox_thread_runner()`` to handle PSA Client calls.
      ``tfm_ns_mailbox_thread_runner()`` constructs mailbox messages and sends
      them to SPE mailbox.

    - ``tfm_ns_mailbox_client_call()`` sends PSA Client calls to the dedicated
      mailbox thread. It doesn't directly deal with mailbox messages.

    - It also relies on NS OS to provide thread management and inter-thread
      communication. Please refer to `NSPE mailbox RTOS abstraction APIs`_ for
      details.

    - It also requires dual-cpu platform to implement NS Inter-Processor
      Communication interrupts. The interrupt handler invokes
      ``tfm_ns_mailbox_wake_reply_owner_isr()`` to deal with PSA Client call
      replies and notify the waiting threads.

Multiple outstanding PSA Client call feature
--------------------------------------------

Multiple outstanding PSA Client call feature can enable dual-cpu platform to
issue multiple PSA Client calls in NS OS and those PSA Client calls can be
served simultaneously.

Without this feature, only a single PSA Client call can be issued and served.
A new PSA Client call cannot be started until the previous one is completed.

When multiple outstanding PSA Client call feature is enabled, while a NS
application is waiting for its PSA Client result, another NS application can be
switched in by NS OS to prepare another PSA Client call or deal with its PSA
client result. It can decrease the CPU idle time of waiting for PSA Client call
completion.

If multiple NS applications request secure services in NS OS, it is recommended
to enable this feature.

To implement this feature in NS OS:

  - Platform should set the number of mailbox queue slots in
    ``NUM_MAILBOX_QUEUE_SLOT`` in platform's ``config.cmake``.
    It will use more data area with multiple mailbox queue slots.

    NSPE and SPE share the same ``NUM_MAILBOX_QUEUE_SLOT`` value.

  - Enable ``TFM_MULTI_CORE_NS_OS``

    For more details, refer to
    :ref:`TFM_MULTI_CORE_NS_OS<mailbox_os_flag>`.

    ``TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD`` can be enabled to select another NS
    mailbox working model.
    See :ref:`TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD<mailbox_os_thread_flag>` for
    details.

Critical section protection between cores
=========================================

Proper protection should be implemented to protect the critical accesses to
shared mailbox resources. The critical sections can include atomic reading and
modifying NSPE mailbox queue status, slot status and other critical operations.

The implementation should protect a critical access to those shared mailbox
resource from corruptions caused by accesses from the peer core. SPE mailbox
also accesses NSPE mailbox queue. Therefore, it is essential to implement
synchronization or protection on NSPE mailbox queue between Secure core and
Non-secure core. NSPE mailbox and SPE mailbox define corresponding critical
section protection APIs respectively. The implementation of those APIs can be
platform specific. Please see more details in `NSPE mailbox APIs`_ and
`SPE mailbox APIs`_.

It is recommended to rely on both hardware and software to implement the
synchronization and protection.

Protection of local mailbox objects can be implemented as static functions
inside NSPE mailbox and SPE mailbox.

Mailbox handling in TF-M
========================

According to communication prototype design [1]_, mailbox implementation should
invoke ``tfm_rpc_register_ops()`` to hook its operations to TF-M RPC module
callbacks during initialization. Mailbox message handling should call TF-M RPC
PSA Client call handlers to deliver PSA Client request to TF-M SPM.

If multiple outstanding NS PSA Client calls should be supported, TF-M SPM can
store the mailbox message handle in a specific field in PSA message structure to
identify the mailbox message, while creating a PSA message. While replying the
PSA Client result, TF-M SPM can extract the mailbox message handle from PSA
message and pass it back to mailbox reply function. SPE mailbox can identify
which mailbox message is completed according to the handle and write the result
to corresponding NSPE mailbox queue slot.

Platform specific Inter-Processor Communication interrupt handler in SPE should
call ``spm_handle_interrupt()`` to notify SPM of the interrupt. SPM will then
send the ``SIGNAL_MAILBOX`` signal to the ``ns_agent_mailbox`` partition, which
will call ``tfm_rpc_client_call_handler()``.

**********************
Mailbox initialization
**********************

It should be guaranteed that NSPE mailbox should not initiate PSA Client request
until SPE mailbox initialization completes.
Refer to dual-core booting sequence [2]_ for more details on the synchronization
between NSPE and SPE during booting.

In current design, the base address of NSPE mailbox queue should be pre-defined
and shared between NSPE mailbox and SPE mailbox.

SPE mailbox initialization
==========================

The SPE mailbox queue memory should be allocated before calling
``tfm_mailbox_init()``. ``tfm_mailbox_init()`` initializes the memory and
variables.
``tfm_mailbox_init()`` calls ``tfm_mailbox_hal_init()`` to perform platform
specific initialization. The base address of NSPE mailbox queue can be
received via ``tfm_mailbox_hal_init()``.

SPE mailbox dedicated Inter-Processor Communication initialization can also be
enabled during SPE mailbox initialization.

After SPE mailbox initialization completes, SPE notifies NSPE that SPE mailbox
functionalities are ready.

NSPE mailbox initialization
===========================

The NSPE mailbox queue memory should be allocated before calling
``tfm_ns_mailbox_init()``. ``tfm_ns_mailbox_init()`` initializes the memory and
variables.
``tfm_ns_mailbox_init()`` calls ``tfm_ns_mailbox_hal_init()`` to perform
platform specific initialization. The base address of NSPE mailbox queue can be
passed to SPE mailbox via ``tfm_ns_mailbox_hal_init()``.

NSPE mailbox dedicated Inter-Processor Communication initialization can also be
enabled during NSPE mailbox initialization.

********************************
Mailbox APIs and data structures
********************************

Data types
==========

Constants
---------

``MAILBOX_SUCCESS``
^^^^^^^^^^^^^^^^^^^

``MAILBOX_SUCCESS`` is a generic return value to indicate success of mailbox
operation.

.. code-block:: c

  #define MAILBOX_SUCCESS        (0)

``MAILBOX_QUEUE_FULL``
^^^^^^^^^^^^^^^^^^^^^^

``MAILBOX_QUEUE_FULL`` is a return value from mailbox function if mailbox queue
is full.

.. code-block:: c

  #define MAILBOX_QUEUE_FULL     (INT32_MIN + 1)

``MAILBOX_INVAL_PARAMS``
^^^^^^^^^^^^^^^^^^^^^^^^

``MAILBOX_INVAL_PARAMS`` is a return value from mailbox function if any
parameter is invalid.

.. code-block:: c

  #define MAILBOX_INVAL_PARAMS   (INT32_MIN + 2)

``MAILBOX_NO_PERMS``
^^^^^^^^^^^^^^^^^^^^

``MAILBOX_NO_PERMS`` is a return value from mailbox function if the caller
doesn't own a proper permission to execute the operation.

.. code-block:: c

  #define MAILBOX_NO_PERMS       (INT32_MIN + 3)

``MAILBOX_NO_PEND_EVENT``
^^^^^^^^^^^^^^^^^^^^^^^^^

``MAILBOX_NO_PEND_EVENT`` is a return value from mailbox function if the
expected event doesn't occur yet.

.. code-block:: c

  #define MAILBOX_NO_PEND_EVENT  (INT32_MIN + 4)

``MAILBOX_CHAN_BUSY``
^^^^^^^^^^^^^^^^^^^^^

``MAILBOX_CHAN_BUSY`` is a return value from mailbox function if the underlying
Inter-Processor Communication resource is busy.

.. code-block:: c

  #define MAILBOX_CHAN_BUSY      (INT32_MIN + 5)

``MAILBOX_CALLBACK_REG_ERROR``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``MAILBOX_CALLBACK_REG_ERROR`` is a return value from mailbox function if the
registration of mailbox callback functions failed.

.. code-block:: c

  #define MAILBOX_CALLBACK_REG_ERROR     (INT32_MIN + 6)

``MAILBOX_INIT_ERROR``
^^^^^^^^^^^^^^^^^^^^^^

``MAILBOX_INIT_ERROR`` is a return value from mailbox function if the mailbox
initialization failed.

.. code-block:: c

  #define MAILBOX_INIT_ERROR     (INT32_MIN + 7)

``MAILBOX_GENERIC_ERROR``
^^^^^^^^^^^^^^^^^^^^^^^^^

``MAILBOX_GENERIC_ERROR`` indicates mailbox generic errors which cannot be
indicated by the codes above.

.. code-block:: c

  #define MAILBOX_GENERIC_ERROR    (INT32_MIN + 8)

PSA Client API types
^^^^^^^^^^^^^^^^^^^^

The following constants define the PSA Client API type values shared between
NSPE and SPE

.. code-block:: c

  #define MAILBOX_PSA_FRAMEWORK_VERSION       (0x1)
  #define MAILBOX_PSA_VERSION                 (0x2)
  #define MAILBOX_PSA_CONNECT                 (0x3)
  #define MAILBOX_PSA_CALL                    (0x4)
  #define MAILBOX_PSA_CLOSE                   (0x5)

Mailbox message structure
-------------------------

``psa_client_params_t`` lists the parameters passed from NSPE to SPE required by
a PSA Client call.

.. code-block:: c

  struct psa_client_params_t {
      union {
          struct {
              uint32_t        sid;
          } psa_version_params;

          struct {
              uint32_t        sid;
              uint32_t        minor_version;
          } psa_connect_params;

          struct {
              psa_handle_t    handle;
              int32_t         type;
              const psa_invec *in_vec;
              size_t          in_len;
              psa_outvec      *out_vec;
              size_t          out_len;
          } psa_call_params;

          struct {
              psa_handle_t    handle;
          } psa_close_params;
      };
  };

The following structure describe a mailbox message and its members.

- ``call_type`` indicates the PSA Client API type.
- ``params`` stores the PSA Client call parameters.
- ``client_id`` records the client ID of the non-secure client. Optional.
  It is used to identify the non-secure tasks in TF-M when NSPE OS enforces
  non-secure task isolation.

.. code-block:: c

  struct mailbox_msg_t {
      uint32_t                     call_type;
      struct psa_client_params_t   params;

      int32_t                      client_id;
  };

Mailbox reply structure
-----------------------

This structure describes a mailbox reply structure, which is managed by NSPE
mailbox in non-secure memory.

.. code-block:: c

  struct mailbox_reply_t {
      int32_t    return_val;
      const void *owner;
      int32_t    *reply;
      uint8_t    *woken_flag;
  };

Mailbox queue status bitmask
----------------------------

``mailbox_queue_status_t`` defines a bitmask to indicate a status of slots in
mailbox queues.

.. code-block:: c

  typedef uint32_t   mailbox_queue_status_t;

NSPE mailbox queue structure
----------------------------

``ns_mailbox_slot_t`` defines a non-secure mailbox queue slot.

.. code-block:: c

  /* A single slot structure in NSPE mailbox queue */
  struct ns_mailbox_slot_t {
      struct mailbox_msg_t   msg;
      struct mailbox_reply_t reply;
  };

``ns_mailbox_queue_t`` describes the NSPE mailbox queue and its members in
non-secure memory.

- ``empty_slots`` is the bitmask of empty slots.
- ``pend_slots`` is the bitmask of slots whose PSA Client call is not replied
  yet.
- ``replied_slots`` is the bitmask of slots whose PSA Client result is returned
  but not extracted yet.
- ``queue`` is the NSPE mailbox queue of slots.
- ``is_full`` indicates whether NS mailbox queue is full.

.. code-block:: c

  struct ns_mailbox_queue_t {
      mailbox_queue_status_t   empty_slots;
      mailbox_queue_status_t   pend_slots;
      mailbox_queue_status_t   replied_slots;

      struct ns_mailbox_slot_t queue[NUM_MAILBOX_QUEUE_SLOT];

      bool                     is_full;
  };

SPE mailbox queue structure
---------------------------

``secure_mailbox_slot_t`` defines a single slot structure in SPE mailbox queue.

- ``ns_slot_idx`` records the index of NSPE mailbox slot containing the mailbox
  message under processing. SPE mailbox determines the reply structure address
  according to this index.
- ``msg_handle`` contains the handle to the mailbox message under processing.
  The handle can be delivered to TF-M SPM while creating PSA message to identify
  the mailbox message.

.. code-block:: c

  /* A handle to a mailbox message in use */
  typedef int32_t    mailbox_msg_handle_t;

  struct secure_mailbox_slot_t {
      uint8_t              ns_slot_idx;
      mailbox_msg_handle_t msg_handle;
  };

``secure_mailbox_queue_t`` describes the SPE mailbox queue in secure memory.

- ``empty_slots`` is the bitmask of empty slots.
- ``queue`` is the SPE mailbox queue of slots.
- ``ns_queue`` stores the address of NSPE mailbox queue structure.
- ``cur_proc_slot_idx`` indicates the index of mailbox queue slot currently
  under processing.

.. code-block:: c

  struct secure_mailbox_queue_t {
      mailbox_queue_status_t       empty_slots;

      struct secure_mailbox_slot_t queue[NUM_MAILBOX_QUEUE_SLOT];
      /* Base address of NSPE mailbox queue in non-secure memory */
      struct ns_mailbox_queue_t    *ns_queue;
      uint8_t                      cur_proc_slot_idx;
  };

NSPE mailbox APIs
=================

NSPE mailbox interface APIs
---------------------------

APIs defined in this section are called by NS software and PSA Client APIs
implementations.

``tfm_ns_mailbox_init()``
^^^^^^^^^^^^^^^^^^^^^^^^^

This function initializes NSPE mailbox.

.. code-block:: c

  int32_t tfm_ns_mailbox_init(struct ns_mailbox_queue_t *queue);

**Parameters**

+-----------+-----------------------------------------+
| ``queue`` | The base address of NSPE mailbox queue. |
+-----------+-----------------------------------------+

**Return**

+---------------------+------------------------------------------+
| ``MAILBOX_SUCCESS`` | Initialization succeeds.                 |
+---------------------+------------------------------------------+
| Other return codes  | Initialization fails with an error code. |
+---------------------+------------------------------------------+

**Usage**

``tfm_ns_mailbox_init()`` invokes ``tfm_ns_mailbox_hal_init()`` to complete
platform specific mailbox and Inter-Processor Communication initialization.
The non-secure memory area for NSPE mailbox queue structure should be statically
or dynamically pre-allocated before calling ``tfm_ns_mailbox_init()``.

``tfm_ns_mailbox_client_call()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function sends the PSA Client request to SPE, waits and fetches PSA Client
result.

.. code-block:: c

  int32_t tfm_ns_mailbox_client_call(uint32_t call_type,
                                     const struct psa_client_params_t *params,
                                     int32_t client_id,
                                     int32_t *reply);

**Parameters**

+---------------+--------------------------------------------------+
| ``call_type`` | Type of PSA Client call                          |
+---------------+--------------------------------------------------+
| ``params``    | Address of PSA Client call parameters structure. |
+---------------+--------------------------------------------------+
| ``client_id`` | ID of non-secure task.                           |
+---------------+--------------------------------------------------+
| ``reply``     | The NS client task private buffer written with   |
|               | PSA Client result                                |
+---------------+--------------------------------------------------+

**Return**

+---------------------+--------------------------------------------+
| ``MAILBOX_SUCCESS`` | PSA Client call is completed successfully. |
+---------------------+--------------------------------------------+
| Other return code   | Operation failed with an error code.       |
+---------------------+--------------------------------------------+

**Usage**

If ``TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD`` is enabled,
``tfm_ns_mailbox_client_call()`` will forward PSA Client calls to the dedicated
mailbox thread via NS OS message queue.
Otherwise, ``tfm_ns_mailbox_client_call()`` directly deals with PSA Client calls
and perform NS mailbox functionalities.

``tfm_ns_mailbox_thread_runner()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function handles PSA Client call inside a dedicated NS mailbox thread.
It constructs mailbox messages and transmits them to SPE mailbox.

.. code-block:: c

  void tfm_ns_mailbox_thread_runner(void *args);

**Parameters**

+----------+-------------------------------------------------------------+
| ``args`` | The pointer to the structure of PSA Client call parameters. |
+----------+-------------------------------------------------------------+

**Usage**

``tfm_ns_mailbox_thread_runner()`` should be executed inside the dedicated
mailbox thread.

.. note::

  ``tfm_ns_mailbox_thread_runner()`` is implemented as an empty function when
  ``TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD`` is disabled.

``tfm_ns_mailbox_wake_reply_owner_isr()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function wakes up the owner task(s) of the returned PSA Client result(s).

.. code-block:: c

  int32_t tfm_ns_mailbox_wake_reply_owner_isr(void);

**Return**

+---------------------------+--------------------------------------------+
| ``MAILBOX_SUCCESS``       | The tasks of replied mailbox messages were |
|                           | found and wake-up signals were sent.       |
+---------------------------+--------------------------------------------+
| ``MAILBOX_NO_PEND_EVENT`` | No replied mailbox message is found.       |
+---------------------------+--------------------------------------------+
| Other return codes        | Operation failed with an error code        |
+---------------------------+--------------------------------------------+

**Usage**

``tfm_ns_mailbox_wake_reply_owner_isr()`` should be called from platform
specific Inter-Processor Communication interrupt handler.

.. note::

  ``tfm_ns_mailbox_wake_reply_owner_isr()`` is implemented as a dummy function
  when ``TFM_MULTI_CORE_NS_OS`` is disabled.

NSPE mailbox HAL APIs
---------------------

The HAL APIs defined in this section should be implemented by platform-specific
implementation.

This section describes a *reference design* of NSPE mailbox HAL APIs. Developers
can define and implement different APIs.

``tfm_ns_mailbox_hal_init()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function executes platform-specific NSPE mailbox initialization.

.. code-block:: c

  int32_t tfm_ns_mailbox_hal_init(struct ns_mailbox_queue_t *queue);

**Parameters**

+-----------+-----------------------------------------+
| ``queue`` | The base address of NSPE mailbox queue. |
+-----------+-----------------------------------------+

**Return**

+---------------------+------------------------------------------+
| ``MAILBOX_SUCCESS`` | Initialization succeeds.                 |
+---------------------+------------------------------------------+
| Other return codes  | Initialization fails with an error code. |
+---------------------+------------------------------------------+

**Usage**

``tfm_ns_mailbox_hal_init()`` performs platform specific mailbox and
Inter-Processor Communication initialization. ``tfm_ns_mailbox_hal_init()`` can
also share the address of NSPE mailbox queue with SPE mailbox via platform
specific implementation.

``tfm_ns_mailbox_hal_notify_peer()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function invokes platform specific Inter-Processor Communication drivers to
send notification to SPE.

.. code-block:: c

  int32_t tfm_ns_mailbox_hal_notify_peer(void);

**Return**

+---------------------+---------------------------------------+
| ``MAILBOX_SUCCESS`` | The operation completes successfully. |
+---------------------+---------------------------------------+
| Other return codes  | Operation fails with an error code.   |
+---------------------+---------------------------------------+

**Usage**

``tfm_ns_mailbox_hal_notify_peer()`` should be implemented by platform specific
Inter-Processor Communication drivers.

``tfm_ns_mailbox_hal_notify_peer()`` should not be exported outside NSPE
mailbox.

``tfm_ns_mailbox_hal_enter_critical()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function enters the critical section of NSPE mailbox queue access.

.. code-block:: c

  void tfm_ns_mailbox_hal_enter_critical(void);

**Usage**

NSPE mailbox invokes ``tfm_ns_mailbox_hal_enter_critical()`` before entering
critical section of NSPE mailbox queue.
``tfm_ns_mailbox_hal_enter_critical()`` implementation is platform specific.

``tfm_ns_mailbox_hal_enter_critical()`` should not be called in any interrupt
service routine.

``tfm_ns_mailbox_hal_exit_critical()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function exits the critical section of NSPE mailbox queue access.

.. code-block:: c

  void tfm_ns_mailbox_hal_exit_critical(void);

**Usage**

NSPE mailbox invokes ``tfm_ns_mailbox_hal_exit_critical()`` after exiting
critical section of NSPE mailbox queue.
``tfm_ns_mailbox_hal_exit_critical()`` implementation is platform specific.

``tfm_ns_mailbox_hal_exit_critical()`` should not be called in any interrupt
service routine.

``tfm_ns_mailbox_hal_enter_critical_isr()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function enters the critical section of NSPE mailbox queue access in an
IRQ handler.

.. code-block:: c

  void tfm_ns_mailbox_hal_enter_critical(void);

**Usage**

NSPE mailbox invokes ``tfm_ns_mailbox_hal_enter_critical_isr()`` before entering
critical section of NSPE mailbox queue in an IRQ handler.
``tfm_ns_mailbox_hal_enter_critical_isr()`` implementation is platform specific.

``tfm_ns_mailbox_hal_exit_critical_isr()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function exits the critical section of NSPE mailbox queue access in an IRQ
handler

.. code-block:: c

  void tfm_ns_mailbox_hal_exit_critical_isr(void);

**Usage**

NSPE mailbox invokes ``tfm_ns_mailbox_hal_exit_critical_isr()`` after exiting
critical section of NSPE mailbox queue in an IRQ handler.
``tfm_ns_mailbox_hal_exit_critical_isr()`` implementation is platform specific.

NSPE mailbox RTOS abstraction APIs
----------------------------------

The APIs defined in this section should be implemented by RTOS-specific
implementation when ``TFM_MULTI_CORE_NS_OS`` is enabled.

.. note::

  If ``TFM_MULTI_CORE_NS_OS`` is set to ``OFF``, the following APIs are defined
  as dummy functions or empty functions.

``tfm_ns_mailbox_os_lock_init()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function initializes the multi-core lock for synchronizing PSA client
call(s). The actual implementation depends on the non-secure use scenario.

.. code-block:: c

  int32_t tfm_ns_mailbox_os_lock_init(void);

**Return**

+---------------------------+---------------------------+
| ``MAILBOX_SUCCESS``       | Initialization succeeded. |
+---------------------------+---------------------------+
| ``MAILBOX_GENERIC_ERROR`` | Initialization failed.    |
+---------------------------+---------------------------+

**Usage**

``tfm_ns_mailbox_init()`` invokes this function to initialize the lock.
If ``TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD`` is enabled,
``tfm_ns_mailbox_os_lock_init()`` is defined as a dummy one.

``tfm_ns_mailbox_os_lock_acquire()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function acquires the multi-core lock for synchronizing PSA client call(s).
The actual implementation depends on the non-secure use scenario.

.. code-block:: c

  int32_t tfm_ns_mailbox_os_lock_acquire(void);

**Return**

+---------------------------+--------------------------------+
| ``MAILBOX_SUCCESS``       | Succeeded to acquire the lock. |
+---------------------------+--------------------------------+
| ``MAILBOX_GENERIC_ERROR`` | Failed to acquire the lock.    |
+---------------------------+--------------------------------+

**Usage**

``tfm_ns_mailbox_client_call()`` invokes this function to acquire the lock when
``TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD`` is disabled
If ``TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD`` is enabled,
``tfm_ns_mailbox_os_lock_acquire()`` is defined as a dummy one.

``tfm_ns_mailbox_os_lock_release()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function releases the multi-core lock for synchronizing PSA client call(s).
The actual implementation depends on the non-secure use scenario.

.. code-block:: c

  int32_t tfm_ns_mailbox_os_lock_release(void);

**Return**

+---------------------------+--------------------------------+
| ``MAILBOX_SUCCESS``       | Succeeded to release the lock. |
+---------------------------+--------------------------------+
| ``MAILBOX_GENERIC_ERROR`` | Failed to release the lock.    |
+---------------------------+--------------------------------+

**Usage**

``tfm_ns_mailbox_client_call()`` invokes this function to release the lock when
``TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD`` is disabled
If ``TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD`` is enabled,
``tfm_ns_mailbox_os_lock_release()`` is defined as a dummy one.

``tfm_ns_mailbox_os_get_task_handle()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function gets the handle of the current non-secure task executing mailbox
functionalities.

.. code-block:: c

  void *tfm_ns_mailbox_os_get_task_handle(void);

**Return**

+-------------+-----------------------------------------------------------+
| Task handle | The non-secure task handle waiting for PSA Client result. |
+-------------+-----------------------------------------------------------+

``tfm_ns_mailbox_os_wait_reply()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function performs use scenario and NS OS specific waiting mechanism to wait
for the reply of the specified mailbox message to be returned from SPE.

.. code-block:: c

  void tfm_ns_mailbox_os_wait_reply(void);

**Usage**

The PSA Client API implementations call ``tfm_ns_mailbox_os_wait_reply()`` to
fall into sleep to wait for PSA Client result.

``tfm_ns_mailbox_os_wake_task_isr()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function wakes up the dedicated task which is waiting for PSA Client
result, via RTOS-specific wake-up mechanism.

.. code-block:: c

  void tfm_ns_mailbox_hal_wait_reply(const void *task_handle);

**Parameters**

+-----------------+----------------------------------------+
| ``task_handle`` | The handle to the task to be woken up. |
+-----------------+----------------------------------------+

``tfm_ns_mailbox_os_mq_create()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function creates and initializes a NS OS message queue.

.. code-block:: c

  void *tfm_ns_mailbox_os_mq_create(ize_t msg_size, uint8_t msg_count);

**Parameters**

+---------------+------------------------------------------+
| ``msg_size``  | The maximum message size in bytes.       |
+---------------+------------------------------------------+
| ``msg_count`` | The maximum number of messages in queue. |
+---------------+------------------------------------------+

**Return**

+----------------------+-----------------------------------------------------+
| message queue handle | The handle of the message queue created, or NULL in |
|                      | case of error.                                      |
+----------------------+-----------------------------------------------------+

**Usage**

If ``TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD`` is disabled,
``tfm_ns_mailbox_os_mq_create()`` is defined as a dummy one.

``tfm_ns_mailbox_os_mq_send()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function sends PSA Client call request via NS OS message queue.

.. code-block:: c

  int32_t tfm_ns_mailbox_os_mq_send(void *mq_handle,
                                    const void *msg_ptr);

**Parameters**

+---------------+----------------------------------------+
| ``mq_handle`` | The handle of message queue.           |
+---------------+----------------------------------------+
| ``msg_ptr``   | The pointer to the message to be sent. |
+---------------+----------------------------------------+

**Return**

+---------------------+-------------------------------------+
| ``MAILBOX_SUCCESS`` | The message is successfully sent.   |
+---------------------+-------------------------------------+
| Other return code   | Operation fails with an error code. |
+---------------------+-------------------------------------+

**Usage**

If ``TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD`` is disabled,
``tfm_ns_mailbox_os_mq_send()`` is defined as a dummy one.

``tfm_ns_mailbox_os_mq_receive()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function receives PSA Client call requests via NS OS message queue.

.. code-block:: c

  int32_t tfm_ns_mailbox_os_mq_receive(void *mq_handle,
                                       void *msg_ptr);

**Parameters**

+---------------+---------------------------------------------------+
| ``mq_handle`` | The handle of message queue.                      |
+---------------+---------------------------------------------------+
| ``msg_ptr``   | The pointer to buffer for message to be received. |
+---------------+---------------------------------------------------+

**Return**

+---------------------+-------------------------------------+
| ``MAILBOX_SUCCESS`` | A message is successfully received. |
+---------------------+-------------------------------------+
| Other return code   | Operation fails with an error code. |
+---------------------+-------------------------------------+

**Usage**

The buffer size must be large enough to contain the request whose size is set
in ``msg_size `` in ``tfm_ns_mailbox_os_mq_create()``.

If ``TFM_MULTI_CORE_NS_OS_MAILBOX_THREAD`` is disabled,
``tfm_ns_mailbox_os_mq_receive()`` is defined as a dummy one.

.. note::

  The function caller should be blocked until a PSA Client call request is
  received from message queue, unless a fatal error occurs.

SPE mailbox APIs
================

SPE mailbox interface APIs
--------------------------

The APIs defined in this section are called in TF-M routines and platform
specific secure interrupt handler.

``tfm_mailbox_handle_msg()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function completes the handling of mailbox messages from NSPE.

.. code-block:: c

  int32_t tfm_mailbox_handle_msg(void);

**Return**

+---------------------+---------------------------------------+
| ``MAILBOX_SUCCESS`` | The operation completes successfully. |
+---------------------+---------------------------------------+
| Other return codes  | Operation fails with an error code.   |
+---------------------+---------------------------------------+

**Usage**

``tfm_mailbox_handle_msg()`` is registered to RPC callback function
``handle_req``.

``tfm_mailbox_handle_msg()`` executes the following tasks:

- Check NSPE mailbox queue status.
- Copy mailbox message(s) from NSPE. Optional.
- Checks and validations if necessary
- Parse mailbox message
- Call TF-M RPC APIs to pass PSA Client request to TF-M SPM.

``tfm_mailbox_reply_msg()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function replies the PSA Client result to NSPE.

.. code-block:: c

  int32_t tfm_mailbox_reply_msg(mailbox_msg_handle_t handle, int32_t reply);

**Parameters**

+------------+-----------------------------------------------------------------+
| ``handle`` | The handle to mailbox message related to the PSA Client result. |
+------------+-----------------------------------------------------------------+
| ``reply``  | The PSA Client result value to be replied.                      |
+------------+-----------------------------------------------------------------+

**Return**

+---------------------+---------------------------------------+
| ``MAILBOX_SUCCESS`` | The operation completes successfully. |
+---------------------+---------------------------------------+
| Other return codes  | Operation fails with an error code.   |
+---------------------+---------------------------------------+

**Usage**

``tfm_mailbox_reply_msg()`` is registered to RPC callback ``reply``.
It is invoked inside handler of ``psa_reply()`` to return the PSA Client result
to NSPE.

``handle`` determines which mailbox message in SPE mailbox queue contains the
PSA Client call. If ``handle`` is set as ``MAILBOX_MSG_NULL_HANDLE``, the return
result is replied to the mailbox message in the first SPE mailbox queue slot.

``tfm_mailbox_init()``
^^^^^^^^^^^^^^^^^^^^^^

This function initializes SPE mailbox.

.. code-block:: c

  int32_t tfm_mailbox_init(void);

**Return**

+---------------------+-------------------------------------------+
| ``MAILBOX_SUCCESS`` | Initialization succeeds.                  |
+---------------------+-------------------------------------------+
| Other return codes  | Initialization failed with an error code. |
+---------------------+-------------------------------------------+

**Usage**

``tfm_mailbox_init()`` invokes ``tfm_mailbox_hal_init()`` to execute platform
specific initialization.

SPE mailbox HAL APIs
--------------------

``tfm_mailbox_hal_notify_peer()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function invokes platform specific Inter-Processor Communication drivers to
send notification to NSPE.

.. code-block:: c

  int32_t tfm_mailbox_hal_notify_peer(void);

**Return**

+---------------------+---------------------------------------+
| ``MAILBOX_SUCCESS`` | The operation completes successfully. |
+---------------------+---------------------------------------+
| Other return codes  | Operation fails with an error code.   |
+---------------------+---------------------------------------+

**Usage**

``tfm_mailbox_hal_notify_peer()`` should be implemented by platform specific
Inter-Processor Communication drivers.

``tfm_mailbox_hal_notify_peer()`` should not be exported outside SPE mailbox.


``tfm_mailbox_hal_init()``
^^^^^^^^^^^^^^^^^^^^^^^^^^

This function is implemented by platform support in TF-M. It completes platform
specific mailbox initialization, including receiving the the address of NSPE
mailbox queue and Inter-Processor Communication initialization.

.. code-block:: c

  int32_t tfm_mailbox_hal_init(struct secure_mailbox_queue_t *s_queue);

**Parameters**

+-------------+----------------------------------------+
| ``s_queue`` | The base address of SPE mailbox queue. |
+-------------+----------------------------------------+

**Return**

+---------------------+-------------------------------------------+
| ``MAILBOX_SUCCESS`` | Initialization succeeds.                  |
+---------------------+-------------------------------------------+
| Other return codes  | Initialization failed with an error code. |
+---------------------+-------------------------------------------+

``tfm_mailbox_hal_enter_critical()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function enters the critical section of NSPE mailbox queue access in SPE.

.. code-block:: c

  void tfm_mailbox_hal_enter_critical(void);

**Usage**

SPE mailbox invokes ``tfm_mailbox_hal_enter_critical()`` before entering
critical section of NSPE mailbox queue.
``tfm_mailbox_hal_enter_critical()`` implementation is platform specific.

``tfm_mailbox_hal_enter_critical()`` can be called in an interrupt service
routine.

``tfm_mailbox_hal_exit_critical()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function exits from the critical section of NSPE mailbox queue access in
SPE.

.. code-block:: c

  void tfm_mailbox_hal_exit_critical(void);

**Usage**

SPE mailbox invokes ``tfm_mailbox_hal_exit_critical()`` when exiting from
critical section of NSPE mailbox queue.
``tfm_mailbox_hal_exit_critical()`` implementation is platform specific.

``tfm_mailbox_hal_exit_critical()`` can be called in an interrupt service
routine.

*********
Reference
*********

.. [1] :doc:`Communication prototype between NSPE and SPE in Dual-core systems <./communication_prototype_between_nspe_and_spe_in_dual_core_systems>`

.. [2] :doc:`Booting a Dual-core system <booting_a_dual_core_system>`

--------------------

*Copyright (c) 2019-2021 Arm Limited. All Rights Reserved.*
*Copyright (c) 2022 Cypress Semiconductor Corporation. All rights reserved.*
