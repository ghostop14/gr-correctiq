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

#ifndef INCLUDED_CORRECTIQ_SWAPIQ_IMPL_H
#define INCLUDED_CORRECTIQ_SWAPIQ_IMPL_H

#include <correctiq/SwapIQ.h>

#define SWAPTYPE_FLOATCOMPLEX 1
#define SWAPTYPE_SHORTCOMPLEX 2
#define SWAPTYPE_BYTECOMPLEX 3

namespace gr {
namespace correctiq {

class SwapIQ_impl : public SwapIQ {
private:
  int d_datatype;

public:
  SwapIQ_impl(int datatype, int datasize);
  ~SwapIQ_impl();

  // Where all the action really happens
  int work(int noutput_items, gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
};

} // namespace correctiq
} // namespace gr

#endif /* INCLUDED_CORRECTIQ_SWAPIQ_IMPL_H */
