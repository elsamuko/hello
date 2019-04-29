CLIENT_HELLO_DIR=$${SRC_DIR}/clienthello
INCLUDEPATH+=$${CLIENT_HELLO_DIR}

SOURCES += $${CLIENT_HELLO_DIR}/main.cpp

# ./scripts/cipher_suites.sh > src/clienthello/ciphersuites.hpp
HEADERS += $${CLIENT_HELLO_DIR}/ciphersuites.hpp

# ./scripts/extensions.sh > src/clienthello/extensions.hpp
HEADERS += $${CLIENT_HELLO_DIR}/extensions.hpp

HEADERS += $${CLIENT_HELLO_DIR}/clienthelloparser.hpp
SOURCES += $${CLIENT_HELLO_DIR}/clienthelloparser.cpp

HEADERS += $${CLIENT_HELLO_DIR}/server.hpp
SOURCES += $${CLIENT_HELLO_DIR}/server.cpp
