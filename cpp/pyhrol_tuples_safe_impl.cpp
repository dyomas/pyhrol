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
#include "pyhrol_tuples_safe_impl.h"
#include "pyhrol_tuple_describer.h"
#include "pyhrol_tuple_in_safe_impl.h"
#include "pyhrol_tuple_out_safe_impl.h"
#include "pyhrol_tuples_data_safe_impl.h"
#include "pyhrol_trace.h"
#include "nullable.hpp"
#include "smart_flag.hpp"

using namespace std;

namespace pyhrol
{

TuplesSafeImpl::TuplesSafeImpl(TuplesDataSafeImpl &data)
  : m_caller_type(ctUnspecified)
  , m_tuple(NULL)
  , m_kw(NULL)
  , m_retval(NULL)
  , m_data(data)
  , mParseVariant(0)
  , mBuildVariant(0)
  , m_required(requiredAll)
  , m_options(optNo)
{
}

TupleIn &TuplesSafeImpl::t_in() const
{
  return *m_data.m_tuples_in[mParseVariant - 1];
}

TupleOut &TuplesSafeImpl::t_out() const
{
  return *m_data.m_tuples_out[mBuildVariant - 1];
}

bool TuplesSafeImpl::m_filled() const
{
  return
       m_caller_type == ctGetter && (m_data.m_fill_state & TuplesDataSafeImpl::fsFilledOut)
    || m_caller_type == ctSetter && (m_data.m_fill_state & TuplesDataSafeImpl::fsFilledIn)
    || m_caller_type == ctUnspecified && ((m_data.m_fill_state & TuplesDataSafeImpl::fsFilledOut) || (m_data.m_fill_state & TuplesDataSafeImpl::fsFilledIn));
}

void TuplesSafeImpl::m_check_parse_state() const
{
  if (mVariantParsed.isNull())
  {
    if (mParseVariant)
    {
      ostringstream ostr;
      bool needDelimiter = false;

      for (tuples_in_t::const_iterator iter = m_data.m_tuples_in.begin(); iter != m_data.m_tuples_in.end(); iter ++)
      {
        TupleInSafeImpl *ptuple = *iter;
        if (!ptuple->parseError.empty())
        {
          if (needDelimiter)
          {
            ostr
              << endl
            ;
          }
          if (ptuple->format().length())
          {
            ostr << ptuple->format();
          }
          else
          {
            ostr << "\"\"";
          }
          ostr << ": " << ptuple->parseError;
          needDelimiter = true;
        }
        ptuple->parseError.clear();
      }
      throw ParseException(ostr.str(), m_data.m_tuples_in.size());
    }
    else if (m_tuple && PyObject_Length(m_tuple))
    {
      throw ParseException();
    }
  }
}

PyObject *TuplesSafeImpl::ubiquitous_caller
(
    exceptionHandler &h
  , PyObject *args
  , PyObject *kwds
  , const bool is_probe
  , const callerTypes ct
)
{
  const callVariants cv = m_data.cv;

  if (is_probe)
  {
    return NULL;
  }

  if (!m_data.m_valid)
  {
    PyErr_Format(PyExc_AttributeError, "Tuple invalid due to one or more previous errors. Call impossible in any way (%s)", Container::container().context_by_address(m_data.m_address).c_str());
    return NULL;
  }

  m_caller_type = ct;
  m_tuple = args;
  m_kw = kwds;

  if (!(m_data.m_fill_state & TuplesDataSafeImpl::fsFilledOut) && ct != ctSetter)
  {
    m_data.mBuildVariantsCnt = 0;
  }

  const bool exception_thrown = !exceptionHandler::call(h, m_data.m_address);

  //Despite of exception thrown user call can leave return value inited
  if (exception_thrown && m_retval)
  {
    Py_DecRef(m_retval);
    m_retval = NULL;
  }

  if (!m_filled())
  {
    m_data.m_valid = false;
    if (!exception_thrown)
    {
      PyErr_Format(PyExc_AttributeError, "Tuple invalid due to unknown reason. Any further calls impossible in any way (%s)", Container::container().context_by_address(m_data.m_address).c_str());
    }
  }


  if (cv == v_getsetter && ct == ctSetter || cv == v_setter || cv == v_protof || cv == v_ctor)
  {
    return NULL;
  }

  if (!PyErr_Occurred() && !m_retval)
  {
    Py_IncRef(Py_None);
    m_retval = Py_None;
  }
  return m_retval;
}

bool TuplesSafeImpl::m_parse(va_list ap)
{
  bool ret_val = false;
  TupleInSafeImpl *ptupleIn = m_data.m_tuples_in[mParseVariant - 1];

  char * const * keywords = ptupleIn->keywords();

  //TODO См TuplesDefaultImpl::m_parse
  if
  (
       ptupleIn->empty() && !PyObject_Length(m_tuple) && (!m_kw || !PyObject_Length(m_kw))
    || m_kw && PyArg_VaParseTupleAndKeywords(m_tuple, m_kw, ptupleIn->format().c_str(), const_cast<char **>(keywords), ap)
    || !m_kw && PyArg_VaParse(m_tuple, ptupleIn->format().c_str(), ap)
  )
  {
    ret_val = true;
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

void TuplesSafeImpl::set_options(const uint8_t _required)
{
  m_required = _required;
}

void TuplesSafeImpl::set_options(const options _opts)
{
  m_options = _opts;
}

void TuplesSafeImpl::set_options(const options _opts, const uint8_t _required)
{
  m_required = _required;
  m_options = _opts;
}

bool TuplesSafeImpl::prepare_parse(const char */*description*/)
{
  bool ret_val = false;

  if (!(m_data.m_fill_state & TuplesDataSafeImpl::fsFilledIn))
  {
    ret_val = true;
    m_data.m_tuples_in.push_back(new TupleInSafeImpl(m_options, m_required));
    m_required = requiredAll;
    m_options = optNo;
  }

  mParseVariant ++;
  return ret_val;
}

bool TuplesSafeImpl::is_parse_enabled()
{
  bool ret_val = false;

  m_data.m_tuples_in[mParseVariant - 1]->build_format(m_data.cv & f_keywords);

  if (mVariantParsed.isNull())
  {
    ret_val = true;
  }

  return ret_val;
}

void TuplesSafeImpl::parse(const bool dummy, ...)
{
  va_list ap;
  va_start(ap, dummy);
  const bool res = m_parse(ap);
  va_end(ap);

  if (res)
  {
    mVariantParsed = mParseVariant - 1;
  }
}

bool TuplesSafeImpl::before_build()
{
  bool ret_val = false;

  if (!m_filled() || mVariantBuilt.assigned())
  {
    ret_val = true;
  }
  else
  {
    m_check_parse_state();
  }
  mBuildVariant = 0;

  return ret_val;
}

bool TuplesSafeImpl::prepare_build(const char */*description*/)
{
  bool ret_val = false;

  if (!(m_data.m_fill_state & TuplesDataSafeImpl::fsFilledOut))
  {
    ret_val = true;
    m_data.mBuildVariantsCnt ++;
    m_data.m_tuples_out.push_back(new TupleOutSafeImpl (m_options));
    m_required = requiredAll;
    m_options = optNo;
  }
  mBuildVariant ++;

  return ret_val;
}

bool TuplesSafeImpl::is_build_enabled()
{
  bool ret_val = false;

  m_data.m_tuples_out[mBuildVariant - 1]->build_format(false);

  if (mVariantBuilt.assigned() && mBuildVariant && (mVariantBuilt.getValue() == mBuildVariant - 1) && !PyErr_Occurred())
  {
    ret_val = true;
  }

  return ret_val;
}

void TuplesSafeImpl::build(const bool dummy, ...)
{
  va_list ap;
  va_start(ap, dummy);
  m_retval = Py_VaBuildValue(m_data.m_tuples_out[mBuildVariant - 1]->format().c_str(), ap);
  va_end(ap);
}

bool TuplesSafeImpl::after_build()
{
  if (mVariantBuilt.isNull())
  {
    if (m_caller_type == ctGetter)
    {
      m_data.m_fill_state = static_cast<TuplesDataSafeImpl::fillState>(m_data.m_fill_state | TuplesDataSafeImpl::fsFilledOut);
    }
    else if (m_caller_type == ctSetter)
    {
      m_data.m_fill_state = static_cast<TuplesDataSafeImpl::fillState>(m_data.m_fill_state | TuplesDataSafeImpl::fsFilledIn);
    }
    else
    {
      m_data.m_fill_state = static_cast<TuplesDataSafeImpl::fillState>(m_data.m_fill_state | (TuplesDataSafeImpl::fsFilledIn | TuplesDataSafeImpl::fsFilledOut));
    }

    m_check_parse_state();
  }

  return mVariantBuilt.assigned();
}

const uint8_t TuplesSafeImpl::parsed_variant() const
{
  if (mVariantParsed.isNull())
  {
    throw UseException();
  }
  return mVariantParsed.getValue();
}

void TuplesSafeImpl::choose_build_variant(const uint8_t build_variant)
{
  if ((m_data.mBuildVariantsCnt && build_variant >= m_data.mBuildVariantsCnt) || (!m_data.mBuildVariantsCnt && build_variant))
  {
    throw UseException(build_variant, m_data.mBuildVariantsCnt);
  }
  mVariantBuilt = build_variant;
}

Tuples *Tuples::factory(TuplesData &data)
{
  /* NOTE
  This class designed to work in pair with TuplesDataSafeImpl implementation of TuplesData
  No alternative at runtime, therefore it is compile time responsiblility
  Otherwise std::bad_cast exception guaranteed here
  */
  return new TuplesSafeImpl(dynamic_cast<TuplesDataSafeImpl &>(data));
}

} //namespace pyhrol
