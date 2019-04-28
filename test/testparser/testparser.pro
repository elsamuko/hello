TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

MAIN_DIR=../..
include( $${MAIN_DIR}/qmake/setup.pri )

HELLO_DIR=$${SRC_DIR}/clienthello
INCLUDEPATH+=$${HELLO_DIR}

SOURCES += testparser.cpp
HEADERS += $${SRC_DIR}/log.hpp

include( $${MAIN_DIR}/qmake/parser.pri )
include( $${MAIN_DIR}/qmake/kaitai.pri )
include( $${MAIN_DIR}/qmake/boost.pri )

linux: LIBS += -lpthread
