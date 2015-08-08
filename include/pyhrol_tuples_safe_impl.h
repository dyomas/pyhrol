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

#ifndef __pyhrol_tuples_safe_impl_h__
#define __pyhrol_tuples_safe_impl_h__

#include <stdarg.h>
#include <string>
#include <vector>
#include <stdexcept>

#include <pyport.h>
#include <object.h>
#include <stdarg.h>

#include "pyhrol_tuples.h"
#include "smart_flag.h"
#include "nullable.h"

namespace pyhrol
{

class TupleInSafeImpl;
class TupleOutSafeImpl;
class TuplesDataSafeImpl;

class TuplesSafeImpl: public Tuples
{
public:
  typedef std::vector<TupleInSafeImpl *> tuples_in_t;
  typedef std::vector<TupleOutSafeImpl *> tuples_out_t;

  TuplesSafeImpl(TuplesDataSafeImpl &);

  virtual TupleIn &t_in() const;
  virtual TupleOut &t_out() const;

  virtual PyObject *ubiquitous_caller
  (
      exceptionHandler &
    , PyObject *args
    , PyObject *kwds
    , const bool is_probe
    , const callerTypes
  );

  virtual void set_options(const uint8_t /*_required*/);
  virtual void set_options(const options /*_opts*/);
  virtual void set_options(const options /*_opts*/, const uint8_t /*_required*/);

  virtual bool prepare_parse(const char *);
  virtual bool is_parse_enabled();
  virtual void parse(const bool /*dummy*/, ...);
  virtual bool before_build();
  virtual bool prepare_build(const char *);
  virtual bool is_build_enabled();
  virtual void build(const bool dummy, ...);
  virtual bool after_build();
  virtual const uint8_t parsed_variant() const;
  virtual void choose_build_variant(const uint8_t = 0U);

private:
  bool m_filled() const;
  void m_check_parse_state() const;

  TuplesSafeImpl(const TuplesSafeImpl &);
  TuplesSafeImpl &operator=(const TuplesSafeImpl &);

  callerTypes m_caller_type;
  PyObject *m_tuple;
  PyObject *m_kw;
  PyObject *m_retval;

  TuplesDataSafeImpl &m_data;

  uint8_t
      mParseVariant
    , mBuildVariant
  ;
  Nullable<uint8_t>
      mVariantBuilt
    , mVariantParsed
  ;

  int8_t m_required;
  options m_options;

  virtual bool m_parse(va_list ap);
};

} //namespace pyhrol

#endif //__pyhrol_tuples_safe_impl_h__
