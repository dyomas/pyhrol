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

#ifndef __pyhrol_tuple_state_h__
#define __pyhrol_tuple_state_h__


#include <stdarg.h>
#include "pyhrol_tuples.h"
#include "pyhrol_check_calls.h"
#include "smart_flag.h"

namespace pyhrol
{

/*
Возможные сценарии:

P -- parse
B -- build
E -- execute
"" -- имитация

"P" "B" -- заполнение
P|"P" "B" E B -- заполнение и исполнение
P E B -- исполнение заполненных
*/

class TuplesDataDefaultImpl;

class TupleState: public Tuples
{
protected:
  const uint8_t m_parse_variant_current() const;
  const uint8_t m_build_variant_current() const;
  void m_check_input() const;
  void m_check_output() const;

  void m_clear_before_execute();

  virtual bool m_execute_enabled() const = 0;
  virtual void m_add_tuple_in(const char */*_description*/, const options /*_opts*/, const int8_t /*_required*/) = 0;
  virtual void m_build_format_in() = 0;
  virtual bool m_parse(va_list ap) = 0;
  virtual void m_parse_no_arg() = 0;
  virtual void m_add_tuple_out(const char */*_description*/, const options /*_opts*/) = 0;
  virtual void m_build_format_out() = 0;
  virtual void m_build(va_list ap) = 0;
  virtual void m_build_no_value() = 0;
  virtual void m_throw_exception() = 0;
  virtual bool m_error_occurred() = 0;

  TuplesDataDefaultImpl &m_data;

private:
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

  CheckCalls m_check;

  TupleState(const TupleState &);
  TupleState &operator= (const TupleState &);

public:
  TupleState(TuplesDataDefaultImpl &);

#ifdef _DEBUG_PYTHON_TUPLES_STATES
  void debug_show_state() const;
#endif //_DEBUG_PYTHON_TUPLES_STATES

  const TupleFillState::modes mode() const;

  /**
  @return
    true -- состояние изменилось
    false -- состояние неизменно
  */
  bool just_filled() const;
  bool is_filled() const;

  void set_mode_setter();
  void set_mode_getter();

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

#ifdef _DEBUG_PYTHON_TUPLES_STATES
  bool debug_just_filled() const;
  void debug_clear();
#endif //_DEBUG_PYTHON_TUPLES_STATES
};

} //namespace pyhrol

#endif //__pyhrol_tuple_state_h__
