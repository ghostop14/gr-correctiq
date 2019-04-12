# Copyright 2011,2012,2016 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

set (CMAKE_CXX_STANDARD 11)

IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
	MESSAGE(STATUS "building for debug...")
ELSE(CMAKE_BUILD_TYPE MATCHES DEBUG)
	MESSAGE(STATUS "Release mode: increasing compiler optimizations...")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)

# Reference for detection
# https://gist.github.com/hideo55/5642892

IF(UNIX)
	IF(NOT APPLE)
    	MESSAGE(STATUS "Detected Linux OS")
		EXEC_PROGRAM(cat ARGS "/proc/cpuinfo" OUTPUT_VARIABLE CPUINFO)
	ELSE(NOT APPLE)
    	MESSAGE(STATUS "Detected Apple OS")
		EXEC_PROGRAM("/usr/sbin/sysctl -n machdep.cpu.features" OUTPUT_VARIABLE CPUINFO)
	ENDIF(NOT APPLE)
	
	STRING(REGEX REPLACE "^.*(fma).*$" "fma" FMA_THERE ${CPUINFO})
   STRING(COMPARE EQUAL "fma" "${FMA_THERE}" FMA_TRUE)
   IF (FMA_TRUE)
		MESSAGE(STATUS "Found FMA.  Setting fma compile flag.")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfma")
   ELSE (FMA_TRUE)
		MESSAGE(STATUS "Not using FMA")
	ENDIF (FMA_TRUE)
ELSE()
    MESSAGE(STATUS "OS is not linux.  Not setting FMA flags.")
ENDIF()

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

list(APPEND correctiq_sources
    correctiq_impl.cc
    correctiq_auto_impl.cc
    correctiq_man_impl.cc
)

set(correctiq_sources "${correctiq_sources}" PARENT_SCOPE)
if(NOT correctiq_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT correctiq_sources)

add_library(gnuradio-correctiq SHARED ${correctiq_sources})
target_link_libraries(gnuradio-correctiq ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES} thrift)
set_target_properties(gnuradio-correctiq PROPERTIES DEFINE_SYMBOL "gnuradio_correctiq_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-correctiq PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-correctiq RUNTIME_COMPONENT "correctiq_runtime" DEVEL_COMPONENT "correctiq_devel")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_correctiq_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_correctiq.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_correctiq.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/correctiq_impl.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/correctiq_auto_impl.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/correctiq_man_impl.cc
)

add_executable(test-correctiq ${test_correctiq_sources})

target_link_libraries(
  test-correctiq
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-correctiq
  thrift
  volk
)

install(TARGETS test-correctiq DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" RUNTIME)

GR_ADD_TEST(test_correctiq test-correctiq)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")

