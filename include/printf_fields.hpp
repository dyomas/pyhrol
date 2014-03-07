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

// $Date: 2011-03-20 01:39:26 +0300 (Sun, 20 Mar 2011) $
// $Revision: 517 $

#include "printf_fields.h"

template <typename F, typename M, typename S> void PrintfFields<F, M, S>::m_init(const parser_t &parser)
{
  const size_t fields_capacity = m_fields.capacity();
  size_t fields_need;

  fields_need = parser.parse(&m_fields[0], fields_capacity, m_fmt.c_str());
  m_fields.resize(fields_need);

  if (fields_need > fields_capacity)
  {
    parser.parse(&m_fields[0], fields_need, m_fmt.c_str());
  }
}

template <typename F, typename M, typename S> const typename PrintfFields<F, M, S>::field_t *PrintfFields<F, M, S>::operator [](const size_t pos) const
{
  return &m_fields[pos];
}

template <typename F, typename M, typename S> const size_t PrintfFields<F, M, S>::size() const
{
  return m_fields.size();
}

template <typename F, typename M, typename S> const typename PrintfFields<F, M, S>::fields_t &PrintfFields<F, M, S>::fields() const
{
  return m_fields;
}

template <typename F, typename M, typename S> const typename PrintfFields<F, M, S>::field_t *PrintfFields<F, M, S>::fields(size_t &_sz) const
{
  _sz = m_fields.size();
  return &m_fields[0];
}

template <typename F, typename M, typename S> const char *PrintfFields<F, M, S>::format() const
{
  return m_fmt.c_str();
}

template <typename F, typename M, typename S> void PrintfFields<F, M, S>::setup(const parser_t &parser, const char *_format)
{
  m_fmt = _format;
  m_init(parser);
}

template <typename F, typename M, typename S> PrintfFields<F, M, S>::PrintfFields()
{
}

template <typename F, typename M, typename S> PrintfFields<F, M, S>::PrintfFields(const parser_t &parser, const char *_format)
  : m_fmt(_format)
{
  m_init(parser);
}
