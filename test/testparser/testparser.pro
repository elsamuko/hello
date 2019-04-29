TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

MAIN_DIR=../..
include( $${MAIN_DIR}/qmake/setup.pri )

SOURCES += testparser.cpp
HEADERS += $${SRC_DIR}/log.hpp

CLIENT_HELLO_DIR=$${SRC_DIR}/clienthello
INCLUDEPATH+=$${CLIENT_HELLO_DIR}

HEADERS += $${CLIENT_HELLO_DIR}/ciphersuites.hpp
HEADERS += $${CLIENT_HELLO_DIR}/extensions.hpp
HEADERS += $${CLIENT_HELLO_DIR}/clienthelloparser.hpp
SOURCES += $${CLIENT_HELLO_DIR}/clienthelloparser.cpp

include( $${MAIN_DIR}/qmake/kaitai.pri )
include( $${MAIN_DIR}/qmake/boost.pri )

linux: LIBS += -lpthread
