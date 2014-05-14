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

// $Date: 2014-04-04 16:35:38 +0400 (Fri, 04 Apr 2014) $
// $Revision: 906 $

#include "pyhrol_trace.h"
#include "pyhrol_tuples_data2_default_impl.h"
#include "pyhrol_tuple_in_default_impl.h"
#include "pyhrol_tuple_out_default_impl.h"

namespace pyhrol
{

TuplesData2DefaultImpl::TuplesData2DefaultImpl(const callVariants cv, const size_t address)
  : TuplesDataDefaultImpl(cv)
  , m_address(address)
{
  PYHROL_TRACE(tpInternal, this, __PRETTY_FUNCTION__);
}

TuplesData2DefaultImpl::~TuplesData2DefaultImpl()
{
  PYHROL_TRACE(tpInternal, this, __PRETTY_FUNCTION__);

  for (tuples_in_t::const_iterator iter = m_tuples_in.begin(); iter != m_tuples_in.end(); iter ++)
  {
    delete *iter;
  }
  for (tuples_out_t::const_iterator iter = m_tuples_out.begin(); iter != m_tuples_out.end(); iter ++)
  {
    delete *iter;
  }
}

TuplesData *TuplesData::factory(const callVariants cv, const size_t address)
{
  return new TuplesData2DefaultImpl(cv, address);
}

} //namespace pyhrol
