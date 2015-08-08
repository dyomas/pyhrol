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

#include <iostream>
#include <pyhrol.h>
#include <pyerrors.h>

struct input: public pyhrol::TupleIn::anything
{
  int n;
  virtual int m_converter(PyObject *args)
  {
    if (!PyString_Check(args))
    {
      throw std::runtime_error(std::string("Unexpected data type (") + args->ob_type->tp_name +")");
    }

    PyStringObject *s = reinterpret_cast<PyStringObject *>(args);
    if (!strcmp(s->ob_sval, "Zero"))
    {
      n = 0;
    }
    else if (!strcmp(s->ob_sval, "One"))
    {
      n = 1;
    }
    else if (!strcmp(s->ob_sval, "Two"))
    {
      n = 2;
    }
    else
    {
      throw std::runtime_error(std::string("Input value \"") + s->ob_sval +"\" invalid");
    }
    return 1;
  }
};

struct output: public pyhrol::TupleOut::anything
{
  int n;
  virtual PyObject *m_converter() const
  {
    if (n == 1)
    {
      return PyString_FromString("First");
    }
    else if (n == 2)
    {
      return PyString_FromString("Second");
    }
    else
    {
      throw std::runtime_error("Unexpected output value");
    }
  }
};

void function_with_convertor(pyhrol::Tuples &_args)
{
  input i;
  output o;

  PYHROL_PARSE_TUPLE_2("\npossible values: \"Zero, One, Two\"", _args, input::converter, i)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_2(NULL, _args, output::converter, &o)
  PYHROL_AFTER_BUILD_VALUE(_args)

  std::cout
    << __func__ << ": I am called\n"
    << "  My arg: " << i.n << '\n'
  ;

  o.n = i.n;

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

static void __attribute__ ((constructor)) __on_load()
{
  PYHROL_REGISTER_FUNCTION(function_with_convertor, "Function calls special function to convert self argument and return value")
}
