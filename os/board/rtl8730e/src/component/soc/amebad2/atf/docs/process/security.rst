Security Handling
=================

Security Disclosures
--------------------

We disclose all security vulnerabilities we find, or are advised about, that are
relevant to Trusted Firmware-A. We encourage responsible disclosure of
vulnerabilities and inform users as best we can about all possible issues.

We disclose TF-A vulnerabilities as Security Advisories, all of which are listed
at the bottom of this page. Any new ones will, additionally, be announced as
issues in the project's `issue tracker`_ with the ``security-advisory`` tag. You
can receive notification emails for these by watching the "Trusted Firmware-A"
project at https://developer.trustedfirmware.org/.

Found a Security Issue?
-----------------------

Although we try to keep TF-A secure, we can only do so with the help of the
community of developers and security researchers.

If you think you have found a security vulnerability, please **do not** report it
in the `issue tracker`_. Instead send an email to
trusted-firmware-security@arm.com

Please include:

* Trusted Firmware-A version (or commit) affected

* A description of the concern or vulnerability

* Details on how to replicate the vulnerability, including:

  - Configuration details

  - Proof of concept exploit code

  - Any additional software or tools required

We recommend using :download:`this PGP/GPG key <./security-reporting.asc>` for
encrypting the information. This key is also available at
http://keyserver.pgp.com and LDAP port 389 of the same server.

The fingerprint for this key is:

::

    1309 2C19 22B4 8E87 F17B FE5C 3AB7 EFCB 45A0 DFD0

If you would like replies to be encrypted, please provide your public key.

Please give us the time to respond to you and fix the vulnerability before going
public. We do our best to respond and fix any issues quickly. We also need to
ensure providers of products that use TF-A have a chance to consider the
implications of the vulnerability and its remedy.

Afterwards, we encourage you to write-up your findings about the TF-A source
code.

Attribution
-----------

We will name and thank you in the :ref:`Change Log & Release Notes` distributed with the source
code and in any published security advisory.

Security Advisories
-------------------

+-----------+------------------------------------------------------------------+
| ID        | Title                                                            |
+===========+==================================================================+
|  |TFV-1|  | Malformed Firmware Update SMC can result in copy of unexpectedly |
|           | large data into secure memory                                    |
+-----------+------------------------------------------------------------------+
|  |TFV-2|  | Enabled secure self-hosted invasive debug interface can allow    |
|           | normal world to panic secure world                               |
+-----------+------------------------------------------------------------------+
|  |TFV-3|  | RO memory is always executable at AArch64 Secure EL1             |
+-----------+------------------------------------------------------------------+
|  |TFV-4|  | Malformed Firmware Update SMC can result in copy or              |
|           | authentication of unexpected data in secure memory in AArch32    |
|           | state                                                            |
+-----------+------------------------------------------------------------------+
|  |TFV-5|  | Not initializing or saving/restoring PMCR_EL0 can leak secure    |
|           | world timing information                                         |
+-----------+------------------------------------------------------------------+
|  |TFV-6|  | Trusted Firmware-A exposure to speculative processor             |
|           | vulnerabilities using cache timing side-channels                 |
+-----------+------------------------------------------------------------------+
|  |TFV-7|  | Trusted Firmware-A exposure to cache speculation vulnerability   |
|           | Variant 4                                                        |
+-----------+------------------------------------------------------------------+
|  |TFV-8|  | Not saving x0 to x3 registers can leak information from one      |
|           | Normal World SMC client to another                               |
+-----------+------------------------------------------------------------------+

.. _issue tracker: https://developer.trustedfirmware.org/project/board/1/
.. _this PGP/GPG key: security-reporting.asc

.. |TFV-1| replace:: :ref:`Advisory TFV-1 (CVE-2016-10319)`
.. |TFV-2| replace:: :ref:`Advisory TFV-2 (CVE-2017-7564)`
.. |TFV-3| replace:: :ref:`Advisory TFV-3 (CVE-2017-7563)`
.. |TFV-4| replace:: :ref:`Advisory TFV-4 (CVE-2017-9607)`
.. |TFV-5| replace:: :ref:`Advisory TFV-5 (CVE-2017-15031)`
.. |TFV-6| replace:: :ref:`Advisory TFV-6 (CVE-2017-5753, CVE-2017-5715, CVE-2017-5754)`
.. |TFV-7| replace:: :ref:`Advisory TFV-7 (CVE-2018-3639)`
.. |TFV-8| replace:: :ref:`Advisory TFV-8 (CVE-2018-19440)`

--------------

*Copyright (c) 2019, Arm Limited. All rights reserved.*
