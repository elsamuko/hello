
LIB_DIR=$${MAIN_DIR}/libs
BOOST_LIB_DIR=$${LIB_DIR}/boost/lib/$${PLATFORM}/$${COMPILE_MODE}
INCLUDEPATH += $${LIB_DIR}/boost/include

unix {
    QMAKE_CXXFLAGS += -isystem $${LIB_DIR}/boost/include
    LIBS += $${BOOST_LIB_DIR}/libboost_system.a
    LIBS += $${BOOST_LIB_DIR}/libboost_thread.a
    LIBS += $${BOOST_LIB_DIR}/libboost_log.a

    unit_test {
        LIBS += $${BOOST_LIB_DIR}/libboost_unit_test_framework.a
    }
}

win32 {
    QMAKE_LFLAGS += /LIBPATH:$${BOOST_LIB_DIR}
}

CONFIG(debug, debug|release) {
    DEFINES += BOOST_ASIO_NO_DEPRECATED
    DEFINES += BOOST_ASIO_ENABLE_HANDLER_TRACKING
}
