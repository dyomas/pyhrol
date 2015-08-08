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

// $Date: 2015-07-23 01:08:43 +0300 (Чт., 23 июля 2015) $
// $Revision: 1049 $

#include <sstream>
#include <iomanip>

#include "pyhrol_tuple_describer_flexible.h"
#include "pyhrol_container.h"
#include "pyhrol_tuple_in_default_impl.h"
#include "pyhrol_tuple_out_default_impl.h"
#include "pyhrol_tuples_default_impl.h"
#include "pyhrol_tuple_format.h"
#include "pyhrol_trace.h"
#include "self_addressed_cstring.hpp"

using namespace std;

namespace pyhrol
{

void TupleDescriberFlexible::m_append(const string &s, const bool remember_insert_position)
{
  if (m_help_length)
  {
    const size_t capacity = m_help_length + 1 + s.length() + 1;
    char *storage = m_allocate(capacity, this);
    if (storage[m_help_length - 1] == '\n')
    {
      memcpy(&storage[m_help_length], s.c_str(), s.length() + 1);
      if (remember_insert_position)
      {
        m_help_length = capacity - 2;
      }
    }
    else
    {
      storage[m_help_length] = '\n';
      memcpy(&storage[m_help_length + 1], s.c_str(), s.length() + 1);
      if (remember_insert_position)
      {
        m_help_length = capacity - 1;
      }
    }
  }
  else
  {
    const size_t capacity = s.length() + 1;
    memcpy(m_allocate(capacity, this), s.c_str(), capacity);
    if (remember_insert_position)
    {
      m_help_length = capacity - 1;
    }
  }
}

void TupleDescriberFlexible::m_describe(const TuplesDefaultImpl &tuples, const TupleFormat &format)
{
  const uint8_t width =
      tuples.t_ins().size() + tuples.t_outs().size() < 10
    ? 1
    : (tuples.t_ins().size() + tuples.t_outs().size() < 100 ? 2 : 3)
  ;
  TuplesDefaultImpl::tuples_in_t::const_iterator iter_in = tuples.t_ins().begin();
  TuplesDefaultImpl::tuples_out_t::const_iterator iter_out = tuples.t_outs().begin();
  const TupleBase *ptuple;
  tupleDirection dir = dirIn;
  uint8_t
      cnt = 0
    , cnt_in = 0
    , cnt_out = 0
  ;

  ostringstream ostr;

  if (tuples.cv() == v_ctor && (tuples.t_ins().size() || tuples.t_outs().size()))
  {
    ostr
      << "Constructors:" << endl
    ;
  }

  while (true)
  {
    if (dir == dirIn && iter_in != tuples.t_ins().end())
    {
      ptuple = *iter_in ++;
    }
    else if (iter_out != tuples.t_outs().end())
    {
      if (dir == dirIn)
      {
        dir = dirOut;
        cnt = 0;
      }
      ptuple = *iter_out ++;
    }
    else
    {
      break;
    }

    describeOptions opt;
    if (ptuple->description.size() == ptuple->index().size())
    {
      opt = optPerLine;
    }
    else if (ptuple->description.size() == ptuple->index().size() + 1)
    {
      opt = optPerLineWithHeader;
    }
    else
    {
      opt = optRaw;
    }

    if (dir == dirOut && !cnt_out && cnt_in)
    {
      ostr << endl;
    }

    if (ptuple->errors().size())
    {
      m_description_errors(ostr, *ptuple, format, dir, cnt);
    }
    else
    {
      TupleBase::strings_t::const_iterator iter_descs = ptuple->description.begin();
      m_description_formatted_header(ostr, iter_descs, *ptuple, format, dir, opt, cnt);
      m_description_formatted_body(ostr, iter_descs, *ptuple, format, dir, opt);
    }

    cnt ++;
    if (dir == dirIn)
    {
      cnt_in ++;
    }
    else if (dir == dirOut)
    {
      cnt_out ++;
    }
  }
  m_append(ostr.str(), false);
}

void TupleDescriberFlexible::m_description_errors
(
    ostream &os
  , const TupleBase &tuple
  , const TupleFormat &format
  , const tupleDirection dir
  , const uint8_t cnt
) const
{
  const TupleFormat::fields_t
      &headerFields = format.errorHeaderFields()
    , &bodyFields = format.errorBodyFields()
  ;
  const string
      &errorHeaderFormat = format.errorHeaderFormat()
    , &errorBodyFormat = format.errorBodyFormat()
  ;

  size_t begin = 0;

  for (TupleFormat::fields_t::const_iterator iter_fld = headerFields.begin(); iter_fld != headerFields.end(); iter_fld ++)
  {
    const TupleFormat::field_t &field = *iter_fld;
    os << errorHeaderFormat.substr(begin, field.before - begin);
    if (field.width >= 0)
    {
      os << setw(field.width);
    }
    os << ((field.flags & tffLeft) ? left : right);
    switch (field.specifier)
    {
      case tfpError:
        for (TupleBase::strings_t::const_iterator iter_err = tuple.errors().begin(); iter_err != tuple.errors().end(); iter_err ++)
        {
          if (iter_err != tuple.errors().begin())
          {
            os << endl;
          }
          os << *iter_err;
        }
        break;
      case tfpDirection: //d, направление (аргумент/значение)
        os << (dir == dirIn ? "->" : "<-");
        break;
      case tfpSerialNumber: //n[*] порядковый номер (сигнатуры или аргумента)
        if (cnt > field.precision)
        {
          os << static_cast<uint16_t>(cnt);
        }
        else if (field.width >= 0)
        {
          os << "";
        }
        break;
      case tfpPercent:
        os << '%';
        break;
      default:
        {
          string tmp(2, '?');
          tmp.insert(1, errorHeaderFormat.substr(field.before, field.after - field.before));
          os << tmp;
        }
        break;
    }
    begin = field.after;
  }
  if (errorHeaderFormat.length())
  {
    os << errorHeaderFormat.substr(begin);
    if (*errorHeaderFormat.rbegin() != '\n')
    {
      os
        << endl
      ;
    }
  }

  const bool forceNewLine = errorBodyFormat.length() && (*errorBodyFormat.rbegin() != '\n');
  for (TupleBase::strings_t::const_iterator iter_err = tuple.errors().begin(); iter_err != tuple.errors().end(); iter_err ++)
  {
    begin = 0;

    for (TupleFormat::fields_t::const_iterator iter_fld = bodyFields.begin(); iter_fld != bodyFields.end(); iter_fld ++)
    {
      const TupleFormat::field_t &field = *iter_fld;
      os << errorBodyFormat.substr(begin, field.before - begin);

      if (field.width >= 0)
      {
        os << setw(field.width);
      }
      os << ((field.flags & tffLeft) ? left : right);
      switch (field.specifier)
      {
        case tfpError:
          os << *iter_err;
          break;
        case tfpSerialNumber:
          if ((iter_err - tuple.errors().begin()) > field.precision)
          {
            os << static_cast<uint16_t>(iter_err - tuple.errors().begin());
          }
          else if (field.width >= 0)
          {
            os << "";
          }
          break;
        case tfpPercent:
          os << '%';
          break;
        default:
          {
            string tmp(2, '?');
            tmp.insert(1, errorBodyFormat.substr(field.before, field.after - field.before));
            os << tmp;
          }
          break;
      }
      begin = field.after;
    }
    os << errorBodyFormat.substr(begin);
    if (forceNewLine)
    {
      os
        << endl
      ;
    }
  }
}

void TupleDescriberFlexible::m_description_formatted_header
(
    std::ostream &ostr
  , TupleBase::strings_t::const_iterator &iter_descs
  , const TupleBase &tuple
  , const TupleFormat &format
  , const tupleDirection dir
  , const describeOptions opt
  , const uint8_t cnt
) const
{
  const TupleFormat::fields_t &headerFields = format.tupleHeaderFields();
  const string &headerFormat = format.tupleHeaderFormat();

  size_t begin = 0;

  for (TupleFormat::fields_t::const_iterator iter = headerFields.begin(); iter != headerFields.end(); iter ++)
  {
    const TupleFormat::field_t &field = *iter;
    ostr << headerFormat.substr(begin, field.before - begin);
    if (field.width >= 0)
    {
      ostr << setw(field.width);
    }
    ostr << ((field.flags & tffLeft) ? left : right);
    switch (field.specifier)
    {
      case tfpDirection: //d, направление (аргумент/значение)
        ostr << (dir == dirIn ? "->" : "<-");
        break;
      case tfpSerialNumber: //n[*] порядковый номер (сигнатуры или аргумента)
        if (cnt > field.precision)
        {
          ostr << static_cast<uint16_t>(cnt);
        }
        else if (field.width >= 0)
        {
          ostr << "";
        }
        break;
      case tfpSignature: //S, сигнатура C++
        ostr << m_signature_cpp(tuple, dir);
        break;
      case tfpPythonFormatString: //F, форматная строка Python
        ostr << tuple.format_no_throw();
        break;
      case tfpPythonSignature: //P, сигнатура Python
        ostr << m_signature_python(tuple, dir);
        break;
      case tfpDescription: //D[*] описание
        if (opt == optPerLineWithHeader)
        {
          ostr << *iter_descs;
        }
        else if (opt == optRaw)
        {
          string tmp;
          while (iter_descs != tuple.description.end())
          {
            if (iter_descs != tuple.description.begin())
            {
              tmp += '\n';
            }
            tmp += *iter_descs;
            iter_descs ++;
          }
          ostr << tmp;
        }
        else
        {
          ostr << "";
        }
        break;
      case tfpPercent:
        ostr << '%';
        break;
      default:
        {
          string tmp(2, '?');
          tmp.insert(1, headerFormat.substr(field.before, field.after - field.before));
          ostr << tmp;
        }
        break;
    }
    begin = field.after;
  }
  if (headerFormat.length())
  {
    ostr << headerFormat.substr(begin);
    if (*headerFormat.rbegin() != '\n')
    {
      ostr
        << endl
      ;
    }
  }
  if (opt == optPerLineWithHeader)
  {
    iter_descs ++;
  }
}

void TupleDescriberFlexible::m_description_formatted_body
(
    std::ostream &ostr
  , TupleBase::strings_t::const_iterator &iter_descs
  , const TupleBase &tuple
  , const TupleFormat &format
  , const tupleDirection dir
  , const describeOptions opt
) const
{
  const string &bodyFormat = format.tupleBodyFormat();
  const bool forceNewLine = bodyFormat.length() && (*bodyFormat.rbegin() != '\n');
  const TupleFormat::fields_t &bodyFields = format.tupleBodyFields();

  for (TupleBase::idx_t::const_iterator iter_idx = tuple.index().begin(); iter_idx != tuple.index().end(); iter_idx ++)
  {
    const TupleBase::idx &idx = *iter_idx;
    size_t begin = 0;

    for (TupleFormat::fields_t::const_iterator iter_fld = bodyFields.begin(); iter_fld != bodyFields.end(); iter_fld ++)
    {
      const TupleFormat::field_t &field = *iter_fld;
      ostr << bodyFormat.substr(begin, field.before - begin);

      if (field.width >= 0)
      {
        ostr << setw(field.width);
      }
      ostr << ((field.flags & tffLeft) ? left : right);
      switch (field.specifier)
      {
        case tfpSerialNumber: //n[*], порядковый номер (сигнатуры или аргумента)
          if ((iter_idx - tuple.index().begin()) > field.precision)
          {
            ostr << static_cast<uint16_t>(iter_idx - tuple.index().begin());
          }
          else if (field.width >= 0)
          {
            ostr << "";
          }
          break;
        case tfpSignature: //[d]S*, тип С++
          {
            string tmp;
            if (field.modifier == tfmSignificant)
            {
              tmp.append(tuple.args().at(idx.significant).type_name_cpp);
            }
            else
            {
              for (uint8_t pos = idx.begin; pos != idx.end; pos ++)
              {
                if (pos != idx.begin)
                {
                  tmp.append(", ");
                }
                tmp.append(tuple.args().at(pos).type_name_cpp);
              }
            }
            ostr << tmp;
          }
          break;
        case tfpPythonFormatString: //[d]F*, Python format unit
          {
            string tmp;
            if (field.modifier == tfmSignificant)
            {
              const TupleBase::arg &arg = tuple.args().at(idx.significant);
              tmp += arg.format_unit;
              if (arg.format_unit2)
              {
                tmp += arg.format_unit2;
              }
            }
            else
            {
              for (uint8_t pos = idx.begin; pos != idx.end; pos ++)
              {
                const TupleBase::arg &arg = tuple.args().at(pos);
                tmp += arg.format_unit;
                if (arg.format_unit2)
                {
                  tmp += arg.format_unit2;
                }
              }
            }
            ostr << tmp;
          }
          break;
        case tfpPythonSignature: //P*, Python variable type
          ostr << tuple.args().at(idx.significant).type_name_python;
          break;
        case tfpDescription: //D[*], описание
          if (opt != optRaw)
          {
            ostr << *iter_descs;
          }
          else if (field.width >= 0)
          {
            ostr << "";
          }
          break;
        case tfpName: //[d]N*, имя (только TupleIn)
          {
            string tmp;
            if (dir == dirIn)
            {
              const ArraySquare &keywords = tuple.keywords();
              if (keywords.size())
              {
                tmp = keywords.get()[iter_idx - tuple.index().begin()];
              }
            }
            if (tmp.length() || field.width >= 0)
            {
              ostr << tmp;
            }
          }
          break;
        case tfpValue: //[d]V*, значение (только TupleIn и TupleBase::idx::optional = true)
          if (dir == dirIn)
          {
            string tmp;
            if (idx.optional)
            {
              if (field.modifier == tfmSignificant)
              {
                tmp.append(tuple.value(idx.significant));
              }
              else
              {
                for (uint8_t pos = idx.begin; pos != idx.end; pos ++)
                {
                  if (pos != idx.begin)
                  {
                    tmp += ", ";
                  }
                  tmp.append(tuple.value(pos));
                }
              }
            }
            else if (tuple.args()[idx.begin].format_unit == 'e')
            {
              tmp.append(tuple.value(idx.begin));
            }
            ostr << tmp;
          }
          else if (field.width >= 0)
          {
            ostr << "";
          }
          break;
        case tfpAddress: //[d]A*, адрес
          {
            string tmp;
            if (field.modifier == tfmSignificant)
            {
              tmp.append(tuple.address(idx.significant));
            }
            else
            {
              for (uint8_t pos = idx.begin; pos != idx.end; pos ++)
              {
                if (pos != idx.begin)
                {
                  tmp += ' ';
                }
                tmp.append(tuple.address(pos));
              }
            }
            ostr << tmp;
          }
          break;
        case tfpPercent:
          ostr << '%';
          break;
        default:
          {
            string tmp(2, '?');
            tmp.insert(1, bodyFormat.substr(field.before, field.after - field.before));
            ostr << tmp;
          }
          break;
      }
      begin = field.after;
    }
    ostr << bodyFormat.substr(begin);
    if (forceNewLine)
    {
      ostr
        << endl
      ;
    }
    if (opt != optRaw)
    {
      iter_descs ++;
    }
  }
}

const string TupleDescriberFlexible::m_signature_cpp(const TupleBase &tuple, const tupleDirection dir) const
{
  ostringstream ostr;
  TupleBase::args_t::const_iterator iter_arg = tuple.args().begin();
  TupleBase::idx_t::const_iterator iter_idx = tuple.index().begin();
  size_t pos = 0;

  while (iter_arg != tuple.args().end())
  {
    if (iter_arg != tuple.args().begin())
    {
      ostr << ", ";
    }
    const TupleBase::arg &arg = *iter_arg;
    ostr << arg.type_name_cpp;
    if (dir == dirIn)
    {
      const TupleBase::idx &idx = *iter_idx;
      if (idx.optional && arg.format_unit != '!')
      {
        ostr << " = " << tuple.value(pos);
      }
    }

    iter_arg ++;
    if (++ pos == iter_idx->end)
    {
      iter_idx ++;
    }
  }
  return ostr.str();
}

const string TupleDescriberFlexible::m_signature_python(const TupleBase &tuple, const tupleDirection dir) const
{
  ostringstream ostr;
  TupleBase::idx_t::const_iterator iter_idx = tuple.index().begin();
  size_t pos = 0;

  if (tuple.opts & optTuple)
  {
    ostr << '(';
  }
  else if (dir == dirOut)
  {
    if (tuple.opts & optList)
    {
      ostr << '[';
    }
    else if (tuple.opts & optDictionary)
    {
      ostr << '{';
    }
  }

  while (iter_idx != tuple.index().end())
  {
    const TupleBase::idx &idx = *iter_idx;
    const TupleBase::arg &arg = tuple.args().at(idx.significant);
    if (iter_idx != tuple.index().begin())
    {
      ostr << ", ";
    }
    ostr << arg.type_name_python;
    if (dir == dirIn && idx.optional)
    {
      ostr << " = " << tuple.value(idx.significant, TupleBase::vsPython);
    }

    iter_idx ++;
  }

  if (tuple.opts & optTuple)
  {
    ostr << ')';
  }
  else if (dir == dirOut)
  {
    if (tuple.opts & optList)
    {
      ostr << ']';
    }
    else if (tuple.opts & optDictionary)
    {
      ostr << '}';
    }
  }
  return ostr.str();
}

const char *TupleDescriberFlexible::to_string() const
{
  return SelfAddressedCstringBase<TupleDescriberFlexible>::get();
}

void TupleDescriberFlexible::help(const char *help)
{
  m_append(help ? help : "");
}

void TupleDescriberFlexible::append(const char *s)
{
  m_append(s ? s : "");
}

void TupleDescriberFlexible::describe(const Tuples &tuples)
{
  /* NOTE
  This class designed to work in pair with TuplesDefaultImpl implementation of Tuples
  No alternative at runtime, therefore it is compile time responsiblility
  Otherwise std::bad_cast exception guaranteed here
  */
  m_describe(dynamic_cast<const TuplesDefaultImpl &>(tuples), Container::container().tupleFormat());
}

void TupleDescriberFlexible::describe(const TuplesDefaultImpl &tuples)
{
  m_describe(tuples, Container::container().tupleFormat());
}

#ifdef _DEBUG_PYTHON_PYTUPLE
void TupleDescriberFlexible::__describe(const TuplesDefaultImpl &tuples, const TupleFormat &format)
{
  m_describe(tuples, format);
}
#endif //_DEBUG_PYTHON_PYTUPLE

TupleDescriberFlexible::TupleDescriberFlexible()
  : m_help_length(0)
{
  PYHROL_TRACE(tpInternal, this, __PRETTY_FUNCTION__);
}

TupleDescriberFlexible::~TupleDescriberFlexible()
{
  PYHROL_TRACE(tpInternal, this, __PRETTY_FUNCTION__);
}

TupleDescriber *TupleDescriber::from_string(const char *doc)
{
  return TupleDescriberFlexible::construct(doc);
}

TupleDescriber *TupleDescriber::factory()
{
  return new TupleDescriberFlexible;
}

} //namespace pyhrol
