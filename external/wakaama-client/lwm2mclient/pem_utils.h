#ifdef _PEM_UTILS_H_
#define _PEM_UTILS_H_

bool convert_pem_privatekey_to_der(const char *private_key_pem, char **private_key_der, uint16_t *len);
bool convert_pem_x509_to_der(const char *cert_buffer_pem, char **cert_buffer_der, uint16_t *len)

#endif
