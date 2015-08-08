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

#include "myclass.h"

#include <pyhrol.h>
#include <stdio.h>
#include <stdarg.h>

class PyType: public pyhrol::TypeWrapper<MyClass>
{
  PyType()
    /* NOTE
      Call to `TypeBase(const char *, const char *)' intentionally omitted here to provoke `std::logic_error' as expected in example scenario. Instead, default `TypeBase' constructor called
      To fix error:
        cmake -D DISABLE_TYPEBASE_DEFAULT_CONSTRUCTOR:BOOL=ON .
        make example_0970
      To provoke abort(3) due to unhandled exception:
        cmake -D DISABLE_TYPEBASE_DEFAULT_CONSTRUCTOR:BOOL=OFF -D ENABLE_TYPEBASE_UNHANDLED_EXCEPTION:BOOL=ON .
        make example_0970
    */
#ifdef DISABLE_TYPEBASE_DEFAULT_CONSTRUCTOR
    : TypeBase("MyClass", NULL)
#endif //DISABLE_TYPEBASE_DEFAULT_CONSTRUCTOR
  {
  }

  virtual void destructor(MyClass &obj) const
  {
    obj.~MyClass();
  }

public:
  static void __attribute__ ((constructor)) init()
  {
#ifdef ENABLE_TYPEBASE_UNHANDLED_EXCEPTION
      m_get(new PyType);
#else //ENABLE_TYPEBASE_UNHANDLED_EXCEPTION
    try
    {
      m_get(new PyType);
    }
    catch (std::exception &ex)
    {
      fprintf(stderr, "*** Exception! %s\n    Type `PyType' not inited\n", ex.what());
    }
#endif //ENABLE_TYPEBASE_UNHANDLED_EXCEPTION
  }
};
