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
#include "correctiq_man_impl.h"
#include "clSComplex.h"

namespace gr {
  namespace correctiq {

    correctiq_man::sptr
    correctiq_man::make(float real,float imag)
    {
      return gnuradio::get_initial_sptr
        (new correctiq_man_impl(real,imag));
    }

    /*
     * The private constructor
     */
    correctiq_man_impl::correctiq_man_impl(float real,float imag)
      : gr::sync_block("correctiq_man",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
        avg_real=real;
        avg_img=imag;
        d_k=gr_complex(avg_real,avg_img);
    }

    /*
     * Our virtual destructor.
     */
    correctiq_man_impl::~correctiq_man_impl()
    {
    }

    float correctiq_man_impl::get_real() {
    	return avg_real;
    }
    float correctiq_man_impl::get_imag() {
    	return avg_img;
    }

    void correctiq_man_impl::set_real(float newValue) {
    	avg_real = newValue;
        d_k=gr_complex(avg_real,avg_img);

    }

    void correctiq_man_impl::set_imag(float newValue) {
    	avg_img = newValue;
        d_k=gr_complex(avg_real,avg_img);
    }

    int correctiq_man_impl::testCPU(int noutput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items) {
        const SComplex *in = (const SComplex *) input_items[0];
        SComplex *out = (SComplex *) output_items[0];
        // const gr_complex *in = (const gr_complex *) input_items[0];
        //gr_complex *out = (gr_complex *) output_items[0];

        int i;

        //gr_complex k(avg_real,avg_img);

        for (i = 0; i < noutput_items; i++)
        {
      	  // out[i] = in[i] + k;

          // out[i] = gr_complex(in[i].real() - avg_real,in[i].imag() - avg_img);
          // slightly faster than creating a new object
          out[i].real = in[i].real + avg_real;
          out[i].imag = in[i].imag + avg_img;

        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

    int
    correctiq_man_impl::work (int noutput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const SComplex *in = (const SComplex *) input_items[0];
        SComplex *out = (SComplex *) output_items[0];
        // const gr_complex *in = (const gr_complex *) input_items[0];
        //gr_complex *out = (gr_complex *) output_items[0];

        int i;

        //gr_complex k(avg_real,avg_img);

        for (i = 0; i < noutput_items; i++)
        {
      	  // out[i] = in[i] + k;

          // out[i] = gr_complex(in[i].real() - avg_real,in[i].imag() - avg_img);
          // slightly faster than creating a new object
          out[i].real = in[i].real + avg_real;
          out[i].imag = in[i].imag + avg_img;

        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }
   /*
    int
    correctiq_man_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      //const SComplex *in = (const SComplex *) input_items[0];
      //SComplex *out = (SComplex *) output_items[0];
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      int i;

      //gr_complex k(avg_real,avg_img);

      for (i = 0; i < noutput_items; i++)
      {
    	  out[i] = in[i] + k;

        // out[i] = gr_complex(in[i].real() - avg_real,in[i].imag() - avg_img);
        // slightly faster than creating a new object
        //out[i].real = in[i].real + avg_real;
        //out[i].imag = in[i].imag + avg_img;

      }

      // Do <+signal processing+>
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }
*/

    void
	correctiq_man_impl::setup_rpc()
    {
#ifdef GR_CTRLPORT
      add_rpc_variable(
        rpcbasic_sptr(new rpcbasic_register_get<correctiq_man, float>(
	  alias(), "real",
	  &clMathConst::get_real,
	  pmt::from_complex(-4.29e9, 0),
          pmt::from_complex(4.29e9, 0),
          pmt::from_complex(0, 0),
	  "", "real", RPC_PRIVLVL_MIN,
          DISPTIME | DISPOPTCPLX | DISPOPTSTRIP)));

      add_rpc_variable(
        rpcbasic_sptr(new rpcbasic_register_set<correctiq_man, float>(
	  alias(), "real",
	  &clMathConst::set_real,
	  pmt::from_complex(-4.29e9, 0),
          pmt::from_complex(4.29e9, 0),
          pmt::from_complex(0, 0),
	  "", "real",
	  RPC_PRIVLVL_MIN, DISPNULL)));

      add_rpc_variable(
        rpcbasic_sptr(new rpcbasic_register_get<correctiq_man, float>(
	  alias(), "imag",
	  &clMathConst::get_imag,
	  pmt::from_complex(-4.29e9, 0),
          pmt::from_complex(4.29e9, 0),
          pmt::from_complex(0, 0),
	  "", "imag", RPC_PRIVLVL_MIN,
          DISPTIME | DISPOPTCPLX | DISPOPTSTRIP)));

      add_rpc_variable(
        rpcbasic_sptr(new rpcbasic_register_set<correctiq_man, float>(
	  alias(), "imag",
	  &clMathConst::set_imag,
	  pmt::from_complex(-4.29e9, 0),
          pmt::from_complex(4.29e9, 0),
          pmt::from_complex(0, 0),
	  "", "imag",
	  RPC_PRIVLVL_MIN, DISPNULL)));
#endif /* GR_CTRLPORT */
    }
  } /* namespace correctiq */
} /* namespace gr */

