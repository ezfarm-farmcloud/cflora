CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (gcg)

SET (EXECUTABLE_OUTPUT_PATH ../bin)
SET (LIBRARY_OUTPUT_PATH ../lib)

FIND_LIBRARY (TP12LIB NAMES tp12 PATHS ../../libtp12/lib)
FIND_LIBRARY (TP3LIB NAMES tp3 PATHS ../../libtp3/lib)
FIND_LIBRARY (UVLIB NAMES uv PATHS /usr/local/lib)
FIND_LIBRARY (CFLIB NAMES cflora PATHS ../../libcflora/lib)
FIND_LIBRARY (INILIB NAMES iniparser PATHS ../../iniparser)

INCLUDE_DIRECTORIES (../include ../../libcflora/include ../../libtp3/include ../../libtp12/include /usr/local/include)

SET (SRCS gcg_connection.cpp gcg_config.cpp gcg_server.cpp)

ADD_LIBRARY (gcgtest STATIC ${SRCS})

ADD_EXECUTABLE (gcg gcg.cpp ${SRCS})

TARGET_LINK_LIBRARIES (gcg ${TP3LIB})
TARGET_LINK_LIBRARIES (gcg ${TP12LIB})
TARGET_LINK_LIBRARIES (gcg ${UVLIB})
TARGET_LINK_LIBRARIES (gcg ${CFLIB})
TARGET_LINK_LIBRARIES (gcg ${INILIB})

