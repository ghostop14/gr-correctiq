/* -*- c++ -*- */
/*
 * Copyright 2019 ghostop14.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "SwapIQ_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace correctiq {

SwapIQ::sptr SwapIQ::make(int datatype, int datasize) {
    return gnuradio::make_block_sptr<SwapIQ_impl>(datatype, datasize);
}

/*
 * The private constructor
 */
SwapIQ_impl::SwapIQ_impl(int datatype, int datasize)
    : gr::sync_block("SwapIQ", gr::io_signature::make(1, 1, datasize),
                     gr::io_signature::make(1, 1, datasize)) {
  d_datatype = datatype;

  if (d_datatype != SWAPTYPE_FLOATCOMPLEX) {
    gr::block::set_output_multiple(2); // Make sure we work with pairs
  }
}

/*
 * Our virtual destructor.
 */
SwapIQ_impl::~SwapIQ_impl() {}

int SwapIQ_impl::work(int noutput_items, gr_vector_const_void_star &input_items,
                      gr_vector_void_star &output_items) {
  // Constructor guarantees we'll have pairs.

  long i;
  long noi;

  switch (d_datatype) {
  case SWAPTYPE_FLOATCOMPLEX: {
    noi = noutput_items * 2; // Each complex is 2 floats
    const float *infloat = (const float *)input_items[0];
    float *outfloat = (float *)output_items[0];

    for (i = 0; i < noi; i += 2) {
      outfloat[i] = infloat[i + 1];
      outfloat[i + 1] = infloat[i];
    }
  } break;
  case SWAPTYPE_SHORTCOMPLEX: {
    noi = noutput_items;
    const int16_t *inShort = (const int16_t *)input_items[0];
    int16_t *outShort = (int16_t *)output_items[0];

    for (i = 0; i < noi; i += 2) {
      outShort[i] = inShort[i + 1];
      outShort[i + 1] = inShort[i];
    }
  } break;
  case SWAPTYPE_BYTECOMPLEX: {
    noi = noutput_items;
    const char *inByte = (const char *)input_items[0];
    char *outByte = (char *)output_items[0];

    for (i = 0; i < noi; i += 2) {
      outByte[i] = inByte[i + 1];
      outByte[i + 1] = inByte[i];
    }
  } break;
  }

  // Tell runtime system how many output items we produced.
  return noutput_items;
}
} /* namespace correctiq */
} /* namespace gr */
