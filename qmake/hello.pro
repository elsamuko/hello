TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

MAIN_DIR=..
include( setup.pri )

SOURCES += $${SRC_DIR}/main.cpp

include( parser.pri )
include( server.pri )

# tls header parser
include( kaitai.pri )
include( boost.pri )

linux: LIBS += -lpthread
