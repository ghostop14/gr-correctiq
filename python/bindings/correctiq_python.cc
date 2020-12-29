/*
 * Copyright 2020 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(correctiq.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(7980e1425a5ccf1f1a32498ea40f342f)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <correctiq/correctiq.h>
// pydoc.h is automatically generated in the build directory
#include <correctiq_pydoc.h>

void bind_correctiq(py::module& m)
{

    using correctiq    = ::gr::correctiq::correctiq;


    py::class_<correctiq, gr::sync_block, gr::block, gr::basic_block,
        std::shared_ptr<correctiq>>(m, "correctiq", D(correctiq))

        .def(py::init(&correctiq::make),
           D(correctiq,make)
        )
        



        ;




}








