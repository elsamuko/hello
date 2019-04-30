#!/usr/bin/env bash
# https://letsencrypt.org/docs/certificates-for-localhost/

SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
MAIN_DIR="$SCRIPT_DIR/.."
SERVER_DIR="$MAIN_DIR/tmp/server"

mkdir -p "$SERVER_DIR"
cd "$SERVER_DIR"

if [ ! -f certificate.pem ]; then
    printf "[dn]
CN=localhost
[req]
distinguished_name = dn
[EXT]
subjectAltName=DNS:localhost
keyUsage=digitalSignature
extendedKeyUsage=serverAuth
" > crt.cfg

    echo "Generate self-signed certificate"
    openssl req -x509 -out certificate.pem -keyout certificate.pem \
        -newkey rsa:2048 -nodes -sha256 \
        -subj '/CN=localhost' -extensions EXT -config crt.cfg
fi

python3 "$SCRIPT_DIR/server.py"
