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

#include <iostream>
#include <pyhrol.h>

using namespace std;
using namespace pyhrol;

void function_with_overloaded_args(Tuples &_args)
{
  int i;
  const char *pch;
  double f;

  PYHROL_PARSE_TUPLE_1(NULL, _args, i)
  PYHROL_PARSE_TUPLE_1(NULL, _args, pch)
  PYHROL_PARSE_TUPLE_1(NULL, _args, f)
  PYHROL_PARSE_TUPLE_0(NULL, _args)
  PYHROL_PARSE_TUPLE_3(NULL, _args, i, pch, f)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  cout
    << __func__ << ": I am called" << endl
  ;

  switch (_args.parsed_variant())
  {
    case 0:
      cout
        << "  Signature 0: i = " << i << endl
      ;
      break;
    case 1:
      cout
        << "  Signature 1: pch = \"" << pch << "\"" << endl
      ;
      break;
    case 2:
      cout
        << "  Signature 2: f = " << f << endl
      ;
      break;
    case 3:
      cout
        << "  Signature 3: <no args>" << endl
      ;
      break;
    case 4:
      cout
        << "  Signature 4: i = " << i << ", pch = \"" << pch << "\", f = " << f << endl
      ;
      break;
  }

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

static void __on_load() __attribute__ ((constructor));

void __on_load()
{
  PYHROL_REGISTER_FUNCTION(function_with_overloaded_args, "Function with some different argument signatures")
}
