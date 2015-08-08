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

#include <cxxabi.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <Python.h>

#include "demangling.h"
#include "self_addressed_cstring.hpp"
#include "pyhrol_container.h"
#include "pyhrol_exception_handler.h"
#include "pyhrol_tuples_exceptions.h"
#include "pyhrol_tuples_default_impl.h"
#include "pyhrol_tuple_describer.h"
#include "pyhrol_tuple_format.h"
#include "pyhrol_tuple_in_default_impl.h"
#include "pyhrol_tuple_out_default_impl.h"
#include "pyhrol_tuples_data2_default_impl.h"
#include "pyhrol_trace.h"

using namespace std;

extern bool _G_signature_hack_enabled;

namespace pyhrol
{


TuplesDefaultImpl::TuplesDefaultImpl(TuplesData2DefaultImpl &data)
  : TupleState(data)
  , m_exec(false)
  , m_tuple(NULL)
  , m_kw(NULL)
  , m_retval(NULL)
{
}

TupleIn &TuplesDefaultImpl::t_in() const
{
  m_check_input();
  return *m_current_tuple_in();
}

TupleOut &TuplesDefaultImpl::t_out() const
{
  m_check_output();
  return *m_current_tuple_out();
}

const TuplesDefaultImpl::tuples_in_t &TuplesDefaultImpl::t_ins() const
{
  return dynamic_cast<TuplesData2DefaultImpl &>(m_data).m_tuples_in;
}

const TuplesDefaultImpl::tuples_out_t &TuplesDefaultImpl::t_outs() const
{
  return dynamic_cast<TuplesData2DefaultImpl &>(m_data).m_tuples_out;
}

const callVariants TuplesDefaultImpl::cv() const
{
  return m_data.cv;
}

void TuplesDefaultImpl::m_generate_description()
{
  TuplesData2DefaultImpl &data = dynamic_cast<TuplesData2DefaultImpl &>(m_data);
  const char **pdoc = NULL;
  try
  {
    pdoc = Container::container().doc_by_address(data.m_address);
  }
  catch (const Container::InvalidLink &ex)
  {
    if (!PyErr_Occurred())
    {
      PyErr_Format(PyExc_LookupError, "%s (%s)", ex.what(), Container::container().context_by_address(data.m_address).c_str());
      PyErr_PrintEx(0);
      PyErr_Clear();
    }
    /*NOTE
    The most probable error occured before -- ParseException
    If it occured this error information silently lost
    */
  }

  if (pdoc)
  {
    TupleDescriber *pdescr = *pdoc ? TupleDescriber::from_string(*pdoc) : TupleDescriber::factory();
    if (pdescr)
    {
      pdescr->describe(*this);
      const char *ptr = pdescr->to_string();
      if (ptr)
      {
        *pdoc = ptr;
      }
      else
      {
        *pdoc = NULL;
        delete pdescr;
      }
    }
  }
}

PyObject *TuplesDefaultImpl::ubiquitous_caller
(
    exceptionHandler &h
  , PyObject *args
  , PyObject *kwds
  , const bool is_probe
  , const callerTypes ct
)
{
  const callVariants cv = m_data.cv;
  const modes mode = is_probe ? (_G_signature_hack_enabled ? modeInitHack : modeInit) : modeRun;
  TuplesData2DefaultImpl &data = dynamic_cast<TuplesData2DefaultImpl &>(m_data);

  if (mode == modeRun && !m_data.m_valid)
  {
    PyErr_Format(PyExc_AttributeError, "Tuple invalid due to one or more previous errors. Call impossible in any way (%s)", Container::container().context_by_address(data.m_address).c_str());
    return NULL;
  }

  if (ct == ctGetter)
  {
    TupleState::set_mode_getter();
  }
  else if (ct == ctSetter)
  {
    TupleState::set_mode_setter();
  }

  if (mode == modeRun)
  {
    m_exec = true;
    m_tuple = args;
    m_kw = kwds;
    m_retval = NULL;
  }

  if (mode == modeRun || mode == modeInitHack)
  {
    m_clear_before_execute();

    bool
        just_filled = false
      , exception_thrown
    ;

    try
    {
      exception_thrown = !exceptionHandler::call(h, data.m_address);
      just_filled = !exception_thrown;
    }
    catch (const ParseException &ex)
    {
      //Неверные аргументы Python; только в режиме исполнения
      PyErr_Format(PyExc_TypeError, "%s (%s)", ex.what(), Container::container().context_by_address(data.m_address).c_str());
      exception_thrown = true;
      just_filled = true;
    }

    /* NOTE
    Despite of exception thrown user call can leave return value inited
    Non null return value in case of not filled tuple is unbelievable internal error
    */
    if (m_retval && (exception_thrown || !TupleState::is_filled()))
    {
      Py_DecRef(m_retval);
      m_retval = NULL;
    }

    if (mode == modeInitHack && PyErr_Occurred())
    {
      PyErr_PrintEx(0);
      PyErr_Clear();
    }

    if (!TupleState::is_filled())
    {
      /* NOTE
      Tuple must be filled during first call in any way.
      Any exceptions or invalid actions inside (TuplesData::*call_t) whereby tuple not filled
      during first call causes inevitable invalidation. Any subsequent calls waved aside
      This in the same way as for v_getsetter calls: even if getter filled successfully, error in setter
      invalidates tuple at all and virce versa
      */
      m_data.m_valid = false;
      just_filled = false;
      if (!exception_thrown)
      {
        PyErr_Format(PyExc_AttributeError, "Tuple invalid due to unknown reason. Any further calls impossible in any way (%s)", Container::container().context_by_address(data.m_address).c_str());
      }
    }
    else if (just_filled)
    {
      //Here: user call executed successfully or caused parse error
      just_filled = false;
      try
      {
        just_filled = TupleState::just_filled();
      }
      catch (const CheckCalls::ForbiddenCall &ex)
      {
        PyErr_Format(PyExc_AttributeError, "%s (%s)", ex.what(), Container::container().context_by_address(data.m_address).c_str());
      }
    }

    if (just_filled || m_data.m_valid && (mode == modeInit || mode == modeInitHack))
    {
      m_generate_description();
    }
  }

  if (cv == v_getsetter && TupleState::mode() == TupleFillState::modSetter || cv == v_setter || cv == v_protof || cv == v_ctor)
  {
    return NULL;
  }

  if (!PyErr_Occurred() && !m_retval && mode == modeRun)
  {
    Py_IncRef(Py_None);
    m_retval = Py_None;
  }
  return m_retval;
}

TupleInDefaultImpl *TuplesDefaultImpl::m_current_tuple_in() const
{
  const uint8_t offset = m_parse_variant_current();
  TuplesData2DefaultImpl &data = dynamic_cast<TuplesData2DefaultImpl &>(m_data);

  /* NOTE What about out of bound here?
  TupleState::prepare_parse calls m_add_tuple_in() and mParseVariant ++ simultaneously in fill mode,
  but after filling it reset mParseVariant and calls mParseVariant ++ only. Yes, it is possible
  */
  if (offset >= data.m_tuples_in.size())
  {
    throw OverflowException(offset, data.m_tuples_in.size(), OverflowException::dInput);
  }
  return data.m_tuples_in[offset];
}

TupleOutDefaultImpl *TuplesDefaultImpl::m_current_tuple_out() const
{
  const uint8_t offset = m_build_variant_current();
  TuplesData2DefaultImpl &data = dynamic_cast<TuplesData2DefaultImpl &>(m_data);

  /* NOTE What about out of bound here?
  TupleState::prepare_build calls m_add_tuple_out() and mBuildVariant ++ simultaneously in fill mode,
  but after filling it reset mBuildVariant and calls mParseVariant ++ only. Yes, it is possible
  */
  if (offset >= data.m_tuples_out.size())
  {
    throw OverflowException(offset, data.m_tuples_out.size(), OverflowException::dOutput);
  }
  return data.m_tuples_out[offset];
}


bool TuplesDefaultImpl::m_execute_enabled() const
{
  return m_exec;
}

void TuplesDefaultImpl::m_add_tuple_in(const char *_description, const options _opts, const int8_t _required)
{
  dynamic_cast<TuplesData2DefaultImpl &>(m_data).m_tuples_in.push_back(new TupleInDefaultImpl(_description, _opts, _required));
}

void TuplesDefaultImpl::m_build_format_in()
{
  m_current_tuple_in()->build_format(m_data.cv & f_keywords);
}

bool TuplesDefaultImpl::m_parse(va_list ap)
{
  bool ret_val = false;
  TupleInDefaultImpl *ptupleIn = m_current_tuple_in();


  if (!m_tuple)
  {
    /*
    NOTE в режиме execute не исключено значение m_tuple = NULL; это означает, что:
      -- для функции PyNoArgsFunction (флаг METH_NOARGS) вызывается проверка аргументов
      -- метод создан только для получения данных (TupleFillState::modGetter)
    в обоих случаях исключение сработает раньше -- в TupleState::parse
    */
    throw ArgsException();
  }

  try
  {
    char * const * keywords = ptupleIn->keywords();
    if
    (
        ptupleIn->args().empty() && !PyObject_Length(m_tuple) && (!m_kw || !PyObject_Length(m_kw))
      || m_kw && PyArg_VaParseTupleAndKeywords(m_tuple, m_kw, ptupleIn->format().c_str(), const_cast<char **>(keywords), ap)
      || !m_kw && PyArg_VaParse(m_tuple, ptupleIn->format().c_str(), ap)
    )
    {
      ret_val = true;
    }
  }
  catch (const TupleBase::FormatException &ex)
  {
    /*TODO
    Возможно правильнее было бы перехватить здесь исключение
    Вопрос особенно актуален, когда сигнатур несколько и большая часть из них правильные
    см также function_dublicate в pytest_pyhrol_keywords.cpp -- этот эффект будет ликвидирован, а многие тесты будут провалены
    ptupleIn->parseError = ex.what();
    */
    throw;
  }

  if (!ret_val && PyErr_Occurred())
  {
    PyObject *ptype, *pvalue, *ptraceback;
    PyErr_Fetch(&ptype, &pvalue, &ptraceback);

    const char *pStrErrorMessage = PyString_AsString(pvalue);
    if (pStrErrorMessage)
    {
      ptupleIn->parseError = pStrErrorMessage;
    }
    Py_DecRef(ptype);
    Py_DecRef(pvalue);
    Py_DecRef(ptraceback);
    PyErr_Clear();
  }

  return ret_val;
}

void TuplesDefaultImpl::m_parse_no_arg()
{
  if (m_tuple && PyObject_Length(m_tuple))
  {
    throw ParseException ();
  }
}

void TuplesDefaultImpl::m_add_tuple_out(const char *_description, const options _opts)
{
  dynamic_cast<TuplesData2DefaultImpl &>(m_data).m_tuples_out.push_back(new TupleOutDefaultImpl (_description, _opts));
}

void TuplesDefaultImpl::m_build_format_out()
{
  m_current_tuple_out()->build_format(false);
}

void TuplesDefaultImpl::m_build(va_list ap)
{
  m_retval = Py_VaBuildValue(m_current_tuple_out()->format().c_str(), ap);
}

void TuplesDefaultImpl::m_build_no_value()
{
}

void TuplesDefaultImpl::m_throw_exception()
{
  ostringstream ostr;
  bool needDelimiter = false;

  const tuples_in_t &tuples_in = dynamic_cast<TuplesData2DefaultImpl &>(m_data).m_tuples_in;

  for (tuples_in_t::const_iterator iter = tuples_in.begin(); iter != tuples_in.end(); iter ++)
  {
    TupleInDefaultImpl *ptuple = *iter;
    if (!ptuple->parseError.empty())
    {
      if (needDelimiter)
      {
        ostr
          << endl
        ;
      }
      ostr << ptuple->format_no_throw() << ": " << ptuple->parseError;
      needDelimiter = true;
    }
    ptuple->parseError.clear();
  }
  //Ошибка, ни один из вариантов сигнатур не подходит
  throw ParseException(ostr.str(), tuples_in.size());
}

bool TuplesDefaultImpl::m_error_occurred()
{
  return PyErr_Occurred();
}

Tuples *Tuples::factory(TuplesData &data)
{
  /* NOTE
  This class designed to work in pair with TuplesData2DefaultImpl implementation of TuplesData
  No alternative at runtime, therefore it is compile time responsiblility
  Otherwise std::bad_cast exception guaranteed here
  */
  return new TuplesDefaultImpl(dynamic_cast<TuplesData2DefaultImpl &>(data));
}

} //namespace pyhrol

