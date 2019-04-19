TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

MAIN_DIR=..
SRC_DIR=$${MAIN_DIR}/src
INCLUDEPATH += $${SRC_DIR}

SOURCES += $$SRC_DIR/main.cpp

LIBS += -lboost_system

# tls header parser
include( kaitai.pri )
