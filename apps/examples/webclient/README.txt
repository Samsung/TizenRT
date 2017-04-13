examples/webclient
^^^^^^^^^^^^^^^^^^

  This is an example of webclient.
  It is executed by "webclient <method> <uri> (<entity> <encoding>)" command.
  <method> and <url> parameters should be given, <entity> and <encoding> parameters are optional.
  <method> is one if "GET", "POST", "PUT", "DELETE".
  <url> should start with "http://" or "https://".
  <entity> is a string value and its default value is NULL.
  The way of sending an entity is content-length by default.
  If <encoding> is "c", it sends an entity with chunked encoding.

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_WEBCLIENT

