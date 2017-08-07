examples/webclient
^^^^^^^^^^^^^^^^^^

  This is an example of webclient.
  It is executed by "webclient <method> <uri> [options]" command.
  <method> and <url> parameters should be given, [options] parameters are optional.
  <method> is one if "GET", "POST", "PUT", "DELETE".
  <url> should start with "http://" or "https://".

  [options] parameter support several different options.
   async=1          : Enable asynchronous mode (default is synchronous)
   chunked=1        : Enable chunked encoding (default is disabled)
   entity=DATA      : Input entity data (default is NULL)
   test_entity=SIZE : Input test entity dummy data (default is 0)

  The way of sending an entity is content-length by default.

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_WEBCLIENT
