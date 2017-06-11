
/*
 * USER STEP: Copy the whole lines in the root CA certificate file
 *
 *            Paste the lines in string format for the following array:
 */
const unsigned char root_ca_pem[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB\r\n"
"ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp\r\n"
"..."
"..."
"hnacRHr2lVz2XTIIM6RUthg/aFzyQkqFOFSDX9HoLPKsEdao7WNq\r\n"
"-----END CERTIFICATE-----\r\n";

const int rootCaLen = sizeof(root_ca_pem);

/*
 * USER STEP: Copy the whole lines in the cert.pem certificate file between
 *
 *            Paste the lines in string format for the following array:
 */
const unsigned char client_cert_pem[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB\r\n"
"..."
"..."
"mwMi4q9hN6MT9FGa0R0KwmD9y+o11yCbrNBbP0Gud/7m/y4r+09By8xr68Lz\r\n"
"-----END CERTIFICATE-----\r\n";
const int clientCertLen = sizeof(client_cert_pem);

/*
 * USER STEP: Copy the whole lines in the privateKey.pem certificate file between
 *
 *            Paste the lines in string format for the following array:
 */
const unsigned char client_private_key_pem[] =
"-----BEGIN RSA PRIVATE KEY-----\r\n"
"MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB\r\n"
"..."
"..."
"9bc8aGVeZ+mD3SYYJamB77XqH2kcCd5Axc2EckdL9ansf+S9v1FP\r\n"
"-----END RSA PRIVATE KEY-----\r\n";
const int clientPrivateKeyLen = sizeof(client_private_key_pem);
