###########
Secure boot
###########
For secure devices it is security critical to enforce firmware authenticity to
protect against execution of malicious software. This is implemented by building
a trust chain where each step in the execution chain authenticates the next
step before execution. The chain of trust in based on a "Root of Trust" which
is implemented using asymmetric cryptography. The Root of Trust is a combination
of an immutable bootloader and a public key (ROTPK).

.. Warning::
    In order to implement a proper chain of trust functionality, it is
    mandatory that the first stage bootloader and ROTPK is stored in an
    **immutable** way. To achieve this the bootloader code must be stored and
    executed from ROM or such part of flash memory which supports write
    protection. ROTPK can be stored in a one-time-programmable (OTP) memory. If
    the SoC has a built-in BL1 (immutable) bootloader and the immutability of
    TF-M secure boot code is not guaranteed then TF-M secure boot code must be
    authenticated by BL1 bootloader before execution. If immutability of root
    of trust (first stage bootloader + ROTPK) is not ensured then there is a
    risk that the secure boot process could be bypassed, which could lead to
    arbitrary code execution on the device. Current TF-M secure boot code is
    intended to be a second stage bootloader, therefore it requires
    authentication before execution. If TF-M secure boot code is used as a first
    stage bootloader then it must be stored according to the above requirements.

*******************************
Second stage bootloader in TF-M
*******************************
By default, the MCUboot project from
`GitHub <https://github.com/mcu-tools/mcuboot>`__ is used as the secure
bootloader in TF-M. The repository is going to be automatically downloaded by
CMake. The version downloaded can be controlled by the ``MCUBOOT_VERSION``
CMake variable. If you wish to use a locally downloaded copy, the CMake variable
``MCUBOOT_PATH`` can be set to its location. This document contains information
about how MCUboot has been integrated to TF-M. For further information about
MCUboot design please refer to the `MCUBoot homepage <https://www.mcuboot.com/>`__.

Bootloader is started when CPU is released from reset. It runs in secure mode.
It authenticates the firmware image by hash (SHA-256) and digital signature
(RSA-3072) validation. Public key, that the checks happens against, can be built
into the bootloader image or can be provisioned to the SoC during manufacturing.
Metadata of the image is delivered together with the image itself in a header
and trailer section. In case of successful authentication, bootloader passes
execution to the secure image. Execution never returns to bootloader until
next reset.

A default RSA key pair is stored in the repository, public key is in ``keys.c``
and private key is in ``root-RSA-3072.pem``.

.. Warning::
    DO NOT use them in production code, they are exclusively for testing!

The private key must be stored in a safe place outside of the repository.
``imgtool.py`` (found in the ``scripts`` directory in the MCUBoot repository,
or installed through the pip package manager) can be used to generate new key
pairs.

The bootloader can handle the secure and non-secure images independently
(multiple image boot) or together (single image boot). In case of multiple image
boot they are signed independently with different keys and they can be updated
separately. In case of single image boot the secure and non-secure image is
handled as a single blob, therefore they must be contiguous in the device
memory. In this case they are signed together and also they can be updated only
together. In order to have the same artefacts at the end of the build regardless
of how the images are handled (independently or together) the images are always
concatenated. In case of single image boot they are concatenated first and then
signed. In case of multiple image boot they are separately signed first and then
concatenated. Preparation of payload is done by Python scripts:
``bl2/ext/mcuboot/scripts/``. At the end of a successful build the signed TF-M
payload can be found in: ``<build_dir>/bin/tfm_s_ns_signed.bin``

*********************
Integration with TF-M
*********************
MCUBoot assumes a predefined memory layout which is described below (applicable
for AN521). It is mandatory to define the primary slot and the secondary slot
partitions, but their size and location can be changed::

    - 0x0000_0000 - 0x0007_FFFF:    BL2 bootloader - MCUBoot
    - 0x0008_0000 - 0x000F_FFFF:    Primary slot : Single binary blob:
                                    Secure + Non-Secure image;
                                    Primary memory partition
      - 0x0008_0000 - 0x0008_03FF:  Common image header
      - 0x0008_0400 - 0x0008_xxxx:  Secure image
      - 0x0008_xxxx - 0x0010_03FF:  Padding (with 0xFF)
      - 0x0010_0400 - 0x0010_xxxx:  Non-secure image
      - 0x0010_xxxx - 0x0010_xxxx:  Hash value(SHA256), RSA signature and other
                                    metadata of combined image

    - 0x0018_0000 - 0x0027_FFFF:    Secondary slot : Secure + Non-Secure image;
                                    Secondary memory partition, structured
                                    identically to the primary slot
    - 0x0028_0000 - 0x0037_FFFF:    Scratch area, only used during image
                                    swapping

Multiple image boot requires a slightly different layout::

    - 0x0000_0000 - 0x0007_FFFF:    BL2 bootloader - MCUBoot
    - 0x0008_0000 - 0x000F_FFFF:    Primary slot : Secure image
      - 0x0008_0000 - 0x0008_03FF:  Secure image header
      - 0x0008_0400 - 0x000x_xxxx:  Secure image
      - 0x000x_xxxx - 0x000x_xxxx:  Hash value(SHA256), RSA signature and other
                                    metadata of secure image

    - 0x0010_0000 - 0x0017_FFFF:    Primary slot : Non-secure image
      - 0x0010_0000 - 0x0010_03FF:  Non-secure image header
      - 0x0010_0400 - 0x001x_xxxx:  Non-secure image
      - 0x001x_xxxx - 0x001x_xxxx:  Hash value(SHA256), RSA signature and other
                                    metadata of non-secure image

    - 0x0018_0000 - 0x001F_FFFF:    Secondary slot : Secure image
    - 0x0020_0000 - 0x0027_FFFF:    Secondary slot : Non-secure image

    - 0x0028_0000 - 0x002F_FFFF:    Scratch area, only used during image
                                    swapping, used for secure and non-secure
                                    image as well

**************************
Firmware upgrade operation
**************************
MCUBoot handles only the firmware authenticity check after start-up and the
firmware switch part of the firmware update process. Downloading the new version
of the firmware is out-of-scope for MCUBoot. MCUBoot supports three different
ways to switch to the new firmware and it is assumed that firmware images are
executed-in-place (XIP). The default behaviour is the overwrite-based image
upgrade. In this case the active firmware is always executed from the primary
slot and the secondary slot is a staging area for new images. Before executing
the new firmware image, the content of the primary slot must be overwritten with
the content of the secondary slot (the new firmware image). The second option is
the image swapping strategy when the content of the two memory slots must be
physically swapped. This needs the scratch area to be defined in the memory
layout. The third option is the direct execute-in-place version, which
eliminates the complexity of image swapping and its administration. Active image
can be executed from either memory slot, but new firmware must be linked to the
address space of the proper (currently inactive) memory slot.

Overwrite operation
===================
Active image is stored in the primary slot, and this image is started always by
the bootloader. Therefore images must be linked to the primary slot. If the
bootloader finds a valid image in the secondary slot, which is marked for
upgrade, then the content of the primary slot will be simply overwritten with
the content of the secondary slot, before starting the new image from the
primary slot. After the content of the primary slot has been successfully
overwritten, the header and trailer of the new image in the secondary slot is
erased to prevent the triggering of another unnecessary image upgrade after a
restart. The overwrite operation is fail-safe and resistant to power-cut
failures. For more details please refer to the MCUBoot
`documentation <https://www.mcuboot.com/mcuboot/design.html>`__.

Swapping operation
==================
This operation can be set with the ``MCUBOOT_UPGRADE_STRATEGY`` compile time
switch (see `Build time configuration`_). With swapping image upgrade strategy
the active image is also stored in the primary slot and it will always be
started by the bootloader. If the bootloader finds a valid image in the
secondary slot, which is marked for upgrade, then contents of the primary slot
and the secondary slot will be swapped, before starting the new image from the
primary slot. Scratch area is used as a temporary storage place during image
swapping. Update mark from the secondary slot is removed when the swapping is
successful. The boot loader can revert the swapping as a fall-back mechanism to
recover the previous working firmware version after a faulty update. The swap
operation is fail-safe and resistant to power-cut failures. For more details
please refer to the MCUBoot
`documentation <https://www.mcuboot.com/mcuboot/design.html>`__.

.. Note::

    After a successful image upgrade the firmware can mark itself as "OK" at
    runtime by setting the image_ok flag in the flash. When this happens, the
    swap is made "permanent" and MCUBoot will then still choose to run it
    during the next boot. Currently TF-M does not set the image_ok flag,
    therefore the bootloader will always perform a "revert" (swap the images
    back) during the next boot.

Direct execute-in-place operation
=================================
This operation can be set with the ``MCUBOOT_UPGRADE_STRATEGY`` compile time
switch (see `Build time configuration`_). When enabling direct-xip operation
then the active image flag is moved between slots during firmware upgrade. If
firmware is executed-in-place (XIP), then two firmware images must be generated.
One of them is linked to be executed from the primary slot memory region and the
other from the secondary slot. The firmware upgrade client, which downloads the
new image, must be aware, which slot hosts the active firmware and which acts as
a staging area and it is responsible for downloading the proper firmware image.
At boot time MCUBoot inspects the version number in the image header and passes
execution to the newer firmware version. New image must be marked for upgrade
which is automatically done by Python scripts at compile time. Image
verification is done the same way in all operational modes. If new image fails
during authentication then MCUBoot erases the memory slot and starts the other
image, after successful authentication.

To select which slot the image is to be executed from, set
``MCUBOOT_EXECUTION_SLOT`` to the desired index. It is suggested that you create
two build directories when building images using this mode, as intermediate
dependencies cannot be reused due to changes in the flash layout.

.. Note::

    Only single image boot is supported with direct-xip upgrade mode.

RAM Loading firmware upgrade
============================
Musca-S supports an image upgrade mode that is separate to the other (overwrite,
swapping and dirext-xip) modes. This is the ``RAM load`` mode (please refer
to the table below). Like the direct-xip mode, this selects the newest image
by reading the image version numbers in the image headers, but instead of
executing it in place, the newest image is copied to RAM for execution. The load
address, the location in RAM where the image is copied to, is stored in the
image header.

.. Note::

    Only single image boot is supported with ``RAM load`` upgrade mode.

Summary of different modes for image upgrade
============================================
Different implementations of the image upgrade operation (whether through
overwriting, swapping, direct-xip or loading into RAM and executing from
there) are supported by the platforms. The table below shows which of these
modes are supported by which platforms:

+---------------------+-----------------+----------------------------------------------------------+
|                     | Without BL2 [1]_| With BL2 [2]_                                            |
+=====================+=================+===============+==========+================+==============+
|                     | XIP             | XIP           | XIP      | XIP            | Not XIP      |
+---------------------+-----------------+---------------+----------+----------------+--------------+
|                     |                 | Overwrite [3]_| Swap [4]_| direct-xip [5]_| RAM load [6]_|
+---------------------+-----------------+---------------+----------+----------------+--------------+
| AN521               | Yes             | Yes           | Yes      | Yes            | No           |
+---------------------+-----------------+---------------+----------+----------------+--------------+
| AN519               | Yes             | Yes           | Yes      | Yes            | No           |
+---------------------+-----------------+---------------+----------+----------------+--------------+
| FVP_SSE300_MPS3     | No              | Yes           | Yes      | Yes            | No           |
+---------------------+-----------------+---------------+----------+----------------+--------------+
| Corstone-Polaris    | NO              | Yes           | Yes      | Yes            | No           |
+---------------------+-----------------+---------------+----------+----------------+--------------+
| LPC55S69            | Yes             | Yes           | No       | Yes            | No           |
+---------------------+-----------------+---------------+----------+----------------+--------------+
| Musca-B1            | Yes             | Yes           | Yes      | Yes            | No           |
+---------------------+-----------------+---------------+----------+----------------+--------------+
| Musca-S1            | Yes             | Yes           | Yes      | Yes            | No           |
+---------------------+-----------------+---------------+----------+----------------+--------------+
| AN524               | Yes             | No            | No       | Yes            | No           |
+---------------------+-----------------+---------------+----------+----------------+--------------+
| AN547               | No              | Yes           | Yes      | Yes            | No           |
+---------------------+-----------------+---------------+----------+----------------+--------------+
| AN552               | No              | Yes           | Yes      | Yes            | No           |
+---------------------+-----------------+---------------+----------+----------------+--------------+
| PSoC64              | Yes             | No            | No       | No             | No           |
+---------------------+-----------------+---------------+----------+----------------+--------------+
| STM_DISCO_L562QE    | No              | Yes           | No       | No             | No           |
+---------------------+-----------------+---------------+----------+----------------+--------------+
| STM_NUCLEO_L552ZE_Q | No              | Yes           | No       | No             | No           |
+---------------------+-----------------+---------------+----------+----------------+--------------+
| nRF9160 DK          | Yes             | Yes           | No       | No             | No           |
+---------------------+-----------------+---------------+----------+----------------+--------------+
| nRF5340 DK          | Yes             | Yes           | No       | No             | No           |
+---------------------+-----------------+---------------+----------+----------------+--------------+
| BL5340 DVK          | Yes             | Yes           | Yes      | No             | No           |
+---------------------+-----------------+---------------+----------+----------------+--------------+

.. [1] To disable BL2, please set the ``BL2`` cmake option to ``OFF``

.. [2] BL2 is enabled by default

.. [3] The image executes in-place (XIP) and is in Overwrite mode for image
    update by default

.. [4] To enable XIP Swap mode, assign the "SWAP_USING_SCRATCH" or
    "SWAP_USING_MOVE" string to the ``MCUBOOT_UPGRADE_STRATEGY``
    configuration variable in the build configuration file, or include this
    macro definition in the command line

.. [5] To enable direct-xip, assign the "DIRECT_XIP" string to the
    ``MCUBOOT_UPGRADE_STRATEGY`` configuration variable in the build
    configuration file, or include this macro definition in the command line

.. [6] To enable RAM load, assign the "RAM_LOAD" string to the
    ``MCUBOOT_UPGRADE_STRATEGY`` configuration variable in the build
    configuration file, or include this macro definition in the command line

*******************
Multiple image boot
*******************
It is possible to update the firmware images independently to support the
scenario when secure and non-secure images are provided by different vendors.
Multiple image boot is supported only together with the overwrite and swap
firmware upgrade modes.

It is possible to describe the dependencies of the images on each other in
order to avoid a faulty upgrade when incompatible versions would be installed.
These dependencies are part of the image manifest area.
The dependencies are composed from two parts:

 - **Image identifier:** The number of the image which the current image (whose
   manifest area contains the dependency entry) depends on. The image identifier
   starts from 0.

 - **Minimum version:** The minimum version of other image must be present on
   the device by the end of the upgrade (both images might be updated at the
   same time).

Dependencies can be added to the images at compile time with the following
compile time switches:

 - ``MCUBOOT_S_IMAGE_MIN_VER`` It is added to the non-secure image and specifies the
   minimum required version of the secure image.
 - ``MCUBOOT_NS_IMAGE_MIN_VER`` It is added to the secure image and specifies the
   minimum required version of the non-secure image.

Example of how to provide the secure image minimum version::

    cmake -DTFM_PLATFORM=arm/musca_b1/sse_200 -DMCUBOOT_S_IMAGE_MIN_VER=1.2.3+4 ..

********************
Signature algorithms
********************
MbedTLS library is used to sign the images. The list of supported signing
algorithms:

  - `RSA-2048`
  - `RSA-3072`: default

Example keys stored in:

 - ``root-RSA-2048.pem``   : Used to sign single image (S+NS) or secure image
   in case of multiple image boot
 - ``root-RSA-2048_1.pem`` : Used to sign non-secure image in case of multiple
   image boot
 - ``root-RSA-3072.pem``   : Used to sign single image (S+NS) or secure image
   in case of multiple image boot
 - ``root-RSA-3072_1.pem`` : Used to sign non-secure image in case of multiple
   image boot

************************
Build time configuration
************************
MCUBoot related compile time switches can be set by cmake variables.

- BL2 (default: True):
    - **True:** TF-M built together with bootloader. MCUBoot is executed after
      reset and it authenticates TF-M and starts secure code.
    - **False:** TF-M built without bootloader. Secure image linked to the
      beginning of the device memory and executed after reset. If it is false
      then using any of the further compile time switches is invalid.
- MCUBOOT_UPGRADE_STRATEGY (default: "OVERWRITE_ONLY"):
    - **"OVERWRITE_ONLY":** Default firmware upgrade operation with overwrite.
    - **"SWAP_USING_SCRATCH":** Activate swapping firmware upgrade operation
      with a scratch area in flash
    - **"SWAP_USING_MOVE":** Activate swapping firmware upgrade operation
      without a scratch area in flash
    - **"DIRECT_XIP":** Activate direct execute-in-place firmware upgrade
      operation.
    - **"RAM_LOAD":** Activate RAM loading firmware upgrade operation, where
      the latest image is copied to RAM and runs from there instead of being
      executed in-place.
- MCUBOOT_SIGNATURE_TYPE (default: RSA):
    - **RSA:** Image is signed with RSA algorithm
- MCUBOOT_SIGNATURE_KEY_LEN (default: 3072):
    - **2048:** Image is signed with 2048 bit key.
    - **3072:** Image is signed with 3072 bit key.
- MCUBOOT_IMAGE_NUMBER (default: 2):
    - **1:** Single image boot, secure and non-secure images are signed and
      updated together.
    - **2:** Multiple image boot, secure and non-secure images are signed and
      updatable independently.
- MCUBOOT_HW_KEY (default: True):
    - **True:** The hash of public key is provisioned to the SoC and the image
      manifest contains the whole public key (imgtool uses
      ``--public_key_format=full``). MCUBoot validates the key before using it
      for firmware authentication, it calculates the hash of public key from the
      manifest and compare against the retrieved key-hash from the hardware.
      This way MCUBoot is independent from the public key(s).  Key(s) can be
      provisioned any time and by different parties.
    - **False:** The whole public key is embedded to the bootloader code and the
      image manifest contains only the hash of the public key (imgtool uses
      ``--public_key_format=hash``). MCUBoot validates the key before using it
      for firmware authentication, it calculates the hash of built-in public key
      and compare against the retrieved key-hash from the image manifest. After
      this the bootloader can verify that the image was signed with a private
      key that corresponds to the retrieved key-hash (it can have more public
      keys embedded in and it may have to look for the matching one). All the
      public key(s) must be known at MCUBoot build time.
- MCUBOOT_LOG_LEVEL:
    Can be used to configure the level of logging in MCUBoot. The possible
    values are the following:

    - **OFF**
    - **ERROR**
    - **WARNING**
    - **INFO**
    - **DEBUG**

    The logging in MCUBoot can be disabled and thus the code size can be reduced
    by setting it to ``OFF``. Its value depends on the build type. If the build
    type is ``Debug`` then default value is ``INFO``. In case of different kinds
    of ``Release`` builds the default value is ``OFF``. The default value can
    be overridden through the command line or in the CMake GUI regardless of the
    build type.
- MCUBOOT_ENC_IMAGES (default: False):
    - **True:** Adds encrypted image support in the source and encrypts the
      resulting image using the ``enc-rsa2048-pub.pem`` key found in the MCUBoot
      repository.
    - **False:** Doesn't add encrypted image support and doesn't encrypt the
      image.

    .. Note::
        The decryption takes place during the upgrade process, when the images
        are being moved between the slots. This means that boards that don't
        already have an image on them with MCUBoot that has been compiled with
        ``MCUBOOT_ENCRYPT_RSA`` enabled need special treatment. In order to load
        an encrypted image to such boards, an upgrade needs to be executed. This
        can be done by using MCUBoot, putting an image in the secondary image
        area, and setting ``MCUBOOT_ENCRYPT_RSA`` to ``ON``. When using the
        ``OVERWRITE_ONLY`` upgrade strategy, this is enough. When using
        ``SWAP_USING_SCRATCH`` or ``SWAP_USING_MOVE``, an image is needed in
        the primary image area as well, to trigger the update.

    .. Warning::
        DO NOT use the ``enc-rsa2048-pub.pem`` key in production code, it is
        exclusively for testing!

Image versioning
================
An image version number is written to its header by one of the Python scripts,
and this number is used by the bootloader when the direct execute-in-place or
the RAM loading mode is enabled. It is also used in case of multiple image boot
when the bootloader checks the image dependencies if any have been added to the
images.

The version number of the image (single image boot) can manually be passed in
through the command line in the cmake configuration step::

    cmake -DTFM_PLATFORM=arm/musca_b1/sse_200 -DIMAGE_VERSION_S=1.2.3+4 ..

Alternatively, the version number can be less specific (e.g 1, 1.2, or 1.2.3),
where the missing numbers are automatically set to zero. The image version
number argument is optional, and if it is left out, then the version numbers of
the image(s) being built in the same directory will automatically change. In
this case, the last component (the build number) automatically increments from
the previous one: 0.0.0+1 -> 0.0.0+2, for as many times as the build is re-ran,
**until a number is explicitly provided**. If automatic versioning is in place
and then an image version number is provided for the first time, the new number
will take precedence and be used instead. All subsequent image versions are
then set to the last number that has been specified, and the build number would
stop incrementing. Any new version numbers that are provided will overwrite
the previous one: 0.0.0+1 -> 0.0.0+2. Note: To re-apply automatic image
versioning, please start a clean build without specifying the image version
number at all. In case of multiple image boot there are separate compile time
switches for both images to provide their version: ``IMAGE_VERSION_S`` and
``IMAGE_VERSION_NS``. These must be used instead of ``IMAGE_VERSION_S``.

Security counter
================
Each signed image contains a security counter in its manifest. It is used by the
bootloader and its aim is to have an independent (from the image version)
counter to ensure rollback protection by comparing the new image's security
counter against the original (currently active) image's security counter during
the image upgrade process. It is added to the manifest (to the TLV area that is
appended to the end of the image) by one of the Python scripts when signing the
image. The value of the security counter is security critical data and it is in
the integrity protected part of the image. The last valid security counter
should always be stored in a non-volatile and trusted component of the device
and its value should always be increased if a security flaw was fixed in the
current image version. The value of the security counter (single image boot) can
be specified at build time in the cmake configuration step::

    cmake -DTFM_PLATFORM=arm/musca_b1/sse_200 -DSECURITY_COUNTER_S=42 ../

The security counter can be independent from the image version, but not
necessarily. Alternatively, if it is not specified at build time with the
``SECURITY_COUNTER`` option the Python script will automatically generate it
from the image version number (not including the build number) and this value
will be added to the signed image. In case of multiple image boot there are
separate compile time switches for both images to provide their security counter
value: ``SECURITY_COUNTER_S`` and ``SECURITY_COUNTER_NS``. These must be used
instead of ``SECURITY_COUNTER_S``. If these are not defined then the security
counter values will be derived from the corresponding image version similar to
the single image boot.

***************************
Signing the images manually
***************************
Normally the build system handles the signing (computing hash over the image
and security critical manifest data and then signing the hash) of the firmware
images. However, the images also can be signed manually by using the ``imgtool``
Python program which is located in the MCUboot repository  in the ``scripts``
folder or can be installed with the pip package manager.
Issue the ``python3 imgtool.py sign --help`` command in the directory for more
information about the mandatory and optional arguments. The tool takes an image
in binary or Intel Hex format and adds a header and trailer that MCUBoot is
expecting. In case of single image boot after a successful build the
``tfm_s_ns.bin`` build artifact (contains the concatenated secure and non-secure
images) must be passed to the script and in case of multiple image boot the
``tfm_s.bin`` and ``tfm_ns.bin`` binaries can be passed to prepare the signed
images.

Signing the secure image manually in case of multiple image boot
================================================================

::

    python3 bl2/ext/mcuboot/scripts/imgtool.py sign \
        --layout <build_dir>/bl2/ext/mcuboot/CMakeFiles/signing_layout_s.dir/signing_layout_s.c.obj \
        -k <tfm_dir>/bl2/ext/mcuboot/root-RSA-3072.pem \
        --public-key-format full \
        --align 1 \
        -v 1.2.3+4 \
        -d "(1,1.2.3+0)" \
        -s 42 \
        -H 0x400 \
        <build_dir>/bin/tfm_s.bin \
        <build_dir>/bin/tfm_s_signed.bin

************************
Testing firmware upgrade
************************
As downloading the new firmware image is out of scope for MCUBoot, the update
process is started from a state where the original and the new image are already
programmed to the appropriate memory slots. To generate the original and a new
firmware package, TF-M is built twice with different build configurations.

Overwriting firmware upgrade
============================
Run TF-M build twice with ``MCUBOOT_IMAGE_NUMBER`` set to "1" in both cases
(single image boot), but with two different build configurations: default and
regression. Save the artifacts between builds, because second run can overwrite
original binaries. Download default build to the primary slot and regression
build to the secondary slot.

Executing firmware upgrade on FVP_MPS2_AEMv8M
---------------------------------------------
.. code-block:: bash

    <ARM_DS_PATH>/sw/models/bin/FVP_MPS2_AEMv8M  \
    --parameter fvp_mps2.platform_type=2 \
    --parameter cpu0.baseline=0 \
    --parameter cpu0.INITVTOR_S=0x10000000 \
    --parameter cpu0.semihosting-enable=0 \
    --parameter fvp_mps2.DISABLE_GATING=0 \
    --parameter fvp_mps2.telnetterminal0.start_telnet=1 \
    --parameter fvp_mps2.telnetterminal1.start_telnet=0 \
    --parameter fvp_mps2.telnetterminal2.start_telnet=0 \
    --parameter fvp_mps2.telnetterminal0.quiet=0 \
    --parameter fvp_mps2.telnetterminal1.quiet=1 \
    --parameter fvp_mps2.telnetterminal2.quiet=1 \
    --application cpu0=<build_dir>/bin/bl2.axf \
    --data cpu0=<default_build_dir>/bin/tfm_s_ns_signed.bin@0x10080000 \
    --data cpu0=<regresssion_build_dir>/bin/tfm_s_ns_signed.bin@0x10180000

Executing firmware upgrade on SSE 200 FPGA on MPS2 board
--------------------------------------------------------

::

    TITLE: Versatile Express Images Configuration File
    [IMAGES]
    TOTALIMAGES: 3                     ;Number of Images (Max: 32)
    IMAGE0ADDRESS: 0x00000000
    IMAGE0FILE: \Software\bl2.axf      ; BL2 bootloader
    IMAGE1ADDRESS: 0x10080000
    IMAGE1FILE: \Software\tfm_sig1.bin ; TF-M default test binary blob
    IMAGE2ADDRESS: 0x10180000
    IMAGE2FILE: \Software\tfm_sig2.bin ; TF-M regression test binary blob

The following message will be shown in case of successful firmware upgrade:

::

    [INF] Starting bootloader
    [INF] Swap type: test
    [INF] Image upgrade secondary slot -> primary slot
    [INF] Erasing the primary slot
    [INF] Copying the secondary slot to the primary slot: 0x100000 bytes
    [INF] Bootloader chainload address offset: 0x80000
    [INF] Jumping to the first image slot
    [Sec Thread] Secure image initializing!

    #### Execute test suites for the Secure area ####
    Running Test Suite PSA protected storage S interface tests (TFM_PS_TEST_2XXX)...
    ...

To update the secure and non-secure images separately (multiple image boot),
set the ``MCUBOOT_IMAGE_NUMBER`` switch to "2" (this is the default
configuration value) and follow the same instructions as in case of single image
boot.

Executing multiple firmware upgrades on SSE 200 FPGA on MPS2 board
------------------------------------------------------------------

::

    TITLE: Versatile Express Images Configuration File
    [IMAGES]
    TOTALIMAGES: 4                     ;Number of Images (Max: 32)
    IMAGE0ADDRESS: 0x00000000
    IMAGE0FILE: \Software\bl2.axf      ; BL2 bootloader
    IMAGE1ADDRESS: 0x10080000
    IMAGE1FILE: \Software\tfm_sign.bin ; TF-M default test binary blob
    IMAGE2ADDRESS: 0x10180000
    IMAGE2FILE: \Software\tfm_ss1.bin  ; TF-M regression test secure (signed) image
    IMAGE3ADDRESS: 0x10200000
    IMAGE3FILE: \Software\tfm_nss1.bin ; TF-M regression test non-secure (signed) image

Note that both the concatenated binary blob (the images are signed separately
and then concatenated) and the separate signed images can be downloaded to the
device because on this platform (AN521) both the primary slots and the secondary
slots are contiguous areas in the Flash (see `Integration with TF-M`_). The
following message will be shown in case of successful firmware upgrades:

::

    [INF] Starting bootloader
    [INF] Swap type: test
    [INF] Swap type: test
    [INF] Image upgrade secondary slot -> primary slot
    [INF] Erasing the primary slot
    [INF] Copying the secondary slot to the primary slot: 0x80000 bytes
    [INF] Image upgrade secondary slot -> primary slot
    [INF] Erasing the primary slot
    [INF] Copying the secondary slot to the primary slot: 0x80000 bytes
    [INF] Bootloader chainload address offset: 0x80000
    [INF] Jumping to the first image slot
    [Sec Thread] Secure image initializing!
    TFM level is: 1
    [Sec Thread] Jumping to non-secure code...

    #### Execute test suites for the Secure area ####
    Running Test Suite PSA protected storage S interface tests (TFM_PS_TEST_2XXX)...
    ...

Swapping firmware upgrade
=============================
Follow the same instructions and platform related configurations as in case of
overwriting build including these changes:

- Set the ``MCUBOOT_UPGRADE_STRATEGY`` compile time switch to "SWAP"
  before build.
- Set the ``MCUBOOT_IMAGE_NUMBER`` compile time switch to "1" (single image
  boot) or "2" (multiple image boot) before build.

During single image boot the following message will be shown in case of
successful firmware upgrade, ``Swap type: test`` indicates that images were
swapped:

::

    [INF] Starting bootloader
    [INF] Image 0: magic= good, copy_done=0x3, image_ok=0x3
    [INF] Scratch: magic=  bad, copy_done=0x0, image_ok=0x2
    [INF] Boot source: primary slot
    [INF] Swap type: test
    [INF] Bootloader chainload address offset: 0x80000
    [INF] Jumping to the first image slot
    [Sec Thread] Secure image initializing!

    #### Execute test suites for the Secure area ####
    Running Test Suite PSA protected storage S interface tests (TFM_PS_TEST_2XXX)...
    ...

Direct execute-in-place firmware upgrade
========================================
Follow the same instructions and platform related configurations as in case of
overwriting build including these changes:

- Set the ``MCUBOOT_UPGRADE_STRATEGY`` compile time switch to "DIRECT_XIP"
  before build.
- set ``MCUBOOT_EXECUTION_SLOT`` to ``1`` in the regression build dir.
- Make sure the image version number was increased between the two build runs
  either by specifying it manually or by checking in the build log that it was
  incremented automatically.

Executing firmware upgrade on FVP_MPS2_AEMv8M
---------------------------------------------

.. code-block:: bash

    <ARM_DS_PATH>/sw/models/bin/FVP_MPS2_AEMv8M  \
    --parameter fvp_mps2.platform_type=2 \
    --parameter cpu0.baseline=0 \
    --parameter cpu0.INITVTOR_S=0x10000000 \
    --parameter cpu0.semihosting-enable=0 \
    --parameter fvp_mps2.DISABLE_GATING=0 \
    --parameter fvp_mps2.telnetterminal0.start_telnet=1 \
    --parameter fvp_mps2.telnetterminal1.start_telnet=0 \
    --parameter fvp_mps2.telnetterminal2.start_telnet=0 \
    --parameter fvp_mps2.telnetterminal0.quiet=0 \
    --parameter fvp_mps2.telnetterminal1.quiet=1 \
    --parameter fvp_mps2.telnetterminal2.quiet=1 \
    --application cpu0=<build_dir>/bin/bl2.axf \
    --data cpu0=<default_build_dir>/bin/tfm_s_ns_signed.bin@0x10080000 \
    --data cpu0=<regresssion_build_dir>/bin/tfm_s_ns_signed.bin@0x10180000

Executing firmware upgrade on SSE 200 FPGA on MPS2 board
--------------------------------------------------------

::

    TITLE: Versatile Express Images Configuration File
    [IMAGES]
    TOTALIMAGES: 3                     ;Number of Images (Max: 32)
    IMAGE0ADDRESS: 0x00000000
    IMAGE0FILE: \Software\bl2.axf      ; BL2 bootloader
    IMAGE1ADDRESS: 0x10080000
    IMAGE1FILE: \Software\tfm_sign.bin ; TF-M default test binary blob
    IMAGE2ADDRESS: 0x10180000
    IMAGE2FILE: \Software\tfm_sig1.bin ; TF-M regression test binary blob

Executing firmware upgrade on Musca-B1 and Musca-S1 boards
----------------------------------------------------------
After the two images have been built, they can be concatenated to create the
combined image using ``srec_cat``:

- Linux::

    srec_cat bin/bl2.bin -Binary -offset 0xA000000 tfm_sign.bin -Binary -offset 0xA020000 tfm_sign_1.bin -Binary -offset 0xA100000 -o tfm.hex -Intel

- Windows::

    srec_cat.exe bin\bl2.bin -Binary -offset 0xA000000 tfm_sign.bin -Binary -offset 0xA020000 tfm_sign_1.bin -Binary -offset 0xA100000 -o tfm.hex -Intel

The following message will be shown in case of successful firmware upgrade,
notice that image with higher version number (``version=1.2.3.5``) is executed:

::

    [INF] Starting bootloader
    [INF] Image 0: version=1.2.3.4, magic= good, image_ok=0x3
    [INF] Image 1: version=1.2.3.5, magic= good, image_ok=0x3
    [INF] Booting image from the secondary slot
    [INF] Bootloader chainload address offset: 0xa0000
    [INF] Jumping to the first image slot
    [Sec Thread] Secure image initializing!

    #### Execute test suites for the Secure area ####
    Running Test Suite PSA protected storage S interface tests (TFM_PS_TEST_2XXX)...
    ...

Executing firmware upgrade on CoreLink SSE-200 Subsystem for MPS3 (AN524)
-------------------------------------------------------------------------

::

    TITLE: Arm MPS3 FPGA prototyping board Images Configuration File

    [IMAGES]
    TOTALIMAGES: 3                     ;Number of Images (Max: 32)

    IMAGE0UPDATE: AUTO                 ;Image Update:NONE/AUTO/FORCE
    IMAGE0ADDRESS: 0x00000000
    IMAGE0FILE: \SOFTWARE\bl2.bin      ;BL2 bootloader
    IMAGE1UPDATE: AUTO
    IMAGE1ADDRESS: 0x00040000
    IMAGE1FILE: \SOFTWARE\tfm_sig0.bin ;TF-M example application binary blob
    IMAGE2UPDATE: AUTO
    IMAGE2ADDRESS: 0x000C0000
    IMAGE2FILE: \SOFTWARE\tfm_sig1.bin ;TF-M regression test binary blob

RAM loading firmware upgrade
============================
To enable RAM loading, please set ``MCUBOOT_UPGRADE_STRATEGY`` to "RAM_LOAD"
(either in the configuration file or through the command line), and then specify
a destination load address in RAM where the image can be copied to and executed
from. The ``IMAGE_LOAD_ADDRESS`` macro must be specified in the target dependent
files, for example with Musca-S, its ``flash_layout.h`` file in the ``platform``
folder should include ``#define IMAGE_LOAD_ADDRESS #0xA0020000``

Executing firmware upgrade on Musca-S board
--------------------------------------------
After two images have been built, they can be concatenated to create the
combined image using ``srec_cat``:

- Linux::

    srec_cat bin/bl2.bin -Binary -offset 0xA000000 tfm_sign_old.bin -Binary -offset 0xA020000 tfm_sign_new.bin -Binary -offset 0xA100000 -o tfm.hex -Intel

- Windows::

    srec_cat.exe bin\bl2.bin -Binary -offset 0xA000000 tfm_sign_old.bin -Binary -offset 0xA020000 tfm_sign_new.bin -Binary -offset 0xA100000 -o tfm.hex -Intel

The following message will be shown in case of successful firmware upgrade when,
RAM loading is enabled, notice that image with higher version number
(``version=0.0.0.2``) is executed:

::

    [INF] Starting bootloader
    [INF] Image 0: version=0.0.0.1, magic= good, image_ok=0x3
    [INF] Image 1: version=0.0.0.2, magic= good, image_ok=0x3
    [INF] Image has been copied from the secondary slot in flash to SRAM address 0xA0020000
    [INF] Booting image from SRAM at address 0xA0020000
    [INF] Bootloader chainload address offset: 0x20000
    [INF] Jumping to the first image slot
    [Sec Thread] Secure image initializing!

--------------

****************************************
Integration with Firmware Update service
****************************************
The shim layer of the Firmware Update partition calls the APIs in
bootutil_misc.c to control the image status.

- Call ``boot_set_pending_multi()`` to make the image as a candidate image for
  booting.
- Call ``boot_set_confirmed_multi()`` to make the image as a permanent image.

.. Note::
    Currently, in direct-xip mode and ram-load mode, TF-M cannot get the
    information of which slot contains the running image from the bootloader.
    So the Firmware Update partition cannot decide where to write the new
    image. As a result, the firmware update service is not supported in
    direct-xip mode and ram-load mode.

*Copyright (c) 2018-2022, Arm Limited. All rights reserved.*
