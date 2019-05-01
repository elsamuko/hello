HEADERS += $${SRC_DIR}/log.hpp
SOURCES += $${SRC_DIR}/log.cpp
SOURCES += $${SRC_DIR}/utils.cpp
HEADERS += $${SRC_DIR}/utils.hpp

# ./scripts/cipher_suites.sh > src/ciphersuites.hpp
HEADERS += $${SRC_DIR}/ciphersuites.hpp

# tls header parser
include( $${MAIN_DIR}/qmake/kaitai.pri )
include( $${MAIN_DIR}/qmake/boost.pri )

linux: LIBS += -lpthread
