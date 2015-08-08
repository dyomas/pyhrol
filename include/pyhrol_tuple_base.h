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

// $Date: 2015-07-20 23:24:41 +0300 (Пн., 20 июля 2015) $
// $Revision: 1047 $

#ifndef __pyhrol_tuple_h__
#define __pyhrol_tuple_h__

#include <vector>

#include "array_square.h"
#include "array_void.h"
#include "pyhrol_common.h"
#include "strings_manip.h"

namespace pyhrol
{

class TupleBase
{
  public:
    struct arg
    {
      char
          format_unit
        , format_unit2
      ;
      std::string
          type_name_cpp
        , type_name_python
      ;
    };
    struct idx
    {
      uint8_t
          begin
        , end
        , significant
      ;
      bool optional;
    };
    typedef std::vector<std::string> strings_t;
    typedef std::vector<arg> args_t;
    typedef std::vector<idx> idx_t;
    typedef std::vector<const void *> addresses_t;

    class FormatException: public AttributeException
    {
      static const std::string m_describe(const strings_t &);
    public:
      FormatException(const strings_t &);
    };

    const strings_t description;
    const options opts;
    const int8_t required;

    TupleBase(const char */*_description*/, const options /*_opts*/, const int8_t _required = requiredAll);
    TupleBase(const TupleBase &);
    ~TupleBase();

    const std::string &format() const;
    const std::string format_no_throw() const;
    const args_t &args() const;
    const idx_t &index() const;
    const ArraySquare &keywords() const;
    const ArrayVoid &values() const;
    const addresses_t &addresses() const;
    const strings_t &errors() const;

    enum value_styles
    {
        vsCpp
      , vsPython
    };

    void value(std::ostream &, const size_t pos, const value_styles = vsCpp) const;
    const std::string value(const size_t pos, const value_styles = vsCpp) const;
    void address(std::ostream &, const size_t pos) const;
    const std::string address(const size_t pos) const;

    void clear_values();
    void build_format(const bool with_keywords);

  protected:
    bool m_arg_is_optional() const;
    void m_add_format_unit
    (
        const char format_unit
      , const char *type_name_cpp
      , const char *type_name_python
      , const char *arg_name
      , const void *value
      , const size_t value_length
      , const char format_unit2 = '\0'
    );

  private:
    void m_add_error(const std::string &/*err_descr*/);
    void m_swap_type_names(const size_t /*pos*/);
    static const strings_t m_parse_strings(const char *);

    bool m_format_actual;
    std::string m_format;
    args_t m_args;
    idx_t m_idx;
    strings_t m_arg_names;
    ArraySquare m_keywords;
    ArrayVoid m_values;
    addresses_t m_addresses;
    strings_t m_errors;
};

} //namespace pyhrol

#endif //__pyhrol_tuple_h__
