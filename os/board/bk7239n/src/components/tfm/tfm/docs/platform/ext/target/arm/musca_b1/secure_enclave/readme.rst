#################################
Musca-B1 Secure Enclave Specifics
#################################

************
Introduction
************

The Musca-B1 System-on-Chip contains two subsystems:

- SSE-200 subsystem to host the main application.
- CryptoIsland-300 subsystem can be used as a Secure Enclave (mentioned as *SE*
  in the document).

If TF-M is built with default configuration to the MUSCA-B1 platform only the
SSE-200 subsystem used. But if the ``FORWARD_PROT_MSG`` cmake flag is turned
on, the TF-M instance running on SSE-200 will communicate with the SE.

For more information you can check the
:doc:`Secure Enclave design document </technical_references/design_docs/secure_enclave_solution>`.

***********
System boot
***********

The desired boot flow would be to start up SE first at power on as SE is the
Root of Trust in the system, and SSE-200 should be started up by SE. But the
current Musca-B1 DAPLink FW releases the SSE-200 subsystem from reset first,
and it would require complex changes to modify this boot order. So an
additional SSE-200 BL0 component was added to the system to imitate that SE is
the subsystem started up first.

.. uml::

  @startuml

  title Implemented boot flow

  start
  :Power On button pressed.;
  :DAPLink Starts up SSE-200.;
  :SSE-200 BL0 starts up the SE (through SCC), then enters wait state.;
  :SE starts to run its MCUBoot image and authenticates all images (SE TF-M
  image and the combined SSE-200 image). If all images are valid, control jumps
  to SE’s TF-M image.;
  :SE's TF-M image starts up, when initialization finishes, SSE-200 is
  virtually released from reset by sending the start address of the combined
  SSE-200 image over MHU;
  :SSE-200 TF-M image starts up and synchronizes with SE over MHU;
  :SSE-200 and SE are ready to communicate;
  stop

  @enduml

.. Note::

   Without the SSE-200 BL0 component, the boot flow can be treated as a valid
   reference solution.

*****
Build
*****

To produce all the images, TF-M build needs to be executed twice:

- One build needed to create the SSE-200 images (BL0 and the combined SSE-200
  image containing TF-M and the non-secure application), target platform needs
  to be set to ``arm/musca_b1/sse_200`` and the ``FORWARD_PROT_MSG`` cmake flag
  also
  needs to be set.
- One build needed to create the SE images (MCUBoot and TF-M), target platform
  needs to be set to ``arm/musca_b1/secure_enclave``.

The order of the two builds is indifferent. The BL2 setting is mandatory for
both builds, but MCUBoot image is only built for the SE platform. The cmake
setup for the two builds must have the same debug profile (eg. debug, release).

To create a unified hex file:

- Windows::

    srec_cat.exe <SE build dir>\bin\bl2.bin -Binary -offset 0x1A020000 ^
                 <SE build dir>\bin\tfm_s_signed.bin -Binary -offset 0x1A200000 ^
                 <SSE-200 build dir>\bin\bl0.bin -Binary -offset 0x1A000000 ^
                 <SSE-200 build dir>\bin\tfm_s_ns_signed.bin -Binary -offset 0x1A260000 ^
                 -o tfm_sse200_w_se.hex -Intel

- Linux::

    srec_cat <SE build dir>/bin/bl2.bin -Binary -offset 0x1A020000 \
             <SE build dir>/bin/tfm_s_signed.bin -Binary -offset 0x1A200000 \
             <SSE-200 build dir>/bin/bl0.bin -Binary -offset 0x1A000000 \
             <SSE-200 build dir>/bin/tfm_s_ns_signed.bin -Binary -offset 0x1A260000 \
             -o tfm_sse200_w_se.hex -Intel

*****************
Known limitations
*****************
- Musca-B1 Secure Enclave cannot reset the whole SoC, only itself. So if SE
  does a system reset it will stuck in a state waiting for a handshake signal
  from SSE-200. (It will never come, as SSE-200 is not reseted in such a
  situation.)

--------------

*Copyright (c) 2020-2021, Arm Limited. All rights reserved.*
