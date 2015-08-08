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

// $Date: 2015-07-20 23:24:41 +0300 (Пн., 20 июля 2015) $
// $Revision: 1047 $

#ifndef __pyhrol_tuple_describer_flexible_h__
#define __pyhrol_tuple_describer_flexible_h__


#include <sstream>
#include <stdint.h>
#include "self_addressed_cstring.h"
#include "pyhrol_tuple_base.h"
#include "pyhrol_tuple_describer.h"

//#define

namespace pyhrol
{

class TupleFormat;
class TuplesDefaultImpl;

class TupleDescriberFlexible: public SelfAddressedCstringBase<TupleDescriberFlexible>, public TupleDescriber
{
  enum tupleDirection
  {
      dirIn
    , dirOut
  };
  enum describeOptions
  {
      optRaw
    , optPerLine
    , optPerLineWithHeader
  };

  size_t m_help_length;
  void m_append(const std::string &, const bool = true);
  void m_describe(const TuplesDefaultImpl &, const TupleFormat &);
  void m_description_errors
  (
      std::ostream &os
    , const TupleBase &tuple
    , const TupleFormat &
    , const tupleDirection /*dir*/
    , const uint8_t /*cnt*/
  ) const;
  void m_description_formatted_header
  (
      std::ostream &os
    , TupleBase::strings_t::const_iterator &iter_descs
    , const TupleBase &tuple
    , const TupleFormat &
    , const tupleDirection /*dir*/
    , const describeOptions /*opt*/
    , const uint8_t /*cnt*/
  ) const;
  void m_description_formatted_body
  (
      std::ostream &os
    , TupleBase::strings_t::const_iterator &iter_descs
    , const TupleBase &tuple
    , const TupleFormat &
    , const tupleDirection /*dir*/
    , const describeOptions /*opt*/
  ) const;
  const std::string m_signature_cpp(const TupleBase &tuple, const tupleDirection /*dir*/) const;
  const std::string m_signature_python(const TupleBase &tuple, const tupleDirection /*dir*/) const;

public:
  virtual const char *to_string() const;
  virtual void help(const char */*help*/);
  virtual void append(const char *);
  virtual void describe(const Tuples &);
  virtual void describe(const TuplesDefaultImpl &);
#ifdef _DEBUG_PYTHON_PYTUPLE
  void __describe(const TuplesDefaultImpl &, const TupleFormat &);
#endif //_DEBUG_PYTHON_PYTUPLE

  TupleDescriberFlexible();
  ~TupleDescriberFlexible();
};

} //namespace pyhrol

#endif //__pyhrol_tuple_describer_flexible_h__
