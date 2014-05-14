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

#ifndef __pyhrol_tuples_default_impl_h__
#define __pyhrol_tuples_default_impl_h__

#include <stdarg.h>
#include <string>
#include <vector>
#include <stdexcept>

#include <pyport.h>
#include <object.h>

#include "pyhrol_tuple_state.h"

namespace pyhrol
{

class TupleInDefaultImpl;
class TupleOutDefaultImpl;
class TuplesData2DefaultImpl;

class TuplesDefaultImpl: public TupleState
{
public:
  typedef std::vector<TupleInDefaultImpl *> tuples_in_t;
  typedef std::vector<TupleOutDefaultImpl *> tuples_out_t;

  TuplesDefaultImpl(TuplesData2DefaultImpl &);

  virtual TupleIn &t_in() const;
  virtual TupleOut &t_out() const;
  const tuples_in_t &t_ins() const;
  const tuples_out_t &t_outs() const;
  const callVariants cv() const;

  virtual PyObject *ubiquitous_caller
  (
      exceptionHandler &
    , PyObject *args
    , PyObject *kwds
    , const bool is_probe
    , const callerTypes
  );

private:
  enum modes
  {
      modeInit
    , modeInitHack
    , modeRun
  };

  void m_link();
  void m_generate_description();

  TuplesDefaultImpl(const TuplesDefaultImpl &);
  TuplesDefaultImpl &operator=(const TuplesDefaultImpl &);

  bool m_exec;
  PyObject *m_tuple;
  PyObject *m_kw;
  PyObject *m_retval;

  TupleInDefaultImpl *m_current_tuple_in() const;
  TupleOutDefaultImpl *m_current_tuple_out() const;

  virtual bool m_execute_enabled() const;
  virtual void m_add_tuple_in(const char */*_description*/, const options /*_opts*/, const int8_t /*_required*/);
  virtual void m_build_format_in();
  virtual bool m_parse(va_list ap);
  virtual void m_parse_no_arg();
  virtual void m_add_tuple_out(const char */*_description*/, const options /*_opts*/);
  virtual void m_build_format_out();
  virtual void m_build(va_list ap);
  virtual void m_build_no_value();
  virtual void m_throw_exception();
  virtual bool m_error_occurred();
};

} //namespace pyhrol

#endif //__pyhrol_tuples_default_impl_h__
