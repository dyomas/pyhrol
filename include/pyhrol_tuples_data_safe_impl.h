/*
 *   Copyright (c) 2013, 2014, Pyhrol, pyhrol@rambler.ru
 *   GEO: N55.703431,E37.623324 .. N48.742359,E44.536997
 * 
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   4. Neither the name of the Pyhrol nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 * 
 *   THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 *   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *   ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 *   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 *   OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *   SUCH DAMAGE.
 */

// $Date: 2014-04-04 16:35:38 +0400 (Пт., 04 апр. 2014) $
// $Revision: 906 $

#ifndef __pyhrol_tuples_data_safe_impl_h__
#define __pyhrol_tuples_data_safe_impl_h__

#include <vector>

#include "pyhrol_tuples.h"
#include "pyhrol_common.h"

namespace pyhrol
{

class TupleInSafeImpl;
class TupleOutSafeImpl;

class TuplesDataSafeImpl: public TuplesData
{
  typedef std::vector<TupleInSafeImpl *> tuples_in_t;
  typedef std::vector<TupleOutSafeImpl *> tuples_out_t;

  enum fillState
  {
      fsNotFilled = 0
    , fsFilledIn = 4
    , fsFilledOut = 8
  };

  fillState m_fill_state;
  bool m_valid;
  tuples_in_t m_tuples_in;
  tuples_out_t m_tuples_out;
  const size_t m_address;

  const callVariants cv;
  uint8_t mBuildVariantsCnt;

  TuplesDataSafeImpl(const TuplesDataSafeImpl &);
  TuplesDataSafeImpl &operator= (const TuplesDataSafeImpl &);

public:
  TuplesDataSafeImpl(const callVariants cv, const size_t);
  ~TuplesDataSafeImpl();

  friend class TuplesSafeImpl;
};

} //namespace pyhrol

#endif //__pyhrol_tuples_data_safe_impl_h__
