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
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

void function_manipulate_strings_simple(pyhrol::Tuples &_args)
{
  const char *str;
  int length;
  size_t size;
  Py_UNICODE *punicode;

  PYHROL_PARSE_TUPLE_2(NULL, _args, str, length)
  PYHROL_PARSE_TUPLE_2(NULL, _args, str, size)
  PYHROL_PARSE_TUPLE_1(NULL, _args, punicode)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  switch (_args.parsed_variant())
  {
    case 0:
      {
        std::cout << "Signature 0: string `" << str << "\', " << length << " bytes length\n";
      }
      break;
    case 1:
      {
        std::cout << "Signature 1: string `" << str << "\' and integer " << size << '\n';
      }
      break;
    case 2:
      {
        const size_t len = wcslen(reinterpret_cast<wchar_t *>(punicode));
        std::string tmp(len * 6, '\0');
        tmp.resize(wcstombs(&tmp[0], reinterpret_cast<wchar_t *>(punicode), len * 6));
        std::cout << "Signature 2: Py_UNICODE `" << tmp << "\', " << len << " wide chars length\n";
      }
      break;
  }

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void function_manipulate_string_objects(pyhrol::Tuples &_args)
{
  Py_buffer buff;
  PyUnicodeObject *punicode_obj;

  PYHROL_PARSE_TUPLE_1(NULL, _args, buff)
  PYHROL_PARSE_TUPLE_1(NULL, _args, punicode_obj)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  switch (_args.parsed_variant())
  {
    case 0:
      {
        std::cout << "Signature 0: buffer `" << std::string(reinterpret_cast<const char *>(buff.buf), buff.len) << "\', " << buff.len << " bytes length\n";
      }
      break;
    case 1:
      {
        std::string tmp(punicode_obj->length * 6, '\0');
        tmp.resize(wcstombs(&tmp[0], reinterpret_cast<wchar_t *>(punicode_obj->str), punicode_obj->length * 6));
        std::cout << "Signature 1: PyUnicodeObject `" << tmp << "\', " << punicode_obj->length << " wide chars length\n";
      }
      break;
  }

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void function_string_codepage_conversion(pyhrol::Tuples &_args)
{
  const char *codepage = "cp1251";
  const int buffer_capacity = 32;
  int32_t buffer_length = buffer_capacity;
  char buffer[buffer_capacity], *pbuffer = buffer;

  PYHROL_PARSE_TUPLE_3(NULL, _args, *codepage, pbuffer, buffer_length)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  std::cout << "String encoded to `" << codepage << "\': [";
  for (size_t pos = 0; pos != buffer_length; pos ++)
  {
    std::cout << std::hex << static_cast<uint16_t>(static_cast<uint8_t>(pbuffer[pos])) << std::dec;
  }
  std::cout << "], " << buffer_length << ", bytes length\n";

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

static void __attribute__ ((constructor)) __on_load()
{
  setlocale(LC_CTYPE, "C.UTF-8");
  PYHROL_REGISTER_FUNCTION(function_manipulate_strings_simple, "Simple string arguments")
  PYHROL_REGISTER_FUNCTION(function_manipulate_string_objects, "Accessing string through objects")
  PYHROL_REGISTER_FUNCTION(function_string_codepage_conversion, "Change string codepage on the fly")
}
