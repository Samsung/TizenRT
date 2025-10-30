INSTALL_NAME_DIR
----------------

macOS directory name for installed targets.

``INSTALL_NAME_DIR`` is a string specifying the directory portion of the
"install_name" field of shared libraries on macOS to use in the
installed targets.

This property is initialized by the value of the variable
:variable:`CMAKE_INSTALL_NAME_DIR` if it is set when a target is
created.
