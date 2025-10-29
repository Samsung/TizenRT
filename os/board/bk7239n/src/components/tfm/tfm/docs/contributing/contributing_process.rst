Contributing Process
====================

Contributions to the TF-M project need to follow the process below.

.. Note::

   Please contact :doc:`maintainers </contributing/maintainers>` for any
   questions.

- Subscribe to `TF-M development mailing list
  <https://lists.trustedfirmware.org/mailman3/lists/tf-m.lists.trustedfirmware.org>`_
  if not subscribed already.
- Refer to the `Roadmap
  <https://developer.trustedfirmware.org/w/tf_m/planning>`_ or send a mail to
  the tf-m@lists.trustedfirmware.org to check if this work is already
  planned/progresses elsewhere.
- Create a task in `Phabricator
  <https://developer.trustedfirmware.org/maniphest>`_, put as many details as
  possible in the description. Add 'Trusted Firmware M' in the 'Tags' field.
- For non-trivial changes, need to follow the design proposal process
  :doc:`Design Proposal Guideline </contributing/tfm_design_proposal_guideline>`
  for the TF-M project.
- After the design has been accepted by the maintainer(s), a corresponding
  patch should be posted; follow guidelines below:

  - Clone the TF-M code on your own machine from `TF-M git repository
    <https://git.trustedfirmware.org/TF-M/trusted-firmware-m.git>`_.
  - Follow the :doc:`TF-M getting started </getting_started/tfm_getting_started>`,
    :doc:`Build Instructions </technical_references/instructions/tfm_build_instruction>` and
    :doc:`Coding Guide </contributing/coding_guide>` for the TF-M project.
  - Make your changes in logical chunks to help reviewers. Each commit should
    be a separate review and either work properly or be squashed after the
    review and before merging.
  - Update documentation in docs/ folder if needed.
  - Test your changes and add details to the commit description.
  - The code is accepted under :doc:`DCO </contributing/dco>`, Developer
    Certificate of Origin, so you must add following fields to your
    commit description:

    .. code-block:: text

       Author: Full Name <email address>
       Signed-off-by: Full Name <email address>

    .. Note::

       Sign off authority needs to adhere to the [DCO](./dco.txt) rules.

  - You must add a `Change-Id
    <https://review.trustedfirmware.org/Documentation/user-changeid.html>` to
    the commit message, which can be generated any way you like (e.g. from the
    SHA of the commit), or use the commit hook.
  - Create a user in the `TF-M Gerrit <https://review.trustedfirmware.org>`,
    then add `SSH keys or HTTP credentials
    <https://review.trustedfirmware.org/settings/>`.
  - Submit your patch for review.

    .. code-block:: shell

       git push ssh://review.trustedfirmware.org:29418/TF-M/trusted-firmware-m.git HEAD:refs/for/master

    or

    .. code-block:: shell

       git push https://review.trustedfirmware.org/TF-M/trusted-firmware-m.git HEAD:refs/for/master

- Add relevant :doc:`maintainers </contributing/maintainers>` for reviewing
  the patch.
- You may be asked to provide further details or make additional changes.
- You can discuss further with maintainer(s) by directly over email if
  necessary.
- Once the change is approved by maintainers, the patch will be merged by the
  maintainer.
- Mark the task as 'resolved' after patch is merged.

--------------

*Copyright (c) 2017-2022, Arm Limited. All rights reserved.*
