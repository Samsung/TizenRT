examples/protobuf_enc
^^^^^^^^^^^^^^^^^

  This directory contains example code that uses Protocol Buffers to test serailize/deserialize

  TizenRT configuration prerequisites:

    CONFIG_HAVE_CXX -- Enable C++ Support

  Optional TizenRT configuration settings:

    CONFIG_HAVE_CXXINITIALIZE -- Enable support for static constructors
      (may not be available on all platforms).

  TizenRT configuration settings specific to this example:

    CONFIG_PROTOBUF=y
    CONFIG_EXAMPLES_PROTOTEST=y

  Also needed:

    CONFIG_HAVE_CXX=y

  And you may have to tinker with the following to get libxx to compile
  properly:

    CONFIG_CXX_NEWLONG=y or =n

  The argument of the 'new' operators should take a type of size_t.  But size_t
  has an unknown underlying.  In the TizenRT sys/types.h header file, size_t
  is typed as uint32_t (which is determined by architecture-specific logic).
  But the C++ compiler may believe that size_t is of a different type resulting
  in compilation errors in the operator.  Using the underlying integer type
  Instead of size_t seems to resolve the compilation issues.
