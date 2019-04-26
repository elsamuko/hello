# ./scripts/cipher_suites.sh > src/ciphersuites.hpp
HEADERS += $${SRC_DIR}/ciphersuites.hpp

# ./scripts/extensions.sh > src/extensions.hpp
HEADERS += $${SRC_DIR}/extensions.hpp

HEADERS += $${SRC_DIR}/log.hpp

HEADERS += $${SRC_DIR}/parser.hpp
SOURCES += $${SRC_DIR}/parser.cpp
