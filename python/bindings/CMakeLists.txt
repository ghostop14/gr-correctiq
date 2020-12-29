# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT correctiq_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT correctiq_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND correctiq_python_files
    correctiq_python.cc
    correctiq_man_python.cc
    correctiq_auto_python.cc
    SwapIQ_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(correctiq 
   ../..
   gr::correctiq
   "${correctiq_python_files}")

install(TARGETS correctiq_python DESTINATION ${GR_PYTHON_DIR}/correctiq COMPONENT pythonapi)
