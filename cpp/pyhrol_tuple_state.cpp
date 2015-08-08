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

#include <iostream>

#include "nullable.hpp"
#include "pyhrol_tuple_state.h"
#include "pyhrol_tuples_data_default_impl.h"
#include "pyhrol_tuples_exceptions.h"
#include "pyhrol_trace.h"
#include "smart_flag.hpp"

using namespace std;

namespace pyhrol
{

const uint8_t TupleState::m_parse_variant_current() const
{
  if (!mParseVariant)
  {
    throw StateException("No tuple variants for parsing ready");
  }
  return mParseVariant - 1;
}

const uint8_t TupleState::m_build_variant_current() const
{
  if (!mBuildVariant)
  {
    throw StateException("No tuple variants for building ready");
  }
  return mBuildVariant - 1;
}

void TupleState::m_check_input() const
{
  m_check.check(CheckCalls::c_add_input);
}

void TupleState::m_check_output() const
{
  m_check.check(CheckCalls::c_add_output);
}

void TupleState::m_clear_before_execute()
{
  m_check.clear();
  mParseVariant = 0;
  mBuildVariant = 0;
  mVariantBuilt.clear();
  mVariantParsed.clear();
  if (!m_data.m_fill_state->filled() && m_data.m_fill_state->mode != TupleFillState::modSetter)
  {
    m_data.mBuildVariantsCnt = 0;
  }
}

TupleState::TupleState(TuplesDataDefaultImpl &data)
  : m_data(data)
  , mParseVariant(0)
  , mBuildVariant(0)
  , m_required(requiredAll)
  , m_options(optNo)
  , m_check(data.cv)
{
}

const TupleFillState::modes TupleState::mode() const
{
  return m_data.m_fill_state->mode;
}

#ifdef _DEBUG_PYTHON_TUPLES_STATES
void TupleState::debug_show_state() const
{
  if (!mVariantParsed.isNull() || !mVariantBuilt.isNull())
  {
    cout << "show_state: ";
    if (!mVariantParsed.isNull())
    {
      cout << "in " << static_cast<uint16_t>(mVariantParsed.getValue());
    }
    if (!mVariantBuilt.isNull())
    {
      if (!mVariantParsed.isNull())
      {
        cout << ", ";
      }
      cout << "out " << static_cast<uint16_t>(mVariantBuilt.getValue());
    }
    cout
      << endl
    ;
  }
}
#endif //_DEBUG_PYTHON_TUPLES_STATES

bool TupleState::just_filled() const
{
  m_check.check(CheckCalls::c_just_filled);
  return m_data.m_fill_state->justFilled();
}

bool TupleState::is_filled() const
{
  return m_data.m_fill_state->filled();
}

void TupleState::set_mode_setter()
{
  m_data.m_fill_state->mode = TupleFillState::modSetter;
}

void TupleState::set_mode_getter()
{
  m_data.m_fill_state->mode = TupleFillState::modGetter;
}

void TupleState::set_options(const uint8_t _required)
{
  m_check.check(CheckCalls::c_set_options);
  m_required = _required;
}

void TupleState::set_options(const options _opts)
{
  m_check.check(CheckCalls::c_set_options);
  m_options = _opts;
}

void TupleState::set_options(const options _opts, const uint8_t _required)
{
  m_check.check(CheckCalls::c_set_options);
  m_required = _required;
  m_options = _opts;
}

bool TupleState::prepare_parse(const char *description)
{
  bool ret_val = false;
  m_check.check(CheckCalls::c_prepare_parse, m_data.m_fill_state->mode);

  if (!m_data.m_fill_state->filled())
  {
  #ifdef _DEBUG_PYTHON_TUPLES_STATES
    cout
      << __func__ << endl
    ;
  #endif //_DEBUG_PYTHON_TUPLES_STATES
    ret_val = true;
    m_add_tuple_in(description, m_options, m_required);
    m_required = requiredAll;
    m_options = optNo;
  }

  mParseVariant ++;
  m_check.set_answer(ret_val);
  return ret_val;
}

bool TupleState::is_parse_enabled()
{
  bool ret_val = false;
  m_check.check(CheckCalls::c_is_parse_enabled);

  m_build_format_in();

  if (m_execute_enabled() && mVariantParsed.isNull())
  {
  #ifdef _DEBUG_PYTHON_TUPLES_STATES
    cout
      << __func__ << endl
    ;
  #endif //_DEBUG_PYTHON_TUPLES_STATES
    ret_val = true;
  }

  m_check.set_answer(ret_val);
  return ret_val;
}

void TupleState::parse(const bool dummy, ...)
{
  m_check.check(CheckCalls::c_parse);

  va_list ap;
  va_start(ap, dummy);
  const bool res = m_parse(ap);
  va_end(ap);

  if (res)
  {
  #ifdef _DEBUG_PYTHON_TUPLES_STATES
    cout
      << __func__ << ": " << static_cast<uint16_t>(mParseVariant) << endl
    ;
  #endif //_DEBUG_PYTHON_TUPLES_STATES
    mVariantParsed = mParseVariant - 1;
  }
}

bool TupleState::before_build()
{
  bool ret_val = false;
  m_check.check(CheckCalls::c_before_build);

  if (mVariantBuilt.isNull())
  {
    m_data.m_fill_state->touchFilled();
  }

  if (!m_data.m_fill_state->filled() || !mVariantBuilt.isNull() || !m_execute_enabled())
  {
  #ifdef _DEBUG_PYTHON_TUPLES_STATES
    cout
      << __func__ << endl
    ;
  #endif //_DEBUG_PYTHON_TUPLES_STATES
    ret_val = true;
  }
  else if (mVariantParsed.isNull())
  {
    if (mParseVariant)
    {
      m_throw_exception();
    }
    else
    {
      m_parse_no_arg();
    }
  }
  mBuildVariant = 0;

  m_check.set_answer(ret_val);
  return ret_val;
}

bool TupleState::prepare_build(const char *description)
{
  bool ret_val = false;
  m_check.check(CheckCalls::c_prepare_build, m_data.m_fill_state->mode);

  if (!m_data.m_fill_state->filled())
  {
  #ifdef _DEBUG_PYTHON_TUPLES_STATES
    cout
      << __func__ << endl
    ;
  #endif //_DEBUG_PYTHON_TUPLES_STATES
    ret_val = true;
    m_data.mBuildVariantsCnt ++;
    m_add_tuple_out(description, m_options);
    m_required = requiredAll;
    m_options = optNo;
  }
  mBuildVariant ++;

  m_check.set_answer(ret_val);
  return ret_val;
}

bool TupleState::is_build_enabled()
{
  bool ret_val = false;
  m_check.check(CheckCalls::c_is_build_enabled);

  m_build_format_out();

  if (m_execute_enabled() && !mVariantBuilt.isNull() && mBuildVariant && (mVariantBuilt.getValue() == mBuildVariant - 1) && !m_error_occurred())
  {
  #ifdef _DEBUG_PYTHON_TUPLES_STATES
    cout
      << __func__ << ": " << static_cast<uint16_t>(mBuildVariant - 1) << endl
    ;
  #endif //_DEBUG_PYTHON_TUPLES_STATES
    ret_val = true;
  }

  m_check.set_answer(ret_val);
  return ret_val;
}

void TupleState::build(const bool dummy, ...)
{
#ifdef _DEBUG_PYTHON_TUPLES_STATES
  cout
    << __func__ << endl
  ;
#endif //_DEBUG_PYTHON_TUPLES_STATES
  m_check.check(CheckCalls::c_build);

  va_list ap;
  va_start(ap, dummy);
  m_build(ap);
  va_end(ap);
}

bool TupleState::after_build()
{
  bool ret_val = false;
  m_check.check(CheckCalls::c_after_build);

  if (mVariantBuilt.isNull() && !m_data.m_fill_state->filled())
  {
    m_data.m_fill_state->confirmFilled();
  }
  if (!mVariantBuilt.isNull() || !m_execute_enabled())
  {
  #ifdef _DEBUG_PYTHON_TUPLES_STATES
    cout
      << __func__ << endl
    ;
  #endif //_DEBUG_PYTHON_TUPLES_STATES
    ret_val = true;
  }
  else if (mVariantParsed.isNull())
  {
    if (mParseVariant)
    {
      m_throw_exception();
    }
    else
    {
      m_parse_no_arg();
    }
  }
  if (m_execute_enabled() && !mVariantBuilt.isNull() && !m_data.mBuildVariantsCnt)
  {
    m_build_no_value();
  }

  m_check.set_answer(ret_val);
  return ret_val;
}

const uint8_t TupleState::parsed_variant() const
{
#ifdef _DEBUG_PYTHON_TUPLES_STATES
  cout
    << __func__ << endl
  ;
#endif //_DEBUG_PYTHON_TUPLES_STATES
  m_check.check(CheckCalls::c_parsed_variant);
  if (mVariantParsed.isNull())
  {
    throw UseException();
  }
  return mVariantParsed.getValue();
}

void TupleState::choose_build_variant(const uint8_t build_variant)
{
#ifdef _DEBUG_PYTHON_TUPLES_STATES
  cout
    << __func__ << endl
  ;
#endif //_DEBUG_PYTHON_TUPLES_STATES
  m_check.check(CheckCalls::c_choose_build_variant);
  if ((m_data.mBuildVariantsCnt && build_variant >= m_data.mBuildVariantsCnt) || (!m_data.mBuildVariantsCnt && build_variant))
  {
    throw UseException(build_variant, m_data.mBuildVariantsCnt);
  }
  mVariantBuilt = build_variant;
}

#ifdef _DEBUG_PYTHON_TUPLES_STATES
bool TupleState::debug_just_filled() const
{
  return m_data.m_fill_state->justFilled();
}

void TupleState::debug_clear()
{
  m_data.m_fill_state->clear();
  m_check.clear();
  mParseVariant = 0;
  mBuildVariant = 0;
  m_data.mBuildVariantsCnt = 0;
  mVariantBuilt.clear();
  mVariantParsed.clear();
}
#endif //_DEBUG_PYTHON_TUPLES_STATES

} //namespace pyhrol
