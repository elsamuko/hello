meta:
  id: tls_server_hello
  endian: be
  license: MIT

seq:
  - id: type
    contents: [0x02]

  - id: size
    type: b24

  - id: version
    type: version

  - id: random
    type: random

  - id: session_id
    type: session_id

  - id: cipher_suite
    type: u2

  - id: compression_method
    type: u1

  - id: extensions
    type: extensions
    if: _io.eof == false

types:
  version:
    seq:
      - id: major
        type: u1

      - id: minor
        type: u1

  random:
    seq:
      - id: gmt_unix_time
        type: u4

      - id: random
        size: 28

  session_id:
    seq:
      - id: len
        type: u1

      - id: sid
        size: len

  extensions:
    seq:
      - id: len
        type: u2

      - id: extensions
        type: extension
        repeat: eos

  extension:
    seq:
      - id: type
        type: u2

      - id: len
        type: u2

      - id: body
        size: len
        type:
          switch-on: type
          cases:
            0: sni
            16: alpn

  sni:
    seq:
      - id: list_length
        type: u2

      - id: server_names
        type: server_name
        repeat: eos

  server_name:
    seq:
      - id: name_type
        type: u1

      - id: length
        type: u2

      - id: host_name
        size: length

  alpn:
    seq:
      - id: ext_len
        type: u2

      - id: alpn_protocols
        type: protocol
        repeat: eos

  protocol:
    seq:
      - id: strlen
        type: u1

      - id: name
        size: strlen
