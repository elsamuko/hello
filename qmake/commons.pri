HEADERS += $${SRC_DIR}/log.hpp
SOURCES += $${SRC_DIR}/log.cpp
HEADERS += $${SRC_DIR}/utils.hpp
SOURCES += $${SRC_DIR}/utils.cpp

HEADERS += $${SRC_DIR}/hellogenerator.hpp
SOURCES += $${SRC_DIR}/hellogenerator.cpp

# ./scripts/cipher_suites.sh > src/ciphersuites.hpp
HEADERS += $${SRC_DIR}/ciphersuites.hpp

# ./scripts/extensions.sh > src/extensions.hpp
HEADERS += $${SRC_DIR}/extensions.hpp

# tls header parser
include( $${MAIN_DIR}/qmake/kaitai.pri )
include( $${MAIN_DIR}/qmake/boost.pri )

linux: LIBS += -lpthread
