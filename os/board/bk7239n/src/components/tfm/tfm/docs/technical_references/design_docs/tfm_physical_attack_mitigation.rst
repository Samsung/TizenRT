#################################################
Physical attack mitigation in Trusted Firmware-M
#################################################

:Authors: Tamas Ban; David Hu
:Organization: Arm Limited
:Contact: tamas.ban@arm.com; david.hu@arm.com

************
Requirements
************
PSA Certified Level 3 Lightweight Protection Profile [1]_ requires protection
against physical attacks. This includes protection against manipulation of the
hardware and any data, undetected manipulation of memory contents, physical
probing on the chip's surface. The RoT detects or prevents its operation outside
the normal operating conditions (such as voltage, clock frequency, temperature,
or external energy fields) where reliability and secure operation has not been
proven or tested.

.. note::

  Mitigation against certain level of physical attacks is a mandatory
  requirement for PSA Level 3 certification.
  The :ref:`tf-m-against-physical-attacks` discussed below
  doesn't provide mitigation against all the physical attacks considered in
  scope for PSA L3 certification. Please check the Protection Profile document
  for an exhaustive list of requirements.

****************
Physical attacks
****************
The goal of physical attacks is to alter the expected behavior of a circuit.
This can be achieved by changing the device's normal operating conditions to
untested operating conditions. As a result a hazard might be triggered on the
circuit level, whose impact is unpredictable in advance but its effect can be
observed. With frequent attempts, a weak point of the system could be identified
and the attacker could gain access to the entire device. There is a wide variety
of physical attacks, the following is not a comprehensive list rather just give
a taste of the possibilities:

  - Inject a glitch into the device power supply or clock line.
  - Operate the device outside its temperature range: cool down or warm it up.
  - Shoot the chip with an electromagnetic field. This can be done by passing
    current through a small coil close to the chip surface, no physical contact
    or modification of the PCB (soldering) is necessary.
  - Point a laser beam on the chip surface. It could flip bits in memory or a
    register, but precise knowledge of chip layout and design is necessary.

The required equipment and cost of these attacks varies. There are commercial
products to perform such attacks. Furthermore, they are shipped with a scripting
environment, good documentation, and a lot of examples. In general, there is a
ton of videos, research paper and blogs about fault injection attacks. As a
result the threshold, that even non-proficient can successfully perform such
attack, gets lower over time.

*****************************************************************
Effects of physical attacks in hardware and in software execution
*****************************************************************
The change in the behavior of the hardware and software cannot be seen in
advance when performing a physical attack. On circuit-level they manifest
in bit faults. These bit faults can cause varied effects in the behavior of
the device micro-architecture:

  - Instruction decoding pipeline is flushed.
  - Altering instructions when decoding.
  - Altering data when fetching or storing.
  - Altering register content, and the program counter.
  - Flip bits in register or memory.

These phenomenons happen at random and cannot be observed directly but the
effect can be traced in software execution. On the software level the following
can happen:

  - A few instructions are skipped. This can lead to taking different branch
    than normal.
  - Corrupted CPU register or data fetch could alter the result of a comparison
    instruction. Or change the value returned from a function.
  - Corrupted data store could alter the config of peripherals.
  - Very precise attacks with laser can flip bits in any register or in memory.

This is a complex domain. Faults are not well-understood. Different fault models
exist but all of them target a specific aspect of fault injection. One of the
most common and probably the easily applicable fault model is the instruction
skip.

***********************************
Mitigation against physical attacks
***********************************
The applicability of these attacks highly depends on the device. Some
devices are more sensitive than others. Protection is possible at hardware and
software levels as well.

On the hardware level, there are chip design principles and system IPs that are
resistant to fault injection attacks. These can make it harder to perform a
successful attack and as a result the chip might reset or erase sensitive
content. The device maker needs to consider what level of physical attack is in
scope and choose a SoC accordingly.

On top of hardware-level protection, a secondary protection layer can be
implemented in software. This approach is known as "defence in depth".

Neither hardware nor software level protection is perfect because both can be
bypassed. The combination of them provides the maximum level of protection.
However, even when both are in place, it is not certain that they provide 100%
protection against physical attacks. The best of what is to achievable to harden
the system to increase the cost of a successful attack (in terms of time and
equipment), thereby making it non profitable to perform them.

.. _phy-att-countermeasures:

Software countermeasures against physical attacks
=================================================
There are practical coding techniques which can be applied to harden software
against fault injection attacks. They significantly decrease the probability of
a successful attack:

  - Control flow monitor

    To catch malicious modification of the expected control flow. When an
    important portion of a program is executed, a flow monitor counter is
    incremented. The program moves to the next stage only if the accumulated
    flow monitor counter is equal to an expected value.

  - Default failure

    The return value variable should always contain a value indicating
    failure. Changing its value to success is done only under one protected
    flow (preferably protected by double checks).

  - Complex constant

    It is hard to change a memory region or register to a pre-defined value, but
    usual boolean values (0 or 1) are easier to manipulate.

  - Redundant variables and condition checks

    To make branch condition attack harder it is recommended to check the
    relevant condition twice (it is better to have a random delay between the
    two comparisons).

  - Random delay

    Successful fault injection attacks require very precise timing. Adding
    random delay to the code execution makes the timing of an attack much
    harder.

  - Loop integrity check

    To avoid to skip critical loop iterations. It can weaken the cryptographic
    algorithms. After a loop has executed, check the loop counter whether it
    indeed has the expected value.

  - Duplicated execution

    Execute a critical step multiple times to prevent fault injection from
    skipping the step. To mitigate multiple consecutive fault injections, random
    delay can be inserted between duplicated executions.

These techniques should be applied in a thoughtful way. If it is applied
everywhere then it can result in messy code that makes the maintenance harder.
Code must be analysed and sensitive parts and critical call path must be
identified. Furthermore, these techniques increase the overall code size which
might be an issue on the constrained devices.

Currently, compilers are not providing any support to implement these
countermeasures automatically. On the contrary, they can eliminate the
protection code during optimization. As a result, the C level protection does
not add any guarantee about the final behavior of the system. The effectiveness
of these protections highly depends on the actual compiler and the optimization
level. The compiled assembly code must be visually inspected and tested to make
sure that proper countermeasures are in-place and perform as expected.

.. _phy-att-threat-model:

******************************************
TF-M Threat Model against physical attacks
******************************************

Physical attack target
======================
A malicious actor performs physical attack against TF-M to retrieve assets from
device. These assets can be sensitive data, credentials, crypto keys. These
assets are protected in TF-M by proper isolation.

For example, a malicious actor can perform the following attacks:

  - Reopen the debug port or hinder the closure of it then connect to the device
    with a debugger and dump memory.
  - Bypass secure boot to replace authentic firmware with a malicious image.
    Then arbitrary memory can be read.
  - Assuming that secure boot cannot be bypassed then an attacker can try to
    hinder the setup of the memory isolation hardware by TF-M
    :term:`Secure Partition Manager` (SPM) and manage to execute the non-secure
    image in secure state. If this is achieved then still an exploitable
    vulnerability is needed in the non-secure code which can be used to inject
    and execute arbitrary code to read the assets.
  - Device might contain unsigned binary blob next to the official firmware.
    This can be any data, not necessarily code. If an attacker manages to
    replace this data with arbitrary content (e.g. a NOP slide leading to a
    malicious code) then they can try to manipulate the program counter to jump
    to this area before setting up the memory isolation.

.. _attacker-capability:

Assumptions on attacker capability
==================================
It is assumed that the attacker owns the following capabilities to perform
physical attack against devices protected by TF-M.

  - Has physical access to the device.
  - Able to access external memory, read and possibly tamper it.
  - Able to load arbitrary candidate images for firmware upgrade.
  - Able to manage that bootloader tries to upgrade the arbitrary image from
    staging area.
  - Able to inject faults on hardware level (voltage or power glitch, EM pulse,
    etc.) to the system.
  - Precise timing of fault injection is possible once or a few times, but in
    general the more intervention is required for a successful attack the harder
    will be to succeed.

It is out of the scope of TF-M mitigation if an attacker is able to directly
tamper or disclose the assets. It is assumed that an attacker has the following
technical limitations.

  - No knowledge of the image signing key. Not able to sign an arbitrary image.
  - Not able to directly access to the chip through debug port.
  - Not able to directly access internal memory.
  - No knowledge of the layout of the die or the memory arrangement of the
    secure code, so precise attack against specific registers or memory
    addresses are out of scope.

Physical attack scenarios against TF-M
======================================
Based on the analysis above, a malicious actor may perform physical attacks
against critical operations in :term:`SPE` workflow and critical modules in
TF-M, to indirectly gain unauthenticated accesses to assets.

Those critical operations and modules either directly access the assets or
protect the assets from disclosure. Those operations and modules can include:

  - Image validation in bootloader
  - Isolation management in TF-M, including platform specific configuration
  - Cryptographic operations
  - TF-M Secure Storage operations
  - PSA client permission check in TF-M

The detailed scenarios are discussed in following sections.

Physical attacks against bootloader
-----------------------------------
Physical attacks may bypass secure image validation in bootloader and a
malicious image can be installed.

The countermeasures is bootloader specific implementation and out of the scope
of this document. TF-M relies on MCUboot by default. MCUboot has already
implemented countermeasures against fault injection attacks [3]_.

.. _physical-attacks-spm:

Physical attacks against TF-M SPM
---------------------------------
TF-M SPM initializes and manages the isolation configuration. It also performs
permission check against secure service requests from PSA clients.

Static isolation configuration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
It is TF-M SPM's responsibility to build up isolation during the initialization
phase. If this is missed or not done correctly then it might be possible for
non-secure code to access some secure memory area or an external device can
access assets in the device through a debug port.

Therefore, hindering the setup of memory or peripheral isolation hardware is an
obvious candidate for physical attacks. The initialization phase has a constant
time execution (like the previous boot-up state), therefore the timing of the
attack is simpler, compared to cases when secure and non-secure runtime firmware
is up-and-running for a while and IRQs make timing unpredictable.

Some examples of attacking isolation configuration are shown in the list below.

  - Hinder the setting of security regions. Try to execute non-secure code as
    secure.
  - Manipulate the setting of secure regions, try to extend the non-secure
    regions to cover a memory area which otherwise is intended to be secure
    area.
  - Hinder the setting of isolation boundary. In this case vulnerable ARoT code
    has access to all memory.
  - Manipulate peripheral configuration to give access to non-secure code to a
    peripheral which is intended to be secure.

PSA client permission checks
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
TF-M SPM performs several permission checks against secure service requests from
a PSA client, such as:

- Check whether the PSA client is a non-secure client or a secure client

  NS client's PSA client ID is negative. NS client is not allowed to directly
  access secure areas. A malicious actor can inject faults when TF-M SPM
  authenticates a NS client. It may manipulate TF-M to accept it as a secure
  client and allow the NS client to access assets.

- Memory access checks

  TF-M SPM checks whether the request has correct permission to access a secure
  memory area. A malicious actor can inject faults when TF-M SPM checks memory
  access permission. It may skip critical check steps or corrupt the check
  result. Thereby a malicious service request may pass TF-M memory access check
  and accesses assets which it is not allowed to.

The physical attacks mentioned above relies on the a malicious NS application or
a vulnerable RoT service to start a malicious secure service request to access
the assets. The malicious actor has to be aware of the accurate timing of
dealing with the malicious request in TF-M SPM. The timing can be affected by
other clients and interrupts.
It should be more difficult than pure fault injection.

Dynamic isolation boundary configuration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Physical attack may affect the isolation boundary setting during TF-M context
switch, especially in Isolation Level 3. For example:

  - A fault injection may cause TF-M SPM to skip clear privileged state before
    switching in an ARoT service.
  - A fault injection may cause TF-M SPM to skip updating MPU regions and
    therefore the next RoT service may access assets belonging to a previous
    one.

However, it is much more difficult to find out the accurate timing of TF-M
context switch, compared to other scenarios in TF-M SPM. It also requires a
vulnerable RoT service to access assets after fault injection.

Physical attacks against TF-M Crypto service
--------------------------------------------
Since crypto operations are done by mbedTLS library or by a custom crypto
accelerator engine and its related software driver stack, the analysis of
physical attacks against crypto operations is out-of-scope for this document.
However, in general the same requirements are applicable for the crypto, to be
compliant with PSA Level 3 certification. That is, it must be resistant against
physical attacks. So crypto software and hardware must be hardened against
side-channel and physical attacks.

Physical attacks against Secure Storage
---------------------------------------
Physical attacks against Internal Trusted Storage
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Based on the assumption in :ref:`attacker-capability`, a malicious actor is
unable to directly retrieve assets via physical attacks against
:term:`Internal Trusted Storage` (ITS).

Instead, a malicious actor can inject faults into isolation configuration of ITS
area in TF-M SPM to gain the access to assets stored in ITS. Refer to
:ref:`physical-attacks-spm` for details.

Physical attacks against Protected Storage
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Based on the assumption in :ref:`attacker-capability`, a malicious actor can be
able to directly access external storage device.
Therefore :term:`Protected Storage` (PS) shall enable encryption and
authentication by default to detect tampering with the content in external
storage device.

A malicious actor can also inject faults into isolation configuration of PS and
external storage device peripherals in TF-M SPM to gain the access to assets
stored in PS. Refer to :ref:`physical-attacks-spm` for details.

It is out of the scope of TF-M to fully prevent malicious actors from directly
tampering with or retrieving content stored in external storage devices.

Physical attacks against platform specific implementation
---------------------------------------------------------
Platform specific implementation includes critical TF-M HAL implementations.
A malicious actor can perform physical attack against those platform specific
implementations to bypass the countermeasures in TF-M common code.

Platform early initialization
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
TFM provides a HAL API for platforms to perform HW initialization before SPM
initialization starts.
The system integrator is responsible to implement this API on a particular SoC
and harden it against physical attacks:

.. code-block:: c

  enum tfm_hal_status_t tfm_hal_platform_init(void);

The API can have several initializations on different modules. The system
integrator can choose to even harden some of these initializations functions
within this platform init API. One of the examples is the debug access setting.

Debug access setting
********************
TF-M configures debug access according to device lifecycle and accessible debug
certificates. In general, TF-M locks down the debug port if the device is in
secure production state.
The system integrator can put the settings into an API and harden it against
physical attacks.

Platform specific isolation configuration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
TFM SPM exposes a HAL API for static and dynamic isolation configuration. The
system integrator is responsible to implement these API on a particular SoC and
harden it against physical attacks.

.. code-block:: c

  enum tfm_hal_status_t tfm_hal_set_up_static_boundaries(void);
  enum tfm_plat_err_t tfm_spm_hal_configure_default_isolation(
                 bool privileged,
                 const struct platform_data_t *platform_data);
  enum tfm_hal_status_t tfm_hal_mpu_update_partition_boundary(uintptr_t start,
                                                              uintptr_t end);

Memory access check
^^^^^^^^^^^^^^^^^^^
TFM SPM exposes a HAL API for platform specific memory access check. The
system integrator is responsible to implement these API on a particular SoC and
harden it against physical attacks.

.. code-block:: c

  tfm_hal_status_t tfm_hal_memory_has_access(const uintptr_t base,
                                             size_t size,
                                             uint32_t attr);

.. _tf-m-against-physical-attacks:

*********************************************
TF-M countermeasures against physical attacks
*********************************************
This section propose a design of software countermeasures against physical
attacks.

Fault injection hardening library
=================================
There is no open-source library which implements generic mitigation techniques
listed in :ref:`phy-att-countermeasures`.
TF-M project implements a portion of these techniques. TF-M software
countermeasures are implemented as a small library Fault Injection Hardening
(FIH) in TF-M code base. A similar library was first introduced and tested in
the MCUboot project (version 1.7.0) [2]_ which TF-M relies on.

The FIH library is put under TF-M ``lib/fih/``.

The implementation of the different techniques was assigned to fault injection
protection profiles. Four profile (OFF, LOW, MEDIUM, HIGH) was introduced to fit
better to the device capability (memory size, TRNG availability) and to
protection requirements mandated by the device threat model. Fault injection
protection profile is configurable at compile-time, default value: OFF.

Countermeasure profiles and corresponding techniques are listed in the table
below.

+--------------------------------+-------------+----------------+--------------+------------------+
|        Countermeasure          | Profile LOW | Profile MEDIUM | Profile HIGH | Comments         |
+================================+=============+================+==============+==================+
| Control flow monitor           | Y           | Y              | Y            |                  |
+--------------------------------+-------------+----------------+--------------+------------------+
| Failure loop hardening         | Y           | Y              | Y            |                  |
+--------------------------------+-------------+----------------+--------------+------------------+
| Complex constant               |             | Y              | Y            |                  |
+--------------------------------+-------------+----------------+--------------+------------------+
| Redundant variables and checks |             | Y              | Y            |                  |
+--------------------------------+-------------+----------------+--------------+------------------+
| Random delay                   |             |                | Y            | Implemented, but |
|                                |             |                |              | depends on HW    |
|                                |             |                |              | capability       |
+--------------------------------+-------------+----------------+--------------+------------------+

Similar to MCUboot four profiles are supported, it can be configured at build
time by setting(default is OFF):

  ``-DTFM_FIH_PROFILE=<OFF, LOW, MEDIUM, HIGH>``

How to use FIH library
======================
As analyzed in :ref:`phy-att-threat-model`, this section focuses on integrating
FIH library in TF-M SPM to mitigate physical attacks.

  - Identify critical function call path which is mandatory for configuring
    isolation or debug access. Transfer them to ``fih_int`` functions with the
    usage of ``FIH_CALL`` and ``FIH_RET`` macros. These are providing the extra
    checking functionality (control flow monitor, redundant checks and
    variables, random delay, complex constant) according to the profile
    settings. More details about usage can be found here:
    ``tf-m/lib/fih/inc/fault_injection_hardening.h``

    Take simplified TF-M SPM initialization flow as an example:

    .. code-block:: c

      main()
        |
        |--> tfm_core_init()
        |           |
        |           |--> tfm_hal_set_up_static_boundaries()
        |           |                  |
        |           |                  |--> platform specific isolation impl.
        |           |
        |           |--> tfm_hal_platform_init()
        |                              |
        |                              |--> platform specific init
        |
        |--> During each partition initialization
                    |
                    |--> tfm_spm_hal_configure_default_isolation()
                                       |
                                       |--> platform specific peripheral
                                            isolation impl.

  - Might make the important setting of peripheral config register redundant
    and verify them to match expectations before continue.

  - Implements an extra verification function which checks the critical hardware
    config before secure code switches to non-secure. Proposed API for this
    purpose:

    .. code-block:: c

      fih_int tfm_hal_verify_static_boundaries(void);

    This function is intended to be called just before the security state
    transition and is responsible for checking all critical hardware
    configuration. The goal is to catch if something is missed and act according
    to system policy. The introduction of one more checking point requires one
    more intervention with precise timing. The system integrator is responsible
    to implement this API on a particular SoC and harden it against physical
    attacks. Make sure that all platform dependent security feature is properly
    configured.

  - The most powerful mitigation technique is to add random delay to the code
    execution. This makes the timing of the attack much harder. However it
    requires an entropy source. It is recommended to use the ``HIGH`` profile
    when hardware support is available. There is a porting API layer to fetch
    random numbers in FIH library:

    .. code-block:: c

      int fih_delay_init(void);
      unsigned char fih_delay_random_uchar(void);

  - Similar countermeasures can be implemented in critical steps in platform
    specific implementation.

    Take memory isolation settings on AN521 and Musca-B1 platforms as an
    example.
    The following hardware components are responsible for memory isolation in a
    SoC, which is based on SSE-200 subsystem.
    System integrators must examine the chip specific memory isolation solution,
    identify the key components and harden the configuration of those.
    This list just serves as an example here for easier understanding:

      - Implementation Defined Attribution Unit (IDAU): Implementation defined,
        it can be a static config or dynamic.
        Contains the default security access permissions of the memory map.
      - SAU: The main module in the CPU to determine the security settings of
        the memory.
      - :term:`MPC`: External module from the CPU point of view. It protects the
        non security aware memories from unauthenticated access. Having a
        properly configured MPC significantly increases the security of the
        system.
      - :term:`PPC`: External module from the CPU
        point of view. Protects the non security aware peripherals from
        unauthenticated access.
      - MPU: Protects memory from unprivileged access. ARoT code has only a
        restricted access in secure domain. It mitigates that a vulnerable or
        malicious ARoT partition can access to device assets.

    The following AN521/Musca-B1 specific isolation configuration functions
    shall be hardened against physical attacks.

    .. code-block:: c

      sau_and_idau_cfg()
      mpc_init_cfg()
      ppc_init_cfg()

    Some platform specific implementation rely on platform standard device
    driver libraries. It can become much more difficult to maintain drivers if
    the standard libraries are modified with FIH library. Platform specific
    implementation can implement duplicated execution and redundant variables/
    condition check when calling platform standard device driver libraries
    according to usage scenarios.

Impact on code size
===================
The addition of protection code against physical attacks increases the code
size. The actual increase depends on the selected profile and where the
mitigation code is added.

Attack experiment with SPM
==========================
The goal is to bypass the setting of memory isolation hardware with simulated
instruction skips in fast model execution (FVP_MPS2_AEMv8M) in order to execute
the regular non-secure test code in secure state. This is done by identifying
the configuration steps which must be bypassed to make this happen. The
instruction skip simulation is achieved by breakpoints and manual manipulation
of the program counter. The following steps are done on AN521 target, but this
can be different on another target:

  - Bypass the configuration of isolation HW: SAU, MPC.
  - Bypass the setting of the PSP limit register. Otherwise, a stack overflow
    exception will happen. Because the secure PSP will be overwritten by the
    address of the non-secure stack and on this particular target the non-secure
    stack is on lower address than the value in the secure PSP_LIMIT register.
  - Avoid the clearing of the least significant bit in the non-secure entry
    point, where BLXNS/BXNS is jumping to non-secure code. Having the least
    significant bit cleared indicates to the hardware to switch security state.

The previous steps are enough to execute the non-secure Reset_Handler() in
secure state. Usually, RTOS is executing on the non-secure side. In order to
properly boot it up further steps are needed:

  - Set the S_VTOR system register to point the address of the NS Vector table.
    Code is executed in secure state therefore when an IRQ hit then the handler
    address is fetched from the table pointed by S_VTOR register. RTOS usually
    do an SVC call at start-up. If S_VTOR is not modified then SPM's SVC handler
    will be executed.
  - TBC: RTX osKernelStart still failing.

The bottom line is that in order to execute the regular non-secure code in
secure state the attacker need to interfere with the execution flow at many
places. Successful attack can be made even harder by adding the described
mitigation techniques and some random delays.


*********
Reference
*********

.. [1] `PSA Certified Level 3 Lightweight Protection Profile <https://www.psacertified.org/app/uploads/2020/11/JSADEN009-PSA_Certified_Level_3_LW_PP-1.0-ALP02.pdf>`_

.. [2] `MCUboot project <https://github.com/mcu-tools/mcuboot/blob/master/boot/bootutil/include/bootutil/fault_injection_hardening.h>`_

.. [3] `MCUboot fault injection mitigation <https://www.trustedfirmware.org/docs/TF-M_fault_injection_mitigation.pdf>`_

--------------------------------

*Copyright (c) 2021, Arm Limited. All rights reserved.*
