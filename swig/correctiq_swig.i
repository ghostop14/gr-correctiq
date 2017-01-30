/* -*- c++ -*- */

#define CORRECTIQ_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "correctiq_swig_doc.i"

%{
#include "correctiq/correctiq.h"
%}


%include "correctiq/correctiq.h"
GR_SWIG_BLOCK_MAGIC2(correctiq, correctiq);
