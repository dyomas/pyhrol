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

#include <sys/types.h>
#include <cxxabi.h>
#include <sstream>

#include "pyhrol_tuple_base.h"
#include "pyhrol_container.h"
#include "pyhrol_keywords.h"
#include "pyhrol_trace.h"
#include "demangling.h"

using namespace std;

namespace pyhrol
{

const std::string TupleBase::FormatException::m_describe(const strings_t &errors)
{
  ostringstream ostr;

  if (errors.size() == 1)
  {
    ostr << "Tuple format invalid, it has format error: " << errors[0];
  }
  else
  {
    bool needDelimiter = false;
    ostr
      << "Tuple format invalid, it has " << errors.size() << " format errors:" << endl
    ;
    for (strings_t::const_iterator iter = errors.begin(); iter != errors.end(); iter ++)
    {
      if (needDelimiter)
      {
        ostr
          << endl
        ;
      }
      ostr << *iter;
      needDelimiter = true;
    }
  }
  return ostr.str();
}

TupleBase::FormatException::FormatException(const strings_t &errors)
  : AttributeException(m_describe(errors))
{
}

TupleBase::TupleBase(const char *_description, const options _opts, const int8_t _required)
  : description(m_parse_strings(_description))
  , opts(_opts)
  , required(_required)
  , m_format_actual(false)
{
  PYHROL_TRACE(tpInternal, this, __PRETTY_FUNCTION__);
}

TupleBase::TupleBase(const TupleBase &cp)
  : description(cp.description)
  , opts(cp.opts)
  , required(cp.required)
  , m_format_actual(cp.m_format_actual)
  , m_format(cp.m_format)
  , m_args(cp.m_args)
  , m_idx(cp.m_idx)
  , m_arg_names(cp.m_arg_names)
  , m_values(cp.m_values)
  , m_addresses(cp.m_addresses)
  , m_errors(cp.m_errors)
{
  PYHROL_TRACE(tpInternal, this, __PRETTY_FUNCTION__);
}

TupleBase::~TupleBase()
{
  PYHROL_TRACE(tpInternal, this, __PRETTY_FUNCTION__);
}

const string &TupleBase::format() const
{
  if (m_errors.size())
  {
    throw FormatException(m_errors);
  }
  return m_format;
}

const string TupleBase::format_no_throw() const
{
  if (m_errors.size())
  {
    return "<invalid>";
  }
  else if (!m_format.size())
  {
    return "\"\"";
  }
  return m_format;
}

const TupleBase::args_t &TupleBase::args() const
{
  return m_args;
}

const TupleBase::idx_t &TupleBase::index() const
{
  return m_idx;
}

const ArraySquare &TupleBase::keywords() const
{
  if (m_errors.size())
  {
    throw FormatException(m_errors);
  }
  return m_keywords;
}

const ArrayVoid &TupleBase::values() const
{
  return m_values;
}

const TupleBase::addresses_t &TupleBase::addresses() const
{
  return m_addresses;
}

const TupleBase::strings_t &TupleBase::errors() const
{
  return m_errors;
}

void TupleBase::value(ostream &os, const size_t pos) const
{
  size_t length;
  void const *pdata = m_values.get(length, pos);

  if (!pdata)
  {
    os << "?<idx>";
    return;
  }
  const arg &a = m_args[pos];
  const bool
      is_null = !m_addresses[pos]
    , is_optional = (required != requiredAll && pos >= static_cast<size_t>(required))
  ;
  bool shown = false;

  if (is_optional || a.format_unit == 'e')
  {
    switch (a.format_unit)
    {
      case 'c': //char
        if (length)
        {
          os << '\'' << *reinterpret_cast<const char *>(pdata) << '\'';
          shown = true;
        }
        break;
      case 'e': //const char *
      case 's': //char *
      case 't':
      case 'z':
        /* NOTE
        length = 0 may means empty C-string "" or uninitialized value
        */
        if (is_null)
        {
          os << "(null)";
        }
        else
        {
          os << '\"' << string(reinterpret_cast<const char *>(pdata), length) << '\"';
        }
        shown = true;
        break;
      case 'B': //bool
        if (length)
        {
          os << (*reinterpret_cast<const bool *>(pdata) ? "true" : "false");
          shown = true;
        }
        break;
      case 'b': //uint8_t
        if (length)
        {
          os << static_cast<const int16_t>(*reinterpret_cast<const uint8_t *>(pdata));
          shown = true;
        }
        break;
      case 'h': //int16_t
        if (length)
        {
          os << *reinterpret_cast<const int16_t *>(pdata);
          shown = true;
        }
        break;
      case 'H': //uint16_t
        if (length)
        {
          os << *reinterpret_cast<const uint16_t *>(pdata);
          shown = true;
        }
        break;
      case 'i': //int32_t
        if (length)
        {
          os << *reinterpret_cast<const int32_t *>(pdata);
          shown = true;
        }
        break;
      case 'I': //uint32_t
        if (length)
        {
          os << *reinterpret_cast<const uint32_t *>(pdata);
          shown = true;
        }
        break;
      case 'l': //long_t
        if (length)
        {
          os << *reinterpret_cast<const long *>(pdata);
          shown = true;
        }
        break;
      case 'k': //ulong_t
        if (length)
        {
          os << *reinterpret_cast<const unsigned long *>(pdata);
          shown = true;
        }
        break;
      case 'L': //int64_t
        if (length)
        {
          os << *reinterpret_cast<const int64_t *>(pdata);
          shown = true;
        }
        break;
      case 'K': //uint64_t
        if (length)
        {
          os << *reinterpret_cast<const uint64_t *>(pdata);
          shown = true;
        }
        break;
      case 'f': //float
        if (length)
        {
          os << *reinterpret_cast<const float *>(pdata);
          shown = true;
        }
        break;
      case 'd': //double
        if (length)
        {
          os << *reinterpret_cast<const double *>(pdata);
          shown = true;
        }
        break;
      case 'D':
        os << (is_null ? "NULL" : "<Py_complex>");
        shown = true;
        break;
      case '!':
        if (length)
        {
          os << '<' << reinterpret_cast<const PyTypeObject *>(pdata)->tp_name << '>';
          shown = true;
        }
        break;
      case 'U':
        os << (is_null ? "NULL" : "<PyUnicodeObject>");
        shown = true;
        break;
      case 'S':
        os << (is_null ? "NULL" : "<PyStringObject>");
        shown = true;
        break;
      case 'u':
        os << (is_null ? "NULL" : "<Py_UNICODE>");
        shown = true;
        break;
      case '&': //in_conv_t, out_cconv_t
        os << m_arg_names[pos];
        shown = true;
        break;
      case 'N':
      case 'O': //const void *value, const PyObject *
        os << (is_null ? "NULL" : "<PyObject>");
        shown = true;
        break;
      default:
        os << '?' << a.format_unit;
        if (a.format_unit2)
        {
          os << a.format_unit2;
        }
        os << '?';
        shown = true;
        break;
    }
  }

  if (!shown)
  {
    os << '?';
  }
}

const string TupleBase::value(const size_t pos) const
{
  ostringstream ostr;
  value(ostr, pos);
  return ostr.str();
}

void TupleBase::address(ostream &os, const size_t pos) const
{
  os << "0x" << hex << reinterpret_cast<size_t>(m_addresses[pos]) << dec;
}

const string TupleBase::address(const size_t pos) const
{
  ostringstream ostr;
  address(ostr, pos);
  return ostr.str();
}

void TupleBase::clear_values()
{
  m_values.clear();
  m_addresses.clear();
}

const TupleBase::strings_t TupleBase::m_parse_strings(const char *description)
{
  strings_t rows;
  if (description)
  {
    const char *begin = NULL;
    const char *end = NULL;

    while(parse_string(begin, end, description, '\n'))
    {
      rows.push_back(string(begin, end));
    }
  }
  return rows;
}

bool TupleBase::m_arg_is_optional() const
{
  return required != requiredAll && m_args.size() >= static_cast<size_t>(required);
}

void TupleBase::m_add_format_unit
(
    const char format_unit
  , const char *type_name
  , const char *arg_name
  , const void *value
  , const size_t value_length
  , const char format_unit2
)
{
  if (!m_format_actual)
  {
    struct arg _arg = {format_unit, format_unit2, type_name ? type_name : ""};
    m_args.push_back(_arg);
    m_arg_names.push_back(arg_name ? arg_name : "");
  }
  m_values.append(value, value ? value_length : 0);
  m_addresses.push_back(value);
}

void TupleBase::m_add_error(const string &err_descr)
{
  m_errors.push_back(err_descr);
}

void TupleBase::m_swap_type_names(const size_t pos)
{
  arg
      &t = m_args[pos -1] //PyTypeObject
    , &o = m_args[pos] //PyObject
  ;

  o.type_name = "$" + t.type_name;
  t.type_name = "PyTypeObject";
}

void TupleBase::build_format(const bool with_keywords)
{
  const options _itms = static_cast<options>(opts & optItemsMask);

  if (m_format_actual)
  {
    return;
  }

  switch (_itms)
  {
    case optTuple:
      m_format += '(';
      break;
    case optList:
      m_format += '[';
      break;
    case optDictionary:
      m_format += '{';
      break;
    default:
      break;
  }

  const args_t::const_iterator
      begin = m_args.begin()
    , end = m_args.end()
  ;
  args_t::const_iterator iter_group = end;
  int8_t cnt_total = 0;

  for (args_t::const_iterator iter = begin; iter != end; iter ++)
  {
    const arg &_arg = *iter;

    if (cnt_total == required)
    {
      if (iter_group != end)
      {
        if (iter_group->format_unit == '&' || iter_group->format_unit == '!' || (iter_group->format_unit == 'e' && iter - iter_group == 1))
        {
          ostringstream ostr;
          ostr << "Expected \'" << (iter_group->format_unit == 'e' ? "s\' or \'t" : "O") << "\' format unit after \'" << iter_group->format_unit << "\', but found optional parameters delimiter \'|\' (position " << m_idx.size() << '.' << static_cast<int16_t>(cnt_total) << ")";
          m_add_error(ostr.str());
        }
        else
        {
          idx _idx = {iter_group - begin, cnt_total, cnt_total - 1, false};
          m_idx.push_back(_idx);
          while (iter_group != iter)
          {
            m_format += (iter_group ++)->format_unit;
          }
        }
        iter_group = end;
      }
      m_format += '|';
    }

    bool grouped = false;

    if (iter_group != end)
    { // Параметр может описываться несколькими символами
      if (iter - iter_group == 1)
      {
        if (iter_group->format_unit == '&' || iter_group->format_unit == '!')
        {
          if (_arg.format_unit == 'O')
          {
            m_format += _arg.format_unit;
            m_format += iter_group->format_unit;
            idx _idx = {iter_group - begin, cnt_total + 1, cnt_total, required != requiredAll && (cnt_total - 1) >= required};
            m_idx.push_back(_idx);
            if (iter_group->format_unit == '!')
            {
              m_swap_type_names(iter - m_args.begin());
            }
            grouped = true;
          }
          else
          {
            ostringstream ostr;
            ostr << "Expected \'O\' format unit after \'" << iter_group->format_unit << "\', but found \'" << _arg.format_unit;
            if (_arg.format_unit2)
            {
              ostr << _arg.format_unit2;
            }
            ostr << "\' (position " << m_idx.size() << '.' << static_cast<int16_t>(cnt_total) << ")";
            m_add_error(ostr.str());
          }
          iter_group = end;
        }
        else if (iter_group->format_unit == 'e')
        {
          if (_arg.format_unit == 's' || _arg.format_unit == 't')
          {
            grouped = true;
          }
          else
          {
            ostringstream ostr;
            ostr << "Expected \'s\' or \'t\' format unit after \'" << iter_group->format_unit << "\', but found \'" << _arg.format_unit;
            if (_arg.format_unit2)
            {
              ostr << _arg.format_unit2;
            }
            ostr << "\' (position " << m_idx.size() << '.' << static_cast<int16_t>(cnt_total) << ")";
            m_add_error(ostr.str());
            iter_group = end;
          }
        }
        else if (_arg.format_unit == 'i')
        {
          m_format += iter_group->format_unit;
          m_format += '#';
          idx _idx = {iter_group - begin, cnt_total + 1, cnt_total - 1, required != requiredAll && (cnt_total - 1) >= required};
          m_idx.push_back(_idx);
          grouped = true;
          iter_group = end;
        }
        else
        {
          m_format += iter_group->format_unit;
          idx _idx = {iter_group - begin, cnt_total, cnt_total - 1, required != requiredAll && (cnt_total - 1) >= required};
          m_idx.push_back(_idx);
          iter_group = end;
        }
      }
      else if (iter - iter_group == 2)
      {// e[st]
        if (_arg.format_unit == 'i')
        {
          idx _idx = {iter_group - begin, cnt_total + 1, cnt_total - 1, required != requiredAll && (cnt_total - 1) >= required};
          grouped = true;
          m_idx.push_back(_idx);
        }
        else
        {
          idx _idx = {iter_group - begin, cnt_total, cnt_total - 1, required != requiredAll && (cnt_total - 1) >= required};
          m_idx.push_back(_idx);
        }
        while (iter_group != iter)
        {
          m_format += (iter_group ++)->format_unit;
        }
        if (_arg.format_unit == 'i')
        {
          m_format += '#';
        }
        iter_group = end;
      }
    }

    if (!grouped)
    {
      if ((_arg.format_unit == 's' || _arg.format_unit == 'z') && _arg.format_unit2 != '*' || _arg.format_unit == 'e' || _arg.format_unit == 'u' || _arg.format_unit == '&' || _arg.format_unit == '!')
      { // Параметр может описываться несколькими символами, анализировать на следующих итерациях
        iter_group = iter;
      }
      else
      {
        m_format += _arg.format_unit;
        if (_arg.format_unit2)
        {
          m_format += _arg.format_unit2;
        }
        idx _idx = {cnt_total, cnt_total + 1, cnt_total, required != requiredAll && cnt_total >= required};
        m_idx.push_back(_idx);
      }
    }
    cnt_total ++;
  }

  if (iter_group != end)
  {
    const char last_format_unit = m_args.rbegin()->format_unit;
    if (iter_group->format_unit == '&' || iter_group->format_unit == '!' || (iter_group->format_unit == 'e' && last_format_unit != 's' && last_format_unit != 't'))
    {
      ostringstream ostr;
      ostr << "Expected \'" << (iter_group->format_unit == 'e' ? "s\' or \'t" : "O") << "\' format unit after \'" << iter_group->format_unit << "\', but end of units list reached (position " << m_idx.size() << '.' << static_cast<int16_t>(cnt_total) << ")";
      m_add_error(ostr.str());
    }
    else
    {
      idx _idx = {iter_group - begin, cnt_total, cnt_total - 1, required != requiredAll && cnt_total >= required};
      while (iter_group != end)
      {
        m_format += (iter_group ++)->format_unit;
      }
      m_idx.push_back(_idx);
    }
  }

  switch (_itms)
  {
    case optTuple:
      m_format += ')';
      break;
    case optList:
      m_format += ']';
      break;
    case optDictionary:
      m_format += '}';
      break;
    default:
      break;
  }

  if (with_keywords && !m_errors.size())
  {
    Keywords keywords;
    for (idx_t::const_iterator iter = m_idx.begin(), iter_end = m_idx.end(); iter != iter_end; iter ++)
    {
      keywords.add(m_arg_names[iter->significant]);
    }

    try
    {
      keywords.build(m_keywords);
    }
    catch (const Keywords::ChoiceError &ex)
    {
      m_add_error(ex.what());
    }
  }

  m_format_actual = true;
}

const string demangle(const std::type_info &ti)
{
  return ::demangle(ti);
}

} //namespace pyhrol
