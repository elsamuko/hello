TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

MAIN_DIR=../..
include( $${MAIN_DIR}/qmake/setup.pri )

SOURCES += testparser.cpp
HEADERS += $${SRC_DIR}/log.hpp
SOURCES += $${SRC_DIR}/utils.cpp
HEADERS += $${SRC_DIR}/utils.hpp

CLIENT_HELLO_DIR=$${SRC_DIR}/clienthello
INCLUDEPATH+=$${CLIENT_HELLO_DIR}
HEADERS += $${CLIENT_HELLO_DIR}/ciphersuites.hpp
HEADERS += $${CLIENT_HELLO_DIR}/extensions.hpp
HEADERS += $${CLIENT_HELLO_DIR}/clienthelloparser.hpp
SOURCES += $${CLIENT_HELLO_DIR}/clienthelloparser.cpp

SERVER_HELLO_DIR=$${SRC_DIR}/serverhello
INCLUDEPATH+=$${SERVER_HELLO_DIR}
HEADERS += $${SERVER_HELLO_DIR}/serverhelloparser.hpp
SOURCES += $${SERVER_HELLO_DIR}/serverhelloparser.cpp

include( $${MAIN_DIR}/qmake/kaitai.pri )
include( $${MAIN_DIR}/qmake/boost.pri )

linux: LIBS += -lpthread
