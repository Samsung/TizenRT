Release Cadence and Process
===========================

Project Release Cadence
-----------------------

The project aims to do a release once every 6 months around April and November
time and are listed in the :ref:`releases/index:Future release plans`.
The releases are to be performed and tagged on a dedicated release branch.
The release process is initiated by an announcement in
`TF-M mailing list <https://lists.trustedfirmware.org/mailman/listinfo/tf-m>`_
and followed by the creation of a release branch.
Normal development on the main branch is not blocked and can be continued.
The testing will be performed on release candidates and depending on
issues found, additional candidates may be created to fix and retest the issues.

:doc:`The Platform owners </contributing/maintainers>` are expected to
verify their platforms and confirm the correct operations or provide fixes
in a timely manner to include in the release. The release notes will list
all verified platforms. The platforms in Trusted Firmware OpenCI is
automatically tested and any issues found shall be fixed.

After the final tag, the changes from the release branch will be back ported
to main branch.

.. uml::

    @startuml
        concise "main branch" as main
        concise "release branch v1.1.x" as rel1
        concise "release branch v1.2.x" as rel2

    @main
        -3 is development
        @0 <-> @8 : release cadence: ~6 months

    @rel1
        0 is rc1
        main -> rel1 : start
        +1 is rc2
        +1 is v1.1.0
        +1 is {-}
        rel1 -> main : back port
        +1 is v1.1.1 #pink
        +1 is {-}
        rel1 -> main : cherry-pick
        +3 is {hidden}

    @rel2
        8 is rc1
        main -> rel2 : start
        +1 is v1.2.0
        +1 is {-}

        rel2 -> main : back port

        @0 <-> @3 : release process
        @4 <-> @5 : hotfix

    @enduml

Although this document specifies the release cadence, this does not preclude
an adhoc release for specific project requirements.

Release Version Scheme
----------------------

Trusted Firmware-M uses a semantic versioning scheme. A version number is
compiled as a dot separated set of numbers:

**TF-Mv<MAJOR>.<MINOR>.<HOTFIX>**

- <MAJOR>: Major release version for significant feature and API changes.
- <MINOR>: Minor release version for incremental features and API changes.
- <HOTFIX>: Used only for backporting **critical bug fix/security patches**.

--------------

*Copyright (c) 2020, Arm Limited. All rights reserved.*
