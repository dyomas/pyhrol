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

  virtual Py_ssize_t length(const Ptr<const MyClass2> &arg) const
  {
    return arg->message().length();
  }

  virtual void concat(const Ptr<MyClass2> &res, const Ptr<const MyClass2> &add) const
  {
    const string s = res->message();
    res->set_message(s + add->message());
  }

  virtual void concat(const Ptr<MyClass2> &dst, const Ptr<const MyClass2> &left, const Ptr<const MyClass2> &right) const
  {
    new(&*dst) MyClass2(left->message() + right->message());
  }

  virtual void repeat(const Ptr<MyClass2> &dst, Py_ssize_t cnt) const
  {
    string res;
    while (cnt --)
    {
      res += dst->message();
    }
    dst->set_message(res);
  }

  virtual void get(const Ptr<MyClass2> &dst, const Ptr<const MyClass2> &src, Py_ssize_t begin, Py_ssize_t end) const
  {
    check_range(src->message().length(), begin, end);
    new(&*dst) MyClass2(src->message().substr(begin, end - begin));
  }

  virtual void assign(const Ptr<MyClass2> &dst, const Ptr<const MyClass2> &arg, Py_ssize_t begin, Py_ssize_t end) const
  {
    check_range(dst->message().length(), begin, end);

    string res = dst->message().substr(0, begin);
    res += arg->message();
    res += dst->message().substr(end);
    dst->set_message(res);
  }

  virtual void del(const Ptr<MyClass2> &dst, Py_ssize_t begin, Py_ssize_t end) const
  {
    check_range(dst->message().length(), begin, end);

    string res = dst->message().substr(0, begin);
    res += dst->message().substr(end);
    dst->set_message(res);
  }

  virtual bool contains(const Ptr<const MyClass2> &src, const Ptr<const MyClass2> &key) const
  {
    return src->message().find(key->message()) != string::npos;
  }

  static void init() __attribute__ ((constructor))
  {
    m_get(new PyType);
  }
};
