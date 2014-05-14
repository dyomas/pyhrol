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

using namespace std;
using namespace pyhrol;

struct point
{
  double X, Y;
};

class PyType: public TypeWrapper<point>
{
  PyType()
    : TypeBase<point>("point", NULL)
  {
    PYHROL_REGISTER_GETTER(PyType, ro, NULL)
    PYHROL_REGISTER_SETTER(PyType, wo, NULL)
    PYHROL_REGISTER_GETSETER(PyType, "rw", ro, wo, NULL)
  }

  virtual void constructor(point &obj, Tuples &_args) const
  {
    double _X, _Y;
    PYHROL_PARSE_TUPLE_2(NULL, _args, _X, _Y)
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    new (&obj) point;
    obj.X = _X;
    obj.Y = _Y;

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  virtual void destructor(point &obj) const
  {
    obj.~point();
  }

  void ro(const Ptr<const point> &obj, Tuples &_args) const
  {
    uint8_t build_variant;
    double _X, _Y;
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_BUILD_VALUE_1(NULL, _args, _X)
    PYHROL_BUILD_VALUE_2(NULL, _args, _X, _Y)
    PYHROL_AFTER_BUILD_VALUE(_args)

    _X = obj->X;
    _Y = obj->Y;
    build_variant = obj->X == obj->Y ? 0 : 1;

    PYHROL_AFTER_EXECUTE(_args, build_variant)
  }

  void wo(const Ptr<point> &obj, Tuples &_args) const
  {
    double _X, _Y;
    Py_complex c;
    PYHROL_PARSE_TUPLE_1(NULL, _args, _X)
    _args.set_options(optTuple);
    PYHROL_PARSE_TUPLE_2(NULL, _args, _X, _Y)
    PYHROL_PARSE_TUPLE_1(NULL, _args, c)
    PYHROL_AFTER_PARSE_TUPLE(_args)
    PYHROL_AFTER_BUILD_VALUE(_args)

    switch (_args.parsed_variant())
    {
      case 0:
        obj->X = _X;
        obj->Y = _X;
        break;
      case 1:
        obj->X = _X;
        obj->Y = _Y;
        break;
      case 2:
        obj->X = c.real;
        obj->Y = c.imag;
        break;
    }

    PYHROL_AFTER_EXECUTE_DEFAULT(_args)
  }

  static void init() __attribute__ ((constructor))
  {
    m_get(new PyType);
  }

};
