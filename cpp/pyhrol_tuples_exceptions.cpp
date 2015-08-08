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

#include <iostream>
#include <sstream>

#include "pyhrol_tuples_exceptions.h"

using namespace std;

namespace pyhrol
{

const string ParseException::m_describe(const string &msg, const size_t cnt)
{
  ostringstream ostr;
  ostr << msg;

  if (cnt > 1)
  {
    ostr
      << endl << "No one from " << static_cast<uint16_t>(cnt) << " signatures parsed";
  }
  return ostr.str();
}

ParseException::ParseException(const string &msg, const size_t cnt)
  : runtime_error(m_describe(msg, cnt))
{
}

ParseException::ParseException()
  : runtime_error("No arguments expected")
{
}

#ifndef PYHROL_SAFE_MODE
ArgsException::ArgsException()
  : AttributeException("Function registered as having no arguments, but arguments parsed")
{
}

OverflowException::OverflowException(const uint8_t wanted, const size_t exists, const directions dir)
  : overflow_error(m_describe(wanted, exists, dir))
{
}

const string OverflowException::m_describe(const uint8_t wanted, const size_t exists, const directions dir)
{
  ostringstream ostr;
  ostr << "There is no tuple with index " << size_t(wanted) << " within the array of " << exists << " " << (dir == dInput ? "input" : "output") << " tuples";
  return ostr.str();
}


StateException::StateException(const string &msg)
  : AttributeException(msg)
{
}
#endif //PYHROL_SAFE_MODE

const string UseException::m_describe(const uint8_t variant, const uint8_t limit)
{
  ostringstream ostr;
  ostr << "Build variant " << static_cast<uint16_t>(variant) << " not in range [0, " << static_cast<uint16_t>(limit) << (limit ? ")" : "]");
  return ostr.str();
}

UseException::UseException(const uint8_t variant, const uint8_t limit)
  : domain_error(m_describe(variant, limit))
{
}

UseException::UseException()
  : domain_error("No variants parsed")
{
}

} //namespace pyhrol
