################################
Platform deprecation and removal
################################

********************************************
Process for Platform deprecation and removal
********************************************

A platform may need to be removed from upstream code due to lack of community
interest or it may have reached end of life. The below section calls out the
process for removing platform support from TF-M.

    1. An email to the TF-M mailing list proposing the removal of the platform.

    2. The merit of the proposal will be considered by the maintainers for a
       period of 4 weeks and community can express their opinion on the same
       during this time window. The platform owner can veto the proposal and
       incase of multiple platform owners with differing opinion or community
       having interest in the platform, then the project maintainer can work
       with the platform owner and use their discretion to decide on the
       proposal.

    3. Once a decision is made to remove the platform, the platform is
       considered to be in `deprecated` state as per platform support lifecyle
       defined here: "https://developer.trustedfirmware.org/w/collaboration/project-maintenance-process/".
       The platform will be marked as deprecated and the TF-M version after
       which it will be removed will also be mentioned. Suitable build time
       or runtime messages needs to be incorporated to the platform to warn
       about the `deprecation`.

    4. The project should strive to keep the `deprecated` platforms
       building/running till the removal. This relies on platform owners being
       still actively involved with the project and maintaining the platform.

    5. Although this will be the usual process for platform deprecation and
       eventual removal, the process still leaves room for the platform
       deprecation to be cancelled after it has been marked as `deprecated`
       due to evolving project and market requirements. This is left to
       consensus between project maintainers and platform owner/s.

    6. Once a platform has been removed, it can be added back in future and
       this would follow the same guidelines as for a new platform contribution.

****************************
List of Deprecated Platforms
****************************

The below list calls out platforms marked for deprecation according to the
above process and the platform will be removed soon after the mentioned
release.

+--------------------------------------+-----------+---------------------------+
| Deprecated Platform                  | Removed   | Comments                  |
|                                      | after     |                           |
|                                      | release   |                           |
+======================================+===========+===========================+
| mps2/an539                           | v1.2.0    | N.A                       |
+--------------------------------------+-----------+---------------------------+
| mps2/sse-200_aws                     | v1.3.0    | N.A                       |
+--------------------------------------+-----------+---------------------------+
| musca_a                              | v1.3.0    | N.A                       |
+--------------------------------------+-----------+---------------------------+
| arm/mps2/fvp_sse300                  | v1.4.0    | N.A                       |
+--------------------------------------+-----------+---------------------------+

--------------

*Copyright (c) 2020-2021, Arm Limited. All rights reserved.*
