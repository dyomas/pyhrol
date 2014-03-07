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

// $Date: 2013-05-21 13:23:01 +0400 (Tue, 21 May 2013) $
// $Revision: 718 $

#ifndef __printf_fields_h__
#define __printf_fields_h__


#include <vector>

#include "printf_parser.h"

template <typename F = char, typename M = char, typename S = char> class PrintfFields
{
  public:
    typedef PrintfParser<F, M, S> parser_t;
    typedef typename PrintfParser<F, M, S>::field field_t;
    typedef std::vector<field_t> fields_t;
  private:
    std::string m_fmt;
    fields_t m_fields;

    void m_init(const parser_t &/*parser*/);
  public:
    const field_t *operator [](const size_t pos) const;
    const size_t size() const;
    const fields_t &fields() const;
    const field_t *fields(size_t &) const;
    const char *format() const;

    void setup(const parser_t &/*parser*/, const char */*_format*/);

    PrintfFields();
    PrintfFields(const parser_t &/*parser*/, const char */*_format*/);
};

#endif //__printf_fields_h__

