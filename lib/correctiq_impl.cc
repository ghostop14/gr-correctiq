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

#include "clSComplex.h"
#include "correctiq_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace correctiq {

correctiq::sptr correctiq::make() {
	return gnuradio::make_block_sptr<correctiq_impl>();
}

/*
 * The private constructor
 */
correctiq_impl::correctiq_impl()
    : gr::sync_block("correctiq",
                     gr::io_signature::make(1, 1, sizeof(gr_complex)),
                     gr::io_signature::make(1, 1, sizeof(gr_complex))) {
  avg_real = 0.0;
  avg_img = 0.0;
}

/*
 * Our virtual destructor.
 */
correctiq_impl::~correctiq_impl() {}

int correctiq_impl::testCPU(int noutput_items,
                            gr_vector_const_void_star &input_items,
                            gr_vector_void_star &output_items) {
  const SComplex *in = (const SComplex *)input_items[0];
  SComplex *out = (SComplex *)output_items[0];

  int i;

  for (i = 0; i < noutput_items; i++) {
    avg_real = ratio * (in[i].real - avg_real) + avg_real;
    avg_img = ratio * (in[i].imag - avg_img) + avg_img;

    // out[i] = gr_complex(in[i].real() - avg_real,in[i].imag() - avg_img);
    // slightly faster than creating a new object
    out[i].real = in[i].real - avg_real;
    out[i].imag = in[i].imag - avg_img;
  }

  // Tell runtime system how many output items we produced.
  return noutput_items;
}

int correctiq_impl::work(int noutput_items,
                         gr_vector_const_void_star &input_items,
                         gr_vector_void_star &output_items) {
  const SComplex *in = (const SComplex *)input_items[0];
  SComplex *out = (SComplex *)output_items[0];

  int i;

  for (i = 0; i < noutput_items; i++) {
    avg_real = ratio * (in[i].real - avg_real) + avg_real;
    avg_img = ratio * (in[i].imag - avg_img) + avg_img;

    // out[i] = gr_complex(in[i].real() - avg_real,in[i].imag() - avg_img);
    // slightly faster than creating a new object
    out[i].real = in[i].real - avg_real;
    out[i].imag = in[i].imag - avg_img;
  }

  // Tell runtime system how many output items we produced.
  return noutput_items;
}
} /* namespace correctiq */
} /* namespace gr */
