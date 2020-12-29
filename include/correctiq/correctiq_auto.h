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

#ifndef INCLUDED_CORRECTIQ_CORRECTIQ_AUTO_H
#define INCLUDED_CORRECTIQ_CORRECTIQ_AUTO_H

#include <correctiq/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace correctiq {

    /*!
     * \brief <+description of block+>
     * \ingroup correctiq
     *
     */
    class CORRECTIQ_API correctiq_auto : virtual public gr::sync_block
    {
     public:
        typedef std::shared_ptr<correctiq_auto> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of correctiq::correctiq_auto.
       *
       * To avoid accidental use of raw pointers, correctiq::correctiq_auto's
       * constructor is in a private implementation
       * class. correctiq::correctiq_auto::make is the public interface for
       * creating new instances.
       */

      virtual double get_freq() = 0;
      virtual float get_gain() = 0;

      virtual void set_freq(double newValue) = 0;
      virtual void set_gain(float newValue) = 0;

      static sptr make(double samp_rate,double freq, float gain, float syncWindow);
    };

  } // namespace correctiq
} // namespace gr

#endif /* INCLUDED_CORRECTIQ_CORRECTIQ_AUTO_H */

