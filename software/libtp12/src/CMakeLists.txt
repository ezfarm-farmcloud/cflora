CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT (libtp12)

SET (LIBRARY_OUTPUT_PATH ../lib)

INCLUDE_DIRECTORIES (../include ../../node/include )

SET (LIBSRCS TTA12_packet.cpp Packet.cpp util.cpp tcpman.cpp)

ADD_LIBRARY (tp12 STATIC ${LIBSRCS})
