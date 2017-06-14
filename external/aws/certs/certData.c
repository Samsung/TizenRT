
/*
 * USER STEP: Copy the whole lines in the root CA certificate file
 *
 *            Paste the lines in string format for the following array:
 */
const unsigned char root_ca_pem[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB\r\n"
"yjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQL\r\n"
"ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp\r\n"
"U2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxW\r\n"
"ZXJpU2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0\r\n"
"aG9yaXR5IC0gRzUwHhcNMDYxMTA4MDAwMDAwWhcNMzYwNzE2MjM1OTU5WjCByjEL\r\n"
"MAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQLExZW\r\n"
"ZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJpU2ln\r\n"
"biwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxWZXJp\r\n"
"U2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9y\r\n"
"aXR5IC0gRzUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCvJAgIKXo1\r\n"
"nmAMqudLO07cfLw8RRy7K+D+KQL5VwijZIUVJ/XxrcgxiV0i6CqqpkKzj/i5Vbex\r\n"
"t0uz/o9+B1fs70PbZmIVYc9gDaTY3vjgw2IIPVQT60nKWVSFJuUrjxuf6/WhkcIz\r\n"
"SdhDY2pSS9KP6HBRTdGJaXvHcPaz3BJ023tdS1bTlr8Vd6Gw9KIl8q8ckmcY5fQG\r\n"
"BO+QueQA5N06tRn/Arr0PO7gi+s3i+z016zy9vA9r911kTMZHRxAy3QkGSGT2RT+\r\n"
"rCpSx4/VBEnkjWNHiDxpg8v+R70rfk/Fla4OndTRQ8Bnc+MUCH7lP59zuDMKz10/\r\n"
"NIeWiu5T6CUVAgMBAAGjgbIwga8wDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8E\r\n"
"BAMCAQYwbQYIKwYBBQUHAQwEYTBfoV2gWzBZMFcwVRYJaW1hZ2UvZ2lmMCEwHzAH\r\n"
"BgUrDgMCGgQUj+XTGoasjY5rw8+AatRIGCx7GS4wJRYjaHR0cDovL2xvZ28udmVy\r\n"
"aXNpZ24uY29tL3ZzbG9nby5naWYwHQYDVR0OBBYEFH/TZafC3ey78DAJ80M5+gKv\r\n"
"MzEzMA0GCSqGSIb3DQEBBQUAA4IBAQCTJEowX2LP2BqYLz3q3JktvXf2pXkiOOzE\r\n"
"p6B4Eq1iDkVwZMXnl2YtmAl+X6/WzChl8gGqCBpH3vn5fJJaCGkgDdk+bW48DW7Y\r\n"
"5gaRQBi5+MHt39tBquCWIMnNZBU4gcmU7qKEKQsTb47bDN0lAtukixlE0kF6BWlK\r\n"
"WE9gyn6CagsCqiUXObXbf+eEZSqVir2G3l6BFoMtEMze/aiCKm0oHw0LxOXnGiYZ\r\n"
"4fQRbxC1lfznQgUy286dUV4otp6F01vvpX1FQHKOtw5rDgb7MzVIcbidJ4vEZV8N\r\n"
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
"MIIDWTCCAkGgAwIBAgIUSspy09t4c2O7tLrwenrkVvbbL8YwDQYJKoZIhvcNAQEL\r\n"
"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\r\n"
"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE3MDUyMjA4NDU1\r\n"
"OVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\r\n"
"ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAJ6dB0xuqqTw8ZBDmcKR\r\n"
"njKUzkOTr1hCzmiEC2Nkfw40KSTUsOapwOm6aAtgeXDLzSdQ8hVx3SS6DFJ+qCRK\r\n"
"b3JH0W7nkVyjEo1OfYVDcMGRvjQbfoPhy6W20oI5/ShS0ZtDNRN9qROsd7zZTy+g\r\n"
"iGxUdWkl2jaaMfRW27+UBXgCbwt1eooMLCFstv214AH/bsOlXbZGELYmD96xIy2k\r\n"
"cGUCnL+/hsTEdiGTnI/zzGG88GUCT/h3Ds5lH2LBWc9204DeYsRZqJey2WOPfNxO\r\n"
"XgWPOt7GCZplkCbYjcKeo+M1Zp2lL5FoFPccAEkCpWW9Zq53+n4qT2YTvCuPHhCo\r\n"
"nDsCAwEAAaNgMF4wHwYDVR0jBBgwFoAUhJX0fNZzdIyuUyzpoWgpcOpdrJkwHQYD\r\n"
"VR0OBBYEFMvqvps+Sv7W5LdbnI5TuAHCVdHDMAwGA1UdEwEB/wQCMAAwDgYDVR0P\r\n"
"AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBQfgpm28QgVULFiZ8YoEZ6aZ1L\r\n"
"r0YDxfUDxGGjLw1PnujYPxMyRkEGztekxsBG3GeDLyTohTQdmMqeV4T4OITwD8Ps\r\n"
"n4E59jq0qKoni1nPpGzQ+vBywTcyUJR5dieFeuc35fI/DKeFnWkYukrjQ9h8nqRQ\r\n"
"qB2VIwcbX8YxJHEK7yYGn2wTEkLREPd1R8KopNJDTXF3zXcUYb863xeD+wHipqkH\r\n"
"fSwZ0J2hLptTTia1nrPac9HVHp8jDrsgEpnqf3V5uBPmjFcyHvOrjDE4lK4/Wvmt\r\n"
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
"MIIEowIBAAKCAQEAnp0HTG6qpPDxkEOZwpGeMpTOQ5OvWELOaIQLY2R/DjQpJNSw\r\n"
"5qnA6bpoC2B5cMvNJ1DyFXHdJLoMUn6oJEpvckfRbueRXKMSjU59hUNwwZG+NBt+\r\n"
"g+HLpbbSgjn9KFLRm0M1E32pE6x3vNlPL6CIbFR1aSXaNpox9Fbbv5QFeAJvC3V6\r\n"
"igwsIWy2/bXgAf9uw6VdtkYQtiYP3rEjLaRwZQKcv7+GxMR2IZOcj/PMYbzwZQJP\r\n"
"+HcOzmUfYsFZz3bTgN5ixFmol7LZY4983E5eBY863sYJmmWQJtiNwp6j4zVmnaUv\r\n"
"kWgU9xwASQKlZb1mrnf6fipPZhO8K48eEKicOwIDAQABAoIBAH1nO4kDiilEUki0\r\n"
"GdJKXSAypG2rSxvmHdlmZLadkxLAzLLQ8lg8aEii13X/PaSfjUEDb5Li5mwOKIXk\r\n"
"m72tmDVrcLtVnIj4aUeNghIlwvVX7GC4SCuvDtqjFAZgILaL3Cr8VO9nD5F9b2HB\r\n"
"Eileq92yXO3YPpGjEdHUBoOOXVCGnXiyMRKSPxA4/EV2EcheWn6NW3WWGp1bthTL\r\n"
"305ommz7yYaldsznMX1KjNYJd0rbed1xKX0kDOEQa2Ux81lP2NpeuAkWQnuFFqm4\r\n"
"ea9F0jucPlYvIITph5zKMIEJUMS+OXtosd1WgqB1Pj5xeJi0GP0pLTU1Ahfpgr9k\r\n"
"m9pshZECgYEA++LooJE2Sai7mKZq/cw0bc0Zu7dTJ7i49wkYGYh/oo/F3A7bgKkX\r\n"
"KB8LIyFN1rL2diPS0pl4ktqCr9fKgkzlFSoFdRNVh+ZiUg70tfxC0QiUSfySr0Bx\r\n"
"cECrRFjL7iZJMxnP+CSwPU7QH9qBvKUABrwHLlMFXN+XFhTFxqoPeDkCgYEAoTQp\r\n"
"kzkXkUjIN/TBIc3XSC7dXG1EFYXQ9dUPO4E5xJ7k3aak02P2tgXpjXk6y5d4rGrh\r\n"
"KnckLmiWE1ysbEqmZ446snQ4TFbnGBt/9PzP4oiTeVosFjHieVfY9pKCoLhbhyMp\r\n"
"xP54dDjSMJmqoLyvWs/6jlRUG/9huVGJ+zK4MBMCgYEAryEe2LTd5+zXMdVTQk6+\r\n"
"jTEzGR7nRBDdGXVLOkV7aI1J5iy2VQg7v3uGBNDdC8+2KIHTzPVDrecbQiNo+5t3\r\n"
"E3X6AWKF700xQxh+8CEKW8f5Qk/HtO27cW6lJYoPj+8yqcLipAbuOlIXxm3oEj8u\r\n"
"e4A0BfNXITGcHJAoym3m/5ECgYAX7GPXI3vp5cFA/KPE3oV6gun7E4QtJYTTVuFB\r\n"
"1XW7RlF0y4xeitFMRihZBIrDOCGChQIRiUjnUkIjcXdtQs7bQ3JWlYTdX3BAVaRx\r\n"
"uSso1Vf9hHJSgOi5VV4viaK1M2bx8Tf7skAyD1SyPbviyt3DPmZSxRUOaqIYHI+7\r\n"
"pdVL3QKBgDg/nYmkLHOqDaVMIT5GnsY5/haxv2VM6ZGO1Dm3yCWr1R9RBhOjFD3H\r\n"
"MIEFyBt7KWeu4tZ9h3NoTA1gTcIrOpTOt5GZ0YOj6epqAkCaC+XfkJHXX/9kKAwK\r\n"
"9bc8aGVeZ+mD3SYYJamB77XqH2kcCd5Axc2EckdL9ansf+S9v1FP\r\n"
"-----END RSA PRIVATE KEY-----\r\n";
const int clientPrivateKeyLen = sizeof(client_private_key_pem);
