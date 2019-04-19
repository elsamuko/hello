TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

MAIN_DIR=..
include( setup.pri )

SRC_DIR=$${MAIN_DIR}/src
INCLUDEPATH += $${SRC_DIR}

SOURCES += $$SRC_DIR/main.cpp
linux: LIBS += -lpthread

# tls header parser
include( kaitai.pri )
include( boost.pri )
