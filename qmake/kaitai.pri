
# https://github.com/kaitai-io/kaitai_struct_cpp_stl_runtime/tree/master/kaitai
KAITAI_DIR=$${SRC_DIR}/kaitai
HEADERS += $${KAITAI_DIR}/custom_decoder.h \
           $${KAITAI_DIR}/kaitaistream.h \
           $${KAITAI_DIR}/kaitaistruct.h
SOURCES += $${KAITAI_DIR}/kaitaistream.cpp

# configure kaitai as extra MOC
# https://doc.qt.io/qt-5/qmake-advanced-usage.html#adding-compilers
KAITAI_MOC=$${MAIN_DIR}/kaitai-struct-compiler/bin/kaitai-struct-compiler

kaitai.output = ${QMAKE_FILE_PATH}$${DIR_SEPARATOR}${QMAKE_FILE_BASE}.cpp ${QMAKE_FILE_PATH}$${DIR_SEPARATOR}${QMAKE_FILE_BASE}.h
kaitai.commands = $${KAITAI_MOC} -t cpp_stl --cpp-standard 11 --outdir ${QMAKE_FILE_PATH} ${QMAKE_FILE_NAME}
kaitai.input = KAITAIS
kaitai.variable_out = HEADERS GENERATED_SOURCES
QMAKE_EXTRA_COMPILERS += kaitai

KAITAIS += $${KAITAI_DIR}/tls_client_hello.ksy
DEFINES += KS_STR_ENCODING_NONE
