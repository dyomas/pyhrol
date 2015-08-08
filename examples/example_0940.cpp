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

void lambda(pyhrol::Tuples &_args)
{
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void _function(pyhrol::Tuples &_args)
{
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

class PyType: public pyhrol::TypeWrapper<std::string>
{
  PyType()
    : pyhrol::TypeBase<std::string>("MyClass", NULL)
  {
    m_add_method<PyType, &PyType::method>("_method", NULL);
    m_type_object.tp_repr = NULL;
    m_type_object.tp_str = NULL;
  }

  virtual void constructor(std::string &obj, pyhrol::Tuples &_args) const
  {
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    new (&obj) std::string;

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  virtual void destructor(std::string &) const
  {
  }

  void method(const pyhrol::Ptr<const std::string> &, pyhrol::Tuples &_args) const
  {
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

public:
  static void init()
  {
    m_get(new PyType);
  }
};

static void __attribute__ ((constructor)) __on_load()
{
  PYHROL_REGISTER_FUNCTION(lambda, NULL)
  PYHROL_REGISTER_FUNCTION(_function, NULL);

  PyType::init();
}
