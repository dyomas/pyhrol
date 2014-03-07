/*
 *   Copyright (c) 2013 Pyhrol, pyhrol@rambler.ru
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
  const string &message() const
  {
    return m_msg;
  }

  void set_message(const string &msg)
  {
    m_msg = msg;
  }

  MyClass2(const string &msg)
    : MyClass(msg.c_str())
  {
  }
};

class PyTypeAny: public TypeNumberAny<MyClass2>, public TypeWrapper<MyClass2>
{
  PyTypeAny()
    : TypeBase<MyClass2>("MyClass", NULL)
  {
    m_add_method<PyTypeAny, &PyTypeAny::say>("say", "Returns previously assigned message");
  }

  void say(const Ptr<MyClass2> &obj, Tuples &_args) const
  {
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    obj->say();

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  virtual void add(const Ptr<MyClass2> &res, Tuples &_args) const
  {
    int i;
    const char *str;
    TypeBase<MyClass2>::argObject as_me, as_me2;

    PYHROL_PARSE_TUPLE_3("MyClass + int\nself\nint", _args, as_me.cls, as_me.pobj, i)
    PYHROL_PARSE_TUPLE_3("int + MyClass\nint\nself", _args, i, as_me.cls, as_me.pobj)
    PYHROL_PARSE_TUPLE_3("MyClass + str\nself\nstr", _args, as_me.cls, as_me.pobj, str)
    PYHROL_PARSE_TUPLE_3("str + MyClass\nstr\nself", _args, str, as_me.cls, as_me.pobj)
    PYHROL_PARSE_TUPLE_4("MyClass + MyClass\n_1\n_2", _args, as_me.cls, as_me.pobj, as_me2.cls, as_me2.pobj)
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    ostringstream ostr;
    switch (_args.parsed_variant())
    {
      case 0:
         ostr << as_me->message() << i;
        break;
     case 1:
        ostr << i << as_me->message();
        break;
      case 2:
        ostr << as_me->message() << str;
        break;
      case 3:
        ostr << str << as_me->message();
        break;
      case 4:
        ostr << as_me->message() << as_me2->message();
        break;
    }
    new (&*res) MyClass2(ostr.str());

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  virtual void inplace_add(const Ptr<MyClass2> &obj, Tuples &_args) const
  {
    int i;
    const char *str;
    TypeBase<MyClass2>::argObject as_me;

    PYHROL_PARSE_TUPLE_1("MyClass += int\nint", _args, i)
    PYHROL_PARSE_TUPLE_1("MyClass += str\nstr", _args, str)
    PYHROL_PARSE_TUPLE_2("MyClass += MyClass\nMyClass", _args, as_me.cls, as_me.pobj)
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    ostringstream ostr;
    ostr << obj->message();

    switch (_args.parsed_variant())
    {
      case 0:
        ostr << i;
        break;
      case 1:
        ostr << str;
        break;
      case 2:
        ostr << as_me->message();
        break;
    }
    obj->set_message(ostr.str());

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
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
    m_get(new PyTypeAny);
  }
};
