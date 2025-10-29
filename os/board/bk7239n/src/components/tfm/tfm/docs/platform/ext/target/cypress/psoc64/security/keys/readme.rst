##################
Image signing keys
##################

- TFM_S_KEY.json      - private OEM key for signing CM0P image
- TFM_S_KEY_PRIV.pem  - private OEM key for signing CM0P image in PEM format
- TFM_NS_KEY.json     - private OEM key for signing CM4 image
- TFM_NS_KEY_PRIV.pem - private OEM key for signing CM4 image in PEM format

The keys included in the repository are for reference and development only.
DO NOT USE THESE KEYS IN ANY ACTUAL DEPLOYMENT!

Use cysecuretools to generate new key pairs defined by the policy file,
for example:

.. code-block:: bash

    cd platform/ext/target/cypress/psoc64/security
    cysecuretools -t cy8ckit-064s0s2-4343w init
    cysecuretools -t cy8ckit-064s0s2-4343w -p policy/policy_multi_CM0_CM4_tfm.json create-keys


Signing keys have to be provisioned to the board:

.. code-block:: bash

    cysecuretools -t cy8ckit-064s0s2-4343w -p policy/policy_multi_CM0_CM4_tfm.json re-provision-device


Please refer to cysecuretools documentation and cypress_psoc64_spec.rst for
more details.
