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

#ifndef INCLUDED_CORRECTIQ_CORRECTIQ_MAN_H
#define INCLUDED_CORRECTIQ_CORRECTIQ_MAN_H

#include <correctiq/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace correctiq {

    /*!
     * \brief <+description of block+>
     * \ingroup correctiq
     *
     */
    class CORRECTIQ_API correctiq_man : virtual public gr::sync_block
    {
     public:
        typedef std::shared_ptr<correctiq_man> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of correctiq::correctiq_man.
       *
       * To avoid accidental use of raw pointers, correctiq::correctiq_man's
       * constructor is in a private implementation
       * class. correctiq::correctiq_man::make is the public interface for
       * creating new instances.
       */

      virtual float get_real() = 0;
      virtual float get_imag() = 0;

      virtual void set_real(float newValue) = 0;
      virtual void set_imag(float newValue) = 0;

      static sptr make(float real,float imag);
    };

  } // namespace correctiq
} // namespace gr

#endif /* INCLUDED_CORRECTIQ_CORRECTIQ_MAN_H */

