# https://wiki.osdev.org/TLS_Handshake
meta:
  id: tls_record
  endian: be
  license: MIT

seq:
  - id: type
    type: u1

  - id: version
    type: u2

  - id: length
    type: u2
