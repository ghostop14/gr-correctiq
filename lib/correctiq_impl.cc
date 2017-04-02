/* -*- c++ -*- */
/* 
 * Copyright 2017 ghostop14.
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

#include <gnuradio/io_signature.h>
#include "correctiq_impl.h"

namespace gr {
  namespace correctiq {

    correctiq::sptr
    correctiq::make()
    {
      return gnuradio::get_initial_sptr
        (new correctiq_impl());
    }

    /*
     * The private constructor
     */
    correctiq_impl::correctiq_impl()
      : gr::block("correctiq",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
        avg_real=0.0;
        avg_img=0.0;
    }

    /*
     * Our virtual destructor.
     */
    correctiq_impl::~correctiq_impl()
    {
    }

    void
    correctiq_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int correctiq_impl::testCPU(int noutput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items) {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        int i;

        for (i = 0; i < noutput_items; i++)
        {
          avg_real = avg_real + ratio * (in[i].real() - avg_real);
          avg_img = avg_img + ratio * (in[i].imag() - avg_img);

//          out[i] = gr_complex(in[i].real() - avg_real,in[i].imag() - avg_img);
          out[i].real(in[i].real() - avg_real);
          out[i].imag(in[i].imag() - avg_img);
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

    int
    correctiq_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      int i;

      for (i = 0; i < noutput_items; i++)
      {
        avg_real = avg_real + ratio * (in[i].real() - avg_real);
        avg_img = avg_img + ratio * (in[i].imag() - avg_img);

        // out[i] = gr_complex(in[i].real() - avg_real,in[i].imag() - avg_img);
        // slightly faster than creating a new object
        out[i].real(in[i].real() - avg_real);
        out[i].imag(in[i].imag() - avg_img);
      }

      // Do <+signal processing+>
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace correctiq */
} /* namespace gr */

