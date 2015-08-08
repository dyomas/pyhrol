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

// $Date: 2014-04-04 16:35:38 +0400 (Пт., 04 апр. 2014) $
// $Revision: 906 $

#ifndef __pyhrol_tuples_exceptions_h__
#define __pyhrol_tuples_exceptions_h__

#include <stdint.h>
#include <stdexcept>

#include "pyhrol_common.h"

namespace pyhrol
{

class ParseException: public std::runtime_error
{
  static const std::string m_describe(const std::string &, const size_t cnt);
public:
  ParseException(const std::string &, const size_t cnt);
  ParseException();
};


#ifndef PYHROL_SAFE_MODE
class ArgsException: public AttributeException
{
public:
  ArgsException();
};

class OverflowException: public std::overflow_error
{
public:
  enum directions
  {
      dInput
    , dOutput
  };
  OverflowException(const uint8_t, const size_t, const directions);

private:
  static const std::string m_describe(const uint8_t, const size_t, const directions);
};


class StateException: public AttributeException
{
public:
  StateException(const std::string &);
};
#endif //PYHROL_SAFE_MODE


class UseException: public std::domain_error
{
  static const std::string m_describe(const uint8_t, const uint8_t);
public:
  UseException(const uint8_t, const uint8_t);
  UseException();
};

} //namespace pyhrol

#endif //__pyhrol_tuples_exceptions_h__
