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

namespace disambiguate
{

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

} //disambiguate

using namespace disambiguate;

class PyType: public TypeSequence<MyClass2>, public TypeWrapper<MyClass2>
{
  PyType()
    : TypeBase<MyClass2>("MyClass", "help")
  {
    m_add_method<PyType, &PyType::say>("say", NULL);
  }

  void say(const Ptr<MyClass2> &obj, Tuples &_args) const
  {
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    obj->say();

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

  virtual void concat(const Ptr<MyClass2> &res, Tuples &_args) const
  {
    const char *arg;
    int64_t num;
    TypeBase<MyClass2>::argObject like_me;

    PYHROL_PARSE_TUPLE_1(NULL, _args, arg)
    PYHROL_PARSE_TUPLE_2(NULL, _args, like_me.cls, like_me.pobj)
    PYHROL_PARSE_TUPLE_1(NULL, _args, num)
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    ostringstream ostr;
    ostr << res->message();
    switch (_args.parsed_variant())
    {
      case 0:
        ostr << arg;
        break;
      case 1:
        ostr << like_me->message();
        break;
      case 2:
        ostr << num;
        break;
    }
    res->set_message(ostr.str());

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  virtual void concat(const Ptr<MyClass2> &dst, const Ptr<const MyClass2> &left, Tuples &_args) const
  {
    const char *arg;
    int64_t num;
    TypeBase<MyClass2>::argObject like_me;

    PYHROL_PARSE_TUPLE_1(NULL, _args, arg)
    PYHROL_PARSE_TUPLE_2(NULL, _args, like_me.cls, like_me.pobj)
    PYHROL_PARSE_TUPLE_1(NULL, _args, num)
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    ostringstream ostr;
    ostr << left->message();
    switch (_args.parsed_variant())
    {
      case 0:
        ostr << arg;
        break;
      case 1:
        ostr << like_me->message();
        break;
      case 2:
        ostr << num;
        break;
    }
    new(&*dst) MyClass2(ostr.str());

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  virtual void assign(const Ptr<MyClass2> &dst, Tuples &_args, Py_ssize_t begin, Py_ssize_t end) const
  {
    const char *arg;
    int64_t num;
    TypeBase<MyClass2>::argObject like_me;

    PYHROL_PARSE_TUPLE_1(NULL, _args, arg)
    PYHROL_PARSE_TUPLE_2(NULL, _args, like_me.cls, like_me.pobj)
    PYHROL_PARSE_TUPLE_1(NULL, _args, num)
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    check_range(dst->message().length(), begin, end);

    ostringstream ostr;
    switch (_args.parsed_variant())
    {
      case 0:
        ostr << arg;
        break;
      case 1:
        ostr << like_me->message();
        break;
      case 2:
        ostr << num;
        break;
    }

    string res = dst->message().substr(0, begin);
    res += ostr.str();
    res += dst->message().substr(end);
    dst->set_message(res);

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  virtual void contains(bool &res, const Ptr<const MyClass2> &src, Tuples &_args) const
  {
    const char *arg;
    int64_t num;
    TypeBase<MyClass2>::argObject like_me;

    PYHROL_PARSE_TUPLE_1(NULL, _args, arg)
    PYHROL_PARSE_TUPLE_2(NULL, _args, like_me.cls, like_me.pobj)
    PYHROL_PARSE_TUPLE_1(NULL, _args, num)
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    ostringstream ostr;
    switch (_args.parsed_variant())
    {
      case 0:
        ostr << arg;
        break;
      case 1:
        ostr << like_me->message();
        break;
      case 2:
        ostr << num;
        break;
    }

    res = src->message().find(ostr.str()) != string::npos;

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  static void init() __attribute__ ((constructor))
  {
    m_get(new PyType);
  }
};
