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
  MyClass2(const char *msg)
    : MyClass(msg)
  {
    if (!*msg)
    {
      throw std::logic_error("Message can not be empty");
    }
  }
};

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


void new_myclass(pyhrol::Tuples &_args)
{
  const char *msg;
  PyObject *retval;

  PYHROL_PARSE_TUPLE_1(NULL, _args, msg)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  PyType::T_struct *pobj = PyType::allocate_static();
  try
  {
    new (&pobj->endosome) MyClass2(msg);
    retval = *pobj;
  }
  catch (...)
  {
    PyType::free_static(pobj);
    throw;
  }

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void new_myclass2(pyhrol::Tuples &_args)
{
  const char *msg;
  PyObject *retval;

  PYHROL_PARSE_TUPLE_1(NULL, _args, msg)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  PyType &type = PyType::get();
  PyType::T_struct *pobj = type.allocate();
  try
  {
    new (&pobj->endosome) PyType::endosome_t(msg);
    retval = *pobj;
  }
  catch (...)
  {
    type.free(pobj);
    throw;
  }

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

static void __attribute__ ((constructor)) __on_load()
{
  PyType::init();
  PYHROL_REGISTER_FUNCTION(new_myclass, NULL)
  PYHROL_REGISTER_FUNCTION(new_myclass2, NULL)
}

