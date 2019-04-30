TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

MAIN_DIR=../..
include( ../setup.pri )

include( serverhello.pri )

HEADERS += $${SRC_DIR}/log.hpp
SOURCES += $${SRC_DIR}/utils.cpp
HEADERS += $${SRC_DIR}/utils.hpp

# ./scripts/cipher_suites.sh > src/ciphersuites.hpp
HEADERS += $${SRC_DIR}/ciphersuites.hpp

# tls header parser
include( ../kaitai.pri )
include( ../boost.pri )

linux: LIBS += -lpthread
