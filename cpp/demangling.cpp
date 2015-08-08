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

// $Date: 2012-11-14 03:27:20 +0400 (Ср., 14 нояб. 2012) $
// $Revision: 674 $

#include <cxxabi.h>
#include "demangling.h"

using namespace std;

const string demangle(const type_info &ti)
{
  const size_t __mangle_buff_size = 1024;
  int status;
  size_t __mangle_buff_len = __mangle_buff_size;
  char __mangle_buff[__mangle_buff_size];
  /*
  Назначение третьего (__length) и четвертого (__status) аргументов неясно: в случае очевидной ошибки нехватки памяти происходит крушение процесса (Segmentation fault или Bus error). Ситуация, при которой хотя бы один из аргументов был изменен не наблюдалась
  */
  const char *mangled_name = abi::__cxa_demangle(ti.name(), __mangle_buff, &__mangle_buff_len, &status);

  return (mangled_name ? mangled_name : "<not_demangled>");
}

ostream &operator << (ostream &os, const type_info &ti)
{
  const size_t __mangle_buff_size = 1024;
  int status;
  size_t __mangle_buff_len = __mangle_buff_size;
  char __mangle_buff[__mangle_buff_size];
  /*
  Назначение третьего (__length) и четвертого (__status) аргументов неясно: в случае очевидной ошибки нехватки памяти происходит крушение процесса (Segmentation fault или Bus error). Ситуация, при которой хотя бы один из аргументов был изменен не наблюдалась
  */
  const char *mangled_name = abi::__cxa_demangle(ti.name(), __mangle_buff, &__mangle_buff_len, &status);

  os << (mangled_name ? mangled_name : "<not_demangled>");
#ifdef _DEBUG
  if (status || __mangle_buff_len != __mangle_buff_size || !mangled_name)
  {
    cerr
      << "status = " << status << endl
      << "mangled_name = \"" << (mangled_name ? mangled_name : "(null)") << "\"" << endl
      << "__mangle_buff_size = " << __mangle_buff_size << endl
      << "__mangle_buff_len = " << __mangle_buff_len << endl
    ;
  }
#endif //_DEBUG
  return os;
}
