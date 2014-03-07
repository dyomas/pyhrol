/*
 *   Copyright (c) 2013 Pyhrol, pyhrol@rambler.ru
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

// $Date: 2013-10-29 01:08:20 +0400 (Tue, 29 Oct 2013) $
// $Revision: 821 $

#ifndef __pyhrol_tuples_data_default_impl_h__
#define __pyhrol_tuples_data_default_impl_h__

#include "pyhrol_tuples.h"
#include "pyhrol_tuple_fill_state.h"

namespace pyhrol
{

class TuplesDataDefaultImpl: public TuplesData
{
  const callVariants cv;
  bool m_valid;
  uint8_t mBuildVariantsCnt;
  TupleFillState *m_fill_state;

  TuplesDataDefaultImpl(const TuplesDataDefaultImpl &);
  TuplesDataDefaultImpl &operator= (const TuplesDataDefaultImpl &);

public:
  TuplesDataDefaultImpl(const callVariants);
  ~TuplesDataDefaultImpl();

  friend class TupleState;
  friend class TuplesDefaultImpl;
#ifdef _DEBUG_PYTHON_TUPLES_STATES
  friend class TestTupleState;
#endif //_DEBUG_PYTHON_TUPLES_STATES
};

} //namespace pyhrol

#endif //__pyhrol_tuples_data_default_impl_h__
