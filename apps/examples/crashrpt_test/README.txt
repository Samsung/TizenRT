apps/examples/crashrpt_test
===========================

Demonstration of the crash diagnostic capture driver, /dev/crashrpt.

What it does
------------

The example holds a body of diagnostic data in memory and registers a
reporter thread that serialises all of it to a file when the system is about
to assert.

  500 nodes of 48 bytes
   50 nodes of 64 bytes
  each node carries a message string of up to 255 bytes

The message is not stored inside the node - the node holds a pointer to it.
On disk the message follows its node, so both end up in the report.

Sequence
--------

  1. The application builds the node data at start-up, while the system is
     healthy.
  2. The reporter thread registers through /dev/crashrpt and parks in
     fin_wait().
  3. An assert occurs.  At the very top of up_assert(), before any crash
     handling begins, the kernel wakes the reporter and waits.
  4. The reporter writes the report and tells the driver it is finished.
  5. Assert handling continues and the board reboots.

Why the data is built up front
------------------------------

Allocating several hundred blocks while the system is on its way down is
exactly the kind of work that should not be attempted.  Building the data
during normal operation also mirrors how a real product would accumulate
diagnostic state.

Temporary file and rename
-------------------------

The report is written to

  <dir>/crashrpt.tmp

and only after the write has completed and been flushed with fsync() is it
renamed to

  <dir>/crashrpt_r<n>.bin

A reader therefore never observes a partially written report.  The final
name either exists and is complete, or it does not exist at all.  If any
step fails the temporary file is removed.

File format
-----------

  Header, 32 bytes
    magic     u32   0x54505243, "CRPT"
    version   u32
    reason    u32   the reason code passed by the kernel
    n48       u32   number of 48 byte nodes
    n64       u32   number of 64 byte nodes
    sz48      u32   sizeof the 48 byte node
    sz64      u32   sizeof the 64 byte node
    reserved  u32

  Then n48 records:
    node      48 bytes, raw
    msglen    u8
    msg       msglen bytes, no terminator

  Then n64 records:
    node      64 bytes, raw
    msglen    u8
    msg       msglen bytes, no terminator

The message pointer inside each node is zeroed in the copy that is written,
since its value has no meaning outside the running process.  This also keeps
the output reproducible.

A one byte length field is sufficient because the message is capped at 255
bytes.

Largest possible report:

  32 + 500 * (48 + 1 + 255) + 50 * (64 + 1 + 255) = 168032 bytes

Actual size is smaller, as message lengths cycle through 32, 96, 160 and 255
so that the boundary case is exercised regularly.

Usage
-----

From TASH:

  crashrpt_test init      Build the data and register the reporter
  crashrpt_test status    Show current state and the largest possible report
  crashrpt_test crash     Raise an assert to exercise the whole path

A normal run looks like:

  TASH>> crashrpt_test init
  crashrpt_test: built 500 nodes of 48 bytes and 50 nodes of 64 bytes
  crashrpt_test: reporter registered and waiting
  crashrpt_test: ready, run 'crashrpt_test crash' to exercise it

  TASH>> crashrpt_test crash
  crashrpt_test: asserting now, the reporter should run before reboot
  crashrpt_test: woken, reason 2
  crashrpt_test: report written to /mnt/crashrpt_r0.bin
  ... assert output follows, then the board reboots ...

Configuration
-------------

  CONFIG_CRASH_REPORT                     required, kernel side
  CONFIG_EXAMPLES_CRASHRPT_TEST           this example
  CONFIG_EXAMPLES_CRASHRPT_TEST_DIR       where the report is written
  CONFIG_EXAMPLES_CRASHRPT_TEST_PRIORITY  reporter priority, must be high
  CONFIG_EXAMPLES_CRASHRPT_TEST_STACKSIZE reporter stack

The directory must be a writable mount point.  On a real product a raw
partition or an append only log is safer than a mounted filesystem, since
writing to one straight after a crash risks corrupting it.

Notes
-----

The node sizes are checked at run time before anything else happens.  They
depend on the pointer size, so on a target where a pointer is not four bytes
the structures would need adjusting and the check will say so rather than
silently writing a different format.

Hardware faults and asserts raised from interrupt context do not reach the
reporter.  By then the exception context has been taken and the scheduler
cannot run it.  Those cases need data published before the fault.
