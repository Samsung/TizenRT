##########################
Log system design document
##########################

:Author: Shawn Shan
:Organization: Arm Limited
:Contact: shawn.shan@arm.com

**********
Background
**********

In current TF-M log system, the SPM and Secure partitions share the same log
APIs and implementations. While TF-M is keep evolving, the requirements for the
log system has changed:

  - Log level is required for both SPM and SP sides to output message in
    different scenarios.
  - SPM only needs simple log format such as hex and string, while SP needs rich
    formatting.
  - Distinctions on log output between SPM and SP are required.

A new log system is needed to separate the SPM and Secure partitions and to
meet their different requirements.

******
Design
******

To allow customizable configurations, the log interfaces are defined as macros.
The macros are easy to be forwarded or even empty. When SPM trying to output
message and a value, it relies on a wrapper function, and finally output the
formatted message by the HAL API.

The design principles of TF-M log system:

  - Configurable log levels.
  - Separated SPM and SP log implementations.
  - Platforms provide log HAL implementations.

SPM Log System
==============

Level Control
-------------
Three log levels for SPM log system are defined:

  - TFM_SPM_LOG_LEVEL_DEBUG
  - TFM_SPM_LOG_LEVEL_INFO
  - TFM_SPM_LOG_LEVEL_ERROR
  - TFM_SPM_LOG_LEVEL_SILENCE

Then a macro ``TFM_SPM_LOG_LEVEL`` is defined as an indicator, it should
be equal to one of the four log levels.

API Definition
--------------
The following three APIs LOG APIs output the given 'msg' with hexadecimal
formatted 'val' together. These APIs provide constrained ability to output
numbers inside SPM. The 'msg' can be skipped with giving an empty string like
"". And these APIs supports constant 'msg' string only, giving a runtime string
as parameter 'msg' would potentially cause a runtime error.

  SPMLOG_DBGMSGVAL(msg, val);

  SPMLOG_INFMSGVAL(msg, val);

  SPMLOG_ERRMSGVAL(msg, val);

A C-function needs to work as an underlayer for these APIs as string formatting
is required. Check 'spm_log_msgval' for details.

.. code-block:: c

  /**
   * brief Output the given message plus one value as hexadecimal. The message
   *       can be skipped if the 'msg' is 'NULL' or 'len' equals 0. The
   *       formatted hexadecimal string for 'value' has a '0x' prefix and
   *       leading zeros are not stripped. This function rely on HAL API
   *       'tfm_hal_output_spm_log' to output the formatted string.
   *
   * \param[in]  msg    A string message
   * \param[in]  len    The length of the message
   * \param[in]  value  A value need to be output
   *
   * \retval >=0        Number of chars output.
   * \retval <0         TFM HAL error code.
   */
  int32_t  spm_log_msgval(const char *msg, size_t len, uint32_t value)

The following three APIs output a message in string.

  SPMLOG_DBGMSG(msg);

  SPMLOG_INFMSG(msg);

  SPMLOG_ERRMSG(msg);

Here is a table about the effective APIs with different SPM log level.

+------------------+-------------------------+---------------------------+---------------------------+-----------------------------+
|                  | TFM_SPM_LOG_LEVEL_DEBUG | TFM_SPM_LOG_LEVEL_INFO    | TFM_SPM_LOG_LEVEL_ERROR   | TFM_SPM_LOG_LEVEL_SILENCE   |
+==================+=========================+===========================+===========================+=============================+
| SPMLOG_DBGMSGVAL |           Yes           |             No            |             No            |            No               |
+------------------+-------------------------+---------------------------+---------------------------+-----------------------------+
| SPMLOG_INFMSGVAL |           Yes           |             Yes           |             No            |            No               |
+------------------+-------------------------+---------------------------+---------------------------+-----------------------------+
| SPMLOG_ERRMSGVAL |           Yes           |             Yes           |             Yes           |            No               |
+------------------+-------------------------+---------------------------+---------------------------+-----------------------------+
| SPMLOG_DBGMSG    |           Yes           |             No            |             No            |            No               |
+------------------+-------------------------+---------------------------+---------------------------+-----------------------------+
| SPMLOG_INFMSG    |           Yes           |             Yes           |             No            |            No               |
+------------------+-------------------------+---------------------------+---------------------------+-----------------------------+
| SPMLOG_ERRMSG    |           Yes           |             Yes           |             Yes           |            No               |
+------------------+-------------------------+---------------------------+---------------------------+-----------------------------+

HAL API
-------
Define HAL API for SPM log system:

.. code-block:: c

  /* SPM log HAL API */
  int32_t tfm_hal_output_spm_log(const char *str, uint32_t len);

Take debug message as an example:

.. code-block:: c

  /* For debug message */
  #define SPMLOG_DBGMSG(msg) tfm_hal_output_spm_log(msg, sizeof(msg))
  /* For debug message with a value */
  #define SPMLOG_DBGMSGVAL(msg, val) spm_log_msgval(msg, sizeof(msg), val)

Partition Log System
====================
Partition log outputting required rich formatting in particular cases. There is
a customized print inside TF-M(``tfm_sp_log_printf``), and it is wrapped as
macro.

Level Control
-------------
Three log levels for partition log system are defined:

  - TFM_PARTITION_LOG_LEVEL_DEBUG
  - TFM_PARTITION_LOG_LEVEL_INFO
  - TFM_PARTITION_LOG_LEVEL_ERROR
  - TFM_PARTITION_LOG_LEVEL_SILENCE

Then a macro ``TFM_PARTITION_LOG_LEVEL`` is defined as an indicator. It should
be equal to one of the four log levels and it is an overall setting for all
partitions.

Log Format
----------
Compared to SPM, SP log API supports formatting. Similar to ``printf``, these
log APIs use a format outputting to output various type of data:

.. code-block:: c

  %d - decimal signed integer
  %u - decimal unsigned integer
  %x - hex(hexadecimal)
  %c - char(character)
  %s - string

API Definition
--------------
Define partition log APIs:

  LOG_DBGFMT(...);

  LOG_INFFMT(...);

  LOG_ERRFMT(...);

Here is a table about the effective APIs with different partition log level.

+------------+-------------------------------+---------------------------------+---------------------------------+---------------------------------+
|            | TFM_PARTITION_LOG_LEVEL_DEBUG | TFM_PARTITION_LOG_LEVEL_INFO    | TFM_PARTITION_LOG_LEVEL_ERROR   | TFM_PARTITION_LOG_LEVEL_SILENCE |
+============+===============================+=================================+=================================+=================================+
| LOG_DBGFMT |              Yes              |                No               |                No               |               No                |
+------------+-------------------------------+---------------------------------+---------------------------------+---------------------------------+
| LOG_INFFMT |              Yes              |                Yes              |                No               |               No                |
+------------+-------------------------------+---------------------------------+---------------------------------+---------------------------------+
| LOG_ERRFMT |              Yes              |                Yes              |                Yes              |               No                |
+------------+-------------------------------+---------------------------------+---------------------------------+---------------------------------+

HAL API
-------
Please refers to the HAL design document.

***********
Log Devices
***********
In most of the cases, a serial device could be used as a log device. And in
other particular cases, a memory-based log device could be applied as well.
These log device interfaces are abstracted into HAL APIs.

.. note::

  It is not recommended to re-use the same HAL for both SPM and SP log
  outputting especially when SPM and SP run under different privileged level,
  which makes them have a different information confidential level. Unless:

  - The SPM log outputting would be disabled as silence in the release version.

--------------

*Copyright (c) 2020, Arm Limited. All rights reserved.*
