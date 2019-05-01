MAIN_DIR=$${_PRO_FILE_PWD_}/../..

include( $${MAIN_DIR}/qmake/setup.pri )
include( $${MAIN_DIR}/qmake/commons.pri )

include( $${MAIN_DIR}/qmake/serverhello/serverhello.pri )
include( $${MAIN_DIR}/qmake/clienthello/clienthello.pri )

SOURCES += testparser.cpp
SOURCES -= $${CLIENT_HELLO_DIR}/main.cpp
SOURCES -= $${SERVER_HELLO_DIR}/main.cpp
