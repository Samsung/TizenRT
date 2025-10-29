#######################
SQUAD metrics dashboard
#######################

:Author: Hugo L'Hostis
:Organization: Arm Limited
:Contact: hugo.lhostis@arm.com

******************
SQUAD Presentation
******************

Software Quality Dashboard (SQUAD) is a tool used by Trusted Firmware-M (TF-M)
to keep track of some metrics for the project. It is a Linaro project (see here
a link to the `SQUAD Documentation`_).

For TF-M the purpose of having such a tool available is to have a history on
some metrics of the project for different configurations of TF-M.

The TF-M SQUAD is available here : `TFM's SQUAD`_. There are several
configurations and metrics that can be selected, here is a link with some of
them already selected :
`SQUAD parametered for Default, profileS and MinsizeProfileS`_.

**************************
Metrics and configurations
**************************

The metrics sent to the dashboard are currently the memory footprint
measurements from the TFM project calculated by the arm-none-eabi-size
function for the 2 files bl2.axf and tfm_s.axf :

   - Text section size.
   - bss section size.
   - data total size.
   - Total file size.

Each metric sent to the dashboard can be compared for different configurations
used. The configurations available currently are the following :

   - Default
   - CoreIPC
   - CoreIPCTfmLevel2
   - DefaultProfileS
   - DefaultProfileM
   - DefaultProfileL
   - MinSizeProfileS (DefaultProfileS with MinsizeRel Cmake type)

For all of the configurations, tests are disabled and the release build type is
used. For more details about the configurations and how the rest of the options
are set for each of them, see
:ref:`Migration from legacy buildsystem`.
More configurations and metrics could be added to the dashboard in the future.

For each metric sent to the SQUAD dashboard, the metric must be linked with the
state of the TF-M repository at the time of the build. On the dashboard this is
visible from a string of numbers in the abscissa of the graphs displayed in
SQUAD. This is the change ID of the latest commit used for the build. This
means that in the case of 2 consecutive days with no new commit for TF-M, no
new data points will be created.

**************
CI integration
**************

The data is currently sent by the `TFM CI's nightly build`_.

The parameter "SQUAD_CONFIGURATIONS" of the CI build is what will trigger the
configurations sent to the SQUAD dashboard. It should be set to the
configurations's names separated by a comma, example :

``SQUAD_CONFIGURATIONS = Default,DefaultProfileS``

In this case, the 2 configurations Default and DefaultProfileS will be enabled
and the data for those (and only those 2) will be sent to the dashboard.
This is case insensitive and will ignore any space between the configurations.
All the files manipulating the data and sending the data is available in the
`tf-m-ci-scripts repo`_.

The script `memory_footprint.py`_ is launched by the CI nightly build, it
gathers and sends the data.

*********************
Adding a new platform
*********************

Currently, all the data sent is from AN521 builds. To add a new platform to the
dashboard :

1. If the new platform is not already tested by the CI nightly build, it needs
to be added.

2. The ``memory_footprint.py`` file in the `tf-m-ci-scripts repo`_ has to be
modified to recognise the new platform.

3. If ``memory_footprint.py`` detects the new platform with a reference
configuration, it should send a new metric with a different name to the
existing ones containing the platform's name.

The data will then be accessible by selecting the correct metrics on the
dashboard.


.. _SQUAD Documentation: https://squad.readthedocs.io/en/latest/index.html
.. _TFM's SQUAD: https://qa-reports.linaro.org/tf/tf-m/metrics/
.. _SQUAD parametered for Default, profileS and MinsizeProfileS: https://qa-reports.linaro.org/tf/tf-m/metrics/?environment=Default&environment=DefaultProfileS&environment=MinSizeProfileS&metric=tfms_size&metric=tfms_data&metric=tfms_bss&metric=bl2_size&metric=bl2_data&metric=bl2_bss&range_tfms_size=0,100&range_tfms_data=0,100&range_tfms_bss=0,100&range_bl2_size=0,100&range_bl2_data=0,100&range_bl2_bss=0,100
.. _TFM CI's nightly build: https://ci.trustedfirmware.org/view/TF-M/job/tf-m-nightly/
.. _tf-m-ci-scripts repo: https://review.trustedfirmware.org/admin/repos/ci/tf-m-ci-scripts
.. _memory_footprint.py: https://git.trustedfirmware.org/ci/tf-m-ci-scripts.git/tree/memory_footprint.py?h=refs/heads/master

--------------

*Copyright (c) 2021, Arm Limited. All rights reserved.*
