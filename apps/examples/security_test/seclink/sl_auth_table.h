// command, type, handler
SL_AUTH_TEST_POOL("ecdh",     SL_AUTH_TYPE_ECDH,              sl_handle_auth_ecdh)
SL_AUTH_TEST_POOL("x25519",   SL_AUTH_TYPE_X25519,            sl_handle_auth_x25519)
SL_AUTH_TEST_POOL("ecdsav",   SL_AUTH_TYPE_ECDSA_VERIFY,      sl_handle_auth_ecdsa_verify)
SL_AUTH_TEST_POOL("ecdsas",   SL_AUTH_TYPE_ECDSA_SIGNATURE,   sl_handle_auth_ecdsa_signature)
SL_AUTH_TEST_POOL("ed25519v", SL_AUTH_TYPE_ED25519_VERIFY,    sl_handle_auth_ed25519_verify)
SL_AUTH_TEST_POOL("ed25519s", SL_AUTH_TYPE_ED25519_SIGNATURE, sl_handle_auth_ed25519_signature)
