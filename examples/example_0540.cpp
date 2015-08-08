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

class MyClass2: public MyClass
{
public:
  void reset(const char *msg)
  {
    m_msg = msg;
  }

  MyClass2(const char *msg)
    : MyClass(msg)
  {
  }
};

template <class T> class PyTypeCommon: public pyhrol::TypeWrapper<T>
{
  virtual void constructor(T &obj, pyhrol::Tuples &_args) const
  {
    const char *msg;
    PYHROL_PARSE_TUPLE_1(NULL, _args, msg)
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    new (&obj) T(msg);

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  virtual void destructor(T &obj) const
  {
    obj.~T();
  }

public:
  void say(const pyhrol::Ptr<T> &obj, pyhrol::Tuples &_args) const
  {
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    obj->say();

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }
};

class PyType: public PyTypeCommon<MyClass>
{
  PyType()
    : pyhrol::TypeBase<MyClass>("MyClass", "help")
  {
    m_add_method<PyTypeCommon<MyClass>, &PyType::say>("say", NULL);
  }

  static void init() __attribute__ ((constructor))
  {
    m_get(new PyType);
  }
};

class PyType2: public PyTypeCommon<MyClass2>
{
  PyType2()
    : pyhrol::TypeBase<MyClass2>("MyClass2", "help")
  {
    m_add_method<PyTypeCommon<MyClass2>, &PyType2::say>("say", NULL);
    m_add_method<PyType2, &PyType2::reset>("reset", NULL);
  }

  void reset(const pyhrol::Ptr<MyClass2> &obj, pyhrol::Tuples &_args) const
  {
    const char *msg;
    PYHROL_PARSE_TUPLE_1(NULL, _args, msg)
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    obj->reset(msg);

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  static void init() __attribute__ ((constructor))
  {
    m_get(new PyType2);
  }
};
