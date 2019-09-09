/* -*- c++ -*- */

#define CORRECTIQ_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "correctiq_swig_doc.i"

%{
#include "correctiq/correctiq.h"
#include "correctiq/SwapIQ.h"
#include "correctiq/correctiq_man.h"
#include "correctiq/correctiq_auto.h"
%}

%include "correctiq/correctiq.h"
GR_SWIG_BLOCK_MAGIC2(correctiq, correctiq);
%include "correctiq/SwapIQ.h"
GR_SWIG_BLOCK_MAGIC2(correctiq, SwapIQ);
%include "correctiq/correctiq_man.h"
GR_SWIG_BLOCK_MAGIC2(correctiq, correctiq_man);
%include "correctiq/correctiq_auto.h"
GR_SWIG_BLOCK_MAGIC2(correctiq, correctiq_auto);
