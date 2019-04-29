SERVER_HELLO_DIR=$${SRC_DIR}/serverhello
INCLUDEPATH+=$${SERVER_HELLO_DIR}

SOURCES += $${SERVER_HELLO_DIR}/main.cpp

HEADERS += $${SERVER_HELLO_DIR}/serverhelloparser.hpp
SOURCES += $${SERVER_HELLO_DIR}/serverhelloparser.cpp

HEADERS += $${SERVER_HELLO_DIR}/client.hpp
SOURCES += $${SERVER_HELLO_DIR}/client.cpp
