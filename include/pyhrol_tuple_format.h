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

#ifndef __pyhrol_tuple_format_h__
#define __pyhrol_tuple_format_h__

#include <vector>

#include "printf_parser.h"
#include "smart_flag.h"

namespace pyhrol
{
/* TODO
     d      направление (аргумент/значение)
     n      порядковый номер (сигнатуры, аргумента или ошибки)
  [a]S      сигнатура/тип C++
  [a]F      форматная строка Python
     D      описание
     N      имя (ключевое слово, только TupleIn)
  [a]V      значение (только TupleIn и TupleBase::idx::optional = true)
  [a]A      адрес
     E      описание ошибки
Head Table
   +       d
   +     + n
   +     + S
   +     + F
   +     + D
         + N
         + V
         + A
В случае ошибки:
   +       d
   +     + n
   +     + E
Модификаторы (для множественных аргументов вида [suz]#, e[st][#]?, O!, O&):
    показывать только значащее значение из пары (если не указано a)
  a показывать все значения
Способы представления:
  ошибки (все параметры игнорируются)
  строка "{[%dnSFD]}"
  таблица "{%*.*[nSFDNVA]}"
*/

enum tupleFormatFlags
{
    tffNo
  , tffLeft //adjust left
};

enum tupleFormatModifiers
{
    tfmSignificant
  , tfmAll
};

enum tupleFormatParts
{
    tfpDirection
  , tfpSerialNumber
  , tfpSignature
  , tfpPythonFormatString
  , tfpDescription
  , tfpName
  , tfpValue
  , tfpAddress
  , tfpError
  , tfpPercent = 0xff
};

class TupleFormat: private PrintfParser<tupleFormatFlags, tupleFormatModifiers, tupleFormatParts>
{
public:
  typedef PrintfParser<tupleFormatFlags, tupleFormatModifiers, tupleFormatParts>::field field_t;
  typedef std::vector<field_t> fields_t;

  static const std::string
      defaultTupleHeaderFormat
    , defaultTupleBodyFormat
    , defaultErrorHeaderFormat
    , defaultErrorBodyFormat
  ;

  struct initer
  {
    std::string
        tupleHeaderFormat
      , tupleBodyFormat
      , errorHeaderFormat
      , errorBodyFormat
    ;
    initer();
  };

  const fields_t &tupleHeaderFields() const;
  const fields_t &tupleBodyFields() const;
  const fields_t &errorHeaderFields() const;
  const fields_t &errorBodyFields() const;

  const std::string &tupleHeaderFormat() const;
  const std::string &tupleBodyFormat() const;
  const std::string &errorHeaderFormat() const;
  const std::string &errorBodyFormat() const;

  void setTupleHeaderFormat(const char *th);
  void setTupleBodyFormat(const char *tb);
  void setErrorHeaderFormat(const char *eh);
  void setErrorBodyFormat(const char *eb);

  TupleFormat(const initer &);
  TupleFormat(const std::string &/*tupleHeader*/, const std::string &/*tupleBody*/, const std::string &/*errorHeader*/, const std::string &/*errorBody*/);

private:
  fields_t
      m_tuple_header
    , m_tuple_body
    , m_error_header
    , m_error_body
  ;
  std::string
      m_tuple_header_format
    , m_tuple_body_format
    , m_error_header_format
    , m_error_body_format
  ;

  void m_init();
};

} //pyhrol

typedef SmartFlag<pyhrol::tupleFormatFlags> tuple_format_flags_t;
typedef SmartFlag<pyhrol::tupleFormatModifiers> tuple_format_modifiers_flag_t;
typedef SmartFlag<pyhrol::tupleFormatParts> tuple_format_parts_flag_t;

#endif //__pyhrol_tuple_format_h__
