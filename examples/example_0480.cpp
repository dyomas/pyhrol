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
#include <tr1/memory>

class MyClass2: public MyClass
{
public:
  MyClass2(const MyClass2 &cp)
    : MyClass(cp.m_msg.c_str())
  {
    std::cout << __func__ << ": (copy)\n";
  }

  MyClass2(const char *msg)
    : MyClass(msg)
  {
    std::cout << __func__ << ": " << m_msg << '\n';
  }

  ~MyClass2()
  {
    std::cout << __func__ << ": " << m_msg << '\n';
  }
};


MyClass2 *from_shared(std::tr1::shared_ptr<MyClass2> *ptr)
{
  return ptr->get();
}


class PyType: public pyhrol::TypeSmart<std::tr1::shared_ptr<MyClass2>, MyClass2, from_shared>
{
  PyType()
    : pyhrol::TypeBase<std::tr1::shared_ptr<MyClass2> >("MyClass", "help")
  {
    pyhrol::TypeWrapper<std::tr1::shared_ptr<MyClass2> >::m_add_method<PyType, &PyType::cnt>("cnt", NULL);
    m_add_method<PyType, &PyType::say>("say", NULL);
  }

  virtual void constructor(std::tr1::shared_ptr<MyClass2> &obj, pyhrol::Tuples &_args) const
  {
    pyhrol::TypeBase<std::tr1::shared_ptr<MyClass2> >::argObject cp;
    const char *msg;
    PYHROL_PARSE_TUPLE_1(NULL, _args, msg)
    PYHROL_PARSE_TUPLE_2(NULL, _args, cp.cls, cp.pobj)
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    if (_args.parsed_variant() == 0)
    {
      new (&obj) std::tr1::shared_ptr<MyClass2> (new MyClass2(msg));
    }
    else if (_args.parsed_variant() == 1)
    {
      new (&obj) std::tr1::shared_ptr<MyClass2> (cp);
    }

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  virtual void destructor(std::tr1::shared_ptr<MyClass2> &obj) const
  {
    obj.~shared_ptr();
  }

  void cnt(const pyhrol::Ptr<const std::tr1::shared_ptr<MyClass2> > &obj, pyhrol::Tuples &_args) const
  {
    long res;
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_BUILD_VALUE_1(NULL, _args, res)
    PYHROL_AFTER_BUILD_VALUE(_args)

    res = obj->use_count();

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  void say(const pyhrol::Ptr<MyClass2> &obj, pyhrol::Tuples &_args) const
  {
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    obj->say();

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  static void init() __attribute__ ((constructor))
  {
    m_get(new PyType);
  }
};
