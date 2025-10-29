########################
Secure Partition Manager
########################
This document describes the Secure Partition Manager(`SPM`) implementation
design in Trusted Firmware-M (`TF-M`).

.. note::
  - The FF-M in this document refers to the accumulated result of two
    specifications:
    `FF-M v1.1 Update <https://developer.arm.com/documentation/aes0039/latest>`_
    on
    `FF-M v1.0 <https://developer.arm.com/-/media/Files/pdf/PlatformSecurityArchitecture/Architect/DEN0063-PSA_Firmware_Framework-1.0.0-2.pdf?revision=2d1429fa-4b5b-461a-a60e-4ef3d8f7f4b4&la=en&hash=BE8C59DBC98212591E1F935C2312D497011CD8C7>`_.
  - The words marked as `interpreted` are defined terms. Find the terms in
    referenced documents if it is not described in this document.

************
Introduction
************
The service access process of FF-M:

.. figure:: media/tfmdev.*
    :align: center
    :name: fig-tfmdev
    :width: 80%

    FF-M service access process

Secure services (aka `Service`) is the component providing secure
functionalities in `SPE`, and `Client` is the user of the `Service`. A service
act as a client when it is accessing its depending services.

Services are grouped into `Secure Partition` (aka `partition`). A partition:

- Contains services with the same purpose.
- Provides implementation required isolation boundaries.
- Is a software development unit.

Each service exposes its `Service ID` (`SID`) and `Handle` for client access
usage. Clients access services by `SID` or `Handle` through FF-M `Client API`.
Partitions use FF-M `Secure Partition API` when it needs to operate on client
data or reply to a client.

`SPM` is the centre of an FF-M compliant implementation, which set up and
maintains a firmware framework that:

- Implements `Client API` and `Secure Partition API`.
- Manages partition runtime to follow FF-M.
- Involves necessary implementation-defined items to support the
  implementation.

SPM interfaces are consist of these two categories:

- FF-M defined API.
- Extended API to support the implementation.

Both API categories are compliant with FF-M concepts and guidelines. The core
concept of TF-M SPM surrounds the FF-M defined service management and access
process. Besides this, another important implementation part is partition
runtime management.

Partition runtime model
=======================
One partition must work under as `ONE` of the runtime models:
`Inter-process communication` (`IPC`) model or `Secure Function` (`SFN`)
model.

A partition that runs under the `IPC` model looks like a classic `process`.
There is `ONE` thread inside the partition keeps waiting for signals. SPM
converts the service accessing info from the `Client API` call into messages
and assert a signal to the partition. The partition calls corresponded service
function indicated by the signal and its bound message, and reply service
returned result to the client. The advantages of this model:

- It provides better isolation by limiting the interfaces on data interactive.
  Data are preferred to be processed in a local buffer.
- It provides a mechanism for handling multiple service access. There is no
  memory mapping mechanism in the MCU system, hence it is hard to provide
  multiple function call contexts when serving multiple-threaded clients if
  the service access is implemented in a function-call based mechanism. This
  model converts multiple service accesses into messages, let the partition
  handles the service access in messages one by one.

The `Secure Function` (`SFN`) model partition is close to a `library`. Each
service is provided as a function entry inside the partition. SPM launches
the target service function after the service is found. The whole procedure
(from client to service function) is a function call. This model:

- Saves the workloads spent on `IPC` scheduling.

Meanwhile, it relaxes the data interactive mechanism, for example, allow
direct memory access (MMIOVEC). And it is hard to enable multiple-threaded
clients service access because of multiple call context-maintenance
difficulties.

An implementation contains only `SFN` partitions fits better in the
resource-constrained devices, it is called an `SFN model implementation`. And
it is an `IPC model implementation` when `IPC` partitions exist in the system.

.. note::
  `IPC model implementation` can handle access to the services in the `SFN`
  partition.

Components and isolation levels
===============================
There are `THREE` isolation levels defined in `FF-M`. These levels can
fulfil different security requirements by defining different isolation
boundaries.

.. figure:: media/modelisolation.*
    :align: center
    :name: fig-modelisolation
    :width: 80%

    Components and isolation boundaries

.. note::
  Concept `ARoT`, `PRoT`, `domain`, and boundaries are in the `FF-M`
  specification.

Not like an `SPE` client that can call `Client API` to access the secure
services in one step, an `NSPE` client needs to cross the secure boundaries
first before calling `Client API`. The component `NS Agent` in
:numref:`fig-modelisolation` represents `NSPE` clients after they crossed
the secure boundaries. This could help `SPM` handles the request in a
unified way instead of care about the special boundaries.

.. note::
  `NS Agent` is a necessary implementation-defined component out of FF-M
  specification. `NS Agent` has a dedicated stack because secure and
  non-secure can not share the stack. It also has dedicated execution bodies.
  For example, RPC-based `NS Agent` has a while loop that keeps waiting for
  messages; and Trustzone-based `NS Agent` has veneer code to take over `NSPE`
  secure call. This makes `NS Agent` is a component more like a `process`.
  Hence in the simplest implementation (`SFN model implementation` mentioned
  above), `NS Agent` is the only process in the system, the scheduling
  logic can be extremely simplified since no other process execution needs to
  be scheduled. But the scheduling interface is still necessary to SPM, this
  could help SPM treat both `SFN` and `IPC` model implementation in a unified
  way.

  Check `NS Agent`_ for details.

Implementation principle
========================
The principles for TF-M SPM implementation:

.. important::
  - SPM can treat these components as the client: NS Agent, SFN Partition,
    and IPC partition.
  - These components can provide services: SFN Partition, IPC partition, and
    built-in services. A built-in service is built up with SPM together.
  - All partition services must be accessed by `Client API`.
  - Partitions interact with client data by `Secure Partition API`.
  - Built-in services are strongly recommended to be accessed by `Client API`.
    Customized interfaces are restricted.
  - Built-in services can call SPM internal interfaces directly.

******************
Runtime management
******************
The runtime execution runs among the components, there are **4** runtime
states:

- `Initializing` state, to set up the SPM runtime environment after system
  powers up
- `IDLE` state, when SPM runtime environment is set up and partitions are
  ready for service access.
- `Serving` state, when partition is under initializing or service access
  handling.
- `Background` state, such as the arrival of secure interrupt or unexpected
  faults. `Background` state returns to the state it preempts. `Background`
  state can be nested.

The state transition diagram:

.. figure:: media/spestate.*
    :align: center
    :name: fig-spestate
    :width: 70%

    SPE runtime execution states

Initializing
============
The goal of TF-M initializing is to perform necessary initialization and
move to the `Serving`_ state. This state starts with platform-specific power
on sequence, then `SPM` takes over the execution to perform these operations:

#. A preparation initialization process before SPM runtime initialization.
#. SPM runtime initialization.
#. A post initialization happens after the SPM runtime initialization and
   before the first partition gets launched.

.. note::
  These procedures and their sub-routines are recommended to be applied with
  execution measurement mechansim to mitigate the `Hardware Fault Injection`
  attack.

Preparation initialization
--------------------------
The purpose of this preparation initialization is to provide a chance for
performing those security required but generic platform power-on skipped
operations, such as:

- Restrict `SPM` execution, for example, set up memory overflow settings for
  SPM runtime memory, or set code out of SPM as un-executable, even though
  SPM is a privileged component in general.

.. note::
  The ``logging``-related peripheral can be set up **AT THIS STEP**, if
  logging is enabled and it needs peripheral support. There is no standalone
  initializing HAL API proposed for logging, so here is an ideal place for
  initializing them.

This procedure is abstracted into one `HAL`, and a few example procedures
are implemented as its sub-routines for reference:

- Architecture extensions initialization, Check chapter
  `Architecture security settings`_ for detailed information.
- Isolation and lifecycle initialization.

The load isolation boundaries need to be set up here, such as SPE/NSPE
boundary, and ARoT/PRoT boundary if isolation level 2 is applied.

The lifecycle is initiated by a secure bootloader usually. And in this stage
of SPM initializing, SPM double-checks the lifecycle set up status (following
a specific lifecycle management guidelines). Note that the hardware debugger
setting can be part of lifecycle settings.

.. important::
  Double-check debugger setting when performing a product release.

SPM runtime initialization
--------------------------
This procedure initializes necessary runtime operations such as memory
allocator, loading partitions and partition-specific initialization
(binding partitions with platform resources).

The general processes:

#. Initialize runtime functionalities, such as memory allocator.
#. Load partitions by repeating below steps:

    * Find a partition load information.
    * Allocate runtime objects for this partition.
    * Link the runtime objects with load information.
    * Init partition contexts (Thread and call context).
    * Init partition isolation boundaries (MMIO e.g.).
    * Init partition interrupts.

After no more partitions to be loaded, the SPM runtime is set up but
partitions' initialization routines have not run yet - the partition runtime
context is initialized for the routine call.

The partition initialization routine is a special service that serves SPM
only, because:

- SPM needs to call the initialization routine, just like it calls into
  the service routine.
- The partition initialization routine can access its depending services.
  Putting initialization routine in the same runtime environment as common
  service routines can avoid special operations.

Hence a `Partition initialization client` needs to be created to initialize
the SFN partitions, because:

- `SPM runtime initialization` happen inside a special runtime environment
  compare to the partition runtime execution, then an environment switching
  is needed.
- IPC partitions are initialized by the scheduler and dependencies are
  handled by signals and messages asynchronously, hence IPC partitions can
  process the dependencies by their own.

The `Partition initialization client` is created differently based on the
implementation runtime model:

- A SFN client is created under the SFN model implementation.
- A IPC client is created under the IPC model implementation. This client
  thread has the highest priority.

As the other partitions, the client is created with context standby, and it
is executed after the `Post initialization`_ stage.

Post initialization
-------------------
Platform code can change specific partition settings in this procedure before
partitions start. A few SPM API is callable at this stage, such as set a
signal into a specific partition, or customized peripheral settings.

Serving
=======
Two execution categories work under this state:

- `Partition initialization routine execution`_.
- `Secure service access`_.

This state indicates the serving is ongoing. It is mainly the service routine
execution, plus a few SPM executions when SPM API gets called.

.. important::
  The service access process introduce in this chapter
  (Such as `Secure service access`_) is abstracted from the FF-M
  specification. Reference the FF-M specification for the details of each
  step.

Partition initialization routine execution
------------------------------------------
The partition initialization routines get called. One partition may access its
depending services during initializing, then this procedure is a
`Secure service access`_.

The initialization routine gets called initially by
`Partition initialization client`, also can be called by Client API before
service access, if the target partition is not initialized but a service
access request is raised by one client.

Secure service access
---------------------
The process of service access:

#. A `client` calls an FF-M Client API.
#. `SPM` validates inputs and looks up for the targeted service.
#. `SPM` constructs the request to be delivered under a proper runtime
   mechanism.
#. The target service gets executed. It can perform internal executions or
   access depending services to prepare the response. It also can wait for
   specific signals.
#. The target service calls FF-M Secure Partition API to request a reply to
   the client.
#. SPM delivers the response to the client, and the API called by the client
   returns.

The mechanism of how SPM interact with the target partition depends on the
partition runtime model.

- Access to a service in an SFN partition is a function call, which does not
  switch the current process indicator.
- Access to a service in an IPC partition leads to scheduling, which switches
  the current process indicator.
- When the execution roams between components because of a function call or
  scheduling, the isolation boundaries NEED to be switched if there are
  boundaries between components.

.. figure:: media/hybridruntime.*
  :align: center
  :name: fig-hybridruntime
  :width: 60%

No matter what kind of partition a client is trying to access, the SPM API is
called firstly as it is the interface for service access. There are two ABI
types when calling SPM API: Cross-boundary or No-cross-boundary.

Calling SPM API
---------------
SPM is placed in the PRoT domain. It MAY have isolation boundaries under
particular isolation levels. For example:

- There are boundaries between ARoT components and SPM under isolated level 2
  and 3.

Then API SPM provided needs to support the function call (no boundary
switching) and cross-boundary call. A direct call reaches the API entrance
directly, while a cross-boundary call needs a mechanism (Supervisor call e.g.)
to cross the boundary first before reaching the API entrance.

.. figure:: media/twocalltypes.*
    :align: center
    :name: fig-twocalltypes
    :width: 60%

    SPM call types

SPM internal execution flow
---------------------------
SPM internal execution flow as shown in diagram:

.. figure:: media/abi_scheduler.*
    :align: center
    :name: fig-abi_scheduler
    :width: 60%

    SPM API runtime

The process:

- PSA API gets called by one of the ABI mentioned in the last chapter as
  `ABI 1` in the diagram.
- The unified API Handler calls FF-M and backend subroutines in sequence.
- The `FF-M` subroutine performs `FF-M` defined operations.
- The backend operations perform target partition runtime model decided
  operations. For example, enqueue message into the target partition under
  the IPC runtime model, or prepare to call context with the message as the
  parameters under the SFN runtime model.
- API Handler triggers different ABI based on the result of the backends.

The API handler:

- Can process the `PROGRAMMER_ERROR` in a unified place.
- Can see the prepared caller and callee context, with exited SPM context. It
  is an ideal place for subsequent operations such as context switching.

A example code:

.. code-block:: c

  void abi(void *p)
  {
      status = spm_api(p);
      /*
       * Now both the caller and calle context are
       * managed by spm_api.
       */
      if (status == ACTION1) {
          /*
           * Check if extra operations are required
           * instead of a direct return.
           */
          exit_action1();
      }
  }

The explanation about `Scheduler Lock`:

Some FF-M API runs as a generic thread to prevent long time exclusive
execution. When a preemption happens, a new partition thread can call SPM API
again, makes SPM API nested. It needs extra memory in SPM to be allocated to
store the preempted context. Lock the scheduler while SPM API is executing can
ensure SPM API complete execution after preemption is handled. There can be
multiple ways to lock the scheduler:

- Set a scheduler lock.
- Set SPM API thread priority as the highest.

Backend service messaging
-------------------------
A message to service is created after the target service is found and the
target partition runtime model is known. The preparation before ABI triggers
the final accessing:

- The message is pushed into partition memory under a specific ABI mechanism
  if the target partition model is `SFN` and there are boundaries between SPM
  and the target partition. After this, requests a specific call type to the
  SPM ABI module.
- The target service routine is get called with the message parameter if
  there are no boundaries between SPM and the target partition and the
  partition runtime is `SFN`.
- The message is queued into the partition message list if the target
  partition runtime model is `IPC`.
- IPC partition replies to the client by `psa_reply`, which is another SPM API
  call procedure.
- SFN partition return triggers an implied `psa_reply`, which is also another
  SPM API call procedure.

.. note::
  The backends also handle the isolation boundary switching.

Sessions and contexts
---------------------
FF-M API allows multiple sessions for a service if the service is classic
connection-based. The service can maintain multiple local session data and use
`rhande` in the message body to identify which client this session is bound
with.

But this does not mean when an ongoing service accessing is preempted,
another service access request can get a chance for new access. This is
because of the limited context storage - supporting multiple contexts in a
common service costs much memory, and runtime operations(allocation and
re-location). Limited the context content in the stack only can mitigate the
effort, but this requirement requires too much for the service development.

The implementation-decisions are:

- IPC partitions handles messages one by one, the client get blocked before
  the service replying to the client.
- The client is blocked when accessing services are handling a service
  request in an SFN partition.

ABI type summary
----------------
The interface type is decided by the runtime model of the target component.
Hence PSA API has two types of ABI: `Cross-boundary ABI` and
`Function call ABI`. After SPM operations, one more component runtime type
shows up: The IPC partition, hence `schedule` is the mechanism when accessing
services inside an IPC partition.

.. figure:: media/spmabitypes.*
    :align: center
    :name: fig-spmabi
    :width: 60%

    ABI types

.. note::
  The API that does not switch context returns directly, which is not
  covered in the above diagram.

IDLE state
==========
The `IDLE state` can be represented by the `NS Agent` action:

- Launching NSPE software (Trustzone case, e.g.), or send a signal to NSPE
  software (RPC case, e.g.).

It is because `NS Agent` is the last component being initialized in the
system. Its execution indicates other partitions' initialization has
accomplished.

Background state
================
Background execution can happen at any time when the arrival of interrupts or
execution faults. An ongoing background execution indicates the state is a
`Background state`. The characteristics:

- The background state has a higher execution priority than other states -
  other states stall when the background state is executing.
- Background execution can be nested. For example, an
  interrupt handler can preempt an ongoing interrupt execution.
- Particular partition code can be involved in the background state, for
  example, the `First Level Interrupt Handler (FLIH)` of one partition.
- Background state MUST return to the state it preempts.

.. note::
  Interrupt handling is a common background state example. Check Interrupt
  design document for details.

******************************
Practical implementation items
******************************
This chapter describes the practical implementation contents.

.. important::
  Arm M-profile architecture is the default hardware architecture when
  describing architecture-specific items.

The general M-profile programming is not involved in this document. The
following chapters introduce the mandatory settings for security
requirements.

Architecture security settings
==============================
When an `Armv8m Security Extension` (Aka `Trustzone-M`) is available in the
system, these settings are required to be set:

- The MSPLIM needs to be set correctly to prevent stack overflow.
- The exception handler priority needs to be decided.
- Boost the secure handler mode priority to prevent NSPE from preempting SPE
  handler mode execution(`AIRCR.PRIS`).
- Disable NSPE hardware faults when a secure fault is happening. Trap in the
  secure fault with the highest priority can be a valid option.
- Push seals on the stack top when a stack is allocated (`TFMV-1`). Also
  check `Stack seal`_ chapter for details.

Besides `Armv8m Security Extension`, these settings need to care when
`Floatpoint Extension` is enabled for partition usage:

- `FPCCR.TS`, `FPCCR.CLRONRET` and `FPCCR.CLRONRETS` need to be set when
  booting.
- `CPACR.CP10` and `CPACR.CP11` need to be set when booting.

.. important::
  Floatpoint usage is prohibited in SPM and background execution.

Stack seal
----------
When Trustzone-M is applied, the architecture specification recommends sealing
the secure stack by:

- Push two `SEAL` values (`0xFEF5EDA5`) at the stack bottom, when a stack is
  allocated.
- Push two `SEAL` values on the stack pointer which is going to be switched
  out.

Check architecture specification and vulnerability `TFMV-1` for details.

Trustzone-M reentrant
---------------------
The Trustzone-M has characteristics that:

- SPE keeps the last assigned stack pointer value when execution leaves SPE.
- SPE execution can be preempted by NSPE which causes an execution left.

It is possible that NSPE preemption caused a second thread calls into SPE and
re-uses the secure stack contains the first thread's context, which obviously
causes information leakage and runtime state inconsistent.

Armv8.1-M provides the hardware setting `CCR_S.TRD` to prevent the reentrant.
On an Armv8.0-M architecture, extra software logic needs to be added at the
veneer entry:

- Check if the local stack points to a `SEAL` when veneer code get executed.

.. code-block:: c

  /* This is a theoretical code that is not in a real project. */
  veneer() {
      content = get_sp_value();
      if (context != SEAL) /* Error if reentrant detected */
          error();
  }

SPM Runtime ABI
===============
This chapter describes the runtime implementation of SPM.

Scheduling
----------
The scheduling logic is put inside the PendSV mode. PendSV mode's priority
is set as one level higher than the default thread mode priority. If
`Trustzone-M` is present, the priority is set as the lowest just above NS
exception priority to prevent a preemption in secure exceptions.

PendSV is an ideal place for scheduling logic, because:

- An interrupt triggered scheduling during PendSV execution lead to another
  PendSV execution before exception return to the thread mode, which can find
  the latest run-able thread.

Function call ABI
-----------------
In the diagram :numref:`fig-abi_scheduler`, the ABI can have two basic
types: cross-boundary and direct call (No-cross-boundary).

When applying `SVCall` (`SVC`) as the cross-boundary mechanism, the
implementation can be straight like:

- The SVC handler calls SPM internal routines, and eventually back to the
  handler before an exit.

Under the IPC model implementation, to re-use `ABI 2` in `No-cross-boundary`,
a software ABI needs to be provided.

While under the SFN model plus isolation level 1, both `ABI 1` and `ABI 2` can
be a direct function call.

NS Agent
========
The `NS Agent`(`NSA`) forwards NSPE service access request to SPM. It is a
special `partition` that:

- It does not provide FF-M aligned secure services.
- It runs with the second-lowest priority under `IPC model implementation`
  (The IDLE thread has the lowest priority).
- It has isolation boundaries and an individual stacks.
- It requires specific services and mechanisms compared to common partitions.

There are two known types for NS Agent:

- Trustzone-M based.
- Remote Procedure Call (RPC) based.

This process is put inside the ARoT domain, to prevent assign unnecessary
PRoT permissions to the NSPE request parsing logic.

Trustzone-M specific
--------------------
The functionalities of a Truszone-M specific NSA is:

- Launch NSPE when booting.
- Wait in the veneer code, and get executed when NSPE accesses services.

As there may be multiple NSPE threads calling into SPE, and SPM wants to
identify them, special mechanisms can be proposed to provide the identification.
Check specific NS ID client ID or context related documents for details.

.. figure:: media/tzcontext.*
    :align: center
    :name: fig-tzcontext
    :width: 40%

    TZ NSA and specific service

RPC specific
------------
Compare to Trustzone-M NSA, RPC NSA looks closer to a generic partition:

- It has a message loop, keep waiting for RPC events.
- It converts received RPC events into FF-M API call to target services.

And compared to generic partitions, the differences are:

- It parses RPC messages to know which NSPE thread is accessing services.
  Hence it needs special interfaces to help SPM to identify the NSPE clients.
- It needs to check NSPE client memory and map to local before calling SPM API.
- It cannot be blocked during API calls, which affects handling the RPC
  requests.

Partition
=========
A partition is a set of services in the same scope. Services are generally
implemented as functions, and the partition exposes the services in different
ways bases on the partition model: `IPC` or `SFN`.

A partition build generates these outputs:

- A partition load information, used by SPM.
- A partition program containing service interface and logic, typically a
  library.
- An optional service API set for easier client usage, by encapsulating
  the low-level `FF-M` Client API. These API needs to be integrated
  into client space.

Partition loading
-----------------
SPM needs to set up runtime objects to manage partitions by parsing the load
information of partitions. In general, the partition load information is
stored in a const memory are can be random read directly, hence SPM can direct
link runtime objects to the load information without a copy operation. This
is called a `Static Load` mechanism.

Each partition has different numbers of dependencies and services, this makes
the load information size of each partition is different, it would be hard
to put such variable size elements in an array. The solution here is putting
these elements in a dedicated section, for SPM enumerating while loading.
Each partition can define variable size load information type bases on the
common load info type.

The common load information:

.. code-block:: c

  struct partition_load_info_t {
      uint32_t        psa_ff_ver;     /* Encode the version with magic    */
      int32_t         pid;            /* Partition ID                     */
      uint32_t        flags;          /* ARoT/PRoT, SFN/IPC, priority     */
      uintptr_t       entry;          /* Entry point                      */
      size_t          stack_size;     /* Stack size                       */
      size_t          heap_size;      /* Heap size                        */
      uint32_t        ndeps;          /* Dependency number                */
      uint32_t        nservices;      /* Service number                   */
      uint32_t        nassets;        /* Asset numbers                    */
      uint32_t        nirqs;          /* Number of IRQ owned by Partition */
  };

  And the example for a specific partition load info:
  struct partition_example_load_info_t {
      struct partition_load_info_t ldi;      /* Common info info          */
      uint32_t                     deps[10]; /* Dependencies              */
      /*                    ... other infos ...                           */
  };

Peripheral binding
------------------
A partition can declare multiple peripherals (Interrupts are part of
peripherals). The peripherals binding process:

- The tooling references symbols in a fixed pattern in the partition load
  information.
- The HAL implementation needs to provide the symbols being referenced.
- SPM calls HAL API to bind the partition info with devices When the partition
  gets loading.
- The platform HAL acknowledges the binding if validation pass on SPM given
  load information.

***************************
Integration and development
***************************
These modules are expected to be object/library level modularised, each
module should be generated into object/library at build time:

.. list-table:: Object level modularization
    :header-rows: 1
    :widths: 40 60

    * - Name
      - Description
    * - SPM
      - All SPM related modules such as SPM, system, and so on.
    * - Platform
      - Platform sources are switchable.
    * - Services and Secure Partition
      - These items should be standalone.
    * - Service Runtime Library
      - This is a shared runtime library.

HAL
===
The HAL here mainly refers to the SPM HAL. The SPM HAL implementation is
running with the same privilege level and hardware mode with SPM. The
implementation is object level modularized with SPM.

Check the `HAL` design document for details.

Configurations
==============
The same TF-M code base is flexible to address different implementation
requirements, from the simplest device with isolation level 1 to the most
complicated device with isolation level 3 and optional isolation rules.

These configurations are set by switches, during the build time, as runtime
support costs extra resources. The common configurations are named `profile`.
There are several profiles defined.

*******
History
*******

.. list-table:: Revision
    :header-rows: 1
    :widths: 20 80

    * - Date
      - Description
    * - 2021 Apr-Sep
      - Updated to cover the implementation for `FF-M v1.1` features.
    * - 2018
      - Created as 'TF-M Inter-Process Communication' which is deprecated as
        this document covers whole SPM content.

--------------

*Copyright (c) 2021, Arm Limited. All rights reserved.*
