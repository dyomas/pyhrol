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

// $Date: 2014-04-30 17:24:23 +0400 (Wed, 30 Apr 2014) $
// $Revision: 914 $

#include <string.h>

#include "pyhrol_tuple_format.h"
#include "printf_parser.hpp"
#include "smart_flag.hpp"

extern char **environ;

namespace pyhrol
{

using namespace std;

const string
    TupleFormat::defaultTupleHeaderFormat = "%-3d%2.2n %F %D\n"
  , TupleFormat::defaultTupleBodyFormat = "      %-30aS%-4aF%-20N%-15aV%D\n"
  , TupleFormat::defaultErrorHeaderFormat = "%-3d%2.2n *** Errors:\n"
  , TupleFormat::defaultErrorBodyFormat = "      %E\n"
;

TupleFormat::initer::initer()
  : tupleHeaderFormat(defaultTupleHeaderFormat)
  , tupleBodyFormat(defaultTupleBodyFormat)
  , errorHeaderFormat(defaultErrorHeaderFormat)
  , errorBodyFormat(defaultErrorBodyFormat)
{
  char const * const * environ_cur = ::environ;
  const char *name_end;
  uint8_t cnt = 0;

  while (*environ_cur)
  {
    name_end = *environ_cur;
    do
    {
      if (*name_end == '=')
      {
        if (name_end - *environ_cur == 26 && !memcmp(*environ_cur, "PYHROL_TUPLE_HEADER_FORMAT", 26))
        {
          tupleHeaderFormat.assign(name_end + 1);
          cnt ++;
        }
        else if (name_end - *environ_cur == 24 && !memcmp(*environ_cur, "PYHROL_TUPLE_BODY_FORMAT", 24))
        {
          tupleBodyFormat.assign(name_end + 1);
          cnt ++;
        }
        else if (name_end - *environ_cur == 26 && !memcmp(*environ_cur, "PYHROL_ERROR_HEADER_FORMAT", 26))
        {
          errorHeaderFormat.assign(name_end + 1);
          cnt ++;
        }
        else if (name_end - *environ_cur == 24 && !memcmp(*environ_cur, "PYHROL_ERROR_BODY_FORMAT", 24))
        {
          errorBodyFormat.assign(name_end + 1);
          cnt ++;
        }
        break;
      }
    }
    while (*name_end ++);

    if (cnt == 4)
    {
      break;
    }

    environ_cur ++;
  }
}

const TupleFormat::fields_t &TupleFormat::tupleHeaderFields() const
{
  return m_tuple_header;
}

const TupleFormat::fields_t &TupleFormat::tupleBodyFields() const
{
  return m_tuple_body;
}

const TupleFormat::fields_t &TupleFormat::errorHeaderFields() const
{
  return m_error_header;
}

const TupleFormat::fields_t &TupleFormat::errorBodyFields() const
{
  return m_error_body;
}

const string &TupleFormat::tupleHeaderFormat() const
{
  return m_tuple_header_format;
}

const string &TupleFormat::tupleBodyFormat() const
{
  return m_tuple_body_format;
}

const string &TupleFormat::errorHeaderFormat() const
{
  return m_error_header_format;
}

const string &TupleFormat::errorBodyFormat() const
{
  return m_error_body_format;
}

void TupleFormat::setTupleHeaderFormat(const char *th)
{
  const size_t fields_cnt = parse(NULL, 0, th);
  fields_t fields(fields_cnt);
  parse(&fields[0], fields.capacity(), th);
  m_tuple_header = fields;
  m_tuple_header_format = th;
}

void TupleFormat::setTupleBodyFormat(const char *tb)
{
  const size_t fields_cnt = parse(NULL, 0, tb);
  fields_t fields(fields_cnt);
  parse(&fields[0], fields.capacity(), tb);
  m_tuple_body = fields;
  m_tuple_body_format = tb;
}

void TupleFormat::setErrorHeaderFormat(const char *eh)
{
  const size_t fields_cnt = parse(NULL, 0, eh);
  fields_t fields(fields_cnt);
  parse(&fields[0], fields.capacity(), eh);
  m_error_header = fields;
  m_error_header_format = eh;
}

void TupleFormat::setErrorBodyFormat(const char *eb)
{
  const size_t fields_cnt = parse(NULL, 0, eb);
  fields_t fields(fields_cnt);
  parse(&fields[0], fields.capacity(), eb);
  m_error_body = fields;
  m_error_body_format = eb;
}

TupleFormat::TupleFormat(const initer &i)
  : m_tuple_header_format(i.tupleHeaderFormat)
  , m_tuple_body_format(i.tupleBodyFormat)
  , m_error_header_format(i.errorHeaderFormat)
  , m_error_body_format(i.errorBodyFormat)
{
  m_init();
}

TupleFormat::TupleFormat(const string &th, const string &tb, const string &eh, const string &eb)
  : m_tuple_header_format(th)
  , m_tuple_body_format(tb)
  , m_error_header_format(eh)
  , m_error_body_format(eb)
{
  m_init();
}

void TupleFormat::m_init()
{
  addFlag('-', tffLeft);
  addModifier("a", tfmAll);
  addSpecifier('d', tfpDirection);
  addSpecifier('n', tfpSerialNumber);
  addSpecifier('S', tfpSignature);
  addSpecifier('F', tfpPythonFormatString);
  addSpecifier('D', tfpDescription);
  addSpecifier('N', tfpName);
  addSpecifier('V', tfpValue);
  addSpecifier('A', tfpAddress);
  addSpecifier('E', tfpError);
  addSpecifier('%', tfpPercent);

  fields_t ret_val;
  size_t fields_cnt;

  fields_cnt = parse(NULL, 0, m_tuple_header_format.c_str());
  ret_val.resize(fields_cnt);
  parse(&ret_val[0], ret_val.capacity(), m_tuple_header_format.c_str());
  m_tuple_header = ret_val;

  fields_cnt = parse(NULL, 0, m_tuple_body_format.c_str());
  ret_val.resize(fields_cnt);
  parse(&ret_val[0], ret_val.capacity(), m_tuple_body_format.c_str());
  m_tuple_body = ret_val;

  fields_cnt = parse(NULL, 0, m_error_header_format.c_str());
  ret_val.resize(fields_cnt);
  parse(&ret_val[0], ret_val.capacity(), m_error_header_format.c_str());
  m_error_header = ret_val;

  fields_cnt = parse(NULL, 0, m_error_body_format.c_str());
  ret_val.resize(fields_cnt);
  parse(&ret_val[0], ret_val.capacity(), m_error_body_format.c_str());
  m_error_body = ret_val;
}

} //pyhrol


using namespace pyhrol;

SMART_FLAG_INSTANTIATION_BEGIN(tupleFormatFlags, t)
{
  t.add(npOr, tffNo, "");
  t.add(npOr, tffLeft, "Left", tffNo);
}
SMART_FLAG_INSTANTIATION_END(tupleFormatFlags, t)

SMART_FLAG_INSTANTIATION_BEGIN(tupleFormatModifiers, t)
{
  t.add(npOr, tfmSignificant, "significant");
  t.add(npOr, tfmAll, "all", tfmSignificant);
}
SMART_FLAG_INSTANTIATION_END(tupleFormatModifiers, t)

SMART_FLAG_INSTANTIATION_BEGIN(tupleFormatParts, t)
{
  t.add(npOr, tfpDirection, "Direction");
  t.add(npOr, tfpSerialNumber, "SerialNumber", tfpDirection);
  t.add(npOr, tfpSignature, "Signature", tfpSerialNumber);
  t.add(npOr, tfpPythonFormatString, "PythonFormatString", tfpSignature);
  t.add(npOr, tfpDescription, "Description", tfpPythonFormatString);
  t.add(npOr, tfpName, "Name", tfpDescription);
  t.add(npOr, tfpValue, "Value", tfpName);
  t.add(npOr, tfpAddress, "Address", tfpValue);
  t.add(npOr, tfpError, "Error", tfpAddress);
  t.add(npOr, tfpPercent, "Percent", tfpError);
}
SMART_FLAG_INSTANTIATION_END(tupleFormatParts, t)

