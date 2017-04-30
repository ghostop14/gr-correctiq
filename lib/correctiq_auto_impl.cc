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
#include "correctiq_auto_impl.h"
#include "clSComplex.h"

// assisted detection of Fused Multiply Add (FMA) functionality
#if !defined(__FMA__) && defined(__AVX2__)
#define __FMA__ 1
#endif

#if defined(FP_FAST_FMA)
#define __FMA__ 1
#endif

#if defined(__FMA__)
#pragma message "FMA support detected.  Compiling for Fused Multiply/Add support."
#else
#pragma message "No FMA support detected.  Compiling for normal math."
#endif

namespace gr {
  namespace correctiq {

    correctiq_auto::sptr
    correctiq_auto::make(float samp_rate,float freq,float gain,float syncWindow)
    {
      return gnuradio::get_initial_sptr
        (new correctiq_auto_impl(samp_rate,freq,gain,syncWindow));
    }

    /*
     * The private constructor
     */
    correctiq_auto_impl::correctiq_auto_impl(float samp_rate,float freq,float gain,float syncWindow)
      : gr::sync_block("correctiq_auto",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
        avg_real=0.0;
        avg_img=0.0;

        d_samp_rate = samp_rate;
        d_freq = freq;
        d_gain = gain;
        d_syncWindow = syncWindow;

    	synchronized = false;

    	std::cout << "CorrectIQ Auto Synchronizing..." << std::endl;

        d_max_syncSamples = (long)(d_samp_rate * syncWindow);
        // std::cout << "Using " << d_max_syncSamples << " samples for synchronization" << std::endl;

    }

    /*
     * Our virtual destructor.
     */
    correctiq_auto_impl::~correctiq_auto_impl()
    {
    }

    float correctiq_auto_impl::get_freq() {
    	return d_freq;
    }

    float correctiq_auto_impl::get_gain() {
    	return d_gain;
    }

    void correctiq_auto_impl::set_freq(float newValue) {
    	std::cout << "CorrectIQ Auto Synchronizing..." << std::endl;
    	d_freq = newValue;
    	synchronized = false;
    	syncCounter = 0;
    }

    void correctiq_auto_impl::set_gain(float newValue) {
    	std::cout << "CorrectIQ Auto Synchronizing..." << std::endl;
    	d_gain = newValue;
    	synchronized = false;
    	syncCounter = 0;
    }

    int correctiq_auto_impl::testCPU(int noutput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items) {
        const SComplex *in = (const SComplex *) input_items[0];
        SComplex *out = (SComplex *) output_items[0];

        int i;

        for (i = 0; i < noutput_items; i++)
        {
            out[i].real = in[i].real - avg_real;
            out[i].imag = in[i].imag - avg_img;
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

    int
    correctiq_auto_impl::work (int noutput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const SComplex *in = (const SComplex *) input_items[0];
      SComplex *out = (SComplex *) output_items[0];

      int i;

      for (i = 0; i < noutput_items; i++)
      {
        if (synchronized) {
            // out[i] = gr_complex(in[i].real() - avg_real,in[i].imag() - avg_img);
            // slightly faster than creating a new object
            out[i].real = in[i].real - avg_real;
            out[i].imag = in[i].imag - avg_img;
        }
        else {
        	// Synchronizing.  Behave just like normal correctiq.
#if defined(__FMA__)
        	// fma(a,b,c) = (a*b)+c
          avg_real = __builtin_fmaf(ratio,(in[i].real - avg_real),avg_real);
          avg_img = __builtin_fmaf(ratio,(in[i].imag - avg_img),avg_img);
#else
          avg_real = ratio * (in[i].real - avg_real) + avg_real;
          avg_img = ratio * (in[i].imag - avg_img) + avg_img;
#endif

            out[i].real = in[i].real - avg_real;
            out[i].imag = in[i].imag - avg_img;

            syncCounter++;
        }

      }

      if (!synchronized && (syncCounter >= d_max_syncSamples)) {
    	  synchronized = true;

      	std::cout << "CorrectIQ Auto offset now synchronized." << std::endl;
      	std::cout << "Applying these offsets... Real:" << std::fixed << std::setw(11)
        << std::setprecision(6) << -avg_real << ", imag:" << -avg_img << std::endl;

      }

      return noutput_items;
    }

    void
	correctiq_auto_impl::setup_rpc()
    {
#ifdef GR_CTRLPORT
      add_rpc_variable(
        rpcbasic_sptr(new rpcbasic_register_get<correctiq_auto, float>(
	  alias(), "frequency",
	  &clMathConst::get_freq,
	  pmt::from_complex(-4.29e9, 0),
          pmt::from_complex(4.29e9, 0),
          pmt::from_complex(0, 0),
	  "", "frequency", RPC_PRIVLVL_MIN,
          DISPTIME | DISPOPTCPLX | DISPOPTSTRIP)));

      add_rpc_variable(
        rpcbasic_sptr(new rpcbasic_register_set<correctiq_auto, float>(
	  alias(), "frequency",
	  &clMathConst::set_freq,
	  pmt::from_complex(-4.29e9, 0),
          pmt::from_complex(4.29e9, 0),
          pmt::from_complex(0, 0),
	  "", "frequency",
	  RPC_PRIVLVL_MIN, DISPNULL)));

      add_rpc_variable(
        rpcbasic_sptr(new rpcbasic_register_get<correctiq_auto, float>(
	  alias(), "gain",
	  &clMathConst::get_gain,
	  pmt::from_complex(-4.29e9, 0),
          pmt::from_complex(4.29e9, 0),
          pmt::from_complex(0, 0),
	  "", "gain", RPC_PRIVLVL_MIN,
          DISPTIME | DISPOPTCPLX | DISPOPTSTRIP)));

      add_rpc_variable(
        rpcbasic_sptr(new rpcbasic_register_set<correctiq_auto, float>(
	  alias(), "gain",
	  &clMathConst::set_gain,
	  pmt::from_complex(-4.29e9, 0),
          pmt::from_complex(4.29e9, 0),
          pmt::from_complex(0, 0),
	  "", "gain",
	  RPC_PRIVLVL_MIN, DISPNULL)));
#endif /* GR_CTRLPORT */
    }
  } /* namespace correctiq */
} /* namespace gr */

