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
#include "correctiq_auto_impl.h"
#include <gnuradio/io_signature.h>
#include <iomanip>
#include <limits>

namespace gr {
namespace correctiq {

correctiq_auto::sptr correctiq_auto::make(double samp_rate, double freq,
                                          float gain, float syncWindow) {
    return gnuradio::make_block_sptr<correctiq_auto_impl>(
        samp_rate, freq, gain, syncWindow);
}

/*
 * The private constructor
 */
correctiq_auto_impl::correctiq_auto_impl(double samp_rate, double freq,
                                         float gain, float syncWindow)
    : gr::sync_block("correctiq_auto",
                     gr::io_signature::make(1, 1, sizeof(gr_complex)),
                     gr::io_signature::make(1, 1, sizeof(gr_complex))) {
  avg_real = 0.0;
  avg_img = 0.0;

  d_samp_rate = samp_rate;
  d_freq = freq;
  d_gain = gain;
  d_syncWindow = syncWindow;

  synchronized = false;

  std::cout << "CorrectIQ Auto Synchronizing..." << std::endl;

  d_max_syncSamples = (long)(d_samp_rate * (double)syncWindow);
  // std::cout << "Using " << d_max_syncSamples << " samples for
  // synchronization" << std::endl;
}

/*
 * Our virtual destructor.
 */
correctiq_auto_impl::~correctiq_auto_impl() {}

double correctiq_auto_impl::get_freq() { return d_freq; }

float correctiq_auto_impl::get_gain() { return d_gain; }

void correctiq_auto_impl::set_freq(double newValue) {
  std::cout << "[CorrectIQ_Auto] Auto Synchronizing..." << std::endl;
  d_freq = newValue;
  synchronized = false;
  syncCounter = 0;

  // reset counters on an unsync
  avg_real = 0.0;
  avg_img = 0.0;
}

void correctiq_auto_impl::set_gain(float newValue) {
  std::cout << "[CorrectIQ_Auto] Auto Synchronizing..." << std::endl;
  d_gain = newValue;
  synchronized = false;
  syncCounter = 0;

  // reset counters on an unsync
  avg_real = 0.0;
  avg_img = 0.0;
}

int correctiq_auto_impl::testCPU(int noutput_items,
                                 gr_vector_const_void_star &input_items,
                                 gr_vector_void_star &output_items) {
  const SComplex *in = (const SComplex *)input_items[0];
  SComplex *out = (SComplex *)output_items[0];

  int i;

  for (i = 0; i < noutput_items; i++) {
    out[i].real = in[i].real - avg_real;
    out[i].imag = in[i].imag - avg_img;
  }

  // Tell runtime system how many output items we produced.
  return noutput_items;
}

int correctiq_auto_impl::work(int noutput_items,
                              gr_vector_const_void_star &input_items,
                              gr_vector_void_star &output_items) {
  const SComplex *in = (const SComplex *)input_items[0];
  SComplex *out = (SComplex *)output_items[0];

  int i;

  for (i = 0; i < noutput_items; i++) {
    if (synchronized) {
      // out[i] = gr_complex(in[i].real() - avg_real,in[i].imag() - avg_img);
      // slightly faster than creating a new object
      out[i].real = in[i].real - avg_real;
      out[i].imag = in[i].imag - avg_img;
    } else {
      // Synchronizing.  Behave just like normal correctiq.
      avg_real = ratio * (in[i].real - avg_real) + avg_real;
      avg_img = ratio * (in[i].imag - avg_img) + avg_img;

      out[i].real = in[i].real - avg_real;
      out[i].imag = in[i].imag - avg_img;

      syncCounter++;
    }
  }

  if (!synchronized && (syncCounter >= d_max_syncSamples)) {
    synchronized = true;

    std::cout << "CorrectIQ Auto offset now synchronized." << std::endl;
    std::cout << "Applying these offsets... Real:" << std::fixed
              << std::setw(11) << std::setprecision(6) << -avg_real
              << ", imag:" << -avg_img << std::endl;
  }

  return noutput_items;
}
} /* namespace correctiq */
} /* namespace gr */
