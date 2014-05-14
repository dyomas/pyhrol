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
#include <sstream>
#include <stdexcept>

#include "pyhrol_tuple_base_safe.h"
#include "pyhrol_container.h"
#include "pyhrol_keywords.h"
#include "pyhrol_trace.h"

using namespace std;

namespace pyhrol
{

TupleBaseSafe::FormatException::FormatException()
  : AttributeException("Format string invalid, tuple inoperable")
{
}

TupleBaseSafe::TupleBaseSafe(const options _opts, const int8_t _required)
  : opts(_opts)
  , required(_required)
  , m_format_actual(false)
  , m_valid(true)
  , m_last_swapped(false)
{
  PYHROL_TRACE(tpInternal, this, __PRETTY_FUNCTION__);
}

TupleBaseSafe::TupleBaseSafe(const TupleBaseSafe &cp)
  : opts(cp.opts)
  , required(cp.required)
  , m_format_actual(cp.m_format_actual)
  , m_valid(cp.m_valid)
  , m_last_swapped(cp.m_last_swapped)
  , m_format(cp.m_format)
  , m_arg_names(cp.m_arg_names)
  , m_keywords(cp.m_keywords)
{
  PYHROL_TRACE(tpInternal, this, __PRETTY_FUNCTION__);
}

TupleBaseSafe::~TupleBaseSafe()
{
  PYHROL_TRACE(tpInternal, this, __PRETTY_FUNCTION__);
}

bool TupleBaseSafe::valid() const
{
  return m_valid;
}

const string &TupleBaseSafe::format() const
{
  if (!m_valid)
  {
    throw FormatException();
  }
  return m_format;
}

const ArraySquare &TupleBaseSafe::keywords() const
{
  if (!m_valid)
  {
    throw FormatException();
  }
  return m_keywords;
}

bool TupleBaseSafe::empty() const
{
  return m_arg_names.size() == 0;
}

void TupleBaseSafe::m_add_format_unit
(
    const char format_unit
  , const char *arg_name
  , const char format_unit2
)
{
  if (!m_format.length())
  {
    switch (static_cast<options>(opts & optItemsMask))
    {
      case optTuple:
        m_format += "()";
        break;
      case optList:
        m_format += "[]";
        break;
      case optDictionary:
        m_format += "{}";
        break;
      default:
        break;
    }
  }

  const bool braced = opts & optItemsMask;
  size_t cnt = 0;

  for (string::const_iterator iter = m_format.begin() + (braced ? 1 : 0), iter_end = m_format.end() - (braced ? 1 : 0); iter != iter_end; iter ++)
  {
    if (*iter != '*' && *iter != '|')
    {
      cnt ++;
    }
  }

  //[!&], O -> O[!&]
  //[szu], i -> [szu]#, кроме s*, z*
  //e, [st], i -> e[st]#

  enum argNameAction
  {
      anInsert
    , anReplace
    , anIgnore
  };
  argNameAction an = anInsert;

  size_t pos = braced ? m_format.length() - 1 : m_format.length();
  if (pos)
  {
    const char ch = m_format[pos - 1];
    if ((ch == '!' || ch == '&') && (format_unit != 'O' || cnt == required))
    {
      if (!m_last_swapped)
      {
        m_valid = false;
      }
    }
    else if (ch == 'e' && (format_unit != 's' && format_unit != 't' || cnt == required))
    {
      m_valid = false;
    }
  }

  if (cnt == required)
  {
    m_format.insert(pos ++, 1, '|');
  }

  m_last_swapped = false;
  if (format_unit == 'O' && pos)
  {
    const char ch = m_format[pos - 1];
    if (ch == '!' || ch == '&')
    {
      m_format.insert(pos - 1, 1, format_unit);
      m_last_swapped = true;
      an = anReplace;
    }
    else
    {
      m_format.insert(pos, 1, format_unit);
    }
  }
  else if (format_unit == 'i' && pos)
  {
    const char ch = m_format[pos - 1];
    if (ch == 's' || ch == 'z' || ch == 'u' || ch == 't')
    {
      m_format.insert(pos, 1, '#');
      an = anIgnore;
    }
    else
    {
      m_format.insert(pos, 1, format_unit);
    }
  }
  else
  {
    if (format_unit == 'e')
    {
      an = anIgnore;
    }
    m_format.insert(pos, 1, format_unit);
    if (format_unit2)
    {
      m_format.insert(pos + 1, 1, format_unit2);
    }
  }

  if (an == anInsert)
  {
    m_arg_names.push_back(arg_name ? arg_name : "");
  }
  else if (an == anReplace)
  {
    m_arg_names[m_arg_names.size() - 1] = arg_name ? arg_name : "";
  }
}

void TupleBaseSafe::build_format(const bool with_keywords)
{
  if (!m_format_actual)
  {
    m_format_actual = true;

    if (with_keywords)
    {
      Keywords keywords;
      for (strings_t::const_iterator iter = m_arg_names.begin(), iter_end = m_arg_names.end(); iter != iter_end; iter ++)
      {
        keywords.add(*iter);
      }

      try
      {
        keywords.build(m_keywords);
      }
      catch (const Keywords::ChoiceError &ex)
      {
      #ifdef _DEBUG_PYTHON_PYTUPLE_SAFE
        cerr
          << "*** ChoiceError: " << ex.what() << endl
        ;
      #endif //_DEBUG_PYTHON_PYTUPLE_SAFE
        m_valid = false;
      }
    }

    if (m_format.length())
    {
      const size_t pos = (opts & optItemsMask) ? m_format.length() - 1 : m_format.length();
      if (pos)
      {
        const char ch = m_format[pos - 1];
        if (ch == 'e' || (ch == '!' || ch == '&') && !m_last_swapped)
        {
          m_valid = false;
        }
      }
    }
  }
}

} //namespace pyhrol
