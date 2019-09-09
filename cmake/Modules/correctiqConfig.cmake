INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_CORRECTIQ correctiq)

FIND_PATH(
    CORRECTIQ_INCLUDE_DIRS
    NAMES correctiq/api.h
    HINTS $ENV{CORRECTIQ_DIR}/include
        ${PC_CORRECTIQ_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    CORRECTIQ_LIBRARIES
    NAMES gnuradio-correctiq
    HINTS $ENV{CORRECTIQ_DIR}/lib
        ${PC_CORRECTIQ_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/correctiqTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CORRECTIQ DEFAULT_MSG CORRECTIQ_LIBRARIES CORRECTIQ_INCLUDE_DIRS)
MARK_AS_ADVANCED(CORRECTIQ_LIBRARIES CORRECTIQ_INCLUDE_DIRS)
