TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

MAIN_DIR=../..
include( $${MAIN_DIR}/qmake/setup.pri )

SOURCES += testparser.cpp

include( $${MAIN_DIR}/qmake/parser.pri )
include( $${MAIN_DIR}/qmake/kaitai.pri )
include( $${MAIN_DIR}/qmake/boost.pri )

linux: LIBS += -lpthread
