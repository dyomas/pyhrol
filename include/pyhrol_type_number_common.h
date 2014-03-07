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

// $Date: 2013-12-10 16:05:29 +0400 (Tue, 10 Dec 2013) $
// $Revision: 862 $

#ifndef __pyhrol_type_number_common_h__
#define __pyhrol_type_number_common_h__

#include "pyhrol_type_base.h"
#include "pyhrol_ptr.h"
#include "pyhrol_exception_handler.h"

namespace pyhrol
{

enum notImplementedBehavior
{
    returnNotImplemented
  , raiseError
};

template <class T> class TypeNumberCommon: virtual public TypeBase<T>
{
public:
  TypeNumberCommon();

protected:
  PyNumberMethods m_methods;

  // x.__neg__() <==> -x
  virtual void negative(const Ptr<T> &, const Ptr<const T> &) const;
  // x.__pos__() <==> +x
  virtual void positive(const Ptr<T> &, const Ptr<const T> &) const;
  // x.__abs__() <==> abs(x)
  virtual void absolute(const Ptr<T> &, const Ptr<const T> &) const;
  // x.__nonzero__() <==> x != 0
  virtual bool nonzero(const Ptr<const T> &) const;
  // x.__invert__() <==> ~x
  virtual void invert(const Ptr<T> &, const Ptr<const T> &) const;
  // x.__int__() <==> int(x)
  // x.__long__() <==> long(x)
  virtual long to_long(const Ptr<const T> &) const;
  // x.__float__() <==> float(x)
  virtual double to_double(const Ptr<const T> &) const;
  // x.__oct__() <==> oct(x)
  virtual const std::string to_oct(const Ptr<const T> &) const;
  // x.__hex__() <==> hex(x)
  virtual const std::string to_hex(const Ptr<const T> &) const;
  // x[y:z] <==> x[y.__index__():z.__index__()]
  virtual void index(const Ptr<T> &, const Ptr<const T> &) const;

private:
  template <void (TypeNumberCommon<T>::*M)(const Ptr<T> &, const Ptr<const T> &) const> struct call_wrapper: exceptionHandler
  {
    const notImplementedBehavior behavior;
    PyObject *self;
    const char * const signature;
    PyObject *retval;
    virtual void call();
    call_wrapper(PyObject *, const char *_context, const notImplementedBehavior = returnNotImplemented);
  };

  struct converter
  {
    virtual PyObject *convert(const Ptr<const T> &) const = 0;
    virtual ~converter();
  };

  struct to_long_converter: converter
  {
    virtual PyObject *convert(const Ptr<const T> &) const;
  };

  struct to_double_converter: converter
  {
    virtual PyObject *convert(const Ptr<const T> &) const;
  };

  struct to_oct_converter: converter
  {
    virtual PyObject *convert(const Ptr<const T> &) const;
  };

  struct to_hex_converter: converter
  {
    virtual PyObject *convert(const Ptr<const T> &) const;
  };

  struct call_converter: exceptionHandler
  {
    const converter &c;
    PyObject *self;
    const char * const signature;
    PyObject *retval;
    virtual void call();
    call_converter(const converter &c, PyObject *, const char *);
  };

  struct call_nonzero: exceptionHandler
  {
    PyObject *self;
    const char * const signature;
    int retval;
    virtual void call();
    call_nonzero(PyObject *, const char *);
  };

  void m_init();

  static PyObject *mediator_nb_negative(PyObject *);
  static PyObject *mediator_nb_positive(PyObject *);
  static PyObject *mediator_nb_absolute(PyObject *);
  static int mediator_nb_nonzero(PyObject *);
  static PyObject *mediator_nb_invert(PyObject *);
  static PyObject *mediator_nb_int(PyObject *);
  static PyObject *mediator_nb_long(PyObject *);
  static PyObject *mediator_nb_float(PyObject *);
  static PyObject *mediator_nb_oct(PyObject *);
  static PyObject *mediator_nb_hex(PyObject *);
  static PyObject *mediator_nb_index(PyObject *);
};

}; //namespace pyhrol

#endif //__pyhrol_type_number_common_h__
