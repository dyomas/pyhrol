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
  MyClass2(const MyClass2 &cp);

public:
  const std::string &get() const
  {
    return m_msg;
  }

  MyClass2(const std::string &msg)
    : MyClass(msg.c_str())
  {
  }
};

namespace pyhrol
{
/* NOTE
To provoke compilation error caused by private copy constructor:
  cmake -D DISABLE_EXPLICIT_TEMPLATE_INSTANTIATION:BOOL=ON .
  make example_0960
*/

#ifndef DISABLE_EXPLICIT_TEMPLATE_INSTANTIATION
template <> PyObject *TypeWrapper<MyClass2>::convert(const MyClass2 &cp)
{
  PyObject *ret_val = NULL;

  typename TypeBase<MyClass2>::T_struct *pstruct = NULL;
  try
  {
    pstruct = TypeBase<MyClass2>::allocate();
    new (&pstruct->endosome) MyClass2(cp.get());
    ret_val = *pstruct;
  }
  catch (...)
  {
    TypeBase<MyClass2>::free(pstruct);
    throw;
  }

  return ret_val;
}
#endif //DISABLE_EXPLICIT_TEMPLATE_INSTANTIATION

}


class PyType: public pyhrol::TypeWrapper<MyClass2>
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

  virtual void constructor(MyClass2 &obj, pyhrol::Tuples &_args) const
  {
    const char *msg;
    PYHROL_PARSE_TUPLE_1(NULL, _args, msg)
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    new (&obj) MyClass(msg);

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  virtual void destructor(MyClass2 &obj) const
  {
    obj.~MyClass2();
  }

public:
  static void init()
  {
    m_get(new PyType);
  }

  static PyType &get()
  {
    return dynamic_cast<PyType &>(m_get());
  }
};

void copy_myclass(pyhrol::Tuples &_args)
{
  PyType::argObject cp;
  PyObject *res;

  PYHROL_PARSE_TUPLE_2(NULL, _args, cp.cls, cp.pobj)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, res)
  PYHROL_AFTER_BUILD_VALUE(_args)

  res = PyType::get().convert(*cp);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

static void __attribute__ ((constructor)) __on_load()
{
  PyType::init();
  PYHROL_REGISTER_FUNCTION(copy_myclass, NULL)
}
