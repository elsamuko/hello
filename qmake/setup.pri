TEMPLATE = app
CONFIG += console c++17
CONFIG += static
CONFIG -= app_bundle
CONFIG -= qt

# set debug/release info
CONFIG(debug, debug|release) {
    COMPILE_FLAG=d
    COMPILE_MODE=debug
}

CONFIG(release, debug|release) {
    COMPILE_MODE=release
}

win32:include( $${MAIN_DIR}/qmake/win.pri )

macx:       PLATFORM=mac
win32:      PLATFORM=win
unix:!macx: PLATFORM=linux
unix:!macx: CONFIG += linux

OBJECTS_DIR = $${MAIN_DIR}/tmp/$${TARGET}/$${COMPILE_MODE}/objects
MOC_DIR = $${MAIN_DIR}/tmp/$${TARGET}/$${COMPILE_MODE}/moc
UI_DIR = $${MAIN_DIR}/tmp/$${TARGET}/$${COMPILE_MODE}/uic
RCC_DIR = $${MAIN_DIR}/tmp/$${TARGET}/$${COMPILE_MODE}/rcc

SRC_DIR=$${MAIN_DIR}/src
INCLUDEPATH += $${SRC_DIR}

DEFINES += __PRETTY_FUNCTION__=__FUNCTION__

message($${TARGET} $${PLATFORM} $${COMPILE_MODE})
