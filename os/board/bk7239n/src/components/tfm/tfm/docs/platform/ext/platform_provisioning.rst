#####################
Platform Provisioning
#####################

TF-M stores any data that should be provisioned at the factory in OTP memory.
The default is that this OTP memory is actually implemented using on-chip flash,
the same that is used to implement the ITS service.

If the lifecycle state is in the ``TFM_SLC_ASSEMBLY_AND_TEST`` [1]_ state (which
is the default for non-provisioned boards), then TF-M will attempt to provision
the:
- HUK
instead of booting. It will read the data from the
``assembly_and_test_prov_data`` struct, and will then provision it to OTP. The
lifecycle state will then transition to ``TFM_SLC_PSA_ROT_PROVISIONING`` [1]_.

If the lifecycle state is in the ``TFM_SLC_PSA_ROT_PROVISIONING`` [1]_ state,
then TF-M will attempt to provision the:

- IAK
- boot seed
- implementation id
- hw version
- bl2 ROTPKs (of which there are up to 4)
- entropy seed

Once all of these have been loaded from the ``psa_rot_prov_data`` struct and
provisioned to OTP, the LCS will transition to ``TFM_SLC_SECURED`` [1]_. Note
that this provisioning step will be run immediately after the
``TFM_SLC_ASSEMBLY_AND_TEST`` [1]_ provisioning stage if the lifecycle
transition completed successfully.

Provisioning development hardware
=================================

If ``TFM_DUMMY_PROVISIONING`` is enabled in the cmake config (as it is by
default), a set of dummy keys / data will be provisioned. The dummy IAK matches
the IAK tested by the TF-M tests, and the dummy bl2 ROTPKs match the dummy bl2
keys used by default. ``TFM_DUMMY_PROVISIONING`` _MUST_ not be used in
production hardware, as the keys are insecure.

Provisioning production hardware
================================

For provisioning of real hardware, firstly ``TFM_DUMMY_PROVISIONING`` must be
disabled. Then it is required to inject the keys into RAM so they populate the
``assembly_and_test_prov_data`` and ``psa_rot_prov_data`` structs, at the
beginning of the TF-M boot. These structs each require a magic value to be set
to be accepted by the provisioning code, which is detailed in
``platform/ext/common/provisioning.c``. Two suggestions for how to do this are:

- Attach a debugger, and inject the values into RAM.
- Flash an image that contains the required data. Care must be taken with this
  approach that the keys are not left in RAM after provisioning, so a different
  image (without provisioning data embedded) must be flashed afterwards, without
  erasing the OTP flash area.

************************************************
Provisioning on CryptoCell-312 enabled platforms
************************************************

On boards that have a CC312 accelerator, and that have the default flash-backed
OTP disabled by setting ``PLATFORM_DEFAULT_OTP=OFF`` in cmake, the CC312 OTP
will be used as a backing for the OTP HAL.

Due to the CC312 requiring a power-cycle to transition LCS, you will be prompted
to manually power-cycle the board between provisioning stages.

Boards with real OTP memory cannot be reprovisioned - care should be taken that
the data being provisioned is the desired data.

*****************************
Platform-specific OTP backing
*****************************

If a platform has a medium that is suitable for storing data with OTP semantics
(Where a bit cannot transition from a 1 to a 0), such as physical OTP memory,
then it can provide a backing for the OTP HAL by implementing the methods
described in ``tfm_plat_otp.h``.

--------------

.. [1] For the definitions of these lifecycle states, please refer to the
       Platform Security Model
       https://developer.arm.com/documentation/den0128/0100/

--------------

*Copyright (c) 2020-2021, Arm Limited. All rights reserved.*
