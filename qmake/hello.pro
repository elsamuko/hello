TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

MAIN_DIR=..
include( setup.pri )

SRC_DIR=$${MAIN_DIR}/src
INCLUDEPATH += $${SRC_DIR}

SOURCES += $${SRC_DIR}/main.cpp

# ./scripts/cipher_suites.sh > src/ciphersuites.hpp
HEADERS += $${SRC_DIR}/ciphersuites.hpp

HEADERS += $${SRC_DIR}/log.hpp

HEADERS += $${SRC_DIR}/parser.hpp
SOURCES += $${SRC_DIR}/parser.cpp

HEADERS += $${SRC_DIR}/server.hpp
SOURCES += $${SRC_DIR}/server.cpp

linux: LIBS += -lpthread

# tls header parser
include( kaitai.pri )
include( boost.pri )
