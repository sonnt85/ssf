# Change Log

## 2.3.0

Features:
* #14 SOCKS proxy support (no authentication), cf. configuration file
* Remove `-b` client command line option (replaced by `circuit` configuration file key)
* Compatibility with `boost-1.63.0` and `openssl-1.0.2k`

Fixed bugs:
* #32 SOCKS v5 microservice
* #35 Use `reuse_addr` socket option on listener microservices (`stream_listener`, `datagram_listener`)
* Improve memory management
* Improve server stability

[Configuration file](README.md#configuration-file):
* Allow TLS parameters as buffers (`tls.ca_cert_buffer`, `tls.cert_buffer`, `tls.key_buffer`, `tls.dh_buffer` keys)
* Move circuit configuration into configuration file (`circuit` key)
* Add an option to save command line arguments (`arguments` key)

## 2.2.0

Features:
* Introducing [microservices](README.md#microservices), base components for client features
* #20 Enable/disable microservices in [configuration file](README.md#configuration-file)
* #24 Gateway ports. Allow client to specify network listening interface on client features:
  `-g` command line option or `gateway_ports` microservice options (`stream_listener` and `datagram_listener`)
* Server as relay only: `-R` command line option

Fixed bugs:
* #23 Delegate server hostname resolution to proxy
* #25 Fix server DOS on bogus connection
* #26 Fix a hang when the client stops during the connection stage

[Configuration file](README.md#configuration-file):
* `http_proxy.credentials.reuse_ntlm` and `http_proxy.credentials.reuse_kerb`: boolean instead of string

## 2.1.0

Features:
* TLS layer over circuit layer
* HTTP proxy support (CONNECT method), cf. configuration file
* HTTP proxy authentication support (Basic, Digest, NTLM [windows only], Negotiate), cf. configuration file
* Basic shell through socket (-X and -Y options)
* Server network interface option

Fixed bugs:
* Linux static link to libstdc++
* Linux dependency to GLIBC2.14 (memcpy)
* Stop behavior (signal instead of user input)
* Port forwarding listening side on localhost only

## 2.0.0

/!\ BC break with version 1.\*.\*

Features:
* New network layer based on SSF network framework

## 1.1.0
Features:
* ssfcp: file copy between client and server
* Rename executables:
  * SSF_Client -> ssfc
  * SSF_Server -> ssfs

Fixed bugs:
* Crash issue due to exception when resolving endpoint
* Exception safety
* Windows compilation warnings (64bits)

## 1.0.0
Features:
* Local and remote TCP port forwarding (-L and -R options)
* Local and remote UDP port forwarding (-U and -V options)
* Local and remote SOCKS server (-D and -F options)
* Native relay protocol (-b option)
* Multi platform (Windows, Linux and OSX)
* TLS connection with the strongest cipher-suites
* Standalone executables
