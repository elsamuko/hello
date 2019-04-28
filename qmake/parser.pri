# ./scripts/cipher_suites.sh > src/ciphersuites.hpp
HEADERS += $${HELLO_DIR}/ciphersuites.hpp

# ./scripts/extensions.sh > src/extensions.hpp
HEADERS += $${HELLO_DIR}/extensions.hpp

HEADERS += $${HELLO_DIR}/parser.hpp
SOURCES += $${HELLO_DIR}/parser.cpp
