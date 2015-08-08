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
#include <algorithm>
#include <pyhrol.h>

class MyClass2: public MyClass
{
public:
  const std::string &message() const
  {
    return m_msg;
  }

  void set_message(const std::string &msg)
  {
    m_msg = msg;
  }

  MyClass2(const std::string &msg)
    : MyClass(msg.c_str())
  {
  }
};


class PyType: public pyhrol::TypeNumber<MyClass2>, public pyhrol::TypeWrapper<MyClass2>
{
  PyType()
    : pyhrol::TypeBase<MyClass2>("MyClass", "help")
  {
    m_add_method<PyType, &PyType::say>("say", NULL);
  }

  void say(const pyhrol::Ptr<MyClass2> &obj, pyhrol::Tuples &_args) const
  {
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    obj->say();

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  virtual void add(const pyhrol::Ptr<MyClass2> &res, const pyhrol::Ptr<const MyClass2> &left, const pyhrol::Ptr<const MyClass2> &right) const
  {
    new (&*res) MyClass2(left->message() + right->message());
  }

  virtual void inplace_add(const pyhrol::Ptr<MyClass2> &obj, const pyhrol::Ptr<const MyClass2> &arg) const
  {
    const std::string s = obj->message();
    obj->set_message(s + arg->message());
  }

  virtual void invert(const pyhrol::Ptr<MyClass2> &obj, const pyhrol::Ptr<const MyClass2> &arg) const
  {
    std::string res(arg->message().length(), '\0');
    std::reverse_copy(arg->message().begin(), arg->message().end(), res.begin());
    new (&*obj) MyClass2(res);
  }

  virtual void constructor(MyClass2 &obj, pyhrol::Tuples &_args) const
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
