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

// $Date: 2014-02-13 01:38:33 +0400 (Thu, 13 Feb 2014) $
// $Revision: 888 $

#ifndef __pyhrol_tuple_base_safe_h__
#define __pyhrol_tuple_base_safe_h__

#include <vector>

#include "array_square.h"
#include "pyhrol_common.h"
#include "strings_manip.h"

namespace pyhrol
{

class TupleBaseSafe
{
  public:
    class FormatException: public AttributeException
    {
    public:
      FormatException();
    };

    struct arg
    {
      char
          format_unit
        , format_unit2
      ;
    };
    typedef std::vector<std::string> strings_t;
    typedef std::vector<arg> args_t;

    const options opts;
    const int8_t required;

    TupleBaseSafe(const options /*_opts*/, const int8_t _required = requiredAll);
    TupleBaseSafe(const TupleBaseSafe &);
    ~TupleBaseSafe();

    bool valid() const;
    const std::string &format() const;
    const ArraySquare &keywords() const;
    bool empty() const;

    void build_format(const bool with_keywords);

  protected:
    void m_add_format_unit
    (
        const char format_unit
      , const char *arg_name
      , const char format_unit2 = '\0'
    );

  private:
    bool
        m_format_actual
      , m_valid
      , m_last_swapped
    ;
    std::string m_format;
    strings_t m_arg_names;
    ArraySquare m_keywords;
};

}; //namespace pyhrol

#endif //__pyhrol_tuple_base_safe_h__
