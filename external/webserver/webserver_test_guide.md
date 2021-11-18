# Python Client to stress test Web Server

### Features

- Protocols supported: HTTPS & HTTP
- HTTP Commands
    - GET
    - PUT
    - POST
    - DELETE
- HTTP Header
    - Supports Keep-alive
    - Chunked Transfer
    - Trailer Header

## Prerequisites

### Python3
**Python modules**

- Requests

` pip install requests`

- Prettytable

` pip install prettytable`

### Source Code
https://github.com/Samsung/TizenRT
Branch: master

### Certificate for HTTPS protocol
**Certificate Generation**
1. Create a san.cnf file with following info.

    `[req]
    default_bits  = 2048
    distinguished_name = req_distinguished_name
    req_extensions = req_ext
    x509_extensions = v3_req
    prompt = no[req_distinguished_name]
    countryName = XX
    stateOrProvinceName = N/A
    localityName = N/A
    organizationName = Self-signed certificate
    commonName = 120.0.0.1: Self-signed certificate[req_ext]
    subjectAltName = @alt_names[v3_req]
    subjectAltName = @alt_names[alt_names]
    IP.1 = 127.0.0.1` 
2. Replace the IP address of **commonName** and **IP.1** with the IP address of the webserver (test board)
3. Run the following commands to generate the certificate and key files

`openssl ecparam -out key.pem -name secp256r1 -genkey`

`openssl req -new -key key.pem -x509 -nodes -days 365 -out cert.pem -config san.cnf `

**Add Certificate in TP1x**
 1. Open the generated **cert.pem** and **key.pem** files in a text editor
 2. Open **apps/examples/webserver/webserver_main.c**
 3. Update value of **ca_crt_rsa[]** and **srv_crt_rsa[]** with the content of cert.pem
 4. Update values of **srv_key_rsa[]** with the content of key.pem

### TP1x Build Configuration

> rtl8721csm / security_hal_test_tz

### Menuconfig Setup
**Enable web server**
> Networking support > Protocols > Webserver 

> Application Configuration > Examples > Webserver example

## Client setup

- The generated cert.pem, key.pem and client.py should be in the same directory
- Update variable **url** in client.py with WebServer IP Address

**Steps to Test Web Server**
Connect TP1x and testing machine to same router
Start web server - webserver start none
Execute `python3 client.py`

**Limitations**
- Certificates used in HTTPS require TP1x board IP address as input. This requires connecting board to network and then again build image.
