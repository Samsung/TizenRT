examples
^^^^^^^^

  Selecting examples:

    The examples directory contains several sample applications that
    can be linked with TinyAra.  The specific example is selected in the
    configs/<board-name>/defconfig file via the CONFIG_EXAMPLES_xyz
    setting where xyz is the name of the example. For example,

      CONFIG_EXAMPLES_HELLO=y

    Selects the examples/hello example.

  Selecting main examples:

    You can enable several examples but you should set one as an entry point
    among them. There are several CONFIG_ENTRY_xyz in Kconfig_ENTRY. When you
    enable some example, then you can see that config in menuconfig and can select
    it as an entry point. If you select manual entry, you can write an entry point
    directly.

For more information for each exmaple, see README on each folder.
