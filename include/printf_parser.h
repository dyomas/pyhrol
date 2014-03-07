/*
 *   Copyright (c) 2013 Pyhrol, pyhrol@rambler.ru
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

// $Date: 2014-02-23 20:22:57 +0400 (Sun, 23 Feb 2014) $
// $Revision: 895 $

#ifndef __printf_parser_h__
#define __printf_parser_h__


#include <stdint.h>
#include <map>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "smart_flag.h"


enum fieldPartTypes
{
    fptNo = 0x0
  , fptFlag = 0x01
  , fptModifierPre = 0x10
  , fptModifier = 0x02
  , fptSpecifier = 0x03
};

typedef SmartFlag<fieldPartTypes> field_part_types_flag_t;

/**
%[<flag>][<width>][.<precision>][<modifier>]<specifier>
%[\w]*([\d]*|[\d]*\.[\d]*)[\w]*[\w]
*/
template <typename F = char, typename M = char, typename S = char> class PrintfParser
{
/*
NOTE
  -- среди спецификаторов формата F должно быть значение '%' для того, чтобы символы корректно экранировались -- пары %% представляются отдельным полем PrintfParser::field, у которого ни один из атрибутов не заполнен
*/
  typedef std::map<char, const F> flags_t;
  typedef std::map<std::string, const M> modifiers_t;
  typedef std::map<char, const S> specifiers_t;
  typedef std::map<char, fieldPartTypes> index_t;

  flags_t m_flags;
  modifiers_t m_modifiers;
  specifiers_t m_specifiers;
  index_t m_index;

  void m_add_index_item(const char /*idx*/, fieldPartTypes /*type*/);
#ifdef _DEBUG
  void __m_test(const fieldPartTypes /*exists*/, const fieldPartTypes /*adding*/);
#endif //_DEBUG
  public:
    struct field
    {
      F flags;
      M modifier;
      S specifier;
      int8_t
          width
        , precision
      ;
      size_t
          before
        , after
      ;
    };
  #ifdef _DEBUG
    void __test();
    void __report(std::ostream &/*os*/) const;
  #endif //_DEBUG

    const size_t parse(field */*pfields*/, const size_t /*fields_cnt*/, const char */*format*/) const;

    void addFlag(const char /*flag*/, const F /*value*/);
    void addModifier(const char */*modifier*/, const M /*value*/);
    void addSpecifier(const char /*specifier*/, const S /*value*/);
};
#endif //__printf_parser_h__

