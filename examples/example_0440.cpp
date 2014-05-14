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

#include <pyhrol.h>
#include "myclass.h"

using namespace std;
using namespace pyhrol;

class MyClass2: public MyClass
{
public:
  double weight;
  const char *say_spanish;
  char say_russian[29];

  MyClass2(const char *msg)
    : MyClass(msg)
    , weight(0)
    , say_spanish("¡Hola, mundo!")
  {
    memcpy(say_russian, "Здравствуй мир!", 29);
  }
};

class PyType: public TypeWrapper<MyClass2>
{
  PyType()
    : TypeBase<MyClass2>("MyClass", "help")
  {
    m_add_member<double, &MyClass2::weight>("weight", NULL);
    m_add_member<const char *, &MyClass2::say_spanish>("say_spanish", NULL);
    m_add_member<char [29], &MyClass2::say_russian>("say_russian", NULL);
  }

  virtual void constructor(MyClass2 &obj, Tuples &_args) const
  {
    const char *msg;
    PYHROL_PARSE_TUPLE_1(NULL, _args, msg)
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    new (&obj) MyClass2(msg);

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  virtual void destructor(MyClass2 &obj) const
  {
    obj.~MyClass2();
  }

  static void init() __attribute__ ((constructor))
  {
    m_get(new PyType);
  }

};
