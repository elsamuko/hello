TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

MAIN_DIR=..
include( setup.pri )

HELLO_DIR=$${SRC_DIR}/clienthello
INCLUDEPATH+=$${HELLO_DIR}

SOURCES += $${HELLO_DIR}/main.cpp
HEADERS += $${SRC_DIR}/log.hpp

include( parser.pri )
include( server.pri )

# tls header parser
include( kaitai.pri )
include( boost.pri )

linux: LIBS += -lpthread
